/*
 * Ray.h
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#ifndef RAY_H_
#define RAY_H_
#include <Eigen/Dense>
using namespace Eigen;

class Ray {
  public:
  Vector3d origin;
  Vector3d direction;
  Ray();
  Ray(Vector3d inOrig, Vector3d inDir);
  virtual ~Ray();
};

#endif /* RAY_H_ */
