/*
 * Ray.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "Ray.h"

Ray::Ray() {
  origin = Vector3d(0, 0, 0);
  direction = Vector3d(0, 0, 0);
}

Ray::Ray(Vector3d inOrig, Vector3d inDir) : origin(inOrig), direction(inDir) {
}

Ray::~Ray() {
  // TODO Auto-generated destructor stub
}

