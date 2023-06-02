#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include "camera.h"
#include "rfid.h"
#include "video.h"
#include "network.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_showImage(QImage image);
    void on_showId(unsigned int id);
    void on_showNullId();
    void on_showCarType(int type);
    void on_showCarPlate(QString plate);

private:
    Ui::MainWindow *ui;

    Camera* m_camera;   //摄像头
    RFID* m_rfid;       //RFID识别
    Video* m_video;     //监控传输
    NetWork* m_network; //网络传输

};

#endif // MAINWINDOW_H
