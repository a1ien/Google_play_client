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
      ui(new Ui::Settings),
      session(MarketSession::getInstance(this))
{
    QObject::connect(this, SIGNAL(NeedToRelogin()), session, SLOT(needToReloginHandler()));
    ui->setupUi(this);
    settings = new QSettings("config.ini", QSettings::IniFormat, this);
    ui->email->setText(settings->value("email").toString());
    ui->password->setText(settings->value("password").toString());
    ui->androidID->setText(settings->value("androidID").toString());
    ui->country->setText(settings->value("country").toString());
    ui->language->setText(settings->value("language").toString());
    ui->operatorSym->setText(settings->value("operator").toString());
    ui->operatorNum->setText(settings->value("operatorNum").toString());

    session->setOperatorAlpha(settings->value("operator").toString());
    session->setOperatorNumeric(settings->value("operatorNum").toString());
    session->setContry(settings->value("country").toString(),settings->value("language").toString());
    emit NeedToRelogin();
}

Settings::~Settings() {
    delete ui;
}

void Settings::on_Save_clicked()
{
    settings->setValue("email", ui->email->text());
    settings->setValue("password", ui->password->text());
    settings->setValue("androidID", ui->androidID->text());
    settings->setValue("country",ui->country->text());
    settings->setValue("language",ui->language->text());
    settings->setValue("operator",ui->operatorSym->text());
    settings->setValue("operatorNum",ui->operatorNum->text());

    session->setOperatorAlpha(settings->value("operator").toString());
    session->setOperatorNumeric(settings->value("operatorNum").toString());
    session->setContry(settings->value("country").toString(),settings->value("language").toString());
    close();
    emit NeedToRelogin();
    session->login(ui->email->text(), ui->password->text(), ui->androidID->text(), QString("HOSTED_OR_GOOGLE"));
}

void Settings::on_Cancel_clicked() {
    ui->email->setText(settings->value("email").toString());
    ui->password->setText(settings->value("password").toString());
    ui->androidID->setText(settings->value("androidID").toString());
    ui->country->setText(settings->value("country").toString());
    ui->language->setText(settings->value("language").toString());
    ui->operatorSym->setText(settings->value("operator").toString());
    ui->operatorNum->setText(settings->value("operatorNum").toString());
    close();
}
