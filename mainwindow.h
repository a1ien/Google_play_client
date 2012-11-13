#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "marketsession.h"
#include "settings.h"
#include "emptysettingswarning.h"
#include "market.pb.h"
#include <QMainWindow>


namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
private slots:
    void on_SearchButton_clicked();

    void on_DownloadButton_clicked();

private:
  Ui::MainWindow *ui;
  Settings* settings;
  EmptySettingsWarning *emptySettsWarn;
  MarketSession* session;

};

#endif // MAINWINDOW_H
