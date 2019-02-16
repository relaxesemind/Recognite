#include "staticmodel.h"

void StaticModel::addDestPair(const QString &path, const QImage &image)
{
     dests.append(std::make_pair(path,image));
}
