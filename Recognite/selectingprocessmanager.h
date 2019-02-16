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
    explicit SelectingProcessManager(const QStringList& paths, QObject *parent = nullptr);
    bool isRunning();

protected:
    void run();

signals:
    void processPercent(int);
    void setEnableDiagram(bool);
    void destPair(const QString&,const QImage&);

public slots:

private:
    QStringList paths;
    bool status;
};

#endif // SELECTINGPROCESSMANAGER_H
