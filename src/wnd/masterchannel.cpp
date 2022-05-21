#include "masterchannel.hpp"

#include "channel.hpp"

#define strCAPTION "VGSynth V0.1"

extern  VHAudioChannel   channels[DEF_AUDIO_CHN_CNT];


SDL_Thread *thread;
static pthread_t thr;

int16_t * scr_GetAudBuffPtr();


static void  *  _run(void *parg) {
  return static_cast<WNDMasterChannel *>(parg)->task(nullptr);
}

void WNDMasterChannel::Init() {

  if (pthread_create(&thr, nullptr, _run, this) != 0) { // _run
    printf("WNDMasterChannel::Init() ctreate_thread failed\n");
    exit(0);
  }
}

void WNDMasterChannel::forceexit()     { flagExit = true; }

void WNDMasterChannel::RenderGFXBuffer(VGRect *prect) {

  int16_t *parr = scr_GetAudBuffPtr();
  if (parr == nullptr)
    return;

  for (int i = 0; i < prect->w; i++) {

    uint8_t v = 255 - (127 + (parr[i] / 256));

    // Set pixel
    gfxbuffer[(v * WNDWIDTH) + i] = 0xFF00;
  }
}

void WNDMasterChannel::RenderChannelsState() {

  for (int i = 0; i < DEF_AUDIO_CHN_CNT; i++) {
    auto keystate = channels[i].voiceBase.keyState;
    bool chnon = (keystate != VHAudioChannel::nKeyState_Off);
    Uint8 r = chnon ? 255 : 55;
    Uint8 g = chnon ? 255 : 55;
    Uint8 b = chnon ? 255 : 55;
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);

    int ypos = i * 16;
    SDL_RenderDrawLine(renderer, 2, ypos, 8, ypos + 10);
  }
  SDL_RenderPresent(renderer);
}

void *WNDMasterChannel::task(void *parg) {

  if (parg == nullptr)
    printf("task_scr arg=nullptr\n");

  window = SDL_CreateWindow(strCAPTION, SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, WNDWIDTH, WNDHEIGHT, 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);

  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  SDL_Surface *data_sf = SDL_CreateRGBSurfaceFrom(
      (uint8_t *)gfxbuffer, WNDWIDTH, WNDHEIGHT, 32, WNDWIDTH * 4, sdlmask32(0),
      sdlmask32(1), sdlmask32(2), 0);

  SDL_Surface *screen = SDL_GetWindowSurface(window);
  SDL_Rect rectscr = {0, 0, WNDWIDTH, WNDHEIGHT};

  while (!flagExit) {

    // Clear full scr ( ? )
    memset(gfxbuffer, 0x00, WNDWIDTH * WNDHEIGHT * 4);

    VGRect chnrect = {0, 0, 256, 256};
    RenderGFXBuffer(&chnrect);

    // copy bitmap to the screen
    if (SDL_BlitScaled(data_sf, nullptr, screen, &rectscr) == 0)
      SDL_UpdateWindowSurface(window);

    RenderChannelsState();

    // 50/20 Hz
    SDL_Delay(50); // 20
  }

  flagExit = true;
  return nullptr;
}
