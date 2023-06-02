#ifndef RFID_API_H
#define RFID_API_H

extern "C"
{
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include<termios.h> //����Linux�µĴ���ͷ�ļ�
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

    int PiccAnticoll(int fd ,unsigned int *id);   /** ������һ���������� */

    void readCardId();

    void start();
    void stop();

    unsigned int id() const;    //��ȡʶ���id

public slots:

signals:
    void recognitionSuccess(unsigned int id);
    void recognitionFailed();


private:
    const char* m_USART1;
    QTimer *m_timer;

    unsigned int m_id;  /* ʶ��Ŀ��� */
};

#endif // RFID_API_H
