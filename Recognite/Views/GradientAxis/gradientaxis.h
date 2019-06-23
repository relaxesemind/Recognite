#ifndef GRADIENTAXIS_H
#define GRADIENTAXIS_H


#include <QWidget>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QLabel>
#include <QVBoxLayout>
#include <QSpacerItem>


class GradientAxis : public QWidget
{
    Q_OBJECT
public:
    explicit GradientAxis(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void setMinLabelValue(float value);
    void setMaxLabelValue(float value);

private:
    QLabel *labelMin, *labelMax;
};

#endif // GRADIENTAXIS_H
