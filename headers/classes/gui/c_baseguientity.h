#ifndef C_BASEGUIENTITY_H
#define C_BASEGUIENTITY_H

#include "classes/c_baseclass.h"

class CBaseGuiEntity : public CBaseClass
{
    protected:
    int z;

    public:
    CBaseGuiEntity();
    virtual ~CBaseGuiEntity();

    virtual void Think()=0;
    virtual void Render()=0;

    //virtual void SetPos( int newx, int newy );
    int GetZ();
    void SetZ( int newz );
};

#endif
