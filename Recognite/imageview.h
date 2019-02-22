#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "Common/consts.h"
#include "gradientaxis.h"

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QResizeEvent>
#include <memory>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>


class ImageView : public QGraphicsView
{
    Q_OBJECT

    using PXitem = QGraphicsPixmapItem;
    using pItem = std::unique_ptr<PXitem>;
public:
    explicit ImageView(QWidget* widget = nullptr);

protected:
    void wheelEvent(QWheelEvent *event)override;

signals:

public slots:
    void setImage(const QPixmap& pixmap);
    void addGradientAxis(float min, float max);

private:
    QGraphicsScene scene;
    qreal currentScale;
    pItem currentImageItem;
    QGraphicsProxyWidget *proxyAxis;
    QHBoxLayout *layout;

    bool isAxisVisible;
    float minValue, maxValue;
};

#endif // IMAGEVIEW_H
