#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),settings(new Settings(this)),session(new MarketSession(this)),downloader(new Downloader(this))
{
  ui->setupUi(this);
  connect(session,SIGNAL(logged()),this,SLOT(onLogon()));
  while(settings->someIsEmpty())
      settings->exec();

  session->login(settings->email(),settings->password(),settings->androidid(),QString("HOSTED_OR_GOOGLE"));

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::onLogon()
{
  App app=session->getAppInfo("");

  GetAssetResponse_InstallAsset ia=session->getInstallAsset(app.id().c_str());
  qDebug()<<ia.DebugString().c_str();
  downloader->DownloadFile(ia,"some.apk");
}
