#ifndef GRAPHER_H
#define GRAPHER_H

#include "Common/magic.h"

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>


using namespace QtCharts;


class Grapher : public QObject
{
    Q_OBJECT

private:
    Grapher();
    Grapher(const Grapher&);
    Grapher operator=(Grapher&);
public:
//    ~Grapher();
    static Grapher& shared() {
        static Grapher service;
        return service;
    }

public:

    void addPointsAtGraph(const QVector<QPointF>& points);
    void clearView();

signals:

public slots:

public:
    QChartView *view;
    QChart *chart;
};

#endif // GRAPHER_H
