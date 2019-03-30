#ifndef AUTOHEIGHTAPPROACHPROCCESS_H
#define AUTOHEIGHTAPPROACHPROCCESS_H

#include <QObject>
#include <QRunnable>
#include <QStringList>
#include <utility>

#include "staticmodel.h"
#include "Common/currentappstate.h"
#include "Models/models.h"
#include "core.h"


class AutoHeightApproachProccess: public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit AutoHeightApproachProccess();
    ~AutoHeightApproachProccess(){}

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

#endif // AUTOHEIGHTAPPROACHPROCCESS_H
