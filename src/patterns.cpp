#include "patterns.hpp"


VHAudioPatterns     patterns;

uint16_t    pattern_mi3[16] = {
    A__OB1, C__OB2, B__OB1, C__OB2,
    A__OB1, C__OB2, B__OB1, C__OB2,
    A__OB1, C__OB2, B__OB1, C__OB2,
    D__OB2, C__OB2, B__OB1, C__OB2 };

uint16_t    pattern_mi[16] = {
    A__O1, C__O2, B__O1, C__O2,
    A__O1, C__O2, B__O1, C__O2,
    A__O1, C__O2, B__O1, C__O2,
    D__O2, C__O2, B__O1, C__O2 };

uint16_t    pattern_mi2[16] = {
    A__O2, C__O3, B__O2, C__O3,
    A__O2, C__O3, B__O2, C__O3,
    A__O2, C__O3, B__O2, C__O3,
    D__O3, C__O3, B__O2, C__O3 };


uint16_t    pattern0[16] = {
    NOTE_C, NOTE_D, NOTE_E, NOTE_F,
    NOTE_D, NOTE_E, NOTE_F, NOTE_G,
    NOTE_E, NOTE_F, NOTE_G, NOTE_A,
    NOTE_D, NOTE_E, NOTE_F, NOTE_G };

uint16_t    pattern1[16] = {
    NOTE_C, NOTE_0, NOTE_E, NOTE_0,
    NOTE_G, NOTE_0, NOTE_E, NOTE_0,
    NOTE_G, NOTE_0, NOTE_F, NOTE_0,
    NOTE_D, NOTE_0, NOTE_D, NOTE_0 };

uint16_t    pattern2[16] = {
    NOTE_G, NOTE_0, NOTE_F, NOTE_0,
    NOTE_D, NOTE_0, NOTE_D, NOTE_0,
    NOTE_G, NOTE_0, NOTE_E, NOTE_0,
    NOTE_C, NOTE_0, NOTE_0, NOTE_0 };

uint16_t    pattern9[16] = {
    NOTE_A, 0, 0, 0,
    NOTE_A, 0, 0, 0,
    NOTE_A, 0, 0, 0,
    NOTE_A, 0, 0, 0 };

uint16_t    pattern8[16] = {
    NOTE_F, NOTE_E, NOTE_D, NOTE_C,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0 };


void InitPatterns() {

    patterns.Add(pattern8);
    patterns.Add(pattern9);
    patterns.Add(pattern_mi);
    patterns.Add(pattern_mi2);
    patterns.Add(pattern0);
    patterns.Add(pattern1);
    patterns.Add(pattern2);

}
