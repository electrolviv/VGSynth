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

WNDMasterChannel wndMasterChannel;

int16_t *srcbuff = nullptr;
int scrbufl;

int16_t *scr_GetAudBuffPtr() { return srcbuff; }
int scr_GetAudBuffLen() { return scrbufl; }

enum eAction { eActionOk = 0, eActionErr, eActionExitApp };

static void HandleEventkey(uint8_t key) {

  if (INRANGE(key, 58, 68)) {
    int idx = key - 58;
    wndMasterChannel.optsSetSelected(idx);
  } else {
    gKeyboard.put(key);
  }
}

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
    HandleEventkey(pevent->key.keysym.sym);
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

      // GFX cached buffer params
      srcbuff = pbuff;
      scrbufl = bufflen;

      RenderAudioBuffer(pbuff, bufflen);

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
