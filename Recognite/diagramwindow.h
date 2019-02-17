#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include "grapher.h"
#include "core.h"


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

private:
    Ui::DiagramWindow *ui;
};

#endif // DIAGRAMWINDOW_H
