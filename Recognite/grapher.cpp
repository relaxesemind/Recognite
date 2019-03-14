#include "grapher.h"

Grapher::Grapher()
{
    view = new ChartView();
    chart = new QChart();

    view->setChart(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setRubberBand(QChartView::RectangleRubberBand);

    updateState();
}

void Grapher::updateState()
{
    chart->legend()->hide();
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Распределение по высотам");

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    axisY->setMax(maxY * 100);
    axisY->setMin(minY * 100);

    axisX->setMin(minX);
    axisX->setMax(maxX);

    axisX->setLabelFormat("%.1f nm");
    axisY->setLabelFormat("%.1f%%");

    axisX->setTickCount(8);
    axisY->setTickCount(8);

    chart->setAxisX(axisX);
    chart->setAxisY(axisY);
}

void Grapher::addGraph(const QVector<QPointF> &points, GrapherMode::Options mode, QColor barColor, QColor lineColor)
{
    if (mode & GrapherMode::BarVisible)
    {
        QBarSeries *series = new QBarSeries();
        QBarSet *barSet = new QBarSet("",nullptr);

        for (QPointF p : points)
        {
            barSet->append(p.y());
        }

        barSet->setColor(barColor);
        series->insert(0,barSet);
        chart->addSeries(series);
    }

    if (mode & GrapherMode::SplineVisible)
    {
        QSplineSeries *seriesLine = new QSplineSeries();
        for (QPointF p : points)
        {
            seriesLine->append(p);
        }

        QPen pen(lineColor);
        pen.setWidthF(2.5);

        seriesLine->setPen(pen);
        chart->addSeries(seriesLine);
    }
}

void Grapher::clearView()
{
    chart->removeAllSeries();
}

void Grapher::setXRange(float min, float max)
{
    minX = min;
    maxX = max;
}

void Grapher::setYRange(float min, float max)
{
    minY = min;
    maxY = max;
}

bool Grapher::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == rubberBand && event->type() == QEvent::Resize)
    {
            QPointF fp = chart->mapToValue(rubberBand->geometry().topLeft());
            QPointF tp = chart->mapToValue(rubberBand->geometry().bottomRight());
            emit rubberBandChanged(fp, tp);
    }

    return QObject::eventFilter(watched, event);
}

void Grapher::rubberZoomAdapt(QPointF fp, QPointF tp)
{
  // qDebug() << "zoom" << fp << " " << tp;
}




