#include "role.h"

Role::Role()
{


}

void Spy::usePower(int playerId)
{
    auto player = gameMaster->players[playerId];
    int target = player->chooseTargets(1)[0];
    player->getOtherRole(target, gameMaster->playerRoles[target]);
    player->changeRoles(target);
    gameMaster->informAllPotentialExchange(playerId, target, playerId);
}

void Bishop::usePower(int playerId)
{
    auto player = gameMaster->players[playerId];
    auto richest = gameMaster->richest(playerId);
    int target;
    if (richest.size() > 1)
    {
        target = player->chooseTargets(1, &richest)[0];
    }
    else
        target = richest[0];
    gameMaster->informAllMoney(playerId, amount);
    gameMaster->informAllMoney(target, -amount);
    gameMaster->transferMoney(target, playerId, amount);
}

void Fool::usePower(int playerId)
{
    auto player = gameMaster->players[playerId];
    auto targets = player->chooseTargets(2);
    player->potentialExchange(targets[0], targets[1]);
    gameMaster->informAllPotentialExchange(targets[0], targets[1], playerId);
    gameMaster->addMoney(playerId, amount);
    gameMaster->informAllMoney(playerId, amount);
}

