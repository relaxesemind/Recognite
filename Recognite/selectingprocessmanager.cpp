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
       auto& models = StaticModel::shared().inputModels;
       QImage dest = Core::shared().binImageFromTxtFile(path);

       for (InputModel& model : models)
       {
           if (model.path == path)
           {
               Core::shared().getTrueHeights(model,*StaticModel::shared().objectsMap.find(path));
           }
       }

       emit destPair(path,dest);
       emit processPercent((1 + i) * 100 / paths.count());
    }

    emit setEnableDiagram(!StaticModel::shared().objectsMap.isEmpty());
    emit isRunning(false);
}
