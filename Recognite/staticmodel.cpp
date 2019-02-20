#include "staticmodel.h"

std::pair<int, int> StaticModel::getMaxMinFrequencies() const
{
    int max = *std::max_element(frequencies.begin(),frequencies.end());
    int min = *std::min_element(frequencies.begin(),frequencies.end());
    return std::make_pair(max,min);
}

void StaticModel::addDestPair(const QString &path, const QImage &image)
{
     dests.append(std::make_pair(path,image));
}
