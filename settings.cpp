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

#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget * parent)
    : QDialog(parent),
      ui(new Ui::Settings)
{
    ui->setupUi(this);
    settings = new QSettings("config.ini", QSettings::IniFormat, this);
    ui->email->setText(settings->value("email").toString());
    ui->password->setText(settings->value("password").toString());
    ui->androidID->setText(settings->value("androidID").toString());
}

Settings::~Settings() {
    delete ui;
}

void Settings::on_Save_clicked() {
    settings->setValue("email", ui->email->text());
    settings->setValue("password", ui->password->text());
    settings->setValue("androidID", ui->androidID->text());
    close();
}

void Settings::on_Cancel_clicked() {
    ui->email->setText(settings->value("email").toString());
    ui->password->setText(settings->value("password").toString());
    ui->androidID->setText(settings->value("androidID").toString());
    close();
}
