#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QObject>
#include "market.pb.h"

class Downloader : public QObject
{
  Q_OBJECT
public:
  explicit Downloader(GetAssetResponse_InstallAsset const& ia, QObject *parent = 0);
  
signals:
  void DownloadFinish();
public slots:
  void requestComplite();
private:
  QNetworkAccessManager manager;
  QNetworkReply* http;
  
};

#endif // DOWNLOADER_H
