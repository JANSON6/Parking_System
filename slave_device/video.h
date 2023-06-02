#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include <QBuffer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkAccessManager>

#define HOSTADDR "192.168.6.54"
#define PORT     50000
#define PACKAGEDATASIZE 400 //一个分包的数据大小


class Video:public QWidget
{
    Q_OBJECT

public:
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



public:
    Video();

public slots:
    void on_readCommand();         //读取服务端发来的命令
    void on_sendCaptureStream(QImage image);   //发送捕获视频流

private:
    QTcpSocket* m_tcpSocket;
    QUdpSocket* m_udpSocket;


};

#endif // VIDEO_H
