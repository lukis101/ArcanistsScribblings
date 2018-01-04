
#ifndef STATES_H
#define STATES_H

#include "SDL/SDL.h"
#include "classes/c_timer.h"
#include "classes/c_camera.h"

enum GameStates
{
    STATE_NULL,
    STATE_TITLE,
    STATE_MENU,
    STATE_GAME,
    STATE_EXIT,
};

/// //////////////////////////////////////
/// MAP MANAGER
class CMap
{
    public:
    int mapid;
    unsigned bkgid;
    unsigned buffer_bkg;
    unsigned terrainid;
    unsigned buffer_terrain;

    bool viewablesides;
    int side_w;
    int side_h;
    int ocean;
    unsigned bkg_w;
    unsigned bkg_h;
    int terrain_w;
    int terrain_h;
    float parallax;

    unsigned char *terraindata;
    bool *collisiondata;

    void ApplyToTerrain();
    void DamageTerrain( Vector_f pos, float radius );
    bool WriteTerrainPixel( int x, int y, Color_c color, bool replace=false );
};

/// //////////////////////////////////////
/// BASE STATE
class GameState
{
    public:
    char id;
    CCamera* camera;
    CMap* Map;

    Vector_f GetCursorPos();

    virtual void logic() = 0;
    virtual void render() = 0;
    virtual ~GameState(){};

    CMap* GetMap();
    CCamera* GetCamera();

    virtual bool IsColliding( int slot, Vector_f offset={0,0,0} ){};
};

extern GameState *currentState;
extern char stateID;
extern char nextState;
void set_next_state( int newState );
void Handle_States();

/// //////////////////////////////////////
/// TITLE
class Title : public GameState
{
    private:
    //int splashid;
    Timer timer;
    int time_running;

    public:
    Title();
    ~Title();

    void logic();
    void render();
};

/// //////////////////////////////////////
/// GAME
class Game : public GameState
{
    private:

    public:
    Game();
    ~Game();

    void logic();
    void render();

    bool IsColliding( int slot, Vector_f offset );
};

#endif
