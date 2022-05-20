#pragma once

#include "defs.hpp"
#include "vhmididefs.hpp"
#include "vhmiditracks.hpp"


namespace VHMIDI {

class VHMIDIFile {

    public:

        VHMIDIFile(char * cstrfilename = nullptr);
        ~VHMIDIFile();
        bool Init(stMIDITrackProps *parr);

        stMIDIHeader  midihdr;

    private:

        bool        checkhdr();
        bool        checktrack(uint32_t foffs, stMIDITrackProps *pdst);
        char  *     strFileName;
};

}

extern VHMIDI::VHMIDIFile           gMIDIFile;
