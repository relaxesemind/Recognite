#include "staticmodel.h"

std::pair<int, int> StaticModel::getMaxMinFrequencies() const
{
    int max = *std::max_element(frequencies.begin(),frequencies.end());
    int min = *std::min_element(frequencies.begin(),frequencies.end());
    return std::make_pair(max,min);
}

int StaticModel::getAccumulateFreq(bool force)
{
    if (force or sumFreq == 0)
    {
        sumFreq = std::accumulate(frequencies.begin(),frequencies.end(),0);
    }

    return sumFreq;
}

void StaticModel::addDestPair(const QString &path, const QImage &image)
{
     dests.append(std::make_pair(path,image));
}
