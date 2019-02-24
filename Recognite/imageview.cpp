#include "imageview.h"

ImageView::ImageView(QWidget* widget) : QGraphicsView(widget)
{
    this->setScene(&scene);
    this->setAlignment(Qt::AlignCenter);
    currentMode = ImageViewMode::sourceAndDestsView;
    currentScale = 1.0;
    layout = nullptr;
    setupSlider();
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

void ImageView::setMode(ImageViewMode mode)
{
    currentMode = mode;

    if (currentMode == ImageViewMode::sourceAndDestsView)
    {
        opacitySlider->setHidden(true);
        currentBinaryItem = nullptr;
    }

    if (currentMode == ImageViewMode::transparentOver)
    {
        opacitySlider->setHidden(false);

    }
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
            qreal opacity = qreal(value) / 100;
            currentBinaryItem->setOpacity(opacity);
        }
    });
}

ImageViewMode ImageView::getCurrentMode() const
{
    return currentMode;
}













