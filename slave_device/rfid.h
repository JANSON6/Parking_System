#ifndef RFID_API_H
#define RFID_API_H

extern "C"
{
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include<termios.h> //引入Linux下的串口头文件
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

};

#include <QTimer>



class RFID : public QObject
{
    Q_OBJECT
public:
    RFID(const char* USART1);

    int init_tty(int fd);

    unsigned char calc_BCC(unsigned char*SerBfr,int len);

    int PiccRequest(int fd);

    int PiccAnticoll(int fd ,unsigned int *id);   /** 设置了一个传出参数 */

    void readCardId();

    void start();
    void stop();

    unsigned int id() const;    //获取识别的id

public slots:

signals:
    void recognitionSuccess(unsigned int id);
    void recognitionFailed();


private:
    const char* m_USART1;
    QTimer *m_timer;

    unsigned int m_id;  /* 识别的卡号 */
};

#endif // RFID_API_H
