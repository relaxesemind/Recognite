#ifndef CORE_H
#define CORE_H

#include <iostream>

#include <QObject>
#include <QImage>
#include <QDebug>
#include <algorithm>
#include <limits>
#include <cmath>
#include <QVector>
#include <QStack>
#include <QPoint>
#include <QByteArray>
#include <QRgb>
#include <QPointF>
//#include <iostream>

#include "Common/magic.h"
#include "Models/models.h"
#include "Managers/parser.h"
#include "Common/consts.h"
#include "staticmodel.h"
#include "Common/settingsstorage.h"


class Core : public QObject
{
    Q_OBJECT
    MakeSingletonFromClass(Core)

public:
    QImage imageFromTxtFile(const QString& path);
    QImage binImageFromTxtFile(const QString& path);
    void setRange(float min, float max);

    std::pair<float,float> findAbsoluteMaxMinHeights();
    void calculateFrequencies(const QString& seriaPath, int numOfColumn);
    void calculateFrequenciesWithInterval(const QString& seriaPath, float interval);
    QVector<QPointF> calcPointsForGraph(const QString& seriaPath);
    void setMinObjectSize(int value);
    void getTrueHeights(const InputModel &model, QVector<Area>& objects);

signals:

public slots:

private:
    Parser parser;
    float maxFromUI;
    float minFromUI;
    int minObjectSize;
    bool inRange(qint32 x, qint32 y, const InputModel& model);
    void fill(const InputModel& model, QVector<QVector<qint32>>& V, qint32 x, qint32 y, qint32 L);
    void formObjects();
};

#endif // CORE_H
