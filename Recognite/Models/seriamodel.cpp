#include "seriamodel.h"

SeriaModel::SeriaModel(const QString &folderS): folderPath(folderS)
{
    QDir folder(folderS);

    if (!folder.exists())
    {
        return;
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

        files.append(i_file.absoluteFilePath());
    }
}

SeriaModel::SeriaModel(const SeriaModel &rvalue)
{
    setFolderPath(rvalue.getFolderPath());
}

SeriaModel &SeriaModel::operator=(const SeriaModel &rvalue)
{
    if (this == &rvalue)
    {
        return *this;
    }

    setFolderPath(rvalue.getFolderPath());
    return *this;
}

void SeriaModel::setFolderPath(const QString &value)
{
    folderPath = value;

    QDir folder(value);


    if (!folder.exists())
    {
        return;
    }

    files.clear();
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

        files.append(i_file.absoluteFilePath());
    }
}

QString SeriaModel::getFolderPath() const
{
    return folderPath;
}

QString SeriaModel::getSeriaLastComponent() const
{
    QStringList components = folderPath.split("/");
    if (components.isEmpty())
    {
        return QString();
    }

    return components.last();
}

QVector<QString> SeriaModel::getFiles() const
{
    return files;
}
