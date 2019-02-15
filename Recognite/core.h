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
#include "Common/magic.h"
#include "Models/models.h"
#include "Managers/parser.h"
#include "Common/consts.h"


class Core : public QObject
{
    Q_OBJECT
    MakeSingletonFromClass(Core)

public:
    QImage imageFromTxtFile(const QString& path);
    QImage binImageFromTxtFile(const QString& path);
    void setRange(int min, int max);

signals:

public slots:

private:
    Parser parser;
    QVector<InputModel> models;
    int rangeMax;
    int rangeMin;
    bool inRange(qint32 x, qint32 y, const InputModel& model);
    void fill(const InputModel& model, QVector<QVector<qint32>>& V, qint32 x, qint32 y, qint32 L);
};

#endif // CORE_H
