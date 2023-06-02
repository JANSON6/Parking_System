#ifndef HTTPBAIDUAPI_H
#define HTTPBAIDUAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QEventLoop>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QElapsedTimer>

class HttpBaiDuApi : public QObject
{
    Q_OBJECT

public:
    /* ������������ */
    #define URLTYPE "application/x-www-form-urlencoded"
    #define JSONTYPE "application/json"

    /* �ٶ�API��Ȩ��֤URL */
    #define BAIDU_TOKENURL     "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&"

    /* ����ʶ�� */
    #define ANIAMLRECONGNITION_APIKEY     "1z8ogsojS99QNGKuCRPV0XU6"
    #define ANIMALRECONGNITION_SECRETKEY  "b29DGwAth2bLv0WVio49wXSen3pr3tfE"
    #define ANIMALRECONGNITION_APIURL     "https://aip.baidubce.com/rest/2.0/image-classify/v1/animal?access_token=%1"

    /* ����ʶ�� */
    #define CARPLATERECONGTION_APIKEY    "8xnvOid7BMZeGT1ORBSjb2zP"
    #define CARPLATERECONGTION_SECRETKEY "SyMlRpYqkpybXiriT5Pz1nMKMBrRrieF"
    #define CARPLATERECONGTION_APIURL    "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate?access_token=%1"

    /* �������� */
    #define FACESEARCH_APIKEY            "Hyfm6CTh3G13fhags1O3F59a"
    #define FACESEARCH_SECRETKEY         "U7CdSvnM6bCXgKNjqOM1wnrz5hxrW8ME"
    #define FACESEARCH_APIURL            "https://aip.baidubce.com/rest/2.0/face/v3/search?access_token=%1"


public:
    explicit HttpBaiDuApi(QObject *parent = nullptr);

    static QString requestAccessToken(QString api_key, QString secret_key);    /*��ȡ��Ȩ��֤����*/

    static QString animalRecognition(QString access_token, QByteArray &imageUrlCodec);    /* ����ͼƬʶ�� */
    static QString carPlateRecognition(QString access_token, QByteArray &imageUrlCodec);  /* ����ͼƬʶ�� */
    static bool    faceSearch(QString access_token, QByteArray &imageBase64);           /* ���������ͼƬ�Ա� */

};

#endif // HTTPBAIDUAPI_H


/**
  ����ʶ��ӿڣ�carPlateRecognition()
  ������access_token : ��Ȩ����  | imageUrlCodec : ͼƬ��url�����ʽ����
  ����ֵ���޴���QString ���ƺš�| �д���QString ���ִ��� ����������ӡ������Ϣ

*/
