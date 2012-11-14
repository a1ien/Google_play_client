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
#include <QVector>
#include "market.pb.h"

const int PROTOCOL_VERSION = 2;

enum MessageTypes {
    UnknownError,
    EmptyResponce,
    SettingsNotSet,
    NoApp,
    ResponceParsingFailed,
    Waiting,
    BadRequest,
    AuthorizationFailedMessagebox,
    AuthorizationTest,
    AuthorizationOk,
    AuthorizationFailedNotification
};

class MarketSession : public QObject {
Q_OBJECT

public:
    static MarketSession * getInstance(QObject * parent);

    void login();
    void login(QString email, QString password, QString androidId, QString accountType);
    void getApp();
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
    explicit MarketSession(QObject *parent = 0); // private constructor

    void postUrl(const QString & url, QMap< QString, QString > params);
    QByteArray executeProtobuf(Request request);
    QByteArray executeRawQuery(const QByteArray & request);
    QByteArray gzipDecompress(QByteArray compressData);

    void executeWaitingTasksQuery();

signals:
    void GetAppSignal();
    void NeedToRelogin();
    void MessageSignal(MessageTypes type, const QString description = "");
public slots:

private slots:
    void loginFinished();
    void needToReloginHandler();

public:
    QString SERVICE;

private:
    typedef void (MarketSession::* Task)();

    const QString         URL_LOGIN;
    RequestContext        context;
    Request               request;
    Response              r;
    QString               authSubToken;
    QNetworkAccessManager qnam;
    QNetworkReply         * http;
    QString               email;
    QString               password;
    QString               androidID;
    QString               accountType;
    QVector<Task>         waitingTaskQuery;
    bool                  isLoggedIn;
    bool                  isLoggingIn;
    bool                  isAuthFailed;
    static MarketSession  * instance;
};

#endif // MARKETSESSION_H
