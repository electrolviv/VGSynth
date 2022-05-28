#include "abuffer.hpp"
#include "stdint.h"

#include "channel.hpp"

#include "midi/vhmidifile.hpp"


#include "runtime/sysclock.hpp"
#include "runtime/keyboard.hpp"

#include <string>

RuntimeClock gRuntimeClock;

VHAudioChannel channels[AUDIO_CHN_CNT];
ABuffer abuffers[AUDIO_CHN_CNT];

VHMIDI::stMIDITrackProps        gTracksMeta[VHMIDI_MAXTRACKS];

VHKeyboard                      gKeyboard;


VHMIDI::Track           gTrack0(0,   &gTracksMeta[0 ]);
VHMIDI::Track           gTrack1(1,   &gTracksMeta[1 ]);
VHMIDI::Track           gTrack2(2,   &gTracksMeta[2 ]);
VHMIDI::Track           gTrack3(3,   &gTracksMeta[3 ]);
VHMIDI::Track           gTrack4(4,   &gTracksMeta[4 ]);
VHMIDI::Track           gTrack5(5,   &gTracksMeta[5 ]);
VHMIDI::Track           gTrack6(6,   &gTracksMeta[6 ]);
VHMIDI::Track           gTrack7(7,   &gTracksMeta[7 ]);
VHMIDI::Track           gTrack8(8,   &gTracksMeta[8 ]);
VHMIDI::Track           gTrack9(9,   &gTracksMeta[9 ]);
VHMIDI::Track           gTrack10(10, &gTracksMeta[10]);
VHMIDI::Track           gTrack11(11, &gTracksMeta[11]);
VHMIDI::Track           gTrack12(12, &gTracksMeta[12]);
VHMIDI::Track           gTrack13(13, &gTracksMeta[13]);
VHMIDI::Track           gTrack14(14, &gTracksMeta[14]);
VHMIDI::Track           gTrack15(15, &gTracksMeta[15]);

// VHMIDI::Tracks          tracks;

// std::string fname = "liszt_hung_pno.mid";
// std::string fname = "stage_1.mid";
std::string             midifilename = "mapleaf.mid";

std::string             midifilepath = "/home/vigatron/Dropbox/VHSources/VHSourcesMisc/TestAudio2/midi";
std::string             midifilefull = midifilepath + "/" + midifilename;
VHMIDI::VHMIDIFile gMIDIFile((char *)midifilefull.c_str());
