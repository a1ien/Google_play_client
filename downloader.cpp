#include "downloader.h"

Downloader::Downloader(GetAssetResponse_InstallAsset const &ia,QObject *parent) :
  QObject(parent),http(NULL)
{

  QUrl url(ia.bloburl().c_str());
  QNetworkRequest req;
  req.setUrl(url);
  req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  req.setRawHeader("Cookie",QString("%1=%2").arg(ia.downloadauthcookiename().c_str())
                   .arg(ia.downloadauthcookievalue().c_str()).toUtf8());
  req.setRawHeader("User-Agent", "Android-Market/2 (sapphire PLAT-RC33); gzip");
  req.setRawHeader("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");

  http = manager.get(req);
  connect(http,SIGNAL(finished()),this, SLOT(requestComplite()));

}


void Downloader::requestComplite()
{

}
