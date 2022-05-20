#pragma once

#include <stdint.h>


class VHAudioDriver {

    public:

        VHAudioDriver();
        ~VHAudioDriver();

        bool            Init();
        void            Deinit();

        bool            IsDataNeed();
        void            ResetFlagDataNeed();
        int16_t *       GetBufferPtr(int *plen);

        void            Thread();

    private:


        bool            flagInit;
        bool            flagExit;
        bool            flagFillData;
        int             buffidx;

};

extern VHAudioDriver globalAudioDriver;
