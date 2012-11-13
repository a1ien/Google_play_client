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

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QObject>
#include "market.pb.h"

class Downloader : public QObject {
Q_OBJECT

public:
  explicit Downloader(QObject *parent = 0);
  void DownloadFile(GetAssetResponse_InstallAsset const& ia,QString const &fileName);
signals:
  void DownloadFinish(QString fileName);
private slots:
  void requestComplete(QNetworkReply *reply);
private:
  QUrl redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const;
  QNetworkAccessManager manager;
  QNetworkRequest req;
};

#endif // DOWNLOADER_H
