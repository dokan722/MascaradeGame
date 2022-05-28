#ifndef ROLE_H
#define ROLE_H
#include "game.h"

class Role
{
protected:
    GameMaster *gameMaster;
public:
    Role();
    Role(GameMaster *gameMaster) : gameMaster(gameMaster) {}
    virtual void usePower(int playerId);
};

class Spy : Role
{
    virtual void usePower(int playerId) override;
};

class Bishop : Role
{
    int amount = 2;
    virtual void usePower(int playerId) override;
};

class Fool : Role
{
    int amount = 1;
    virtual void usePower(int playerId) override;
};

class Inquisitor : Role
{
    virtual void usePower(int playerId) override;
};

class Judge : Role
{
    virtual void usePower(int playerId) override;
};

class Peasant : Role
{
    virtual void usePower(int playerId) override;
};

class Queen : Role
{
    virtual void usePower(int playerId) override;
};

class Roi : Role
{
    virtual void usePower(int playerId) override;
};

class Witch : Role
{
    virtual void usePower(int playerId) override;
};

class Cheat : Role
{
    virtual void usePower(int playerId) override;
};

class Widow : Role
{
    virtual void usePower(int playerId) override;
};

class Thief : Role
{
    virtual void usePower(int playerId) override;
};

#endif // ROLE_H
