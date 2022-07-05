#include "effect_vol_drift.hpp"

EffectVolumeDrift::EffectVolumeDrift()
    : _enabled(false), _fangle(0), _radius(0), _pretimer(0), _rtimer(0),
      _rangle(0), _divider(0) {}

void EffectVolumeDrift::Set(uint16_t fangle, uint16_t radius,
                            uint16_t startdelay) {
  _fangle = fangle;
  _radius = radius;
  _pretimer = startdelay;
  Press();
}

void EffectVolumeDrift::Press() {
  _rangle = 0;
  _divider = 0;
  _rtimer = _pretimer;
  _enabled = true;
}

void EffectVolumeDrift::Tick() {

  _divider++;
  _divider &= 0xF;

  if (!_divider) {

    if (_rtimer) {
      _rtimer--;
    } else {
      _rangle += _fangle;
    }
  }
}

bool EffectVolumeDrift::IsEnabled() {
  return _enabled; // Effect activated ?
}

int16_t EffectVolumeDrift::GetDriftValue() {
  int16_t r = _rtimer ? 0 : ((((int32_t)_radius) * sinfunc(_rangle)) / I16MAX);
  return r;
}
