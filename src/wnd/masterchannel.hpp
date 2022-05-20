#pragma once

#include "wnd.hpp"

int16_t     *   scr_GetAudBuffPtr();

class WNDMasterChannel {

public:

    static const int WNDWIDTH = 256;
    static const int WNDHEIGHT = 256;

    WNDMasterChannel()  { }
    ~WNDMasterChannel() { }

    void            Init();
    void            forceexit();

    SDL_Window  *    window;

    void        *    task(void *parg);

private:

        bool            flagExit = false;

        uint32_t        gfxbuffer[WNDWIDTH * WNDHEIGHT];

        SDL_Renderer *  renderer;

        void            RenderBuffer();
        void            RenderChannels();

};
