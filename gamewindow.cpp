#include "gamewindow.h"
#include "startingwindow.h"
#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>


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
    connect(player.get(), SIGNAL(waitForMove(QString, QStringList)), this, SLOT(askForMove(QString, QStringList)));
    connect(player.get(), SIGNAL(notified(QVector<int>, QString)), this, SLOT(notify(QVector<int>, QString)));
    gameMaster.setParams(difficulty, numberOfPlayers);
    gameMaster.start();
}

void GameWindow::askForMove(QString question, QStringList answers)
{
    bool ok;
    QString text = QInputDialog::getItem(this, tr("Decyzja"),
                                         question, answers,
                                         0, false, &ok);
    player->setTheMove(text);
}


void GameWindow::notify(QVector<int> players, QString message)
{
    QString text = "";
    if (players.size() > 1)
        text = "Gracze: ";
    else
        text = "Gracz ";
    for (int i: players)
        text += QString::number(i) + ", ";
    text[text.length() - 2] = '.';
    text += message;
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
    player->wake();
}
