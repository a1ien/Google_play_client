#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      settings(new Settings(this)),
      session(new MarketSession()),
      downloader(new Downloader(this))
{
    ui->setupUi(this);
    connect(session, SIGNAL(MessageSignal(MessageTypes, QString &)), this, SLOT(messageSignalHandler(MessageTypes, QString &)));
    connect(this, SIGNAL(MessageSignal(MessageTypes, QString &)), this, SLOT(messageSignalHandler(MessageTypes, QString &)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_Download_clicked() {
    if (settings->someIsEmpty()) {
        // QMessageBox::information(this, tr("Your credentials are incompelete"),
        //         tr("\tNot all of your settings are specified.\n\n\tPlease, fill all settings fields before download."));
        emit MessageSignal(SettingsNotSet);
    }
    else {
        connect(session, SIGNAL(logged()), this, SLOT(onLogon()));
        session->login(settings->email(), settings->password(), settings->androidID(), QString("HOSTED_OR_GOOGLE"));
    }
}

void MainWindow::onLogon() {
    App app = session->getAppInfo(QString("pname:%1").arg(ui->SearchString->text().trimmed()));

    if (app.has_id()) {
        ui->AppInfo->clear();
        ui->AppInfo->append(QString("Title:\t%1").arg(app.title().c_str()));
        ui->AppInfo->append(QString("Version:\t%1").arg(app.version().c_str()));
        ui->AppInfo->append(QString("Rating:\t%1").arg(app.rating().c_str()));
     QString qtype;
     switch(app.apptype()) {
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

     ui->AppInfo->append(QString("Type:\t%1").arg(qtype));
     QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
             QString ("%1.%2.apk").arg(app.title().c_str()).arg(app.version().c_str()), tr("*.apk"));
     if (fileName.isEmpty()) {
         return;
     }
     downloader->DownloadFile(session->getInstallAsset(app.id().c_str()), fileName);
    }
}

void MainWindow::on_SearchString_textEdited(const QString & arg1) {
    ui->Download->setEnabled(!arg1.trimmed().isEmpty());
}


void MainWindow::on_SettingsButton_clicked() {
    settings->exec();
}

void MainWindow::messageSignalHandler(MessageTypes type, const QString description) {
    // QMessageBox::information(this, tr("Your credentials are incompelete"),
    //         tr("\tNot all of your settings are specified.\n\n\tPlease, fill all settings fields before download."));
    QString text   = "",
            header = "";
    bool appIsDead           = false,
         displayInMessageBox = false;
    switch (type) {
        case EmptyResponce:
            text = "Responce contains no data.";
            break;
        case NoApp:
            text = "Application not found.";
            break;
        case ResponceParsingFailed:
            text = "Responce has incorrect format and cannot be parsed";
            break;
        case SettingsNotSet:
            text = "\tNot all of your settings are specified."
                    "\n\n\tPlease, fill all settings fields before download.";
            header = "\tNot all of your settings are specified.\n\n\tPlease, fill all settings fields before download.";
            displayInMessageBox = true;
            break;
        case UnknownError:
            text = "Unknown error occured. Send this info to developers please:\n" + description;
            break;
        case Waiting:
            text = "Waiting for your response completion...";
            break;
        /*default:
            text = "The message handler has got a message that is not supported yet."
                    "\nPlease contact the developer team.";*/
    }
    if (displayInMessageBox) {
        QMessageBox::information(this, header, text);
    }
    else {
        ui->AppInfo->clear();
        ui->AppInfo->append("Message");
    }
    if (appIsDead) {
        QCoreApplication::exit(type);
    }
}
