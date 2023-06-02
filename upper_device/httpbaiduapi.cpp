#include "httpbaiduapi.h"

HttpBaiDuApi::HttpBaiDuApi(QObject *parent) : QObject(parent)
{

}

QString HttpBaiDuApi::requestAccessToken(QString api_key, QString secret_key)
{

    //ƴ�Ӽ�Ȩ��������url
    QString url = QString(BAIDU_TOKENURL).arg(api_key).arg(secret_key);

    QNetworkAccessManager manager;
    QNetworkRequest request;

    //��װ����ͷ��
    request.setUrl(url);
    request.setRawHeader("Content-Type", URLTYPE);

    //��������,�������ȴ�����������Ӧ
    QEventLoop loop;
    QNetworkReply *reply = manager.post( request, QByteArray() );
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //����JSON����
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject   obj  = json.object();

    if(reply != nullptr || reply->error() == QNetworkReply::NoError)
    {
        /* û�д������� */
        QString access_token = obj.value("access_token").toString();
        //qDebug()<<"access_token = "<<access_token;
        return access_token;
    }
    else
    {
        /* ���ִ������� */
        qDebug()<<QString::fromLocal8Bit("������룺")<<reply->error();
        qDebug()<<QString::fromLocal8Bit("������Ϣ��")<<reply->errorString();
        return QString();
    }
}

/* ����ʶ�� */
QString HttpBaiDuApi::animalRecognition(QString access_token, QByteArray &imageUrlCodec)
{
    if(access_token.isEmpty())
    {
        return QString();
    }

    QNetworkAccessManager manager;
    QNetworkRequest request;

    //ƴ��API����url
    QString url = QString(ANIMALRECONGNITION_APIURL).arg(access_token);
    request.setUrl(url);
    request.setRawHeader("Content-Type", URLTYPE);

    //��������,�������ȴ�����������Ӧ
    QEventLoop loop;
    QNetworkReply *reply = manager.post( request, imageUrlCodec );
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //����JSON����
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
        qDebug()<<QString::fromLocal8Bit("������룺")<<reply->error();
        qDebug()<<QString::fromLocal8Bit("������Ϣ��")<<reply->errorString();
        return QString();
    }
}

/* ����ʶ�� */
QString HttpBaiDuApi::carPlateRecognition(QString access_token, QByteArray &imageUrlCodec)
{
    if(access_token.isEmpty())
    {
        return QString();
    }

    QNetworkAccessManager manager;
    QNetworkRequest request;

    //ƴ��API����url
    QString url = QString(CARPLATERECONGTION_APIURL).arg(access_token);
    request.setUrl(url);
    request.setRawHeader("Content-Type", URLTYPE);

    //��������,�������ȴ�����������Ӧ
    QEventLoop loop;
    QNetworkReply *reply = manager.post( request, imageUrlCodec );
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //����JSON����
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
        qDebug()<<QString::fromLocal8Bit("������룺")<<reply->error();
        qDebug()<<QString::fromLocal8Bit("������Ϣ��")<<reply->errorString();
        return QString();
    }
}

/* ����ʶ�� */
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

    //����������JSON��ʽ����
    data.insert("image", QJsonValue(faceImage));
    data.insert("image_type", QJsonValue("BASE64"));
    data.insert("group_id_list", QJsonValue("1"));

    QJsonDocument jsonData(data);
    QByteArray body = jsonData.toJson();

    //ƴ��API����url
    QString url = QString(FACESEARCH_APIURL).arg(access_token);
    request.setUrl(url);
    request.setRawHeader("Content-Type", JSONTYPE);

    //��������,�������ȴ�����������Ӧ
    QEventLoop loop;
    QNetworkReply *reply = manager.post( request, body );
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug()<<reply->readAll().data();

    //����JSON����
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject   jsonObj = json.object();
    //bug :reply���Ի�ȡ���ݣ�������ȡ��ʱ���ǿ�����
    if(reply != nullptr || reply->error() == QNetworkReply::NoError)
    {
        QJsonArray user_list = jsonObj["user_list"].toArray();
        QJsonObject person = user_list[0].toObject();

        QString group_id = person.value("group_id").toString();
        QString user_id  = person.value("user_id").toString();
        QString user_info = person.value("user_info").toString();
        double score = person.value("score").toDouble();

        qDebug()<<QString::fromLocal8Bit("�û��� = ")<<group_id.toUtf8();
        qDebug()<<QString::fromLocal8Bit("�û��� = ")<<user_id.toUtf8();
        qDebug()<<QString::fromLocal8Bit("�û���Ϣ = ")<<user_info;
        qDebug()<<QString::fromLocal8Bit("ƥ��ֵ = ")<<score;
        return true;
    }
    else
    {
        qDebug()<<QString::fromLocal8Bit("������룺")<<reply->error();
        qDebug()<<QString::fromLocal8Bit("������Ϣ��")<<reply->errorString();
        return false;
    }
}
