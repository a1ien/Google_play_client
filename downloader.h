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

// The apk file downloader class
class Downloader : public QObject {
Q_OBJECT

public:
    explicit Downloader(QObject * parent = 0);
    // This method can be used to download an apk file
    void DownloadFile(GetAssetResponse::InstallAsset const & ia, QString const & fileName);

signals:
    // This signal is usually sent when download is finished
    void DownloadFinish(QString fileName);

private slots:
    // This slot is usually used when request is finished
    void requestComplete(QNetworkReply * reply);

private:
    // Google doesn't gives a direct link to a file, it uses a redirection
    QUrl redirectUrl(const QUrl & possibleRedirectUrl, const QUrl & oldRedirectUrl) const;
    QNetworkAccessManager manager;
    QNetworkRequest req;
};

#endif // DOWNLOADER_H
