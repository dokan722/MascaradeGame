#include "gamewindow.h"
#include "startingwindow.h"
#include <QWidget>
#include <QInputDialog>


GameWindow::GameWindow(QWidget *parent)
    : QMainWindow{parent}
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

void GameWindow::startGame()
{
    std::cerr << "Starting game..." << std::endl;
    player = gameMaster.getHumanPLayer();
    connect(player.get(), SIGNAL(waitForMove()), this, SLOT(askForMove()));
    connect(player.get(), SIGNAL(cardRevealed(int, QString)), this, SLOT(onCardRevealed(int, QString)));
    gameMaster.start();
}

void GameWindow::askForMove()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("User name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    player->setTheMove(text);
}


void GameWindow::onCardRevealed(int player, QString card)
{
    std::cerr << "Card revealed" << std::endl;
}
