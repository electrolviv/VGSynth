#pragma once

#include "defs.hpp"
#include "sintbl.hpp"

class EffectVolumeDrift {

public:
  EffectVolumeDrift();
  void Set(uint16_t fangle, uint16_t radius, uint16_t startdelay);
  void Press();
  void Tick();
  bool IsEnabled();
  int16_t GetDriftValue();

private:
  bool _enabled;

  uint16_t _fangle;
  uint16_t _radius;
  uint16_t _pretimer;

  uint16_t _rtimer;
  uint32_t _rangle;

  uint8_t _divider;
};
