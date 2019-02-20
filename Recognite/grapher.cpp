#include "grapher.h"

Grapher::Grapher()
{
    view = new ChartView();
    chart = new QChart();

    view->setChart(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setRubberBand(QChartView::RectangleRubberBand);

//    rubberBand = view->findChild<QRubberBand *>();
//    if (rubberBand)
//    {
//        rubberBand->installEventFilter(this);
//        connect(this,&Grapher::rubberBandChanged,this,&Grapher::rubberZoomAdapt);
//    }
}

void Grapher::addPointsAtGraph(const QVector<QPointF>& points, GrapherMode::Options mode)
{
    chart->legend()->hide();
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Диаграмма высот в процентах от максимальной высоты");

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    axisY->setMax(maxY);
    axisY->setMin(minY);

    axisX->setMin(minX);
    axisX->setMax(maxX);

    axisX->setLabelFormat("%.1f nm");
    axisY->setLabelFormat("%.1f");

    axisX->setTickCount(8);
    axisY->setTickCount(8);

    chart->setAxisX(axisX);
    chart->setAxisY(axisY);

    if (mode & GrapherMode::BarVisible)
    {
        QBarSeries *series = new QBarSeries();
        QBarSet *barSet = new QBarSet("",nullptr);

        for (QPointF p : points)
        {
            barSet->append(p.y());
        }

        barSet->setColor(QColor("orange"));
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

        QColor color;
        color.setNamedColor("red");
        QPen pen(color);
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




