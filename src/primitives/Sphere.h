/*
 * Sphere.h
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include <iostream>
#include <string>
#include <Eigen/Dense>
#include "Primitive.h"
#include <math.h>
using namespace Eigen;

class Sphere : public Primitive {

  public:
  Vector3d center;
  double radius;
  double squareRadius;
  double inverseRadius;

  Sphere(Vector3d inCenter, double inRadius);
  int intersect(Ray& ray, double& distance);
  Vector3d getNormal(const Vector3d& position);
  virtual ~Sphere();
};

#endif /* SPHERE_H_ */
