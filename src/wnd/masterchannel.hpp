#pragma once

#include "wnd.hpp"

int16_t     *   scr_GetAudBuffPtr();

class WNDMasterChannel {

public:
  static const int WNDWIDTH = (264 * 4);
  static const int WNDHEIGHT = 128 * 6;

  WNDMasterChannel() {}
  ~WNDMasterChannel() {}

  void Init();
  void forceexit();

  SDL_Window *window = nullptr;

  void *task(void *parg);

  void optsSetSelected(int idx);

  void setNewForm(uint8_t newform);

private:
  bool flagExit = false;
  SDL_Renderer *renderer = nullptr;

  void RenderGFXBuffer(VGRect *prect);
  void RenderChannelsState();

  void PaintOptions();

  static const int optsw = 32;
  static const int optsh = 32;
  static const int optsSpacerX = 16;
  static const int optsSpacerY = 16;

  int optsIndexSelected = 0;
  bool optsRepaintNeed = true;

  bool optsSelected(int idx);
  void optsSetForeColor(int idx);
  void optsSetBackColor(int idx);

  void textout();
};
