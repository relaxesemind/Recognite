#include "selectingprocessmanager.h"

SelectingProcessManager::SelectingProcessManager(const QStringList &paths) : paths(paths)
{

}



void SelectingProcessManager::run()
{
    emit isRunning(true);
    emit setEnableDiagram(false);

    StaticModel::shared().objectsMap.clear();

    for (int i = 0; i < paths.count(); ++i)
    {
       QString path = paths.at(i);
       QImage dest = Core::shared().binImageFromTxtFile(path);
       emit destPair(path,dest);
       emit processPercent((1 + i) * 100 / paths.count());
    }

    emit setEnableDiagram(!StaticModel::shared().objectsMap.isEmpty());
    emit isRunning(false);
}
