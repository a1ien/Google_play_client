#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  settings(new Settings(this)),
  session(new MarketSession())
{

  ui->setupUi(this);
//  ui->SettingsButton->setIcon("toolbar-settings");
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_Download_clicked()
{
    if (settings->someIsEmpty())
        QMessageBox::information(this, tr("Your credentials are incompelete"), tr("\tNot all of your settings are specified.\n\tPlease, fill all settings fields before download."));
    else
    {
        connect(session,SIGNAL(logged()),this,SLOT(onLogon()));
        session->login(settings->email(),settings->password(),settings->androidid(),QString("HOSTED_OR_GOOGLE"));
    }
}
void MainWindow::onLogon()
{
  App app;
  app.CopyFrom(session->getAppInfo(QString ("pname:%1").arg(ui->SearchString->text().trimmed())));

  qDebug()<<app.DebugString().c_str();

 if (app.has_id())
  {
     ui->AppInfo->clear();
     ui->AppInfo->append(QString ("Title:\t%1").arg(app.title().c_str()));
     ui->AppInfo->append(QString ("Version:\t%1").arg(app.version().c_str()));
     ui->AppInfo->append(QString ("Rating:\t%1").arg(app.rating().c_str()));
     QString qtype;
     switch(app.apptype())
     {
        case 0:
         qtype = "None";
         break;
        case 1:
         qtype = "Application";
         break;
        case 2:
         qtype = "Ringtone";
         break;
        case 3:
         qtype = "Wallpaper";
         break;
        case 4:
         qtype = "Game";
         break;
     }
     ui->AppInfo->append(QString ("Type:\t%1").arg(qtype));
     QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), QString ("%1.%2.apk").arg(app.title().c_str()).arg(app.version().c_str()), tr("*.apk"));

  }

}

void MainWindow::on_SearchString_textEdited(const QString &arg1)
{
    if (!arg1.trimmed().isEmpty())
        ui->Download->setEnabled(true);
}

void MainWindow::on_Settings_clicked()
{
    settings->exec();
}
