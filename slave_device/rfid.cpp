#include "rfid.h"

RFID::RFID(const char* USART1)
{
    m_USART1 = USART1;

    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, this, &RFID::readCardId);
}

int RFID::init_tty(int fd)
{
    //1.定义串口结构体
    struct termios old_cfg,new_cfg;
    //2.得到当前串口的属性配置
    //extern int tcgetattr (int __fd, struct termios *__termios_p) __THROW;
    if(tcgetattr(fd,&old_cfg) != 0)
    {
        perror("tcgetattr");
        return -1;
    }
    //3.设置串口的通信模式
    //extern void cfmakeraw (struct termios *__termios_p)
    //将新的配置属性清零
    bzero(&new_cfg,sizeof(new_cfg));
    new_cfg = old_cfg;
    cfmakeraw(&new_cfg);

    //4.配置串口波特度 数据位 停止位 奇偶校验位
    cfsetispeed(&new_cfg, B9600);
    cfsetospeed(&new_cfg, B9600);
    //激活选项：CLOCAL CREAD
    new_cfg.c_cflag  |=  CLOCAL | CREAD;
    //设置数据位：8位，需使用掩码设置
    new_cfg.c_cflag &= ~CSIZE; /* 用数据位掩码清空数据位设置 */
    new_cfg.c_cflag |= CS8;
    //设置无校验
    new_cfg.c_cflag &= ~PARENB;
    //设置停止位
    new_cfg.c_cflag &=  ~CSTOPB;

    //清空缓冲区
    new_cfg.c_cc[VTIME]  = 0;
    new_cfg.c_cc[VMIN] = 4;
    tcflush(fd,TCIFLUSH);     // 清空fd
    //5 将配置好的串口设置到系统中
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
    //1.定义数据帧
    unsigned char wbuf[128],rbuf[128];
    fd_set rfds;
    struct timeval tv;
    int retval;

    memset(wbuf,0,128);
    memset(rbuf,0,128);
    //2.将命令嵌入到数据帧数组中
    //主机命令
    wbuf[0] = 0x07; //数据帧长度：0x07
    wbuf[1] = 0x02; //命令类型为ISO14443A类型
    wbuf[2] = 0x41; //具体的命令为'A' --- 65 4*16+1
    wbuf[3] = 0x01; //信息长度:1B
    wbuf[4] = 0x52;
    wbuf[5] = calc_BCC(wbuf,wbuf[0]-2);
    wbuf[6] = 0x03; //结束符
    //3 将数据帧写入到串口
    write(fd,wbuf,7); //写入具体的数据大小

    //[4] 等待模块的响应 ---- io复用来监听--select

     #if 0
       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
    #endif
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = 300; //5min
    tv.tv_usec = 0;
    //开启监听
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
          //监听成功，读取从机响应的数据
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
    //1.定义数据帧
    unsigned char wbuf[128],rbuf[128];
    fd_set rfds;
    struct timeval tv;
    int retval;
    unsigned int ID;
    memset(wbuf,0,128);
    memset(rbuf,0,128);
    //2.将命令嵌入到数据帧数组中
    //主机命令
    wbuf[0] = 0x08; //数据帧长度：0x08
    wbuf[1] = 0x02; //命令类型为ISO14443A类型
    wbuf[2] = 0x42; //具体的命令为'B'
    wbuf[3] = 0x02;//信息长度:2B
    wbuf[4] = 0x93;
    wbuf[5] = 0x00;
    wbuf[6] = calc_BCC(wbuf,wbuf[0]-2);
    wbuf[7] = 0x03; //结束符
    //3 将数据帧写入到串口
    write(fd,wbuf,8); //写入具体的数据大小

    //[4] 等待模块的响应 ---- io复用来监听--select

     #if 0
       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
    #endif
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = 300; //5min
    tv.tv_usec = 0;
    //开启监听
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
          //监听成功，读取从机响应的数据
          retval = read(fd,rbuf,10);
          if(retval < 0)
          {
              printf("PiccAnticoll = %d\n",retval);
          }
          if(rbuf[2] == 0x00)
          {
              //printf("PiccAnticoll sucess\n");

              //卡的ID是存放在rbuf[4] rbuf[5] rbuf[6] rbuf[7]
              //如果想要把四个字节一起输出，则将这四个字节拼接成一个int
              ID = rbuf[7]<<24 | rbuf[6]<<16 | rbuf[5]<<8 | rbuf[4];
              *id = ID;  /** 传出参数 */
              //printf("PiccAnticoll the card id = %x\n",ID);
              return 0;
          }
    }
     FD_CLR(fd, &rfds);
     return 1;
}

void RFID::readCardId() /** id为获取的IC卡编号 */
{
    m_id = 0;       //重置识别的卡号
    //1. 打开串口
    int fd = open(m_USART1,O_RDWR);
    if(-1 == fd)
    {
        perror("串口打开失败");
        return;
    }
    //2. 初始化串口
    init_tty(fd);

    //3.根据开发文档中的访问设备的流程---读ID（请求-A 防碰撞--B）
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
        //识别成功发射 《识别成功》 信号
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





