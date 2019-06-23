#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pool(new QThreadPool(this))
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);
    folderPath = "/home";

    setupListWidget();
    setupImageView();
    setupProgressBar();
    ui->tableWidget->setStyleSheet(QString( "background: #ECEFFE;" ));

//    connect(&Core::shared(),&Core::imageBuildProgress,this,[this](int value)
//    {
//        this->ui->progressBar->setValue(value > 93 ? 0 : value);
//    });
}

MainWindow::~MainWindow()
{
    delete pool;
    delete ui;
}

void MainWindow::updateProcessPercentage(int value)
{
    ui->progressBar->setValue(value >= 100 ? 0 : value);
}

void MainWindow::enableDiagramButton(bool flag)
{
    ui->diagramPushButton->setEnabled(flag);
}

void MainWindow::setupListWidget()
{
    QListWidget *listWidget = ui->listWidget;

    listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->imageView->addGradientAxis(0,0);
    connect(listWidget, &QListWidget::customContextMenuRequested,this, &MainWindow::showListMenuAtPos);

    //mock windows
//    listWidget->addItems({
//                             "C:/dev/selection_new/txt/seria-300119/seria-300119-sample(1)",
//                             "C:/dev/selection_new/txt/seria-300119/seria-300119-sample(2)"
//                         });
//    SeriaModel s1("C:/dev/selection_new/txt/seria-300119/seria-300119-sample(1)"),
//               s2("C:/dev/selection_new/txt/seria-300119/seria-300119-sample(2)");
//    StaticModel::shared().series.append({s1,s2});
//    CurrentAppState::shared().currentSeria = s1;

    //mock macOS
//    listWidget->addItems({
//                             "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)",
//                             "/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(2)"
//                         });
//    SeriaModel s1("/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(1)"),
//               s2("/Users/ivanovegor/Documents/dev/recognite/Recognite/txt/seria-300119/seria-300119-sample(2)");
//    StaticModel::shared().series.append({s1,s2});
//    CurrentAppState::shared().currentSeria = s1;
}

void MainWindow::setupImageView()
{
    ui->imageView->setStyleSheet(QString(
                      "background-color: #d6dbff;"
                      ));
    ui->imageViewSelected->setStyleSheet(QString(
                      "background-color: #d6dbff;"
                      ));
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

void MainWindow::setupProgressBar()
{
    ui->progressBar->setStyleSheet(QString(
                           "QProgressBar:horizontal {"
//                           "border: 1px solid gray;"
                           "border-radius: 3px;"
                           "background: white;"
                           "padding: 1px;"
                           "}"
                           "QProgressBar::chunk:horizontal {"
                           "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 #FF893D, stop: 1 white);"
                           "}"
    ));
}

void MainWindow::addSeria()
{   
    QString path = QFileDialog::getExistingDirectory(this, tr("Выберите папку с серией изображений"),
                                                 folderPath,
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
    folderPath = seria.getFolderPath();
}

void MainWindow::addSeries()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::DontUseNativeDialog);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setDirectory(folderPath);


    QListView *lView = dialog.findChild<QListView *>("listView");
    if (lView)
    {
        lView->setSelectionMode(QAbstractItemView::MultiSelection);
    }

    QTreeView *tView = dialog.findChild<QTreeView *>();
    if (tView)
    {
        tView->setSelectionMode(QAbstractItemView::MultiSelection);
    }

    dialog.exec();

    auto dirs = dialog.selectedFiles();

    if (dirs.count() == 0)
    {
        return;
    }

    for (QString& dir : dirs)
    {
        if (dir.isEmpty())
        {
            continue;
        }

        qDebug() << "dir : " << dir;

        SeriaModel seria(dir);
        CurrentAppState::shared().currentSeria = seria;
        StaticModel::shared().series.append(seria);
        folderPath = seria.getFolderPath();
        ui->listWidget->addItem(dir);
    }
}

void MainWindow::buildImages()
{
    auto& model = StaticModel::shared();
    auto& sources = model.sources;
    auto& series = model.series;
    model.sources.clear();
    model.dests.clear();
    model.inputModels.clear();

    ImagesBuiderProcess *process = new ImagesBuiderProcess(series);

    connect(process,&ImagesBuiderProcess::isRunning,this,[this](bool currentlyRunning)
    {
        ui->processPushButton->setEnabled(!currentlyRunning);
        if (currentlyRunning)
        {
            imageParseSpinner = new WaitingSpinnerWidget(ui->imageView, Qt::ApplicationModal, true);
            imageParseSpinner->setRoundness(70.0);
            imageParseSpinner->setMinimumTrailOpacity(15.0);
            imageParseSpinner->setTrailFadePercentage(70.0);
            imageParseSpinner->setNumberOfLines(12);
            imageParseSpinner->setLineLength(12);
            imageParseSpinner->setLineWidth(4);
            imageParseSpinner->setInnerRadius(10);
            imageParseSpinner->setRevolutionsPerSecond(1);
            imageParseSpinner->start();
        }
        else if (imageParseSpinner != nullptr)
        {
            imageParseSpinner->stop();

            delete imageParseSpinner;
            imageParseSpinner = nullptr;
        }
    });

    connect(process,&ImagesBuiderProcess::isDone,this,[this, &sources](bool status)
    {
        if (!status)
        {
            return;
        }

        qDebug() << "inputModels size = " << StaticModel::shared().inputModels.size();
        qDebug() << "sources size = " << StaticModel::shared().sources.size();

        if (!sources.isEmpty())
        {
            auto firstIt = sources.begin();
            CurrentAppState::shared().currentFilePath = firstIt.key();
            ui->imageView->setImage(QPixmap::fromImage(firstIt.value()));
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


    });

    connect(process,&ImagesBuiderProcess::processPercent,this,[this](int value)
    {
        this->ui->progressBar->setValue(value >= 100 ? 0 : value);
    });

    this->pool->start(process);
}

void MainWindow::animateProgressBarFlash()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->label, "color");
    animation->setDuration(1000);
    animation->setStartValue(QColor(0, 0, 0));
    animation->setEndValue(QColor(240, 240, 240));
    animation->start();
}

void MainWindow::on_loadTxtFiles_triggered()
{
    addSeria();
}

void MainWindow::on_maxHeightSlider_valueChanged(int value)
{
    float fValue = static_cast<float>(value) / 10.f;
    CurrentAppState::shared().maxFromUI = fValue;

    ui->maxHeightLineEdit->setText(QString::number(fValue,'f',1));

    if (value < ui->minHeightSlider->value())
    {
        ui->maxHeightSlider->setValue(ui->minHeightSlider->value());
    }
}

void MainWindow::on_minHeightSlider_valueChanged(int value)
{
    float fValue = static_cast<float>(value) / 10.f;
    CurrentAppState::shared().minFromUI = fValue;

    ui->minHeightLineEdit->setText(QString::number(fValue,'f',1));

    if (value > ui->maxHeightSlider->value())
    {
        ui->minHeightSlider->setValue(ui->maxHeightSlider->value());
    }
}

void MainWindow::on_pushButton_clicked()//build images
{   
    buildImages();
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

        qDebug() << CurrentAppState::shared().currentFilePath;
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
    SelectingProcessManager *selectingTask = new SelectingProcessManager(std::move(paths));   

    connect(selectingTask,&SelectingProcessManager::destPair,&StaticModel::shared(),&StaticModel::addDestPair);
    connect(selectingTask,&SelectingProcessManager::setEnableDiagram,this,&MainWindow::enableDiagramButton);
    connect(selectingTask,&SelectingProcessManager::processPercent,this,&MainWindow::updateProcessPercentage);
    connect(selectingTask,&SelectingProcessManager::isDone,this,&MainWindow::selectingTaskIsFinished);
    connect(selectingTask,&SelectingProcessManager::isRunning,this,[this](bool isRunning)
    {
        CurrentAppState::shared().selectingTaskIsRunning = isRunning;
        this->ui->processPushButton->setEnabled(!isRunning);
        if (isRunning)
        {
            selectionSpinner = new WaitingSpinnerWidget(ui->imageViewSelected, Qt::ApplicationModal, true);
            selectionSpinner->setRoundness(70.0);
            selectionSpinner->setMinimumTrailOpacity(15.0);
            selectionSpinner->setTrailFadePercentage(70.0);
            selectionSpinner->setNumberOfLines(12);
            selectionSpinner->setLineLength(12);
            selectionSpinner->setLineWidth(4);
            selectionSpinner->setInnerRadius(10);
            selectionSpinner->setRevolutionsPerSecond(1);
            selectionSpinner->setColor(Qt::white);
            selectionSpinner->start();
        }
        else if (selectionSpinner != nullptr)
        {
            selectionSpinner->stop();

            delete selectionSpinner;
            selectionSpinner = nullptr;
        }
    });

    _selTask = selectingTask;
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

    QAction *addItemAction = new QAction(QString("Добавить серию"),this);

    connect(addItemAction,&QAction::triggered,this,[&listWidget,this]
    {
        this->addSeria();
    });

    QAction *addItemsAction = new QAction(QString("Добавить несколько серий"),this);

    connect(addItemsAction,&QAction::triggered,this,[&listWidget,this]
    {
        this->addSeries();
    });

    QAction *removeAction = new QAction(QString("Удалить серию"),this);
    removeAction->setShortcut(QKeySequence::Delete);

    connect(removeAction,&QAction::triggered,this,[&listWidget, this]
    {
        auto& series = StaticModel::shared().series;

        qDebug() << "до - " << series.count();

        for (int i = 0; i < listWidget->selectedItems().size(); ++i)
        {
            QListWidgetItem *item = listWidget->takeItem(listWidget->currentRow());
            auto it = std::find_if(series.begin(),series.end(),[&item](SeriaModel& seria){return seria.getFolderPath() == item->text();});
            if (it != series.end())
            {
                series.erase(it);
            }
            delete item;
        }
        if (series.count())
        {
           CurrentAppState::shared().currentSeria = series.first();
        }
        StaticModel::shared().sources.clear();
        StaticModel::shared().dests.clear();
        StaticModel::shared().frequencies.clear();
        StaticModel::shared().objectsMap.clear();
        this->buildImages();
        qDebug() << "После - " << series.count();
    });

    QPushButton *diagramButton = ui->diagramPushButton;
    QPushButton *processButton = ui->processPushButton;

    QAction *removeAll = new QAction(QString("Удалить все"),this);
    connect(removeAll, &QAction::triggered, this, [&listWidget, &tableWidget, &imageView, &destView, &diagramButton, &processButton]
    {
        listWidget->clear();
        tableWidget->clear();
        tableWidget->setColumnCount(0);
        imageView->clearView();
        destView->clearView();
        StaticModel::shared().dropModel();
        diagramButton->setEnabled(false);
        processButton->setEnabled(false);
    });

    menu.addActions({removeAction, addItemAction, addItemsAction,removeAll});
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
    _selTask = nullptr;
}

void MainWindow::on_maxHeightLineEdit_editingFinished()
{
    bool ok;
    float number = ui->maxHeightLineEdit->text().toFloat(&ok);
    if (!ok)
    {
        float fValue = static_cast<float>(ui->maxHeightSlider->value()) / 10.f;
        ui->maxHeightLineEdit->setText(QString::number(fValue,'f',1));
        CurrentAppState::shared().maxFromUI = fValue;
    }

    ui->maxHeightSlider->setValue(static_cast<int>(number * 10));
}

void MainWindow::on_minHeightLineEdit_editingFinished()
{
    bool ok;
    float number = ui->minHeightLineEdit->text().toFloat(&ok);
    if (!ok)
    {
        float fValue = static_cast<float>(ui->minHeightSlider->value()) / 10.f;
        ui->minHeightLineEdit->setText(QString::number(fValue,'f',1));
        CurrentAppState::shared().minFromUI = fValue;
    }

    ui->minHeightSlider->setValue(static_cast<int>(number * 10));
}

void MainWindow::on_action_2_triggered()//load several series
{
    this->addSeries();
}

void MainWindow::on_action_3_triggered()//save images
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Выберите папку с серией изображений"),
                                                 folderPath,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
    {
        return;
    }

    qDebug() << "path = " << path;

    auto& sources = StaticModel::shared().sources;
    int i = 0;
    for (auto it = sources.begin(); it != sources.end(); ++it)
    {
        QImage image = it.value();
        if (!image.save(path + "/" + QString::number(++i) + ".jpg","JPG"))
        {
            qDebug() << "failed";
        }
    }
}

void MainWindow::on_processCancelButton_clicked()
{
    if (_selTask != nullptr)
    {
        _selTask->flag_stop = true;
    }
}
