#include "video.h"

Video::Video()
{

}

void Video::run()
{
    QUdpSocket udpSocket;

    if(udpSocket.bind(QHostAddress("192.168.6.54"), 50000))    //本地绑定
    {
        qDebug()<<"bind succeed!";
    }

    QByteArray imageCache;    /* 一帧图像总缓存 */
    int packetAmount = 0;     /* 一帧分包总数   */
    int dataTotalSize = 0;    /* 一帧数据总字节 */
    int recvPacketCount = 0;  /* 接收的分包数   */

    while(1)
    {
        udpSocket.waitForReadyRead(-1);     //没有数据的时候阻塞

        while(1)
        {
            if(udpSocket.hasPendingDatagrams())
            {

                QByteArray onePacket;   /* 一个分包接收缓存 */
                onePacket.resize(udpSocket.pendingDatagramSize());   //重置一个分包缓存大小
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
