#include "diagramwindow.h"
#include "ui_diagramwindow.h"
#include "Common/currentappstate.h"

DiagramWindow::DiagramWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiagramWindow)
{
    ui->setupUi(this);
    ui->menubar->setNativeMenuBar(false);
    mode = GrapherMode::SplineVisible;

    this->setWindowTitle("Графики");

    //min = 0.02 nm  max = 2 nm x 100

    ui->horizontalSlider->setMinimum(2);
    ui->horizontalSlider->setMaximum(250);
    ui->horizontalSlider->setValue(30);
    ui->lineEdit->setText("0.30");

    auto &series = StaticModel::shared().series;
    std::for_each(series.begin(),series.end(),[this](SeriaModel& seria)
    {
       barColors.insert(seria.getFolderPath(),gena.get());
       splineColors.insert(seria.getFolderPath(),gena.get());
    });

    maxX =  CurrentAppState::shared().maxFromUI;
    minX = CurrentAppState::shared().minFromUI;
    CurrentAppState::shared().maxXfromDiagramWindow = maxX;
    CurrentAppState::shared().minXfromDiagramWindow = minX;

    drawGraph();
    ui->gridLayout->addWidget(Grapher::shared().view,1,0);

    auto& state = CurrentAppState::shared();

    ui->horizontalSlider_3->setRange(static_cast<int>(state.minFromUI * 10),static_cast<int>(state.maxFromUI * 10));
    ui->horizontalSlider_3->setValue(static_cast<int>(state.maxFromUI * 10));

}

DiagramWindow::~DiagramWindow()
{
    delete ui;
}

void DiagramWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(static_cast<float>(value ) / 100,'f',2));
    recalc();
}

void DiagramWindow::on_lineEdit_editingFinished()
{
    QString text = ui->lineEdit->text();
    bool ok;
    float number = text.toFloat(&ok);
    if (!ok)
    {
        float fValue = static_cast<float>(ui->horizontalSlider->value()) / 10.f;
        ui->lineEdit->setText(QString::number(fValue,'f',1));
    }

    ui->horizontalSlider->setValue(static_cast<int>(number * 100));
    recalc();
}

void DiagramWindow::on_pushButton_clicked()//recalc
{
    recalc();
}

void DiagramWindow::drawGraph()
{
    Grapher::shared().clearView();
    pointsForGraph = Core::shared().calcPointsForGraph();
    auto pair = StaticModel::shared().getMaxMinFrequencies();

    auto& freq = StaticModel::shared().frequencies;

    int yMin = pair.second;
    int yMax = pair.first;
    int sum = 0;

    std::for_each(freq.begin(),freq.end(),[&sum](QVector<float> const& f)
    {
        sum += std::accumulate(f.begin(),f.end(),0);
    });

    Grapher::shared().setXRange(minX,maxX);
    Grapher::shared().setYRange(static_cast<float>(yMin * 100) / static_cast<float>(sum),
                                static_cast<float>(yMax * 100) / static_cast<float>(sum));
    Grapher::shared().updateState();

    for (auto it = pointsForGraph.begin(); it != pointsForGraph.end(); ++it)
    {
        int numberOfAreas = 0;
        auto& objectsMap = StaticModel::shared().objectsMap;
        SeriaModel seria(it.key());
        auto files = seria.getFiles();

        for (auto it = objectsMap.begin(); it != objectsMap.end(); ++it)
        {
            if (files.contains(it.key()))
            {
                numberOfAreas += it.value().size();
            }
        }

        QStringList legendTitle = it.key().split('/');
        QString title = legendTitle.last() + " N = " + QString::number(numberOfAreas);

        if (title.size() > 30)
        {
            title = title.left(30);
        }

        if (!legendTitle.isEmpty() and !it.value().isEmpty())
        {
            Grapher::shared().addGraph(it.value(),title,mode,barColors[it.key()],splineColors[it.key()]);
        }
    }
}

void DiagramWindow::recalc()
{
    int value = ui->horizontalSlider->value();
    float fValue = static_cast<float>(value) / 100;

    auto& series = StaticModel::shared().series;
    std::for_each(series.begin(),series.end(),[fValue](SeriaModel const& seria)
    {
        Core::shared().calculateFrequenciesWithInterval(seria.getFolderPath(),fValue);
    });

    drawGraph();
}

void DiagramWindow::writeDataToStream(QTextStream& out, SeriaModel const& seria)
{
    int numberOfAreas = 0;
    int value = ui->horizontalSlider->value();
    float fValue = static_cast<float>(value) / 100;
    float max = StaticModel::shared().absoluteMAXheight;
    float min = StaticModel::shared().absoluteMINheight;
    auto& freq = StaticModel::shared().frequenciesForExport;
    auto& objectsMap = StaticModel::shared().objectsMap;
    auto files = seria.getFiles();

    for (auto it = objectsMap.begin(); it != objectsMap.end(); ++it)
    {
        if (files.contains(it.key()))
        {
            numberOfAreas += it.value().size();
        }
    }

    float minUI = CurrentAppState::shared().minFromUI;
    float maxUI = CurrentAppState::shared().maxFromUI;

    QString lastComponent = seria.getSeriaLastComponent();
    out << utf8Str("Название серии : ") << lastComponent << "\n";
    out << utf8Str("Количество объектов : ") << QString::number(numberOfAreas) << "\n";
    out << utf8Str("Абсолютный максимум : ") << QString::number(max,'f',1) << utf8Str("нм")
        << utf8Str(" Абсолютный минимум : ") << QString::number(min,'f',1) << utf8Str("нм") << "\n";
    out << utf8Str("Выбранная нижняя граница : ") << QString::number(minUI,'f',1) << utf8Str("нм") <<
           utf8Str(" Выбранная верхняя граница : ") << QString::number(maxUI,'f',1) << utf8Str("нм") << "\n";
    out << utf8Str("Размер кармана : ") << QString::number(fValue,'f',2) << utf8Str("нм") << "\n";

    auto& points = freq[seria.getFolderPath()];

    for (int i = 0; i < points.count(); ++i)
    {
        float leftEdge = min + i*fValue;
        if (!(leftEdge < minUI - fValue || leftEdge > maxUI + fValue))
        {
            out << QString::number(points[i]) << "\n";
        }
    }

    out << "\n" << utf8Str("//////////////////Все значения////////////////////") << "\n \n";

    for (int i = 0; i < points.count(); ++i)
    {
        out << QString::number(points[i]) << "\n";
    }
}

QString generateFilePath(QString const& unique)
{
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/seriesExportTxt";
    bool ok;
    QString seriaName = QInputDialog::getText(nullptr, "Введите название серии","Название:", QLineEdit::Normal, "", &ok);

    if (!ok)
    {
        return QString("invalidInvalidInvalid");
    }

    QString fileName = "/" + seriaName + ".txt";

    if (!QDir(initialPath).exists())
    {
        QDir().mkdir(initialPath);
    }

    return initialPath + fileName;
}

void DiagramWindow::on_exportFileAction_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(this,"Экспорт серии", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if (directory.isEmpty())
    {
        return;
    }

    auto& series = StaticModel::shared().series;

    repeat(i,series.count())
    {
        SeriaModel seria = series.at(i);
        QString lastComponent = seria.getSeriaLastComponent();
        QString path = directory + "/" + lastComponent + ".txt";
        qDebug() << "export path = " << path;

        QFile file(path);
        if (file.open(QIODevice::ReadWrite | QFile::Text))
        {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            writeDataToStream(stream, seria);
        }
        file.close();
    }
}


void DiagramWindow::on_barDiagramChecker_triggered()
{
    mode ^= GrapherMode::BarVisible;
    drawGraph();
}

void DiagramWindow::on_splineChecker_triggered()
{
    mode ^= GrapherMode::SplineVisible;
    drawGraph();
}

void DiagramWindow::on_action_8_triggered()//random colors
{
    auto & series = StaticModel::shared().series;

    std::for_each(series.begin(),series.end(),[this](SeriaModel& seria)
    {
       barColors.insert(seria.getFolderPath(),gena.get());
       splineColors.insert(seria.getFolderPath(),gena.get());
    });
    drawGraph();
}

void DiagramWindow::on_horizontalSlider_3_valueChanged(int value)
{
    maxX = static_cast<float>(value) / 10.f;
    CurrentAppState::shared().maxXfromDiagramWindow = maxX;
    recalc();
}
