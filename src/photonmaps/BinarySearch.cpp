/*
 * BinarySearch.cpp
 *
 *  Created on: Aug 27, 2011
 *      Author: nh
 */

#include "BinarySearch.h"
using namespace common;

BinarySearch::BinarySearch(double _photonHitRadius) : PhotonMap(_photonHitRadius) {
}

BinarySearch::~BinarySearch() {
  for (vector<PhotonHit*>::iterator it = xHits.begin(); it != xHits.end(); ++it) {
    delete *it;
  }
}

void BinarySearch::add(PhotonHit* photonHit) {
  xHits.push_back(photonHit);
  yHits.push_back(photonHit);
  zHits.push_back(photonHit);
}

void BinarySearch::initDraw() {
  sort(xHits.begin(), xHits.end(), xComp());
  sort(yHits.begin(), yHits.end(), yComp());
  sort(zHits.begin(), zHits.end(), zComp());
}

void BinarySearch::drawHit(HitPoint* hitPoint, Color3f& color) {
  int size = xHits.size();
  vector<bool> boolVec;
  boolVec.resize(size);

  vector<PhotonHit*>::iterator lowerX = lower_bound(xHits.begin(), xHits.end(), hitPoint->location.x() - photonHitRadius, xComp());
  vector<PhotonHit*>::iterator upperX = upper_bound(xHits.begin(), xHits.end(), hitPoint->location.x() + photonHitRadius, xComp());
  while (lowerX < upperX) {
    boolVec[(unsigned int) ((uintptr_t) *lowerX) % size] = true;
    ++lowerX;
  }
  vector<PhotonHit*>::iterator lowerY = lower_bound(yHits.begin(), yHits.end(), hitPoint->location.y() - photonHitRadius, yComp());
  vector<PhotonHit*>::iterator upperY = upper_bound(yHits.begin(), yHits.end(), hitPoint->location.y() + photonHitRadius, yComp());
  while (lowerY < upperY) {
    unsigned int hash = (unsigned int) ((uintptr_t) *lowerY) % size;
    boolVec[hash] = boolVec[hash] & true;
    ++lowerY;
  }
  vector<PhotonHit*>::iterator lowerZ = lower_bound(zHits.begin(), zHits.end(), hitPoint->location.z() - photonHitRadius, zComp());
  vector<PhotonHit*>::iterator upperZ = upper_bound(zHits.begin(), zHits.end(), hitPoint->location.z() + photonHitRadius, zComp());
  vector<PhotonHit*> hits;
  while (lowerZ < upperZ) {
    unsigned int hash = (unsigned int) ((uintptr_t) *lowerZ) % size;
    if (boolVec[hash]) hits.push_back((PhotonHit*) *lowerZ);
    ++lowerZ;
  }
  color += getColor(hits.begin(), hits.end(), hitPoint);
}
