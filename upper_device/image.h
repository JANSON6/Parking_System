#ifndef IMAGE_H
#define IMAGE_H
#include <QByteArray>
#include <QTextCodec>
#include <QImage>
#include <QBuffer>
#include <QDebug>
/* ͼ���� */
class Image
{
public:
    Image();

    /* ��ͼ��ת�ɶ���ʶ��ͼ���ʽ */
    static QByteArray imageToUrlCodec(QImage image);
    static QByteArray imageToBase64(QString &fileName);
};

#endif // IMAGE_H
