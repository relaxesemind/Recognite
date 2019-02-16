#include "diagramwindow.h"
#include "ui_diagramwindow.h"

DiagramWindow::DiagramWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiagramWindow)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(Grapher::shared().view,1,0);
}

DiagramWindow::~DiagramWindow()
{
    delete ui;
}

void DiagramWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(value));
}
