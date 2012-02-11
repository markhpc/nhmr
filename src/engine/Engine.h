/*
 * Engine.h
 *
 *  Created on: Jul 24, 2011
 *      Author: nh
 */

#ifndef ENGINE_H_
#define ENGINE_H_
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <math.h>
#include <inttypes.h>
#include <time.h>
#include <boost/optional.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <ctime>
#include <tr1/unordered_map>
#include "common/Settings.h"
#include "common/Ray.h"
#include "common/Scene.h"
#include "common/Color3f.h"
#include "common/HitPoint.h"
#include "common/PhotonHit.h"
#include "photonmaps/PhotonMap.h"
#include "photonmaps/LinearSearch.h"
#include "photonmaps/BinarySearch.h"
#include "photonmaps/SpatialHash.h"
#include "photonmaps/KDTree.h"

using namespace Eigen;
using namespace std;

const double epsilon = 0.00000000001f;
const int aaSamplesX = 1;
const int aaSamplesY = 1;
const int traceDepth = 6;

class Engine {
  public:
  boost::ptr_vector<boost::optional<HitPoint> > hitPoints;
  Scene scene;
  int width, height;
  Engine(int _width, int _height);
  virtual ~Engine();
  void initRender(Vector3d position, Vector3d target);
  int raytrace(Ray& ray, int& primId, double& distance);
  virtual Color3f renderPixel(int x, int y);
};

#endif /* ENGINE_H_ */
