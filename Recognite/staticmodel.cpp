#include "staticmodel.h"

void StaticModel::init()
{

}

std::pair<int, int> StaticModel::getMaxMinFrequencies() const
{
    int max = std::numeric_limits<int>::min();
    int min = std::numeric_limits<int>::max();

    std::for_each(frequencies.begin(),frequencies.end(),[&min,&max](QVector<int>& f)
    {
        max = *std::max_element(f.begin(),f.end());
        min = *std::min_element(f.begin(),f.end());
    });

    return std::make_pair(max,min);
}

void StaticModel::dropModel()
{
    frequencies.clear();
    sources.clear();
    dests.clear();
    inputModels.clear();
    objectsMap.clear();
}
