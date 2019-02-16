#ifndef STATICMODEL_H
#define STATICMODEL_H

#include "Common/magic.h"
#include "Models/models.h"

#include <QObject>
#include <QVector>
#include <QMap>
#include <utility>
#include <QImage>
#include <QString>


class StaticModel : public QObject
{
    Q_OBJECT
    MakeSingletonFromClass(StaticModel)

public slots:
        void addDestPair(const QString &path, const QImage &image);

public:
    float absoluteMAXheight;
    float absoluteMINheight;
    QVector<int> frequencies;
    QVector<std::pair<QString,QImage>> sources, dests;
    QMap <
         QString,
         QVector<Area>
         > objectsMap;
};

#endif // STATICMODEL_H
