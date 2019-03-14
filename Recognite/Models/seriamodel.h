#ifndef SERIAMODEL_H
#define SERIAMODEL_H

#include <QString>
#include <QVector>
#include <QDir>
#include <QStringList>
#include <QFileInfoList>


class SeriaModel
{
public:

    SeriaModel() = default;
    SeriaModel(const QString& folder);
    SeriaModel(SeriaModel const&);
    SeriaModel &operator=(SeriaModel const& rvalue);

    void setFolderPath(const QString &value);

    QString getFolderPath() const;

    QVector<QString> getFiles() const;

private://property
    QString folderPath;
    QVector<QString> files;
};


#endif // SERIAMODEL_H
