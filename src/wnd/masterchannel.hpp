#pragma once

#include "rect.h"
#include "wnd.hpp"

int16_t     *   scr_GetAudBuffPtr();

class WNDMasterChannel {

public:
  static const int WNDWIDTH = 512;
  static const int WNDHEIGHT = 256;

  WNDMasterChannel() {}
  ~WNDMasterChannel() {}

  void Init();
  void forceexit();

  SDL_Window *window = nullptr;

  void *task(void *parg);

private:
  bool flagExit = false;
  uint32_t gfxbuffer[WNDWIDTH * WNDHEIGHT] = {0};
  SDL_Renderer *renderer = nullptr;

  void RenderGFXBuffer(VGRect *prect);
  void RenderChannelsState();
};
