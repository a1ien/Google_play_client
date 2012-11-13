#include "downloader.h"
#include <QDebug>
#include <QFile>

Downloader::Downloader(QObject *parent) :
  QObject(parent)
{
  connect(&manager,SIGNAL(finished(QNetworkReply*)),this, SLOT(requestComplite(QNetworkReply*)));
}

void Downloader::DownloadFile(const GetAssetResponse_InstallAsset &ia, const QString &fileName)
{
  QUrl url(ia.bloburl().c_str());
  req.setUrl(url);
  req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  req.setRawHeader("Cookie",QString("%1=%2").arg(ia.downloadauthcookiename().c_str())
                   .arg(ia.downloadauthcookievalue().c_str()).toUtf8());
  req.setRawHeader("User-Agent", "Android-Market/2 (sapphire PLAT-RC33); gzip");
  req.setRawHeader("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");

  QNetworkReply* reply=manager.get(req);
  reply->setProperty("fileName",fileName);
}

QUrl Downloader::redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const {
    QUrl redirectUrl;
    if(!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != oldRedirectUrl)
        redirectUrl = possibleRedirectUrl;
    return redirectUrl;
}

void Downloader::requestComplite(QNetworkReply* reply)
{
  QVariant possibleRedirectUrl =
      reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

  // We'll deduct if the redirection is valid in the redirectUrl function
  QUrl urlRedirectedTo = redirectUrl(possibleRedirectUrl.toUrl(), urlRedirectedTo);

  // If the URL is not empty, we're being redirected.
  if(!urlRedirectedTo.isEmpty())
    {
      qDebug()<<possibleRedirectUrl;
      req.setUrl(urlRedirectedTo);
      QNetworkReply* r=manager.get(req);
      r->setProperty("fileName",reply->property("fileName"));
    }
  else
    {
      qDebug()<<possibleRedirectUrl;
      qDebug()<<reply->property("fileName");
      QFile file(reply->property("fileName").toString());
      file.open(QIODevice::WriteOnly);
      file.write(reply->readAll());
      file.close();
      urlRedirectedTo.clear();
      emit DownloadFinish(reply->property("fileName").toString());
    }

  reply->deleteLater();
}
