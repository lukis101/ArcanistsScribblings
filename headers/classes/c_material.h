#ifndef C_MATERIAL_H
#define C_MATERIAL_H

#include "types.h"

class CMaterial
{
    private:
    bool valid;
    int texid;      // ID of GL texture
    unsigned buffer;
    int filter;
    bool mirrored;
    bool flipped;
    bool animated;
    short baseframe;
    Color_f color;
    Frame *frames;

    public:
    CMaterial();
    ~CMaterial();

    void SetValid( bool newbool );
    bool IsValid();

    void SetBuffer( unsigned buffer );
    unsigned* GetBuffer();
    void SetTexID( int newid );
    int GetTexID();
    void SetFilter( int newfilter );
    int GetFilter();

    void SetMirrored( bool newbool );
    bool IsMirrored();

    void SetAnimated( bool newbool );
    bool IsAnimated();
    void SetBaseFrame( short newframe );
    short GetBaseFrame();

    void GenFrames( short amount );
    void SetFrame( short frameid, Frame newframe );
    Frame GetFrame( short frameid );
    void SetFramePos( short frameid, float newx=0.f, float newy=0.f );
    void SetFrameSize( short frameid, float neww=100.f, float newh=100.f );
    void SetFrameUV( short frameid, Rect_f newuv );
    void SetFrameUV( short frameid, float newu=0.f, float newv=0.f, float neww=1.f, float newh=1.f );

    void SetColor( Color_f newcolor );
    Color_f GetColor();

    bool should_think;
    virtual void Think();
};

#endif
