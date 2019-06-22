#include "parsersettingsdialog.h"
#include "ui_parsersettingsdialog.h"

ParserSettingsDialog::ParserSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParserSettingsDialog)
{
    ui->setupUi(this);
    bool commas = SettingsStorage::shared().parseMode == ParseMode::commasMode;
    bool mode1024 = SettingsStorage::shared().parseMode == ParseMode::mode1024;
    if (commas)
    {
        ui->checkBox->setCheckState(Qt::CheckState::Checked);
        ui->checkBox_2->setCheckState(Qt::CheckState::Unchecked);
    }
    else
    {
        ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
    }

    if (mode1024)
    {
        ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
        ui->checkBox_2->setCheckState(Qt::CheckState::Checked);
    }
    else
    {
        ui->checkBox_2->setCheckState(Qt::CheckState::Unchecked);
    }

    ui->lineEdit->setEnabled(!(commas or mode1024));
    ui->lineEdit_2->setEnabled(!(commas or mode1024));
}

ParserSettingsDialog::~ParserSettingsDialog()
{
    delete ui;
}

void ParserSettingsDialog::on_buttonBox_accepted()
{
    QString lineText = ui->lineEdit->text();
    QString numberText = ui->lineEdit_2->text();
    auto& settings = SettingsStorage::shared();

    if (lineText.isEmpty() or numberText.isEmpty())
    {
        return;
    }

    QChar lineSeparator = lineText.at(0);;
    QChar numberSeparator = numberText.at(0);

    settings.lineSeparator = lineSeparator;
    settings.numberSeparator = numberSeparator;
    settings.parseMode = ParseMode::defaultMode;

    if (ui->checkBox->isChecked())
    {
        settings.parseMode = ParseMode::commasMode;
    }
    if (ui->checkBox_2->isChecked())
    {
        settings.parseMode = ParseMode::mode1024;
    }
}

void ParserSettingsDialog::on_checkBox_clicked(bool checked)
{
   ui->lineEdit->setEnabled(!checked);
   ui->lineEdit_2->setEnabled(!checked);

   if (ui->checkBox_2->isChecked() and checked)
   {
        ui->checkBox_2->setCheckState(Qt::CheckState::Unchecked);
   }
}

void ParserSettingsDialog::on_checkBox_2_clicked(bool checked)
{
    ui->lineEdit->setEnabled(!checked);
    ui->lineEdit_2->setEnabled(!checked);
    if (ui->checkBox->isChecked() and checked)
    {
         ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
    }
}
