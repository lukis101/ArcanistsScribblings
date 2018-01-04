#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <memory>
#include <cmath>
#include <vector>
#include "SDL/SDL.h"

#include "types.h"
#include "classes/c_material.h"
#include "classes/c_baseclass.h"
#include "classes/c_timer.h"
#include "classes/object_types.h"

/// Rendering enums
enum RENDER_MODES
{
    RENDER_RECOLOR = 0,
    RENDER_GRADIENT_H,
    RENDER_GRADIENT_V,
    RENDER_GRADIENT_C,
    RENDER_BLUR_H,
    RENDER_BLUR_V,
    RENDER_INVERT,
};
enum GRADIENT_TYPES
{
    GRADIENT_HORIZONTAL = 0,
    GRADIENT_VERTICAL,
    GRADIENT_DIAGONAL_UP,
    GRADIENT_DIAGONAL_DOWN,
    GRADIENT_RADIAL,
};

/// Entity enums
enum BUTTON_TYPES
{
    BUTTON_SQUARE = 0,
    BUTTON_CIRCLE,
    BUTTON_NORMAL,
    BUTTON_HOVERED,
    BUTTON_PRESSED,
};

/// Keyboard buttons
enum KEYS
{
    KEY_ESC = 0,
    KEY_ENTER,
    KEY_CTRL,
    KEY_SHIFT,
    KEY_SPACE,
    KEY_CONSOLE,
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_ARROW_RIGHT,
    KEY_ARROW_LEFT,
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_AMOUNT,
};

extern type_Mouse Mouse;
extern button Keyboard[];

extern Timer GlobalTimer;
extern Timer FPSTimer;
extern Timer LogicTimer;

    extern float delta;

    extern std::vector< CMaterial > *Materials;
    extern std::vector< BaseClass > *EntityList;
    extern std::vector< BaseClass > *GuiList;
    extern std::vector< Timer     > *TimerList;

extern short FPS_MAX;
extern const short SCREEN_W;
extern const short SCREEN_H;
extern const short SCREEN_BPP;
extern const Uint32 SCREEN_FLAGS;

extern short ENTS_MAX;
extern short PARTICLES_MAX;

extern int EntityIds;
extern int GuiIds;
extern int TimerIds;

/// SDL globals
extern SDL_Event event;
extern SDL_Surface *screen;

/// Basic Constants
extern const Vector_i vector_izero;
extern const Vector_f vector_fzero;
extern const Color_c color_white;
extern const Color_c color_black;
extern const Color_c color_transp;

/// Debug bools
extern const bool DEBUG_GLSL; // Output shader logs
extern const bool DEBUG_PLAYER;

#endif
