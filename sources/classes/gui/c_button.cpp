
#include <iostream>

#include "SDL/SDL.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "globals.h"
#include "resources.h"
#include "functions/rendering.h"
#include "functions/logic.h"
#include "functions/image_proc.h"
#include "classes/gui/c_button.h"

CButton::CButton()
{
    buffer = 0;
    shape = BUTTON_SQUARE;
    hovered = false;
    pressed = false;
};
CButton::~CButton()
{
    glDeleteBuffers( 1, &buffer );
};

void CButton::Think()
{
    bool cursorinside = false;
    bool leftinside = false;
    if ( shape == BUTTON_SQUARE )
    {
        if ( IsInsidei( Mouse.x, Mouse.y, rect ) )
        {
            cursorinside = true;
        }
        if ( IsInsidei( Mouse.left.x, Mouse.left.y, rect ) )
        {
            leftinside = true;
        }
    }
    else if ( shape == BUTTON_CIRCLE )
    {
        Circle_i offsetted = circle;
        offsetted.x += int(pos.x);
        offsetted.y += int(pos.y);
        if ( IsInsidei( Mouse.x, Mouse.y, circle ) )
        {
            cursorinside = true;
        }
        if ( IsInsidei( Mouse.left.x, Mouse.left.y, circle ) )
        {
            leftinside = true;
        }
    }
    if ( cursorinside )
    {
        hovered = true;
    }
    else
    {
        hovered = false;
    }
    if ( pressed )
    {
        if ( !Mouse.left.state )
        {
            if ( leftinside )
            {
                OnRelease();
            }
            pressed = false;
        }
    }
    else
    {
        if ( Mouse.left.state && leftinside )
        {
            pressed = true;
            OnPress();
        }
    }

    // Dragging
    if ( pressed )
    {
        //SetPos( Mouse.x, Mouse.y );
    }

};
void CButton::Render()
{
    float newy = 0.f;
    CMaterial surface = surface_normal;
    if ( pressed )
    {
        newy=3.f;
        if ( surface_pressed.IsValid() ) surface = surface_pressed;
    }
    else
    {
        if ( hovered )
        {
            newy=1.f;
            if ( surface_hovered.IsValid() ) surface = surface_hovered;
        }
    }

    Vector_f offsetted = { pos.x, pos.y+newy };

    if ( shape == BUTTON_SQUARE )
    {
        offsetted.x -= rect.w;
        offsetted.y -= rect.h;
    }
    else if ( shape == BUTTON_CIRCLE )
    {
        offsetted.x -= circle.r;
        offsetted.y -= circle.r;
    }
    RenderTexture( surface.GetTexID(), offsetted, &buffer );
};

void CButton::SetPos( float newx, float newy )
{
    pos = {newx, newy};
    if ( shape == BUTTON_SQUARE )
    {
        rect.x = int(newx);
        rect.y = int(newy);
    }
    else if ( shape == BUTTON_CIRCLE )
    {
        circle.x = int(newx);
        circle.y = int(newy);
    }
}
void CButton::SetImage( CMaterial newmat, char image_type, char newshape )
{
    switch ( image_type )
    {
        case BUTTON_HOVERED: surface_hovered = newmat; break;
        case BUTTON_PRESSED: surface_pressed = newmat; break;
        default : surface_normal = newmat;
            shape = newshape;
            Frame frame = newmat.GetFrame( 1 );
            if ( newshape == BUTTON_SQUARE )
            {
                rect.x = pos.x;
                rect.y = pos.y;
                rect.w = frame.image.w/2;
                rect.h = frame.image.h/2;
            }
            else if ( newshape == BUTTON_CIRCLE )
            {
                circle.x = pos.x;
                circle.y = pos.y;
                circle.d = frame.image.w;
                circle.r = circle.d/2;
            }
            else std::cout<<"\n\nUNKNOWN BUTTON TYPE!!\n\n";
            break;
    }
};
void CButton::OnPress()
{
    std::cout<<"Pressed!\n";
}
void CButton::OnRelease()
{
    std::cout<<"Released!\n";
}

