/*
 * LinearSearch.h
 *
 *  Created on: Aug 27, 2011
 *      Author: nh
 */

#ifndef LINEARSEARCH_H_
#define LINEARSEARCH_H_
#include <iostream>
#include <vector>
#include <iterator>
#include "PhotonMap.h"
#include "common/PhotonHit.h"

class LinearSearch : public PhotonMap {
  public:
    std::vector<common::PhotonHit*> hits;
    LinearSearch(double _photonHitRadius);
    void add(common::PhotonHit*);
    void initDraw();
    void drawHit(HitPoint& hitPoint, Color3f& color);
    virtual ~LinearSearch();
};

#endif /* LINEARSEARCH_H_ */
