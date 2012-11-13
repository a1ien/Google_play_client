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
