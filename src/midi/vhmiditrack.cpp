#include <stdio.h>
#include "vhmidicmd.hpp"
#include "vhmiditrack.hpp"


namespace VHMIDI {


static uint8_t      tmpbuff[1024];

Track::Track(uint8_t n, stMIDITrackProps *props) {

    trkn        = n;
    pprops      = props;
    pos         = 0;

    cachedbyte  = 0;
    usecache    = false;

    cache.init(n);
}

Track::~Track()
{

}

bool Track::passtrack()
{
    bool        cmdok;

    pos = 0;

    while(pos < pprops->len) {

        uint8_t pfx;

        cmdok = false;

        // Time-offset
        if(!gettrackmval(&midipack.offstime)) break;
        if(!gettrackbyte(&pfx)) break;

        if(pfx>=0x80) midipack.lastpfx = pfx;

        if(      pfx == MIDI_CMD_SYSEXSTART )  { if(!cmdtrack_sysexstart())  break; }
        else if( pfx == MIDI_CMD_SYSEXSTOP )   { if(!cmdtrack_sysexstop())   break; }
        else if( pfx == MIDI_CMD_SYSMETA )     { if(!cmdtrack_meta())        break; }
        else  {
            // roll-back
            if( pfx < 0x80 ) { usecache=true; }  if(!cmdtrack_evt()) break;
        }

        cmdok = true;
    }

    if(!cmdok) {
        printf("MIDI Protocol error pfx=%x\n", midipack.lastpfx );
    }

    return cmdok;

}

bool Track::gettrackbyte(uint8_t *pbyte) {

    if(usecache) {
        usecache = false;
        return cachedbyte;
    }

    return cache.getbyte(pbyte);
}

bool Track::gettrackmval(uint16_t * pval) {

    uint16_t r = 0;
    uint8_t b1, b2, b3;

    if(!gettrackbyte(&b1)) return false;
    r |= b1 & 0x7F; if(b1<0x80) { *pval = r; return true; }
    if(!gettrackbyte(&b2)) return false;
    r <<= 7; r |= b2 & 0x7F; if(b2<0x80) { *pval = r; return true; }
    if(!gettrackbyte(&b3)) return false;
    r <<= 7; r |= b3 & 0x7F; *pval = r;

    return true;
}

bool Track::gettrackbin(uint8_t *pdst, uint16_t sz) {

    for(uint16_t i=0;i<sz;i++)
        if(!gettrackbyte(pdst+i))
            return false;

    return true;
}


//uint32_t Track::gettrackoffs()  {
//    return offset;
//}

//uint32_t Track::gettracklen()   {
//    return length;
//}

////     printf("Track #%i\n", n);

bool Track::cmdtrack_sysexstart()  { // 0xF0

    uint16_t    metasize;
    if(!gettrackmval(&metasize)) return false;
    if(!gettrackbin( tmpbuff, metasize )) return false;

    return true;
}

bool Track::cmdtrack_sysexstop() {

    return false;
}

bool Track::cmdtrack_meta() { // 0xFF

    if(!gettrackbyte(&midipack.metatype)) return false;
    if(!gettrackmval(&midipack.metasize)) return false;
    if(!gettrackbin( tmpbuff, midipack.metasize )) return false;

    // printf("META type=%X size=%X\n", midipack.metatype, midipack.metasize);

    // Check
    bool r = true;

    switch(midipack.metatype) {

    case MIDI_META_TRACKNAME:   logmeta("META-TRACKNAME " ); break;
    case MIDI_META_LYRIC:       logmeta("META-LYRIC     " ); break;
    case MIDI_META_SMPTE:       logmeta("META-SMPTE     " ); break;
    case MIDI_META_PFX:         logmeta("META-PREFIX    " ); break;
    case MIDI_META_PORT:        logmeta("META-PORT      " ); break;
    case MIDI_META_TIMESIGNAT:  logmeta("META-TIMESIGN  " ); break;
    case MIDI_META_KEYSIGNAT:   logmeta("META-KEYSSIGN  " ); break;
    case MIDI_META_TEMPO:       logmeta("META-TEMPO     " ); break;
    case MIDI_META_TEXTMARKER:  logmeta("TEXT MARKER    " ); break;
    case MIDI_META_ENDTRK:      logmeta("META-ENDTRK    " ); break;

    default: {
        printf("META Unknown: %X\n", midipack.metatype);
        r = false;
    }
    }

    return r;
}

bool Track::cmdtrack_evt() {

    midievt.evt = midipack.lastpfx;
    uint8_t cmd = midievt.evt >> 0x04;
    uint8_t chn = midievt.evt &  0x0F;

    if(!event_readparams(&midievt))
        return false;

    switch(cmd) {

    case MIDI_EVT_NOTEOFF:      { logevent("NOTEOFF   ");    } break;
    case MIDI_EVT_NOTEON:       { logevent("NOTEON    ");    } break;
    case MIDI_EVT_CHANPOLY:     { logevent("CHANPOLY  ");    } break;
    case MIDI_EVT_CTRL:         { logevent("CHNCTRL   ");    } break;
    case MIDI_EVT_PROGCHANGE:   { logevent("PRGCHANGE ");    } break;
    case MIDI_EVT_AFTERTOUCH:   { logevent("ARTOUCH   ");    } break;
    case MIDI_EVT_WHEEL:        { logevent("WHEEL     ");    } break;
    default: { return false; }

    }

    return true;
}

bool Track::event_readparams(stchunkevt * pevt) {

    uint8_t params_count = calc_params_count(pevt->evt);

    if(params_count == 2) {
        if(!gettrackbyte( & pevt->b2)) return false;
        if(!gettrackbyte( & pevt->b3)) return false;
    } else if(params_count == 1) {
        if(!gettrackbyte( & pevt->b2)) return false;
    } else {
        return false;
    }

    return true;
}


void Track::logmeta(const char *pfx) {
    uint8_t *pstr = tmpbuff;
    uint16_t sz = midipack.metasize;
    *(pstr + sz) = 0;
    printf("LOGMIDI: %s %s\n", pfx, pstr);
}

void Track::logevent(const char *pfx) {
    uint8_t parcnt= calc_params_count(midievt.evt);
    printf("@ %.6d LOGEVENT: %s ", midipack.offstime, pfx);
    if(parcnt == 2)         { printf("%.2d %.2d\n", midievt.b2, midievt.b3); }
    else if(parcnt == 1)    { printf("%.2d\n", midievt.b2);}
}



/*

// bool lookuptrkbyte(uint8_t trkn, uint8_t *pbyte); // check only

bool VHMIDIFile::lookuptrkbyte(uint8_t trkn, uint8_t *pbyte) {
    uint32_t pos = trackoffs[trkn] + trackpos[trkn];
    ofile.Seek(pos);
    bool r = ofile.Read(pbyte, 1);
    return r;
}
*/

//void Track::init(uint32_t offs, uint32_t len) { offset = offs; length = len; enabled = true; }

//void Track::rewind() { pos = 0; usecache = false; }

//void Track::prevbyte() { usecache = true; }


}
