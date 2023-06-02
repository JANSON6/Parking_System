#include "network.h"

NetWork::NetWork(QObject *parent) : QObject(parent)
{
    m_tcpSocket = new QTcpSocket;
    connect(m_tcpSocket, &QTcpSocket::connected, this, &NetWork::on_connectSucceed);
    m_tcpSocket->connectToHost(HOSTADDR, PORT);

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &NetWork::on_recvData);



}

void NetWork::on_connectSucceed()
{
    qDebug()<<"tcp connect to host succeed!";
}

void NetWork::on_recvData()
{
    QByteArray cache;
    cache = m_tcpSocket->readAll();

    Packet header;
    memcpy(&header, cache.data(), sizeof(Packet));

    switch(header.type)
    {
        case T_CARINFO:
            {
                int type = cache.mid(sizeof(Packet), header.memberSize[0]).toInt();

                QByteArray plate;
                plate.resize(header.memberSize[1]);
                memcpy(plate.data(), cache.data() + sizeof(Packet) + header.memberSize[0],  header.memberSize[1]);

                QString text = QString::fromUtf8(plate);

                emit gotCarType(type);
                emit gotCarPlate(text);
            }
    }
}

void NetWork::on_sendCardId(unsigned int id)
{
    //数据包缓存
    QByteArray cache;

    //包头
    Packet header;
    header.type = T_RFID;
    header.memberSize[0] = sizeof(id);
    header.memberSize[1] = 0;
    header.memberSize[2] = 0;
    header.memberSize[3] = 0;
    header.memberSize[4] = 0;
    header.memberCount = 1;

    int size = sizeof(Packet) + sizeof(id);

    cache.resize(size);

    memcpy(cache.data(), &header, sizeof(header));
    memcpy(cache.data() + sizeof(header), &id, header.memberSize[0]);

    m_tcpSocket->write(cache);
}
