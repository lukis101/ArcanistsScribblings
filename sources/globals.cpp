
#include "SDL/SDL.h"

#include "types.h"
#include "globals.h"

#include "classes/object_types.h"

//struct Circle{ int x, y, r, d; };

/// 1280 1024    800 600    640 480
short FPS_MAX = 60;
//const short SCREEN_W      = 800;
//const short SCREEN_H      = 600;
const short SCREEN_W      = 800;
const short SCREEN_H      = 600;
const short SCREEN_BPP    = 32;
const Uint32 SCREEN_FLAGS = SDL_SWSURFACE
//|SDL_DOUBLEBUF
|SDL_GL_DOUBLEBUFFER|SDL_OPENGL
//|SDL_FULLSCREEN
;

    std::vector< CMaterial > *Materials  = NULL; // nullptr
    std::vector< BaseClass > *EntityList = NULL;
    std::vector< BaseClass > *GuiList    = NULL;
    std::vector< Timer     > *TimerList  = NULL;

short ENTS_MAX = 1024;
short PARTICLES_MAX = 600;

int EntityIds = 0;
int GuiIds    = 0;
int TimerIds  = 0;

type_Mouse Mouse;
button Keyboard[KEY_AMOUNT];

Timer GlobalTimer = new CTimer();
Timer FPSTimer = new CTimer();
Timer LogicTimer = new CTimer();

/// SDL globals
SDL_Event event;
SDL_Surface *screen = NULL; // nullptr

/// Basic Constants
const Vector_i vector_izero = {0,0};
const Vector_f vector_fzero = {0.f,0.f};
const Color_c color_white = {255,255,255,255};
const Color_c color_black = {0,0,0,255};
const Color_c color_transp = {0,0,0,0};

/// Debug bools
const bool DEBUG_GLSL = true;
const bool DEBUG_PLAYER = false;

