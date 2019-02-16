#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);
    pool = new QThreadPool(this);

    ui->listWidget->addItems(QStringList
    {
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_1.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_2.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_3.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_4.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_5.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_6.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_7.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_8.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_9.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_10.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_11.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_12.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_13.txt",
         "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)/310119-1_1F Height_14.txt"
   });
}

MainWindow::~MainWindow()
{
    delete selectingTask;
    delete pool;
    delete ui;
}

void MainWindow::updateProcessPercentage(int value)
{
    QProgressBar *bar = ui->progressBar;
    auto& dests = StaticModel::shared().dests;

    bar->setTextVisible(true);
    if (value == 100)
    {
        bar->setValue(0);
        QList<QTableWidgetItem *> list = ui->tableWidget->selectedItems();
        if (list.isEmpty())
        {
            return;
        }

        int id = list.first()->column();
        if (id < 0 or id > dests.count() - 1)
        {
            return;
        }
        ui->imageViewSelected->setImage(QPixmap::fromImage(dests[id].second));
    }
    else
    {
        bar->setValue(value);
    }
}

void MainWindow::enableDiagramButton(bool flag)
{
    ui->diagramPushButton->setEnabled(flag);
}

void MainWindow::makeImageFromFilePath(const QString &path)
{
    QImage image = Core::shared().imageFromTxtFile(path);
    auto& sources = StaticModel::shared().sources;
    auto pair = std::make_pair(path,image);

    if (std::find(sources.begin(),sources.end(), pair) == sources.end())
    {
        sources.append(pair);
    }

    ui->imageView->setImage(QPixmap::fromImage(image));
    this->updateTableWidget();
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

void MainWindow::on_pushButton_clicked()//build images
{
    QListWidget *listWidget = ui->listWidget;
    for (int i = 0; i < listWidget->count(); ++i)
    {
        this->makeImageFromFilePath(listWidget->item(i)->text());
    }
}

void MainWindow::updateTableWidget()
{
    QTableWidget *table = ui->tableWidget;
    auto& sources = StaticModel::shared().sources;

    table->setColumnCount(sources.count());
    table->setRowCount(1);

    for (int i = 0; i < sources.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        table->setRowHeight(i,115);
        table->setColumnWidth(i,115);
        item->setData(Qt::DecorationRole,QPixmap::fromImage(sources.at(i).second).scaled(110,110,Qt::KeepAspectRatio));
        table->setItem(0,i,item);
    }
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int id = index.column();
    auto& sources = StaticModel::shared().sources;
    auto& dests = StaticModel::shared().dests;

    if (id < 0 or id > sources.count() - 1)
    {
        return;
    }

    ui->imageView->setImage(QPixmap::fromImage(sources.at(id).second));

    if (id > dests.count() - 1)
    {
        return;
    }

    ui->imageViewSelected->setImage(QPixmap::fromImage(dests.at(id).second));
}

void MainWindow::on_processPushButton_clicked()
{
    auto& sources = StaticModel::shared().sources;
    auto& dests = StaticModel::shared().dests;

    if (sources.isEmpty())
    {
        AppMessage("Ошибка","Изображения отсутствуют");
        return;
    }

    if (selectingTask and selectingTask->isRunning())
    {
        qDebug () << "task is running";
        return;
    }

    int min = ui->minHeightLineEdit->text().toInt();
    int max = ui->maxHeightLineEdit->text().toInt();
    Core::shared().setRange(min,max);

    QStringList paths;

    for (int i = 0; i < sources.count(); ++i)
    {
        paths << sources[i].first;
    }

    if (paths.isEmpty())
    {
        return;
    }

    dests.clear();
    selectingTask = new SelectingProcessManager(paths,this);

    connect(selectingTask,&SelectingProcessManager::destPair,&StaticModel::shared(),&StaticModel::addDestPair);
    connect(selectingTask,&SelectingProcessManager::setEnableDiagram,this,&MainWindow::enableDiagramButton);
    connect(selectingTask,&SelectingProcessManager::processPercent,this,&MainWindow::updateProcessPercentage);

    pool->start(selectingTask);
}


void MainWindow::on_diagramPushButton_clicked()
{
//    DiagramWindow *diagram = new DiagramWindow(this);

//    diagram->show();
    auto pair = Core::shared().findAbsoluteMaxMinHeights();
    qDebug() << "max = " << pair.first << " min = " << pair.second;
}






