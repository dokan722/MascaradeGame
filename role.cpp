#include "role.h"
#include "game.h"

void Spy::usePower(int playerId)
{
    auto player = gameMaster->players[playerId];
    int target = player->chooseTargets(1, "Spy")[0];
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
        target = player->chooseTargets(1,"Bishop", &richest)[0];
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
    auto targets = player->chooseTargets(2, "Fool");
    player->changeRoles(targets[0], targets[1]);
    gameMaster->informAllPotentialExchange(targets[0], targets[1], playerId);
    gameMaster->informAllMoney(playerId, amount);
    gameMaster->addMoney(playerId, amount);

}

void Inquisitor::usePower(int playerId)
{
    auto player = gameMaster->players[playerId];
    int targetId = player->chooseTargets(1, "Inquisitor")[0];
    auto target = gameMaster->players[targetId];
    QString answer = target->answerWithRole();
    gameMaster->informAllClaimRole(targetId, answer);
    QString actualRole = gameMaster->playerRoles[targetId];
    gameMaster->informAllRole(targetId, actualRole);
    if (answer != actualRole)
    {
        gameMaster->informAllMoney(playerId, amount);
        gameMaster->informAllMoney(targetId, -amount);
        gameMaster->addMoney(playerId, amount);
        gameMaster->addMoney(targetId, -amount);
    }
}

void Judge::usePower(int playerId)
{
    int amount = gameMaster->bank;
    gameMaster->informAllMoney(playerId, amount);
    gameMaster->addMoney(playerId, amount);
    gameMaster->bank = 0;
}

void Peasant::usePower(int playerId)
{
    int amount = gameMaster->peasants;
    gameMaster->informAllMoney(playerId, amount);
    gameMaster->addMoney(playerId, amount);
}

void Queen::usePower(int playerId)
{
    gameMaster->informAllMoney(playerId, amount);
    gameMaster->addMoney(playerId, amount);
}

void Roi::usePower(int playerId)
{
    gameMaster->informAllMoney(playerId, amount);
    gameMaster->addMoney(playerId, amount);
}

void Witch::usePower(int playerId)
{
    auto player = gameMaster->players[playerId];
    int target = player->chooseTargets(1, "Witch")[0];
    gameMaster->informAllCashExchange(playerId, target);
    std::swap(gameMaster->playerMoney[playerId], gameMaster->playerMoney[target]);
}

void Cheat::usePower(int playerId)
{
    gameMaster->winCon = 10;
    gameMaster->cheater = playerId;
}

void Widow::usePower(int playerId)
{
    gameMaster->playerMoney[playerId] = std::max(10, gameMaster->playerMoney[playerId]);
}

void Thief::usePower(int playerId)
{
    int num = gameMaster->numberOfPlayers;
    int targetId1 = (playerId + 1) % num;
    int targetId2 = (playerId - 1 + num) % num;
    gameMaster->informAllMoney(targetId1, -1);
    gameMaster->informAllMoney(targetId2, -1);
    gameMaster->informAllMoney(playerId, 2);
    gameMaster->addMoney(targetId1, -1);
    gameMaster->addMoney(targetId2, -1);
    gameMaster->addMoney(playerId, 2);
}
























