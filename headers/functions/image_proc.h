#ifndef IMAGE_PROC_H
#define IMAGE_PROC_H

#include "SDL/SDL.h"
#include <iostream>

unsigned char component_blend( unsigned char base, unsigned char blend, float scale );

bool LoadImage( std::string filename, unsigned GLTexID, unsigned TexID );

void apply_surface( SDL_Surface* source, SDL_Surface* destination, int x, int y );

#endif
