#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "marketsession.h"
#include "settings.h"
#include "downloader.h"
#include <QMainWindow>


namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT
  
public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

public slots:
    void onLogon();
private:
    Ui::MainWindow * ui;
    Settings       * settings;
    MarketSession  * session;
    Downloader     * download;
};

#endif // MAINWINDOW_H
