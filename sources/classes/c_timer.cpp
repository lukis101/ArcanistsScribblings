
#include "time.h"
#include "classes/c_timer.h"

CTimer::CTimer()
{
    startticks = 0;
    pausedticks = 0;
    started = false;
    paused = false;
}

bool CTimer::IsStarted() { return started; }
bool CTimer::IsPaused() { return paused; }

void CTimer::Start()
{
    started = true;
    paused = false;
    #ifdef __emscripten_h__
        startticks = emscripten_get_now();
    #else
        startticks = SDL_GetTicks();
    #endif
    pausedticks = 0;
}
void CTimer::Stop()
{
    started = false;
    paused = false;
    #ifdef __emscripten_h__
        pausedticks = emscripten_get_now()-startticks;
    #else
        pausedticks = SDL_GetTicks()-startticks;
    #endif
}
void CTimer::Pause()
{
    if ( (started) && (paused==false) )
    {
        paused = true;
        #ifdef __emscripten_h__
            pausedticks = emscripten_get_now()-startticks;
        #else
            pausedticks = SDL_GetTicks()-startticks;
        #endif
    }
}
void CTimer::Unpause()
{
    if ( paused )
    {
        paused = false;
        pausedticks = 0;
        #ifdef __emscripten_h__
            startticks = emscripten_get_now()-pausedticks;
        #else
            startticks = SDL_GetTicks()-pausedticks;
        #endif
    }
}

#ifdef __emscripten_h__
    float CTimer::GetTicks()
    {
        if( started )
        {
            if (paused) return pausedticks;
            else return emscripten_get_now()-startticks;
        }
        return 0;
    }
#else
    int CTimer::GetTicks()
    {
        if( started )
        {
            if (paused) return pausedticks;
            else return SDL_GetTicks()-startticks;
        }
        return 0;
    }
#endif
float CTimer::GetSeconds()
{
    if (started) return float( GetTicks() )/1000.f;
    return 0;
}
