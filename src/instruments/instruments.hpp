#pragma once

#include <stdint.h>

struct stInstrBlock {
  /* uint8_t space[16]; */
};

struct stInstrument {
    uint8_t        gen_sig_type;
    stInstrBlock   blk[16];
};


extern stInstrument globalInstruments[1024];

