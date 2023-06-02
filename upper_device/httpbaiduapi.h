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
    /* 请求正文类型 */
    #define URLTYPE "application/x-www-form-urlencoded"
    #define JSONTYPE "application/json"

    /* 百度API鉴权认证URL */
    #define BAIDU_TOKENURL     "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&"

    /* 动物识别 */
    #define ANIAMLRECONGNITION_APIKEY     "1z8ogsojS99QNGKuCRPV0XU6"
    #define ANIMALRECONGNITION_SECRETKEY  "b29DGwAth2bLv0WVio49wXSen3pr3tfE"
    #define ANIMALRECONGNITION_APIURL     "https://aip.baidubce.com/rest/2.0/image-classify/v1/animal?access_token=%1"

    /* 车牌识别 */
    #define CARPLATERECONGTION_APIKEY    "8xnvOid7BMZeGT1ORBSjb2zP"
    #define CARPLATERECONGTION_SECRETKEY "SyMlRpYqkpybXiriT5Pz1nMKMBrRrieF"
    #define CARPLATERECONGTION_APIURL    "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate?access_token=%1"

    /* 人脸搜索 */
    #define FACESEARCH_APIKEY            "Hyfm6CTh3G13fhags1O3F59a"
    #define FACESEARCH_SECRETKEY         "U7CdSvnM6bCXgKNjqOM1wnrz5hxrW8ME"
    #define FACESEARCH_APIURL            "https://aip.baidubce.com/rest/2.0/face/v3/search?access_token=%1"


public:
    explicit HttpBaiDuApi(QObject *parent = nullptr);

    static QString requestAccessToken(QString api_key, QString secret_key);    /*获取鉴权认证令牌*/

    static QString animalRecognition(QString access_token, QByteArray &imageUrlCodec);    /* 动物图片识别 */
    static QString carPlateRecognition(QString access_token, QByteArray &imageUrlCodec);  /* 车牌图片识别 */
    static bool    faceSearch(QString access_token, QByteArray &imageBase64);           /* 人脸库检索图片对比 */

};

#endif // HTTPBAIDUAPI_H


/**
  车牌识别接口：carPlateRecognition()
  参数：access_token : 鉴权令牌  | imageUrlCodec : 图片的url编码格式数据
  返回值：无错误【QString 车牌号】| 有错误【QString 空字串】 发生错误会打印错误信息

*/
