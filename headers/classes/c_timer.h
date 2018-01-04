
#ifndef TIMER_H
#define TIMER_H

#include "SDL/SDL.h"

class CTimer
{
    private:
    bool started, paused;
    #ifdef __emscripten_h__
        float startTicks, pausedTicks;
    #else
        Uint32 startticks;
        int pausedticks;
    #endif

    public:
    CTimer();

    bool IsStarted();
    bool IsPaused();

    void Start();
    void Stop();
    void Pause();
    void Unpause();

    #ifdef __emscripten_h__
        float GetTicks();
    #else
        int GetTicks();
    #endif
    float GetSeconds();
};
#endif
