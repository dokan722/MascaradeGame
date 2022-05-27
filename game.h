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
protected:
    GameMaster *gameMaster;
    int myId;
public:
    Player() {}

    void assign(GameMaster *gm, int id)
    {
        gameMaster = gm;
        myId = id;
    }

    virtual QString saySomething(QString question, QStringList answers) = 0;
    virtual void notify(QVector<int> player, QString card) = 0;
    virtual Decision chooseMove() = 0;
    virtual void checkSelf() = 0;
    virtual void changeRoles(int playerId) = 0;
    virtual void changeRoles(int playerId1, int playerId2) = 0;
    virtual QString chooseRoleToPlay() = 0;
    virtual QString answerWithRole() = 0;
    virtual bool questionRole(QString role) = 0;
    virtual QVector<int> chooseTargets(int numOfTargets) = 0;
    virtual void getOtherRole(int playerId, QString role) = 0;
    virtual void playerGetsMoney(int playerId, int amount) = 0;
    virtual void potentialExchange(int playerId1, int playerId2) = 0;
    virtual void cashExchange(int playerId1, int playerId2) = 0;
    virtual void informDecision(int playerId, Decision dec) = 0;
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

    void checkSelf() override
    {

    }

    void changeRoles(int playerId) override
    {

    }

    void changeRoles(int playerId1, int playerId2) override
    {

    }

    QString chooseRoleToPlay() override
    {
        return "Król";
    }

    QString answerWithRole() override
    {
        return "Wieśniak";
    }

    bool questionRole(QString role) override
    {
        return false;
    }

    QVector<int> chooseTargets(int numOfTargets) override
    {
        QVector<int> targets;
        for (int i = 0; i < numOfTargets; ++i)
            targets.push_back(0);
        return targets;
    }

    void getOtherRole(int playerId, QString role) override
    {

    }

    void playerGetsMoney(int playerId, int amount) override
    {

    }

    void potentialExchange(int playerId1, int playerId2) override
    {

    }

    void cashExchange(int playerId1, int playerId2) override
    {

    }

    void informDecision(int playerId, Decision dec) override
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

    void notify(QVector<int> players, QString msg) override
    {
        mutex.lock();
        emit notified(players, msg);
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

    void checkSelf() override;
    void changeRoles(int playerId) override;
    void changeRoles(int playerId1, int playerId2) override;
    QString chooseRoleToPlay() override;
    QString answerWithRole() override {return chooseRoleToPlay();}
    bool questionRole(QString role) override;
    QVector<int> chooseTargets(int numOfTargets) override;
    void getOtherRole(int playerId, QString role) override;
    void playerGetsMoney(int playerId, int amount) override;
    void potentialExchange(int playerId1, int playerId2) override;
    void cashExchange(int playerId1, int playerId2) override;
    void informDecision(int playerId, Decision dec) override;

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


    QVector<QStringList> rolesMatrix = {
        {"Sędzia", "Biskup", "Król", "Królowa", "Wiedźma", "Oszust"},
        {"Sędzia", "Biskup", "Król", "Królowa", "Złodziej", "Wiedźma", "Szpieg",},
        {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Wieśniak"},
        {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Wieśniak", "Oszust"},
        {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Szpieg", "Wieśniak", "Oszust"},
        {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Szpieg", "Wieśniak", "Oszust", "Inkwizytor"},
        {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Szpieg", "Wieśniak", "Oszust", "Inkwizytor", "Wdowa"},
        {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Złodziej", "Wiedźma", "Szpieg", "Wieśniak", "Oszust", "Inkwizytor", "Wdowa"}
    };
public:
    QStringList rolesInPlay;
    QStringList playerRoles;
    QString difficulty;
    int numberOfPlayers;

    GameMaster(QObject *parent = nullptr);

    void setParams(QString diff, int num)
    {
        difficulty = diff;
        numberOfPlayers = num;
        int row = num - 6;
        rolesInPlay = rolesMatrix[row];
        //std::copy(rolesInPlay.begin(), rolesInPlay.end(), playerRoles);
        playerRoles = rolesInPlay;
        std::srand ( unsigned ( std::time(0) ) );
        std::random_shuffle(playerRoles.begin(), playerRoles.end());
    }

    void run() override
    {
        int player = 0;
        players.push_back(humanPlayer);
        humanPlayer->assign(this, 0);
        for (int i = 1; i < numberOfPlayers; ++i)
        {
            players.push_back(std::make_shared<ComputerPlayer>());
            players[i]->assign(this, i);
        }
        while (true)
        {
            humanPlayer->saySomething("takie role", rolesInPlay);
            humanPlayer->saySomething("takie role wymieszane", playerRoles);
        }
    }

    std::shared_ptr<HumanPlayer> getHumanPLayer()
    {
        return humanPlayer;
    }

    void informAllDecision(int playerId, Decision decision, int notTarget = -1)
    {
        for (int i = 0; i < numberOfPlayers; ++i)
        {
            if (i != playerId)
                players[i]->informDecision(playerId, decision);
        }
    }

    void informAllRole(int playerId, QString role)
    {
        for (int i = 0; i < numberOfPlayers; ++i)
            players[i]->getOtherRole(playerId, role);
    }

    void questionAll(QString role, int notTarget = -1)
    {
        for (int i = 0; i < numberOfPlayers; ++i)
        {
            if (i != notTarget)
                players[i]->questionRole(role);
        }
    }

    void informAllMoney(int playerId, int amount)
    {
        for (int i = 0; i < numberOfPlayers; ++i)
            players[i]->playerGetsMoney(playerId, amount);
    }

    void informAllPotentialExchange(int playerId1, int playerId2, int notTarget = -1)
    {
        for (int i = 0; i < numberOfPlayers; ++i)
        {
            if (i != notTarget)
                players[i]->potentialExchange(playerId1, playerId2);
        }
    }

    void informAllCashExchange(int playerId1, int playerId2, int notTarget = -1)
    {
        for (int i = 0; i < numberOfPlayers; ++i)
        {
            if (i != notTarget)
                players[i]->cashExchange(playerId1, playerId2);
        }
    }

private:
    int current_player = 0;
    std::shared_ptr<HumanPlayer> humanPlayer = std::make_shared<HumanPlayer>();
    QVector<std::shared_ptr<Player>> players;
};

#endif // GAME_H
