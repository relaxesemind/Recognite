#include "diagramwindow.h"
#include "ui_diagramwindow.h"

DiagramWindow::DiagramWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiagramWindow)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(Grapher::shared().view);
}

DiagramWindow::~DiagramWindow()
{
    delete ui;
}
