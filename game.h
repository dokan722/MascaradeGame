#ifndef GAME_H
#define GAME_H

#include <QtCore>
#include <iostream>
#include "role.h"

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
    virtual QVector<int> chooseTargets(int numOfTargets, QVector<int> *possibleTargets = nullptr) = 0;
    virtual void getOtherRole(int playerId, QString role) = 0;
    virtual void playerGetsMoney(int playerId, int amount) = 0;
    virtual void potentialExchange(int playerId1, int playerId2) = 0;
    virtual void cashExchange(int playerId1, int playerId2) = 0;
    virtual void informDecision(int playerId, Decision dec) = 0;
    virtual void informRoleClaim(int playerId, QString role) = 0;
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

    QVector<int> chooseTargets(int numOfTargets, QVector<int> *possibleTargets = nullptr) override
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

    void informRoleClaim(int playerId, QString role) override
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
    QVector<int> chooseTargets(int numOfTargets, QVector<int> *possibleTargets = nullptr) override;
    void getOtherRole(int playerId, QString role) override;
    void playerGetsMoney(int playerId, int amount) override;
    void potentialExchange(int playerId1, int playerId2) override;
    void cashExchange(int playerId1, int playerId2) override;
    void informDecision(int playerId, Decision dec) override;
    void informRoleClaim(int playerId, QString role) override;

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

    QMap<QString, std::shared_ptr<Role>> roleInstances;
public:
    QStringList rolesInPlay;
    QStringList playerRoles;
    QList<int> playerMoney;
    QString difficulty;
    int numberOfPlayers;
    int bank = 0;
    int peasants = 0;
    int winCon = 13;
    int cheater = -1;

    GameMaster(QObject *parent = nullptr);


    void createRoleInstances()
    {
        roleInstances.insert("Sędzia", std::make_shared<Judge>(this));
        roleInstances.insert("Biskup", std::make_shared<Bishop>(this));
        roleInstances.insert("Król", std::make_shared<Roi>(this));
        roleInstances.insert("Błazen", std::make_shared<Fool>(this));
        roleInstances.insert("Królowa", std::make_shared<Queen>(this));
        roleInstances.insert("Złodziej", std::make_shared<Thief>(this));
        roleInstances.insert("Wiedźma", std::make_shared<Witch>(this));
        roleInstances.insert("Szpieg", std::make_shared<Spy>(this));
        roleInstances.insert("Wieśniak", std::make_shared<Peasant>(this));
        roleInstances.insert("Oszust", std::make_shared<Cheat>(this));
        roleInstances.insert("Inkwizytor", std::make_shared<Inquisitor>(this));
        roleInstances.insert("Wdowa", std::make_shared<Widow>(this));
    }

    void setParams(QString diff, int num)
    {
        difficulty = diff;
        numberOfPlayers = num;
        int row = num - 6;
        rolesInPlay = rolesMatrix[row];
        playerRoles = rolesInPlay;
        if (rolesInPlay.contains("Wieśniak"))
            playerRoles.push_back("Wieśniak");
        createRoleInstances();
        std::srand ( unsigned ( std::time(0) ) );
        std::random_shuffle(playerRoles.begin(), playerRoles.end());
    }

    void run() override
    {
        int currentPlayer = 0;
        players.push_back(humanPlayer);
        humanPlayer->assign(this, 0);
        for (int i = 1; i < numberOfPlayers; ++i)
        {
            players.push_back(std::make_shared<ComputerPlayer>());
            players[i]->assign(this, i);
        }
        for (int i = 0; i < numberOfPlayers; ++i)
        {
            informAllRole(i, playerRoles[i]);
            playerMoney.push_back(6);
        }
        for (int i = 0; i < 4; ++i)
        {
            int targetId = players[i]->chooseTargets(1)[0];
            players[i]->changeRoles(targetId);
            informAllPotentialExchange(i, targetId, i);
        }
        while (mostMoney() < winCon && noBankrupts())
        {
            winCon = 13;
            auto player = players[currentPlayer];
            Decision dec = player->chooseMove();
            informAllDecision(currentPlayer, dec);
            if (dec == Decision::CHECK)
            {
               player->checkSelf();
            }
            else if (dec == Decision::CHANGE)
            {
                int targetId = player->chooseTargets(1)[0];
                player->changeRoles(targetId);
                informAllPotentialExchange(currentPlayer, targetId, currentPlayer);
            }
            else if (dec == Decision::USE)
            {
                QString chosenRole = player->chooseRoleToPlay();
                auto candidates = questionAll(chosenRole, currentPlayer);
                candidates.push_back(currentPlayer);
                if (candidates.size() > 1)
                {
                    for (int i : candidates)
                    {
                        informAllRole(i, playerRoles[i]);
                    }
                    QVector<int> winners;
                    QVector<int> losers;
                    for (int i : candidates)
                    {
                        QString role = playerRoles[i];
                        if (role == chosenRole)
                            winners.push_back(i);
                        else
                            losers.push_back(i);
                    }
                    peasants = winners.size();
                    for (int i : winners)
                        roleInstances[chosenRole]->usePower(i);
                    for (int i : losers)
                    {
                        informAllMoney(i, -1);
                        addMoney(i, -1);
                    }
                }
                else
                    roleInstances[chosenRole]->usePower(currentPlayer);
            }
            currentPlayer = (currentPlayer + 1) % numberOfPlayers;
            printMoney();
        }
        if (winCon < 13)
            humanPlayer->notify({cheater}, "Wygrana");
        else
            humanPlayer->notify(richest(), "Wygrana");

    }

    void printMoney()
    {
        for (int i = 0; i < numberOfPlayers; ++i)
            std::cerr << "Player " << i << " money: " << playerMoney[i] << std::endl;
        std::cerr << "Bank money: " << bank << std::endl;
    }

    bool noBankrupts()
    {
        for (int i = 0; i < numberOfPlayers; ++i)
            if (playerMoney[i] < 1)
                return false;
        return true;
    }

    std::shared_ptr<HumanPlayer> getHumanPLayer()
    {
        return humanPlayer;
    }

    int mostMoney(int playerId = -1)
    {
        int max = 0;
        for (int i = 0; i < numberOfPlayers; ++i)
        {
            if (playerMoney[i] > max && i != playerId)
                max = playerMoney[i];
        }
        return max;
    }

    QVector<int> richest(int playerId = -1)
    {
        int max = mostMoney(playerId);
        QVector<int> result;
        for (int i = 0; i < numberOfPlayers; ++i)
            if (playerMoney[i] == max)
                result.push_back(i);
        return result;
    }

    void addMoney(int playerId, int amount)
    {
        playerMoney[playerId] += amount;
    }

    void transferMoney(int from, int to, int amount)
    {
        addMoney(from, -amount);
        addMoney(to, amount);
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

    QVector<int> questionAll(QString role, int notTarget = -1)
    {
        QVector<int> result;
        for (int i = 0; i < numberOfPlayers; ++i)
        {
            if (i != notTarget)
            {
                if (players[i]->questionRole(role))
                    result.push_back(i);
            }
        }
        return result;
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

    void informAllClaimRole(int playerId, QString role)
    {
        for (int i = 0; i < numberOfPlayers; ++i)
        {
            if (i != playerId)
                players[i]->informRoleClaim(playerId, role);
        }
    }
public:
    std::shared_ptr<HumanPlayer> humanPlayer = std::make_shared<HumanPlayer>();
    QVector<std::shared_ptr<Player>> players;
};

#endif // GAME_H
