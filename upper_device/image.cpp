#include "image.h"

Image::Image()
{

}

QByteArray Image::imageToUrlCodec(QImage image)
{
    /* ��QBuffer������QByteArray�������ļ�һ�������� */
    QByteArray buffer;
    QBuffer data(&buffer);
    data.open(QIODevice::WriteOnly);
    image.save(&data, "JPG");

    /* 1 - ��ͼƬ����base64���� */
    QByteArray base64 = buffer.toBase64();

    /* 2 - ��base64������ɵ�ͼƬ����URL���� */
    QByteArray urlencode = base64.toPercentEncoding();
    urlencode.insert(0, "image=");
    return urlencode;
}

QByteArray Image::imageToBase64(QString &fileName)
{
    QImage image(fileName);

    /* ��QBuffer������QByteArray�������ļ�һ�������� */
    QByteArray buffer;
    QBuffer data(&buffer);
    data.open(QIODevice::WriteOnly);
    image.save(&data, "JPG");

    /* ��ͼƬ����base64���� */
    QByteArray base64 = buffer.toBase64();
    return base64;
}

