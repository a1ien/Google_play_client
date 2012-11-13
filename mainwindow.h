#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "marketsession.h"
#include "settings.h"
#include "market.pb.h"
#include "downloader.h"
#include <QMainWindow>
#include "marketsession.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

signals:
    void MessageSignal(MessageTypes type, QString description = "");

private slots:
    void on_Download_clicked();

    void on_SearchString_textEdited(const QString &arg1);

    void on_SettingsButton_clicked();

public slots:
  void onLogon();
  void messageSignalHandler(MessageTypes type, const QString description);

private:
  Ui::MainWindow * ui;
  Settings       * settings;
  MarketSession  * session;
  Downloader     * downloader;
};

#endif // MAINWINDOW_H
