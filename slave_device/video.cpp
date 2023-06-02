#include "video.h"
#include <QDebug>
#include <unistd.h>
Video::Video()
{
    //UDP
    m_udpSocket = new QUdpSocket();

}

void Video::on_readCommand()
{

}

void Video::on_sendCaptureStream(QImage image)
{
    //把image的数据保存在一个QByteArray中
    QByteArray imageArr;
    QBuffer buffer(&imageArr);
    buffer.open(QIODevice::ReadWrite);
    image.save(&buffer, "BMP");         //如果没有指定图片格式，会导致buffer为空。

    /* 初始化包头 */
    int packageAmount = imageArr.size() / PACKAGEDATASIZE;
    int packageSize = sizeof(packageHeader) + PACKAGEDATASIZE;
    int lastDataSize = imageArr.size() % PACKAGEDATASIZE;

    if(lastDataSize != 0)
    {
        packageAmount += 1;   //不能取整数量+1
    }
    //前n个包头
    packageHeader header;
    header.dataTotalSize = imageArr.size();
    header.packageAmount = packageAmount;
    header.uPackageTotalSize = packageSize;
    header.uPackageDataSize = PACKAGEDATASIZE;

    //最后一个包头
    packageHeader lastHeader;
    if(lastDataSize == 0)   //没有余下的字节
    {
        lastHeader.dataTotalSize = header.dataTotalSize;
        lastHeader.packageAmount = packageAmount;
        lastHeader.uPackageTotalSize = header.uPackageTotalSize;
        lastHeader.uPackageIndex = packageAmount - 1;
        lastHeader.uPackageDataSize = PACKAGEDATASIZE;
        lastHeader.uDataOffset = imageArr.size() - PACKAGEDATASIZE - 1;
    }
    else        //有余下的字节
    {
        lastHeader.dataTotalSize = header.dataTotalSize;
        lastHeader.packageAmount = packageAmount;
        lastHeader.uPackageTotalSize = sizeof(packageHeader) + lastDataSize;
        lastHeader.uPackageIndex = packageAmount - 1;
        lastHeader.uPackageDataSize = lastDataSize;
        lastHeader.uDataOffset = imageArr.size() - lastDataSize - 1;
    }

    QByteArray sendData(header.uPackageTotalSize, 0);

    //发送前n个包
    for(int i=0; i < packageAmount - 1; i++)
    {
//        sendData.fill(0);
        char *p = sendData.data();

        header.uPackageIndex = i;
        header.uDataOffset = i * PACKAGEDATASIZE;

        memcpy(p, &header, sizeof (packageHeader));
        memcpy(p + sizeof (packageHeader), imageArr.mid(header.uDataOffset, PACKAGEDATASIZE), PACKAGEDATASIZE);

        m_udpSocket->writeDatagram(sendData, QHostAddress(HOSTADDR), PORT); /** 发送数据 */
    }

    //发送最后一个包
    sendData.resize(lastHeader.uPackageTotalSize);
    char *p = sendData.data();
    memcpy(p, &lastHeader, 24);
    memcpy(p + sizeof(packageHeader), imageArr.mid(lastHeader.uDataOffset, lastHeader.uPackageDataSize), lastHeader.uPackageDataSize);
    m_udpSocket->writeDatagram(sendData, QHostAddress(HOSTADDR), PORT);

}
