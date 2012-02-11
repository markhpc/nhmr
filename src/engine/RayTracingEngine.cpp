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

void RayTracingEngine::drawPass(HitPoint& hitPoint, Color3f& color, float rIndexPrev, int depth) {
  if (depth > traceDepth) return;
  if (hitPoint.primitive.light) {
    color += Color3f(1, 1, 1);
    return;
  }
  depth++;

  for (unsigned int l = 0; l < scene.lights.size(); l++) {
    Primitive& light = *(scene.lights[l]);
    double shade = calculateShadow(light, hitPoint.location);
    if (shade == 0) continue;
    color += calculateDiffuse(hitPoint.primitive, light, hitPoint.location) * shade;
    color += calculateSpecular(hitPoint, light) * shade;
  }
  color += calculateReflection(hitPoint, rIndexPrev, depth);
  color += calculateRefraction(hitPoint, rIndexPrev, depth);
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

  double diff = primitive.material.diffusion;
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
    Primitive& tempPrimitive = *(scene.primitives[s]);
    if (&tempPrimitive != &light && abs(tempPrimitive.intersect(tmpRay, tDist)) == 1) return 0.0;
  }
  return 1.0f;
}

Color3f RayTracingEngine::calculateSpecular(HitPoint& hitPoint, Primitive& light) {
  if (hitPoint.primitive.material.specular <= 0) return Color3f(0, 0, 0);
  Vector3d normalVector = hitPoint.primitive.getNormal(hitPoint.location);
  Vector3d lightVector = ((Sphere&) light).center - hitPoint.location;
  float attenuation = 1 / lightVector.squaredNorm();
  lightVector.normalize();

  Vector3d r = lightVector - 2.0 * lightVector.dot(normalVector) * normalVector;
  double dot = hitPoint.ray.direction.dot(r);

  if (dot <= 0) return Color3f(0, 0, 0);

  double spec = pow(dot, 20) * hitPoint.primitive.material.specular;
//  std::cout << "dot: " << dot << ", spec: " << spec << ", light color: ";
//  std::cout << light.material.color.r << " " << light.material.color.g << " " << light.material.color.b << "\n";
  return spec * light.material.color;
}

Color3f RayTracingEngine::calculateReflection(HitPoint& hitPoint, float rIndexPrev, int depth) {
  double reflection = hitPoint.primitive.material.reflection;
  if (reflection <= 0) return Color3f(0, 0, 0);

  Primitive& primitive = hitPoint.primitive;
  Color3f reflectionColor(0, 0, 0);
  Vector3d intersectionPoint = hitPoint.location;
  Vector3d N = primitive.getNormal(intersectionPoint);
  Vector3d R = hitPoint.ray.direction - 2 * hitPoint.ray.direction.dot(N) * N;
  double tmpDistance = 1000000;
  Ray ray = Ray(intersectionPoint + R * epsilon, R);
  int primId = -1;

  int hitType = raytrace(ray, primId, tmpDistance);
  if (primId != -1) {
    Primitive& newPrim = *(scene.primitives[primId]);
    intersectionPoint = ray.origin + tmpDistance * ray.direction;
    HitPoint hitPoint = HitPoint(intersectionPoint, ray, hitType, newPrim);
    drawPass(hitPoint, reflectionColor, rIndexPrev, depth);
  }
  return (reflection * reflectionColor * primitive.material.color);
}

Color3f RayTracingEngine::calculateRefraction(HitPoint& hitPoint, float rIndexPrev, int depth) {
  Primitive& primitive = hitPoint.primitive;
  double refraction = primitive.material.refraction;

  if (refraction == 0) return Color3f(0, 0, 0);

  double rIndex = primitive.material.rIndex;
  double n = rIndexPrev / rIndex;
  Vector3d intersectionPoint = hitPoint.location;
  Vector3d N = primitive.getNormal(intersectionPoint) * (double) hitPoint.hitType;

  double cosI = -1 * N.dot(hitPoint.ray.direction);
  double cosT2 = 1.0 - n * n * (1.0 - cosI * cosI);
  if (cosT2 <= 0.0) return Color3f(0, 0, 0);

  Vector3d T = n * hitPoint.ray.direction + (n * cosI - sqrt(cosT2)) * N;

  Color3f rColor(0, 0, 0);
  double tmpDistance = 1000000;
  Ray ray = Ray(intersectionPoint + T * epsilon, T);
  int primId = -1;

  int hitType = raytrace(ray, primId, tmpDistance);
  if (primId != -1) {
    Primitive& newPrim = *(scene.primitives[primId]);
//    std::cout << "found a primitive\n";
    intersectionPoint = ray.origin + tmpDistance * ray.direction;
    HitPoint hitPoint = HitPoint(intersectionPoint, ray, hitType, newPrim);
    drawPass(hitPoint, rColor, rIndex, depth);
  }
  Color3f absorbance = primitive.material.color * 0.15 * -tmpDistance;
  Color3f transparency(exp(absorbance.r), exp(absorbance.b), exp(absorbance.g));
//  std::cout << "rColor: " << rColor.r << ", " << rColor.g << ", " << rColor.b << "\n";
//  std::cout << "transparency: " << transparency.r << ", " << transparency.g << ", " << transparency.b << "\n";

  return rColor * transparency;
//  return rColor;
}

Color3f RayTracingEngine::renderPixel(int x, int y) {
  Color3f color(0, 0, 0);
  int depth = 0;
  boost::optional<HitPoint>& hitPoint = hitPoints[y * width + x];
  if (hitPoint) {
    drawPass(hitPoint.get(), color, 1, depth);
  }
  return color;
}
