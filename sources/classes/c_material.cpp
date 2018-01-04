
#include "classes/c_material.h"

CMaterial::CMaterial()
{
    valid = false;
    texid = 0;
    buffer = 0;
    filter = 0x2600; // GL_NEAREST=0x2600 GL_LINEAR=0x2601
    mirrored = false;
    animated = false;
    baseframe = 1;
    color.r = 1.f;
    color.g = 1.f;
    color.b = 1.f;
    color.a = 1.f;
}
CMaterial::~CMaterial()
{
}

void CMaterial::SetValid(bool newbool) { valid = newbool; }
bool CMaterial::IsValid() { return valid; }

void CMaterial::SetBuffer(unsigned newbuffer) { buffer = newbuffer; }
unsigned* CMaterial::GetBuffer() { return &buffer; }

void CMaterial::SetTexID(int newid) { texid = newid; }
int CMaterial::GetTexID() { return texid; }

void CMaterial::SetFilter(int newfilter) { filter = newfilter; }
int CMaterial::GetFilter() { return filter; }


void CMaterial::SetMirrored(bool newbool) { mirrored = newbool; }
bool CMaterial::IsMirrored() { return mirrored; }


void CMaterial::SetAnimated(bool newbool) { should_think = newbool; animated = newbool; }
bool CMaterial::IsAnimated() { return animated; }

void CMaterial::SetBaseFrame( short newframe ) { baseframe = newframe; }
short CMaterial::GetBaseFrame() { return baseframe; }


void CMaterial::GenFrames( short amount )
{
    frames = new Frame[amount];
}
void CMaterial::SetFrame( short frameid, Frame newframe )
{
    frames[frameid-1] = newframe;
}
Frame CMaterial::GetFrame( short frameid )
{
    return frames[frameid-1];
}
void CMaterial::SetFramePos( short frameid, float newx, float newy )
{
    frames[frameid-1].image.x = newx;
    frames[frameid-1].image.y = newy;
}
void CMaterial::SetFrameSize( short frameid, float neww, float newh )
{
    frames[frameid-1].image.w = neww;
    frames[frameid-1].image.h = newh;
}
void CMaterial::SetFrameUV( short frameid, float newu, float newv, float neww, float newh )
{
    frames[frameid-1].uv.x = newu;
    frames[frameid-1].uv.y = newv;
    frames[frameid-1].uv.w = neww;
    frames[frameid-1].uv.h = newh;
}
void CMaterial::SetFrameUV( short frameid, Rect_f newuv )
{
    frames[frameid-1].uv.x = newuv.x;
    frames[frameid-1].uv.y = newuv.y;
    frames[frameid-1].uv.w = newuv.w;
    frames[frameid-1].uv.h = newuv.h;
}

void CMaterial::SetColor( Color_f newcolor ) { color = newcolor; }
Color_f CMaterial::GetColor() { return color; }

void CMaterial::Think()
{
}
