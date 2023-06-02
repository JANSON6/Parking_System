#include "camera.h"
#include <QDebug>

Camera::Camera(const char* devicePath)
{
    memset(m_rgb, 0, 640*480*3);

    m_timer = new QTimer;
    m_timer->setInterval(100);   //设置定时器精度
    connect(m_timer, &QTimer::timeout, this, &Camera::getRGBFrame);

    /** 1 - 打开摄像头设备驱动文件 */
    m_fd = open(devicePath, O_RDWR);
    if(m_fd < 0)
    {
        perror("open error");
        return;
    }

    /** 2 - 设置 摄像头采集 数据格式 */
    struct v4l2_format vfmt;
    vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    vfmt.fmt.pix.width = 640;
    vfmt.fmt.pix.height = 480;
    vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

    //设置摄像头采集数据格式
    if(ioctl(m_fd, VIDIOC_S_FMT, &vfmt) < 0)
    {
        perror("VIDIOC_S_FMT error!");
        return;
    }

    memset(&vfmt, 0, sizeof(vfmt));
    vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    //获取摄像头采集数据格式
    if(ioctl(m_fd, VIDIOC_G_FMT, &vfmt) < 0)
    {
        perror("VIDIOC_G_FMT error1!");
        return;
    }

    if(vfmt.fmt.pix.width == 640 && vfmt.fmt.pix.height == 480 && vfmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV)
    {
        printf("VIDIOC_G_FMT succeed!\n");
    }
    else
    {
        printf("VIDIOC_G_FMT error2!\n");
        fprintf(stderr, "VIDIOC_G_FMT error: %s\n", strerror(errno));
    }

    /** 3 - 申请内核空间 用于摄像头采集图像数据 */
    struct v4l2_requestbuffers reqbuffers;
    reqbuffers.count = 4;
    reqbuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuffers.memory = V4L2_MEMORY_MMAP;

    //向驱动程序请求缓冲区
    if(ioctl(m_fd, VIDIOC_REQBUFS, &reqbuffers) < 0)
    {
        perror("VIDIOC_REQBUFS error!");
        return;
    }

    /** 4 - 将申请好的缓冲区队列眏射到用户空间 */
    struct v4l2_buffer mapbuffer;
    mapbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    for(int i=0; i<4; i++)
    {
        mapbuffer.index = i;

        //获取帧缓冲区的相关信息
        if(ioctl(m_fd, VIDIOC_QUERYBUF, &mapbuffer) < 0)
        {
            perror("VIDIOC_QUERYBUF error!");
            return;
        }

        m_mptr[i] = (unsigned char*)mmap(NULL, mapbuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, mapbuffer.m.offset);
        m_size[i] = mapbuffer.length;

        //将帧数据放入驱动程序队列中
        if(ioctl(m_fd, VIDIOC_QBUF, &mapbuffer) < 0)
        {
            perror("VIDIOC_QBUF error!");
            return;
        }
    }

}

void Camera::start()
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    //摄像头捕获流启动
    if(ioctl(m_fd, VIDIOC_STREAMON, &type) < 0)
    {
        perror("VIDIOC_STREAMON error!");
        return;
    }

    m_timer->start();
}

void Camera::stop()
{
    m_timer->stop();

    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    //摄像头捕获流停止
    if(ioctl(m_fd, VIDIOC_STREAMOFF, &type) < 0)
    {
        perror("VIDIOC_STREAMOFF error!");
        return;
    }
}

void Camera::destory()
{
    for(int i=0; i < 4; i++)
    {
        munmap(m_mptr[i], m_size[i]);
    }
    ::close(m_fd);
}

/** 该算法需要执行所有的像素，运行速率很低 */
void Camera::yuv422ToRgb24(unsigned char *buffer, unsigned char *rgbdata, int w, int h)
{
    int r1, g1, b1;
    int r2, g2, b2;

    for(int i=0; i<w*h/2; i++)
    {
        char data[4];
        memcpy(data, buffer+i*4, 4);
        //Y0U0Y1V1  -->[Y0 U0 V1] [Y1 U0 V1]
        unsigned char Y0=data[0];
        unsigned char U0=data[1];
        unsigned char Y1=data[2];
        unsigned char V1=data[3];

        r1 = Y0+1.4075*(V1-128); if(r1>255)r1=255; if(r1<0)r1=0;
        g1 =Y0- 0.3455 * (U0-128) - 0.7169*(V1-128); if(g1>255)g1=255; if(g1<0)g1=0;
        b1 = Y0 + 1.779 * (U0-128);  if(b1>255)b1=255; if(b1<0)b1=0;
        r2 = Y1+1.4075*(V1-128);if(r2>255)r2=255; if(r2<0)r2=0;
        g2 = Y1- 0.3455 * (U0-128) - 0.7169*(V1-128); if(g2>255)g2=255; if(g2<0)g2=0;
        b2 = Y1 + 1.779 * (U0-128);  if(b2>255)b2=255; if(b2<0)b2=0;

        rgbdata[i*6+0]=r1;
        rgbdata[i*6+1]=g1;
        rgbdata[i*6+2]=b1;
        rgbdata[i*6+3]=r2;
        rgbdata[i*6+4]=g2;
        rgbdata[i*6+5]=b2;
    }
}

/**                                                                                               */
int Camera::yuyv2rgb(int y, int u, int v)
{
    unsigned int pixel24 = 0;
    unsigned char* pixel = (unsigned char*)&pixel24;
    int r, g, b;
    static int ruv, guv, buv;

    // 色度
    ruv = 1596 * (v - 128);
    guv = 391 * (u - 128) + 813 * (v - 128);
    buv = 2018 * (u - 128);

    // RGB
    r = (1164 * (y - 16) + ruv) / 1000;
    g = (1164 * (y - 16) - guv) / 1000;
    b = (1164 * (y - 16) + buv) / 1000;

    if (r > 255)
        r = 255;
    if (g > 255)
        g = 255;
    if (b > 255)
        b = 255;
    if (r < 0)
        r = 0;
    if (g < 0)
        g = 0;
    if (b < 0)
        b = 0;

    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;

    return pixel24;
}

void Camera::yuyv2rgb0(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
    unsigned int in, out;
    int y0, u, y1, v;
    unsigned int pixel24;
    unsigned char* pixel = (unsigned char*)&pixel24;
    unsigned int size = width * height * 2;

    for (in = 0, out = 0; in < size; in += 4, out += 6) {
        // YUYV
        y0 = yuv[in + 0];
        u = yuv[in + 1];
        y1 = yuv[in + 2];
        v = yuv[in + 3];

        pixel24 = yuyv2rgb(y0, u, v); // RGB
        rgb[out + 0] = pixel[0];
        rgb[out + 1] = pixel[1];
        rgb[out + 2] = pixel[2];

        pixel24 = yuyv2rgb(y1, u, v); // RGB
        rgb[out + 3] = pixel[0];
        rgb[out + 4] = pixel[1];
        rgb[out + 5] = pixel[2];
    }
}
/**                                                                                               */


void Camera::getFrame()
{
    struct v4l2_buffer readbuffer;
    readbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(ioctl(m_fd, VIDIOC_DQBUF, &readbuffer) < 0)
    {
        perror("VIDIOC_DQBUF error!");
        return;
    }

    //直接获取眏射的数据改成rgb
    yuyv2rgb0((unsigned char*)m_mptr[readbuffer.index], m_rgb, 640, 480);

    if(ioctl(m_fd, VIDIOC_QBUF, &readbuffer) < 0)
    {
        perror("VIDIOC_QBUF error!");
        return;
    }
}

void Camera::getRGBFrame()
{
    getFrame();
    QImage image(m_rgb, 640, 480, QImage::Format_RGB888);

    emit gotOneRGBFrameData(image);
}
