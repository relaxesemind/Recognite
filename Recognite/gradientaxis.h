#ifndef GRADIENTAXIS_H
#define GRADIENTAXIS_H


#include <QWidget>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QLabel>
#include <QVBoxLayout>


class GradientAxis : public QWidget
{
    Q_OBJECT
public:
    explicit GradientAxis(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // GRADIENTAXIS_H
