#include "parsersettingsdialog.h"
#include "ui_parsersettingsdialog.h"

ParserSettingsDialog::ParserSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParserSettingsDialog)
{
    ui->setupUi(this);
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
