#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "Common/consts.h"
#include "Views/GradientAxis/gradientaxis.h"
#include "Views/ImageView/imageviewmode.h"
#include "staticmodel.h"

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QResizeEvent>
#include <memory>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>


class ImageView : public QGraphicsView
{
    Q_OBJECT

    using PXitem = QGraphicsPixmapItem;
    using pItem = std::unique_ptr<PXitem>;
public:
    explicit ImageView(QWidget* widget = nullptr);

protected:
    void wheelEvent(QWheelEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;

signals:
    void showHeightToolTip(const QPoint& matrixCoord, const QPoint& globalPos);

public slots:
    void setImage(const QPixmap& pixmap);
    void setBinaryImage(const QPixmap& pixmap);
    void addGradientAxis(float min, float max);
    void showSlider(bool flag);

private:
    void setupSlider();

    QGraphicsScene scene;
    qreal currentScale;
    pItem currentImageItem;
    pItem currentBinaryItem;
    QSlider *opacitySlider;
    qreal opacity;

    QGraphicsProxyWidget *proxyAxis;
    QHBoxLayout *layout;
    GradientAxis *axis;

    bool isAxisVisible;
    float minValue, maxValue;
};

#endif // IMAGEVIEW_H
