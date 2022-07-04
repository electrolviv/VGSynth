#include "effect_freq_drift.hpp"

EffectFrequencyDrift::EffectFrequencyDrift()
    : _enabled(false), _fangle(0), _radius(0), _pretimer(0), _rtimer(0),
      _rangle(0), _divider(0) {}

void EffectFrequencyDrift::Set(uint16_t fangle, uint16_t radius,
                               uint16_t startdelay) {
  _fangle = fangle;
  _radius = radius;
  _pretimer = startdelay;
  Press();
}

void EffectFrequencyDrift::Press() {
  _rangle = 0;
  _divider = 0;
  _rtimer = _pretimer;
  _enabled = true;
}

void EffectFrequencyDrift::Tick() {

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

bool EffectFrequencyDrift::IsEnabled() {
  return _enabled; // Effect activated ?
}

int16_t EffectFrequencyDrift::GetDriftValue() {
  int16_t r = _rtimer ? 0 : ((((int32_t)_radius) * sinfunc(_rangle)) / I16MAX);
  return r;
}
