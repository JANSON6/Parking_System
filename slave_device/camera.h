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
#include <linux/videodev2.h>  //linux系统的摄像头V4l2框架头文件
};

#include <QWidget>
#include <QTimer>

#define DEVICE_PATH "/dev/video7"


class Camera:public QWidget
{
    Q_OBJECT

public:
    Camera(const char* devicePath);   //初始化摄像头，内核空间眏射至用户空间

    void    start();                         //摄像头开始捕获图像
    void     stop();                         //摄像头停止捕获图像
    void  destory();                         //销毁用户空间的内核空间眏射
    void getFrame();                         //取帧
    void yuv422ToRgb24(unsigned char *buffer, unsigned char *rgbdata, int w, int h);    //yuv格式图像数据转换成rgb格式

    int yuyv2rgb(int y, int u, int v);
    void yuyv2rgb0(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

    void getRGBFrame();                      //获取rgb格式帧，会发送信号

signals:
    void gotOneRGBFrameData(QImage image);       /* 截获了一帧图像数据的时候发出 */

private:
    int             m_fd;                  /* 摄像头驱动设备描述符 */
    unsigned char*  m_mptr[4];             /* 图像缓冲眏射首地址数组 */
    unsigned int    m_size[4];             /* 图像缓冲区大小数组 */

    unsigned char m_rgb[640*480*3];

    QTimer*         m_timer;                /* 计时器 */
};

#endif // CAMERA_THREAD_H




