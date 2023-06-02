#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QBuffer>
#include <QUdpSocket>

#define PORT 50000

/* 包头 */
struct packageHeader
{
    int    dataTotalSize;        //总数据大小
    int    packageAmount;        //分包的总数
    int    uPackageTotalSize;    //当前分包总大小
    int    uPackageIndex;        //当前分包包号
    int    uPackageDataSize;     //当前分包数据大小
    int    uDataOffset;          //当前分包数据在总数据的偏移量
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
