#ifndef C_BASEENTIY_H
#define C_BASEENTIY_H

#include "classes/c_baseclass.h"
#include "types.h"

class CBaseEntity : public CBaseClass
{
    protected:

    public:
    float radius;
    CBaseEntity();
    virtual ~CBaseEntity();

    virtual void Think()=0;
    virtual void Render()=0;

//    Vector_f GetPos();
//    void SetPos( float newx, float newy );
//    void SetPos( Vector_f );
};

#endif
