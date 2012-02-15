/*
 * PhotonEngine.h
 *
 *  Created on: Nov 8, 2011
 *      Author: nh
 */

#ifndef PHOTONENGINE_H_
#define PHOTONENGINE_H_

#include "Engine.h"
#include "common/PhotonHit.h"

class PhotonEngine: public Engine {
  public:
  PhotonMap* photonMap;
  PhotonMap* shadowMap;
  PhotonMap* specMap;

  PhotonEngine(int _width, int _height, double _photonHitRadius);
  virtual ~PhotonEngine();
  void createPhotonMap();
  void russianRoulette(common::PhotonHit* photonHit, int iteration);
  virtual Color3f renderPixel(int x, int y);
  bool photonTrace(Primitive& light, int i, int iMax, int j, int jMax);
  void createShadowPhoton(common::PhotonHit* photonHit, int iteration);
  void reflectPhoton(common::PhotonHit* photonHit, int iteration);
  void refractPhoton(common::PhotonHit* photonHit, int iteration);
  Vector3d getRandomPointOnSphere(double radius);
  Vector3d getHaltonPointOnSphere(int id, double radius);
  Vector3d getRandomPointOnHalfSphere(double radius);
  Vector3d getHaltonPointOnHalfSphere(int id, double radius);
  Vector3d getPointOnSphere(double radius, double theta, double z);
  Vector3d rotateVector(Vector3d vector, Vector3d normal1, Vector3d normal2);
  float halton(int id, int prime);
  Color3f finalGather(HitPoint* hit);
  Color3f finalGather2(HitPoint* hit);
  bool finalGatherHelper(HitPoint* hit, Vector3d direction, Color3f& color);
};

#endif /* PHOTONENGINE_H_ */
