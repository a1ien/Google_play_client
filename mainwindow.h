#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "marketsession.h"
#include "settings.h"
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
  
private:
  Ui::MainWindow *ui;
  Settings* settings;
  MarketSession* session;

};

#endif // MAINWINDOW_H
