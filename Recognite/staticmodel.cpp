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
}

void StaticModel::addDestPair(const QString &path, const QImage &image)
{
    dests.insert(path,image);
}

QStringList StaticModel::getAllFolderFiles(const QString& folderPath) const
{
    QStringList filePaths;
    QDir folder(folderPath);

    if (!folder.exists())
    {
        return QStringList{};
    }

    folder.setFilter(QDir::Files);
    folder.setSorting(QDir::Name);

    QFileInfoList folderitems(folder.entryInfoList());

        foreach (QFileInfo i_file, folderitems)
        {
            QString i_filename(i_file.fileName());
            if (i_filename == "." || i_filename == ".." || i_filename.isEmpty())
            {
                continue;
            }

            filePaths << i_file.absoluteFilePath();
        }

    return filePaths;
}

QStringList StaticModel::getAllFolderFiles(int index) const
{
    if (index >= 0 and index <= folders.count())
    {
        return getAllFolderFiles(folders[index]);
    }
    else
    {
        return QStringList{};
    }
}

QVector<QImage> StaticModel::getCurrentSeriaImages() const
{
    QStringList files = CurrentAppState::shared().getCurrentSeriaFiles();
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
    QStringList files = CurrentAppState::shared().getCurrentSeriaFiles();
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





