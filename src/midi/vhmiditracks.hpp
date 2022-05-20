#pragma once

#include "vhmididefs.hpp"
#include "vhmiditrack.hpp"

namespace VHMIDI {

class Tracks {

    public:

        Tracks();
        ~Tracks();

        void        inittrack(int n, uint32_t offs, uint32_t len);
        uint32_t    gettrackoffset(int n);
        uint32_t    gettracklength(int n);

        bool        passtrack(int n);

    private:

        Track   trk[VHMIDI_MAXTRACKS];

};

}
