#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),settings(new Settings(this)),session(new MarketSession())
{
  ui->setupUi(this);
  while(settings->someIsEmpty())
      settings->exec();

  session->login(settings->email(),settings->password(),settings->androidid(),QString("HOSTED_OR_GOOGLE"));
}

MainWindow::~MainWindow()
{
  delete ui;
}
