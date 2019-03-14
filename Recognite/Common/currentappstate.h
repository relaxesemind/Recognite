#ifndef CURRENTAPPSTATE_H
#define CURRENTAPPSTATE_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include "Views/ImageView/imageviewmode.h"
#include "Models/seriamodel.h"


class CurrentAppState
{
public:
    CurrentAppState(CurrentAppState const&) = delete;
    CurrentAppState& operator=(CurrentAppState const&) = delete;
    static CurrentAppState& shared()
    {
        static CurrentAppState instance;
        return instance;
    }

private:
    CurrentAppState();

public:
   QString currentFilePath; // отображаемый на ImageView
   SeriaModel currentSeria;
   bool selectingTaskIsRunning; // поток с выделением областей
   ImageViewMode imageViewMode;

};

#endif // CURRENTAPPSTATE_H
