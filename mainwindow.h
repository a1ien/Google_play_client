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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "marketsession.h"
#include "settings.h"
#include "market.pb.h"
#include "downloader.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>
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
  void autoSuggest();
  void AppsResponseHeandle(AppsResponse response);

private:
  void setupSuggest();
  void showCompletion(QList< QPair<QString,QString> >const& data);
  bool eventFilter(QObject *obj, QEvent * ev);
  void doneCompletion();

  Ui::MainWindow * ui;
  Settings       * settings;
  MarketSession  * session;
  Downloader     * downloader;
  QTableWidget   * suggest;
  QTimer         * timer;
};

#endif // MAINWINDOW_H
