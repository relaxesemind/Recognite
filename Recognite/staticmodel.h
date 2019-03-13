#ifndef STATICMODEL_H
#define STATICMODEL_H

#include "Common/magic.h"
#include "Models/models.h"

#include <QObject>
#include <QVector>
#include <QMap>
#include <utility>
#include <QImage>
#include <algorithm>
#include <QString>
#include <QChar>


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
        int getAccumulateFreq(bool force = true);

public slots:
        void addDestPair(const QString &path, const QImage &image);

public:
    float absoluteMAXheight;
    float absoluteMINheight;
    QVector<int> frequencies;
    QVector<std::pair<QString,QImage>> sources, dests;
    QVector<InputModel> inputModels;
    QMap <
         QString,
         QVector<Area>
         > objectsMap;

private:
    int sumFreq;
};

#endif // STATICMODEL_H
