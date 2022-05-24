#include "startingwindow.h"
#include "gamewindow.h"

#include <QApplication>
#include <QPushButton>
#include <QMainWindow>

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    StartingWindow w;
    GameWindow g;
    w.setGameWindow(g);
    g.setStartingWindow(w);
    g.hide();
    w.show();
    return app.exec();
}
