#include "vhmidifreq.hpp"

#include "notes.hpp"


static uint16_t  midifreqs[] = {

    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100,

    NOTE_C  /16,   // 12
    NOTE_Cd /16,   // 13
    NOTE_D  /16,   // 14
    NOTE_Dd /16,   // 15
    NOTE_E  /16,   // 16
    NOTE_F  /16,   // 17
    NOTE_Fd /16,   // 18
    NOTE_G  /16,   // 19
    NOTE_Gd /16,   // 20
    NOTE_A  /16,   // 21
    NOTE_Ad /16,   // 22
    NOTE_B  /16,   // 23

    NOTE_C  /8,    // 24
    NOTE_Cd /8,    // 25
    NOTE_D  /8,    // 26
    NOTE_Dd /8,    // 27
    NOTE_E  /8,    // 28
    NOTE_F  /8,    // 29
    NOTE_Fd /8,    // 30
    NOTE_G  /8,    // 31
    NOTE_Gd /8,    // 32
    NOTE_A  /8,    // 33
    NOTE_Ad /8,    // 34
    NOTE_B  /8,    // 35

    NOTE_C  /4,    // 36
    NOTE_Cd /4,    // 37
    NOTE_D  /4,    // 38
    NOTE_Dd /4,    // 39
    NOTE_E  /4,    // 40
    NOTE_F  /4,    // 41
    NOTE_Fd /4,    // 42
    NOTE_G  /4,    // 43
    NOTE_Gd /4,    // 44
    NOTE_A  /4,    // 45
    NOTE_Ad /4,    // 46
    NOTE_B  /4,    // 47

    NOTE_C  /2,    // 48
    NOTE_Cd /2,    // 49
    NOTE_D  /2,    // 50
    NOTE_Dd /2,    // 51
    NOTE_E  /2,    // 52
    NOTE_F  /2,    // 53
    NOTE_Fd /2,    // 54
    NOTE_G  /2,    // 55
    NOTE_Gd /2,    // 56
    NOTE_A  /2,    // 57
    NOTE_Ad /2,    // 58
    NOTE_B  /2,    // 59

    NOTE_C,        // 60,
    NOTE_Cd,       // 61,
    NOTE_D,        // 62,
    NOTE_Dd,       // 63,
    NOTE_E,        // 64,
    NOTE_F,        // 65,
    NOTE_Fd,       // 66,
    NOTE_G,        // 67,
    NOTE_Gd,       // 68,
    NOTE_A,        // 69, A4 (!)
    NOTE_Ad,       // 70,
    NOTE_B,        // 71,

    NOTE_C  *2,    // 72
    NOTE_Cd *2,    // 73
    NOTE_D  *2,    // 74
    NOTE_Dd *2,    // 75
    NOTE_E  *2,    // 76
    NOTE_F  *2,    // 77
    NOTE_Fd *2,    // 78
    NOTE_G  *2,    // 79
    NOTE_Gd *2,    // 80
    NOTE_A  *2,    // 81
    NOTE_Ad *2,    // 82
    NOTE_B  *2,    // 83

    NOTE_C  *4,    // 84
    NOTE_Cd *4,    // 85
    NOTE_D  *4,    // 86
    NOTE_Dd *4,    // 87
    NOTE_E  *4,    // 88
    NOTE_F  *4,    // 89
    NOTE_Fd *4,    // 90
    NOTE_G  *4,    // 91
    NOTE_Gd *4,    // 92
    NOTE_A  *4,    // 93
    NOTE_Ad *4,    // 94
    NOTE_B  *4,    // 95

    NOTE_C  *8,    // 96
    NOTE_Cd *8,    // 97
    NOTE_D  *8,    // 98
    NOTE_Dd *8,    // 99
    NOTE_E  *8,    // 100
    NOTE_F  *8,    // 101
    NOTE_Fd *8,    // 102
    NOTE_G  *8,    // 103
    NOTE_Gd *8,    // 104
    NOTE_A  *8,    // 105
    NOTE_Ad *8,    // 106
    NOTE_B  *8,    // 107

    NOTE_C  *16,    // 108
    NOTE_Cd *16,    // 109
    NOTE_D  *16,    // 110
    NOTE_Dd *16,    // 111
    NOTE_E  *16,    // 112
    NOTE_F  *16,    // 113
    NOTE_Fd *16,    // 114
    NOTE_G  *16,    // 115
    NOTE_Gd *16,    // 116
    NOTE_A  *16,    // 117
    NOTE_Ad *16,    // 118
    NOTE_B  *16,    // 119
    1, 1, 1, 1, 1, 1, 1, 1

};

uint16_t MIDINoteFreq(uint8_t note) {
    return midifreqs[note&0x7F];
}
