#ifndef TRAVERSEWALKSETTINGS_H
#define TRAVERSEWALKSETTINGS_H

#include <QDialog>
#include "staticmodel.h"
#include <iostream>

namespace Ui {
class TraverseWalkSettings;
}

class TraverseWalkSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TraverseWalkSettings(QWidget *parent = 0);
    ~TraverseWalkSettings();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::TraverseWalkSettings *ui;
};

#endif // TRAVERSEWALKSETTINGS_H
