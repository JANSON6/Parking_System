#include "httpbaiduapi.h"

HttpBaiDuApi::HttpBaiDuApi(QObject *parent) : QObject(parent)
{

}

QString HttpBaiDuApi::requestAccessToken(QString api_key, QString secret_key)
{

    //拼接鉴权令牌请求url
    QString url = QString(BAIDU_TOKENURL).arg(api_key).arg(secret_key);

    QNetworkAccessManager manager;
    QNetworkRequest request;

    //封装请求头部
    request.setUrl(url);
    request.setRawHeader("Content-Type", URLTYPE);

    //发起请求,并阻塞等待服务器的响应
    QEventLoop loop;
    QNetworkReply *reply = manager.post( request, QByteArray() );
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //解析JSON数据
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject   obj  = json.object();

    if(reply != nullptr || reply->error() == QNetworkReply::NoError)
    {
        /* 没有错误的情况 */
        QString access_token = obj.value("access_token").toString();
        //qDebug()<<"access_token = "<<access_token;
        return access_token;
    }
    else
    {
        /* 出现错误的情况 */
        qDebug()<<QString::fromLocal8Bit("错误代码：")<<reply->error();
        qDebug()<<QString::fromLocal8Bit("错误信息：")<<reply->errorString();
        return QString();
    }
}

/* 动物识别 */
QString HttpBaiDuApi::animalRecognition(QString access_token, QByteArray &imageUrlCodec)
{
    if(access_token.isEmpty())
    {
        return QString();
    }

    QNetworkAccessManager manager;
    QNetworkRequest request;

    //拼接API请求url
    QString url = QString(ANIMALRECONGNITION_APIURL).arg(access_token);
    request.setUrl(url);
    request.setRawHeader("Content-Type", URLTYPE);

    //发起请求,并阻塞等待服务器的响应
    QEventLoop loop;
    QNetworkReply *reply = manager.post( request, imageUrlCodec );
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //解析JSON数据
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject   jsonObj = json.object();

    if(reply != nullptr || reply->error() == QNetworkReply::NoError)
    {
        QJsonArray result = jsonObj.value("result").toArray();
        QJsonObject animal = result[0].toObject();
        QString name = animal.value("name").toString();
        return name;
    }
    else
    {
        qDebug()<<QString::fromLocal8Bit("错误代码：")<<reply->error();
        qDebug()<<QString::fromLocal8Bit("错误信息：")<<reply->errorString();
        return QString();
    }
}

/* 车牌识别 */
QString HttpBaiDuApi::carPlateRecognition(QString access_token, QByteArray &imageUrlCodec)
{
    if(access_token.isEmpty())
    {
        return QString();
    }

    QNetworkAccessManager manager;
    QNetworkRequest request;

    //拼接API请求url
    QString url = QString(CARPLATERECONGTION_APIURL).arg(access_token);
    request.setUrl(url);
    request.setRawHeader("Content-Type", URLTYPE);

    //发起请求,并阻塞等待服务器的响应
    QEventLoop loop;
    QNetworkReply *reply = manager.post( request, imageUrlCodec );
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //解析JSON数据
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject   jsonObj = json.object();

    if(reply != nullptr || reply->error() == QNetworkReply::NoError)
    {
        QJsonObject words_result = jsonObj.value("words_result").toObject();
        QString number = words_result.value("number").toString();
        //qDebug()<<number;
        return number;
    }
    else
    {
        qDebug()<<QString::fromLocal8Bit("错误代码：")<<reply->error();
        qDebug()<<QString::fromLocal8Bit("错误信息：")<<reply->errorString();
        return QString();
    }
}

/* 人脸识别 */
bool HttpBaiDuApi::faceSearch(QString access_token, QByteArray &imageBase64)
{
    if(access_token.isEmpty())
    {
        return false;
    }

    QNetworkAccessManager manager;
    QNetworkRequest request;

    QJsonObject data;
    QString faceImage(imageBase64);

    //设置请求体JSON格式参数
    data.insert("image", QJsonValue(faceImage));
    data.insert("image_type", QJsonValue("BASE64"));
    data.insert("group_id_list", QJsonValue("1"));

    QJsonDocument jsonData(data);
    QByteArray body = jsonData.toJson();

    //拼接API请求url
    QString url = QString(FACESEARCH_APIURL).arg(access_token);
    request.setUrl(url);
    request.setRawHeader("Content-Type", JSONTYPE);

    //发起请求,并阻塞等待服务器的响应
    QEventLoop loop;
    QNetworkReply *reply = manager.post( request, body );
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug()<<reply->readAll().data();

    //解析JSON数据
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject   jsonObj = json.object();
    //bug :reply可以获取数据，但是提取的时候是空数据
    if(reply != nullptr || reply->error() == QNetworkReply::NoError)
    {
        QJsonArray user_list = jsonObj["user_list"].toArray();
        QJsonObject person = user_list[0].toObject();

        QString group_id = person.value("group_id").toString();
        QString user_id  = person.value("user_id").toString();
        QString user_info = person.value("user_info").toString();
        double score = person.value("score").toDouble();

        qDebug()<<QString::fromLocal8Bit("用户组 = ")<<group_id.toUtf8();
        qDebug()<<QString::fromLocal8Bit("用户码 = ")<<user_id.toUtf8();
        qDebug()<<QString::fromLocal8Bit("用户信息 = ")<<user_info;
        qDebug()<<QString::fromLocal8Bit("匹配值 = ")<<score;
        return true;
    }
    else
    {
        qDebug()<<QString::fromLocal8Bit("错误代码：")<<reply->error();
        qDebug()<<QString::fromLocal8Bit("错误信息：")<<reply->errorString();
        return false;
    }
}
