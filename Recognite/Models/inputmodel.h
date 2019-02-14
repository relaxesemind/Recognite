#ifndef INPUTMODEL_H
#define INPUTMODEL_H


#include <QVector>
#include <QString>


struct InputModel
{
    InputModel() = default;
    InputModel(int _id, const QString& _path, QVector<QVector<float>>&& _matrix): id(_id), path(_path), matrix(_matrix) {}

//properties
    int id;
    QString path;
    QVector<QVector<float>> matrix;
};

#endif // INPUTMODEL_H
