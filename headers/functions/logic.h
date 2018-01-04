#ifndef LOGIC_H
#define LOGIC_H

#include "SDL/SDL.h"
#include "types.h"

bool IsInsidei( int x, int y, Rect_i rect );
bool IsInsidei( int x, int y, Circle_i circ );

bool IsInsidef( float x, float y, Rect_f rect );
bool IsInsidef( float x, float y, Circle_f circ );

#endif
