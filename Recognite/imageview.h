#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "Common/consts.h"
#include "gradientaxis.h"
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

enum class ImageViewMode : int
{
     sourceAndDestsView = 0,
     transparentOver = 1
};

class ImageView : public QGraphicsView
{
    Q_OBJECT

    using PXitem = QGraphicsPixmapItem;
    using pItem = std::unique_ptr<PXitem>;
public:
    explicit ImageView(QWidget* widget = nullptr);

    ImageViewMode getCurrentMode() const;

protected:
    void wheelEvent(QWheelEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;

signals:
    void showHeightToolTip(const QPoint& matrixCoord, const QPoint& globalPos);

public slots:
    void setImage(const QPixmap& pixmap);
    void setBinaryImage(const QPixmap& pixmap);
    void addGradientAxis(float min, float max);
    void setMode(ImageViewMode mode);

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

    ImageViewMode currentMode;
    bool isAxisVisible;
    float minValue, maxValue;
};

#endif // IMAGEVIEW_H
