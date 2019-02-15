#include "selectingprocessmanager.h"

SelectingProcessManager::SelectingProcessManager(const QStringList &paths, QObject *parent) : QObject(parent), paths(paths)
{
    status = false;
    setAutoDelete(true);
}

bool SelectingProcessManager::isRunning()
{
    return status;
}

void SelectingProcessManager::run()
{
    status = true;
    qDebug ()  << " true";
    for (int i = 0; i < paths.count(); ++i)
    {
       QString path = paths.at(i);
       QImage dest = Core::shared().binImageFromTxtFile(path);
       emit destPair(path,dest);
       emit processPercent((1 + i) * 100 / paths.count());
    }
    status = false;
    qDebug()  << " false";
}
