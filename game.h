#ifndef GAME_H
#define GAME_H

#include <QtCore>
#include <iostream>


class Player
{
public:
    virtual QString saySomething(QString question, QStringList answers) = 0;
    virtual void revealCard(QVector<int> player, QString card) = 0;
};


class ComputerPlayer: public Player
{
public:
    QString saySomething(QString question, QStringList answers) override
    {
        return "foobar";
    }
    void revealCard(QVector<int>, QString) override
    {
    }
};


class HumanPlayer: public QObject, public Player
{
    Q_OBJECT
public:
    void wake()
    {
        mutex.lock();
        bed.wakeAll();
        mutex.unlock();
    }

    void setTheMove(QString move)
    {
        mutex.lock();
        this->move = std::move(move);
        bed.wakeAll();
        mutex.unlock();
    }

    virtual QString saySomething(QString question, QStringList answers) override
    {
        mutex.lock();
        emit waitForMove(question, answers);
        bed.wait(&mutex);
        auto ans = std::move(move);
        mutex.unlock();
        return ans;
    }

    void revealCard(QVector<int> players, QString card) override
    {
        mutex.lock();
        emit cardRevealed(players, card);
        bed.wait(&mutex);
        mutex.unlock();
    }

private:
    QMutex mutex;
    QWaitCondition bed;
    QString move;

signals:
    void waitForMove(QString question, QStringList answers);
    void cardRevealed(QVector<int> players, QString card);
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
            QStringList ans = {"yes", "no"};
            auto move = players[player]->saySomething("test", ans);
            std::cerr << "Move of player" << player << "=" << move.toStdString();
            if (++player == players.size())
            {
                player = 0;
                for (auto p: players)
                {
                    QVector<int> pl = {1, 2};
                    p->revealCard(pl, "xd");
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
