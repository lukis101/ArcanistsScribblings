#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "types.h"
#include "classes/entity/c_baseentity.h"

class CPlayer : public CBaseEntity
{
    protected:
    bool local;
    bool active, input;

    float walktime, falltime;
    bool walking, falling, drowning, colliding;
    bool onground;
    bool stunned;

    Vector_f velocity;
    Vector_f accel;

    short team;
    Color_c tcolor;
    Color_c colors[3];

    std::string name;
    bool mirrored;
    int health;
    short minions;

    public:

    CPlayer();
    ~CPlayer();

    void Think();
    void Render();
};

#endif
