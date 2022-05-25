#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H


#include <QMainWindow>
#include <QWidget>
#include "game.h"


class StartingWindow;
class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);

signals:

private:
    int numberOfPlayers;
    QString difficulty;
    StartingWindow *starting;

    GameMaster gameMaster;
    std::shared_ptr<HumanPlayer> player;

public:
    void setParameters(QString diff, int num);
    void setStartingWindow(StartingWindow &w);
    void startGame();
public slots:
    void askForMove(QString question, QStringList answers);
    void notify(QVector<int> players, QString message);
};

#endif // GAMEWINDOW_H
