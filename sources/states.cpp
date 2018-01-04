
#include "SDL/SDL.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>

#include "globals.h"
#include "states.h"
#include "resources.h"
#include "glsl.h"

#include "functions/image_proc.h"
#include "functions/rendering.h"
#include "functions/math.h"
#include "functions/entity.h"

char stateID = STATE_GAME;
char nextState = STATE_GAME;

bool IsInMap(int x, int y, CMap* Map);
void UpdRect(Rect_i rect, int w, int h);

GameState *currentState = NULL;

void Handle_States()
{
    char curstate = currentState->id;
    //If the state needs to be changed
    if( ( curstate != nextState) )
    {
        if( nextState != STATE_EXIT )
        {
            delete currentState;
        }
        switch( nextState )
        {
            case STATE_TITLE:
                currentState = new Title();
                break;
            case STATE_GAME:
                currentState = new Game();
                break;
        }
        stateID = nextState;
        nextState = STATE_NULL;
    }
}

void set_next_state( int newState )
{
    if( nextState != STATE_EXIT )
    {
        nextState = newState;
    }
}


/// //////////////////////////////////////
/// MAP MANAGER
void CMap::ApplyToTerrain(  )
{
};
void CMap::DamageTerrain( Vector_f pos, float radius )
{
    if ( (int(pos.x+radius)<side_w) || (int(pos.x-radius)>=side_w+terrain_w) )
        return;
    if ( (int(pos.y+radius)<side_h) || (int(pos.y-radius)>=side_h+terrain_h) )
        return;

    Color_c color_grey{120,120,120,255};
    Rect_i delta{0,0,terrain_w,terrain_h};

    /// array pos = ( (y-1)*w + x-1 )*4

    int x1 = int(pos.x);
    int y1 = int(pos.y);

    // Outline
    int borders=5;
    int x=0,y=0,r=int(radius)+borders;
    for(int y=-r; y<=r; y++)
        for(int x=-r; x<=r; x++)
            if(x*x+y*y <= r*r+r*0.8f)
                if ( IsInMap(x1+x, y1+y, this) )
                {
                    if (y1+y<y1)
                        WriteTerrainPixel(x1+x, y1+y, color_grey);
                    else
                        WriteTerrainPixel(x1+x, y1+y, color_white);
                }
    // Center
    r-=borders;
    for(int y=-r; y<=r; y++)
        for(int x=-r; x<=r; x++)
            if( x*x+y*y <= r*r+r*0.8f )
                if ( IsInMap(x1+x, y1+y, this) )
                {
                    WriteTerrainPixel(x1+x, y1+y, color_transp);
                }

    /// Update texture buffer
    glBindTexture( GL_TEXTURE_2D, terrainid );
        UpdRect(delta, terrain_w, terrain_h);
        glTexSubImage2D(GL_TEXTURE_2D, 0, delta.x, delta.y, delta.w, delta.h, GL_RGBA, GL_UNSIGNED_BYTE, terraindata);
    glBindTexture( GL_TEXTURE_2D, 0 );
};
bool CMap::WriteTerrainPixel( int x, int y, Color_c color, bool replace )
{
    int x1 = x-side_w;
    int y1 = y-side_h;
    int N = ((y1*terrain_w)+x1)*4;
    if ( (terraindata[N+3]==0) && !(replace) ) return false;
    terraindata[N]=color.r;
    terraindata[N+1]=color.g;
    terraindata[N+2]=color.b;
    terraindata[N+3]=color.a;
    if (color.a==0)
        collisiondata[N/4]=false;
    else
        collisiondata[N/4]=true;

    return true;
}

/// //////////////////////////////////////////////////////////////////////
/// BASE GAMESTATE
CMap* GameState::GetMap()
{
    return Map;
}
Vector_f GameState::GetCursorPos()
{
    Vector_f pos = currentState->camera->GetFocus();
    pos.z = camera->pos.z;
    pos.x += (Mouse.x - currentState->camera->size.x/2.f)/pos.z;
    pos.y += (Mouse.y - currentState->camera->size.y/2.f)/pos.z;
    return pos;
}

/// //////////////////////////////////////////////////////////////////////
/// TITLE
Title::Title()
{
    id = STATE_TITLE;
    //load_image( "textures/splash.png" );
    time_running = 0;

    camera = new CCamera;
};
Title::~Title()
{
    delete camera;
    EntityIds = 1;
    GuiIds    = 1;
    TimerIds  = 1;
};

void Title::logic()
{
};
void Title::render()
{
    //SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGBA( screen->format, 0xBB, 0xFF, 0xFF, 0 ) );
    //apply_surface( splash, screen, 0, 0 );
};

/// //////////////////////////////////////////////////////////////////////
/// GAME
GLuint FramebufferName = 0;
GLuint renderedTexture = 0;

Game::Game()
{
    id = STATE_GAME;

    EntityList = new std::vector< BaseClass >;
//    GuiList    = new std::vector< BaseClass >;
//    TimerList  = new std::vector< Timer     >;
    EntityList->reserve(200);
//    GuiList   ->reserve(20);
//    TimerList ->reserve(20);

    camera = new CCamera;

/// ///////////////////////////////////////////////////////////////////////
/// TERRAIN LOADING
    Map = new CMap;
        Map->mapid = 1;
        Map->terrainid = MAT_TERRAIN_GRASSLAND;
        Map->buffer_bkg = 0;
        Map->bkgid = MAT_BACKGROUND_GRASSLAND;
        Map->buffer_terrain = 0;

        Map->viewablesides = true;
        Map->side_w = 640;
        Map->side_h = 480;
        Map->ocean  = 100;
        Map->bkg_w = 480;
        Map->bkg_h = 235;
        Map->parallax = 0.6f;

    const char filename[] = "textures/terrain/grassland/terrain-2.png";
    glGenTextures( 1, &(Map->terrainid) );
    LoadImage(filename, Map->terrainid, MAT_TERRAIN_GRASSLAND);

    glBindTexture( GL_TEXTURE_2D, Map->terrainid );

    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &(Map->terrain_w) );
    glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &(Map->terrain_h) );

    Map->terraindata = new unsigned char[Map->terrain_w*Map->terrain_h*4];
    Map->collisiondata = new bool[Map->terrain_w*Map->terrain_h];

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, Map->terraindata);

    for (unsigned i=0; i<(Map->terrain_w*Map->terrain_h); i++)
        Map->collisiondata[i] = ( (Map->terraindata[i*4])>1 ) ? true : false;

    glBindTexture( GL_TEXTURE_2D, 0 );
/// ///////////////////////////////////////////////////////////////////////


/// ///////////////////////////////////////////////////////////////////////
/// FRAMEBUFFER STUFF
// TO-DO: move to main initgl()
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_W, SCREEN_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );

    glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0 );

    // Set the list of draw buffers.
    //GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    //glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
/// ///////////////////////////////////////////////////////////////////////

    Button btn;
    btn = CreateEntity(btn);
    btn->SetPos( 128.f, 128.f );
    btn->SetImage( GetMaterial(MAT_COG_BIG), BUTTON_NORMAL,  BUTTON_CIRCLE );
    btn->SetImage( GetMaterial(MAT_COG_BIG), BUTTON_HOVERED, BUTTON_CIRCLE );
    btn->SetImage( GetMaterial(MAT_COG_BIG), BUTTON_PRESSED, BUTTON_CIRCLE );
    //DeleteEntity(btn);

    Player p1;
    p1 = CreateEntity(p1);
    p1->SetPos( {720.f,420.f} );
    camera->Center( {720.f,420.f} );
    //p2 = CreateEntity(p2);
    //p2->SetPos(300.f,300.f);

};
Game::~Game()
{
    glDeleteBuffers( 1, &(Map->buffer_bkg) );
    glDeleteBuffers( 1, &(Map->buffer_terrain) );
    delete camera;
    delete EntityList;
    EntityIds = 1;
    GuiIds    = 1;
    TimerIds  = 1;
};

void Game::logic()
{
    /// Camera logic
    float delta = LogicTimer->GetSeconds();
    float scroll_rate = (400.f*camera->pos.z)*delta;
    float zoom_rate = (3.f*delta)*camera->pos.z;
    float zoom_pending = 0.f;

    // 1) Input
    // A) Translation
    if ( Keyboard[KEY_W].state ) camera->pos.y -= scroll_rate;
    if ( Keyboard[KEY_A].state ) camera->pos.x -= scroll_rate;
    if ( Keyboard[KEY_S].state ) camera->pos.y += scroll_rate;
    if ( Keyboard[KEY_D].state ) camera->pos.x += scroll_rate;
    if ( Mouse.middle.state )
    {
        float dist_x = float(Mouse.middle.x-Mouse.x);
        float dist_y = float(Mouse.middle.y-Mouse.y);
        camera->Pan( dist_x, dist_y );
    }
    else camera->backedup = false;

    // B) Zoom
    if ( Keyboard[KEY_CTRL].state )
    {
        if (!( (zoom_pending==1.f || zoom_pending==0.f) && camera->pos.z==1.f ))
            zoom_pending = 1.f-camera->pos.z;
    }
    if ( Mouse.WheelUp )
    {
        Mouse.WheelUp = false;
        zoom_pending -= zoom_rate;
    }
    if ( Mouse.WheelDown )
    {
        Mouse.WheelDown = false;
        zoom_pending += zoom_rate;
    }
    ////////////////////////////////
    if ( Mouse.right.state )
    {
        //camera->Center( GetCursorPos() );
    }
    ///////////////////////////////

    // 2) First limiting pass (zoom)
    if ( camera->pos.z+zoom_pending < 0.5f )
    {
        zoom_pending = (0.5f-camera->pos.z);
    }
    else if ( camera->pos.z+zoom_pending > 2.5f )
    {
        zoom_pending = (2.5f-camera->pos.z);
    }

    // 3) Apply zoom
    Vector_f curfocus = camera->GetFocus(); //unscaled world
    Vector_f cursorpos = currentState->GetCursorPos();
    camera->pos.z += zoom_pending;

    // 4) Second limiting pass (zoom)
    float arena_h = (Map->side_h +Map->ocean +Map->terrain_h);
    float arena_w = Map->terrain_w;
    if (Map->viewablesides) arena_w += Map->side_w*2.0f;

    if ( camera->size.x > arena_w*camera->pos.z )
    {
        camera->pos.z = camera->size.x/arena_w;
        zoom_pending = 0.f;
    }
    if ( camera->size.y > arena_h*camera->pos.z )
    {
        camera->pos.z = camera->size.y/arena_h;
        zoom_pending = 0.f;
    }

    // 5) View adjusting
    if ( zoom_pending != 0.f )
    {
        // Center to previous (unscaled world) coords
        camera->Center( curfocus );

        Vector_f newcursorpos = currentState->GetCursorPos();
        float thex = (cursorpos.x-newcursorpos.x);
        float they = (cursorpos.y-newcursorpos.y);

        camera->Center( {curfocus.x+thex, curfocus.y+they} );
    }

    // 6) Third limiting pass (translation)
    float camborder_x = arena_w*camera->pos.z-camera->size.x;
    float camborder_y = arena_h*camera->pos.z-camera->size.y;
    if ( camera->pos.x < 0.f ) camera->pos.x = 0.f;
    else if ( camera->pos.x > camborder_x ) camera->pos.x = camborder_x;
    if ( camera->pos.y < 0.f ) camera->pos.y = 0.f;
    else if ( camera->pos.y > camborder_y ) camera->pos.y = camborder_y;

    /// Per-entity logic
    unsigned ents = EntityList->size();
    for ( unsigned i=0; i<ents; i++ )
    {
        if ( EntityList->at(i) != NULL )
        {
            EntityList->at(i)->Think();
        }
    }

    /// /////////////////////////////////////////////
    /// DEBUG: COLLISION
//    if( IsColliding( GetCursorPos() ) )
//        std::cout<<"Colliding\n";
//    else std::cout<<"Not colliding\n";

};
void Game::render()
{
    glClear( GL_COLOR_BUFFER_BIT );

    glEnableVertexAttribArray( sh_v_pos );
    glEnableVertexAttribArray( sh_v_uv );


    /// 1) Background
    // Diffrent matrices for parallax calculations
    glm::mat4 TransMat = glm::translate( glm::vec3( -camera->pos.x*(Map->parallax), -camera->pos.y*(Map->parallax), 0.f ) );
    glm::mat4 ScaleMat = glm::scale( glm::vec3( 1.f, 1.f, 1.f ) );
    glUniformMatrix4fv(	sh_projmat, 1, GL_FALSE, glm::value_ptr(ProjMat * TransMat * ScaleMat) );

    float arena_h = (Map->side_h +Map->ocean +Map->terrain_h)*camera->pos.z;
    float arena_w = 0.f;
    if (Map->viewablesides) arena_w = (Map->side_w*2.0f + Map->terrain_w)*camera->pos.z;
    else arena_w = (Map->terrain_w)*camera->pos.z;
    if ( arena_w < camera->size.x ) arena_w = camera->size.x;
    if ( arena_h < camera->size.y ) arena_h = camera->size.y;

    float backg_w = (arena_w-camera->size.x)*(Map->parallax)+camera->size.x;
    float backg_h = (arena_h-camera->size.y)*(Map->parallax)+camera->size.y;

    float scale_w = backg_w/float(Map->bkg_w);
    float scale_h = backg_h/float(Map->bkg_h);
    float scale = scale_w;
    if (scale_h > scale_w) scale = scale_h;

    // First pass - horizontal blur
    glBindFramebuffer(GL_FRAMEBUFFER, 1);
    RenderMaterial( Map->bkgid, 1, vector_fzero, scale );

    // Normal matrices
    TransMat = glm::translate( glm::vec3( -camera->pos.x, -camera->pos.y, 0.f ) );
    ScaleMat = glm::scale( glm::vec3( camera->pos.z, camera->pos.z, 1.f ) );
    glUniformMatrix4fv(	sh_projmat, 1, GL_FALSE, glm::value_ptr(ProjMat * TransMat * ScaleMat) );

    // Second pass - vertical blur
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RenderTexture(renderedTexture, {camera->pos.x/camera->pos.z, camera->pos.y/camera->pos.z}, NULL, 1.f/camera->pos.z, 3, true);

    /// 2) Terrain
    Vector_f terrainpos = { float( (Map->viewablesides) ? Map->side_w : 0.f ), float(Map->side_h) };
    RenderTexture( Map->terrainid, terrainpos, NULL, 1.f, 0);

    /// 3) TESTS
    RenderMaterial( MAT_CHAR_HAT_1, 1, { Map->side_w/2.f, Map->side_h/2.f }, 2.f );
    RenderTexture( GetMaterialID(MAT_CHAR_BODY_1), {150.f, 100.f}, NULL, 2.f, 1);

    Rect_f oceanpos = { 0.0, (Map->side_h+Map->terrain_h)-5.f, (Map->side_w*2.0f + Map->terrain_w), Map->ocean+5.f };
    RenderGradient( oceanpos, { 19, 39, 40, 255 }, { 90, 213, 217, 255 }, GRADIENT_VERTICAL );

    RenderColor( { 200.f, Map->side_h/2.f, 100.f, 100.f }, 0, 255, 255, 150 );

    /// 4) Entities
    int ents = EntityList->size();
    for ( int i=0; i<ents; i++ )
    {
        if ( EntityList->at(i) != NULL )
        {
            EntityList->at(i)->Render();
        }
    }

    /// 4) End
    SDL_GL_SwapBuffers();
};
bool Game::IsColliding( int slot, Vector_f offset )
{
    int siz = EntityList->size();
    CBaseClass* temp = EntityList->at(slot).get();
    CBaseEntity* ent1;
    if ( temp->IsCollidable() )
        ent1 = dynamic_cast<CBaseEntity*>(temp);
    else return false;

    int x = int( ent1->GetPos().x + offset.x );
    int y = int( ent1->GetPos().y + offset.y );
    int rad = int( ent1->radius );

    /// Ent to ent
    for (unsigned i=0; i<(EntityList->size()); i++)
    {
        CBaseClass* temp = EntityList->at(i).get();
        if ( i==slot ) continue;
        if ( temp->IsCollidable() )
        {
            CBaseEntity* ent2 = dynamic_cast<CBaseEntity*>(temp);
            if ( distance(x, y, ent2->GetPos().x, ent2->GetPos().y)< rad+ent2->radius )
                return true;
        }
    }

    /// Terrain
    int x1=0,y1=0;
    for(int y1=-rad; y1<=rad; y1++)
        for(int x1=-rad; x1<=rad; x1++)
            if(IsInMap(x+x1, y+y1, Map))
                if(x1*x1+y1*y1 <= rad*rad+rad*0.8f)
                {
                    int N = ((y+y1-Map->side_h)*Map->terrain_w)+x+x1-Map->side_w;
                    if ( Map->collisiondata[N] ) return true;
                }

    return false;
}

//////////////////////////////////////////////
// Helpers
bool IsInMap(int x, int y, CMap* Map)
{
    int x1 = x - Map->side_w;
    int y1 = y - Map->side_h;
    if ( (x1>-1) && (x1<Map->terrain_w) && (y1>-1) && (y1<Map->terrain_h) ) return true;
    return false;
}
void UpdRect(Rect_i rect, int w, int h)
{
    if ( rect.x<0 ) rect.x=0;
    else if ( rect.x>=w ) rect.x=w-1;
    if ( rect.y<0 ) rect.y=0;
    else if ( rect.y>=h ) rect.y=h-1;

    if ( rect.w>=w ) rect.w=w-1;
    if ( rect.h>=h ) rect.h=h-1;
}

