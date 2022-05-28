#pragma once

#include "defs.hpp"
#include "rect.h"

#include "SDL.h"

class OsciChannel {

public:
  static const int w = 256;
  static const int h = 128;

  OsciChannel();
  ~OsciChannel();

  void init();
  void deinit();

  void render(int16_t *parr);

  SDL_Surface *GetSurface() { return data_sf; }

private:
  uint32_t *pgfxbuffer = nullptr;
  int16_t abuff[w];
  SDL_Surface *data_sf = nullptr;
};