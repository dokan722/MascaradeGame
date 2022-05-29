#include "game.h"

GameMaster::GameMaster(QObject *parent):
    QThread(parent)
{

}

void HumanPlayer::checkSelf()
{
    QString role = gameMaster->playerRoles[myId];
    QString msg = "Twoja rola to: " + role;
    notify({myId}, msg);
}

void HumanPlayer::changeRoles(int playerId)
{
    QStringList answers = {"Tak", "Nie"};
    QString answer = saySomething("Czy chcesz zamienić rolę?", answers);
    if (answer == answers[0])
        std::swap(gameMaster->playerRoles[myId], gameMaster->playerRoles[playerId]);
}

void HumanPlayer::changeRoles(int playerId1, int playerId2)
{
    QStringList answers = {"Tak", "Nie"};
    QString answer = saySomething("Czy chcesz zamienić rolę?", answers);
    if (answer == answers[0])
        std::swap(gameMaster->playerRoles[playerId1], gameMaster->playerRoles[playerId2]);
}

QString HumanPlayer::chooseRoleToPlay()
{
    QString answer = saySomething("Wybierz rolę ", gameMaster->rolesInPlay);
    return answer;
}

bool HumanPlayer::questionRole(QString role, int playerId)
{
    QStringList answers = {"Tak", "Nie"};
    QString answer = saySomething(
                "Czy chcesz zakwestionować i ogłosić, że " + role +
                " to twoja rola, a nie gracza " + QString::number(playerId) + "?",
                answers);
    if (answer == answers[0])
        return true;
    return false;
}

QVector<int> HumanPlayer::chooseTargets(int numOfTargets, QVector<int> *possibleTargets)
{
    QVector<int> targets;
    QStringList posTargets;
    if (possibleTargets == nullptr)
    {
        for (int i = 0; i < gameMaster->numberOfPlayers; ++i)
        {
            if (i != myId)
                posTargets.append(QString::number(i));
        }
    }
    else
    {
        for (int i : *possibleTargets)
        {
            if (i != myId)
                posTargets.append(QString::number(i));
        }
    }
    for (int i = 0; i < numOfTargets; ++i)
    {
        int target = saySomething("Wybierz cel ", posTargets).toInt();
        posTargets.removeOne(QString::number(target));
        targets.push_back(target);
    }
    return targets;
}

void HumanPlayer::getOtherRole(int playerId, QString role)
{
    notify({playerId}, " rola to " + role);
}

void HumanPlayer::playerGetsMoney(int playerId, int amount)
{
    notify({playerId}, "otrzymuje " + QString::number(amount) + " monet");
}

void HumanPlayer::potentialExchange(int playerId1, int playerId2)
{
    notify({playerId1, playerId2}, "dokonano potencjalnej zamiany");
}

void HumanPlayer::cashExchange(int playerId1, int playerId2)
{
    notify({playerId1, playerId2}, "zamienili się monetami");
}

void HumanPlayer::informDecision(int playerId, Decision dec)
{
    QString decision = "";
    if (dec == Decision::CHECK)
        decision = "sprawdzić kartę";
    else if (dec == Decision::CHANGE)
        decision = "dokonać potencjalnej zamiany";
    else if (dec == Decision::USE)
        decision = "zadeklarować rolę";
    notify({playerId}, "zdecydował się " + decision);
}

void HumanPlayer::informRoleClaim(int playerId, QString role)
{
    notify({playerId}, "twierdzi, że jego rola to " + role);
}

Decision ComputerPlayer::chooseMove()
{
    double maxProb = 0;
    if (gameMaster->playerMoney[myId] >= 10)
        preferedRoles[type].push_back("Oszust");
    else
        preferedRoles[type].removeOne("Oszust");
    for (QString &role : preferedRoles[type])
    {
        if (role == "Sędzia")
        {
            if (gameMaster->bank < 1)
                continue;
            if (type == Type::TRYHARD && gameMaster->bank < 2)
                continue;
        }
        if (role == "Wdowa" && gameMaster->playerMoney[myId] > 7)
            continue;
        if (probability[myId][roleToId(role)] > maxProb)
        {
            maxProb = probability[myId][roleToId(role)];
        }
    }
    if (maxProb > 0.3)
        return Decision::USE;
    else if (type == Type::TRYHARD || type == Type::TROLL || type ==Type::STOPPER)
        return Decision::CHANGE;
    else
        return Decision::CHECK;
}

void ComputerPlayer::checkSelf()
{
    QString myRole = gameMaster->playerRoles[myId];
    for (int i = 0; i < 12; ++i)
        probability[myId][i] = 0;
    for (int i = 0; i < probability.size(); ++i)
        probability[i][roleToId(myRole)] = 0;
    probability[myId][roleToId(myRole)] = 1;
    lastUpdate[myId] = time;
    checkMemory();
}

void ComputerPlayer::changeRoles(int playerId)
{
    double myProb = 0;
    double hisProb = 0;
    for (QString &role : preferedRoles[type])
    {
        int id = roleToId(role);
        myProb += probability[myId][id];
        hisProb += probability[playerId][id];
    }
    if (hisProb > myProb)
    {
        for (int i = 0; i < 12; ++i)
            std::swap(probability[myId][i], probability[playerId][i]);
        std::swap(gameMaster->playerRoles[myId], gameMaster->playerRoles[playerId]);
    }
}

void ComputerPlayer::changeRoles(int playerId1, int playerId2)
{
    double hisProb1 = 0;
    double hisProb2 = 0;
    int hisMoney1 = gameMaster->playerMoney[playerId1];
    int hisMoney2 = gameMaster->playerMoney[playerId2];
    for (QString &role : preferedRoles[Type::TRYHARD])
    {
        int id = roleToId(role);
        hisProb1 += probability[playerId1][id];
        hisProb2 += probability[playerId2][id];
    }
    if ((hisProb1 > hisProb2 && hisMoney1 > hisMoney2) || (hisProb1 < hisProb2 && hisMoney1 < hisMoney2))
    {
        for (int i = 0; i < 12; ++i)
            std::swap(probability[playerId1][i], probability[playerId2][i]);
        std::swap(gameMaster->playerRoles[playerId1], gameMaster->playerRoles[playerId2]);
    }
}

QString ComputerPlayer::chooseRoleToPlay()
{
    double maxProb = 0;
    QString bestRole = preferedRoles[type][0];
    for (QString &role : preferedRoles[type])
    {
        if (role == "Sędzia")
        {
            if (gameMaster->bank < 1)
                continue;
            if (type == Type::TRYHARD && gameMaster->bank < 2)
                continue;
        }
        if (role == "Wdowa" && gameMaster->playerMoney[myId] > 7)
            continue;
        if (probability[myId][roleToId(role)] > maxProb)
        {
            bestRole = role;
            maxProb = probability[myId][roleToId(role)];
        }
    }
    return bestRole;
}

QString ComputerPlayer::answerWithRole()
{
    double maxProb = 0;
    QString role = idToRole(0);
    for (int i = 0; i < 12; ++i)
    {
        if (probability[myId][i] > maxProb)
        {
            maxProb = probability[myId][i];
            role = idToRole(i);
        }
    }
    return role;
}

bool ComputerPlayer::questionRole(QString role, int playerId)
{
    double myProb = probability[myId][roleToId(role)];
    double hisProb = probability[playerId][roleToId(role)];
    double myMoney = gameMaster->playerMoney[myId];
    double hisMoney = gameMaster->playerMoney[playerId];
    double coef = hisMoney / myMoney;
    if (myProb * coef > hisProb)
        return true;
    return false;
}

QVector<int> ComputerPlayer::chooseTargets(int numOfTargets, QVector<int> *possibleTargets)
{
    QVector<int> targets;
    int value = QRandomGenerator::global()->bounded(gameMaster->numberOfPlayers);
    auto richest = gameMaster->richest();
    targets.push_back(gameMaster->richest()[value % richest.size()]);
    if (numOfTargets > 1)
    {
        for (int i = 1; i < numOfTargets; ++i)
        {
            int num = gameMaster->numberOfPlayers;
            for (int i = num - 1; i >= 0;  --i)
                if (!targets.contains(i) && i != myId && gameMaster->playerMoney[i] < gameMaster->playerMoney[targets[0]])
                    targets.push_back(i);
        }
    }
    if (targets.size() != numOfTargets)
        throw;
    return targets;
}

void ComputerPlayer::getOtherRole(int playerId, QString role)
{
    for (int i = 0; i < 12; ++i)
        probability[playerId][i] = 0;
    for (int i = 0; i < probability.size(); ++i)
        probability[i][roleToId(role)] = 0;
    probability[playerId][roleToId(role)] = 1;
    lastUpdate[playerId] = time;
    checkMemory();
}

void ComputerPlayer::potentialExchange(int playerId1, int playerId2)
{
    for (int i = 0; i < 12; ++i)
    {
        double sumProb = probability[playerId1][i] + probability[playerId2][i];
        probability[playerId1][i] = sumProb / 2;
        probability[playerId2][i] = sumProb / 2;
    }
    lastUpdate[playerId1] = time;
    lastUpdate[playerId2] = time;
    checkMemory();
}

void ComputerPlayer::checkMemory()
{
    time++;
    QVector<int> forgotten;
    for (int i = 0; i < lastUpdate.size(); ++i)
        if (lastUpdate[i] < time - memoryTime)
            forgotten.push_back(i);
    if (forgotten.size() < 2)
        return;
    for (int i = 0; i < 12; ++i)
    {
        double sumProb = 0;
        for (int j : forgotten)
            sumProb += probability[j][i];
        for (int j : forgotten)
            probability[j][i] = sumProb / forgotten.size();
    }
}

void ComputerPlayer::assign(GameMaster *gm, int id)
{
    gameMaster = gm;
    myId = id;
    setBot(gm->difficulty, gm->numberOfPlayers);
}






































