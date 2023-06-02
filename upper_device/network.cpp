#include "network.h"

NetWork::NetWork(QObject *parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer;
    m_tcpServer->listen(QHostAddress::Any, PORT);

    connect(m_tcpServer, &QTcpServer::newConnection, this, &NetWork::on_haveConnected);
}

void NetWork::on_recvData()
{
    //���ݻ�����
    QByteArray cache;
    cache = m_tcpSocket->readAll();

    //��ȡ��ͷ
    Packet header;
    memcpy(&header, cache.data(), sizeof(header));

    //ͨ�������Ͱ����ݷ��͵���Ӧģ��Ĳۺ����д���
    switch(header.type)
    {
        case T_RFID:    //RFIDʶ��ģ��
            {
                unsigned int id  = 0;
                memcpy(&id, cache.data() + sizeof(header), header.memberSize[0]);
                emit gotIcCardId(id);
            }
    }
}

void NetWork::on_haveConnected()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection(); //��ȡ���ӵ��׽���
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &NetWork::on_recvData);
}

//���������ͷ�������λ��������ʾ
/** ֵ��ע����ǳ�������һ�������ַ������ת��utf-8�������ַ�����3���ֽڵ��º�����ַ��ضϣ���������Ҫ������ת��utf-8��������ȥת��char*���� */
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


