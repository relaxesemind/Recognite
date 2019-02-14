#include "grapher.h"

Grapher::Grapher()
{
    view = new QChartView();
    chart = new QChart();

    view->setChart(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setRubberBand(QChartView::RectangleRubberBand);

    QScatterSeries *pointSeria = new QScatterSeries();
    pointSeria->append(QPoint(1,1));
    QColor color;
    color.setNamedColor("red");
    pointSeria->setColor(color);
    pointSeria->setName("unknown");
    pointSeria->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    pointSeria->setMarkerSize(20.0);
    chart->addSeries(pointSeria);
    chart->createDefaultAxes();
}

//Grapher::~Grapher()
//{
//   delete view;
//   delete chart;
//}

void Grapher::addPointsAtGraph(const QVector<QPointF> &points)
{
  /*  QScatterSeries *series = new QScatterSeries();
    for (QPointF &p : points) {
        series->append(p);
    }
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series->setMarkerSize(10.0);

            QColor color;
            switch (type) {
            case CellType::Blast:
                 color.setNamedColor("orange");
                 series->setName("Бласт");
                break;
            case CellType::NoBlast :
                color.setNamedColor("green");
                series->setName("Не Бласт");
                break;
            }

            series->setColor(color);
            chart->addSeries(series);
            chart->createDefaultAxes()
            ///
    QScatterSeries *pointSeria = new QScatterSeries();
    pointSeria->append(point);
    QColor color;
    color.setNamedColor("red");
    pointSeria->setColor(color);
    pointSeria->setName("unknown");
    pointSeria->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    pointSeria->setMarkerSize(20.0);
    chart->addSeries(pointSeria);
    chart->createDefaultAxes();*/;

}

void Grapher::clearView()
{

}
