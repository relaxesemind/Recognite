#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include <algorithm>
#include <limits>
#include <QVector>
#include <QStack>
#include <QPoint>
#include <QByteArray>
#include <QRgb>
#include "Common/magic.h"
#include "Models/models.h"
#include "Managers/parser.h"


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
    int rangeMax;
    int rangeMin;
    bool inRange(qint32 x, qint32 y, const QVector<QVector<float>>& matrix);
    void fill(const QVector<QVector<float>>& matrix, QVector<QVector<qint32>>& V, qint32 x, qint32 y, qint32 L);
};

#endif // CORE_H
