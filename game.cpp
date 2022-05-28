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

bool HumanPlayer::questionRole(QString role)
{
    QStringList answers = {"Tak", "Nie"};
    QString answer = saySomething(
                "Czy chcesz zakwestionować i ogłosić, że twoja rola to " + role + " ?",
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
        posTargets.removeAt(target);
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





































