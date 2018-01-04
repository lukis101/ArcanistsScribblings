

#include "SDL/SDL.h"
#include "functions/math.h"
#include "functions/logic.h"

bool IsInsidei( int x, int y, Circle_i circ )
{
    if ( distance( x, y, circ.x, circ.y ) <= circ.r )
    {
        return true;
    }
    return false;
}
bool IsInsidei( int x, int y, Rect_i rect )
{
    if ( ( x >= rect.x-rect.w ) && ( x <= rect.x+rect.w ) )
    {
        if ( ( y >= rect.y-rect.h ) && ( y <= rect.y+rect.h ) )
        {
            return true;
        }
    }
    return false;
}

