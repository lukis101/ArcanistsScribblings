#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <sstream>
#include <string>
#include <cmath>
#include <cstdio>
#include <iostream>
#undef main
// 1280 1024    800 600    640 480
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const Uint32 SCREEN_FLAGS = SDL_SWSURFACE|SDL_DOUBLEBUF
//|SDL_FULLSCREEN
//|SDL_OPENGL
;

int FRAMES_PER_SECOND = 20;

const int MOVE_SPEED = 100;
const int MAX_FALL_SPEED = 80;
const int MAX_RISE_SPEED = 80;
const int COLLISION_Y = 5;

const float JUMP_VELOCITY = 10.f;
const int DOT_WIDTH = 16;
const int DOT_HEIGHT = 16;

const char endl='\n';

struct pixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *collision = NULL;
SDL_Surface *image = NULL;
SDL_Surface *image2 = NULL;

SDL_Event event;

//Uint8 *keystate = SDL_GetKeyState(NULL);

struct keystate
{
    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;
};
keystate input;

bool colliding = false;
bool folowing = false;

struct Circle{ int x, y, r, d; };

class Dot
{
    public:
    Circle c;
    float xVel, yVel;
    bool on_ground;
    int moving; // 0-not; 1-moving(natural); 2-moving(from input, walking)

    Dot();
    void handle_input();
    void handle_input2();
    //bool check_collison(Circle &data, int x_offset, int y_offset);
    void move(Circle &data,int deltatime);
    void draw();
    void think(Circle &data,int deltatime);
};
class Dot2
{
    public:
    Circle c;
    int xVel, yVel;

    Dot2();
    void handle_input();
    void move();
    void draw();
};

class Timer
{
    private:
    int startTicks;
    int pausedTicks;
    bool paused;
    bool started;

    public:
    Timer();
    void start();
    void stop();
    void pause();
    void unpause();

    int get_ticks();

    bool is_started();
    bool is_paused();
};
//////////////////////////////////////////////////////////////////////////////////////////////////
/// PIXEL MANIPULATION / DRAWING
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    if (y<surface->h && x<surface->w && x>=0 && y>=0)
    {
        Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
        *(Uint32 *)p = pixel;
    }
}
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    if (y<surface->h && x<surface->w && x>=0 && y>=0)
    {
        Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
        return *(Uint32 *)p;
    }
    else return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
/// COLLISION
double distance( int x1, int y1, int x2, int y2 )
{
    //std::cout<<"Coords= "<<(y2-y1)*(y2-y1)<<endl;
    return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}
bool check_circle_circle_collision( Circle &A, Circle &B )
{
    double a = distance( A.x+A.r, A.y+A.r, B.x+B.r, B.y+B.r );
    int b = A.r + B.r;
    //std::cout<<"Comparing "<<a<<" with "<<b<<endl;
    if( a < b )
    {
        return true;
    }
    return false;
}
bool check_circle_pixel_collision( Circle &target, SDL_Surface* surface )
{
    colliding=false;
    int cx = target.x+target.r;
    int cy = target.y+target.r;
    int r = target.r;

    for(int y=-r; y<=r; y++)
        for(int x=-r; x<=r; x++)
            if(x*x+y*y <= r*r+r*0.8f)
            {
                if (getpixel(surface,cx+x, cy+y)!=0)
                    return true;
            }
    return false;

//    double error = (double)-c.r;
//    double x = (double)c.r -0.5 ;
//    double y = (double)0.5;
//    double cx = c.x - 0.5+c.r;
//    double cy = c.y - 0.5+c.r;
//    while (x >= y)
//    {
//        if( getpixel(surface, (int)(cx + x), (int)(cy + y)) !=0 ||
//            getpixel(surface, (int)(cx + y), (int)(cy + x)) !=0 )
//            colliding=true;
//        if (x != 0)
//        {
//            if( getpixel(surface, (int)(cx - x), (int)(cy + y)) !=0 ||
//                getpixel(surface, (int)(cx + y), (int)(cy - x)) !=0 )
//                colliding=true;
//        }
//        if (y != 0)
//        {
//        if( getpixel(surface, (int)(cx + x), (int)(cy - y)) !=0 ||
//            getpixel(surface, (int)(cx - y), (int)(cy + x)) !=0 )
//            colliding=true;
//        }
//        if (x != 0 && y != 0)
//        {
//        if( getpixel(surface, (int)(cx - x), (int)(cy - y)) !=0 ||
//            getpixel(surface, (int)(cx - y), (int)(cy - x)) !=0 )
//            colliding=true;
//        }
//        error += y;
//        ++y;
//        error += y;
//        if (error >= 0)
//        {
//            --x;
//            error -= x;
//            error -= x;
//        }
//    }
//    return colliding;
}
bool check_square_pixel_collision( int x1, int y1, int width, SDL_Surface* surface )
{
    for(int y=y1; y<=y1+width; y++)
        for(int x=x1; x<=x1+width; x++)
        {
            if (getpixel(surface,x,y)!=0)
                return true;
        }
    return false;
}
bool check_collison(Circle &target, Circle &target2, int x_offset=0, int y_offset=0)
{
    int precision = 1;
    colliding = false;
    Circle offseted = target;
    offseted.x += x_offset;
    offseted.y += y_offset;

    if (check_circle_pixel_collision(offseted,collision)==true
        //|| check_circle_circle_collision(offseted,target2)==true
        //|| check_square_pixel_collision(target.x,target.y,16,collision)==true
        )
    {
        colliding = true;
    }

    return colliding;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

SDL_Surface *load_image( std::string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        SDL_SetColorKey( loadedImage, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGBA( loadedImage->format, 0xFF, 0, 0xFF, 0 ) );
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );
    }

    return optimizedImage;
}
void apply_surface( SDL_Surface* source, SDL_Surface* destination, int x, int y )
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, NULL, destination, &offset );
}
bool init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
    int flags = IMG_INIT_PNG;
    int initted=IMG_Init(flags);
    if( initted & flags != flags) {
    }
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SCREEN_FLAGS );

    if( screen == NULL )
    {
        return false;
    }

    SDL_WM_SetCaption( "Arcanists (pre-Alpha)", NULL );

    return true;
}
bool precache()
{
    background = load_image( "textures/terrain/terrain-3.tga" );
    collision = load_image( "textures/terrain/terrain-3.tga" );
    image = load_image( "textures/circle-big.bmp" );
    image2 = load_image( "textures/bomb-fire.bmp" );

    if( image == NULL )
    {
        return false;
    }
    return true;
}
void clean_up()
{
    SDL_FreeSurface( image );
    SDL_FreeSurface( image2 );
    SDL_Quit();
}
//////////////////////////////////////////////////////////////////////////////////////////////////

Dot::Dot()
{
    c.x = 0;
    c.y = 0;
    c.r = 16/2;
    xVel = 0.f;
    yVel = 0.f;
    on_ground = false;
    moving = 0;
}
void Dot::handle_input()
{
    if( event.type == SDL_KEYDOWN )
    {
        if (on_ground)
        {
            if ( event.key.keysym.sym==SDLK_KP9 )
            {
                yVel -= JUMP_VELOCITY;
                xVel = 200;
                on_ground=false;
                moving = 1;
            }
            if ( event.key.keysym.sym==SDLK_KP7 )
            {
                yVel -= JUMP_VELOCITY;
                xVel = -200;
                on_ground=false;
                moving = 1;
            }
            if ( event.key.keysym.sym==SDLK_KP6 )
            {
                yVel -= JUMP_VELOCITY*0.7;
                xVel = 500;
                //c.y--;
                on_ground=false;
                moving = 1;
            }
            if ( event.key.keysym.sym==SDLK_KP4 )
            {
                yVel -= JUMP_VELOCITY*0.7;
                xVel = -500;
                //c.y--;
                on_ground=false;
                moving = 1;
            }
        }
        switch( event.key.keysym.sym )
        {
            case SDLK_UP:    input.UP    = true; break;
            case SDLK_DOWN:  input.DOWN  = true; break;
            case SDLK_LEFT:  input.LEFT  = true; break;
            case SDLK_RIGHT: input.RIGHT = true; break;
        }
    }
    else if( event.type == SDL_KEYUP )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_UP:    input.UP    = false; break;
            case SDLK_DOWN:  input.DOWN  = false; break;
            case SDLK_LEFT:  input.LEFT  = false; break;
            case SDLK_RIGHT: input.RIGHT = false; break;
        }
    }
}
void Dot::handle_input2()
{
    //std::cout<<"OnGround="<<on_ground<<"\nMoving="<<moving<<"\n\n";
    //if (on_ground)
    //{
        //if ( moving == 2 )
            xVel = 0;
            yVel = 0;
        //if (input.UP   ) {yVel += JUMP_VELOCITY*-1; moving = 1;}
        //if (input.DOWN ) {yVel = 0; moving = 1;}
        //if (input.UP   ) {FRAMES_PER_SECOND = 10;}
        //if (input.DOWN ) {FRAMES_PER_SECOND = 2;}
        if (input.UP   ) {yVel += MOVE_SPEED*-1; moving = 2;} else {yVel += MOVE_SPEED;};
        if (input.DOWN ) {yVel += MOVE_SPEED; moving = 2;} else {yVel += MOVE_SPEED*-1;};
        if (input.LEFT ) {xVel += MOVE_SPEED*-1; moving = 2;} else {xVel += MOVE_SPEED;};
        if (input.RIGHT) {xVel += MOVE_SPEED; moving = 2;} else {xVel += MOVE_SPEED*-1;};
    //}
}
void Dot::move(Circle &data,int deltatime)
{
//    if ( SDL_MUSTLOCK(background) )
//        if ( SDL_LockSurface(background) < 0 )
//            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());

    int precision = 1;
    colliding=false;
    // Identify current state
    on_ground = check_collison(c,data,0,1);
    colliding = check_collison(c,data,0,0);

    //std::cout<<"Moving"<<endl;
    //std::cout<<colliding<<endl;
    //std::cout<<"On ground = "<<on_ground<<endl;
    //std::cout<<"Moving = "<<moving<<endl;

    if(colliding)
    {
       //std::cout<<"Stuck!( " <<c.x<<" ; "<<c.y<<" )\n";
       c.y--;
       xVel = 0;
       yVel = 0;
       return;
       /// TO-DO!
    }
//    else
//    {
//        // Don't bother if standing still
//        if(on_ground)
//        {
//            //std::cout<<"On ground...\n";
//            if (yVel==0 && xVel==0)
//            {
//                moving = 0;
//                return;
//            }
//        }
//    }

    //std::cout<<"Moving...\n";

    ///////////// GRAVITY //////////////
//    if (yVel>0.f)
//    {
//        yVel=(yVel*1.1+0.1);
//        if(yVel>MAX_FALL_SPEED) yVel = MAX_FALL_SPEED;
//        //moving = 1;
//    }
//    if (yVel<0.f)
//    {
//        yVel=(yVel/1.1+0.3);
//        //moving = 1;
//    }
//    if (yVel==0 && !(on_ground))
//    {
//        yVel+=0.3;
//        //moving = 1;
//    }
    ////////////////////////////////////

    float xMovement = xVel * ( deltatime / 1000.f );
    float yMovement = yVel * ( deltatime / 1000.f );

    c.x += xMovement;
    c.y += yMovement;

    // Handle moving area limitations //
    if( c.x<0 )
    {
        c.x = 0;
        xVel = 0;
        std::cout<<"Trying to go out of left bounds!\n\n";
    }
    if( c.x+DOT_WIDTH>SCREEN_WIDTH )
    {
        c.x = SCREEN_WIDTH-DOT_WIDTH;
        xVel = 0;
        std::cout<<"Trying to go out of right bounds!\n\n";
    }
    if( c.y+DOT_WIDTH>SCREEN_HEIGHT )
    {
        c.y = SCREEN_HEIGHT-DOT_WIDTH;
        xVel = 0;
        yVel = 0;
        std::cout<<"Hit the ocean!\n\n";
    }
    ////////////////////////////////////
/*
    //on_ground = check_collison(c,data,0,1);
    colliding = check_collison(c,data,0,0);

    // Try to do normal collision check
    if( colliding )
    {
        std::cout<<"\nColliding with something!\n";
        c.x -= xMovement;
        c.y -= yMovement;
        colliding = true;
    }
    else
    {
        //std::cout<<on_ground<<endl;
        std::cout<<moving<<endl;
        //on_ground = check_collison(c,data,0,1);
        colliding = false;
        // If we end up in air while moving because of player input
        // (while walking) then try to climb down
        if (moving = 2)
        {
            if (on_ground) // If we -were- on ground
            {
                //c.y++;
                on_ground = check_collison(c,data,0,1);
                colliding = check_collison(c,data,0,0);
                colliding = false;
                if (!on_ground) // And now we're not
                {
                    c.y--;
                    std::cout<<"Trying to climb down..\n";
                    for (int i=1; i<=COLLISION_Y; i++)
                    {
                        c.y += i;
                        if( check_collison(c,data,0,0)==false )
                        {
                            std::cout<<"Climbed by "<<i<<"\n\n";
                            colliding = false;
                            on_ground = true;
                            return;
                        }
                        else c.y -= i;
                    }
                    std::cout<<"Failed to climb down..\n";
                    xVel = 0;
                    moving = 1;
                }
                //c.y--;
            }
        }
    }

    // If colliding, try to do precise movement
    if (colliding)
    {
        int xplusminus, yplusminus;
        if (xMovement>0) xplusminus = 1; else xplusminus = -1;
        if (yMovement>0) yplusminus = 1; else yplusminus = -1;

        // Movement along 1 axis
        if (yMovement==0 || xMovement==0)
        {
            if(yMovement==0) // Horizontal
            {
                std::cout<<"Horizontal movement!\n";
                // If bumped into something while moving because of player input(walking),
                // then try to climb up
                if (moving == 2)
                {
                    c.x += xVel;
                    on_ground = check_collison(c,data,0,1);
                    if (on_ground)
                    {
                        std::cout<<"Trying to climb up..\n";
                        for (int i=1; i<=COLLISION_Y; i++)
                        {
                            c.y -= i;
                            if( check_collison(c,data,0,0)==false )
                            {
                                std::cout<<"Climbed by "<<i<<"\n\n";
                                colliding = false;
                                return;
                            }
                            else c.y += i;
                        }
                    }
                    c.x -= xVel;
                    std::cout<<"Failed to climb up\n\n";
                }
                // Otherwise: bounce! >:D
                for (int i=(xVel-xplusminus);i*xplusminus>0;i-=xplusminus)
                {
                    c.x += i;
                    if( check_collison(c,data,0,0)==false )
                    {
                        std::cout<<"X Moved by "<<i<<" instead of "<<xMovement<<"\n";
                        std::cout<<"X Moved to x="<<c.x<<" y="<<c.y<<"\n";
                        //c.x += i;
                        //c.x -= xplusminus;
                        colliding = false;
                        //return;
                    }
                    else c.x -= i;
                }
                if (colliding)
                {
                    std::cout<<"Failed to move horizontaly\n\n";
                }
                xVel=0;
            }
            else // Vertical
            {
                std::cout<<"Vertical movement!\n";
                for (int i=yMovement-yplusminus;i*yplusminus>0;i-=yplusminus)
                {
                    c.y += i;
                    if( check_collison(c,data,0,0)==false )
                    {
                        bool realy = check_collison(c,data,0,0);
                        if (realy) std::cout<<"\nDAFUQ!\n";
                        std::cout<<"Y Moved by "<<i<<" instead of "<<yMovement<<"\n";
                        std::cout<<"Y Moved from y="<<c.y-i<<" to y="<<c.y<<"\n";
                        //c.y += i;
                        //c.y -= yplusminus;
                        colliding = false;
                        yVel=0;
                        //return;
                    }
                    else c.y -= i;
                }
                if (colliding)
                {
                    std::cout<<"Failed to move verticaly\n\n";
                }
                yVel=0;
            }
        }
        else // 2D movement
        {
            std::cout<<"2D movement!\n";
            std::cout<<"xVel= "<<xMovement<<"  yVel= "<<yMovement<<"\n";
            float xstep=1, ystep=1;
            if (xMovement<yMovement)
            {
                std::cout<<"xVel < yVel\n";
                xstep=(xMovement/yMovement);
                for (int i=(yMovement-yplusminus+(0.5*yplusminus));i*yplusminus>0;i-=yplusminus)
                {
                    c.x += i*xstep+(0.5*xplusminus);
                    c.y += i+(0.5*yplusminus);

                    if( check_collison(c,data,0,0)==false )
                    {
                        bool realy = check_collison(c,data,0,0);
                        if (realy) std::cout<<"\nDAFUQ!\n";
                        std::cout<<"X Moved by "<<floor(i*xstep+(0.5*xplusminus))<<" instead of "<<xMovement<<"\n";
                        std::cout<<"Y Moved by "<<floor(i+(0.5*yplusminus))<<" instead of "<<yMovement<<"\n";
                        colliding = false;
                        on_ground = check_collison(c,data,0,1);
                        yVel=0;
                        break;
                    }
                    else
                    {
                        c.x -= i*xstep+(0.5*xplusminus);
                        c.y -= i+(0.5*yplusminus);
                    }
                }
                if (colliding)
                {
                    std::cout<<"Failed to move in 2D\n";
                }
                yVel=0;
                xVel=0;
            }
            else
            {
                std::cout<<"xVel > yVel\n";
                ystep = yMovement/xMovement;
                for (int i=(xMovement-xplusminus+(0.5*xplusminus));i*xplusminus>0;i-=xplusminus)
                {
                    c.x += i+(0.5*xplusminus);
                    c.y += i*ystep+(0.5*yplusminus);

                    if( check_collison(c,data,0,0)==false )
                    {
                        bool realy = check_collison(c,data,0,0);
                        if (realy) std::cout<<"\nDAFUQ!\n";
                        std::cout<<"X Moved by "<<floor(i+(0.5*xplusminus))<<" instead of "<<xMovement<<"\n";
                        std::cout<<"Y Moved by "<<floor(i*ystep+(0.5*yplusminus))<<" instead of "<<yMovement<<"\n";
                        colliding = false;
                        on_ground = check_collison(c,data,0,1);
                        yVel=0;
                        break;
                    }
                    else
                    {
                        c.x -= i+(0.5*xplusminus);
                        c.y -= i*ystep+(0.5*yplusminus);
                    }
                }
                if (colliding)
                {
                    std::cout<<"Failed to move in 2D\n";
                }
                yVel=0;
                xVel=0;
            }
        }
    }
    if(check_collison(c,data,0,0))
    {
       std::cout<<"OMG, WTF, STILL STUCK!?(at speed of "<<xMovement<<" , "<<yMovement<<")\n";
    }*/

    //if (SDL_MUSTLOCK(background)) SDL_UnlockSurface(background);
}
void Dot::draw()
{
    apply_surface( image2, screen, c.x, c.y );
}
void Dot::think(Circle &data,int deltatime)
{
    Dot::move(data,deltatime);
    Dot::draw();
}
//////////////////////////////////////////////////////////////////////////////////////////////////

Dot2::Dot2()
{
    c.x = 50;
    c.y = 50;
    c.r = 97/2;
    xVel = 0;
    yVel = 0;
}
void Dot2::handle_input()
{
    if( event.type == SDL_KEYDOWN )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_w: yVel -= MOVE_SPEED; break;
            case SDLK_s: yVel += MOVE_SPEED; break;
            case SDLK_a: xVel -= MOVE_SPEED; break;
            case SDLK_d: xVel += MOVE_SPEED; break;
        }
    }
    else if( event.type == SDL_KEYUP )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_w: yVel = 0; break;
            case SDLK_s: yVel = 0; break;
            case SDLK_a: xVel = 0; break;
            case SDLK_d: xVel = 0; break;
        }
    }
}
void Dot2::move()
{

    c.x += xVel;
    if( (c.x<0) || (c.x+DOT_WIDTH>SCREEN_WIDTH) )
    {
        c.x -= xVel;
    }

    c.y += yVel;
    if( ( c.y < 0 ) || ( c.y + DOT_HEIGHT > SCREEN_HEIGHT ) )
    {
        c.y -= yVel;
    }
}
void Dot2::draw()
{
    apply_surface( image, screen, c.x, c.y );
}
//////////////////////////////////////////////////////////////////////////////////////////////////

Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}
void Timer::start()
{
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}
void Timer::stop()
{
    started = false;
    paused = false;
}
void Timer::pause()
{
    if( (started==true) && (paused==false) )
    {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}
void Timer::unpause()
{
    if( paused == true )
    {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}
int Timer::get_ticks()
{
    if( started == true )
    {
        if( paused == true )
        {
            return pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}
bool Timer::is_started()
{
    return started;
}
bool Timer::is_paused()
{
    return paused;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

void draw_circle(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel)
{
    double error = (double)-radius;
    double x = (double)radius -0.5;
    double y = (double)0.5;
    double cx = n_cx - 0.5;
    double cy = n_cy - 0.5;

    while (x >= y)
    {
        putpixel(surface, (int)(cx + x), (int)(cy + y), pixel);
        putpixel(surface, (int)(cx + y), (int)(cy + x), pixel);
        if (x != 0)
        {
            putpixel(surface, (int)(cx - x), (int)(cy + y), pixel);
            putpixel(surface, (int)(cx + y), (int)(cy - x), pixel);
        }
        if (y != 0)
        {
            putpixel(surface, (int)(cx + x), (int)(cy - y), pixel);
            putpixel(surface, (int)(cx - y), (int)(cy + x), pixel);
        }
        if (x != 0 && y != 0)
        {
            putpixel(surface, (int)(cx - x), (int)(cy - y), pixel);
            putpixel(surface, (int)(cx - y), (int)(cy - x), pixel);
        }
        error += y;
        ++y;
        error += y;
        if (error >= 0)
        {
            --x;
            error -= x;
            error -= x;
        }
    }
}
void fill_circle(SDL_Surface *surface,SDL_Surface *collision, int cx, int cy, int radius, Uint32 pixel)
{
    Uint32 COLOR_TRANSPARENT = 0;
    Uint32 COLOR_BLACK = 4278190080;
    Uint32 COLOR_WHITE = 4294967295;
    Uint32 COLOR_GREY = 4286019447;
    // Outline
    int x=0,y=0,r=radius+4;
    for(int y=-r; y<=r; y++)
    for(int x=-r; x<=r; x++)
        if(x*x+y*y <= r*r+r*0.8f)
        {
            if (cy+y<cy)
            {
                if (getpixel(surface,cx+x, cy+y)!=0)
                putpixel(surface,cx+x, cy+y,COLOR_GREY);
            }
            else
            {
                if (getpixel(surface,cx+x, cy+y)!=0)
                putpixel(surface,cx+x, cy+y,COLOR_WHITE);
            }
        }
    // Center
    r=radius;
    for(int y=-r; y<=r; y++)
    for(int x=-r; x<=r; x++)
        if(x*x+y*y <= r*r+r*0.8f)
        {
            if (getpixel(surface,cx+x, cy+y)!=0)
            putpixel(surface,cx+x, cy+y,COLOR_TRANSPARENT);
            if (getpixel(collision,cx+x, cy+y)!=0)
            putpixel(collision,cx+x, cy+y,COLOR_TRANSPARENT);
        }
}

void paint_mouse()
{
    if ( SDL_MUSTLOCK(background) )
        if ( SDL_LockSurface(background) < 0 )
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());

    int i1,i2;
    fill_circle(background,collision,event.button.x,event.button.y,40,0);

    if (SDL_MUSTLOCK(background)) SDL_UnlockSurface(background);
}

int main( int argc, char* args[] )
{
    bool running = true;
    bool setup = true;
    bool should_paint = false;
    int reason=0;

    Dot ball;
    Dot2 ball2;

    Timer fps;
    Timer update;
    Timer delta;

    int frame = 0;
    int tic_toc=0;

    if( init() == false )
    {
        std::cout<<"Initialising failed!";
        SDL_Delay(2);
        return 424;
    }
    if( precache() == false )
    {
        std::cout<<"Missing resource files!";
        SDL_Delay(2);
        SDL_Quit();
        return 404;
    }

    std::stringstream caption;
    update.start();
    fps.start();
    delta.start();
    while( running == true )
    {
        ///////////////// INPUTS //////////////////
        while( SDL_PollEvent( &event ) )
        {
            ball.handle_input();
        ball.handle_input2();
            //ball2.handle_input();
            if( event.type == SDL_QUIT )
            {
                running = false;
                reason = 1;
            }
            if( event.type == SDL_MOUSEBUTTONDOWN )
            {
                should_paint = true;
            }
            if( event.type == SDL_MOUSEBUTTONUP )
            {
                should_paint = false;
            }
            switch( event.key.keysym.sym )
            {
                case SDLK_ESCAPE:
                    running = false;
                    reason = 1;
                default: ;
            }
        }
        ///////////////////////////////////////////

        //apply_surface( image, screen, 250, 250 );
        //apply_surface( image2, screen, 0, 0 );


        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGBA( screen->format, 0xBB, 0xFF, 0xFF, 0 ) );
        apply_surface( background, screen, 0, 0 );

        if (should_paint)
        {
            //if(tic_toc==1) // draw for one(2nd) frame, out of 5
                paint_mouse();
        }

        if( !setup )
        {
            ball.think( ball2.c,delta.get_ticks() );
            delta.start();

            ball2.move(  );
            ball2.draw();
        }
        else
        {
            SDL_Surface *collision = NULL;

            setup = false;
        }
        if( SDL_Flip( screen ) == -1 )
        {
            return 404;
        }

        frame++;
        if (tic_toc<1) tic_toc++;
        else tic_toc=0;

//        if( update.get_ticks() > 200 )
//        {
//            std::stringstream caption;
//            caption << "Average Frames Per Second: " << frame / ( fps.get_ticks() / 1000.f );
//            SDL_WM_SetCaption( caption.str().c_str(), NULL );
//            frame=0;
//            //fps.start();
//            update.start();
//        }
/// FPS CAP
//        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
//        {
//            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
//            fps.start();
//        }
    }
//  delete[] temp_image;
//  delete[] image;
//  fclose(inbin);
    clean_up();

    // 0   = X button
    // 1   = other legit exit
    // 404 = missing resources
    // 424 = initialising failed
    return reason;
}
