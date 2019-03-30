#include "parsersettingsdialog.h"
#include "ui_parsersettingsdialog.h"

ParserSettingsDialog::ParserSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParserSettingsDialog)
{
    ui->setupUi(this);
    bool flag = SettingsStorage::shared().commasParse;
    if (flag)
    {
        ui->checkBox->setCheckState(Qt::CheckState::Checked);
    }
    else
    {
        ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
    }
    ui->lineEdit->setEnabled(!flag);
    ui->lineEdit_2->setEnabled(!flag);
}

ParserSettingsDialog::~ParserSettingsDialog()
{
    delete ui;
}

void ParserSettingsDialog::on_buttonBox_accepted()
{
    QString lineText = ui->lineEdit->text();
    QString numberText = ui->lineEdit_2->text();

    if (lineText.isEmpty() or numberText.isEmpty())
    {
        return;
    }

    QChar lineSeparator = lineText.at(0);;
    QChar numberSeparator = numberText.at(0);

    SettingsStorage::shared().lineSeparator = lineSeparator;
    SettingsStorage::shared().numberSeparator = numberSeparator;
}

void ParserSettingsDialog::on_checkBox_clicked(bool checked)
{
   ui->lineEdit->setEnabled(!checked);
   ui->lineEdit_2->setEnabled(!checked);
   SettingsStorage::shared().commasParse = checked;
}
