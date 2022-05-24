#include "startingwindow.h"
#include "gamewindow.h"
#include <QApplication>
#include <QPushButton>
#include <QFormLayout>
#include <QInputDialog>
#include <QSpinBox>
#include <QComboBox>

StartingWindow::StartingWindow(QWidget *parent)
    : QWidget{parent}
{

    setFixedSize(400, 300);

    startButton = new QPushButton("Rozpocznij grę", this);
    quitButton = new QPushButton("Wyjdź", this);
    numberOfPlayers = new QSpinBox();
    difficulty = new QComboBox();
    settings = new QFormLayout();

    numberOfPlayers->setMinimum(6);
    numberOfPlayers->setMaximum(13);
    difficulty->addItem("Łatwy");
    difficulty->addItem("Średni");
    difficulty->addItem("Trudny");

    settings->addRow("Liczba graczy:", numberOfPlayers);
    settings->addRow("Trudność:", difficulty);

    quitButton->setGeometry(100, 200, 200, 80);
    startButton->setGeometry(100, 80, 200, 80);
    setLayout(settings);

    connect(startButton, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(quitButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
}

void StartingWindow::startGame()
{
    QString diff = difficulty->currentText();
    int num = numberOfPlayers->value();
    game->setParameters(diff, num);
    //game-startgame
    game->show();
    this->hide();
    game->startGame();
}
