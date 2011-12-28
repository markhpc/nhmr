/*
 * SpatialHash.h
 *
 *  Created on: Sep 12, 2011
 *      Author: nh
 */

#ifndef SPATIALHASH_H_
#define SPATIALHASH_H_
#include <vector>
#include <iostream>
#include <tr1/unordered_map>
#include "common/Settings.h"
#include "common/PhotonHit.h"
#include "PhotonMap.h"

class SpatialHash: public PhotonMap {
  public:
  int size;
  std::vector<common::PhotonHit*> hits;
  std::tr1::unordered_multimap<int, common::PhotonHit*> photonHash;
  std::vector< std::vector<common::PhotonHit*> > photonVector;

  SpatialHash(double _photonHitRadius);
  virtual ~SpatialHash();
  void initDraw();
  void add(common::PhotonHit* photonHit);
  void drawHit(HitPoint& hitPoint, Color3f& color);
  private:
  int _hash(int x, int y, int z);
};

#endif /* SPATIALHASH_H_ */
