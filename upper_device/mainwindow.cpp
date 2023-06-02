#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //剩余车位
    m_remainStall = 1000;
    ui->label_space->setText(QString::number(m_remainStall));
    connect(this, &MainWindow::remainStallChanged, this, &MainWindow::on_showRemainStall);

    //时间模块
    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, this, &MainWindow::on_runTime);
    m_timer->start(1000);

    //监控模块
    m_video = new Video;
    connect(m_video, &Video::gotOneFrameData, this, &MainWindow::on_showImage);
    m_video->start();

    //网络模块
    m_network = new NetWork;
    connect(m_network, &NetWork::gotIcCardId, this, &MainWindow::on_showCardId);
    connect(this, &MainWindow::gotCarInfo, m_network, &NetWork::on_sendCarInfo);

    //截屏和识别车牌模块
    m_captureFlag = false;
    connect(this, &MainWindow::gotCapture, this, &MainWindow::on_recognizingCarPlates);
    connect(this, &MainWindow::gotCapture, this, &MainWindow::on_showCapture);

    //数据处理模块
    m_dataManager = new DataManager;
    connect(this, &MainWindow::gotEnterInfo, m_dataManager, &DataManager::on_insertAdmissionInfo);          //获得入场信息 -- 激活数据库插入入场数据槽函数
    connect(this, &MainWindow::gotEnterInfo, this, &MainWindow::on_insertRowData_t1);                       //获得入场信息 -- 激活插入条目信息槽函数
    connect(this, &MainWindow::gotLeaveInfo, m_dataManager, &DataManager::on_replenishDepartureInfo);       //获得离场信息 -- 激活数据库补充离场数据槽函数
    connect(m_dataManager, &DataManager::calculateFinish, this, &MainWindow::on_supplementRowData_t1);      //完成费用计算 -- 激活补充条目信息槽函数
    connect(this, &MainWindow::gotTransactInfo, m_dataManager, &DataManager::on_insertTransactCardInfo);    //获得办卡信息 -- 激活数据库插入办卡数据槽函数
    connect(m_dataManager, &DataManager::transactFinish, this, &MainWindow::on_insertRowData_t2);           //完成办卡操作 -- 激活插入条目信息槽函数
    ui->dateEdit_ttransactDate->setDate(QDate::currentDate());
    initTableWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTableWidget()
{
    /** 车辆出入记录表 */
    QStringList tableHeader_1;
    tableHeader_1<<QString::fromLocal8Bit("卡号")
                 <<QString::fromLocal8Bit("车牌")
                 <<QString::fromLocal8Bit("入场时间")
                 <<QString::fromLocal8Bit("离场时间")
                 <<QString::fromLocal8Bit("费用");

    ui->tableWidget->setColumnCount(5);             //设置列数为5
    ui->tableWidget->setAlternatingRowColors(true); //交替颜色显示
    for(int i=0; i<ui->tableWidget->columnCount(); i++)
    {

        QTableWidgetItem *item = new QTableWidgetItem;  //创建单元格
        item->setText(tableHeader_1[i]);                //将文本填充到单元格

        QFont font = item->font();
        font.setBold(true);                             //字体加粗
        font.setPixelSize(16);
        item->setFont(font);

        ui->tableWidget->setHorizontalHeaderItem(i, item);  //将单元格设置到水平表头
    }

    /** 月卡车记录表 */
    QStringList tableHeader_2;
    tableHeader_2<<QString::fromLocal8Bit("车主姓名")
                 <<QString::fromLocal8Bit("车牌")
                 <<QString::fromLocal8Bit("起始日期")
                 <<QString::fromLocal8Bit("结束日期")
                 <<QString::fromLocal8Bit("联系电话");

    ui->tableWidget_2->setColumnCount(5);             //设置列数为5
    ui->tableWidget_2->setAlternatingRowColors(true); //交替颜色显示
    for(int i=0; i<ui->tableWidget_2->columnCount(); i++)
    {

        QTableWidgetItem *item = new QTableWidgetItem;  //创建单元格
        item->setText(tableHeader_2[i]);                //将文本填充到单元格

        QFont font = item->font();
        font.setBold(true);                             //字体加粗
        font.setPixelSize(16);
        item->setFont(font);

        ui->tableWidget_2->setHorizontalHeaderItem(i, item);  //将单元格设置到水平表头
    }
}


void MainWindow::on_showImage(QImage image)
{
    ui->label_screen1->setPixmap(QPixmap::fromImage(image).scaled(ui->label_screen1->size()));

    //截屏
    if(m_captureFlag)
    {
        m_carPlateImage = image;
        m_captureFlag = false;
        emit gotCapture();
    }
}

void MainWindow::on_showCardId(unsigned int id)
{
    //显示卡号
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

    //按时间保存图片
    QDateTime time = QDateTime::fromSecsSinceEpoch(m_time);
    QString text = QString("%1-%2").arg(time.date().toString("yyyy-MM-dd")).arg(time.time().toString("hh-mm-ss"));
    QString fileName = QString("C:/Users/JANSON/Desktop/TMP/%1.bmp").arg(text);

    m_carPlateImage.save(fileName, "BMP");
}

void MainWindow::on_recognizingCarPlates()
{
    //车牌识别和显示
    QString access_token = HttpBaiDuApi::requestAccessToken(CARPLATERECONGTION_APIKEY, CARPLATERECONGTION_SECRETKEY);
    QByteArray imageUrl = Image::imageToUrlCodec(m_carPlateImage);
    m_carPlate = HttpBaiDuApi::carPlateRecognition(access_token, imageUrl);

    //m_carPlate = QString("S30HY6");
    ui->label_carPlate->setText(m_carPlate);

    //数据库插入数据
    if( ! m_carPlate.isEmpty()) //识别车牌不为空
    {
        //入场记录检索
        if(m_dataManager->searchCardId(m_cardId))
        {
            emit gotLeaveInfo(m_cardId, m_time);

            //车位数量变更
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

        //月卡检索
        if(m_dataManager->searchCarPlate(m_carPlate))
        {
            ui->label_type->setText(QString::fromLocal8Bit("月卡车"));
            m_carType = 1;
        }
        else
        {
            ui->label_type->setText(QString::fromLocal8Bit("临时车"));
            m_carType = 2;
        }
    }
    else                        //识别车牌为空
    {
        //入场记录检索
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

        //临时车显示
        ui->label_type->setText(QString::fromLocal8Bit("临时车"));
        m_carType = 2;
    }

    emit gotCarInfo(m_carType, m_carPlate);
}

void MainWindow::on_runTime()
{
    //获取时间和显示时间
    QDateTime dateTime(QDateTime::currentDateTime());   //获取当前时间
    m_time = dateTime.toSecsSinceEpoch();

    QDate date = dateTime.date();
    QTime time = dateTime.time();

    QString text = QString("%1 %2").arg(date.toString("yyyy-MM-dd")).arg(time.toString("hh:mm:ss"));
    ui->label_time->setText(text);
}

//表一 插入一行数据
void MainWindow::on_insertRowData_t1(QString cardId, QString carPlate, qint64 time)
{
    //插入一行
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row + 1);

    //创建要插入的单元格
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

//表一 补充一行数据
//通过比较卡号和离场时间为"-"字符串时才更新该行数据
void MainWindow::on_supplementRowData_t1(QString cardId, double cost)
{
    //查找匹配的卡号
    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(cardId, Qt::MatchContains);

    foreach(QTableWidgetItem* item, items)  //item类似于for中创建的i变量 items则是要遍历的容器
    {
        int row = item->row();  //获取匹配项的行号

        if(ui->tableWidget->item(row, 3)->text() == QString("-"))   //判断第4列的参数是否为 "-"
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
    //插入一行
    int row = ui->tableWidget_2->rowCount();
    ui->tableWidget_2->setRowCount(row + 1);

    //创建要插入的单元格
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



//识别按钮
void MainWindow::on_pushButton_recogn_clicked()
{
    m_captureFlag = true;
}

//tab2 - 办理按钮
void MainWindow::on_pushButton_ttransact_clicked()
{
    //获取表单数据
    QString name = ui->lineEdit_tname->text();
    QString carPlate = ui->lineEdit_tcarPlate->text();
    qint64 startDate = ui->dateEdit_ttransactDate->dateTime().toSecsSinceEpoch();
    qint64 dueDate = ui->dateEdit_tdueDate->dateTime().toSecsSinceEpoch();
    QString phone = ui->lineEdit_tphone->text();


    emit gotTransactInfo(name, carPlate, startDate, dueDate, phone);

    //清空
    ui->lineEdit_tname->clear();
    ui->lineEdit_tphone->clear();
    ui->lineEdit_tcarPlate->clear();
}


