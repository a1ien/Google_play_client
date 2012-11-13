#include "downloader.h"
#include <QDebug>
#include <QFile>
#include "marketsession.h"

Downloader::Downloader(QObject *parent) :
  QObject(parent)
{
    connect(&manager,SIGNAL(finished(QNetworkReply*)),this, SLOT(requestComplete(QNetworkReply *)));
}

void Downloader::DownloadFile(const GetAssetResponse_InstallAsset &ia, const QString &fileName)
{
    QUrl url(ia.bloburl().c_str());
    req = MarketSession::setUsualHeaderSet(url);
    req.setRawHeader("Cookie", QString("%1=%2").arg(ia.downloadauthcookiename().c_str())
            .arg(ia.downloadauthcookievalue().c_str()).toUtf8());

    QNetworkReply * reply = manager.get(req);
    reply->setProperty("fileName", fileName);
}

QUrl Downloader::redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const {
    QUrl redirectUrl;
    if (!possibleRedirectUrl.isEmpty() && (possibleRedirectUrl != oldRedirectUrl)) {
        redirectUrl = possibleRedirectUrl;
    }
    return redirectUrl;
}

void Downloader::requestComplete(QNetworkReply * reply)
{
    QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    // We'll deduct if the redirection is valid in the redirectUrl function
    QUrl urlRedirectedTo = redirectUrl(possibleRedirectUrl.toUrl(), urlRedirectedTo);

    // If the URL is not empty, we're being redirected.
    if (!urlRedirectedTo.isEmpty()) {
        req.setUrl(urlRedirectedTo);
        QNetworkReply * r = manager.get(req);
        r->setProperty("fileName", reply->property("fileName"));
    }
    else {
        QFile file(reply->property("fileName").toString());
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();
        urlRedirectedTo.clear();
        emit DownloadFinish(reply->property("fileName").toString());
    }
    reply->deleteLater();
}
