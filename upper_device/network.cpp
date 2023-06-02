#include "network.h"

NetWork::NetWork(QObject *parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer;
    m_tcpServer->listen(QHostAddress::Any, PORT);

    connect(m_tcpServer, &QTcpServer::newConnection, this, &NetWork::on_haveConnected);
}

void NetWork::on_recvData()
{
    //数据缓存区
    QByteArray cache;
    cache = m_tcpSocket->readAll();

    //获取包头
    Packet header;
    memcpy(&header, cache.data(), sizeof(header));

    //通过包类型把数据发送到对应模块的槽函数中处理
    switch(header.type)
    {
        case T_RFID:    //RFID识别模块
            {
                unsigned int id  = 0;
                memcpy(&id, cache.data() + sizeof(header), header.memberSize[0]);
                emit gotIcCardId(id);
            }
    }
}

void NetWork::on_haveConnected()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection(); //获取连接的套接字
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &NetWork::on_recvData);
}

//将车辆类型发送至下位机用来显示
/** 值得注意的是车牌中有一个中文字符，如果转成utf-8该中文字符会变成3个字节导致后面的字符截断，所以我们要把它先转成utf-8的数组在去转成char*类型 */
void NetWork::on_sendCarInfo(int type, QString carPlate)
{
    QByteArray cache;

    Packet header;
    header.type = T_CARINFO;
    header.memberSize[0] = sizeof(type);
    header.memberSize[1] = carPlate.toUtf8().size();
    header.memberSize[2] = 0;
    header.memberSize[3] = 0;
    header.memberSize[4] = 0;
    header.memberCount = 2;

    cache.resize(sizeof(Packet) + header.memberSize[0] + header.memberSize[1]);

    memcpy(cache.data(), &header, sizeof(Packet));
    memcpy(cache.data() + sizeof(Packet), &type, header.memberSize[0]);
    memcpy(cache.data() + sizeof(Packet) + header.memberSize[0], carPlate.toUtf8().data(), header.memberSize[1]);

    m_tcpSocket->write(cache);
}


