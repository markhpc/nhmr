/*
 * SDLDrawer.h
 *
 *  Created on: Oct 4, 2011
 *      Author: nh
 */

#ifndef SDLDRAWER_H_
#define SDLDRAWER_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <assert.h>
#include <SDL/SDL.h>
#include "common/Settings.h"
#include "engine/Engine.h"

class SDLDrawer {
  public:
    SDL_Surface *scr;
    SDL_Event event;
    bool done;
    void processEvent();
    static SDLDrawer* instance();
    uint32_t getColor(Color3f color);
    void draw(Engine& engine, std::vector<std::vector<Color3f> >& layers, int layer, int iteration);
    void quit();
    ~SDLDrawer();
  protected:
    SDLDrawer(); // constructor
  private:
    static SDLDrawer* pInstance;
};
#endif /* SDLDRAWER_H_ */
