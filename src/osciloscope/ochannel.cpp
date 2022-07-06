#include "ochannel.hpp"

#define DEBUG_OSCI 1

#define sdlmask32(X) 0xFF << (X * 8)

#define colorBackground 0x2F2F2F
#define colorFrame 0x5F5F5F
#define colorValue 0xFF00

#define setpxl(X, Y, C)                                                        \
  ;                                                                            \
  {                                                                            \
    uint16_t offs = ((Y)*w) + X;                                               \
    pgfxbuffer[offs] = C;                                                      \
  }

OsciChannel::OsciChannel() {
  abuff[0] = 0; // warn fixed
}

OsciChannel::~OsciChannel() {
  deinit(); //
}

void OsciChannel::init() {
  int bts = w * h * 4;
  pgfxbuffer = (uint32_t *)malloc(bts);

#ifdef DEBUG_OSCI
  printf("OsciChannel::init() malloc @ %p %d bytes \n", pgfxbuffer, bts);
#endif

  if (pgfxbuffer != nullptr) {
    // Clear full scr ( ? )
    memset(pgfxbuffer, colorBackground & 0xFF, bts);

    data_sf =
        SDL_CreateRGBSurfaceFrom((uint8_t *)pgfxbuffer, w, h, 32, w * 4,
                                 sdlmask32(0), sdlmask32(1), sdlmask32(2), 0);
  } else {
    printf("OsciChannel:: WARN! malloc ! *\n");
  }

  // Fill background
  /*
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      setpxl(x, y, colorBackground);
    }
  }
  */

  for (int16_t x = 0; x < w; x++) {
    setpxl(x, 0, colorFrame);
    setpxl(x, h - 1, colorFrame);
  }

  for (int16_t y = 0; y < h; y++) {
    setpxl(0, y, colorFrame);
    setpxl(w - 1, y, colorFrame);
  }
}

void OsciChannel::deinit() {
  if (pgfxbuffer != nullptr) {
    free(pgfxbuffer);
    pgfxbuffer = nullptr;
#ifdef DEBUG_OSCI
    printf("OsciChannel::deinit() buffer deallocated \n");
#endif
  }
}

void OsciChannel::render(int16_t *parr) {

  for (int i = 0; i < w; i++) {

    // restore previous
    uint8_t v1 = (255 - (127 + (abuff[i] / 256))) >> 1;
    setpxl(i, v1, colorBackground);

    abuff[i] = parr[i];

    // fill new
    uint8_t v2 = (255 - (127 + (abuff[i] / 256))) >> 1;
    setpxl(i, v2, colorValue);
  }
}
