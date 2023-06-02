#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QBuffer>
#include <QUdpSocket>

#define PORT 50000

/* ��ͷ */
struct packageHeader
{
    int    dataTotalSize;        //�����ݴ�С
    int    packageAmount;        //�ְ�������
    int    uPackageTotalSize;    //��ǰ�ְ��ܴ�С
    int    uPackageIndex;        //��ǰ�ְ�����
    int    uPackageDataSize;     //��ǰ�ְ����ݴ�С
    int    uDataOffset;          //��ǰ�ְ������������ݵ�ƫ����
};

class Video : public QThread
{
    Q_OBJECT

public:
    Video();

    void run() override;

signals:
    void gotOneFrameData(QImage image);


};

#endif // VIDEO_H
