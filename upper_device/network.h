#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#define PORT     50000

class NetWork : public QObject
{
    Q_OBJECT
public:
    //数据包
    struct Packet
    {
        int type;
        int memberCount;
        int memberSize[5];
    };

    //数据包类型
    enum PacketType
    {
        T_RFID = 0,
        T_CARINFO = 1
    };

public:
    explicit NetWork(QObject *parent = nullptr);

public slots:
    void on_recvData();
    void on_haveConnected();
    void on_sendCarInfo(int type, QString carPlate);

signals:
    void gotIcCardId(unsigned int id);

private:
    QTcpServer* m_tcpServer;
    QTcpSocket* m_tcpSocket;
};

#endif // NETWORK_H
