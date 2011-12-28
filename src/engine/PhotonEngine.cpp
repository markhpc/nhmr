/*
 * PhotonEngine.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: nh
 */

#include "PhotonEngine.h"

PhotonEngine::PhotonEngine(int _width, int _height, double _photonHitRadius) :
    Engine(_width, _height),
    photonMap(new SpatialHash(_photonHitRadius)),
    shadowMap(new SpatialHash(_photonHitRadius)) {
  createPhotonMap();
}

PhotonEngine::~PhotonEngine() {
  delete photonMap;
  delete shadowMap;
}

void PhotonEngine::createPhotonMap() {
  int hits = 0;
  int tries = 0;
  int offset = rand();
  for (unsigned int i = 0; i < scene.lights.size(); i++) {
    Primitive& light = scene.lights[i];
    Sphere& sphere = (Sphere&) light;
    int photons = Settings::instance()->photons;
    while (hits < photons) {
      double distance = 1000000;
      Vector3d direction = getHaltonPointOnSphere(offset+tries, 1);
      Vector3d position = sphere.center + (direction * sphere.radius);
      Ray ray(position + direction*epsilon, direction);

      boost::optional<Primitive&> primitive = raytrace(ray, 1, distance);
      if (primitive) {
        Vector3d intersectionPoint = ray.origin + ray.direction * distance;
        Color3f color = primitive.get().material.color * light.material.color;
        photonMap->add(new common::PhotonHit(intersectionPoint, color, primitive.get()));
        createShadowPhoton(primitive.get(), intersectionPoint, color, 0);
        hits++;
      }
      tries++;
    }
  }
  photonMap->fluxWeight = 1.0 / tries;
  shadowMap->fluxWeight = 1.0 / tries;
  photonMap->initDraw();
  shadowMap->initDraw();
}

bool PhotonEngine::photonTrace(Primitive& light, int i, int iMax, int j, int jMax) {
  Sphere& sphere = (Sphere&) light;
  double distance = 1000000;
  double theta = 2*pi*(i + rand()/((double) RAND_MAX))/iMax; // 0 to 2pi
  double z = 2*(j + rand()/((double) RAND_MAX))/jMax - 1;  // -1 to 1
  Vector3d direction = getPointOnSphere(1, theta, z);
  Vector3d position = sphere.center + direction * sphere.radius;
  Ray ray(position + direction*epsilon, direction);

  boost::optional<Primitive&> primitive = raytrace(ray, 1, distance);
  if (primitive) {
    Vector3d intersectionPoint = ray.origin + ray.direction * distance;
    Color3f color = primitive.get().material.color * light.material.color;
    photonMap->add(new common::PhotonHit(intersectionPoint, color, primitive.get()));
    createShadowPhoton(primitive.get(), intersectionPoint, color, 0);
    return true;
  }
  return false;
}

void PhotonEngine::createShadowPhoton(Primitive& primitive, Vector3d intersectionPoint, Color3f color, int iteration) {
  // short circuit if we've reached the max iterations;
  if (iteration > 1) return;
  double distance = 1000000;
  Vector3d primNormal = primitive.getNormal(intersectionPoint);
  Vector3d zAxis(0, 0, 1);
  Vector3d direction = getRandomPointOnSphere(1);
  direction = rotateVector(direction, zAxis, primNormal);
  Ray ray(intersectionPoint + direction*epsilon, direction);
  boost::optional<Primitive&> newPrimitive = raytrace(ray, 1, distance);
  if (newPrimitive) {
    Vector3d newIntersectionPoint = ray.origin + ray.direction * distance;
    Color3f newColor = color * newPrimitive.get().material.color;
    shadowMap->add(new common::PhotonHit(newIntersectionPoint, newColor, newPrimitive.get()));
    createShadowPhoton(newPrimitive.get(), newIntersectionPoint, newColor, iteration + 1);
  }
}

Vector3d PhotonEngine::rotateVector(Vector3d vector, Vector3d normal1, Vector3d normal2) {
  Quaterniond q;
  q.setFromTwoVectors(normal1, normal2);
  Vector3d newVector = q * vector;
  return newVector;
}

Vector3d PhotonEngine::getRandomPointOnSphere(double radius) {
  double mult = 2.0 / RAND_MAX;
  double theta = rand()*mult*pi; // 0 to 2pi
  double z = rand()*mult - 1;  // -1 to 1
  return getPointOnSphere(radius, theta, z);
}

Vector3d PhotonEngine::getHaltonPointOnSphere(int id, double radius) {
  double theta = 2.0 * pi * halton(id, 2);
  double z = 2.0 * halton(id, 3) - 1;
  return getPointOnSphere(radius, theta, z);
}

Vector3d PhotonEngine::getRandomPointOnHalfSphere(double radius) {
  double mult = 2.0 / RAND_MAX;
  double theta = rand()*mult*pi;
  double z = rand() / (float) RAND_MAX;
  return getPointOnSphere(radius, theta, z);
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

bool PhotonEngine::finalGatherHelper(HitPoint& hit, Vector3d direction, Color3f& color) {
  double distance = 1000000;
  Vector3d position = hit.location;
  Vector3d normal = hit.primitive.getNormal(hit.location);
  Ray ray(position + direction*epsilon, direction);
  boost::optional<Primitive&> primitive = raytrace(ray, 1, distance);
  if (primitive) {
    Color3f tempColor;
    Vector3d intersectionPoint = ray.origin + ray.direction * distance;
    HitPoint tempHit = HitPoint(intersectionPoint, ray, primitive.get());
    photonMap->drawHit(tempHit, tempColor);
    shadowMap->drawHit(tempHit, tempColor);
    color += hit.primitive.material.diffusion * hit.primitive.material.color * tempColor;
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

//FIXME: Some kind of problem with how rays are hitting the rear wall.  It's
// causing speckling due to missed ray intersections.  Need to investigate.
Color3f PhotonEngine::finalGather(HitPoint& hit) {
  Color3f color(0, 0, 0);
  Vector3d position = hit.location;
  Vector3d normal = hit.primitive.getNormal(hit.location);
  int samples = Settings::instance()->samples;
  int hits = 0;
  int tries = 0;
  int offset = rand();
  while (hits < samples) {
    Vector3d direction = getHaltonPointOnHalfSphere(tries+offset, 1);
//    Vector3d direction = getRandomPointOnHalfSphere(1);
    direction = rotateVector(direction, Vector3d(0, 0, 1), normal);
    if (finalGatherHelper(hit, direction, color)) ++hits;
    else {
//      std::cout << "location:  x: " << hit.location.x() << " y: " << hit.location.y() << " z: " << hit.location.z() << "\n";
//      std::cout << "normal:    x: " << normal.x() << " y: " << normal.y() << " z: " << normal.z() << "\n";
//      std::cout << "direction: x: " << direction.x() << " y: " << direction.y() << " z: " << direction.z() << "\n";
    }
    ++tries;
  }
//  if (tries > 2.0 * hits) {
//    std::cout << "samples: " << samples << ", tries: " << tries << ", hits: " << hits << "\n";
//    std::cout << "normal: x: " << normal.x() << " y: " << normal.y() << " z: " << normal.z() << "\n";
//  }
  return (1.0/tries) * color;
}

Color3f PhotonEngine::renderPixel(int x, int y) {
  Color3f color(0, 0, 0);
  boost::optional<HitPoint>& hitPoint = hitPoints[y * width + x];

  // Short Circuit if nothing is there.
  if (!hitPoint) return color;
//  photonMap->drawHit(hitPoint.get(), color);
  // If set, return finalGather rather than direct visualization of the photonMap.
  if (Settings::instance()->finalGather) {
    color += finalGather(hitPoint.get());
  }
  else shadowMap->drawHit(hitPoint.get(), color);
  return color;
}
