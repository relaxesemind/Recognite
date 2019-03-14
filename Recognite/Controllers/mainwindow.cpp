#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pool(new QThreadPool(this)),
    selectingTask(nullptr)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);

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
    ui->progressBar->setValue(value == 100 ? 0 : value);
}

void MainWindow::enableDiagramButton(bool flag)
{
    ui->diagramPushButton->setEnabled(flag);
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

    //mock
    listWidget->addItems(QStringList{
                             "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)",
                             "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(2)"
                         });
    SeriaModel s1("/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)"),
               s2("/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(2)");
    StaticModel::shared().series.append({s1,s2});
    CurrentAppState::shared().currentSeria = s1;
}

void MainWindow::setupImageView()
{
    connect(ui->imageView,&ImageView::showHeightToolTip,this,[](const QPoint& coord, const QPoint& globalPos)
    {

        auto& models = StaticModel::shared().inputModels;
        QString path = CurrentAppState::shared().currentFilePath;

        for (InputModel& model : models)
        {
            if (model.path == path and model.isSafelyIndexes(coord.x(),coord.y()))
            {
                QToolTip::showText(globalPos,QString::number(model.matrix[coord.y()][coord.x()],'f',2));
                break;
            }
        }
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

    qDebug() << path;

    SeriaModel seria(path);
    CurrentAppState::shared().currentSeria = seria;
    StaticModel::shared().series.append(seria);
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
           QDir folder(path);

           if (!folder.exists())
           {
               continue;
           }

           SeriaModel seria(path);
           QVector<QString> filePaths = seria.getFiles();

           std::for_each(filePaths.begin(),filePaths.end(),[this](const QString& path)
           {
               this->makeImageFromFilePath(path);
           });
    }

    if (listWidget->count())
    {
        CurrentAppState::shared().currentSeria = SeriaModel(listWidget->item(0)->text());
    }
    else
    {
        return;
    }

    // inputModels and sources is ready

    if (!sources.isEmpty())
    {
        auto firstIt = sources.begin();
        ui->imageView->setImage(QPixmap::fromImage(firstIt.value()));
        CurrentAppState::shared().currentFilePath = firstIt.key();
    }
    else
    {
        return;
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
    SeriaModel seria = CurrentAppState::shared().currentSeria;
    auto& sources = StaticModel::shared().sources;

    QVector<QString> files = seria.getFiles();

    table->clear();
    table->setColumnCount(files.count());
    table->setRowCount(1);
    const int imgSize = Consts::previewSourceImageHeight;

    for (int i = 0; i < files.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        table->setRowHeight(i,imgSize + 5);
        table->setColumnWidth(i,imgSize + 5);
        auto it = sources.find(files[i]);

        if (it == sources.end())
        {
            continue;
        }

        item->setData(Qt::DecorationRole,QPixmap::fromImage(it.value()).scaled(imgSize,imgSize,Qt::KeepAspectRatio));
        table->setItem(0,i,item);
    }
}

void MainWindow::updateViewWithSeria()
{
    updateTableWidget();
    updateImageViews();
}

void MainWindow::updateImageViews()
{
    QImage sourceImage = StaticModel::shared().getCurrentImage();
    QImage binImage = StaticModel::shared().getCurrentBinImage();

    switch (CurrentAppState::shared().imageViewMode) {
    case ImageViewMode::sourceAndDestsView:
           ui->imageView->setImage(QPixmap::fromImage(sourceImage));
           ui->imageViewSelected->setImage(QPixmap::fromImage(binImage));
           ui->imageViewSelected->setHidden(false);
           ui->widget_2->setHidden(false);
           ui->imageView->showSlider(false);
        break;
    case ImageViewMode::transparentOver:
           ui->imageView->setImage(QPixmap::fromImage(sourceImage));
           ui->imageView->setBinaryImage(QPixmap::fromImage(binImage));
           ui->imageView->showSlider(true);
           ui->widget_2->setHidden(true);
           ui->imageViewSelected->setHidden(true);
        break;
    default:
        break;
    }
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int id = index.column();
    auto files = CurrentAppState::shared().currentSeria.getFiles();
    if (id >= 0 and id < files.count())
    {
        CurrentAppState::shared().currentFilePath = files[id];
        updateImageViews();
    }
}

void MainWindow::on_processPushButton_clicked()
{
    auto& sources = StaticModel::shared().sources;
    auto& dests = StaticModel::shared().dests;
    bool isRunning = CurrentAppState::shared().selectingTaskIsRunning;

    if (sources.isEmpty())
    {
        AppMessage("Ошибка","Изображения отсутствуют");
        return;
    }

    if (isRunning)
    {
        qDebug () << "task is running";
        return;
    }

    int min = ui->minHeightSlider->value();
    int max = ui->maxHeightSlider->value();
    int minObjSize = ui->minObjSizeLineEdit->text().toInt();
    Core::shared().setRange(static_cast<float>(min) / 10.f,static_cast<float>(max) / 10.f);
    Core::shared().setMinObjectSize(minObjSize);

    QVector<QString> files;
    auto& series = StaticModel::shared().series;

    std::for_each(series.begin(),series.end(),[&files](SeriaModel const& seria)
    {
        files.append(seria.getFiles());
    });

    QStringList paths(files.toList());

    if (paths.isEmpty())
    {
        return;
    }

    dests.clear();
    selectingTask = new SelectingProcessManager(paths);

    connect(selectingTask,&SelectingProcessManager::destPair,&StaticModel::shared(),&StaticModel::addDestPair);
    connect(selectingTask,&SelectingProcessManager::setEnableDiagram,this,&MainWindow::enableDiagramButton);
    connect(selectingTask,&SelectingProcessManager::processPercent,this,&MainWindow::updateProcessPercentage);
    connect(selectingTask,&SelectingProcessManager::isDone,this,&MainWindow::selectingTaskIsFinished);
    connect(selectingTask,&SelectingProcessManager::isRunning,this,[](bool isRunning){
        CurrentAppState::shared().selectingTaskIsRunning = isRunning;
    });

    pool->start(selectingTask);
}


void MainWindow::on_diagramPushButton_clicked()
{
    Grapher::shared().clearView();
    auto& series = StaticModel::shared().series;

    std::for_each(series.begin(),series.end(),[](SeriaModel const& seria)
    {
        Core::shared().calculateFrequenciesWithInterval(seria.getFolderPath(),Consts::defaultFrequencyInterval);
    });

    (new DiagramWindow(this))->show();
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

        listWidget->addItem(path);
        SeriaModel seria(path);
        CurrentAppState::shared().currentSeria = seria;
        StaticModel::shared().series.append(seria);
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

    menu.addActions(QList<QAction *>{removeAction, addItemAction,removeAll});
    menu.exec(globalPos);
}

void MainWindow::on_changeShowMode_triggered()
{
   ImageViewMode& mode = CurrentAppState::shared().imageViewMode;
   if (mode == ImageViewMode::transparentOver)
   {
       mode = ImageViewMode::sourceAndDestsView;
   }
   else if (mode == ImageViewMode::sourceAndDestsView)
   {
       mode = ImageViewMode::transparentOver;
   }
   updateViewWithSeria();
}

void MainWindow::on_actionInputFormatEdit_triggered()
{
    (new ParserSettingsDialog(this))->show();
}


void MainWindow::on_action_TraverseWalk_triggered()
{
   (new TraverseWalkSettings(this))->show();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString folderPath = item->text();
    if (!folderPath.isEmpty())
    {
        SeriaModel seria(folderPath);
        CurrentAppState::shared().currentSeria = seria;
        auto files = seria.getFiles();

        if (!files.isEmpty())
        {
            CurrentAppState::shared().currentFilePath = files.first();
            updateViewWithSeria();
        }
    }
}

void MainWindow::selectingTaskIsFinished()
{
    updateViewWithSeria();
}





