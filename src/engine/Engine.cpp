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

boost::optional<Primitive&> Engine::raytrace(Ray& ray, double depth, double& distance) {
  boost::optional<Primitive&> primitive;

  if (depth > traceDepth) return primitive;

  for (unsigned int i = 0; i < scene.primitives.size(); i++) {
    Primitive& tempPrimitive = scene.primitives[i];
    int hit = tempPrimitive.intersect(ray, distance);
    if (hit == Primitive::INSIDE) {
//      std::cout << "inside!\n";
      return boost::optional<Primitive&>();
    }
    if (hit == Primitive::HIT) primitive = boost::optional<Primitive&>(tempPrimitive);
  }
  return primitive;
}

void Engine::initRender(Vector3d position, Vector3d target) {
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

      boost::optional<Primitive&> primitive = raytrace(ray, 1, distance);
      if (primitive) {
        Vector3d intersectionPoint = ray.origin + ray.direction * distance;
        HitPoint hitPoint = HitPoint(intersectionPoint, ray, primitive.get());
        hitPoints.push_back(new boost::optional<HitPoint>(hitPoint));
      } else {
    	hitPoints.push_back(new boost::optional<HitPoint>());
      }
    }
  }
//  createPhotonMap();
}

Color3f Engine::renderPixel(int x, int y) {
  Color3f color(0, 0, 0);
  boost::optional<HitPoint>& hitPoint = hitPoints[y * width + x];
  if (hitPoint) {
//    drawpass(hitPoint.get(), color);
//    photonMap->drawHit(hitPoint.get(), color);
//    shadowMap->drawHit(hitPoint.get(), color);
  }
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

Engine::~Engine() {
}

