
#include "classes/gui/c_baseguientity.h"

CBaseGuiEntity::CBaseGuiEntity()
{
    z = 0;
    valid = true;
    should_render = true;
    should_think = true;
};
CBaseGuiEntity::~CBaseGuiEntity()
{
};


int CBaseGuiEntity::GetZ()
{
    return z;
};
void CBaseGuiEntity::SetZ( int newz )
{
    z = newz;
};

