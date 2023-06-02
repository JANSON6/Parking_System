#ifndef CAMERA_THREAD_H
#define CAMERA_THREAD_H

extern "C"
{
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>
#include <linux/videodev2.h>  //linuxϵͳ������ͷV4l2���ͷ�ļ�
};

#include <QWidget>
#include <QTimer>

#define DEVICE_PATH "/dev/video7"


class Camera:public QWidget
{
    Q_OBJECT

public:
    Camera(const char* devicePath);   //��ʼ������ͷ���ں˿ռ�j�����û��ռ�

    void    start();                         //����ͷ��ʼ����ͼ��
    void     stop();                         //����ͷֹͣ����ͼ��
    void  destory();                         //�����û��ռ���ں˿ռ�j��
    void getFrame();                         //ȡ֡
    void yuv422ToRgb24(unsigned char *buffer, unsigned char *rgbdata, int w, int h);    //yuv��ʽͼ������ת����rgb��ʽ

    int yuyv2rgb(int y, int u, int v);
    void yuyv2rgb0(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

    void getRGBFrame();                      //��ȡrgb��ʽ֡���ᷢ���ź�

signals:
    void gotOneRGBFrameData(QImage image);       /* �ػ���һ֡ͼ�����ݵ�ʱ�򷢳� */

private:
    int             m_fd;                  /* ����ͷ�����豸������ */
    unsigned char*  m_mptr[4];             /* ͼ�񻺳�j���׵�ַ���� */
    unsigned int    m_size[4];             /* ͼ�񻺳�����С���� */

    unsigned char m_rgb[640*480*3];

    QTimer*         m_timer;                /* ��ʱ�� */
};

#endif // CAMERA_THREAD_H




