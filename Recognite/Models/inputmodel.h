#ifndef INPUTMODEL_H
#define INPUTMODEL_H


#include <QVector>
#include <QString>
#include <algorithm>
#include <cmath>

struct InputModel
{
    InputModel() = default;
    InputModel(int _id, const QString& _path, QVector<QVector<float>>&& _matrix): id(_id), path(_path), matrix(_matrix), min(0.f), max(0.f) {}

    template<class lambda> void foreachHeight(lambda callableObject)
    {
        for (int i = 0; i < matrix.size(); ++i)
        {
            std::for_each(matrix[i].begin(),matrix[i].end(),callableObject);
        }
    }

    InputModel& operator=(const InputModel& rvalue)
    {
        if (this == &rvalue)
        {
            return *this;
        }

        id = rvalue.id;
        path = rvalue.path;
        matrix = rvalue.matrix;

        return *this;
    }

    int sizeX() const
    {
        return matrix.isEmpty() ? 0 : matrix.first().size();
    }

    int sizeY() const
    {
        return matrix.size();
    }

    bool isValid() const
    {
        return !path.isEmpty() and !matrix.isEmpty();
    }

    int colorOfHeight(int x, int y) const
    {
        return static_cast<int>(matrix[y][x] * 255.f / std::abs(max - min));
    }

//properties
    int id;
    QString path;
    QVector<QVector<float>> matrix; 
    float min, max;

};

#endif // INPUTMODEL_H
