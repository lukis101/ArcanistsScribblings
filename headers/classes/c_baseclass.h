#ifndef BASECLASS_H
#define BASECLASS_H

#include <glm/glm.hpp>
#include "types.h"

class CBaseClass
{
    protected:
    int id;         // Unique id
    int slot;       // [n] slot inside vector
    unsigned int buffer;
    bool valid;

    bool collidable;
    Vector_f pos;

    public:
    CBaseClass();
    virtual ~CBaseClass();

    void SetId( int newid );
    int GetId();
    void SetSlot( int newslot );
    int GetSlot();

    bool should_render;
    bool should_think;

    virtual void Render()=0;
    virtual void Think()=0;

    bool IsCollidable();
    void SetCollidable( bool newbool );

    virtual Vector_f GetPos();
    virtual void SetPos( float newx, float newy );
    virtual void SetPos( Vector_f );

};

#endif
