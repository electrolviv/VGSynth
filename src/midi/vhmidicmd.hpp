#pragma once

/* */

#define     MIDI_CMD_SYSEXSTART     0xF0
#define     MIDI_CMD_SYSEXSTOP      0xF7
#define     MIDI_CMD_SYSMETA        0xFF

/* */

#define     MIDI_META_SEQUENCE      0x00
#define     MIDI_META_TEXT          0x01
#define     MIDI_META_COPYRIGHT     0x02
#define     MIDI_META_TRACKNAME     0x03
#define     MIDI_META_LYRIC         0x05
#define     MIDI_META_TEXTMARKER    0x06
#define     MIDI_META_PFX           0x20
#define     MIDI_META_PORT          0x21
#define     MIDI_META_ENDTRK        0x2F

#define     MIDI_META_TEMPO         0x51
#define     MIDI_META_SMPTE         0x54
#define     MIDI_META_TIMESIGNAT    0x58
#define     MIDI_META_KEYSIGNAT     0x59

/* */

#define     MIDI_EVT_NOTEOFF        0x08
#define     MIDI_EVT_NOTEON         0x09

#define     MIDI_EVT_CHANPOLY       0x0A
#define     MIDI_EVT_CTRL           0x0B
#define     MIDI_EVT_PROGCHANGE     0x0C
#define     MIDI_EVT_AFTERTOUCH     0x0D
#define     MIDI_EVT_WHEEL          0x0E
