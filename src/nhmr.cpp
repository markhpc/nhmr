/*
 ============================================================================
 Name        : rhmr.c
 Author      : nhm
 Version     : 0.1 pre-alpha
 Copyright   : Mark Nelson, 2011
 Description : Raytracer/Photon Mapping engine implementation
 ============================================================================
 */

#include <iostream>
#include <inttypes.h>
#include <SDL/SDL.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "common/Color3f.h"
#include "common/SDLDrawer.h"
#include "engine/RayTracingEngine.h"
#include "engine/PhotonEngine.h"

int main(int argc, char *argv[]) {
  // seed the random number generator
  srand((unsigned int) time(NULL));

  // Initialize the settings instance
  Settings::instance()->initialize(argc, argv);

  // Get the SDL drawer instance
  SDLDrawer* drawer = SDLDrawer::instance();

  // Get various settings
  int width = drawer->scr->w;
  int height = drawer->scr->h;
  int iterations = Settings::instance()->iterations;
  float photonRadius = Settings::instance()->photonRadius;
  float photonMult = Settings::instance()->photonMult;

  // Start the timer
  boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();

  // Create the layers
  std::vector<std::vector<Color3f> > layers;

  // Draw the raytracing layer
  layers.push_back(std::vector<Color3f>(width * height));
  RayTracingEngine rtEngine(width, height);
//  rtEngine.initRender(Vector3d(50, 52, 0), Vector3d(50, 52, 1));
  rtEngine.initRender(Vector3d(0, 0, 0), Vector3d(0, 0, 1));
  drawer->draw(rtEngine, layers, layers.size() - 1, 0);

  // Draw the photon layer
  layers.push_back(std::vector<Color3f>(width * height));

  int i = 0;
  while (!drawer->done) {
    // Load the engine
    if (i < iterations) {
      PhotonEngine engine(width, height, photonRadius, i);
      photonRadius *= photonMult;
//      engine.initRender(Vector3d(50, 52, 0), Vector3d(50, 52, 1));
      engine.initRender(Vector3d(0, 0, 0), Vector3d(0, 0, 1));
      drawer->draw(engine, layers, layers.size() - 1, i);
      std::cout << "Finished pass: " << i << "\n";
      ++i;
    }
    // Finish up
    if (i == iterations  || drawer->done) {
      boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();
      boost::posix_time::time_duration duration = end - start;
      std::cout << "Render time: " << to_simple_string(duration) << std::endl;
      ++i;
    }
    drawer->processEvent();
  }
  drawer->quit();
  return EXIT_SUCCESS;
}
