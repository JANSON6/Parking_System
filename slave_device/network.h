#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpSocket>

#define HOSTADDR "192.168.6.54"
#define PORT      50000

class NetWork: public QObject
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
    void on_connectSucceed();
    void on_recvData();

    void on_sendCardId(unsigned int id);

signals:
    void gotCarType(int type);
    void gotCarPlate(QString carPlate);

private:
    QTcpSocket* m_tcpSocket;

};

#endif // NETWORK_H
