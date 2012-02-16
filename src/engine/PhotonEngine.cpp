/*
 * PhotonEngine.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: nh
 */

#include "PhotonEngine.h"

PhotonEngine::PhotonEngine(int _width, int _height, double _photonHitRadius, int _iteration) :
    Engine(_width, _height),
    photonMap(new SpatialHash(_photonHitRadius)),
    shadowMap(new SpatialHash(_photonHitRadius)),
    specMap(new SpatialHash(_photonHitRadius)),
    iteration(_iteration) {
  createPhotonMap();
}

PhotonEngine::~PhotonEngine() {
  delete photonMap;
  delete shadowMap;
  delete specMap;
}

void PhotonEngine::createPhotonMap() {
  int hits = 0;
  int tries = 0;
  int offset = rand();
  int photons = Settings::instance()->photons;
  while (hits < photons) {
    for (unsigned int i = 0; i < scene.lights.size(); i++) {
      if (hits >= photons) break;
      Primitive& light = *(scene.lights[i]);
      Sphere& sphere = (Sphere&) light;
      double distance = 1000000;
      Vector3d direction = getHaltonPointOnSphere(offset+tries, 1);
      Vector3d position = sphere.center + (direction * sphere.radius);
      Ray ray(position + direction*epsilon, direction);
      int primId = -1;

      if (raytrace(ray, primId, distance) == Primitive::HIT) {
        Primitive& newPrim = *(scene.primitives[primId]);
        Vector3d intersectionPoint = ray.origin + ray.direction * distance;
        Color3f color = newPrim.material.color * light.material.color;
        russianRoulette(new common::PhotonHit(intersectionPoint, direction, color, light, newPrim, 0), 0);
        hits++;
      }
      tries++;
    }
  }
  photonMap->fluxWeight = 1.0 / tries;
  shadowMap->fluxWeight = 1.0 / tries;
  specMap->fluxWeight = 1.0 / tries;
  photonMap->initDraw();
  shadowMap->initDraw();
  specMap->initDraw();
  std::cout << "Created the photonMap, tries: " << tries << ", hits: " << hits << "\n";
}

void PhotonEngine::russianRoulette(common::PhotonHit* photonHit, int iteration) {
  float diffusion = photonHit->primitive.material.diffusion;
  float reflection = photonHit->primitive.material.reflection;
  float refraction = photonHit->primitive.material.refraction;
  float total = diffusion + reflection + refraction;

  if (total < 1.0) total = 1.0;

//  if (photonHit->specular) {
//    std::cout << "diffusion: " << diffusion << ", reflection: " << reflection << ", refraction: " << refraction << ", total: " << total << "\n";
//  }

  float randNum = (float) rand() / RAND_MAX;
  if (randNum < diffusion / total) {
//    if (photonHit->specular) {
//      Color3f c = photonHit->color;
//      std::cout << "adding specular photon, iteration: " << iteration << "\n";
//      std::cout << "color: " << c.r << ", " << c.g << ", " << c.b << "\n";
//      photonHit->color = Color3f(5000, 5000, 5000);
//      shadowMap->add(photonHit);
//    } else {
//      photonHit->color = Color3f(0, 0, 0);
//    }

    if (iteration == 0) {
      photonMap->add(photonHit);
    } else if (photonHit->specular == 0){
      shadowMap->add(photonHit);
    } else {
      specMap->add(photonHit);
    }

    createShadowPhoton(photonHit, iteration);
  } else if (randNum < (diffusion + reflection) / total) {
    reflectPhoton(photonHit, iteration);
//    std::cout << "reflected photon at depth: " << iteration << "\n";
  } else if (randNum < (diffusion + reflection + refraction) / total) {
//    std::cout << "refracted photon at depth: " << iteration << "\n";
    refractPhoton(photonHit, iteration);
  }
}

void PhotonEngine::createShadowPhoton(common::PhotonHit* photonHit, int iteration) {
  // short circuit if we've reached the max iterations;
  if (iteration > 3) return;
  double distance = 1000000;
  Vector3d primNormal = photonHit->primitive.getNormal(photonHit->location);
  Vector3d zAxis(0, 0, 1);
  Vector3d direction = getDiffuseVector(primNormal);
//  while (direction.dot(primNormal) < 0) {
//    std::cout << "primNormal: " << primNormal.x() << ", " << primNormal.y() << ", " << primNormal.z();
//    std::cout << " direction: " << direction.x() << ", " << direction.y() << ", " << direction.z() << "\n";
//    direction = getRandomPointOnSphere(1);
//  }
//  direction = rotateVector(direction, zAxis, primNormal);

  if (direction.dot(primNormal) < 0) {
    std::cout << "uh oh, the primNormal and random ray bounce are going in opposite directions.\n";
  }
//  std::cout << "primNormal: " << primNormal.x() << ", " << primNormal.y() << ", " << primNormal.z();
//  std::cout << " direction: " << direction.x() << ", " << direction.y() << ", " << direction.z() << "\n";

  Ray ray(photonHit->location + direction*epsilon, direction);
  int primId = -1;

  if (raytrace(ray, primId, distance) == Primitive::HIT) {
    Primitive& newPrim = *(scene.primitives[primId]);
    Vector3d newIntersectionPoint = ray.origin + ray.direction * distance;
    Color3f newColor = photonHit->color * newPrim.material.color;
    russianRoulette(new common::PhotonHit(newIntersectionPoint, direction, newColor, photonHit->primitive, newPrim, photonHit->specular), iteration + 1);
  }
}

void PhotonEngine::reflectPhoton(common::PhotonHit* photonHit, int iteration) {
  // short circuit if we've reached the max iterations;
  if (iteration > 3) return;
  Vector3d N = photonHit->primitive.getNormal(photonHit->location);
  Vector3d R = photonHit->direction - 2 * photonHit->direction.dot(N) * N;
  Ray ray = Ray(photonHit->location + R * epsilon, R);
  int primId = -1;
  double distance = 1000000;

  if (raytrace(ray, primId, distance) == Primitive::HIT) {
    Primitive& newPrim = *(scene.primitives[primId]);
    Vector3d newIntersectionPoint = ray.origin + ray.direction * distance;
    Color3f newColor = photonHit->color * newPrim.material.color;
    russianRoulette(new common::PhotonHit(newIntersectionPoint, R, newColor, photonHit->primitive, newPrim, 1), iteration + 1);
  }
}

void PhotonEngine::refractPhoton(common::PhotonHit* photonHit, int iteration) {
  // short circuit if we've reached the max iterations;
  if (iteration > 3) return;

  double rIndex = photonHit->primitive.material.rIndex;
  double n = photonHit->oldPrimitive.material.rIndex / rIndex;
//  double n = rIndex / photonHit->oldPrimitive.material.rIndex;
  Vector3d N = photonHit->primitive.getNormal(photonHit->location);
  if (N.dot(photonHit->direction) > 0) {
    N *= -1.0;
//    n = 1 / rIndex;
  }
//  std::cout << "n: " << n << "rIndexPrev: " << photonHit->oldPrimitive.material.rIndex << " rIndex: " << rIndex << "\n";

  double cosI = -1 * N.dot(photonHit->direction);
  double cosT2 = 1.0 - n * n * (1.0 - cosI * cosI);
  if (cosT2 <= 0.0) return;
//  std::cout << "in refractPhoton, got past costT2 check\n";
  Vector3d T = n * photonHit->direction + (n * cosI - sqrt(cosT2)) * N;

  Vector3d temp = photonHit->direction;
//  std::cout << "old: " << temp.x() << ", " << temp.y() << ", " << temp.z() << ", ";
//  std::cout << "new: " << T.x() << ", " << T.y() << ", " << T.z() << "\n";


  Color3f rColor(0, 0, 0);
  Ray ray = Ray(photonHit->location + T * epsilon, T);
  int primId = -1;
  double distance = 1000000;

  if (raytrace(ray, primId, distance) != Primitive::MISS) {
    Primitive& newPrim = *(scene.primitives[primId]);
    Vector3d newIntersectionPoint = ray.origin + ray.direction * distance;
    Color3f newColor = photonHit->color * newPrim.material.color;
//    std::cout << "in refractPhoton, calling russianRoulette\n";
    russianRoulette(new common::PhotonHit(newIntersectionPoint, T, newColor, photonHit->primitive, newPrim, 1), iteration + 1);
  }
}

Vector3d PhotonEngine::rotateVector(Vector3d vector, Vector3d normal1, Vector3d normal2) {
  Quaterniond q;
  q.setFromTwoVectors(normal1, normal2);
  Vector3d newVector = q * vector;
  return newVector;
}

Vector3d PhotonEngine::getRandomPointOnSphere(double radius) {
  double mult = 2 / (double) RAND_MAX;
  while (true) {
    Vector3d vector(1 - mult*rand(), 1 - mult*rand(), 1 - mult*rand());
    if (vector.norm() <= 1) {
      return vector*radius;
    }
  }
}

Vector3d PhotonEngine::getDiffuseVector(Vector3d normal) {
  normal.normalize();
  double mult = 2 / (double) RAND_MAX;
  while (true) {
    Vector3d vector(1 - mult*rand(), 1 - mult*rand(), 1 - mult*rand());
    if (vector.norm() <= 1 && vector.dot(normal) > 0) {
      return vector;
    }
  }
}

/*
Vector3d PhotonEngine::getRandomPointOnSphere(double radius) {
  double mult = 2.0 / RAND_MAX;
  double theta = rand()*mult*pi; // 0 to 2pi
  double z = rand()*mult - 1;  // -1 to 1
  return getPointOnSphere(radius, theta, z);
}
*/

Vector3d PhotonEngine::getHaltonPointOnSphere(int id, double radius) {
  double theta = 2.0 * pi * halton(id, 2);
  double z = 2.0 * halton(id, 3) - 1;
  return getPointOnSphere(radius, theta, z);
}
/*
Vector3d PhotonEngine::getRandomPointOnHalfSphere(double radius) {
  double mult = 2.0 / RAND_MAX;
  double theta = rand()*mult*pi;
  double z = rand() / (float) RAND_MAX;
  return getPointOnSphere(radius, theta, z);
}
*/
Vector3d PhotonEngine::getRandomPointOnHalfSphere(double radius) {
  while (true) {
    double x = radius * rand() / (double) RAND_MAX;
    double y = radius * rand() / (double) RAND_MAX;
    double z = radius * rand() / (double) RAND_MAX;

    if (y > 0 && sqrt(x*x+y*y+z*z) < radius) {
      return Vector3d(x, y, z);
    }
  }
}

Vector3d PhotonEngine::getHaltonPointOnHalfSphere(int id, double radius) {
  double theta = 2.0 * pi * halton(id, 2);
  double z = 1.0 * halton(id, 3);
  return getPointOnSphere(radius, theta, z);
}

Vector3d PhotonEngine::getPointOnSphere(double radius, double theta, double z) {
  double y = sqrt(1 - z*z) * sin(theta);
  double x = sqrt(1 - z*z) * cos(theta);
  Vector3d direction = Vector3d(x, y, z);
  direction.normalize();
  return direction;
}

bool PhotonEngine::finalGatherHelper(HitPoint* hit, Vector3d direction, Color3f& color) {

  double distance = 1000000;
  Vector3d position = hit->location;
  Vector3d normal = hit->primitive.getNormal(hit->location);
  Ray ray(position + direction*epsilon, direction);
  int primId = -1;
  int hitType = raytrace(ray, primId, distance);
  if (primId != -1) {
    Primitive& newPrim = *(scene.primitives[primId]);
    Color3f tempColor(0, 0, 0);
    Vector3d intersectionPoint = ray.origin + ray.direction * distance;
    HitPoint* tempHit = new HitPoint(intersectionPoint, ray, hitType, newPrim, Color3f(1, 1, 1));
    photonMap->drawHit(tempHit, tempColor);
//    if (tempColor.r + tempColor.g + tempColor.b > 3.0) {
//      std::cout << "tempColor1: " << tempColor.r << ", " << tempColor.g << ", " << tempColor.b << "\n";
//      std::cout << "primId: " << primId << "\n";
//      std::cout << "hitType: " << hitType << "\n";
//    }
    shadowMap->drawHit(tempHit, tempColor);
    delete tempHit;
//    std::cout << "tempColor2: " << tempColor.r << ", " << tempColor.g << ", " << tempColor.b << "\n";
    color += hit->primitive.material.diffusion * hit->primitive.material.color * tempColor;
//    std::cout << "Color: " << color.r << ", " << color.g << ", " << color.b << "\n";
    return true;
  }
  return false;
}

float PhotonEngine::halton(int id, int prime) {
  float h = 0.0;
  float f = 1.0/prime;
  float fct = 1.0;
  while(id>0)
  {
    fct *= f;
    h += (id%prime)*fct;
    id /= prime;
  }
  return h;
}

Color3f PhotonEngine::finalGather(HitPoint* hit, int rand) {
  Color3f color(0, 0, 0);
  Vector3d position = hit->location;
  Vector3d normal = hit->primitive.getNormal(hit->location);
  int samples = Settings::instance()->samples;
  int hits = 0;
  int offset = rand + iteration*samples;
  while (hits < samples) {
//    Vector3d direction = getDiffuseVector(normal);
    Vector3d direction = getHaltonPointOnHalfSphere(hits+offset, 1);
//    Vector3d direction = getRandomPointOnHalfSphere(1);
    direction = rotateVector(direction, Vector3d(0, 0, 1), normal);
    finalGatherHelper(hit, direction, color);
//    else {
//      std::cout << "location:  x: " << hit.location.x() << " y: " << hit.location.y() << " z: " << hit.location.z() << "\n";
//      std::cout << "normal:    x: " << normal.x() << " y: " << normal.y() << " z: " << normal.z() << "\n";
//      std::cout << "direction: x: " << direction.x() << " y: " << direction.y() << " z: " << direction.z() << "\n";
//    }
    hits++;
  }
//  if (tries > 2.0 * hits) {
//    std::cout << "samples: " << samples << ", tries: " << tries << ", hits: " << hits << "\n";
//    std::cout << "normal: x: " << normal.x() << " y: " << normal.y() << " z: " << normal.z() << "\n";
//  }
  color = (1.0/samples) * color;
//  std::cout << "Final Color: " << color.r << ", " << color.g << ", " << color.b << "\n";
  return color * hit->primitive.material.diffusion;
}

Color3f PhotonEngine::renderPixel(int x, int y) {
//  std::cout << "PhotonEngine - Rendering Pixel x: " << x << ", y: " << y << "\n";
  Color3f color(0, 0, 0);
//  boost::optional<HitPoint>& hitPoint = hitPoints[y * width + x];

  // Short Circuit if nothing is there.
//  if (!hitPoint) return color;
//  photonMap->drawHit(hitPoint.get(), color);
  // If set, return finalGather rather than direct visualization of the photonMap.

//  for (unsigned int i = 0; i < hitPoints[x][y].size(); ++i) {
  int width = Settings::instance()->width;
  int size = hitPoints[x][y].size();
  for (unsigned int i = 0; i < size; ++i) {
    HitPoint* hitPoint = hitPoints[x][y][i];
    Color3f hitColor(0, 0, 0);
    if (Settings::instance()->finalGather) {
      int rand = RAND_MAX * halton(RAND_MAX * halton(x+y*width, 5), 7);
//      int rand = 0;
//      std::cout << "rand: " << rand << "\n";
      hitColor += finalGather(hitPoint, rand);
    }
    else {
//      photonMap->drawHit(hitPoint, hitColor);
      shadowMap->drawHit(hitPoint, hitColor);
    }
    specMap->drawHit(hitPoint, hitColor);
    color += hitColor * hitPoint->contribution;
//    std::cout << "contribution: " << hitPoint->contribution.r << ", " << hitPoint->contribution.g << ", " << hitPoint->contribution.b << "\n";
  }
  return color;
}
