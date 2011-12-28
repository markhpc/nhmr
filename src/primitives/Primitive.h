/*
 * Primitive.h
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_
#include <string>
#include <Eigen/Dense>
#include "materials/Material.h"
#include "common/Ray.h"
#include "common/Color3f.h"
using namespace Eigen;

class Primitive {
  public:
  static const int HIT = 1;
  static const int MISS = 0;
  static const int INSIDE = -1;
  Material material;
  std::string name;
  bool light;

  enum types_t {
    SPHERE,
    PLANE
  };
  types_t type;

  virtual int intersect(Ray& ray, double& distance) =  0;
  virtual Vector3d getNormal(const Vector3d& position) = 0;
  virtual ~Primitive() {};
  virtual Color3f color() { return material.color; }
};

#endif /* PRIMITIVE_H_ */
