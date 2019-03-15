#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include "grapher.h"
#include "core.h"
#include "staticmodel.h"
#include "Common/magic.h"
#include "Common/currentappstate.h"
#include "Common/colorgenerator.h"

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QInputDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTextCodec>
#include <QColorDialog>


namespace Ui {
class DiagramWindow;
}

class DiagramWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DiagramWindow(QWidget *parent = 0);
    ~DiagramWindow();
signals:
    void numberOfColumnDidChange(int);

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_lineEdit_editingFinished();

    void on_pushButton_clicked();

    void on_exportFileAction_triggered();

    void on_barDiagramChecker_triggered();

    void on_splineChecker_triggered();

    void on_action_8_triggered();

private:
    Ui::DiagramWindow *ui;
    void drawGraph();
    void writeDataToStream(QTextStream &out);
    QMap<QString, QVector<QPointF>> pointsForGraph;
    GrapherMode::Options mode;
    ColorGenerator<> gena;
    QMap<QString,QColor> barColors, splineColors;
};

#endif // DIAGRAMWINDOW_H
