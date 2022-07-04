#include <AL/al.h>
#include <AL/alc.h>

#include "defs.hpp"
#include <string>

#include "midi/vhmidifile.hpp"

#include "notes.hpp"
#include "patterns.hpp"


#include "rendersample.hpp"
#include "audiodrv.hpp"

#include "sigsrc.hpp"

#include "wnd/masterchannel.hpp"

#include "runtime/keyboard.hpp"

WNDMasterChannel wndMasterChannel;

int16_t *srcbuff = nullptr;
int scrbufl;

static uint8_t runtimeForm = eSigForm_SIN;

int16_t *scr_GetAudBuffPtr() { return srcbuff; }
int scr_GetAudBuffLen() { return scrbufl; }

enum eAction { eActionOk = 0, eActionErr, eActionExitApp };

static void HandleEventkey(uint8_t key) {

  if (INRANGE(key, SDL_SCANCODE_F1, SDL_SCANCODE_F12)) {
    wndMasterChannel.optsSetSelected(key - SDL_SCANCODE_F1);
  } else if (key == SDL_SCANCODE_KP_MINUS) {
    bool ovf = (runtimeForm == eSigForm_First);
    runtimeForm = ovf ? (eSigForm_Last - 1) : (runtimeForm - 1);
    wndMasterChannel.setNewForm(runtimeForm);
  } else if (key == SDL_SCANCODE_KP_PLUS) {
    bool ovf = (runtimeForm == (eSigForm_Last - 1));
    runtimeForm = ovf ? eSigForm_First : (runtimeForm + 1);
    wndMasterChannel.setNewForm(runtimeForm);
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

void testcase_000_sintable() {
  printf("TESTCASE #0 : Sin Table\n");
  for (int i = 0; i < 4096; i++) {

    if (i == 3072) {
      asm("nop");
    }

    int16_t val = VHSigSrc::value(eSigForm_SIN, i);
    printf("Angle : %d Value : %d\n", i, val);
  }
  exit(0);
}


int main() {

  VHSigSrc::Init();

  // testcase_000_sintable();

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  setbuf(stdout, NULL);

  TTF_Init();

  wndMasterChannel.Init();

  InitPatterns();

  SDL_Delay(100);

  AudioLoop();

  wndMasterChannel.forceexit();
  SDL_Delay(100);
  SDL_DestroyWindow(wndMasterChannel.window);

  TTF_Quit();

  SDL_Quit();
  printf("Done !\n");

  return 0;
}
