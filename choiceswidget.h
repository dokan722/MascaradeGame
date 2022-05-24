#ifndef CHOICESWIDGET_H
#define CHOICESWIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGui>



class ChoicesWidget : public QWidget
{
    Q_OBJECT
public:
    ChoicesWidget(QString introText, QVector<QString> choices, QWidget *parent = nullptr);

private:
    QVBoxLayout *layout;
    QLabel *introLabel;
    QVector<QPushButton *> choicesLabels;

public slots:
    void emitChoice(uint32_t choice);

signals:
    void selected(int choiceIndex);
};

#endif // CHOICESWIDGET_H
