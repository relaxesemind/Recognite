#include "gradientaxis.h"

GradientAxis::GradientAxis(QWidget *parent) : QWidget(parent)
{
    setHidden(false);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    QLabel *label1 = new QLabel("max",this);
    labelMax = label1;
    label1->setMinimumWidth(25);
    label1->setAlignment(Qt::AlignCenter);
    label1->setStyleSheet("QLabel { background-color : white; color : black; }");


    QLabel *label2 = new QLabel("min",this);
    labelMin = label2;
    label2->setMinimumWidth(25);
    label2->setAlignment(Qt::AlignCenter);
    label2->setStyleSheet("QLabel { background-color : black; color : white; }");

    QSpacerItem *spacer = new QSpacerItem(5,1000,QSizePolicy::Expanding,QSizePolicy::Minimum);

    layout->addWidget(label1);
    layout->addItem(spacer);
    layout->addWidget(label2);
    setLayout(layout);
    setMaximumSize(25,1000);

    this->setStyleSheet(QString("GradientAxis{background-color : qlineargradient(spread:pad, x1:0, y1:1,"
                          " x2:0, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255)); }"));
}

void GradientAxis::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void GradientAxis::setMinLabelValue(float value)
{
    QString text = QString::number(value,'f',1);
    labelMin->setText(text);
}

void GradientAxis::setMaxLabelValue(float value)
{
    QString text = QString::number(value,'f',1);
    labelMax->setText(text);
}

