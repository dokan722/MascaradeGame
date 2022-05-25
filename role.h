#ifndef ROLE_H
#define ROLE_H
#include "game.h"

class Role
{
    GameMaster *gameMaster;
public:
    Role();
    Role(GameMaster *gameMaster) : gameMaster(gameMaster) {}
    void usePower(int playerId);
};

#endif // ROLE_H
