#pragma once

#include "vhmididefs.hpp"
#include "vhmiditrackcache.hpp"


namespace VHMIDI {


class Track {

public:

    Track( uint8_t n, stMIDITrackProps * props);
    ~Track();

    void        init         (uint32_t offs, uint32_t len);
    void        rewind       ();
    void        prevbyte     ();

    bool        gettrackbyte (uint8_t *pbyte);
    bool        gettrackmval (uint16_t * pval);
    bool        gettrackbin  (uint8_t *pdst, uint16_t sz);

    bool        event_readparams(stchunkevt * pevt);

    uint32_t    gettrackoffs();
    uint32_t    gettracklen();
    bool        passtrack();

    uint8_t      cachedbyte;
    bool         usecache;

private:

    uint8_t                 trkn;
    stMIDITrackProps    *   pprops;
    uint32_t                pos;

    stmidipack      midipack;
    stchunkevt      midievt;
    TrackCache      cache;

    bool         cmdtrack_sysexstart    ();
    bool         cmdtrack_sysexstop     ();
    bool         cmdtrack_meta          ();
    bool         cmdtrack_evt           ();

    void         logmeta(const char * pfx);
    void         logevent(const char * pfx);

};

}
