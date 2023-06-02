#include "datamanager.h"
#include <QDebug>
DataManager::DataManager(QObject *parent) : QObject(parent)
{
   m_dataBase = QSqlDatabase::addDatabase("QSQLITE");

   //�������ݿ�
   m_dataBase.setDatabaseName("park.db");
   if( ! m_dataBase.open() )
   {
       qDebug()<<m_dataBase.lastError().text();
       return;
   }

   //��ӱ�
   //���������¼��
   QString sql = QString("create table if not exists AccessRecord(CardID TEXT, CarPlate TEXT, EnterTime INTEGER, LeaveTime INTEGER, Cost REAL);");
   QSqlQuery query;

   if( ! query.exec(sql) )
   {
       qDebug()<<"create table AccessRecord error!"<<query.lastError().text();
   }

   //�¿�����¼��
   sql = QString("create table if not exists VipRecord(Name TEXT, CarPlate TEXT, TtransactTime INTEGER, DueTime INTEGER, Phone TEXT);");
   if( ! query.exec(sql) )
   {
       qDebug()<<"create table VipRecord error!"<<query.lastError().text();
   }

}

//�����Ƿ�����ͬ���ż�¼���Ҹü�¼��û�볡���ҵ������棬û�ҵ����ؼ�
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

//�����Ƿ�����ͬ�ĳ��Ƽ�¼���ҵ������棬û�ҵ����ؼ�
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

//�����볡����
void DataManager::on_insertAdmissionInfo(QString cardId, QString carPlate, qint64 etime)
{
    QString sql = QString("insert into AccessRecord values('%1', '%2', %3, %4, %5);").arg(cardId).arg(carPlate).arg(etime).arg(0).arg(0.0);
    QSqlQuery query;

    if( ! query.exec(sql) )
    {
        qDebug()<<"insert into AccessRecord error!"<<query.lastError().text();
    }
}

//�����볡����
void DataManager::on_replenishDepartureInfo(QString cardId, qint64 ltime)
{
    //���Ҳ���ȡ�볡ʱ��
    QString sql = QString("select * from AccessRecord where CardID = '%1' and LeaveTime = 0;").arg(cardId);
    QSqlQuery query;

    if( ! query.exec(sql) )
    {
        qDebug()<<"select AccessRecord error!"<<query.lastError().text();
        return;
    }

    query.next();
    qint64 etime = query.value("EnterTime").toULongLong();

    //�Ʒ��㷨
    double cost;
    cost = (ltime - etime) * 1.0;


    //�滻�볡��Ϣ
    sql = QString("update AccessRecord set LeaveTime = %1 , Cost = %2 where CardID = '%3' and LeaveTime = 0;").arg(ltime).arg(cost).arg(cardId);
    if( ! query.exec(sql) )
    {
        qDebug()<<"update AccessRecord error!"<<query.lastError().text();
    }

    //���ͼ����������ź�
    emit calculateFinish(cardId, cost);
}


//��Ӱ쿨��Ϣ
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
