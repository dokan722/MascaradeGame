#ifndef GAME_H
#define GAME_H

#include <QtCore>
#include <iostream>

enum Decision
{
    NONE,
    CHECK,
    CHANGE,
    USE
};

class GameMaster;
class Player
{
    GameMaster *gameMaster;
public:
    Player() {}
    Player(GameMaster *gm)
    {
        gameMaster = gm;
    }
    virtual QString saySomething(QString question, QStringList answers) = 0;
    virtual void notify(QVector<int> player, QString card) = 0;
    virtual Decision chooseMove() = 0;
//    virtual void checkSelf() = 0;
//    virtual void changeRole(int playerId) = 0;
//    virtual QString chooseRoleToPlay() = 0;
//    virtual QString answerWithRole() = 0;
//    virtual bool questionRole(QString role) = 0;
//    virtual QVector<int> chooseTargets(int numOfTargets) = 0;
//    virtual void getOtherRole(int playerId, QString role) = 0;
//    virtual void playerGetsMoney(int playerId, int amount) = 0;
//    virtual void potentialExchange(int playerId1, int playerId2) = 0;
//    virtual void cashExchange(int playerId1, int playerId2) = 0;
};


class ComputerPlayer: public Player
{
public:
    QString saySomething(QString question, QStringList answers) override
    {
        return "foobar";
    }
    void notify(QVector<int>, QString) override
    {
    }
    Decision chooseMove() override
    {
        return Decision::CHECK;
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

    void notify(QVector<int> players, QString card) override
    {
        mutex.lock();
        emit notified(players, card);
        bed.wait(&mutex);
        mutex.unlock();
    }

    Decision chooseMove() override
    {
        QStringList answers = {"Sprawdź swoją kartę",
                               "Wykonaj potencjalną zamianę",
                               "Zadeklaruj rolę"};
        QString decision = saySomething("Wybierz co chcesz zrobić", answers);
        if (decision == answers[0])
            return Decision::CHECK;
        else if (decision == answers[1])
            return Decision::CHANGE;
        else if (decision == answers[2])
            return Decision::USE;
        return Decision::NONE;
    }

private:
    QMutex mutex;
    QWaitCondition bed;
    QString move;

signals:
    void waitForMove(QString question, QStringList answers);
    void notified(QVector<int> players, QString card);
};



class GameMaster: public QThread
{
    Q_OBJECT
private:
    QString difficulty;
    int numberOfPlayers;
public:
    GameMaster(QObject *parent = nullptr);

    void setParams(QString diff, int num)
    {
        difficulty = diff;
        numberOfPlayers = num;
    }

    void run() override
    {
        int player = 0;
        players.push_back(humanPlayer);
        for (int i = 1; i < numberOfPlayers; ++i)
            players.push_back(std::make_shared<ComputerPlayer>());
        while (true)
        {
            Decision decision = players[player]->chooseMove();
            if (decision == Decision::CHECK)
                std::cerr << "Check" << std::endl;
            else if (decision == Decision::CHANGE)
                std::cerr << "Change" << std::endl;
            else if (decision == Decision::USE)
                std::cerr << "Use" << std::endl;
            else
                std::cerr << "Oof" << std::endl;
            player = (player + 1) % numberOfPlayers;
        }
    }

    std::shared_ptr<HumanPlayer> getHumanPLayer()
    {
        return humanPlayer;
    }

    void informAllDecision(Decision decision)
    {

    }

    void informAllRole(QString role)
    {

    }

    void questionAll(QString role)
    {

    }

    void informAllMoney(int playerId, int amount)
    {

    }

    void informAllPotentialExchange(int playerId1, int playerId2, int not_target = -1)
    {

    }

    void informAllCashExchange(int playerId, int playerId2, int not_target = -1)
    {

    }

private:
    int current_player = 0;
    std::shared_ptr<HumanPlayer> humanPlayer = std::make_shared<HumanPlayer>();
    QVector<std::shared_ptr<Player>> players;
};

#endif // GAME_H
