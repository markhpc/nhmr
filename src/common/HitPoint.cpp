/*
 * HitPoint.cpp
 *
 *  Created on: Aug 23, 2011
 *      Author: nh
 */

#include "HitPoint.h"

HitPoint::HitPoint(Vector3d _location, Ray _ray, int _hitType, Primitive& _primitive) : location(_location), ray(_ray), primitive(_primitive), hitType(_hitType) {
}

HitPoint::~HitPoint() {
  // TODO Auto-generated destructor stub
}

