#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDateTime>

#include "video.h"
#include "network.h"
#include "datamanager.h"
#include "httpbaiduapi.h"
#include "image.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initTableWidget();

public slots:
    //监控、卡号、剩余车位
    void on_showImage(QImage image);
    void on_showCardId(unsigned int id);
    void on_showRemainStall();
    //拍照与识别
    void on_showCapture();
    void on_recognizingCarPlates();
    //系统时间
    void on_runTime();
    //车辆出入记录表 控件
    void on_insertRowData_t1(QString cardId, QString carPlate, qint64 time);
    void on_supplementRowData_t1(QString cardId, double cost);
    //月卡车记录表 控件
    void on_insertRowData_t2(QString name, QString carPlate, qint64 transactTime, qint64 dueTime, QString phone);


signals:
    void remainStallChanged();
    void gotCapture();
    void gotCarInfo(int type, QString carPlate);
    void gotEnterInfo(QString cardId, QString carPlate, qint64 time);
    void gotLeaveInfo(QString cardId, qint64 time);
    void gotTransactInfo(QString name, QString carPlate, qint64 transactTime, qint64 dueTime, QString phone);

private slots:
    void on_pushButton_recogn_clicked();
    void on_pushButton_ttransact_clicked();

private:
    Ui::MainWindow *ui;
    int m_remainStall;      //剩余车位
    QTimer* m_timer;        //计时器
    qint64 m_time;          //时间
    QString m_cardId;       //卡号
    QString m_carPlate;     //车牌号
    int     m_carType;      //车辆类型
    Video*   m_video;
    NetWork* m_network;
    bool m_captureFlag;       //截屏标志位
    QImage m_carPlateImage;   //车牌截图
    DataManager* m_dataManager;
};

#endif // MAINWINDOW_H
