#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include <QBuffer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkAccessManager>

#define HOSTADDR "192.168.6.54"
#define PORT     50000
#define PACKAGEDATASIZE 400 //һ���ְ������ݴ�С


class Video:public QWidget
{
    Q_OBJECT

public:
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



public:
    Video();

public slots:
    void on_readCommand();         //��ȡ����˷���������
    void on_sendCaptureStream(QImage image);   //���Ͳ�����Ƶ��

private:
    QTcpSocket* m_tcpSocket;
    QUdpSocket* m_udpSocket;


};

#endif // VIDEO_H
