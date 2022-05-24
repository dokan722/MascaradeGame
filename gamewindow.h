#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H


#include <QWidget>

class StartingWindow;
class GameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);

signals:

private:
    int numberOfPlayers;
    QString difficulty;
    StartingWindow *starting;

public:
    void setParameters(QString diff, int num);
    void setStartingWindow(StartingWindow &w);

};

#endif // GAMEWINDOW_H
