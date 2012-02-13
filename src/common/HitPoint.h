/*
 * HitPoint.h
 *
 *  Created on: Aug 23, 2011
 *      Author: nh
 */

#ifndef HITPOINT_H_
#define HITPOINT_H_
#include <Eigen/Dense>
#include "common/Ray.h"
#include "primitives/Primitive.h"

using namespace Eigen;

class HitPoint {
  public:
  Vector3d location;
  Ray ray;
  Primitive& primitive;
  int hitType;
  float contribution;

  HitPoint(Vector3d _location, Ray _ray, int _hitType, Primitive& _primitive);
  virtual ~HitPoint();
};

#endif /* HITPOINT_H_ */
