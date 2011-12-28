/*
 * common::PhotonHit.cpp
 *
 *  Created on: Aug 23, 2011
 *      Author: nh
 */

#include "PhotonHit.h"

common::PhotonHit::PhotonHit(Vector3d _location, Color3f _color, Primitive& _primitive) : location(_location), color(_color), primitive(_primitive) {
}

double common::PhotonHit::x() const {
  return location.x();
}

double common::PhotonHit::y() const {
  return location.y();
}

double common::PhotonHit::z() const {
  return location.z();
}

bool common::xComp(const common::PhotonHit* a, const common::PhotonHit* b) {
  return a->x() < b->x();
}

bool common::yComp(const common::PhotonHit* a, const common::PhotonHit* b) {
  return a->y() < b->y();
}

bool common::zComp(const common::PhotonHit* a, const common::PhotonHit* b) {
  return a->z() < b->z();
}

common::PhotonHit::~PhotonHit() {
  // TODO Auto-generated destructor stub
}


