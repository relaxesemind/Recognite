#ifndef CURRENTAPPSTATE_H
#define CURRENTAPPSTATE_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include "Views/ImageView/imageviewmode.h"

class CurrentAppState
{
public:
    CurrentAppState(CurrentAppState const&) = delete ;
    CurrentAppState& operator=(CurrentAppState const&) = delete ;
    static CurrentAppState& shared()
    {
        static CurrentAppState instance;
        return instance;
    }

private:
    CurrentAppState();

public:
   QString currentFilePath; // отображаемый на ImageView
   QString currentFolder; // текущая папка
   bool selectingTaskIsRunning; // поток с выделением областей
   ImageViewMode imageViewMode;

   QStringList getAllFolderFiles() const;
x
};

#endif // CURRENTAPPSTATE_H
