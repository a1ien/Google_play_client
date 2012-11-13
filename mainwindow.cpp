#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),settings(new Settings(this)),session(new MarketSession())
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
  App app;
  app.CopyFrom(session->getAppInfo(""));


  Request_RequestGroup group;
  GetAssetRequest assetRequest;
  assetRequest.set_assetid(app.id());


  group.mutable_getassetrequest()->CopyFrom(assetRequest);
  GetAssetResponse assetResponse=session->execute(group)->getassetresponse();
  GetAssetResponse_InstallAsset ia=assetResponse.installasset(0);
  qDebug()<<ia.DebugString().c_str();
  download=new Downloader(ia);
}
