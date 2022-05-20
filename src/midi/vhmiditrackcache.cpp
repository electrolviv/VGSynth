#include "vhmiditrackcache.hpp"

VHMIDI::TrackCache::TrackCache():
    idx(0),
    trkno(0),
    finflag(0),

    fileblockalen(0),
    fileblockapos(0),

    fileblockblen(0),
    fileblockbpos(0)

{
    fileblocka[0] = 0;
    fileblockb[0] = 0;
}

VHMIDI::TrackCache::~TrackCache() { }

void VHMIDI::TrackCache::init(uint8_t trkn) {
    trkno       = trkn;
    idx         = 0;
    finflag     = 0;
    loadblock(idx);
}


void VHMIDI::TrackCache::loadblock(uint8_t i) {

    void    *pdst   = (!idx) ? fileblocka : fileblockb;
    uint8_t *pcnt   = (!idx) ? &fileblockalen : &fileblockblen;
    uint8_t *ppos   = (!idx) ? &fileblockapos : &fileblockbpos;

    BRIDGE_VHMIDI_READBLOCK(trkno, pdst, lim, pcnt, &finflag );
    *ppos = 0;
}

bool VHMIDI::TrackCache::getbyte(uint8_t *pb) {

    uint8_t     *pdst  = (!idx) ?  fileblocka    :  fileblockb    ;
    uint8_t     *ppos  = (!idx) ? &fileblockapos : &fileblockbpos ;
    uint8_t     *plim  = (!idx) ? &fileblockalen : &fileblockblen ;

    if( *ppos < *plim ) {
        *pb = *(pdst + (*ppos));
        (*ppos)++;

        if( (*ppos) == (*plim) ) {

            if(finflag)
                return false;

            idx ^=1;
            loadblock(idx);
        }

        return true;
     }

    return false;
}


