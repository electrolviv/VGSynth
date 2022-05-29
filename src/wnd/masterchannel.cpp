#include "masterchannel.hpp"

#include "abuffer.hpp"
#include "channel.hpp"
#include "ochannel.hpp"

#define strCAPTION "VGSynth V0.1"

extern VHAudioChannel channels[AUDIO_CHN_CNT];
extern ABuffer abuffers[AUDIO_CHN_CNT];

SDL_Thread *thread;
static pthread_t thr;

int16_t * scr_GetAudBuffPtr();

OsciChannel ochannel[16];

static void *_run(void *parg) {
  return static_cast<WNDMasterChannel *>(parg)->task(nullptr);
}

void WNDMasterChannel::Init() {

  if (pthread_create(&thr, nullptr, _run, this) != 0) { // _run
    printf("WNDMasterChannel::Init() ctreate_thread failed\n");
    exit(0);
  }
}

void WNDMasterChannel::forceexit()     { flagExit = true; }


void WNDMasterChannel::RenderChannelsState() {

  for (int i = 0; i < AUDIO_CHN_CNT; i++) {
    auto keystate = channels[i].voiceBase.keyState;
    bool chnon = (keystate != VHAudioChannel::nKeyState_Off);
    Uint8 r = chnon ? 255 : 55;
    Uint8 g = chnon ? 255 : 55;
    Uint8 b = chnon ? 255 : 55;
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);

    int ypos = i * 16;
    SDL_RenderDrawLine(renderer, 2, ypos, 8, ypos + 10);
  }
}

void WNDMasterChannel::optsSetForeColor(int idx) {

  Uint8 r = optsSelected(idx) ? 255 : 55;
  Uint8 g = optsSelected(idx) ? 1 : 0;
  Uint8 b = optsSelected(idx) ? 1 : 0;

  SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void WNDMasterChannel::optsSetBackColor(int idx) {

  Uint8 r = optsSelected(idx) ? 155 : 100;
  Uint8 g = optsSelected(idx) ? 1 : 0;
  Uint8 b = optsSelected(idx) ? 1 : 0;

  SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void WNDMasterChannel::PaintOptions() {

  SDL_Rect rects[6];

  rects[0].x = 10;
  rects[0].y = 560;

  rects[0].w = optsw;
  rects[0].h = optsh;

  for (int i = 1; i < 6; i++) {
    rects[i].x = rects[0].x + (i * (optsw + optsSpacerX));
    rects[i].y = rects[0].y;
    rects[i].w = rects[0].w;
    rects[i].h = rects[0].h;
  }

  for (int i = 0; i < 6; i++) {
    optsSetBackColor(i);
    SDL_RenderFillRect(renderer, &rects[i]);
    optsSetForeColor(i);
    SDL_RenderDrawRect(renderer, &rects[i]);
  }
}

bool WNDMasterChannel::optsSelected(int idx) {
  return idx == optsIndexSelected;
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

  SDL_Surface *screen = SDL_GetWindowSurface(window);

  for (int i = 0; i < 16; i++)
    ochannel[i].init();

  // Loop
  while (!flagExit) {

    for (int j = 0; j < 4; j++) {

      for (int i = 0; i < 4; i++) {

	int idx = (j * 4) + i;

	// Render Channels : 0-15

	// VGRect srcrect = {0, 0, 256, 128};
	// VGRect chnrect = {i * 260, j * 130, 256, 128};
	// SDL_Rect destrect = {0, 0, WNDWIDTH, WNDHEIGHT};
	SDL_Rect dstrect = {i * 260, j * 130, 256, 128};

	// int16_t *pabuff = scr_GetAudBuffPtr();
	int16_t *pabuff = abuffers[idx].buff;

	if (pabuff != nullptr) {

	  ochannel[idx].render(pabuff);

	  SDL_Surface *psurface = ochannel[idx].GetSurface();

	  // copy bitmap to the screen
	  // if (SDL_BlitScaled(psurface, nullptr, screen, &rectscr) == 0)
	  //  SDL_UpdateWindowSurface(window);
	  SDL_BlitSurface(psurface, NULL, screen, &dstrect);
	}
      }
    }

    RenderChannelsState();

    if (optsRepaintNeed) {
      PaintOptions();
      optsRepaintNeed = false;
    }

    SDL_RenderPresent(renderer);

    // 50/20 Hz
    SDL_Delay(50); // 20
  }

  flagExit = true;
  return nullptr;
}
