/*
 * PhotonMap.cpp
 *
 *  Created on: Oct 5, 2011
 *      Author: nh
 */

#include "PhotonMap.h"

PhotonMap::PhotonMap(double _photonHitRadius) : photonHitRadius(_photonHitRadius), fluxWeight(1.0 / Settings::instance()->photons) {
}
