/*
 * SDLDrawer.cpp
 *
 *  Created on: Oct 4, 2011
 *      Author: nh
 */

#include "SDLDrawer.h"

//TODO think about ramifications of not explicitly cleaning this up.
SDLDrawer* SDLDrawer::pInstance = NULL;

SDLDrawer* SDLDrawer::instance() {
  if (pInstance == NULL) {
    pInstance = new SDLDrawer();
  }
  return pInstance;
}

SDLDrawer::SDLDrawer() : scr(SDL_SetVideoMode(Settings::instance()->width, Settings::instance()->height, 32, SDL_SWSURFACE)), done(false) {
  assert(scr);
  SDL_Init(SDL_INIT_VIDEO);
}

void SDLDrawer::processEvent() {
  while (SDL_PollEvent(&event)) {
    // Any of these event types will end the program
    switch (event.type) {
    case SDL_KEYDOWN:
      done = true;
      std::cout << "Key down\n";
      break;
    case SDL_KEYUP:
      done = true;
      std::cout << "Key up\n";
      break;
    case SDL_QUIT:
      done = true;
      std::cout << "Quit\n";
      break;
    default:
      break;
    }
  }
}

uint32_t SDLDrawer::getColor(Color3f color) {
  uint8_t red = (color.r < 1) ? (uint8_t) (color.r * 255) : 255;
  uint8_t green = (color.g < 1) ? (uint8_t) (color.g * 255) : 255;
  uint8_t blue = (color.b < 1) ? (uint8_t) (color.b * 255) : 255;
  return (uint32_t) (red << 16) + (green << 8) + (blue << 0);
}

void SDLDrawer::draw(Engine& engine, std::vector<std::vector<Color3f> >& layers, int layer, int iteration) {
  uint32_t *pixels = (uint32_t *) scr->pixels;
  std::vector<Color3f>& colors = layers[layer];
  for (int y = 0; y < scr->h; y++) {
    SDL_LockSurface(scr);
    for (int x = 0; x < scr->w; x++) {
      processEvent();
      if (done) {
        SDL_UnlockSurface(scr);
        SDL_UpdateRect(scr, 0, y, x, 1);
        return;
      }
      int location = y * scr->w + x;
      Color3f oldColor = colors[location];
      Color3f newColor = engine.renderPixel(x, y);
      colors[location] = ((oldColor * iteration) + newColor) * (1.0 / (iteration + 1));
      Color3f color = Color3f();
      for (unsigned int layer = 0; layer < layers.size(); layer++) {
        color += layers[layer][location];
      }
      pixels[location] = getColor(color);
    }
    SDL_UnlockSurface(scr);
    SDL_UpdateRect(scr, 0, y, scr->w, 1);
  }
}

void SDLDrawer::quit() {
  SDL_Quit();
}
