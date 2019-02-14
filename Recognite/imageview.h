#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <memory>

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

private:
    QGraphicsScene scene;
    pItem currentImageItem;
};

#endif // IMAGEVIEW_H
