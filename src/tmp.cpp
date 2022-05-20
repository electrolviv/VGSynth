/*

#pragma once

#include "wnd.hpp"

class WNDAudioChannels {

public:

    SDL_Window * window;

    void Init();

    void  *  task(void *parg) {

        window = SDL_CreateWindow(
            "Audio Channels",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WNDWIDTH, WNDHEIGHT, 0 );
        // SDL_Window *window = SDL_CreateWindowFrom(wndParent);


        while(!flagExit) {

            SDL_Delay(10);

            if (SDL_PollEvent(&event))
            {
                // printf("Achannel event type = %X\n", event.type);

            if(event.type == SDL_WINDOWEVENT) {
                if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                    flagExit = true;
            }
        }

    }

    flagExit = true;
    return nullptr;
}

bool         getflagexit()   { return flagExit; }
void         forceexit()     { flagExit = true; }

private:

bool         flagExit = false;
static const int WNDWIDTH   = 128;
static const int WNDHEIGHT  = 256;


};


*/

/*
#include "achannels.hpp"

static void  *  _run(void *parg) {
    return ((WNDAudioChannels *)parg) -> task(nullptr);
}


void WNDAudioChannels::Init()
{

    pthread_t thr;

    if(pthread_create(&thr, nullptr, _run, this) != 0) {
        printf("ctreate_thread failed\n");
        exit(0);
    }

}
*/
