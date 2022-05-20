#pragma once

#include "vhmididefs.hpp"

namespace VHMIDI {


class TrackCache {

public:

    TrackCache();
    ~TrackCache();

    void    init(uint8_t trkn);
    void    loadblock(uint8_t indx);
    bool    getbyte(uint8_t *pb);

private:

    static const    uint8_t lim = 64;

    uint8_t         fileblocka[lim];
    uint8_t         fileblockb[lim];

    uint8_t         fileblockapos;

    uint8_t         fileblockalen;

    uint8_t         fileblockblen;
    uint8_t         fileblockbpos;

    uint8_t         idx;
    uint8_t         trkno;
    uint8_t         finflag;
};

}
