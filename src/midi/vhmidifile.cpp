#include "vhmidifile.hpp"
#include "string.h"

#include "vhmidicmd.hpp"


const char * strhdr = "MThd";
const char * strblk = "MTrk";

namespace VHMIDI {


VHMIDIFile::VHMIDIFile(char * cstrfilename) {
    midihdr.tracks = 0;
    strFileName = cstrfilename;
}

VHMIDIFile::~VHMIDIFile() { }

bool VHMIDIFile::Init(VHMIDI::stMIDITrackProps *parr ) {

    if(!BRIDGE_VHMIDI_INITFILE(strFileName))
        return false;

    if(!checkhdr())
        return false;

    // First track offset
    uint32_t foffs = sizeof(stMIDIChunk) + sizeof(stMIDIHeader);

    // Mark tracks
    for(int i=0;i<VHMIDI_MAXTRACKS;i++) {

        if(i<midihdr.tracks) {
            if(!checktrack(foffs, parr+i))
                return false;
            foffs += sizeof(stMIDIChunk) + (parr+i)->len;
        } else {
            parr->len = 0;
            parr->offs= 0;
        }
    }

    return true;
}


bool VHMIDIFile::checkhdr() {

    uint32_t        foffs = 0;
    uint32_t        rcnt; // IO file cnt
    stMIDIChunk     sChunk;


    if(!BRIDGE_VHMIDI_READFILE(foffs, sizeof(stMIDIChunk), &sChunk, &rcnt))
        return false;

    if(0 != memcmp(sChunk.type, strhdr, 4))
        return false;

    sChunk.length = MIDItoMSB32(sChunk.length);

    foffs += sizeof(stMIDIChunk);

    if(!BRIDGE_VHMIDI_READFILE(foffs, sizeof(midihdr), &midihdr, &rcnt))
        return false;

    midihdr.format   = MIDItoMSB16(midihdr.format);
    midihdr.tracks   = MIDItoMSB16(midihdr.tracks);
    midihdr.division = MIDItoMSB16(midihdr.division);

    return true;
}

bool VHMIDIFile::checktrack(uint32_t foffs,  stMIDITrackProps *pdst) {

    uint32_t        rcnt;
    stMIDIChunk     sChunk;

    if(!BRIDGE_VHMIDI_READFILE(foffs, sizeof(stMIDIChunk), &sChunk, &rcnt))
        return false;

    if(0!=memcmp(sChunk.type, strblk, 4))
        return false;

    pdst->offs = foffs + sizeof(stMIDIChunk);
    pdst->len  = MIDItoMSB32(sChunk.length);

    return true;
}


}
