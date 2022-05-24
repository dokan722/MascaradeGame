#include "choiceswidget.h"



ChoicesWidget::ChoicesWidget(QString introText, QVector<QString> choices, QWidget *parent):
    QWidget{parent},
    layout(new QVBoxLayout(this)),
    introLabel(new QLabel(this))
{
    setLayout(layout);
    layout->addWidget(introLabel);
    introLabel->setText(introText);
    for (int i = 0; i < choices.size(); ++i)
    {
        auto button = new QPushButton(this);
        button->setText(choices[i]);
        layout->addWidget(button);
        connect(button, SIGNAL(clicked()), this, SLOT(emitChoice(i)));
        choicesLabels.push_back(button);
    }
}


void ChoicesWidget::emitChoice(uint32_t choice)
{
    emit selected(choice);
}
