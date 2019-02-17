#include "diagramwindow.h"
#include "ui_diagramwindow.h"

DiagramWindow::DiagramWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiagramWindow)
{
    ui->setupUi(this);
    auto points = Core::shared().calcPointsForGraph();
    Grapher::shared().addPointsAtGraph(points);
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

void DiagramWindow::on_lineEdit_editingFinished()
{
    QString text = ui->lineEdit->text();
    emit numberOfColumnDidChange(text.toInt());
}

void DiagramWindow::on_pushButton_clicked()//recalc
{
    QString text = ui->lineEdit->text();
    int value = text.toInt();
    Core::shared().calculateFrequencies(value);
    auto points = Core::shared().calcPointsForGraph();
    Grapher::shared().clearView();
    Grapher::shared().addPointsAtGraph(points);
}
