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
    //��ء����š�ʣ�೵λ
    void on_showImage(QImage image);
    void on_showCardId(unsigned int id);
    void on_showRemainStall();
    //������ʶ��
    void on_showCapture();
    void on_recognizingCarPlates();
    //ϵͳʱ��
    void on_runTime();
    //���������¼�� �ؼ�
    void on_insertRowData_t1(QString cardId, QString carPlate, qint64 time);
    void on_supplementRowData_t1(QString cardId, double cost);
    //�¿�����¼�� �ؼ�
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
    int m_remainStall;      //ʣ�೵λ
    QTimer* m_timer;        //��ʱ��
    qint64 m_time;          //ʱ��
    QString m_cardId;       //����
    QString m_carPlate;     //���ƺ�
    int     m_carType;      //��������
    Video*   m_video;
    NetWork* m_network;
    bool m_captureFlag;       //������־λ
    QImage m_carPlateImage;   //���ƽ�ͼ
    DataManager* m_dataManager;
};

#endif // MAINWINDOW_H
