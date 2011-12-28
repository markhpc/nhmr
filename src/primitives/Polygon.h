/*
 * Polygon.h
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include <vector>
#include "common/Plane.h"
#include "Primitive.h"

class Polygon : public Primitive {

  public:
  Plane plane;
  std::vector<Vector3d> vertices;
  Polygon(std::vector<Vector3d> _vertices);
  int intersect(Ray& ray, double& distance);
  Vector3d getNormal(const Vector3d& position);
  virtual ~Polygon();
};

#endif /* POLYGON_H_ */
