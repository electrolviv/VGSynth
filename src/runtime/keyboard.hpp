#pragma once

#include "stdint.h"

class VHKeyboard {

public:

    VHKeyboard(): head(0), tail(0) { }

    ~VHKeyboard() { }

    void put(uint8_t k)   {
        buff[head] = k;
        head++;
        if(head>15) head = 0;
    }

    bool get(uint8_t *pk) {
        if(head==tail) return false;
        *pk = buff[tail];
        tail++; if(tail>15) tail = 0;
        return true;
    }

private:

    uint8_t head;
    uint8_t tail;
    uint8_t buff[16];

};

extern VHKeyboard gKeyboard;
