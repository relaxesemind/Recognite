#include "staticmodel.h"

void StaticModel::init()
{

}

std::pair<int, int> StaticModel::getMaxMinFrequencies() const
{
    int max = std::numeric_limits<int>::min();
    int min = std::numeric_limits<int>::max();

    std::for_each(frequencies.begin(),frequencies.end(),[&min,&max](const QVector<int>& f)
    {
        max = *std::max_element(f.begin(),f.end());
        min = *std::min_element(f.begin(),f.end());
    });

    return std::make_pair(max,min);
}

void StaticModel::dropModel()
{
    frequencies.clear();
    sources.clear();
    dests.clear();
    inputModels.clear();
    objectsMap.clear();
    series.clear();

    absoluteMAXheight = std::numeric_limits<int>::min();
    absoluteMINheight = std::numeric_limits<int>::max();
}

void StaticModel::addDestPair(const QString &path, const QImage &image)
{
    dests.insert(path,image);
}


QVector<QImage> StaticModel::getCurrentSeriaImages() const
{
    SeriaModel seria = CurrentAppState::shared().currentSeria;
    QVector<QString> files = seria.getFiles();

    QVector<QImage> result;

    for (int i = 0; i < files.count(); i++)
    {
        QString filePath = files[i];
        auto it = sources.find(filePath);
        if (it != sources.end())
        {
            result.append(*it);
        }
    }

    return result;
}

QVector<QImage> StaticModel::getCurrentSeriaBinImages() const
{
    SeriaModel seria = CurrentAppState::shared().currentSeria;
    QVector<QString> files = seria.getFiles();

    QVector<QImage> result;

    for (int i = 0; i < files.count(); i++)
    {
        QString filePath = files[i];
        auto it = dests.find(filePath);
        if (it != dests.end())
        {
            result.append(*it);
        }
    }

    return result;
}

QImage StaticModel::getCurrentImage() const
{
    QString currentFile = CurrentAppState::shared().currentFilePath;
    auto it = sources.find(currentFile);

    if (it != sources.end())
    {
        return *it;
    }

    return QImage();
}

QImage StaticModel::getCurrentBinImage() const
{
    QString currentFile = CurrentAppState::shared().currentFilePath;
    auto it = dests.find(currentFile);

    if (it != dests.end())
    {
        return *it;
    }

    return QImage();
}





