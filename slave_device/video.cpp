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
    //��image�����ݱ�����һ��QByteArray��
    QByteArray imageArr;
    QBuffer buffer(&imageArr);
    buffer.open(QIODevice::ReadWrite);
    image.save(&buffer, "BMP");         //���û��ָ��ͼƬ��ʽ���ᵼ��bufferΪ�ա�

    /* ��ʼ����ͷ */
    int packageAmount = imageArr.size() / PACKAGEDATASIZE;
    int packageSize = sizeof(packageHeader) + PACKAGEDATASIZE;
    int lastDataSize = imageArr.size() % PACKAGEDATASIZE;

    if(lastDataSize != 0)
    {
        packageAmount += 1;   //����ȡ������+1
    }
    //ǰn����ͷ
    packageHeader header;
    header.dataTotalSize = imageArr.size();
    header.packageAmount = packageAmount;
    header.uPackageTotalSize = packageSize;
    header.uPackageDataSize = PACKAGEDATASIZE;

    //���һ����ͷ
    packageHeader lastHeader;
    if(lastDataSize == 0)   //û�����µ��ֽ�
    {
        lastHeader.dataTotalSize = header.dataTotalSize;
        lastHeader.packageAmount = packageAmount;
        lastHeader.uPackageTotalSize = header.uPackageTotalSize;
        lastHeader.uPackageIndex = packageAmount - 1;
        lastHeader.uPackageDataSize = PACKAGEDATASIZE;
        lastHeader.uDataOffset = imageArr.size() - PACKAGEDATASIZE - 1;
    }
    else        //�����µ��ֽ�
    {
        lastHeader.dataTotalSize = header.dataTotalSize;
        lastHeader.packageAmount = packageAmount;
        lastHeader.uPackageTotalSize = sizeof(packageHeader) + lastDataSize;
        lastHeader.uPackageIndex = packageAmount - 1;
        lastHeader.uPackageDataSize = lastDataSize;
        lastHeader.uDataOffset = imageArr.size() - lastDataSize - 1;
    }

    QByteArray sendData(header.uPackageTotalSize, 0);

    //����ǰn����
    for(int i=0; i < packageAmount - 1; i++)
    {
//        sendData.fill(0);
        char *p = sendData.data();

        header.uPackageIndex = i;
        header.uDataOffset = i * PACKAGEDATASIZE;

        memcpy(p, &header, sizeof (packageHeader));
        memcpy(p + sizeof (packageHeader), imageArr.mid(header.uDataOffset, PACKAGEDATASIZE), PACKAGEDATASIZE);

        m_udpSocket->writeDatagram(sendData, QHostAddress(HOSTADDR), PORT); /** �������� */
    }

    //�������һ����
    sendData.resize(lastHeader.uPackageTotalSize);
    char *p = sendData.data();
    memcpy(p, &lastHeader, 24);
    memcpy(p + sizeof(packageHeader), imageArr.mid(lastHeader.uDataOffset, lastHeader.uPackageDataSize), lastHeader.uPackageDataSize);
    m_udpSocket->writeDatagram(sendData, QHostAddress(HOSTADDR), PORT);

}
