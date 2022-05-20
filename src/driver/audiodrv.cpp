#include "audiodrv.hpp"
#include <stdio.h>


extern "C" {
    #include <alsa/asoundef.h>
    #include <alsa/asoundlib.h>
    #include <pthread.h>
};



#define PCM_DEVICE "default"

#define LOGS(X) { printf("%s\n", X); fflush(stdout); }

static int                       pcm;
static unsigned int              tmp;
static int                       channels = 1;
static int                       rate = 44100;
static snd_pcm_t               * pcm_handle;
static snd_pcm_hw_params_t     * params;
static snd_pcm_uframes_t         frames;
static int                       buffsize;
static int16_t                   buff1[1024*4];
static int16_t                   buff2[1024*4];
static pthread_t                 thr;

VHAudioDriver                    globalAudioDriver;


static void* _workerThreadProc(void* rawArg) {

    /* ... достаем аргументы по указателю rawArg ... */
    if(rawArg!=nullptr) { LOGS("AudioDriver arg was ignored");}

    /* No other thread is going to join() this one */
    // pthread_detach(pthread_self());

    /* ... тут всякая полезная нагрузка ... */
    globalAudioDriver.Thread();
    pthread_exit(nullptr);
}


VHAudioDriver::VHAudioDriver() :
    flagInit(false),
    flagExit(false),
    flagFillData(false),
    buffidx(0)
{

}


VHAudioDriver::~VHAudioDriver()
{
    if(flagInit)
        Deinit();
}

bool VHAudioDriver::Init()
{
    LOGS("VHAudioDriver::Init()");

    if(pthread_create(&thr, nullptr, _workerThreadProc, (void*)nullptr) != 0) {
        LOGS("pthread_create() failed");
        return false;
    }

    return true;
}

void VHAudioDriver::Deinit() {
    flagExit = true;
    while(flagInit) { usleep(1000); }
    usleep(1000);
}

void VHAudioDriver::Thread()
{

    /* Open the PCM device in playback mode */
    pcm = snd_pcm_open(
        &pcm_handle,
        PCM_DEVICE,
        SND_PCM_STREAM_PLAYBACK,
        SND_PCM_NONBLOCK);

    if ( pcm < 0) {
        printf("ERROR: Can't open \"%s\" PCM device. %s\n", PCM_DEVICE, snd_strerror(pcm));
        return;
    }

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(pcm_handle, params);

    /* Set parameters */
    pcm = snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if ( pcm < 0) printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));
    pcm = snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    if (pcm < 0) printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));
    pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels);
    if ( pcm< 0) printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));
    pcm = snd_pcm_hw_params_set_rate(pcm_handle, params, rate, 0);
    if (pcm < 0) printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

    /* Write parameters */
    pcm = snd_pcm_hw_params(pcm_handle, params);
    if (pcm < 0) printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

    /* Allocate buffer to hold single period */
    snd_pcm_hw_params_get_period_size(params, &frames, 0);

    /* Resume information */
    printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));
    printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));
    snd_pcm_hw_params_get_channels(params, &tmp);
    printf("channels: %x ", tmp);
    if (tmp == 1)         printf("(mono)\n");
    else if (tmp == 2)    printf("(stereo)\n");
    snd_pcm_hw_params_get_rate(params, &tmp, 0);
    printf("rate: %u bps\n", tmp);
    printf("frames: %ld\n", frames);

    buffsize = frames * channels * 2;
    snd_pcm_hw_params_get_period_time(params, &tmp, NULL);

    for(unsigned int i=0;i<frames;i++) { buff1[i] = 0; }
    for(unsigned int i=0;i<frames;i++) { buff2[i] = 0; }

    flagExit = false;
    flagInit = true;
    buffidx = 0;

    flagFillData = false;

    snd_pcm_sframes_t   delay;

    fflush(stdout);

    while(!flagExit)
    {

            int16_t *pbuff = !buffidx ? buff1 : buff2;

            pcm = snd_pcm_writei(pcm_handle, pbuff, frames);
            if(pcm == -EPIPE) { LOGS("AudioBuff ERR"); }

            buffidx ^=1; flagFillData = true;
            snd_pcm_delay(pcm_handle, &delay);

            while(delay > 4000)
            {
                snd_pcm_delay(pcm_handle, &delay);
                usleep(1000);
            }

    }

    LOGS("VHAudioDriver::Deinit()");

    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);

    flagInit = false;

}

bool VHAudioDriver::IsDataNeed() {
    return flagFillData;
}

void VHAudioDriver::ResetFlagDataNeed() {
    flagFillData = false;
}

int16_t *VHAudioDriver::GetBufferPtr(int *plen)
{
    int16_t *pbuff = !buffidx ? buff1 : buff2;
    *plen = frames;
    return pbuff;
}

