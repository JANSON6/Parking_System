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
    //���������¼��
    bool searchCardId(QString cardId);
    //�¿�����¼��
    bool searchCarPlate(QString carPlate);

public slots:
    //���������¼��
    void on_insertAdmissionInfo(QString cardId, QString carPlate, qint64 etime);   //�����볡��Ϣ
    void on_replenishDepartureInfo(QString cardId, qint64 ltime);                  //�����볡��Ϣ
    //�¿�����¼��
    void on_insertTransactCardInfo(QString name, QString carPlate, qint64 transactTime, qint64 dueTime, QString phone);

signals:
    void calculateFinish(QString cardId, double cost);
    void transactFinish(QString name, QString carPlate, qint64 transactTime, qint64 dueTime, QString phone);

private:
    QSqlDatabase m_dataBase;

};

#endif // DATAMANAGER_H
