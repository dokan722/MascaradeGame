#include "startingwindow.h"
#include "gamewindow.h"

#include <QApplication>
#include <QPushButton>
#include <QMainWindow>
#include <iostream>

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    StartingWindow w;
    GameWindow g;
    w.setGameWindow(g);
    g.setStartingWindow(w);
    g.hide();
    w.show();
    std::cerr << "XDDDDDDD" << std::endl;
    return app.exec();
}
