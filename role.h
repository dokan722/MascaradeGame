#ifndef ROLE_H
#define ROLE_H

class GameMaster;
class Role
{
protected:
    GameMaster *gameMaster;
public:
    Role() {}
    Role(GameMaster *gameMaster) : gameMaster(gameMaster) {}
    virtual void usePower(int playerId) = 0;
};

class Spy : public Role
{
    using Role::Role;
    void usePower(int playerId) override;
};

class Bishop : public Role
{
    using Role::Role;
    int amount = 2;
    void usePower(int playerId) override;
};

class Fool : public Role
{
    using Role::Role;
    int amount = 1;
    void usePower(int playerId) override;
};

class Inquisitor : public Role
{
    using Role::Role;
    int amount = 4;
    void usePower(int playerId) override;
};

class Judge : public Role
{
    using Role::Role;
    void usePower(int playerId) override;
};

class Peasant : public Role
{
    using Role::Role;
    void usePower(int playerId) override;
};

class Queen : public Role
{
    using Role::Role;
    int amount = 2;
    void usePower(int playerId) override;
};

class Roi : public Role
{
    using Role::Role;
    int amount = 3;
    void usePower(int playerId) override;
};

class Witch : public Role
{
    using Role::Role;
    void usePower(int playerId) override;
};

class Cheat : public Role
{
    using Role::Role;
    void usePower(int playerId) override;
};

class Widow : public Role
{
    using Role::Role;
    void usePower(int playerId) override;
};

class Thief : public Role
{
    using Role::Role;
    void usePower(int playerId) override;
};

#endif // ROLE_H
