#include "currentappstate.h"

CurrentAppState::CurrentAppState():
    currentFilePath(""),currentFolder(""),selectingTaskIsRunning(false),imageViewMode(ImageViewMode::sourceAndDestsView)
{

}

QStringList CurrentAppState::getAllFolderFiles() const
{
    QStringList filePaths;

    QDir folder(currentFolder);

    folder.setFilter(QDir::Files);
    folder.setSorting(QDir::Name);

    QFileInfoList folderitems(folder.entryInfoList());

        foreach (QFileInfo i_file, folderitems)
        {
            QString i_filename(i_file.fileName());
            if (i_filename == "." || i_filename == ".." || i_filename.isEmpty())
            {
                continue;
            }

            filePaths << i_file.absoluteFilePath();
        }

    return filePaths;
}
