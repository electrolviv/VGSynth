#pragma once

// int    BMP                  = 110;      // BMP * 16 @ minute @ 60 sec

class RuntimeClock {

public:

    int    runtime_bmp_clock    = 0;
    int    bmp_limit            = 8000;

    void   ClkTick() {
        runtime_bmp_clock = (runtime_bmp_clock >= bmp_limit) ? 0 : (runtime_bmp_clock+1);
    }
};

extern RuntimeClock gRuntimeClock;
