#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ʣ�೵λ
    m_remainStall = 1000;
    ui->label_space->setText(QString::number(m_remainStall));
    connect(this, &MainWindow::remainStallChanged, this, &MainWindow::on_showRemainStall);

    //ʱ��ģ��
    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, this, &MainWindow::on_runTime);
    m_timer->start(1000);

    //���ģ��
    m_video = new Video;
    connect(m_video, &Video::gotOneFrameData, this, &MainWindow::on_showImage);
    m_video->start();

    //����ģ��
    m_network = new NetWork;
    connect(m_network, &NetWork::gotIcCardId, this, &MainWindow::on_showCardId);
    connect(this, &MainWindow::gotCarInfo, m_network, &NetWork::on_sendCarInfo);

    //������ʶ����ģ��
    m_captureFlag = false;
    connect(this, &MainWindow::gotCapture, this, &MainWindow::on_recognizingCarPlates);
    connect(this, &MainWindow::gotCapture, this, &MainWindow::on_showCapture);

    //���ݴ���ģ��
    m_dataManager = new DataManager;
    connect(this, &MainWindow::gotEnterInfo, m_dataManager, &DataManager::on_insertAdmissionInfo);          //����볡��Ϣ -- �������ݿ�����볡���ݲۺ���
    connect(this, &MainWindow::gotEnterInfo, this, &MainWindow::on_insertRowData_t1);                       //����볡��Ϣ -- ���������Ŀ��Ϣ�ۺ���
    connect(this, &MainWindow::gotLeaveInfo, m_dataManager, &DataManager::on_replenishDepartureInfo);       //����볡��Ϣ -- �������ݿⲹ���볡���ݲۺ���
    connect(m_dataManager, &DataManager::calculateFinish, this, &MainWindow::on_supplementRowData_t1);      //��ɷ��ü��� -- �������Ŀ��Ϣ�ۺ���
    connect(this, &MainWindow::gotTransactInfo, m_dataManager, &DataManager::on_insertTransactCardInfo);    //��ð쿨��Ϣ -- �������ݿ����쿨���ݲۺ���
    connect(m_dataManager, &DataManager::transactFinish, this, &MainWindow::on_insertRowData_t2);           //��ɰ쿨���� -- ���������Ŀ��Ϣ�ۺ���
    ui->dateEdit_ttransactDate->setDate(QDate::currentDate());
    initTableWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTableWidget()
{
    /** ���������¼�� */
    QStringList tableHeader_1;
    tableHeader_1<<QString::fromLocal8Bit("����")
                 <<QString::fromLocal8Bit("����")
                 <<QString::fromLocal8Bit("�볡ʱ��")
                 <<QString::fromLocal8Bit("�볡ʱ��")
                 <<QString::fromLocal8Bit("����");

    ui->tableWidget->setColumnCount(5);             //��������Ϊ5
    ui->tableWidget->setAlternatingRowColors(true); //������ɫ��ʾ
    for(int i=0; i<ui->tableWidget->columnCount(); i++)
    {

        QTableWidgetItem *item = new QTableWidgetItem;  //������Ԫ��
        item->setText(tableHeader_1[i]);                //���ı���䵽��Ԫ��

        QFont font = item->font();
        font.setBold(true);                             //����Ӵ�
        font.setPixelSize(16);
        item->setFont(font);

        ui->tableWidget->setHorizontalHeaderItem(i, item);  //����Ԫ�����õ�ˮƽ��ͷ
    }

    /** �¿�����¼�� */
    QStringList tableHeader_2;
    tableHeader_2<<QString::fromLocal8Bit("��������")
                 <<QString::fromLocal8Bit("����")
                 <<QString::fromLocal8Bit("��ʼ����")
                 <<QString::fromLocal8Bit("��������")
                 <<QString::fromLocal8Bit("��ϵ�绰");

    ui->tableWidget_2->setColumnCount(5);             //��������Ϊ5
    ui->tableWidget_2->setAlternatingRowColors(true); //������ɫ��ʾ
    for(int i=0; i<ui->tableWidget_2->columnCount(); i++)
    {

        QTableWidgetItem *item = new QTableWidgetItem;  //������Ԫ��
        item->setText(tableHeader_2[i]);                //���ı���䵽��Ԫ��

        QFont font = item->font();
        font.setBold(true);                             //����Ӵ�
        font.setPixelSize(16);
        item->setFont(font);

        ui->tableWidget_2->setHorizontalHeaderItem(i, item);  //����Ԫ�����õ�ˮƽ��ͷ
    }
}


void MainWindow::on_showImage(QImage image)
{
    ui->label_screen1->setPixmap(QPixmap::fromImage(image).scaled(ui->label_screen1->size()));

    //����
    if(m_captureFlag)
    {
        m_carPlateImage = image;
        m_captureFlag = false;
        emit gotCapture();
    }
}

void MainWindow::on_showCardId(unsigned int id)
{
    //��ʾ����
    m_cardId = QString("%1").arg(id, 0, 16);
    ui->label_id->setText(m_cardId);
    m_captureFlag = true;
}

void MainWindow::on_showRemainStall()
{
    ui->label_space->setText(QString::number(m_remainStall));
}



void MainWindow::on_showCapture()
{
    ui->label_capture->setPixmap(QPixmap::fromImage(m_carPlateImage).scaled(ui->label_capture->size()));

    //��ʱ�䱣��ͼƬ
    QDateTime time = QDateTime::fromSecsSinceEpoch(m_time);
    QString text = QString("%1-%2").arg(time.date().toString("yyyy-MM-dd")).arg(time.time().toString("hh-mm-ss"));
    QString fileName = QString("C:/Users/JANSON/Desktop/TMP/%1.bmp").arg(text);

    m_carPlateImage.save(fileName, "BMP");
}

void MainWindow::on_recognizingCarPlates()
{
    //����ʶ�����ʾ
    QString access_token = HttpBaiDuApi::requestAccessToken(CARPLATERECONGTION_APIKEY, CARPLATERECONGTION_SECRETKEY);
    QByteArray imageUrl = Image::imageToUrlCodec(m_carPlateImage);
    m_carPlate = HttpBaiDuApi::carPlateRecognition(access_token, imageUrl);

    //m_carPlate = QString("S30HY6");
    ui->label_carPlate->setText(m_carPlate);

    //���ݿ��������
    if( ! m_carPlate.isEmpty()) //ʶ���Ʋ�Ϊ��
    {
        //�볡��¼����
        if(m_dataManager->searchCardId(m_cardId))
        {
            emit gotLeaveInfo(m_cardId, m_time);

            //��λ�������
            m_remainStall++;
            if(m_remainStall > 1000)
            {
                m_remainStall = 1000;
            }
            emit remainStallChanged();
        }
        else
        {
            emit gotEnterInfo(m_cardId, m_carPlate, m_time);

            m_remainStall--;
            if(m_remainStall < 0)
            {
                m_remainStall = 0;
            }
            emit remainStallChanged();
        }

        //�¿�����
        if(m_dataManager->searchCarPlate(m_carPlate))
        {
            ui->label_type->setText(QString::fromLocal8Bit("�¿���"));
            m_carType = 1;
        }
        else
        {
            ui->label_type->setText(QString::fromLocal8Bit("��ʱ��"));
            m_carType = 2;
        }
    }
    else                        //ʶ����Ϊ��
    {
        //�볡��¼����
        if(m_dataManager->searchCardId(m_cardId))
        {
            emit gotLeaveInfo(m_cardId, m_time);

            m_remainStall++;
            if(m_remainStall > 1000)
            {
                m_remainStall = 1000;
            }
            emit remainStallChanged();
        }
        else
        {
            emit gotEnterInfo(m_cardId, QString("-"), m_time);

            m_remainStall--;
            if(m_remainStall < 0)
            {
                m_remainStall = 0;
            }
            emit remainStallChanged();
        }

        //��ʱ����ʾ
        ui->label_type->setText(QString::fromLocal8Bit("��ʱ��"));
        m_carType = 2;
    }

    emit gotCarInfo(m_carType, m_carPlate);
}

void MainWindow::on_runTime()
{
    //��ȡʱ�����ʾʱ��
    QDateTime dateTime(QDateTime::currentDateTime());   //��ȡ��ǰʱ��
    m_time = dateTime.toSecsSinceEpoch();

    QDate date = dateTime.date();
    QTime time = dateTime.time();

    QString text = QString("%1 %2").arg(date.toString("yyyy-MM-dd")).arg(time.toString("hh:mm:ss"));
    ui->label_time->setText(text);
}

//��һ ����һ������
void MainWindow::on_insertRowData_t1(QString cardId, QString carPlate, qint64 time)
{
    //����һ��
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row + 1);

    //����Ҫ����ĵ�Ԫ��
    QTableWidgetItem* item1 = new QTableWidgetItem(cardId);
    QTableWidgetItem* item2 = new QTableWidgetItem(carPlate);

    QString etime = QString("%1 %2").arg(QDateTime::fromSecsSinceEpoch(time).date().toString("yyyy-MM-dd"))
                                         .arg(QDateTime::fromSecsSinceEpoch(time).time().toString("hh:mm:ss"));

    QTableWidgetItem* item3 = new QTableWidgetItem(etime);
    QTableWidgetItem* item4 = new QTableWidgetItem(QString("-"));
    QTableWidgetItem* item5 = new QTableWidgetItem(QString("-"));

    ui->tableWidget->setItem(row, 0, item1);
    ui->tableWidget->setItem(row, 1, item2);
    ui->tableWidget->setItem(row, 2, item3);
    ui->tableWidget->setItem(row, 3, item4);
    ui->tableWidget->setItem(row, 4, item5);
}

//��һ ����һ������
//ͨ���ȽϿ��ź��볡ʱ��Ϊ"-"�ַ���ʱ�Ÿ��¸�������
void MainWindow::on_supplementRowData_t1(QString cardId, double cost)
{
    //����ƥ��Ŀ���
    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(cardId, Qt::MatchContains);

    foreach(QTableWidgetItem* item, items)  //item������for�д�����i���� items����Ҫ����������
    {
        int row = item->row();  //��ȡƥ������к�

        if(ui->tableWidget->item(row, 3)->text() == QString("-"))   //�жϵ�4�еĲ����Ƿ�Ϊ "-"
        {
            QString ltime = QString("%1 %2").arg(QDateTime::fromSecsSinceEpoch(m_time).date().toString("yyyy-MM-dd"))
                                            .arg(QDateTime::fromSecsSinceEpoch(m_time).time().toString("hh:mm:ss"));

            ui->tableWidget->item(row, 3)->setText(ltime);
            ui->tableWidget->item(row, 4)->setText(QString::number(cost));

            break;
        }
    }
}

void MainWindow::on_insertRowData_t2(QString name, QString carPlate, qint64 transactTime, qint64 dueTime, QString phone)
{
    //����һ��
    int row = ui->tableWidget_2->rowCount();
    ui->tableWidget_2->setRowCount(row + 1);

    //����Ҫ����ĵ�Ԫ��
    QTableWidgetItem* item1 = new QTableWidgetItem(name);
    QTableWidgetItem* item2 = new QTableWidgetItem(carPlate);

    QString ttime = QString("%1").arg(QDateTime::fromSecsSinceEpoch(transactTime).date().toString("yyyy-MM-dd"));
    QString dtime = QString("%1").arg(QDateTime::fromSecsSinceEpoch(dueTime).date().toString("yyyy-MM-dd"));

    QTableWidgetItem* item3 = new QTableWidgetItem(ttime);
    QTableWidgetItem* item4 = new QTableWidgetItem(dtime);
    QTableWidgetItem* item5 = new QTableWidgetItem(phone);

    ui->tableWidget_2->setItem(row, 0, item1);
    ui->tableWidget_2->setItem(row, 1, item2);
    ui->tableWidget_2->setItem(row, 2, item3);
    ui->tableWidget_2->setItem(row, 3, item4);
    ui->tableWidget_2->setItem(row, 4, item5);
}



//ʶ��ť
void MainWindow::on_pushButton_recogn_clicked()
{
    m_captureFlag = true;
}

//tab2 - ����ť
void MainWindow::on_pushButton_ttransact_clicked()
{
    //��ȡ������
    QString name = ui->lineEdit_tname->text();
    QString carPlate = ui->lineEdit_tcarPlate->text();
    qint64 startDate = ui->dateEdit_ttransactDate->dateTime().toSecsSinceEpoch();
    qint64 dueDate = ui->dateEdit_tdueDate->dateTime().toSecsSinceEpoch();
    QString phone = ui->lineEdit_tphone->text();


    emit gotTransactInfo(name, carPlate, startDate, dueDate, phone);

    //���
    ui->lineEdit_tname->clear();
    ui->lineEdit_tphone->clear();
    ui->lineEdit_tcarPlate->clear();
}


