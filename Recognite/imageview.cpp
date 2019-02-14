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
    if (degrees.y() > 0)
    {
        scale(1.05,1.05);
    }
    else
    {
        scale(1/1.05,1/1.05);
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
