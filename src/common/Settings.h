/*
 * Settings.h
 *
 *  Created on: Oct 4, 2011
 *      Author: nh
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>

class Settings {
  public:
    int photons;
    float photonRadius;
    int iterations;
    float photonMult;
    int width;
    int height;
    int samples;
    bool finalGather;
    static Settings* instance();
    void initialize(int argc, char *argv[]);
    ~Settings();
  protected:
    Settings(); // constructor
  private:
    static Settings* pInstance;
};
#endif /* SETTINGS_H_ */
