#include <AL/al.h>
#include <AL/alc.h>

#include "defs.hpp"
#include <string>


#include "midi/vhmidifile.hpp"


#include "notes.hpp"
#include "patterns.hpp"


#include "rendersample.hpp"
#include "audiodrv.hpp"


#include "wnd/masterchannel.hpp"

#include "runtime/keyboard.hpp"

#define INRANGE(V, F, T) ((V >= F) && (V <= T))

int16_t *srcbuff = nullptr;
int16_t     *   scr_GetAudBuffPtr() { return srcbuff; }

enum eAction { eActionOk = 0, eActionErr, eActionExitApp };

static eAction HandleEvent(const SDL_Event *pevent) {

  auto etype = pevent->type;
  eAction ret = eActionOk;

  if (INRANGE(etype, SDL_MOUSEMOTION, SDL_MOUSEWHEEL)) {

  } else if (etype == SDL_QUIT) {

    ret = eActionExitApp;

  } else if (etype == SDL_WINDOWEVENT) {

    if (pevent->window.event == SDL_WINDOWEVENT_CLOSE)
      ret = eActionExitApp;

  } else if (etype == SDL_KEYDOWN) {
    uint8_t k = pevent->key.keysym.sym;
    printf("key %d !\n", k);
    gKeyboard.put(k);
  }

  if (ret == eActionErr)
    printf("Unknown event type = %X\n", etype);

  return ret;
}

static void AudioLoop() {

  SDL_Event event;
  int bufflen;
  int16_t *pbuff;
  bool flagloop = true;

  // Initialize Audio driver
  globalAudioDriver.Init();

  while (flagloop) {

    // Fill audio buffer
    if (globalAudioDriver.IsDataNeed()) {
      pbuff = globalAudioDriver.GetBufferPtr(&bufflen);
      srcbuff = pbuff;
      RenderBuffer(pbuff, bufflen);
      globalAudioDriver.ResetFlagDataNeed();
    }

    SDL_Delay(1);

    // Check exit window
    if (SDL_PollEvent(&event)) {
      eAction r = HandleEvent(&event);
      if (r == eActionExitApp)
	flagloop = false;
    }
  }
}

int main() {

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  setbuf(stdout, NULL);

  WNDMasterChannel wndMasterChannel;
  wndMasterChannel.Init();

  InitPatterns();

  SDL_Delay(100);

  AudioLoop();

  wndMasterChannel.forceexit();
  SDL_Delay(100);
  SDL_DestroyWindow(wndMasterChannel.window);

  SDL_Quit();
  printf("Done !\n");

  return 0;
}
