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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      settings(new Settings(this)),
      session(MarketSession::getInstance(this)),
      downloader(new Downloader(this)),
      suggest(new QTableWidget(this)),
      timer(new QTimer(this))
{
    ui->setupUi(this);
    connect(session, SIGNAL(MessageSignal(MessageTypes, QString)), this, SLOT(messageSignalHandler(MessageTypes, QString)));
    connect(this, SIGNAL(MessageSignal(MessageTypes, QString)), this, SLOT(messageSignalHandler(MessageTypes, QString)));
    connect(session, SIGNAL(GetAppSignal()), this, SLOT(getAppSignalHandler()));
    setupSuggest();
    timer->setSingleShot(true);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), SLOT(autoSuggest()));
    connect(ui->SearchString, SIGNAL(textEdited(QString)), timer, SLOT(start()));
    connect(session,SIGNAL(SearcheComplite(AppsResponse)),SLOT(AppsResponseHeandle(AppsResponse)));

    session->login(settings->email(), settings->password(), settings->androidID(), QString("HOSTED_OR_GOOGLE"));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_Download_clicked() {
    //check if data necessary for login is specified
    if (settings->someIsEmpty()) {
        emit MessageSignal(SettingsNotSet);
    }
    else {
        //Log in Auth server
        session->login(settings->email(), settings->password(), settings->androidID(), QString("HOSTED_OR_GOOGLE"));
        session->getApp();
    }
}

void MainWindow::getAppSignalHandler() {
    qDebug() << "\nCALL: MainWindow::getAppSignalHandler()";
    //Get application info
    App app = session->getAppInfo(ui->SearchString->text().trimmed());
    //Parse app info and show it in AppInfo field
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


        if(app.has_extendedinfo())
        {
            if(app.extendedinfo().has_description())
                ui->AppInfo->append(QString("Description:\t%1").arg(app.extendedinfo().description().c_str()));
        }

        //Preset current directory in settings
        if(settings->getSettings().value("currentDir").toString().isEmpty())
            settings->getSettings().setValue("currentDir", QDir ::currentPath());
        //Open a dialogue to save downloaded package
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        QString ("%3/%1.%2.apk").arg(app.title().c_str()).arg(app.version().c_str()).arg(settings->getSettings().value("currentDir").toString()),
                                                        tr("*.apk"));
        //Save destination directory in settings
        if(!fileName.isEmpty())
            settings->getSettings().setValue("currentDir", QFileInfo (fileName).absolutePath());
        else
            return;
        //Download a package
        downloader->DownloadFile(session->getInstallAsset(app.id().c_str()), fileName);
    }
}

void MainWindow::on_SearchString_textEdited(const QString & arg1) {
    //Enable "Download" button only if Search field isn't empty
    ui->Download->setEnabled(!arg1.trimmed().isEmpty());
}


void MainWindow::on_SettingsButton_clicked() {
    //Open Settings window
    settings->exec();
}

void MainWindow::messageSignalHandler(MessageTypes type, const QString description) {
    //Show errors in AppInfo field & messages
    qDebug() << "\nCALL: MainWindow::messageSignalHandler()";
    QString text   = "",
            header = "";
    bool appIsDead           = false,
            displayInMessageBox = false;
    switch (type) {
    case EmptyResponce:
        text = "Response contains no data.";
        break;
    case NoApp:
        text = "Requested package is not found.";
        break;
    case ResponceParsingFailed:
        text = "Response has incorrect format and cannot be parsed";
        break;
    case SettingsNotSet:
        text = "Not all of required settings are specified."
                "\nPlease, check your e-mail, password and android ID in the settings.";
        header = "Google Play client";
        displayInMessageBox = true;
        break;
    case UnknownError:
        text = "Unknown error occured. Send this info to developers, please:\n" + description;
        break;
    case Waiting:
        text = "Waiting for your response completion...";
        break;
    case BadRequest:
        text = "Bad request.";
        break;
    case AuthorizationFailedMessagebox:
        text = "Authorization failed. Check your email and password in the settings.";
        displayInMessageBox = true;
        break;
    case AuthorizationFailedNotification:
        text = "Authorization failed, change your settings please.";
        break;
    case AuthorizationTest:
        text = "Trying to log in, wait please...";
        break;
    case AuthorizationOk:
        text = "Logged in.";
        break;
    default:
        text = "The message handler has got a message that is not supported yet."
                "\nPlease contact the developer team.";
    }
    qDebug() << QString("messageSignalHandler: {\n\ttype: %1\n\tdescription: %2"
                        "\n\ttext = %3\n}\n").arg(QString(type + '0'), description, text);
    if (displayInMessageBox) {
        QMessageBox::information(this, header, text);
    }
    else {
        ui->AppInfo->clear();
        ui->AppInfo->append(text);
    }
    if (appIsDead) {
        QCoreApplication::exit(type);
    }
}

void MainWindow::autoSuggest()
{
    //Show suggests if text in Search field doesn't start from "pname:" and its length is more than 3 symbols
    if(ui->SearchString->text().length()<3) return;
    if(!ui->SearchString->text().startsWith("pname:",Qt::CaseSensitive))
    {
       session->searcheApp(ui->SearchString->text());
    }
}

void MainWindow::AppsResponseHeandle(AppsResponse response)
{
    if (!response.app_size())
        return;

    const QPalette &pal = ui->SearchString->palette();
    QColor color = pal.color(QPalette::Disabled, QPalette::WindowText);

    suggest->setUpdatesEnabled(false);
    suggest->clear();
    suggest->setRowCount(response.app_size());
    for(int i=0;i!=response.app_size();i++)
    {
        QTableWidgetItem * titel=new QTableWidgetItem(response.app(i).title().c_str());
        QTableWidgetItem * rating=new QTableWidgetItem(response.app(i).rating().c_str());
        titel->setTextAlignment(Qt::AlignLeft);
        titel->setData(Qt::UserRole,response.app(i).packagename().c_str());
        rating->setTextAlignment(Qt::AlignRight);
        titel->setTextColor(color);
        rating->setTextColor(color);

        suggest->setItem(i,0,titel);
        suggest->setItem(i,1,rating);
        suggest->setRowHeight(i,20);
    }

    suggest->setCurrentItem(suggest->item(0,0));
    suggest->resizeColumnsToContents();
    suggest->adjustSize();
    suggest->setUpdatesEnabled(true);

    suggest->move(ui->SearchString->mapToGlobal(QPoint(0, ui->SearchString->height())));
    suggest->setFocus();
    suggest->show();
}

void MainWindow::setupSuggest()
{
    suggest->setColumnCount(2);
    suggest->setEditTriggers(QTableWidget::NoEditTriggers);
    suggest->setFrameStyle(QFrame::Box | QFrame::Plain);
    suggest->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    suggest->setSelectionBehavior(QTableWidget::SelectRows);
    suggest->verticalHeader()->hide();
    suggest->horizontalHeader()->hide();
    suggest->installEventFilter(this);
    suggest->setMouseTracking(true);
    suggest->setWindowFlags(Qt::Popup);
    suggest->setFocusPolicy(Qt::NoFocus);
    suggest->setFocusProxy(ui->SearchString);
    //suggest->hide();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    {
        if (obj != suggest)
            return false;

        if (ev->type() == QEvent::MouseButtonPress) {
            suggest->hide();
            ui->SearchString->setFocus();
            return true;
        }

        if (ev->type() == QEvent::KeyPress) {

            bool consumed = false;
            int key = static_cast<QKeyEvent*>(ev)->key();
            switch (key) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                doneCompletion();
                consumed = true;

            case Qt::Key_Escape:
                ui->SearchString->setFocus();
                suggest->hide();
                consumed = true;

            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Home:
            case Qt::Key_End:
            case Qt::Key_PageUp:
            case Qt::Key_PageDown:
                break;

            default:
                ui->SearchString->setFocus();
                ui->SearchString->event(ev);
                suggest->hide();
                break;
            }

            return consumed;
        }

        return false;
    }
}

void MainWindow::doneCompletion()
{
    timer->stop();
    suggest->hide();
    ui->SearchString->setFocus();
    QTableWidgetItem *item = suggest->currentItem();
    if (item) {
        ui->SearchString->setText(QString("pname:%1").arg(item->data(Qt::UserRole).toString()));
        QKeyEvent *e;
        e = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        QApplication::postEvent(ui->SearchString, e);
        e = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier);
        QApplication::postEvent(ui->SearchString, e);
    }
}
