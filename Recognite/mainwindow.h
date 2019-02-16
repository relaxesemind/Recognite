#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "diagramwindow.h"
#include "Managers/parser.h"
#include "core.h"
#include "selectingprocessmanager.h"
#include "staticmodel.h"


#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <utility>
#include <memory>
#include <QPixmap>
#include <QStringList>
#include <QThreadPool>



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

    void on_maxHeightLineEdit_textChanged(const QString &arg1);

    void on_minHeightLineEdit_textEdited(const QString &arg1);

    void on_pushButton_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_processPushButton_clicked();

    void on_diagramPushButton_clicked();

private:
    void updateTableWidget();
    void makeImageFromFilePath(const QString& path);

private:
    Ui::MainWindow *ui;
    QThreadPool *pool;
    SelectingProcessManager *selectingTask;
};

#endif // MAINWINDOW_H
