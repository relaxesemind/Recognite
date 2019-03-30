#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "diagramwindow.h"
#include "Managers/parser.h"
#include "core.h"

#include "Managers/selectingprocessmanager.h"
#include "Managers/imagesbuiderprocess.h"

#include "staticmodel.h"
#include "parsersettingsdialog.h"
#include "traversewalksettings.h"
#include "Common/currentappstate.h"

#include "Views/ImageView/imageview.h"
#include "Views/waitingspinnerwidget.h"


#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <utility>
#include <memory>
#include <QPixmap>
#include <QStringList>
#include <QThreadPool>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QDesktopWidget>
#include <QToolTip>
#include <QDir>
#include <QFileInfo>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateProcessPercentage(int);
    void enableDiagramButton(bool);

private slots:

    void on_loadTxtFiles_triggered();

    void on_maxHeightSlider_valueChanged(int value);

    void on_minHeightSlider_valueChanged(int value);

    void on_pushButton_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_processPushButton_clicked();

    void on_diagramPushButton_clicked();

    void showListMenuAtPos(QPoint pos);

    void on_changeShowMode_triggered();

    void on_actionInputFormatEdit_triggered();

    void on_action_TraverseWalk_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void selectingTaskIsFinished();

    void on_maxHeightLineEdit_editingFinished();

    void on_minHeightLineEdit_editingFinished();

private:
    void updateTableWidget();
    void updateViewWithSeria();
    void updateImageViews();
    void makeImageFromFilePath(const QString& path);
    void setupListWidget();
    void setupImageView();
    void addSeria();
    void buildImages();

private:
    Ui::MainWindow *ui;
    QThreadPool *pool;
//    SelectingProcessManager *selectingTask;
    QString folderPath;
    WaitingSpinnerWidget *spinner;
};

#endif // MAINWINDOW_H
