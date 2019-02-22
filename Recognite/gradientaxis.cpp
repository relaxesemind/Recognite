#include "gradientaxis.h"

GradientAxis::GradientAxis(QWidget *parent) : QWidget(parent)
{
    setMaximumSize(25,1000);
    setStyleSheet(QString("background-color : qlineargradient(spread:pad, x1:0, y1:1,"
                                                              " x2:0, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));
}
