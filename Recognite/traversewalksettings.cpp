#include "traversewalksettings.h"
#include "ui_traversewalksettings.h"

TraverseWalkSettings::TraverseWalkSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TraverseWalkSettings)
{
    ui->setupUi(this);
    ui->lineEdit_2->setText(QString::number(StaticModel::shared().traverseWalkStep));
    ui->lineEdit->setText(QString::number(StaticModel::shared().derivativeStability,'f',2));
}

TraverseWalkSettings::~TraverseWalkSettings()
{
    delete ui;
}

void TraverseWalkSettings::on_buttonBox_accepted()
{
    QString deriv = ui->lineEdit->text();
    QString walkSize = ui->lineEdit_2->text();

    if (deriv.isEmpty() or walkSize.isEmpty())
    {
        return;
    }

    StaticModel::shared().traverseWalkStep = walkSize.toInt();
    StaticModel::shared().derivativeStability = deriv.toFloat();

}
