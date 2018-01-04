
#include "classes/c_baseclass.h"

CBaseClass::CBaseClass()
{
    collidable = false;
};

CBaseClass::~CBaseClass()
{
};

int CBaseClass::GetId() { return id; };
void CBaseClass::SetId( int newid )
{
    id = newid;
    valid = true;
};

bool CBaseClass::IsCollidable() { return collidable; };
void CBaseClass::SetCollidable( bool newbool ) { collidable=newbool; };

int CBaseClass::GetSlot() { return slot; };
void CBaseClass::SetSlot( int newslot ) { slot = newslot; };

Vector_f CBaseClass::GetPos() { return pos; };
void CBaseClass::SetPos( float newx, float newy ) { pos = { newx, newy }; };
void CBaseClass::SetPos( Vector_f newpos ) { pos = newpos; };

