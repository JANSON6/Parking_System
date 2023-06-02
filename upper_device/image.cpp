#include "image.h"

Image::Image()
{

}

QByteArray Image::imageToUrlCodec(QImage image)
{
    /* 用QBuffer对象让QByteArray对象像文件一样被操作 */
    QByteArray buffer;
    QBuffer data(&buffer);
    data.open(QIODevice::WriteOnly);
    image.save(&data, "JPG");

    /* 1 - 对图片进行base64编码 */
    QByteArray base64 = buffer.toBase64();

    /* 2 - 对base64编码完成的图片进行URL编码 */
    QByteArray urlencode = base64.toPercentEncoding();
    urlencode.insert(0, "image=");
    return urlencode;
}

QByteArray Image::imageToBase64(QString &fileName)
{
    QImage image(fileName);

    /* 用QBuffer对象让QByteArray对象像文件一样被操作 */
    QByteArray buffer;
    QBuffer data(&buffer);
    data.open(QIODevice::WriteOnly);
    image.save(&data, "JPG");

    /* 对图片进行base64编码 */
    QByteArray base64 = buffer.toBase64();
    return base64;
}

