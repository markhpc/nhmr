/*
 * LinearSearch.cpp
 *
 *  Created on: Aug 27, 2011
 *      Author: nh
 */

#include "LinearSearch.h"

LinearSearch::LinearSearch(double _photonHitRadius) : PhotonMap(_photonHitRadius) {
}

LinearSearch::~LinearSearch() {
  for (std::vector<common::PhotonHit*>::iterator it = hits.begin(); it != hits.end(); ++it) {
    delete *it;
  }
}

void LinearSearch::add(common::PhotonHit* photonHit) {
  hits.push_back(photonHit);
}

void LinearSearch::initDraw() {
}

void LinearSearch::drawHit(HitPoint* hitPoint, Color3f& color) {
  color += getColor(hits.begin(), hits.end(), hitPoint);
}
