#include "inputmodel.h"

InputModel::InputModel(): id(0), path(QString()), matrix(0), min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::min())
{

}

InputModel::InputModel(int _id, const QString &_path, QVector<QVector<float> > &&_matrix): id(_id), path(_path), matrix(_matrix),
    min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::min())
{

}

std::pair<float, float> InputModel::getMaxMin(bool force)
{
    if (force or (max == std::numeric_limits<float>::min() and min == std::numeric_limits<float>::max()))
    {
        float maxValue = std::numeric_limits<float>::min();
        float minValue = std::numeric_limits<float>::max();

        for (int i = 0; i < matrix.size(); ++i)
        {
            std::for_each(matrix[i].begin(),matrix[i].end(),[&maxValue, &minValue](float value)
            {
                maxValue = value > maxValue ? value : maxValue;
                minValue = value < minValue ? value : minValue;
            });
        }
        min = minValue;
        max = maxValue;
    }

    return std::make_pair(max,min);
}
