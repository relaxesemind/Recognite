#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "Common/magic.h"
#include "Models/inputmodel.h"
#include "Common/settingsstorage.h"


class Parser : public QObject
{
    Q_OBJECT
public:
     Parser() = default;

     InputModel inputModelFromFileDefault(const QString& path, int id = 0);
     InputModel inputModelFromFileWithCommas(const QString& path, int id = 0);
     InputModel inputModelFromNanoScope(const QString& path, int id = 0);

};

#endif // PARSER_H
