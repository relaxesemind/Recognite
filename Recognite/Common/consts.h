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
   const float defaultFrequencyInterval = 0.3f; /* для построения диаграмм */
}

#endif // CONSTS_H
