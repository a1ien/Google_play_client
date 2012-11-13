#include "emptysettingswarning.h"
#include "ui_emptysettingswarning.h"

EmptySettingsWarning::EmptySettingsWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmptySettingsWarning),
    settings(new Settings())
{
    ui->setupUi(this);
}

EmptySettingsWarning::~EmptySettingsWarning()
{
    delete ui;
}

void EmptySettingsWarning::on_OK_clicked()
{
    close();
}
