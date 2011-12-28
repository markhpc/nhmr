/*
 * Plane.h
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#ifndef PLANE_H_
#define PLANE_H_
#include <Eigen/Dense>
using namespace Eigen;

class Plane {
  public:
  Vector3d normal;
  float distance;
  Plane();
  Plane(Vector3d normal, float distance);
  virtual ~Plane();
};

#endif /* PLANE_H_ */
