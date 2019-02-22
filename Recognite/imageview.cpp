#include "imageview.h"

ImageView::ImageView(QWidget* widget) : QGraphicsView(widget)
{
    this->setScene(&scene);
    this->setAlignment(Qt::AlignCenter);
    currentScale = 1.0;
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if (scene.items().isEmpty())
    {
        return;
    }

    QPoint degrees = event->angleDelta() / 8;
    qreal zoom = degrees.y() > 0 ? Consts::zoomMultiple : 1 / Consts::zoomMultiple;
    currentScale *= zoom;

    if (currentScale > 1)
    {
        layout->setContentsMargins(0,0,16,0);
    }
    else
    {
       layout->setContentsMargins(0,0,0,0);
    }

    scale(zoom,zoom);
}

void ImageView::setImage(const QPixmap &pixmap)
{
    currentImageItem = std::make_unique<PXitem>(pixmap);
    scene.addItem(currentImageItem.get());
    ImageView::centerOn(currentImageItem.get());
    ImageView::fitInView(currentImageItem.get(),Qt::KeepAspectRatio);
    update();
}

void ImageView::addGradientAxis(float min, float max)
{
    maxValue = max;
    minValue = min;
    isAxisVisible = true;
    layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignRight);
    layout->setDirection(QBoxLayout::RightToLeft);
    layout->setContentsMargins(0,0,0,0);

    QWidget *gradient = new QWidget(this);
    gradient->setMaximumSize(25,1000);
    gradient->setStyleSheet(QString("background-color : qlineargradient(spread:pad, x1:0, y1:1,"
                                    " x2:0, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));

    layout->addWidget(gradient,1);

//    GradientAxis *axis = new GradientAxis(this);
//    layout->addWidget(axis,1);

    this->setLayout(layout);
}
