/*
 * Plane.cpp
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#include "Plane.h"

Plane::Plane() : normal(Vector3d()), distance(0) {
}

Plane::Plane(Vector3d inNorm, float inDist) : normal(inNorm), distance(inDist) {
}

Plane::~Plane() {
  // TODO Auto-generated destructor stub
}

