#pragma once

#include <stdint.h>

#include "math/sintbl.hpp"

#include "channel.hpp"

void RenderBuffer(int16_t *presult, int buflen);
void MakeBufferTest(int16_t *parr, uint16_t len, int16_t note);
