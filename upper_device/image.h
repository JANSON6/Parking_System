#ifndef IMAGE_H
#define IMAGE_H
#include <QByteArray>
#include <QTextCodec>
#include <QImage>
#include <QBuffer>
#include <QDebug>
/* 图像类 */
class Image
{
public:
    Image();

    /* 把图像转成动物识别图像格式 */
    static QByteArray imageToUrlCodec(QImage image);
    static QByteArray imageToBase64(QString &fileName);
};

#endif // IMAGE_H
