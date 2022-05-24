#ifndef GAME_H
#define GAME_H

#include <QtCore>
#include <iostream>


class Player
{
public:
    virtual QString saySomething() = 0;
    virtual void revealCard(int player, QString card) = 0;
};


class ComputerPlayer: public Player
{
public:
    QString saySomething() override
    {
        return "foobar";
    }
    void revealCard(int, QString) override
    {
    }
};


class HumanPlayer: public QObject, public Player
{
    Q_OBJECT
public:
    void setTheMove(QString move)
    {
        mutex.lock();
        this->move = std::move(move);
        bed.wakeAll();
        mutex.unlock();
    }

    virtual QString saySomething()
    {
        mutex.lock();
        emit waitForMove();
        bed.wait(&mutex);
        auto ans = std::move(move);
        mutex.unlock();
        return ans;
    }

    void revealCard(int player, QString card) override
    {
        emit cardRevealed(player, card);
    }

private:
    QMutex mutex;
    QWaitCondition bed;
    QString move;

signals:
    void waitForMove();
    void cardRevealed(int player, QString card);
};



class GameMaster: public QThread
{
    Q_OBJECT
public:
    GameMaster(QObject *parent = nullptr);

    void run() override
    {
        int player = 0;
        while (true)
        {
            std::cerr << "Waiting for player" << player << std::endl;
            auto move = players[player]->saySomething();
            std::cerr << "Move of player" << player << "=" << move.toStdString();
            if (++player == players.size())
            {
                player = 0;
                for (auto p: players)
                {
                    p->revealCard(0, "xd");
                }
            }
        }
    }

    std::shared_ptr<HumanPlayer> getHumanPLayer()
    {
        return humanPlayer;
    }

private:
    std::shared_ptr<HumanPlayer> humanPlayer = std::make_shared<HumanPlayer>();
    QVector<std::shared_ptr<Player>> players;
};

#endif // GAME_H
