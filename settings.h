//This file is part of Google Play Client.
//
//Google Play Client is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Foobar is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Google Play Client.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
    class Settings;
}

class Settings : public QDialog {
Q_OBJECT

public:
    explicit Settings(QWidget * parent = 0);
    ~Settings();
    QString email() {
        return settings->value("email").toString();
    }
    QString password() {
        return settings->value("password").toString();
    }
    QString androidID() {
        return settings->value("androidID").toString();
    }

    bool someIsEmpty() {
        return settings->value("email").toString().isEmpty()
            || settings->value("password").toString().isEmpty()
            || settings->value("androidID").toString().isEmpty();
    }

private slots:
    void on_Save_clicked();
    void on_Cancel_clicked();

private:
    Ui::Settings * ui;
    QSettings    * settings;
};

#endif // SETTINGS_H
