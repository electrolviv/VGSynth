#include "ochannel.hpp"

#define DEBUG_OSCI 1

#define sdlmask32(X) 0xFF << (X * 8)

#define colorBackground 0x00
#define colorValue 0xFF00

OsciChannel::OsciChannel() {}

OsciChannel::~OsciChannel() { deinit(); }

void OsciChannel::init() {
  int bts = w * h * 4;
  pgfxbuffer = (uint32_t *)malloc(bts);

#ifdef DEBUG_OSCI
  printf("OsciChannel::init() malloc @ %p %d bytes \n", pgfxbuffer, bts);
#endif

  if (pgfxbuffer != nullptr) {
    // Clear full scr ( ? )
    memset(pgfxbuffer, colorBackground, w * h * 4);

    data_sf =
        SDL_CreateRGBSurfaceFrom((uint8_t *)pgfxbuffer, w, h, 32, w * 4,
                                 sdlmask32(0), sdlmask32(1), sdlmask32(2), 0);
  } else {
    printf("OsciChannel:: WARN! malloc ! *\n");
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
    pgfxbuffer[(v1 * w) + i] = colorBackground;

    abuff[i] = parr[i];

    // fill new
    uint8_t v2 = (255 - (127 + (abuff[i] / 256))) >> 1;
    pgfxbuffer[(v2 * w) + i] = colorValue;
  }
}
