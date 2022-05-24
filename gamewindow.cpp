#include "gamewindow.h"
#include "startingwindow.h"
#include <QWidget>

GameWindow::GameWindow(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(400, 400);
}

void GameWindow::setStartingWindow(StartingWindow &w)
{
    starting = &w;
}

void GameWindow::setParameters(QString diff, int num)
{
    difficulty = diff;
    numberOfPlayers = num;
}
