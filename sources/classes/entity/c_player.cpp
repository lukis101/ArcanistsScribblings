
#include "globals.h"
#include "resources.h"
#include "states.h"
#include "functions/rendering.h"

#include "classes/entity/c_player.h"

CPlayer::CPlayer()
{
    local = true;
    active = true;
    input = true;
    collidable = true;

    falling = true;
    colliding = false;
    walking = false;
    drowning = false;
    onground = false;

    mirrored = false;
    stunned = false;

    team = 1;
    minions = 0;

    radius = 40.f/2.f;

    pos = {0.f,0.f,1.f};
    velocity = {0.f,0.f};
    accel = {0.f,0.f};

    colors[0]={255,255,255,255}; // Primary
    colors[1]={255,255,255,255}; // Secondary
    colors[2]={255,255,255,255}; // Accessory
    colors[3]={255,255,255,255}; // Skin
};
CPlayer::~CPlayer()
{
};


void CPlayer::Think()
{
    if (DEBUG_PLAYER) std::cout<<"\nThinking:\n";
    if ( pos.y > currentState->GetMap()->side_h+currentState->GetMap()->terrain_h )
    {
        drowning = true;
        velocity = vector_fzero;
        return;
    }
    else drowning = false;

    colliding = currentState->IsColliding( slot );
    onground = currentState->IsColliding( slot,{0.f,1.f} );

    if ( colliding )
    {
        /// STUCK
        if (DEBUG_PLAYER) std::cout<<"Stuck!\n";
        pos.y-=1.f;
        velocity = vector_fzero;
        accel = vector_fzero;
        walking = false;
        falling = false;
        onground = true;
    }
    else
    {
        float delta = LogicTimer->GetSeconds();
        if (DEBUG_PLAYER) std::cout<<"Delta = "<<delta<<"\n";
        if (DEBUG_PLAYER) std::cout<<"Velocity.X = "<<velocity.x<<"\n";

        float eps = 0.05f;
        //if ( !drowning )
            if ( onground && fabsf(velocity.y)<eps )
            {
                /// ------------ Onground ------------ ///
                if (DEBUG_PLAYER) std::cout<<"On ground...\n";

                /// 1) Status Logic
                falling = false;
                if (active)
                {
                    input = true;
                }
                else
                {
                    input = false;
                    walking = false;
                    velocity = vector_fzero;
                }

                /// 2) Move
                float rangex = (velocity.x<0.f) ? -1.f : 1.f;
                float movex = fabsf(velocity.x)*delta;
                if ( currentState->IsColliding(slot,{movex*rangex, 0.f}) )
                {
                    //Hit an obstacle, try to climb up
                    if (DEBUG_PLAYER) std::cout<<"Bump.\n";
                    short MAX_CLIMB = 5;
                    for (short n=1; n<=MAX_CLIMB; n++)
                    {
                        if ( !(currentState->IsColliding(slot,{movex*rangex,float(n)*(-1.f)})) )
                        {
                            pos.x += movex*rangex;
                            pos.y -= float(n);
                            if (DEBUG_PLAYER) std::cout<<"Climbed "<<n<<" px up\n";
                            break;
                        }
                        // Failed to climb up, stay
                    }
                }
                else if( !(currentState->IsColliding(slot,{movex*rangex, 1.f})) )
                {
                    // Would float, try to climb down
                    //std::cout<<"Climbing down\n";
                    short MAX_CLIMB = 5;
                    for (short n=1; n<=MAX_CLIMB; n++)
                    {
                        if ( currentState->IsColliding(slot,{movex*rangex,float(n+1)}) )
                        {
                            pos.x += movex*rangex;
                            pos.y += float(n);
                            if (DEBUG_PLAYER) std::cout<<"Climbed "<<n<<" px down\n";
                            break;
                        }
                        else if ( n==MAX_CLIMB )
                        {
                            // Failed to climb down, start falling
                            if (DEBUG_PLAYER) std::cout<<"Fell off.\n";
                            pos.x += movex*rangex;
                            velocity.x *= 0.5f;
                            velocity.y = 0.f;
                            walking = false;
                            falling = false;
                            onground = false;
                        }
                    }
                }
                else
                {
                    // Flat ground, yay!
                    //std::cout<<"Walked normally by "<<movex*rangex<<"\n";
                    pos.x += movex*rangex;
                }

                /// 3) Handle input (For next frame)
                if (input && onground)
                {
                    float walkspeed = 50.f;
                    float newvel_x = 0.f;
                    float newvel_y = 0.f;

                    if ( Keyboard[KEY_ARROW_UP   ].state );
                    if ( Keyboard[KEY_ARROW_DOWN ].state );
                    if ( Keyboard[KEY_ARROW_LEFT ].state ) newvel_x -= walkspeed;
                    if ( Keyboard[KEY_ARROW_RIGHT].state ) newvel_x += walkspeed;

                    if ( Keyboard[KEY_SHIFT].state )
                    {
                        newvel_y = -300.f;
                        newvel_x = 250.f;
                        if (mirrored) newvel_x *= -1.f;
                        onground = false;
                        walking = false;
                        if (DEBUG_PLAYER) std::cout<<"Far Jump.\n";
                    }
                    if ( Keyboard[KEY_CTRL].state )
                    {
                        newvel_y = -500.f;
                        newvel_x = 100.f;
                        if (mirrored) newvel_x *= -1.f;
                        onground = false;
                        walking = false;
                        if (DEBUG_PLAYER) std::cout<<"High Jump.\n";
                    }

                    if ( Mouse.left.state )
                    {
                        currentState->GetMap()->DamageTerrain(currentState->GetCursorPos(),50);
                    }
                    if ( Mouse.right.state )
                    {
                        pos = currentState->GetCursorPos(); // Follow cursor
                    }

                    if ( fabsf(newvel_x)>eps || fabsf(newvel_y)>eps )
                    {
                        velocity.x = newvel_x;
                        velocity.y = newvel_y;
                        if ( velocity.x<0.f ) mirrored = true;
                        else mirrored = false;
                        if (!walking)
                        {
                            walking = true;
                            walktime = GlobalTimer->GetTicks();
                        }
                    }
                    else velocity = vector_fzero;
                }
            }
            else
            {
                /// ------------ Airborne ------------ ///
                //std::cout<<"Airborne...\n";
                walking = false;
                delta = LogicTimer->GetSeconds();

                /// Ballsitics: decay x vel
                float ratio = 0.99f;
                velocity.x *= ratio;
                /// Wind influence
                // velocity.x += windvel;
                /// Gravity:
                float g = 1000.f;
                velocity.y += g*delta;

                float rangex = (velocity.x<0.f) ? -1.f : 1.f;
                float rangey = 1.f;
                if (velocity.y<0.f)
                    rangey = -1.f;
                else
                    if (!falling)
                    {
                        falling = true;
                        falltime = GlobalTimer->GetTicks();
                    }
                float movex = fabsf(velocity.x)*delta;
                float movey = fabsf(velocity.y)*delta;
                if ( currentState->IsColliding(slot,{movex*rangex, movey*rangey}) )
                {
                    // Can't move there, move as much as possible
                    if (DEBUG_PLAYER) std::cout<<"Slam...\n";
                    float stepx, stepy;
                    if ( movex>movey )
                    {
                        // X>Y
                        if ( movex<=1.f )
                        {
                            stepx=movex;
                            stepy=movey;
                        }
                        else
                        {
                            stepx=1.f;
                            stepy=movey/movex;
                        }
                    }
                    else
                    {
                        // Y>X
                        if ( movey<=1.f )
                        {
                            stepx=movex;
                            stepy=movey;
                        }
                        else
                        {
                            stepx=movex/movey;
                            stepy=1.f;
                        }
                    }
                    while( !(movex<0.f) && !(movey<0.f) )
                    {
                        if ( currentState->IsColliding(slot,{movex*rangex,movey*rangey}) )
                        {
                            movex -= stepx;
                            movey -= stepy;
                        }
                        else
                        {
                            pos.x += movex*rangex;
                            pos.y += movey*rangey;
                            // Bounce!
                            velocity=vector_fzero;
                            break;
                        }
                    }
                }
                else
                {
                    // Nothing blocks movement, continue
                    // *BUG* Passes small obstacles at high speeds
                    pos.x += movex*rangex;
                    pos.y += movey*rangey;
                }
            }
    }


    colliding = currentState->IsColliding( slot );
    if ( colliding ) // *FACEPALM*
        if (DEBUG_PLAYER) std::cout<<"FAIL: Still stuck!\n\n";

    //currentState->camera->Center( pos );

//    pos.x += velocity.x;
//    pos.y += velocity.y;
};

void CPlayer::Render()
{
    Color_c color_white1{ 255, 255, 255,   0 }; // ***** A = -1 *****
    Color_c color_white2{ 255, 255, 255, 255 };
    Color_c color_yellow1{ 255, 255, 200, 215 };
    Color_c color_yellow2{ 250, 250,  70, 215 };

    Color_c color_red1{ 255, 200, 200, 215 };
    Color_c color_red2{ 250,  70,  70, 215 };
    Color_c color_green1{ 200, 255, 200, 215 };
    Color_c color_green2{  70, 240,  70, 215 };
    Color_c color_blue1{ 200, 200, 255, 215 };
    Color_c color_blue2{ 70,  70,  250, 215 };

    Color_c col1 = color_white1;
    Color_c col2 = color_green2;
    if (mirrored) col2.g = 180;

    if (!onground) col2 = color_white2;
    if (falling)   col2 = color_yellow2;
    if (colliding) col2 = color_red2;
    if (drowning)  col2 = color_blue2;

    Rect_f rect = { pos.x-radius, pos.y-radius, radius*2.f, radius*2.f };
    RenderGradient( rect, col1, col2, GRADIENT_RADIAL );

    RenderMaterial( MAT_CHAR_BODY_1, 0, {pos.x-15.f, pos.y-6.f} , 0.6f );
    RenderMaterial( MAT_CHAR_FACE_1, 0, {pos.x-7.f, pos.y-20.f} , 0.5f );
};
