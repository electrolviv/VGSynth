#include    "defs.hpp"
#include    "fsys/vhfile.hpp"


namespace VHMIDI {

static VHFile       ofile;


bool    BRIDGE_VHMIDI_INITFILE(const char *fname) {

    uint32_t fsize;

    if( fname == nullptr) return false; // // no file given
    if(!ofile.GetFileSize(fname, &fsize)) return false; // check file
    return ofile.SafeOpen(fname, VHFile::eMode::read_mode);
}


bool    BRIDGE_VHMIDI_READFILE(uint32_t offs, uint32_t cnt, void *pdst, uint32_t *pcnt) {
    ofile.Seek(offs);
    return ofile.Read(pdst, cnt);
}

bool    BRIDGE_VHMIDI_READBLOCK(
    uint8_t     trk,
    void     *  pdst,
    uint8_t     cnt,
    uint8_t  *  prdcnt,
    uint8_t  *  pfinflag ) {
    return false;
}

}
