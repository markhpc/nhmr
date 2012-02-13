/*
 * SpatialHash.cpp
 *
 *  Created on: Sep 12, 2011
 *      Author: nh
 */

#include "SpatialHash.h"

SpatialHash::SpatialHash(double _photonHitRadius) :
    PhotonMap(_photonHitRadius),
    size(Settings::instance()->width * Settings::instance()->height) {
  photonVector.resize(size);
}

SpatialHash::~SpatialHash() {
  for (std::vector<common::PhotonHit*>::iterator it = hits.begin(); it != hits.end(); ++it) {
    delete *it;
  }
}

void SpatialHash::initDraw() {
}

void SpatialHash::add(common::PhotonHit* photonHit) {
  hits.push_back(photonHit);
  // get the cell size
  double cellSize = 1.0 / (photonHitRadius * 2);
  int xMin = (int) (photonHit->x() - photonHitRadius) * cellSize;
  int xMax = (int) (photonHit->x() + photonHitRadius) * cellSize;
  int yMin = (int) (photonHit->y() - photonHitRadius) * cellSize;
  int yMax = (int) (photonHit->y() + photonHitRadius) * cellSize;
  int zMin = (int) (photonHit->z() - photonHitRadius) * cellSize;
  int zMax = (int) (photonHit->z() + photonHitRadius) * cellSize;
  for (int x = xMin; x <= xMax; x++) {
    for (int y = yMin; y <= yMax; y++) {
      for (int z = zMin; z <= zMax; z++) {
        photonVector[_hash(x, y, z)].push_back(photonHit);
      }
    }
  }
}

int SpatialHash::_hash(int x, int y, int z) {
  return ((unsigned int) ((x * 73856093) ^ (y * 19349663) ^ (z * 83492791))) % size;
}

void SpatialHash::drawHit(HitPoint* hitPoint, Color3f& color) {
  double cellSize = 1.0 / (photonHitRadius * 2);
  int x = (int) hitPoint->location.x() * cellSize;
  int y = (int) hitPoint->location.y() * cellSize;
  int z = (int) hitPoint->location.z() * cellSize;
  std::vector<common::PhotonHit*> hits = photonVector[(_hash(x,y,z))];
  color += getColor(hits.begin(), hits.end(), hitPoint);
}
