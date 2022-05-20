#pragma once

#include <stdint.h>

#define VHMIDI_MAXTRACKS    16

// move byte 3 to byte 0
// move byte 1 to byte 2
// move byte 2 to byte 1
// byte 0 to byte 3

#define MIDItoMSB16(num)  ( (num << 8) | (num >> 8 ) )

#define MIDItoMSB32(num) \
    ( ( (num >> 24) & 0xff      )   |   \
    ( (num << 8 ) & 0xff0000    )   |   \
    ( (num >> 8 ) & 0xff00      )   |   \
    ( (num << 24) & 0xff000000  ) )


namespace VHMIDI {

struct stMIDIChunk {
    uint8_t     type[4];
    uint32_t    length;
};

struct stMIDIHeader {
    uint16_t format;
    uint16_t tracks;
    uint16_t division;
};

struct stmidipack {
    uint16_t    offstime;
    uint8_t     lastpfx;
    uint8_t     metatype;
    uint16_t    metasize;
};

struct stchunkevt {
    uint8_t evt;
    uint8_t b2;
    uint8_t b3;
    uint8_t res;
};

struct stMIDITrackProps {
    uint32_t offs;
    uint32_t len;
};

uint8_t      calc_params_count(uint8_t evt);

bool    BRIDGE_VHMIDI_INITFILE(const char *fname);

bool    BRIDGE_VHMIDI_READFILE(
    uint32_t    offs,
    uint32_t    cnt,
    void     *  pdst,
    uint32_t *  pcnt);

bool    BRIDGE_VHMIDI_READBLOCK(
    uint8_t     trk,
    void     *  pdst,
    uint8_t     cnt,
    uint8_t  *  prdcnt,
    uint8_t  *  pfinflag );

uint16_t    pckeyfreq(uint16_t keycode);

}

extern VHMIDI::stMIDITrackProps     gTracksMeta[VHMIDI_MAXTRACKS];
