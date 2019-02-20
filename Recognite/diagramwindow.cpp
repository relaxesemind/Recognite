#include "diagramwindow.h"
#include "ui_diagramwindow.h"

DiagramWindow::DiagramWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiagramWindow)
{
    ui->setupUi(this);
    ui->menubar->setNativeMenuBar(false);
    mode = GrapherMode::BarVisible | GrapherMode::SplineVisible;
    drawGraph();
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
    drawGraph();
}

void DiagramWindow::drawGraph()
{
    Grapher::shared().clearView();
    pointsForGraph = Core::shared().calcPointsForGraph();
    float max = StaticModel::shared().absoluteMAXheight;
    float min = StaticModel::shared().absoluteMINheight;
    auto pair = StaticModel::shared().getMaxMinFrequencies();

    Grapher::shared().setXRange(min,max);
    Grapher::shared().setYRange(pair.second / pair.first, 1);
    Grapher::shared().addPointsAtGraph(pointsForGraph,mode);
}

void DiagramWindow::writeDataToStream(QTextStream& out)
{
    quint32 numberOfAreas = 0;
    QString numOfColumnsText = ui->lineEdit->text();
    int numOfColumns = numOfColumnsText.toInt();
    float max = StaticModel::shared().absoluteMAXheight;
    float min = StaticModel::shared().absoluteMINheight;
    auto& objectsMap = StaticModel::shared().objectsMap;

    std::for_each(objectsMap.begin(),objectsMap.end(),[&numberOfAreas](QVector<Area>& vector){
        numberOfAreas += vector.size();
    });

    out << QString("Всего объектов : ").toUtf8() << QString::number(numberOfAreas) << "\n";
    out << QString("Абсолютный максимум : ").toUtf8() << QString::number(max,'f',1) << QString("нм").toUtf8()
        << QString(" Абсолютный минимум : ").toUtf8() << QString::number(min,'f',1) << QString("нм").toUtf8() << "\n";
    out << QString("Количество карманов : ").toUtf8() << numOfColumnsText << QString(" Интервал между карманами : ").toUtf8()
        << QString::number((max - min) / (float)numOfColumns,'f',2) << QString("нм.").toUtf8() << "\n";

    for (int i = 0; i < pointsForGraph.count(); ++i)
    {
        out << QString("#").toUtf8() << QString::number(i) << QString(" Попадания : ").toUtf8() << QString::number(pointsForGraph[i].y()) << "\n";

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
    QString path = generateFilePath();
    qDebug() << path;
    if (path == QString("invalidInvalidInvalid"))
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
