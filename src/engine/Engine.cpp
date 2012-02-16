/*
 * Engine.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "Engine.h"

Engine::Engine(int _width, int _height) :
  scene(), width(_width), height(_height) {
}

Engine::~Engine() {
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      for (std::vector<HitPoint*>::iterator it = hitPoints[x][y].begin(); it != hitPoints[x][y].end(); ++it) {
        delete *it;
      }
    }
  }
}

int Engine::raytrace(Ray& ray, int& primId, double& distance) {
//  boost::optional<Primitive&> primitive;
  int ret = Primitive::MISS;
  for (unsigned int i = 0; i < scene.primitives.size(); i++) {
    Primitive* tempPrimitive = scene.primitives[i];
    int hit = tempPrimitive->intersect(ray, distance);
    if (hit == Primitive::INSIDE || hit == Primitive::HIT) {
      ret = hit;
      primId = i;
    }
  }
//  std::cout << "primId: " << primId << "\n";
  return ret;
}

void Engine::initRender(Vector3d position, Vector3d target) {
  // Initialize the hitPoint vector
  hitPoints = vector<vector<vector<HitPoint*> > > (width, vector<vector<HitPoint*> >(height, vector<HitPoint*>()));

  Vector3d origin = Vector3d(0, 0, -5);
  Vector3d p1 = Vector3d(-4, 3, 0);
  Vector3d p2 = Vector3d(4, 3, 0);
  Vector3d p3 = Vector3d(4, -3, 0);
  Vector3d p4 = Vector3d(-4, -3, 0);

  // calculate camera matrix
  Vector3d zaxis = target - position;

  zaxis.normalize();
  Vector3d up( 0, 1, 0);
  Vector3d xaxis = up.cross(zaxis);
  Vector3d yaxis = xaxis.cross(-zaxis);

  Matrix4d m;
  m << xaxis.x(), xaxis.y(), xaxis.z(), 0,
       yaxis.x(), yaxis.y(), yaxis.z(), 0,
       zaxis.x(), zaxis.y(), zaxis.z(), 0,
       0,       0,       0,     1;

  Projective3d a(m);

  a = a.inverse();
  a(0,3) = position.x();
  a(1,3) = position.y();
  a(2,3) = position.z();
  origin = (a * origin.homogeneous().eval()).hnormalized();
  p1 = (a * p1.homogeneous().eval()).hnormalized();
  p2 = (a * p2.homogeneous().eval()).hnormalized();
  p3 = (a * p3.homogeneous().eval()).hnormalized();
  p4 = (a * p4.homogeneous().eval()).hnormalized();

  Vector3d dx = (p2 - p1) * (1.0f / width);
  Vector3d dy = (p4 - p1) * (1.0f / height);

  // Create the array of hit points
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      double distance = 1000000;
      Vector3d direction = p1 + (double) y * dy + (double) x * dx;
      direction = direction - origin;
      direction.normalize();
      Ray ray(origin + direction*epsilon, direction);

      int primId = -1;
      int hitType = raytrace(ray, primId, distance);
      if (primId != -1) {
//        std::cout << "x: " << x << ", y: " << y << ", primId: " << primId << "\n";
        Primitive& newPrim = *(scene.primitives[primId]);
        Vector3d intersectionPoint = ray.origin + ray.direction * distance;
        HitPoint* newHit = new HitPoint(intersectionPoint, ray, hitType, newPrim, Color3f(1, 1, 1));
        hitPoints[x][y].push_back(newHit);
        calculateReflection(newHit, x, y, 1, 0);
        calculateRefraction(newHit, x, y, 1, 0);
      }
    }
  }
}

void Engine::calculateReflection(HitPoint* hitPoint, int x, int y, float rIndexPrev, int depth) {
  double reflection = hitPoint->primitive.material.reflection;
  if (depth > traceDepth || reflection <= 0) return;

  Primitive& primitive = hitPoint->primitive;
  Color3f reflectionColor(0, 0, 0);
  Vector3d intersectionPoint = hitPoint->location;
  Vector3d N = primitive.getNormal(intersectionPoint);
  Vector3d R = hitPoint->ray.direction - 2 * hitPoint->ray.direction.dot(N) * N;
  double tmpDistance = 1000000;
  Ray ray = Ray(intersectionPoint + R * epsilon, R);
  int primId = -1;

  int hitType = raytrace(ray, primId, tmpDistance);
  if (primId != -1) {
    Color3f contribution = hitPoint->contribution * primitive.material.color * reflection;
//    std::cout << "contribution: " << contribution.r << ", " << contribution.g << ", " << contribution.b << "\n";
    Primitive& newPrim = *(scene.primitives[primId]);
    intersectionPoint = ray.origin + tmpDistance * ray.direction;
    HitPoint* newHit = new HitPoint(intersectionPoint, ray, hitType, newPrim, contribution);
    hitPoints[x][y].push_back(newHit);
    calculateReflection(newHit, x, y, rIndexPrev, depth+1);
    calculateRefraction(newHit, x, y, rIndexPrev, depth+1);
  }
}

void Engine::calculateRefraction(HitPoint* hitPoint, int x, int y, float rIndexPrev, int depth) {
  Primitive& primitive = hitPoint->primitive;
  double refraction = primitive.material.refraction;

  if (depth > traceDepth || refraction == 0) return;

  double rIndex = primitive.material.rIndex;
  double n = rIndexPrev / rIndex;
  Vector3d intersectionPoint = hitPoint->location;
//  Vector3d N = primitive.getNormal(intersectionPoint) * (double) hitPoint.hitType;
  Vector3d N = primitive.getNormal(intersectionPoint);
  if (N.dot(hitPoint->ray.direction) > 0) N *= -1.0;

  double cosI = -1 * N.dot(hitPoint->ray.direction);
  double cosT2 = 1.0 - n * n * (1.0 - cosI * cosI);
  if (cosT2 <= 0.0) return;

  Vector3d T = n * hitPoint->ray.direction + (n * cosI - sqrt(cosT2)) * N;

  Color3f rColor(0, 0, 0);
  double tmpDistance = 1000000;
  Ray ray = Ray(intersectionPoint + T * epsilon, T);
  int primId = -1;

  int hitType = raytrace(ray, primId, tmpDistance);
  if (primId != -1) {
    Primitive& newPrim = *(scene.primitives[primId]);
//    Color3f absorbance = newPrim.material.color * 0.15 * -tmpDistance;
//    Color3f transparency(exp(absorbance.r), exp(absorbance.b), exp(absorbance.g));
    Color3f contribution = hitPoint->contribution * primitive.material.color * refraction;
    intersectionPoint = ray.origin + tmpDistance * ray.direction;
    HitPoint* newHit = new HitPoint(intersectionPoint, ray, hitType, newPrim, contribution);
    hitPoints[x][y].push_back(newHit);
    calculateReflection(newHit, x, y, rIndex, depth+1);
    calculateRefraction(newHit, x, y, rIndex, depth+1);
  }
//  Color3f absorbance = primitive.material.color * 0.15 * -tmpDistance;
//  Color3f transparency(exp(absorbance.r), exp(absorbance.b), exp(absorbance.g));
//  std::cout << "rColor: " << rColor.r << ", " << rColor.g << ", " << rColor.b << "\n";
//  std::cout << "transparency: " << transparency.r << ", " << transparency.g << ", " << transparency.b << "\n";

//  return rColor * transparency;
}

Color3f Engine::renderPixel(int x, int y) {
  Color3f color(0, 0, 0);
//  boost::optional<HitPoint>& hitPoint = hitPoints[y * width + x];
//  if (hitPoint) {
//    drawpass(hitPoint.get(), color);
//    photonMap->drawHit(hitPoint.get(), color);
//    shadowMap->drawHit(hitPoint.get(), color);
//  }
//  if (hitPoint) photonPass(hitPoint.get(), color);
//  if (hitPoint) drawpass(hitPoint.get(), color);
  /*
  // Make a jittered grid AA sample
  int aaSamples = aaSamplesX * aaSamplesY;
//  if (aaSamples > 1 && current.status && current.reflection == 0) {
  if (aaSamples > 1 && hit) {
    color = Color3f(0, 0, 0);

    float jitterX = 0.5;
    float jitterY = -0.5;
    float iAASamplesX = 1.0f / aaSamplesX;
    float iAASamplesY = 1.0f / aaSamplesY;
    for (int tx = 0; tx < aaSamplesX; tx++) {
      for (int ty = 0; ty < aaSamplesY; ty++) {
        double distance = 1000000;
        float randx = ((rand() % 20) - 10) * 0.001f;
        float randy = ((rand() % 20) - 10) * 0.001f;
        float xpos = (2.0f * tx * iAASamplesX) - (aaSamplesX - 1 - randx) * iAASamplesX;
        float ypos = (2.0f * ty * iAASamplesY) - (aaSamplesY - 1 - randy) * iAASamplesY;
//        std::cout << "xpos: " << xpos << " ypos: " << ypos << "\n";
//        direction = Vector3d(sx + xpos * dx, sy + ypos * dy, 0) - origin;
//
        direction.normalize();
        ray = Ray(origin, direction);
        raytrace(ray, color, 1, 1, distance);
//        std::cout << "red: " << color.r << " green: " << color.g << " blue: " << color.b << "\n";
        jitterX = -jitterX;
      }
      jitterY = -jitterY;
    }
//    std::cout << "red: " << color.r << " green: " << color.g << " blue: " << color.b << "\n";
    color *= 1 / (float) aaSamples;
//    std::cout << "red: " << color.r << " green: " << color.g << " blue: " << color.b << "\n";
  }
  */
  return color;
}

