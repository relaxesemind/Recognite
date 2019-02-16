#include "imageview.h"

ImageView::ImageView(QWidget* widget) : QGraphicsView(widget)
{
    this->setScene(&scene);
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if (scene.items().isEmpty())
    {
        return;
    }

    QPoint degrees = event->angleDelta() / 8;
    qreal zoom = Consts::zoomMultiple;

    if (degrees.y() > 0)
    {
        scale(zoom,zoom);
    }
    else
    {
        scale(1 / zoom,1 / zoom);
    }
}

void ImageView::setImage(const QPixmap &pixmap)
{
    currentImageItem = std::make_unique<PXitem>(pixmap);
    scene.addItem(currentImageItem.get());
    ImageView::centerOn(currentImageItem.get());
    ImageView::fitInView(currentImageItem.get(),Qt::KeepAspectRatio);
    update();
}
