#include "grapher.h"

Grapher::Grapher()
{
    view = new QChartView();
    chart = new QChart();

    view->setChart(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setRubberBand(QChartView::RectangleRubberBand);
}

//Grapher::~Grapher()
//{
//   delete view;
//   delete chart;
//}

void Grapher::addPointsAtGraph(const QVector<QPointF>& points)
{
    QSplineSeries *series = new QSplineSeries();

    chart->legend()->hide();
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Диаграмма высот в процентах от максимальной высоты");

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    axisY->setMax(100);
    axisY->setMin(0);

    axisX->setMin(0);
    axisX->setMax(points.size() - 1);

    axisY->setMinorTickCount(5);

    axisX->setLabelFormat("%i");
    axisY->setLabelFormat("%i%%");

    chart->setAxisX(axisX);
    chart->setAxisY(axisY);

    for (QPointF p : points)
    {
        series->append(p);
    }

//    series->setMarkerShape(QSplineSeries::SeriesTypeLine);
//    series->setMarkerSize(10.0);
    QColor color;
    color.setNamedColor("red");
    QPen pen(color);
    pen.setWidthF(2.5);
    series->setPen(pen);

    chart->addSeries(series);


}

void Grapher::clearView()
{
    chart->removeAllSeries();

}
