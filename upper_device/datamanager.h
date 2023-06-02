#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>



class DataManager : public QObject
{
    Q_OBJECT

public:
    explicit DataManager(QObject *parent = nullptr);
    //车辆出入记录表
    bool searchCardId(QString cardId);
    //月卡车记录表
    bool searchCarPlate(QString carPlate);

public slots:
    //车辆出入记录表
    void on_insertAdmissionInfo(QString cardId, QString carPlate, qint64 etime);   //插入入场信息
    void on_replenishDepartureInfo(QString cardId, qint64 ltime);                  //补充离场信息
    //月卡车记录表
    void on_insertTransactCardInfo(QString name, QString carPlate, qint64 transactTime, qint64 dueTime, QString phone);

signals:
    void calculateFinish(QString cardId, double cost);
    void transactFinish(QString name, QString carPlate, qint64 transactTime, qint64 dueTime, QString phone);

private:
    QSqlDatabase m_dataBase;

};

#endif // DATAMANAGER_H
