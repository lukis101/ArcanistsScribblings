#ifndef CAMERA_H
#define CAMERA_H

#include "types.h"

class CCamera
{
    private:
    int parentid;

    public:
    CCamera();
    //~Camera();
    Vector_f pos;
    Vector_f backup;
    Vector_f size;

    bool backedup;

    void Pan( float amountx, float amounty );
    void Center( Vector_f targetpos );
    Vector_f GetFocus();

    void SetPos( Vector_f newpos );
    void SetPos( float newx, float newy, float newz=0 );
    Vector_f GetPos();

    void SetZ( float newz );
    float GetZ();

    void SetSize( Vector_f newsize );
    void SetSize( float neww, float newh );
    Vector_f GetSize();

    void SetParent( int newparent );
    int GetParent();
};

#endif
