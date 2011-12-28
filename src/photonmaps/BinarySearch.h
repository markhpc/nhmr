/*
 * BinarySearch.h
 *
 *  Created on: Aug 27, 2011
 *      Author: nh
 */

#ifndef BINARYSEARCH_H_
#define BINARYSEARCH_H_
#include <iostream>
#include <stdint.h>
#include <vector>
#include <iterator>
#include <tr1/unordered_map>
#include "PhotonMap.h"
#include "common/PhotonHit.h"

using namespace std;
class BinarySearch : public PhotonMap {
  public:
    struct xComp {
      bool operator()(const common::PhotonHit* a, const common::PhotonHit* b) {
        return a->x() < b->x();
      }
      bool operator()(const common::PhotonHit* a, double &b) {
        return a->x() < b;
      }
      bool operator()(const common::PhotonHit* a, double const &b) {
        return a->x() < b;
      }
      bool operator()(double const &a, const common::PhotonHit* b) {
        return a < b->x();
      }
    };

    struct yComp {
      bool operator()(const common::PhotonHit* a, const common::PhotonHit* b) {
        return a->y() < b->y();
      }
      bool operator()(const common::PhotonHit* a, double &b) {
        return a->y() < b;
      }
      bool operator()(const common::PhotonHit* a, double const &b) {
        return a->y() < b;
      }
      bool operator()(double const &a, const common::PhotonHit* b) {
        return a < b->y();
      }
    };

    struct zComp {
      bool operator()(const common::PhotonHit* a, const common::PhotonHit* b) {
        return a->z() < b->z();
      }
      bool operator()(const common::PhotonHit* a, double &b) {
        return a->z() < b;
      }
      bool operator()(const common::PhotonHit* a, double const &b) {
        return a->z() < b;
      }
      bool operator()(double const &a, const common::PhotonHit* b) {
        return a < b->z();
      }
    };

    std::vector<common::PhotonHit*> xHits;
    std::vector<common::PhotonHit*> yHits;
    std::vector<common::PhotonHit*> zHits;

    BinarySearch(double _photonHitRadius);
    void add(common::PhotonHit*);
    void initDraw();
    void drawHit(HitPoint& hitPoint, Color3f& color);
    virtual ~BinarySearch();
};

#endif /* BINARYSEARCH_H_ */
