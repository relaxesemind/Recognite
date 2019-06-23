#ifndef SELECTINGPROCESSMANAGER_H
#define SELECTINGPROCESSMANAGER_H

#include "core.h"
#include "staticmodel.h"

#include <QObject>
#include <QRunnable>
#include <QThreadPool>
#include <QStringList>
#include <utility>


class SelectingProcessManager : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit SelectingProcessManager(QStringList&& paths);
    ~SelectingProcessManager(){}

    bool flag_stop;

protected:
    void run();

signals:
    void processPercent(int);
    void setEnableDiagram(bool);
    void destPair(const QString&, const QImage&);
    void isDone();
    void isRunning(bool);

public slots:

private:
    QStringList paths;
};

#endif // SELECTINGPROCESSMANAGER_H
