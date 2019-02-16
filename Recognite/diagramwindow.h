#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include "grapher.h"

namespace Ui {
class DiagramWindow;
}

class DiagramWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DiagramWindow(QWidget *parent = 0);
    ~DiagramWindow();

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::DiagramWindow *ui;
};

#endif // DIAGRAMWINDOW_H
