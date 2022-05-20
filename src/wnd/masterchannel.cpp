#include "masterchannel.hpp"

#include "channel.hpp"

extern  VHAudioChannel   channels[DEF_AUDIO_CHN_CNT];


SDL_Thread *thread;
static pthread_t thr;

int16_t * scr_GetAudBuffPtr();


static void  *  _run(void *parg) {
    return ((WNDMasterChannel *)parg) -> task(nullptr);
}


void WNDMasterChannel::Init() {


    if(pthread_create(&thr, nullptr, _run, this) != 0) { // _run
        printf("ctreate_thread failed\n");
        exit(0);
    }


}

void WNDMasterChannel::forceexit()     { flagExit = true; }

void WNDMasterChannel::RenderBuffer() {

    int16_t * parr = scr_GetAudBuffPtr();
    if(parr == nullptr)
        return;

    memset(gfxbuffer, 0x00, WNDWIDTH * WNDHEIGHT * 4);

    for(int i=0;i<WNDWIDTH;i++) {
        uint8_t v = 255 - (127 + (parr[i]/256));
        gfxbuffer[(v*WNDWIDTH)+i] = 0xFF;
        // gfxbuffer[(v*WNDWIDTH)+i+1] = 0xFF;

    }

}

void WNDMasterChannel::RenderChannels() {


    for(int i=0;i<DEF_AUDIO_CHN_CNT;i++) {

        if(channels[i].voiceBase.keyState == VHAudioChannel::nKeyState_Off) {
            SDL_SetRenderDrawColor(renderer, 55, 55, 55, SDL_ALPHA_OPAQUE);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        }

        SDL_RenderDrawLine(renderer, 2, i*16, 8, (i*16)+10);

    }
    SDL_RenderPresent(renderer);

}


void *WNDMasterChannel::task(void *parg) {

    if(parg == nullptr)
        printf("task_scr arg=nullptr\n");

    window = SDL_CreateWindow(
                "SDL2Test",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                WNDWIDTH, WNDHEIGHT, 0 );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Surface * data_sf = SDL_CreateRGBSurfaceFrom(
                (uint8_t *)gfxbuffer,
                WNDWIDTH, WNDHEIGHT,
                32, WNDWIDTH * 4,
                sdlmask32(0), sdlmask32(1), sdlmask32(2), 0);

    SDL_Surface *     screen  = SDL_GetWindowSurface(window);
    SDL_Rect          rectscr = { 0, 0, WNDWIDTH, WNDHEIGHT };


    while(!flagExit)
    {
        RenderBuffer();

        if(SDL_BlitScaled(data_sf, nullptr, screen, &rectscr) == 0)
            SDL_UpdateWindowSurface(window);

        RenderChannels();


        // 50/20 Hz
        SDL_Delay(50); // 20

    }

    flagExit = true;
    return nullptr;
}




