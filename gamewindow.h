#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H


#include <QWindow>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QStylePainter>
#include <deque>
#include "game.h"

class StartingWindow;


class VerticalTabBar: public QTabBar
{
    // adapted from https://stackoverflow.com/a/50579369
public:
    QSize tabSizeHint(int index) const
    {
        QSize s = QTabBar::tabSizeHint(index);
        s.transpose();
        return s;
    }
protected:
    void paintEvent(QPaintEvent *){
        QStylePainter painter(this);
        QStyleOptionTab opt;

        for(int i = 0; i < count(); i++) {
            initStyleOption(&opt,i);
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);
            painter.save();

            QSize s = opt.rect.size();
            s.transpose();
            QRect r(QPoint(), s);
            r.moveCenter(opt.rect.center());
            opt.rect = r;

            QPoint c = tabRect(i).center();
            painter.translate(c);
            painter.rotate(90);
            painter.translate(-c);
            painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
            painter.restore();
        }
    }
};

class VerticalTabWidget: public QTabWidget
{
public:
    VerticalTabWidget(QWidget *parent=0):
        QTabWidget(parent)
    {
        setTabBar(new VerticalTabBar);
        setTabPosition(QTabWidget::West);
    }
};


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

    GameMaster gameMaster;
    std::shared_ptr<HumanPlayer> player;

    QLabel *logView;
    std::deque<QString> logEntries;
    void log(QString message);

    QLabel *choiceQuestionLabel;
    QComboBox *choiceAnswerBox;
    QPushButton *choiceAcceptButton;
    void showChoice(const QString &question, const QStringList &answers);

    QTableWidget *scoreTable;

    VerticalTabWidget *rolesDescription;
protected:
    void closeEvent(QCloseEvent *event) override;

public:
    void setParameters(QString diff, int num);
    void setStartingWindow(StartingWindow &w);
    void startGame();

public slots:
    void askForMove(QString question, QStringList answers);
    void notify(QVector<int> players, QString message);
    void updateMoney(int, QList<int> money);
    void acceptChoice();
};

#endif // GAMEWINDOW_H
