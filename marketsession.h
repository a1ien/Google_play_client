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

#ifndef MARKETSESSION_H
#define MARKETSESSION_H

#include <QObject>
#include <QMap>
#include <QtNetwork/QNetworkReply>
#include "market.pb.h"

const int PROTOCOL_VERSION = 2;

enum MessageTypes {
    UnknownError          = 0,
    EmptyResponce         = 1,
    SettingsNotSet        = 2,
    NoApp                 = 3,
    ResponceParsingFailed = 4,
    Waiting               = 5
};

class MarketSession : public QObject {
Q_OBJECT

public:
    explicit MarketSession(QObject *parent = 0);

    void login(QString email, QString password, QString androidId, QString accountType);
    void setAndroidID(QString & androidId) {
        context.set_androidid(androidId.toAscii());
    }
    void setAuthSubToken(QString authSubToken) {
        context.set_authsubtoken(authSubToken.toAscii());
        this->authSubToken = authSubToken;
    }
    QString & getAuthSubToken() {
        return authSubToken;
    }

    Response::ResponseGroup * execute(Request::RequestGroup requestGroup);
    App getAppInfo(QString name);

    static QNetworkRequest setUsualHeaderSet(QUrl url);

    GetAssetResponse::InstallAsset getInstallAsset(QString appId);
private:
    void postUrl(const QString & url, QMap<QString, QString> params);
    QByteArray executeProtobuf(Request request);
    QByteArray executeRawQuery(const QByteArray & request);
    QByteArray gzipDecompress(QByteArray compressData);

signals:
    void logged();
    void MessageSignal(MessageTypes type, const QString description = "");
public slots:

private slots:
    void loginFinished();

public:
    QString SERVICE;

private:
    const QString         URL_LOGIN;
    RequestContext        context;
    Request               request;
    Response              r;
    QString               authSubToken;
    QNetworkAccessManager qnam;
    QNetworkReply         * http;
};

#endif // MARKETSESSION_H
