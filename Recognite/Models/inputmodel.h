#ifndef INPUTMODEL_H
#define INPUTMODEL_H


#include <QVector>
#include <QString>
#include <algorithm>

struct InputModel
{
    InputModel() = default;
    InputModel(int _id, const QString& _path, QVector<QVector<float>>&& _matrix): id(_id), path(_path), matrix(_matrix) {}

    template<class lambda> void foreachHeight(lambda call)
    {
        for (int i = 0; i < matrix.size();++i)
        {
            std::for_each(matrix[i].begin(),matrix[i].end(),call);
        }
    }

    int sizeX()
    {
        return matrix.isEmpty() ? 0 : matrix.first().size();
    }

    int sizeY()
    {
        return matrix.size();
    }

    bool isValid()
    {
        return !path.isEmpty() and !matrix.isEmpty();
    }

//properties
    int id;
    QString path;
    QVector<QVector<float>> matrix; 
};

#endif // INPUTMODEL_H
