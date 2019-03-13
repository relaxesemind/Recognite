#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pool(new QThreadPool(this))
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);
    selectingTask = nullptr;
    taskIsRunning = false;
    modeFlag = false;
    currentImageId = -1;

    StaticModel::shared().init();

    setupListWidget();
    setupImageView();
}

MainWindow::~MainWindow()
{
    delete pool;
    delete ui;
}

void MainWindow::updateProcessPercentage(int value)
{
    QProgressBar *bar = ui->progressBar;
    auto& dests = StaticModel::shared().dests;

    if (value == 100)
    {
        bar->setValue(0);
//        QList<QTableWidgetItem *> list = ui->tableWidget->selectedItems();
//        if (list.isEmpty())
//        {
//            return;
//        }

//        int id = list.first()->column();
//        if (id < 0 or id > dests.count() - 1)
//        {
//            return;
//        }

//        if (modeFlag == true)
//        {
//            if (currentImageId < dests.count() - 1 and currentImageId >= 0)
//            {
//                ui->imageView->setBinaryImage(QPixmap::fromImage(dests.at(currentImageId).second));
//            }
//        }
//        else
//        {
//            ui->imageViewSelected->setImage(QPixmap::fromImage(dests[id].second));
//        }

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

void MainWindow::setTaskIsRunning(bool flag)
{
    taskIsRunning = flag;
}

void MainWindow::makeImageFromFilePath(const QString &path)
{
    QImage image = Core::shared().imageFromTxtFile(path);
    auto& sources = StaticModel::shared().sources;

    if (std::find(sources.begin(),sources.end(), image) == sources.end())
    {
        sources.insert(path,image);
    }
}

void MainWindow::setupListWidget()
{
    QListWidget *listWidget = ui->listWidget;
    listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->imageView->addGradientAxis(0,0);

    connect(listWidget, &QListWidget::customContextMenuRequested,this, &MainWindow::showListMenuAtPos);
}

void MainWindow::setupImageView()
{
    connect(ui->imageView,&ImageView::showHeightToolTip,this,[this](const QPoint& coord, const QPoint& globalPos)
    {

        auto& sources = StaticModel::shared().sources;

        if (!(currentImageId < sources.count() and currentImageId >= 0))
        {
            return ;
        }

//        QString path = sources.at(currentImageId).first;
//        auto& models = StaticModel::shared().inputModels;
//        for (InputModel& model : models)
//        {
//            if (model.path == path and model.isSafelyIndexes(coord.x(),coord.y()))
//            {

//                QToolTip::showText(globalPos,QString::number(model.matrix[coord.y()][coord.x()],'f',2));
//                break;
//            }
//        }
    });
}

void MainWindow::on_loadTxtFiles_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Выберите папку с серией изображений"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
    {
        return;
    }

    ui->listWidget->addItem(path);
}

void MainWindow::on_maxHeightSlider_valueChanged(int value)
{
    float fValue = static_cast<float>(value) / 10.f;

    ui->maxHeightLineEdit->setText(QString::number(fValue,'f',1));

    if (value < ui->minHeightSlider->value())
    {
        ui->maxHeightSlider->setValue(ui->minHeightSlider->value());
    }
}

void MainWindow::on_minHeightSlider_valueChanged(int value)
{
    float fValue = static_cast<float>(value) / 10.f;

    ui->minHeightLineEdit->setText(QString::number(fValue,'f',1));

    if (value > ui->maxHeightSlider->value())
    {
        ui->minHeightSlider->setValue(ui->maxHeightSlider->value());
    }
}

void MainWindow::on_maxHeightLineEdit_textChanged(const QString &arg1)
{
//    bool ok;
//    int number = arg1.toFloat(&ok);
//    if (!ok)
//    {
//        float fValue = static_cast<float>(ui->maxHeightSlider->value()) / 10.f;
//        ui->maxHeightLineEdit->setText(QString::number(fValue,'f',1));
//    }

//    if (number > 255)
//    {
//        ui->maxHeightLineEdit->setText(QString::number(255));
//    }
//    if (number < 0)
//    {
//        ui->maxHeightLineEdit->setText(QString::number(0));
//    }
}

void MainWindow::on_minHeightLineEdit_textEdited(const QString &arg1)
{
//    bool ok;
//    int number = arg1.toInt(&ok,10);
//    if (!ok)
//    {
//        ui->minHeightLineEdit->setText(QString::number(ui->minHeightSlider->value()));
//    }

//    if (number > 255)
//    {
//        ui->minHeightLineEdit->setText(QString::number(255));
//    }
//    if (number < 0)
//    {
//        ui->minHeightLineEdit->setText(QString::number(0));
//    }
}

void MainWindow::on_pushButton_clicked()//build images
{   
    QListWidget *listWidget = ui->listWidget;
    auto& sources = StaticModel::shared().sources;

    for (int i = 0; i < listWidget->count(); ++i)
    {
           QString path = listWidget->item(i)->text();
           QStringList filePaths;

           QDir folder(path);
           folder.setFilter(QDir::Files);
           folder.setSorting(QDir::Name);

           QFileInfoList folderitems(folder.entryInfoList());

               foreach (QFileInfo i_file, folderitems)
               {
                   QString i_filename(i_file.fileName());
                   if (i_filename == "." || i_filename == ".." || i_filename.isEmpty())
                   {
                       continue;
                   }

                   filePaths << i_file.absoluteFilePath();
               }
           qDebug() << filePaths;

           foreach (QString path, filePaths)
           {
               this->makeImageFromFilePath(path);
           }
    }


    if (!sources.isEmpty())
    {
        ui->imageView->setImage(QPixmap::fromImage(sources.first()));
        currentImageId = 0;
    }


   this->updateTableWidget();

   auto pair = Core::shared().findAbsoluteMaxMinHeights();
   float maxNumber = pair.first;
   float minNumber = pair.second;
   QString max = QString::number(pair.first);
   QString min = QString::number(pair.second);
   int interval = static_cast<int>((maxNumber - minNumber) / 30);


   ui->label_4->setText(QString("Абсолютный MAX = ") + max + QString("нм"));
   ui->label_5->setText(QString("Абсолютный MIN = ") + min + QString("нм"));
   ui->maxHeightSlider->setSingleStep(interval);
   ui->maxHeightSlider->setMaximum(static_cast<int>(maxNumber * 10));
   ui->maxHeightSlider->setMinimum(static_cast<int>(minNumber * 10));
   ui->minHeightSlider->setMaximum(static_cast<int>(maxNumber * 10));
   ui->minHeightSlider->setMinimum(static_cast<int>(minNumber * 10));
   ui->maxHeightSlider->setValue(static_cast<int>(maxNumber * 10));
   ui->minHeightSlider->setValue(static_cast<int>((maxNumber - minNumber) / 4) * 10);

}

void MainWindow::updateTableWidget()
{
    QTableWidget *table = ui->tableWidget;
    auto& sources = StaticModel::shared().sources;

    table->setColumnCount(sources.count());
    table->setRowCount(1);
    const int imgSize = Consts::previewSourceImageHeight;

    for (int i = 0; i < sources.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        table->setRowHeight(i,imgSize + 5);
        table->setColumnWidth(i,imgSize + 5);
        item->setData(Qt::DecorationRole,QPixmap::fromImage(sources.at(i).second).scaled(imgSize,imgSize,Qt::KeepAspectRatio));
        table->setItem(0,i,item);
    }
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int id = index.column();
    currentImageId = id;
    auto& sources = StaticModel::shared().sources;
    auto& dests = StaticModel::shared().dests;
    ImageViewMode mode = ui->imageView->getCurrentMode();

    if (id < 0 or id > sources.count() - 1)
    {
        return;
    }


    if (mode == ImageViewMode::sourceAndDestsView)
    {
        ui->imageView->setImage(QPixmap::fromImage(sources.at(id).second));
        if (id > dests.count() - 1)
        {
            return;
        }
        ui->imageViewSelected->setImage(QPixmap::fromImage(dests.at(id).second));
    }

    if (mode == ImageViewMode::transparentOver)
    {
        ui->imageView->setImage(QPixmap::fromImage(sources.at(id).second));
        if (id > dests.count() - 1)
        {
            return;
        }
        ui->imageView->setBinaryImage(QPixmap::fromImage(dests.at(id).second));
    }
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

    if (taskIsRunning)
    {
        qDebug () << "task is running";
        return;
    }

    int min = ui->minHeightSlider->value();
    int max = ui->maxHeightSlider->value();
    int minObjSize = ui->minObjSizeLineEdit->text().toInt();
    Core::shared().setRange(static_cast<float>(min) / 10.f,static_cast<float>(max) / 10.f);
    Core::shared().setMinObjectSize(minObjSize);

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
    selectingTask = new SelectingProcessManager(paths);

    connect(selectingTask,&SelectingProcessManager::destPair,&StaticModel::shared(),&StaticModel::addDestPair);
    connect(selectingTask,&SelectingProcessManager::setEnableDiagram,this,&MainWindow::enableDiagramButton);
    connect(selectingTask,&SelectingProcessManager::processPercent,this,&MainWindow::updateProcessPercentage);
    connect(selectingTask,&SelectingProcessManager::isRunning,this,&MainWindow::setTaskIsRunning);

    pool->start(selectingTask);
    updateTableWidget();
}


void MainWindow::on_diagramPushButton_clicked()
{
    Grapher::shared().clearView();
    Core::shared().calculateFrequenciesWithInterval(0.3f);
    DiagramWindow *diagram = new DiagramWindow(this);
    diagram->show();
}

void MainWindow::showListMenuAtPos(QPoint pos)
{
    QListWidget *listWidget = ui->listWidget;
    QTableWidget *tableWidget = ui->tableWidget;
    ImageView *imageView = ui->imageView;
    ImageView *destView = ui->imageViewSelected;

    QPoint globalPos = listWidget->mapToGlobal(pos);
    QMenu menu(this);

    QAction *addItemAction = new QAction(QString("Добавить файлы"),this);

    connect(addItemAction,&QAction::triggered,this,[&listWidget,this]
    {
        QString path = QFileDialog::getExistingDirectory(this, tr("Выберите папку с серией изображений"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
        if (path.isEmpty())
        {
            return;
        }

        ui->listWidget->addItem(path);
    });

    QAction *removeAction = new QAction(QString("Удалить файл"),this);
    removeAction->setShortcut(QKeySequence::Delete);

    connect(removeAction,&QAction::triggered,this,[&listWidget]
    {
        for (int i = 0; i < listWidget->selectedItems().size(); ++i)
        {
            QListWidgetItem *item = listWidget->takeItem(listWidget->currentRow());
            delete item;
        }
    });

    QAction *removeAll = new QAction(QString("Удалить все"),this);
    connect(removeAll, &QAction::triggered, this, [&listWidget, &tableWidget, &imageView, &destView]{
        listWidget->clear();
        tableWidget->clear();
        tableWidget->setColumnCount(0);
        imageView->setImage(QPixmap());
        destView->setImage(QPixmap());
        StaticModel::shared().dropModel();
    });
///
    menu.addActions(QList<QAction *>{removeAction, addItemAction,removeAll});
    menu.exec(globalPos);
}

void MainWindow::on_changeShowMode_triggered()
{
    modeFlag = !modeFlag;
    ImageViewMode mode;
    auto& dests = StaticModel::shared().dests;

    if (modeFlag == true)
    {
        mode = ImageViewMode::transparentOver;
        if (currentImageId < dests.count() - 1 and currentImageId >= 0)
        {
            ui->imageView->setBinaryImage(QPixmap::fromImage(dests.at(currentImageId).second));
        }
    }
    else
    {
        mode = ImageViewMode::sourceAndDestsView;
    }

    ui->imageViewSelected->setHidden(modeFlag);
    ui->widget_2->setHidden(modeFlag);
    ui->widget->setHidden(modeFlag);
    ui->imageView->setMode(mode);
}

void MainWindow::on_actionInputFormatEdit_triggered()
{
    (new ParserSettingsDialog(this))->show();
}


void MainWindow::on_action_TraverseWalk_triggered()
{
   (new TraverseWalkSettings(this))->show();
}





