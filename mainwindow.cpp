#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  settings(new Settings(this)),
  emptySettsWarn(new EmptySettingsWarning()),
  session(new MarketSession())
{
  ui->setupUi(this);      
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_DownloadButton_clicked()
{

    while (ui->SearchString->textLength() == 0)
    {
        DownloadButton->setEnabled(False);
    }
    if (settings->someIsEmpty())
        emptySettsWarn->exec();
    else
    {
        session->login(settings->email(),settings->password(),settings->androidid(),QString("HOSTED_OR_GOOGLE"));
        App *app;
        //initialized *app
        if (app->has_id())
        {

        }
        else

    }
}
