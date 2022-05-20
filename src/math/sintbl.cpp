#include "sintbl.hpp"
#include <math.h>

#define     PI 3.14159265


// SINTable  1024 x 0xFFFF
static bool         sintblDone = false;
static int16_t      sintbl[1024];
static VHSinTblInit sintlInit;


VHSinTblInit::VHSinTblInit()
{
    if(!sintblDone) {

    // Make SIN
    for(int i=0;i<1024;i++) {
        sintbl[i] = int((sin((i * PI * 2) / 1024) * 32767) );
    }

    sintblDone = true;
    }

}

VHSinTblInit::~VHSinTblInit()
{

}


int16_t VHSinTbl::GetValue(int angle) {
    return sintbl[angle&0x3FF];
}
