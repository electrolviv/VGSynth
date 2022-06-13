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

void WNDMasterChannel::textout() {


  // this is the color in rgb format,
  // maxing out all would give you the color white,
  // and it will be your text's color
  SDL_Color White = {255, 255, 255};

  // this opens a font style and sets a size
  // "Sans.ttf";
  const char *strfile =
      "/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf";
  // const char *strfile = "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf";

  TTF_Font *font = TTF_OpenFont(strfile, 32);

  // as TTF_RenderText_Solid could only be used on
  // SDL_Surface then you have to create the surface first
  SDL_Surface *surfaceMessage =
      TTF_RenderText_Solid(font, "Multichannel", White);

  // now you can convert it into a texture
  SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  SDL_Rect Message_rect; // create a rect
  Message_rect.x = 0;    // controls the rect's x coordinate
  Message_rect.y = 620;  // controls the rect's y coordinte
  Message_rect.w = 200;  // controls the width of the rect
  Message_rect.h = 34;   // controls the height of the rect

  // (0,0) is on the top left of the window/screen,
  // think a rect as the text's box,
  // that way it would be very simple to understand

  // Now since it's a texture, you have to put RenderCopy
  // in your game loop area, the area where the whole code executes

  // you put the renderer's name first, the Message,
  // the crop size (you can ignore this if you don't want
  // to dabble with cropping), and the rect which is the size
  // and coordinate of your texture
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

  // Don't forget to free your surface and texture
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
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

  textout();
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

  // Init channels
  for (int i = 0; i < 16; i++)
    ochannel[i].init();

  // Loop
  while (!flagExit) {

    for (int j = 0; j < 4; j++) {

      for (int i = 0; i < 4; i++) {

	int idx = (j * 4) + i;

	// Render channels
	SDL_Rect dstrect = {i * 260, j * 130, 256, 128};

	// int16_t *pabuff = scr_GetAudBuffPtr();
	int16_t *pabuff = abuffers[idx].buff;

	if (pabuff != nullptr) {

	  ochannel[idx].render(pabuff);

	  SDL_Surface *psurface = ochannel[idx].GetSurface();
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

void WNDMasterChannel::optsSetSelected(int idx) {
  optsIndexSelected = idx;
  optsRepaintNeed = true;
}

void WNDMasterChannel::setNewForm(uint8_t newform) {
  printf("New FORM=%d\n", newform);
  for (int i = 0; i < AUDIO_CHN_CNT; i++) {
    channels[i].setSigForm(newform);
  }
}
