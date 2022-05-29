#include "gamewindow.h"
#include "startingwindow.h"
#include <iostream>
#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QApplication>
#include <QHeaderView>
#include <QTextEdit>
#include <QPixmap>


namespace
{
static const QVector<QStringList> rolesMatrix = {
    {"Sędzia", "Biskup", "Król", "Królowa", "Wiedźma", "Oszust"},
    {"Sędzia", "Biskup", "Król", "Królowa", "Złodziej", "Wiedźma", "Szpieg",},
    {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Wieśniak"},
    {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Wieśniak", "Oszust"},
    {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Szpieg", "Wieśniak", "Oszust"},
    {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Szpieg", "Wieśniak", "Oszust", "Inkwizytor"},
    {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Wiedźma", "Szpieg", "Wieśniak", "Oszust", "Inkwizytor", "Wdowa"},
    {"Sędzia", "Biskup", "Król", "Błazen", "Królowa", "Złodziej", "Wiedźma", "Szpieg", "Wieśniak", "Oszust", "Inkwizytor", "Wdowa"}
};

QString getRoleHtml(const QString &roleName)
{
    QString description;
    QString image;
    if (roleName == "Szpieg") {
        description = "<p>Wykonując akcję potencjalnej zamiany Szpieg w ukryciu podgląda"
                " swoją kartę i kartę przeciwnika (lub ze stołu) zanim dokona"
                " ewentualnej zamiany.";
        image = "img/spy.png";
    }
    else if (roleName == "Biskup") {
        description = "<p>Biskup zabiera dla siebie 2 monety od najbogatszego z pozostałych"
               " graczy. W przypadku remisu, Biskup decyduje, od kogo weźmie"
                " monety.";
        image = "img/bishop.png";
    }
    else if (roleName == "Błazen") {
        description = "Błazen otrzymuje 1 monetę z banku i dokonuje potencjalnej zamiany"
                " dwóch kart należących do dwóch innych graczy, pod stołem, bez"
                " patrzenia na ich treść.";
        image = "img/fool.png";
    }
    else if (roleName == "Inkwizytor") {
        description = "Inkwizytor wskazuje innego gracza. Gracz ten musi stwierdzić, jaką"
                " jest postacią, a następnie odkryć swoją kartę. Jeśli się pomylił, musi"
                "   zapłacić Inkwizytorowi 4 monety. Jeśli ma rację, akcja nie przynosi"
                " żadnego efektu.";
        image = "img/inquisitor.png";
    }
    else if (roleName == "Sędzia") {
        description = "Sędzia zabiera wszystkie monety (grzywny) znajdujące się w danej"
                " chwili na planszy Sądu.";
        image = "img/judge.png";
    }
    else if (roleName == "Wieśniak") {
        description = "Wieśniak otrzymuje 1 monetę z banku. Jeśli jednak w danej turze"
                " ujawnieni zostaną obaj Wieśniacy, obaj otrzymują po 2 monety z"
                " banku. Ostatecznie, w jedności siła!";
        image = "img/peasant.png";
    }
    else if (roleName == "Królowa") {
        description = "Królowa otrzymuje z banku 2 monety.";
        image = "img/queen.png";
    }
    else if (roleName == "Król") {
        description = "Król otrzymuje z banku 3 monety.";
        image = "img/roi.png";
    }
    else if (roleName == "Wiedźma") {
        description = "Wiedźma może wymienić całą swoją pieniężną fortunę na fortunę"
                " wybranego gracza.";
        image = "img/witch.png";
    }
    else if (roleName == "Oszust") {
        description = "Jeśli Oszust posiada co najmniej 10 monet, wygrywa grę.";
        image = "img/cheat.png";
    }
    else if (roleName == "Wdowa") {
        description = "Wdowa otrzymuje tyle monet z banku, aby w sumie posiadać ich 10.";
        image = "img/widow.png";
    }
    else if (roleName == "Złodziej") {
        description = "Złodziej zabiera 1 monetę graczowi siedzącemu po swojej lewej stronie"
                "i 1 monetę graczowi siedzącemu po swojej prawej stronie.";
        image = "img/thief.png";
    }
    else {
        description = "<p>Ta rola nie posiada jeszcze opisu.</p>";
        image = "img/unknown.png";
    }
    return "<h1>" + roleName + "</h1>" + description + "<div style=\"text-align: center\"><img width=\"120\" src=\""
            + "./" + image + "\"></div>";
}
}


GameWindow::GameWindow(QWidget *parent)
    : QWidget{parent}
{
    setMinimumSize(QSize(1200, 800));

    auto layout = new QGridLayout(this);
    layout->setColumnStretch(1, 1);
    layout->setRowStretch(0, 1);
    setLayout(layout);

    // SETUP GAME LOG
    logView = new QLabel(this);
    logView->setWordWrap(true);
    layout->addWidget(logView, 0, 1, Qt::AlignLeft);

    // SETUP QUESTION AREA
    choiceQuestionLabel = new QLabel(this);
    choiceQuestionLabel->setStyleSheet("QLabel { font-size: 120%; font-weight: bold; }");
    layout->addWidget(choiceQuestionLabel, 1, 0, 1, 3, Qt::AlignCenter);

    choiceAnswerBox = new QComboBox(this);
    choiceAnswerBox->setEnabled(false);
    choiceAnswerBox->setMinimumWidth(400);
    layout->addWidget(choiceAnswerBox, 2, 0, 1, 3, Qt::AlignCenter);

    choiceAcceptButton = new QPushButton(this);
    choiceAcceptButton->setText("Zatwierdź");
    choiceAcceptButton->setEnabled(false);
    layout->addWidget(choiceAcceptButton, 3, 2, Qt::AlignRight);
    connect(choiceAcceptButton, SIGNAL(clicked()), this, SLOT(acceptChoice()));

    // SETUP SCOREBOARD
    scoreTable = new QTableWidget(this);
    scoreTable->setColumnCount(2);
    scoreTable->verticalHeader()->setVisible(false);
    scoreTable->setHorizontalHeaderLabels(QStringList{"Kto", "Liczba monet"});
    scoreTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(scoreTable, 0, 2, Qt::AlignRight);

    // SETUP ROLES DESCTIPRION
    rolesDescription = new VerticalTabWidget(this);
    layout->addWidget(rolesDescription, 0, 0, Qt::AlignLeft);
}

void GameWindow::setStartingWindow(StartingWindow &w)
{
    starting = &w;
}

void GameWindow::setParameters(QString diff, int num)
{
    difficulty = diff;
    numberOfPlayers = num;

    // initialize score table
    scoreTable->setRowCount(num + 1);
    scoreTable->setItem(0, 0, new QTableWidgetItem("Sąd"));
    scoreTable->setItem(0, 1, new QTableWidgetItem("0 monet"));
    scoreTable->setItem(1, 0, new QTableWidgetItem("Ty (Gracz 0)"));
    scoreTable->setItem(1, 1, new QTableWidgetItem("6 monet"));
    for (int i = 1; i < num; ++i) {
        scoreTable->setItem(i + 1, 0, new QTableWidgetItem("Gracz " + QString::number(i)));
        scoreTable->setItem(i + 1, 1, new QTableWidgetItem("6 monet"));
    }

    // insert manual entries
    for (const auto &role: rolesMatrix[num - 6]) {
        auto desc = new QTextEdit(rolesDescription);
        desc->setHtml(getRoleHtml(role));
        rolesDescription->addTab(desc, role);
        desc->setReadOnly(true);
    }
}

void GameWindow::startGame()
{
    std::cerr << "Starting game..." << std::endl;
    player = gameMaster.getHumanPLayer();
    connect(player.get(), SIGNAL(waitForMove(QString, QStringList)), this, SLOT(askForMove(QString, QStringList)));
    connect(player.get(), SIGNAL(notified(QVector<int>, QString)), this, SLOT(notify(QVector<int>, QString)));
    connect(&gameMaster,  SIGNAL(playerTurnEnd(int, QList<int>)), this, SLOT(updateMoney(int, QList<int>)));
    gameMaster.setParams(difficulty, numberOfPlayers);
    gameMaster.start();
}

void GameWindow::askForMove(QString question, QStringList answers)
{
    showChoice(question, answers);
}


void GameWindow::notify(QVector<int> players, QString message)
{
    QString text = "<b>";
    if (players.size() == 1)
        text += "Gracz #" + QString::number(players[0]);
    else {
        text += "Gracze #" + QString::number(players[0]);
        for (int i = 1; i < players.size(); ++i) {
            text += ", #" + QString::number(players[i]);
        }
    }
    text += ":</b> ";
    text += message;

    log(text);
    player->wake();
}

void GameWindow::log(QString message)
{
    logEntries.push_back(std::move(message));
    while (logEntries.size() > 24)
        logEntries.pop_front();
    QString logText = logEntries[0];
    for (size_t i = 1; i < logEntries.size(); ++i)
        logText.append("<br>" + logEntries[i]);
    logView->setText(logText);
}

void GameWindow::showChoice(const QString &question, const QStringList &answers)
{
    choiceQuestionLabel->setText(question);
    choiceAnswerBox->addItems(answers);
    choiceAnswerBox->setEnabled(true);
    choiceAcceptButton->setEnabled(true);
}

void GameWindow::acceptChoice()
{
    auto choice = choiceAnswerBox->currentText();
    if (choice.isEmpty())
        return;
    player->setTheMove(choice);
    log("<span style=\"color: gray\"><b>" + choiceQuestionLabel->text() + ":</b> " + choice + "</span>");

    choiceQuestionLabel->setText("");
    while (choiceAnswerBox->count() > 0)
        choiceAnswerBox->removeItem(0);
    choiceAnswerBox->setEnabled(false);
    choiceAcceptButton->setEnabled(false);
}

void GameWindow::updateMoney(int bank, QList<int> money)
{
    scoreTable->setItem(0, 1, new QTableWidgetItem(QString::number(bank) + " monet"));
    for (int i = 0; i < numberOfPlayers; ++i) {
        scoreTable->setItem(i + 1, 1, new QTableWidgetItem(QString::number(money[i]) + " monet"));
    }

}

void GameWindow::closeEvent(QCloseEvent *event)
{
    gameMaster.terminate();
    qApp->quit();
}
