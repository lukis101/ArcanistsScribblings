#ifndef C_BUTTON_H
#define C_BUTTON_H

#include "SDL/SDL.h"
//#include "globals.h"
#include "types.h"
#include "classes/gui/c_baseguientity.h"
#include "classes/c_material.h"

class CButton : public CBaseGuiEntity
{
    private:
    CMaterial surface_normal;
    CMaterial surface_hovered;
    CMaterial surface_pressed;
    Circle_i circle;
    Rect_i rect;
    char shape;
    bool hovered;
    bool pressed;
    //bool pressed2;

    public:
    CButton();
    ~CButton();

    void Think();
    void Render();

    void SetPos( float newx, float newy );

    void SetImage( CMaterial newmat, char image_type=3, char shape=0 );
    void OnPress();
    void OnRelease();

};

#endif
