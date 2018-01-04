
#include "classes/c_camera.h"
#include "globals.h"

CCamera::CCamera()
{
    pos.x = 0.f;
    pos.y = 0.f;
    pos.z = 1.f;
    backedup = false;
    backup.x = 0.f;
    backup.y = 0.f;
    backup.z = 1.f;
    size.x = float(SCREEN_W);
    size.y = float(SCREEN_H);
    parentid = -1;
}
//Camera::~Camera()
//{
//}

void CCamera::Pan( float amountx, float amounty )
{
    parentid = -1;
    if (!backedup)
    {
        backedup = true;
        backup = pos;
    }
    else
    {
        pos.x = backup.x;
        pos.y = backup.y;
    }
    pos.x += amountx;
    pos.y += amounty;
}
void CCamera::Center( Vector_f targetpos ) // Center on (world coords)
{
    pos.x = targetpos.x*pos.z-(size.x/2.f);
    pos.y = targetpos.y*pos.z-(size.y/2.f);
}
Vector_f CCamera::GetFocus() // Get center (world coords)
{
    Vector_f focus;
    focus.x = (pos.x+(size.x/2.f))/pos.z;
    focus.y = (pos.y+(size.y/2.f))/pos.z;
    return focus;
}

void CCamera::SetPos( Vector_f newpos ) { pos = newpos; }
void CCamera::SetPos( float newx, float newy, float newz )
{
    pos = { newx, newy, (newz==0 ? 1.f : newz) };
}
Vector_f CCamera::GetPos() { return pos; }

void CCamera::SetZ( float newz ) { pos.z = newz; }
float CCamera::GetZ() { return pos.z; }

void CCamera::SetSize( Vector_f newsize ) { size = newsize; }
void CCamera::SetSize( float neww, float newh ) { size = { neww, newh }; }
Vector_f CCamera::GetSize() { return size; }

void CCamera::SetParent( int newparent ) { parentid = newparent; }
int CCamera::GetParent() { return parentid; }
