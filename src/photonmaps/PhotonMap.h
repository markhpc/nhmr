/*
 * PhotonMap.h
 *
 *  Created on: Aug 27, 2011
 *      Author: nh
 */

#ifndef PHOTONMAP_H_
#define PHOTONMAP_H_
#include <boost/ptr_container/ptr_vector.hpp>
#include <iostream>
#include "common/Settings.h"
#include "common/PhotonHit.h"
#include "common/HitPoint.h"
#include "common/Color3f.h"

const double pi = 3.14159;

class PhotonMap {
  public:
    double photonHitRadius;
    double fluxWeight;
    PhotonMap(double _photonHitRadius);
    virtual void add(common::PhotonHit *photonHit) = 0;
    virtual void drawHit(HitPoint* hitPoint, Color3f& color) = 0;
    virtual void initDraw() = 0;
    virtual ~PhotonMap() {};
    Color3f getColor(std::vector<common::PhotonHit*>::iterator lower, std::vector<common::PhotonHit*>::iterator upper, HitPoint* hitPoint) {
      Color3f tempColor(0, 0, 0);
      while (lower != upper) {
        common::PhotonHit* photonHit = (*lower);
        Vector3d vector = photonHit->location - hitPoint->location;
        double distance = vector.norm();
        if (distance < photonHitRadius) {
          vector.normalize();
          float dot = photonHit->direction.dot(hitPoint->primitive.getNormal(photonHit->location));
          if (dot < 0) {
            tempColor += photonHit->color * -dot * photonHit->primitive.material.diffusion;
          }
        }
        ++lower;
      }
      double weight = 16 * fluxWeight;
      return tempColor * (1.0 / (pi * photonHitRadius * photonHitRadius)) * weight;
    }
};



#endif /* PHOTONMAP_H_ */
