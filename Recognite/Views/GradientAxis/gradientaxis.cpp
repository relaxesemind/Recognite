#include "gradientaxis.h"

GradientAxis::GradientAxis(QWidget *parent) : QWidget(parent)
{
    setHidden(false);

//    QVBoxLayout *layout = new QVBoxLayout(this);
//    layout->setMargin(0);
//    QLabel *label1 = new QLabel("label",this);
//    QLabel *label2 = new QLabel("label",this);
//    QLabel *label3 = new QLabel("label");
//    label1->setAlignment(Qt::AlignTop);
//    label2->setAlignment(Qt::AlignBottom);
//    layout->setAlignment(label1,Qt::AlignTop);

//    layout->addWidget(label1);
//    layout->addWidget(label2);
//    layout->addWidget(label3);

//    setLayout(layout);
    setMaximumSize(25,1000);
    this->setStyleSheet(QString("GradientAxis {background-color : qlineargradient(spread:pad, x1:0, y1:1,"
                          " x2:0, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255)); }"));
}

void GradientAxis::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

