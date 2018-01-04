
#include <cmath>
#include "functions/math.h"

double distance( int x1, int y1, int x2, int y2 )
{
    return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

unsigned int poweroftwo( unsigned int num )
{
    if( num != 0 )
    {
        num--;
        num |= (num >> 1);  //Or first 2 bits
        num |= (num >> 2);  //Or next 2 bits
        num |= (num >> 4);  //Or next 4 bits
        num |= (num >> 8);  //Or next 8 bits
        num |= (num >> 16); //Or next 16 bits
        num++;
    }
    return num;
}

