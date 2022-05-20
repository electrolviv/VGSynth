#include "vhmididefs.hpp"
#include "notes.hpp"

namespace VHMIDI {

static const uint8_t evt_params_count[8] = {
    2, // 8x
    2, // 9x
    2, // Ax
    2, // Bx
    1, // Cx
    1, // Dx
    2, // Ex
    0  // F - invalid !
};

uint8_t calc_params_count(uint8_t evt) {
    uint8_t idx = (evt>>4)&7;
    return evt_params_count[idx];
}


uint16_t    midi_pckey[] = {

    'z', C__O2,
                    's', Cd_O2,
    'x', D__O2,
                    'd', Dd_O2,
    'c', E__O2,


    'v', F__O2,
                    'g', Fd_O2,
    'b', G__O2,
                    'h', Gd_O2,
    'n', A__O2,
                    'j', Ad_O2,
    'm', B__O2,


    // ------------------------

    'q', C__O3,
                    '2', Cd_O3,
    'w', D__O3,
                    '3', Dd_O3,
    'e', E__O3,


    'r', F__O3,
                    '5', Fd_O3,
    't', G__O3,
                    '6', Gd_O3,
    'y', A__O3,
                    '7', Ad_O3,
    'u', B__O3,


};

uint16_t    pckeyfreq(uint16_t keycode) {

    uint16_t r = B__O4;

    for(int i=0;i<(sizeof(midi_pckey)/2);i+=2) {

        if(keycode == midi_pckey[i])
        { r = midi_pckey[i+1]; break; }
    }

    return r;
}


}

//    // Check tracks syntax
//    for(uint8_t i=0;i<midihdr.tracks;i++) {
//        if(! tracks.passtrack(i))
//            return;
//    }
