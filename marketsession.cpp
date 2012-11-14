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

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QStringList>
#include <QEventLoop>
#include <QDebug>
#include "zlib/zlib.h"
#include "marketsession.h"
#include <QDebug>
#include "decompressor.h"

MarketSession::MarketSession(QObject * parent)
    : QObject(parent),
      SERVICE("androidsecure"),
      URL_LOGIN("https://www.google.com/accounts/ClientLogin")
{
    context.set_issecure(true);
    context.set_version(2009011);
    context.set_usercountry("GB");
    context.set_userlanguage("en");
    context.set_deviceandsdkversion("passion:9");
    context.set_operatoralpha("T-Mobile");
    context.set_simoperatoralpha("T-Mobile");
    context.set_operatornumeric("310260");
    context.set_simoperatornumeric("310260");
}

Response::ResponseGroup * MarketSession::execute(Request::RequestGroup requestGroup) {
    request.Clear();
    request.mutable_context()->CopyFrom(context);
    request.add_requestgroup()->CopyFrom(requestGroup);
    qDebug() << request.DebugString().c_str();

    QByteArray responseBytes = executeProtobuf(request);
    r.Clear();
    if (!r.ParseFromArray(responseBytes.constData(), responseBytes.size())) {
        emit MessageSignal(ResponceParsingFailed); ///////////////////
        return 0;
    }
    return r.mutable_responsegroup(0);
}

App MarketSession::getAppInfo(QString name) {
    Request::RequestGroup group;
    AppsRequest app;
    app.set_query(name.toAscii());
    app.set_startindex(0);
    app.set_entriescount(10);

    group.mutable_appsrequest()->CopyFrom(app);
    Response::ResponseGroup * responseGroup = execute(group);
    AppsResponse * appResponse;
    if ((responseGroup == 0) || ((appResponse =
            responseGroup->mutable_appsresponse()) == 0) ||
            (appResponse->app_size() == 0)) {
        emit MessageSignal(NoApp); ///////////////////
        return App();
    }
    return appResponse->app(0);
}

GetAssetResponse::InstallAsset MarketSession::getInstallAsset(QString appId) {
  Request::RequestGroup group;
  GetAssetRequest assetRequest;
  assetRequest.set_assetid(appId.toAscii());
  group.mutable_getassetrequest()->CopyFrom(assetRequest);
  return execute(group)->getassetresponse().installasset(0);
}


void MarketSession::login(QString email, QString password, QString androidId, QString accountType) {
    setAndroidID(androidId);
    QMap<QString,QString> params;
    params.insert("Email",email);
    params.insert("Passwd", password);
    params.insert("service", SERVICE);
    params.insert("accountType", accountType);
    postUrl(URL_LOGIN, params);
}

void MarketSession::loginFinished() {
    qDebug("postURL");
    QString authKey;
    QStringList st = QString(http->readAll()).split(QRegExp("[\n\r=]"));
    while (!st.empty()) {
        if(st.takeFirst() == QString("Auth")) {
            authKey = st.takeFirst();
            break;
        }
    }
    qDebug() << authKey.toAscii();
    setAuthSubToken(authKey);
    http->deleteLater();
    emit logged();
}

void MarketSession::postUrl(const QString & url, QMap< QString, QString > params) {
    QString data = "";
    foreach(const QString & key, params.keys()) {
        data.append(QString("&%1=%2").arg(key).arg(params.value(key)));
    }
    data.remove(0, 1);
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    http = qnam.post(req, data.toUtf8());
    connect(http, SIGNAL(finished()), this, SLOT(loginFinished()));
}

QByteArray MarketSession::executeProtobuf(Request request) {
    QByteArray requestBytes(request.SerializeAsString().c_str(), request.ByteSize());
    // QByteArray responseBytes = executeRawQuery(requestBytes);
    return executeRawQuery(requestBytes);
}

QNetworkRequest MarketSession::setUsualHeaderSet(QUrl url) {
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", "Android-Market/2 (sapphire PLAT-RC33); gzip");
    request.setRawHeader("Accept-Charset","ISO-8859-1,utf-8;q=0.7,*;q=0.7");

    return request;
}

QByteArray MarketSession::executeRawQuery(const QByteArray & request) {
    QString requestAddress;
    requestAddress = (context.issecure())
            ? "https://android.clients.google.com/market/api/ApiRequest"
            : "http://android.clients.google.com/market/api/ApiRequest";

    QByteArray data;
    QUrl url(requestAddress);
    QNetworkAccessManager manager;
    QNetworkRequest req = setUsualHeaderSet(url);

    QString requestData = QString("version=%1&request=%2")
            .arg(PROTOCOL_VERSION)
            .arg(QString(request.toBase64()));
    QNetworkReply * http = manager.post(req, requestData.toUtf8());
    QEventLoop eventLoop;
    connect(http, SIGNAL(finished()), & eventLoop, SLOT(quit()));
    eventLoop.exec();
    qint32 len = http->header(QNetworkRequest::ContentLengthHeader).toUInt();
    if (len == 0) {
        emit MessageSignal(EmptyResponce);
    }
    data = gzipDecompress(http->read(len));
    delete http;
    return data;

}

QByteArray MarketSession::gzipDecompress(QByteArray compressData) {
    Decompressor decompressor;
    QByteArray decompressedData = decompressor.getDecompressedData(compressData);

    return decompressedData;
}
