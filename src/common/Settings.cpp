/*
 * Settings.cpp
 *
 *  Created on: Oct 4, 2011
 *      Author: nh
 */

#include "Settings.h"

Settings* Settings::pInstance = NULL;

Settings* Settings::instance() {
  if (pInstance == NULL) {
    pInstance = new Settings();
  }
  return pInstance;
}

void Settings::initialize(int argc, char *argv[]) {
  // Parse command line options
  int c;
  while((c =  getopt(argc, argv, "p:r:i:m:w:h:s:f:")) != EOF) {
    switch (c) {
      case 'p':
        photons = atoi(optarg);
        break;
      case 'r':
        photonRadius = atof(optarg);
        break;
      case 'i':
        iterations = atoi(optarg);
        break;
      case 'm':
        photonMult = atof(optarg);
        break;
      case 'w':
        width = atoi(optarg);
        break;
      case 'h':
        height = atoi(optarg);
        break;
      case 's':
        samples = atoi(optarg);
        break;
      case 'f':
        finalGather = atoi(optarg);
        break;
    }
  }
}

Settings::Settings() : photons(10000), photonRadius(5.0), iterations(1), photonMult(1.0), width(640), height(480), samples(16), finalGather(true) {
}

