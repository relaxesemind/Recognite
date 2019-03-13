#include "diagramwindow.h"
#include "ui_diagramwindow.h"

DiagramWindow::DiagramWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiagramWindow)
{
    ui->setupUi(this);
    ui->menubar->setNativeMenuBar(false);
    mode = GrapherMode::BarVisible | GrapherMode::SplineVisible;

    //min = 0.02 nm  max = 2 nm x 100

    ui->horizontalSlider->setMinimum(2);
    ui->horizontalSlider->setMaximum(150);
    ui->horizontalSlider->setValue(30);
    ui->lineEdit->setText("0.30нм");
    drawGraph();
    ui->gridLayout->addWidget(Grapher::shared().view,1,0);
}

DiagramWindow::~DiagramWindow()
{
    delete ui;
}

void DiagramWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(static_cast<float>(value ) / 100,'f',2) + QString("нм").toUtf8());
}

void DiagramWindow::on_lineEdit_editingFinished()
{
    QString text = ui->lineEdit->text();
    emit numberOfColumnDidChange(text.toInt());
}

void DiagramWindow::on_pushButton_clicked()//recalc
{
//    QString text = ui->lineEdit->text();
    int value = ui->horizontalSlider->value();
    float fValue = static_cast<float>(value) / 100;
//    Core::shared().calculateFrequenciesWithInterval(CurrentAppState::shared().currentFolder, fValue);
    drawGraph();
}

void DiagramWindow::drawGraph()
{
//    Grapher::shared().clearView();
//    pointsForGraph = Core::shared().calcPointsForGraph();
//    float max = StaticModel::shared().absoluteMAXheight;
//    float min = StaticModel::shared().absoluteMINheight;
//    auto pair = StaticModel::shared().getMaxMinFrequencies();
//    int sum = StaticModel::shared().getAccumulateFreq();

//    int yMin = pair.second;
//    int yMax = pair.first;

//    qDebug() << "sum = " << sum;
//    qDebug() << "ymin = " << yMin << " yMax = " << yMax;

//    Grapher::shared().setXRange(min,max);
//    Grapher::shared().setYRange(static_cast<float>(yMin)/static_cast<float>(sum),
//                                static_cast<float>(yMax)/static_cast<float>(sum));
//    Grapher::shared().addPointsAtGraph(pointsForGraph,mode);
}

void DiagramWindow::writeDataToStream(QTextStream& out)
{
    quint32 numberOfAreas = 0;
    int value = ui->horizontalSlider->value();
    float fValue = static_cast<float>(value) / 100;
    float max = StaticModel::shared().absoluteMAXheight;
    float min = StaticModel::shared().absoluteMINheight;
    auto& objectsMap = StaticModel::shared().objectsMap;

    std::for_each(objectsMap.begin(),objectsMap.end(),[&numberOfAreas](QVector<Area>& vector){
        numberOfAreas += vector.size();
    });

    out << QString("Всего объектов : ").toUtf8() << QString::number(numberOfAreas) << "\n";
    out << QString("Абсолютный максимум : ").toUtf8() << QString::number(max,'f',1) << QString("нм").toUtf8()
        << QString(" Абсолютный минимум : ").toUtf8() << QString::number(min,'f',1) << QString("нм").toUtf8() << "\n";
//    out << QString("Количество карманов : ").toUtf8() << numOfColumnsText << QString(" Интервал между карманами : ").toUtf8()
//        << QString::number((max - min) / (float)numOfColumns,'f',2) << QString("нм.").toUtf8() << "\n";
    out << QString("Размер кармана : ").toUtf8() << QString::number(fValue,'f',2) << QString("нм").toUtf8() << "\n";

    for (int i = 0; i < pointsForGraph.count(); ++i)
    {
        out << QString::number(min + i*fValue,'f',2) << " " << QString::number(pointsForGraph[i].y()) << "\n";

    }
}

QString generateFilePath()
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
    QString defaultFilter("Текстовые файлы (*.txt)");
    QString path = QFileDialog::getSaveFileName(this,"Экспорт Диаграммы",QDir::currentPath(),
                                                "Текстовые файлы (*.txt);;Все файлы (*.*)",&defaultFilter);
    qDebug() << path;
    if (path.isEmpty())
    {
        AppMessage("Ошибка","Некорректное название серии");
        return;
    }

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        writeDataToStream(stream);
    }

    file.close();
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
