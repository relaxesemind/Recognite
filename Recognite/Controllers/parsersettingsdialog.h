#ifndef PARSERSETTINGSDIALOG_H
#define PARSERSETTINGSDIALOG_H

#include <QDialog>
#include "Common/settingsstorage.h"
#include <QDebug>


namespace Ui {
class ParserSettingsDialog;
}

class ParserSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParserSettingsDialog(QWidget *parent = 0);
    ~ParserSettingsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_checkBox_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

private:
    Ui::ParserSettingsDialog *ui;
};

#endif // PARSERSETTINGSDIALOG_H
