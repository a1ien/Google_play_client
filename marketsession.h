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

// Allowed message types
// for MessageSignal / messageSignalHandler
enum MessageTypes {
    UnknownError,                    // The type of any error which is not listed here
    EmptyResponce,                   // The response is empty
    SettingsNotSet,                  // Settings were not set
    NoApp,                           // No such application in Google Play
    PaidApp,                        // It's Paid App
    AppDownloaded,                   // App was success download
    ResponceParsingFailed,           // Cannot parse the response
    Waiting,                         // Waiting for operation completion
    BadRequest,                      // Bad request
    AuthorizationFailedMessagebox,   // Authorization failed (wrong email / password)
                                     //  -- message should be shown in the MessageBox
    AuthorizationFailedNotification, // Authorization failed (wrong email / password)
                                     //  -- message should be shown in the notification area
    AuthorizationTest,               // Authorization try
    AuthorizationOk                  // User has been authorized successfully
};

// Market session ~singletone class
class MarketSession : public QObject {
Q_OBJECT

public:
    // This method returns the single instance
    static MarketSession * getInstance(QObject * parent);

    // login() method overloads:
    void login();
    // accountType should be QString("HOSTED_OR_GOOGLE") by default
    void login(QString email, QString password, QString androidId, QString accountType);

    // App downloader initialization method
    void getApp();

    // Setters, getters
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

    void setOperatorNumeric(QString const& num)
    {
        context.set_operatornumeric(num.toUtf8());
        context.set_simoperatornumeric(num.toUtf8());
    }

    void setOperatorAlpha(QString const& alpha)
    {
        context.set_operatoralpha(alpha.toUtf8());
        context.set_simoperatoralpha(alpha.toUtf8());
    }

    void setContry(QString const& contry,QString const& lang)
    {
        context.set_usercountry(contry.toUtf8());
        context.set_userlanguage(lang.toUtf8());
    }

    // Responce executor
    Response::ResponseGroup * execute(Request::RequestGroup requestGroup);

    // This method returns the apk description
    App getAppInfo(QString name);

    // Static method to set commonly used headers
    static QNetworkRequest setUsualHeaderSet(QUrl url);

    GetAssetResponse::InstallAsset getInstallAsset(QString appId);

    void searcheApp(QString const& query);

private:
    // Private constructor
    explicit MarketSession(QObject *parent = 0);


    void postUrl(const QString & url, QMap< QString, QString > params);
    QByteArray executeProtobuf(Request request);
    QByteArray executeRawQuery(const QByteArray & request);
    QByteArray gzipDecompress(QByteArray compressData);

    void executeWaitingTasksQuery();

signals:
    void GetAppSignal();
    void NeedToRelogin();
    void MessageSignal(MessageTypes type, const QString description = "");
    void SearcheComplite(AppsResponse);
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
