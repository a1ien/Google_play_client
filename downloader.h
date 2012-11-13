#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QObject>
#include "market.pb.h"

class Downloader : public QObject
{
  Q_OBJECT
public:
  explicit Downloader(QObject *parent = 0);
  void DownloadFile(GetAssetResponse_InstallAsset const& ia,QString const &fileName);
signals:
  void DownloadFinish(QString fileName);
private slots:
  void requestComplite(QNetworkReply *reply);
private:
  QUrl redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const;
  QNetworkAccessManager manager;
  QNetworkRequest req;
  
};

#endif // DOWNLOADER_H
