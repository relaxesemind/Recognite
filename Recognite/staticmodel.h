#ifndef STATICMODEL_H
#define STATICMODEL_H

#include "Common/magic.h"
#include "Models/models.h"
#include "Common/currentappstate.h"

#include <QObject>
#include <QVector>
#include <QMap>
#include <utility>
#include <QImage>
#include <algorithm>
#include <QString>
#include <QDir>


class StaticModel : public QObject
{
    Q_OBJECT
    MakeSingletonFromClass(StaticModel)

public:
        void init();
        template<class lambda> void foreachArea(lambda call)
        {
            std::for_each(objectsMap.begin(),objectsMap.end(),[&](QVector<Area>& singleImageObjects)
            {
                std::for_each(singleImageObjects.begin(),singleImageObjects.end(),call);
            });

        }
        std::pair<int,int> getMaxMinFrequencies() const;
        void dropModel();

public slots:
        void addDestPair(const QString &path, const QImage &image);
        QStringList getAllFolderFiles(const QString& folder) const;
        QStringList getAllFolderFiles(int index) const;
        QVector<QImage> getCurrentSeriaImages() const;
        QVector<QImage> getCurrentSeriaBinImages() const;
        QImage getCurrentImage() const;
        QImage getCurrentBinImage() const;

public:
    float absoluteMAXheight; // по абсолютно всем моделям
    float absoluteMINheight; // по абсолютно всем моделям

    QMap<QString, QVector<int>> frequencies; // частоты для диаграмм (ключ - путь к папке серии)
    QMap<QString,QImage> sources, dests; // сурсы и десты для всего ключ - путь к txt
    QVector<InputModel> inputModels; // после парса из txt
    QVector<QString> folders;
    QMap<QString,QVector<Area>> objectsMap; // Объекты соответствуют единственной модели
};

#endif // STATICMODEL_H
