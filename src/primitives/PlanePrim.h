/*
 * PlanePrim.h
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#ifndef PLANEPRIM_H_
#define PLANEPRIM_H_

#include "common/Plane.h"
#include "Primitive.h"

class PlanePrim : public Primitive {

  public:
  Plane plane;

  PlanePrim(Vector3d inNorm, double inDist);
  int intersect(Ray& ray, double& distance);
  Vector3d getNormal(const Vector3d& position);
  virtual ~PlanePrim();
};

#endif /* PLANEPRIM_H_ */
