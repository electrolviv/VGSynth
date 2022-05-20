#pragma once

#include <stdint.h>

class VHSinTblInit {

    public:

        VHSinTblInit();
        ~VHSinTblInit();

    private:

};

class VHSinTbl {
public:
    static int16_t GetValue(int angle);
};
