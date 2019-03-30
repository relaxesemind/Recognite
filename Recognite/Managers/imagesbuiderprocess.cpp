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

    for (int i = 0; i < series.count(); ++i)
    {
        SeriaModel seria = series.at(i);
        QDir folder(seria.getFolderPath());

        if (!folder.exists())
        {
            continue;
        }

        QVector<QString> filePaths = seria.getFiles();

        if (i == 0)
        {
             CurrentAppState::shared().currentSeria = seria;
        }

        std::for_each(filePaths.begin(),filePaths.end(),[&sources](const QString& path)
        {
            QImage image = Core::shared().imageFromTxtFile(path);

            if (std::find(sources.begin(),sources.end(), image) == sources.end())
            {
                sources.insert(path,image);
            }
        });
    }

    emit isRunning(false);
    emit isDone(sources.count() ? true : false);
}
