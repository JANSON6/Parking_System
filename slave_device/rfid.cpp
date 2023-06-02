#include "rfid.h"

RFID::RFID(const char* USART1)
{
    m_USART1 = USART1;

    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, this, &RFID::readCardId);
}

int RFID::init_tty(int fd)
{
    //1.���崮�ڽṹ��
    struct termios old_cfg,new_cfg;
    //2.�õ���ǰ���ڵ���������
    //extern int tcgetattr (int __fd, struct termios *__termios_p) __THROW;
    if(tcgetattr(fd,&old_cfg) != 0)
    {
        perror("tcgetattr");
        return -1;
    }
    //3.���ô��ڵ�ͨ��ģʽ
    //extern void cfmakeraw (struct termios *__termios_p)
    //���µ�������������
    bzero(&new_cfg,sizeof(new_cfg));
    new_cfg = old_cfg;
    cfmakeraw(&new_cfg);

    //4.���ô��ڲ��ض� ����λ ֹͣλ ��żУ��λ
    cfsetispeed(&new_cfg, B9600);
    cfsetospeed(&new_cfg, B9600);
    //����ѡ�CLOCAL CREAD
    new_cfg.c_cflag  |=  CLOCAL | CREAD;
    //��������λ��8λ����ʹ����������
    new_cfg.c_cflag &= ~CSIZE; /* ������λ�����������λ���� */
    new_cfg.c_cflag |= CS8;
    //������У��
    new_cfg.c_cflag &= ~PARENB;
    //����ֹͣλ
    new_cfg.c_cflag &=  ~CSTOPB;

    //��ջ�����
    new_cfg.c_cc[VTIME]  = 0;
    new_cfg.c_cc[VMIN] = 4;
    tcflush(fd,TCIFLUSH);     // ���fd
    //5 �����úõĴ������õ�ϵͳ��
    if ((tcsetattr(fd, TCSANOW, &new_cfg)) != 0)
    {
    perror("tcsetattr");
    return -1;
    }
    return 0;
}

unsigned char RFID::calc_BCC(unsigned char *SerBfr, int len)
{
    unsigned char BCC = 0;
    int i;
    for(i=0; i<(SerBfr[0]-2); i++) {
    BCC ^= SerBfr[i];
    }
    return ~BCC;
}


int RFID::PiccRequest(int fd)
{
    //1.��������֡
    unsigned char wbuf[128],rbuf[128];
    fd_set rfds;
    struct timeval tv;
    int retval;

    memset(wbuf,0,128);
    memset(rbuf,0,128);
    //2.������Ƕ�뵽����֡������
    //��������
    wbuf[0] = 0x07; //����֡���ȣ�0x07
    wbuf[1] = 0x02; //��������ΪISO14443A����
    wbuf[2] = 0x41; //���������Ϊ'A' --- 65 4*16+1
    wbuf[3] = 0x01; //��Ϣ����:1B
    wbuf[4] = 0x52;
    wbuf[5] = calc_BCC(wbuf,wbuf[0]-2);
    wbuf[6] = 0x03; //������
    //3 ������֡д�뵽����
    write(fd,wbuf,7); //д���������ݴ�С

    //[4] �ȴ�ģ�����Ӧ ---- io����������--select

     #if 0
       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
    #endif
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = 300; //5min
    tv.tv_usec = 0;
    //��������
    retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    switch(retval)
    {
        case -1:
        perror("PiccRequest");
        break;
        case 0:
        printf("timeout\n");
        break;
        default:
          //�����ɹ�����ȡ�ӻ���Ӧ������
          retval = read(fd,rbuf,8);
          if(retval < 0)
          {
              printf("PiccRequest = %d\n",retval);
          }
          if(rbuf[2] == 0x00)
          {
              //printf("PiccRequest sucess\n");
              return 0;
          }
    }
     FD_CLR(fd, &rfds);

    return 1;

}

int RFID::PiccAnticoll(int fd, unsigned int *id)
{
    //1.��������֡
    unsigned char wbuf[128],rbuf[128];
    fd_set rfds;
    struct timeval tv;
    int retval;
    unsigned int ID;
    memset(wbuf,0,128);
    memset(rbuf,0,128);
    //2.������Ƕ�뵽����֡������
    //��������
    wbuf[0] = 0x08; //����֡���ȣ�0x08
    wbuf[1] = 0x02; //��������ΪISO14443A����
    wbuf[2] = 0x42; //���������Ϊ'B'
    wbuf[3] = 0x02;//��Ϣ����:2B
    wbuf[4] = 0x93;
    wbuf[5] = 0x00;
    wbuf[6] = calc_BCC(wbuf,wbuf[0]-2);
    wbuf[7] = 0x03; //������
    //3 ������֡д�뵽����
    write(fd,wbuf,8); //д���������ݴ�С

    //[4] �ȴ�ģ�����Ӧ ---- io����������--select

     #if 0
       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
    #endif
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = 300; //5min
    tv.tv_usec = 0;
    //��������
    retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    switch(retval)
    {
        case -1:
        perror("PiccAnticoll");
        break;
        case 0:
        printf("timeout\n");
        break;
        default:
          //�����ɹ�����ȡ�ӻ���Ӧ������
          retval = read(fd,rbuf,10);
          if(retval < 0)
          {
              printf("PiccAnticoll = %d\n",retval);
          }
          if(rbuf[2] == 0x00)
          {
              //printf("PiccAnticoll sucess\n");

              //����ID�Ǵ����rbuf[4] rbuf[5] rbuf[6] rbuf[7]
              //�����Ҫ���ĸ��ֽ�һ������������ĸ��ֽ�ƴ�ӳ�һ��int
              ID = rbuf[7]<<24 | rbuf[6]<<16 | rbuf[5]<<8 | rbuf[4];
              *id = ID;  /** �������� */
              //printf("PiccAnticoll the card id = %x\n",ID);
              return 0;
          }
    }
     FD_CLR(fd, &rfds);
     return 1;
}

void RFID::readCardId() /** idΪ��ȡ��IC����� */
{
    m_id = 0;       //����ʶ��Ŀ���
    //1. �򿪴���
    int fd = open(m_USART1,O_RDWR);
    if(-1 == fd)
    {
        perror("���ڴ�ʧ��");
        return;
    }
    //2. ��ʼ������
    init_tty(fd);

    //3.���ݿ����ĵ��еķ����豸������---��ID������-A ����ײ--B��
    if(PiccRequest(fd))
    {
        //printf("PiccRequest falied\n");
        //return;
    }
    if(PiccAnticoll(fd, &m_id))
    {
        //printf("PiccAnticoll falied\n");
        //return;
    }


    if(m_id != 0)
    {
        //ʶ��ɹ����� ��ʶ��ɹ��� �ź�
        emit recognitionSuccess(m_id);
    }
    else
    {
        emit recognitionFailed();
    }
}

void RFID::start()
{
    m_timer->start(1000);
}

void RFID::stop()
{
    m_timer->stop();
}

unsigned int RFID::id() const
{
    return m_id;
}





