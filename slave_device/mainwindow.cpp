#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_camera = new Camera("/dev/video7") ;
    m_rfid = new RFID("/dev/ttySAC1");
    m_video = new Video;

    //ÍøÂç´«ÊäÄ£¿é
    m_network = new NetWork;
    connect(m_network, &NetWork::gotCarType, this, &MainWindow::on_showCarType);
    connect(m_network, &NetWork::gotCarPlate, this, &MainWindow::on_showCarPlate);

    //ÉãÏñÍ·Ä£¿é
    connect(m_camera, &Camera::gotOneRGBFrameData, m_video, &Video::on_sendCaptureStream);
    connect(m_camera, &Camera::gotOneRGBFrameData, this, &MainWindow::on_showImage);

    m_camera->start();

    //RFIDÄ£¿é
    connect(m_rfid, &RFID::recognitionSuccess, this, &MainWindow::on_showId);
    connect(m_rfid, &RFID::recognitionSuccess, m_network, &NetWork::on_sendCardId);
    connect(m_rfid, &RFID::recognitionFailed, this, &MainWindow::on_showNullId);

    m_rfid->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_showImage(QImage image)
{
    ui->label_screen->setPixmap(QPixmap::fromImage(image).scaled(ui->label_screen->size()));
}

void MainWindow::on_showId(unsigned int id)
{
    ui->label_id->setText(QString("%1").arg(id, 0, 16));
}

void MainWindow::on_showNullId()
{
    ui->label_id->setText(QString::fromLocal8Bit("Put in IC card"));
}

void MainWindow::on_showCarType(int type)
{
    if(type == 1)
    {
        ui->label_type->setText(QString("Member Vehicle"));
    }
    else
    {
        ui->label_type->setText(QString("Temporary Vehicle"));
    }
}

void MainWindow::on_showCarPlate(QString plate)
{
    ui->label_carPlate->setText(plate);
}
