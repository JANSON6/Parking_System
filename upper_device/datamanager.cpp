#include "datamanager.h"
#include <QDebug>
DataManager::DataManager(QObject *parent) : QObject(parent)
{
   m_dataBase = QSqlDatabase::addDatabase("QSQLITE");

   //连接数据库
   m_dataBase.setDatabaseName("park.db");
   if( ! m_dataBase.open() )
   {
       qDebug()<<m_dataBase.lastError().text();
       return;
   }

   //添加表
   //车辆出入记录表
   QString sql = QString("create table if not exists AccessRecord(CardID TEXT, CarPlate TEXT, EnterTime INTEGER, LeaveTime INTEGER, Cost REAL);");
   QSqlQuery query;

   if( ! query.exec(sql) )
   {
       qDebug()<<"create table AccessRecord error!"<<query.lastError().text();
   }

   //月卡车记录表
   sql = QString("create table if not exists VipRecord(Name TEXT, CarPlate TEXT, TtransactTime INTEGER, DueTime INTEGER, Phone TEXT);");
   if( ! query.exec(sql) )
   {
       qDebug()<<"create table VipRecord error!"<<query.lastError().text();
   }

}

//检索是否有相同卡号记录，且该记录还没离场，找到返回真，没找到返回假
bool DataManager::searchCardId(QString cardId)
{
    QString sql = QString("select * from AccessRecord where CardID = '%1' and LeaveTime = 0;").arg(cardId);
    QSqlQuery query;
    if( ! query.exec(sql) )
    {
        qDebug()<<"select * from AccessRecord error!"<<query.lastError().text();
    }

    bool flag = false;
    while(query.next())
    {
        flag = true;
    }

    if(flag == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//检索是否有相同的车牌记录，找到返回真，没找到返回假
bool DataManager::searchCarPlate(QString carPlate)
{
    QString sql = QString("select * from VipRecord where CarPlate = '%1';").arg(carPlate);
    QSqlQuery query;
    if( ! query.exec(sql) )
    {
        qDebug()<<"select * from VipRecord error!"<<query.lastError().text();
    }

    bool flag = false;
    while(query.next())
    {
        flag = true;
    }

    if(flag == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//插入入场数据
void DataManager::on_insertAdmissionInfo(QString cardId, QString carPlate, qint64 etime)
{
    QString sql = QString("insert into AccessRecord values('%1', '%2', %3, %4, %5);").arg(cardId).arg(carPlate).arg(etime).arg(0).arg(0.0);
    QSqlQuery query;

    if( ! query.exec(sql) )
    {
        qDebug()<<"insert into AccessRecord error!"<<query.lastError().text();
    }
}

//补充离场数据
void DataManager::on_replenishDepartureInfo(QString cardId, qint64 ltime)
{
    //查找并获取入场时间
    QString sql = QString("select * from AccessRecord where CardID = '%1' and LeaveTime = 0;").arg(cardId);
    QSqlQuery query;

    if( ! query.exec(sql) )
    {
        qDebug()<<"select AccessRecord error!"<<query.lastError().text();
        return;
    }

    query.next();
    qint64 etime = query.value("EnterTime").toULongLong();

    //计费算法
    double cost;
    cost = (ltime - etime) * 1.0;


    //替换离场信息
    sql = QString("update AccessRecord set LeaveTime = %1 , Cost = %2 where CardID = '%3' and LeaveTime = 0;").arg(ltime).arg(cost).arg(cardId);
    if( ! query.exec(sql) )
    {
        qDebug()<<"update AccessRecord error!"<<query.lastError().text();
    }

    //发送计算费用完成信号
    emit calculateFinish(cardId, cost);
}


//添加办卡信息
void DataManager::on_insertTransactCardInfo(QString name, QString carPlate, qint64 transactTime, qint64 dueTime, QString phone)
{
    QString sql = QString("insert into VipRecord values('%1', '%2', %3, %4, '%5');").arg(name).arg(carPlate).arg(transactTime).arg(dueTime).arg(phone);
    QSqlQuery query;
    if( ! query.exec(sql) )
    {
        qDebug()<<"insert into VipRecord error!"<<query.lastError().text();
    }

    emit transactFinish(name, carPlate, transactTime, dueTime, phone);
}
