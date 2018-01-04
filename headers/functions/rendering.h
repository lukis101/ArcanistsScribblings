#ifndef RENDERING_H
#define RENDERING_H

#include "types.h"
#include "globals.h"

void RenderTexture( unsigned tex_id, Vector_f pos, unsigned * buffer = NULL, float scale=1.f, short rendermode=0, bool flipped=false );

void RenderMaterial( unsigned matid, short frameid=0, Vector_f offset=vector_fzero, float scale=1.f, float rotation=0.f, Vector_f uvoffset=vector_fzero );

void RenderColor( Rect_f rect, unsigned char r=255, unsigned char g=255, unsigned char b=255, unsigned char a=255 );

void RenderGradient( Rect_f rect, Color_c ColA=color_black, Color_c ColB=color_white, unsigned type=GRADIENT_VERTICAL );

#endif
