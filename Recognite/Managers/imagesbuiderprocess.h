#ifndef IMAGESBUIDERPROCESS_H
#define IMAGESBUIDERPROCESS_H

#include <QObject>
#include <QRunnable>
#include <QStringList>
#include <utility>

#include "staticmodel.h"
#include "Common/currentappstate.h"
#include "Models/models.h"
#include "core.h"


class ImagesBuiderProcess : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit ImagesBuiderProcess(QVector<SeriaModel> const& series);
    ~ImagesBuiderProcess(){}

protected:
    void run();

signals:
    void processPercent(int);
    void isDone(bool);
    void isRunning(bool);

public slots:

private:
     QVector<SeriaModel> series;
};

#endif // IMAGESBUIDERPROCESS_H
