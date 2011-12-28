/*
 * Sphere.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "Sphere.h"

Sphere::Sphere(Vector3d inCenter, double inRadius) {
  center = inCenter;
  radius = inRadius;
  squareRadius = inRadius * inRadius;
  inverseRadius = 1.0 / inRadius;
  type = SPHERE;
  light = false;
}
/*
 int Sphere::intersect(Ray &ray, double &distance) { // returns distance
 Vector3d op = center - ray.origin;
 double b = op.dot(ray.direction);
 double det = b * b - op.dot(op) + squareRadius;

 if (det < 0) return MISS;
 det = sqrt(det);

 float i1 = b - det;
 float i2 = b + det;
 if (i2 > 0) {
 if (i1 < 0) {
 //      if (i2 < distance) {
 distance = i2;
 return INSIDE;
 //      }
 } else {
 if (i1 < distance) {
 distance = i1;
 return HIT;
 }
 }
 }
 return MISS;
 }
 */

//  double t = (b + det < b - det) ? b + det : b - det;
//  double t = b - det;
//  if (t > 0  && t < distance) {
//    distance = t;
//    if (b + det < 0) {
//      return INSIDE;
//    }
//    return HIT;
//  }
//  t = b + det;
//  if (t > 0 && t < distance) {
//    distance = t;
//    return HIT;
//  }
//  return MISS;
//}
/*
 int Sphere::intersect(Ray& ray, double& distance) {
 Vector3d vector = ray.origin - center;
 double a = ray.direction.dot(ray.direction);
 double b = 2 * vector.dot(ray.direction);
 double c = vector.dot(vector) - squareRadius;
 double disc = b * b - 4 * a * c;

 if (disc < 0) return MISS;

 double distSqrt = sqrt(disc);
 double q = (b < 0) ? (-b - distSqrt) * 0.5f : (-b + distSqrt) * 0.5f;

 // compute t0 and t1
 double t0 = q / a;
 double t1 = c / q;

 // make sure t0 is smaller than t1
 if (t0 > t1) {
 double temp = t0;
 t0 = t1;
 t1 = temp;
 }

 // if t0 is farther than the starting distance, there's a already a primitive
 // closer to the camera.  If t1 is less than 0, the object is behind the camera.
 if (t0 > distance || t1 < 0) return MISS;

 // if t0 is less than zero, the intersection point is at t1, otherwise t0
 // TODO: Do we care if we are inside the sphere (ie t0 < 0)?
 distance = (t0 < 0) ? t1 : t0;
 return HIT;
 }
 */

int Sphere::intersect(Ray& ray, double& distance) {
  Vector3d vector = ray.origin - center;
  double b = -vector.dot(ray.direction);
  double det = (b * b) - vector.dot(vector) + squareRadius;
  int retVal = MISS;

  if (det > 0) {
    det = (double) sqrt(det);
    double i1 = b - det;
    double i2 = b + det;
    if (i2 > 0) {
      if (i1 < 0) {
//        if (i2 < distance) {
          distance = i2;
          retVal = INSIDE;
//        }
      } else {
        if (i1 < distance) {
          distance = i1;
          retVal = HIT;
        }
      }
    }
  }
//  std::cout << "name: " << name << " retVal: " << retVal << " distance: " << distance << "\n";
  return retVal;
}

Vector3d Sphere::getNormal(const Vector3d& position) {
  Vector3d ret = position - center;
  ret.normalize();
  return ret;
//  return ((position - center) * inverseRadius);
}

Sphere::~Sphere() {
  // TODO Auto-generated destructor stub
}

