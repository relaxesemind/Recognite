#include "imageview.h"

ImageView::ImageView(QWidget* widget) : QGraphicsView(widget)
{
    this->setScene(&scene);
    this->setAlignment(Qt::AlignCenter);
    currentScale = 1.0;
    opacity = 1.0;
    layout = nullptr;
    this->setMouseTracking(true);
    setupSlider();
    showSlider(false);
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

    if (layout)
    {
        if (currentScale >= 1)
        {
            layout->setContentsMargins(0,0,16,0);
        }
        else
        {
            layout->setContentsMargins(0,0,0,0);
        }
    }

    scale(zoom,zoom);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF l = mapToScene(event->pos().x(),event->pos().y());
    qint32 x = static_cast<qint32> (l.x());
    qint32 y = static_cast<qint32> (l.y());
    emit showHeightToolTip(QPoint(x,y), event->globalPos());
}

void ImageView::setImage(const QPixmap &pixmap)
{
    currentImageItem = std::make_unique<PXitem>(pixmap);
    scene.addItem(currentImageItem.get());
    ImageView::centerOn(currentImageItem.get());
    ImageView::fitInView(currentImageItem.get(),Qt::KeepAspectRatio);
    update();
}

void ImageView::setBinaryImage(const QPixmap &pixmap)
{
    currentBinaryItem = std::make_unique<PXitem>(pixmap);
    scene.addItem(currentBinaryItem.get());
    currentBinaryItem->setOpacity(opacity);
    ImageView::centerOn(currentBinaryItem.get());
    ImageView::fitInView(currentBinaryItem.get(),Qt::KeepAspectRatio);
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

    GradientAxis *axisWidget = new GradientAxis(this);
    axis = axisWidget;
    layout->addWidget(axisWidget,1);
    this->setLayout(layout);
}

void ImageView::showSlider(bool flag)
{
    opacitySlider->setHidden(!flag);
}

void ImageView::clearView()
{
    setImage(QPixmap());
    setBinaryImage(QPixmap());
}

void ImageView::setupSlider()
{
    QSlider *slider = new QSlider(Qt::Horizontal,this);
    slider->setMaximum(100);
    slider->setMinimum(0);
    slider->setGeometry(10,10,150,15);
    slider->setValue(100);
    slider->setHidden(true);
    opacitySlider = slider;
    connect(slider,&QSlider::valueChanged,this,[this](int value){
        if (currentBinaryItem and currentBinaryItem->scene() == &scene)
        {
            qreal op = qreal(value) / 100;
            opacity = op;
            currentBinaryItem->setOpacity(op);
        }
    });
}














