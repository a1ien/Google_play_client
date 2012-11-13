#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "marketsession.h"
#include "settings.h"
<<<<<<< HEAD
#include "emptysettingswarning.h"
#include "market.pb.h"
=======
#include "downloader.h"
>>>>>>> 8e74a52da19ed27e20e4fbeb12ce86621b52127a
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
    void on_Download_clicked();

    void on_SearchString_textEdited(const QString &arg1);

    void on_Settings_clicked();

public slots:
  void onLogon();

private:
  Ui::MainWindow *ui;
  Settings* settings;
  EmptySettingsWarning *emptySettsWarn;
  MarketSession* session;
  Downloader* download;

};

#endif // MAINWINDOW_H
