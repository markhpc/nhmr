/*
 * PlanePrim.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "PlanePrim.h"

PlanePrim::PlanePrim(Vector3d inNorm, double inDist) : plane(Plane(inNorm, inDist)) {
//  plane = new Plane(inNorm, inDist);
  type = PLANE;
  light = false;
}

Vector3d PlanePrim::getNormal(const Vector3d& position) {
  return plane.normal;
}

int PlanePrim::intersect(Ray& ray, double& distance) {
  double dot = plane.normal.dot(ray.direction);
  if (dot != 0) {
    double newDistance = -(plane.normal.dot(ray.origin) + plane.distance) / dot;
    if (newDistance > 0 && newDistance < distance) {
      distance = newDistance;
      return HIT;
    }
  }
  return MISS;
}

PlanePrim::~PlanePrim() {
  // TODO Auto-generated destructor stub
}

