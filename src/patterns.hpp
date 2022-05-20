#pragma once

#include <stdint.h>
#include "notes.hpp"

class VHAudioPattern {

public:
    uint16_t    notes[16];
};


class VHAudioPatterns {

    public:

        VHAudioPatterns() {

            _patternsCount = 0;

            runtimePatternNumber = 0;
            runtimePatternPos    = 0;

            for(int i=0;i<256;i++) { patterns[i] = nullptr; }
        }

        ~VHAudioPatterns() { }

        void        Add(uint16_t *pPattern)     {
            Set(_patternsCount, pPattern);
            _patternsCount++;
        }

        void        Set(int idx, uint16_t *ptr) { patterns[idx] = ptr;  }
        uint16_t *  Get(int idx)                { return patterns[idx]; }
        int         patternsCount() { return _patternsCount; }

        // Runtime
        int         runtimePatternNumber;
        int         runtimePatternPos;

        void        NextPos() {

            runtimePatternPos++;
            if(runtimePatternPos >= 16) {
                runtimePatternPos = 0;

                runtimePatternNumber++;
                if(runtimePatternNumber>=_patternsCount)
                    runtimePatternNumber = 0;
            }
        }

     private:

        uint16_t    * patterns[256];
        int           _patternsCount;
};

extern VHAudioPatterns     patterns;

void InitPatterns();
