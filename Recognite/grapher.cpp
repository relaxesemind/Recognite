#include "grapher.h"
#include "Common/currentappstate.h"

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

//    chart->setAnimationOptions(QChart::SeriesAnimations);
}

void Grapher::addGraph(const QVector<QPointF> &points, QString const& title, GrapherMode::Options mode, QColor barColor, QColor lineColor)
{
    chart->legend()->show();
    chart->legend()->setAlignment(Qt::AlignRight);

    if (mode & GrapherMode::BarVisible)
    {
        QBarSet *barSet = new QBarSet("",nullptr);
        QBarSeries *seria = new QBarSeries();

        for (QPointF p : points)
        {
            if (!(p.x() < CurrentAppState::shared().leftColumnEdge - 1 || p.x() > CurrentAppState::shared().rightColumnEdge))
            {
                barSet->append(p.y());
            }
        }

        barSet->setColor(barColor);
        seria->append(barSet);
        seria->setName(title);
        chart->addSeries(seria);
    }

    if (mode & GrapherMode::SplineVisible)
    {
        QSplineSeries *seriesLine = new QSplineSeries();

        for (QPointF p : points)
        {
            if (!(p.x() < CurrentAppState::shared().leftColumnEdge - 1 || p.x() > CurrentAppState::shared().rightColumnEdge))
            {
                seriesLine->append(p);
            }
        }

//        seriesLine->append(points.toList());

        QPen pen(lineColor);
        pen.setWidthF(2.5);

        seriesLine->setPen(pen);
        seriesLine->setName(title);
        chart->addSeries(seriesLine);
    }

    if (chart->series().isEmpty())
    {
        return;
    }

    chart->createDefaultAxes();

    QValueAxis *axisX = new QValueAxis();
//    QValueAxis *axisY = new QValueAxis();

//    axisY->setMax(maxY);
//    axisY->setMin(minY);

    axisX->setMin(minX);
    axisX->setMax(maxX);

    axisX->setLabelFormat("%.1f nm");
//    axisY->setLabelFormat("%.1f%%");

    axisX->setTickCount(8);
//    axisY->setTickCount(8);

    chart->setAxisX(axisX);
//    chart->setAxisY(axisY);
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




