/*
 * RayTracingEngine.h
 *
 *  Created on: Nov 7, 2011
 *      Author: nh
 */

#ifndef RAYTRACINGENGINE_H_
#define RAYTRACINGENGINE_H_

#include "Engine.h"

class RayTracingEngine: public Engine {
  public:
  RayTracingEngine(int _width, int _height);
  virtual ~RayTracingEngine();
  virtual Color3f renderPixel(int x, int y);
  virtual double calculateShadow(Primitive& light, Vector3d& intersectionPoint);
  virtual Color3f calculateDiffuse(Primitive& primitive, Primitive& light, Vector3d& intersectionPoint);
  virtual Color3f calculateSpecular(HitPoint& hitPoint, Primitive& light);
  virtual Color3f calculateReflection(HitPoint& hitPoint, int &depth);
  virtual void drawPass(HitPoint& hitPoint, Color3f& color, int& depth);
};

#endif /* RAYTRACINGENGINE_H_ */
