#ifndef CONSTS_H
#define CONSTS_H

#include <QObject>
#include <QRgb>

namespace Consts
{
   const qreal zoomMultiple = 1.05; /* зум ImageView */
   const QRgb whiteRgb = qRgb(255,255,255);
   const QRgb blackRgb = qRgb(0,0,0);
   const int previewSourceImageHeight = 110; /* размер превьюшки на tableWidget */
//   const float derivativeStability = 0.05f; /* если разница производных меньше этой константы они считаются равными */
//   const int traverseWalkStep = 1; /* размер шага в пикселях для алгоритма расширения области пика объекта (поиск истинной высоты) */

}

#endif // CONSTS_H
