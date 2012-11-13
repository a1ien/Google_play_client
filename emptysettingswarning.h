#ifndef EMPTYSETTINGSWARNING_H
#define EMPTYSETTINGSWARNING_H

#include <QDialog>
#include "settings.h"

namespace Ui {
    class EmptySettingsWarning;
}

class EmptySettingsWarning : public QDialog {
Q_OBJECT

public:
    explicit EmptySettingsWarning(QWidget *parent = 0);
    ~EmptySettingsWarning();

private slots:
    void on_OK_clicked();

private:
    Ui::EmptySettingsWarning *ui;
    Settings* settings;
};

#endif // EMPTYSETTINGSWARNING_H
