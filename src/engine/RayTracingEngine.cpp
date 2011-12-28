/*
 * RayTracingEngine.cpp
 *
 *  Created on: Nov 7, 2011
 *      Author: nh
 */

#include "RayTracingEngine.h"

RayTracingEngine::RayTracingEngine(int _width, int _height) :
  Engine(_width, _height) {
}

RayTracingEngine::~RayTracingEngine() {
  // TODO Auto-generated destructor stub
}

void RayTracingEngine::drawPass(HitPoint& hitPoint, Color3f& color, int& depth) {
  if (depth > traceDepth) return;
  if (hitPoint.primitive.light) {
    color += Color3f(1, 1, 1);
    return;
  }

  for (unsigned int l = 0; l < scene.lights.size(); l++) {
    Primitive& light = scene.lights[l];
    double shade = calculateShadow(light, hitPoint.location);
    color += calculateDiffuse(hitPoint.primitive, light, hitPoint.location) * shade;
    color += calculateSpecular(hitPoint, light) * shade;
  }
  color += calculateReflection(hitPoint, depth);
}

Color3f RayTracingEngine::calculateDiffuse(Primitive& primitive, Primitive& light, Vector3d& intersectionPoint) {
  if (primitive.material.diffusion == 0) return Color3f(0, 0, 0);

  Sphere& sphere = (Sphere&) light;
  Vector3d normalVector = primitive.getNormal(intersectionPoint);
  Vector3d lightVector = sphere.center - intersectionPoint;
  lightVector = lightVector - (sphere.radius * lightVector.normalized());
//  float kc = 1;
//  float kl = 2 / sphere.radius;
//  float kq = 1 / sphere.squareRadius;
//  float attenuation = 1 / (kc + kl*lightVector.norm() + kq*lightVector.squaredNorm());
  float attenuation = 1 / lightVector.squaredNorm();
  lightVector.normalize();

  double dot = normalVector.dot(lightVector);
  if (dot <= 0) return Color3f(0, 0, 0);

  double diff = attenuation * primitive.material.diffusion;
  return dot * primitive.material.color * light.material.color * diff;
}

double RayTracingEngine::calculateShadow(Primitive& light, Vector3d& intersectionPoint) {
  Sphere& sphere = (Sphere&) light;
  Vector3d lightVector = sphere.center - intersectionPoint;
  lightVector = lightVector - (sphere.radius * lightVector.normalized());
  double tDist = lightVector.norm();
  lightVector.normalize();
  Ray tmpRay = Ray(intersectionPoint + lightVector * epsilon, lightVector);
  for (unsigned int s = 0; s < scene.primitives.size(); s++) {
    Primitive& tempPrimitive = scene.primitives[s];
    if (&tempPrimitive != &light && tempPrimitive.intersect(tmpRay, tDist) == 1) return 0.0;
  }
  return 1.0f;
}

Color3f RayTracingEngine::calculateSpecular(HitPoint& hitPoint, Primitive& light) {
  if (hitPoint.primitive.material.reflection <= 0) return Color3f(0, 0, 0);
  Vector3d normalVector = hitPoint.primitive.getNormal(hitPoint.location);
  Vector3d lightVector = ((Sphere&) light).center - hitPoint.location;
  float attenuation = 1 / lightVector.squaredNorm();
  lightVector.normalize();

  Vector3d r = lightVector - 2.0 * lightVector.dot(normalVector) * normalVector;
  double dot = hitPoint.ray.direction.dot(r);

  if (dot <= 0) return Color3f(0, 0, 0);

  double spec = attenuation*pow(dot, 20) * hitPoint.primitive.material.reflection;
  std::cout << "dot: " << dot << ", spec: " << spec << ", light color: ";
  std::cout << light.material.color.r << " " << light.material.color.g << " " << light.material.color.b << "\n";
  return spec * light.material.color;
}

Color3f RayTracingEngine::calculateReflection(HitPoint& hitPoint, int& depth) {
  double reflection = hitPoint.primitive.material.reflection;
  if (reflection <= 0) return Color3f(0, 0, 0);

  Primitive& primitive = hitPoint.primitive;
  Color3f reflectionColor(0, 0, 0);
  Vector3d intersectionPoint = hitPoint.location;
  Vector3d N = primitive.getNormal(intersectionPoint);
  Vector3d R = hitPoint.ray.direction - 2 * hitPoint.ray.direction.dot(N) * N;
  double tmpDistance = 1000000;
  Ray ray = Ray(intersectionPoint + R * epsilon, R);

  boost::optional<Primitive&> opPrimitive = raytrace(ray, depth, tmpDistance);
  if (opPrimitive) {
    intersectionPoint = ray.origin + tmpDistance * ray.direction;
    HitPoint hitPoint = HitPoint(intersectionPoint, ray, opPrimitive.get());
    drawPass(hitPoint, reflectionColor, depth);
  }
  return (reflection * reflectionColor * primitive.material.color);
}

Color3f RayTracingEngine::renderPixel(int x, int y) {
  Color3f color(0, 0, 0);
  int depth = 0;
  boost::optional<HitPoint>& hitPoint = hitPoints[y * width + x];
  if (hitPoint) {
    drawPass(hitPoint.get(), color, depth);
  }
  return color;
}
