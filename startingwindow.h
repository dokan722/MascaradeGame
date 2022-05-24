#ifndef STARTINGWINDOW_H
#define STARTINGWINDOW_H
#include "gamewindow.h"

#include <QWidget>

class QPushButton;
class QInputDialog;
class QFormLayout;
class QSpinBox;
class QComboBox;
class StartingWindow : public QWidget
{
    Q_OBJECT
public:
    explicit StartingWindow(QWidget *parent = nullptr);


signals:

public slots:
    void startGame();
    void setGameWindow(GameWindow &w)
    {
        game = &w;
    }
private:
    QPushButton *startButton;
    QPushButton *quitButton;
    QSpinBox *numberOfPlayers;
    QComboBox *difficulty;
    QFormLayout *settings;
    GameWindow *game;


};

#endif // STARTINGWINDOW_H
