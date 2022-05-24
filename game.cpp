#include "game.h"

GameMaster::GameMaster(QObject *parent):
    QThread(parent)
{
    players.push_back(humanPlayer);
    players.push_back(std::make_shared<ComputerPlayer>());
    players.push_back(std::make_shared<ComputerPlayer>());
}
