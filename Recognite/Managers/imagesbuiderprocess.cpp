#include "imagesbuiderprocess.h"

ImagesBuiderProcess::ImagesBuiderProcess(const QVector<SeriaModel> &series): series(series)
{

}

void ImagesBuiderProcess::run()
{
    emit isRunning(true);

    auto& sources = StaticModel::shared().sources;
    StaticModel::shared().inputModels.clear();
    sources.clear();

    int numOfFiles = 0;
    int filesProcessed = 0;

    for (SeriaModel &seria : series)
    {
        numOfFiles += seria.getFiles().count();
    }

    for (int i = 0; i < series.count(); ++i)
    {
        SeriaModel seria = series.at(i);
        QDir folder(seria.getFolderPath());

        if (!folder.exists()){continue;}

        QVector<QString> filePaths = seria.getFiles();

        if (i == 0)
        {
             CurrentAppState::shared().currentSeria = seria;
        }

        for (int j = 0; j < filePaths.count(); ++j)
        {
            QString path = filePaths.at(j);
            QImage image = Core::shared().imageFromTxtFile(path);
            if (std::find(sources.begin(),sources.end(), image) == sources.end())
            {
                sources.insert(path,image);
            }
            emit processPercent((++filesProcessed) * 100 / numOfFiles);
        }

//        std::for_each(filePaths.begin(),filePaths.end(),[&sources](const QString& path)
//        {
//            QImage image = Core::shared().imageFromTxtFile(path);

//            if (std::find(sources.begin(),sources.end(), image) == sources.end())
//            {
//                sources.insert(path,image);
//            }
//        });
        }

    emit isRunning(false);
    emit isDone(sources.count() ? true : false);
}
