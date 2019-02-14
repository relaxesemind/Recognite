#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include "Common/magic.h"
#include "Models/inputmodel.h"

class Parser : public QObject
{
    Q_OBJECT
public:
     Parser() = default;

     InputModel inputModelFromFile(const QString& path, int id = 0);

};

#endif // PARSER_H
