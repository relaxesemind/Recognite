#include "mainwindow.h"
#include "ui_mainwindow.h"


//"C:/dev/selection_new/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_1.txt"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()//selectedFile
{
//    QString filePath = ui->listWidget->selectedItems().first()->text();
    QImage image = Core::shared().imageFromTxtFile("C:/dev/selection_new/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_1.txt");
    ui->imageView->setImage(QPixmap::fromImage(image));
}

void MainWindow::on_loadTxtFiles_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(this,"Открыть файл","","*.txt *.all");
    ui->listWidget->addItems(files);
}

void MainWindow::on_maxHeightSlider_valueChanged(int value)
{
    ui->maxHeightLineEdit->setText(QString::number(value));

    if (value < ui->minHeightSlider->value())
    {
        ui->maxHeightSlider->setValue(ui->minHeightSlider->value());
    }
}

void MainWindow::on_minHeightSlider_valueChanged(int value)
{
    ui->minHeightLineEdit->setText(QString::number(value));

    if (value > ui->maxHeightSlider->value())
    {
        ui->minHeightSlider->setValue(ui->maxHeightSlider->value());
    }
}

void MainWindow::on_maxHeightLineEdit_textChanged(const QString &arg1)
{
    bool ok;
    int number = arg1.toInt(&ok,10);
    if (!ok)
    {
        ui->maxHeightLineEdit->setText(QString::number(ui->maxHeightSlider->value()));
    }

    if (number > 255)
    {
        ui->maxHeightLineEdit->setText(QString::number(255));
    }
    if (number < 0)
    {
        ui->maxHeightLineEdit->setText(QString::number(0));
    }
}

void MainWindow::on_minHeightLineEdit_textEdited(const QString &arg1)
{
    bool ok;
    int number = arg1.toInt(&ok,10);
    if (!ok)
    {
        ui->minHeightLineEdit->setText(QString::number(ui->minHeightSlider->value()));
    }

    if (number > 255)
    {
        ui->minHeightLineEdit->setText(QString::number(255));
    }
    if (number < 0)
    {
        ui->minHeightLineEdit->setText(QString::number(0));
    }
}

void MainWindow::on_pushButton_clicked()
{
    int min = ui->minHeightSlider->value();
    int max = ui->maxHeightSlider->value();
    Core::shared().setRange(min,max);

    QImage image = Core::shared().binImageFromTxtFile("C:/dev/selection_new/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_1.txt");
    ui->imageViewSelected->setImage(QPixmap::fromImage(image));
}












