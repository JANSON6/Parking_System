#include "video.h"

Video::Video()
{

}

void Video::run()
{
    QUdpSocket udpSocket;

    if(udpSocket.bind(QHostAddress("192.168.6.54"), 50000))    //���ذ�
    {
        qDebug()<<"bind succeed!";
    }

    QByteArray imageCache;    /* һ֡ͼ���ܻ��� */
    int packetAmount = 0;     /* һ֡�ְ�����   */
    int dataTotalSize = 0;    /* һ֡�������ֽ� */
    int recvPacketCount = 0;  /* ���յķְ���   */

    while(1)
    {
        udpSocket.waitForReadyRead(-1);     //û�����ݵ�ʱ������

        while(1)
        {
            if(udpSocket.hasPendingDatagrams())
            {

                QByteArray onePacket;   /* һ���ְ����ջ��� */
                onePacket.resize(udpSocket.pendingDatagramSize());   //����һ���ְ������С
                udpSocket.readDatagram(onePacket.data(), onePacket.size());

                packageHeader header;
                memcpy(&header, onePacket.data(), 24);

                recvPacketCount ++;

                if(packetAmount == 0)
                {
                    packetAmount = header.packageAmount;
                    dataTotalSize = header.dataTotalSize;
                    imageCache.resize(dataTotalSize);
                }

                memcpy(imageCache.data() + header.uDataOffset, onePacket.data() + sizeof(packageHeader), header.uPackageDataSize);
                if(recvPacketCount == packetAmount)
                {
                    recvPacketCount = 0;
                    dataTotalSize = 0;
                    break;
                }
            }
        }
        QImage image = QImage::fromData(imageCache);
        emit gotOneFrameData(image);
    }

}
