#ifndef PARSERSETTINGSDIALOG_H
#define PARSERSETTINGSDIALOG_H

#include <QDialog>
#include "staticmodel.h"


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

private:
    Ui::ParserSettingsDialog *ui;
};

#endif // PARSERSETTINGSDIALOG_H
