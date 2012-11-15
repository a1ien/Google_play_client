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

#include "downloader.h"
#include <QDebug>
#include <QFile>
#include "marketsession.h"

Downloader::Downloader(QObject *parent) :
  QObject(parent)
{
    connect(&manager,SIGNAL(finished(QNetworkReply*)),this, SLOT(requestComplete(QNetworkReply *)));
}

void Downloader::DownloadFile(const GetAssetResponse::InstallAsset &ia, const QString &fileName)
{
    if(!ia.has_bloburl())
      return;
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
        emit MessageSignal(AppDownloaded,reply->property("fileName").toString());
    }
    reply->deleteLater();
}
