#ifndef MYDATABASE_H
#define MYDATABASE_H

#include"connectionpool.h"
#include"connectionpool_gss.h"
#include"server/task_service/task_service.h"
#include<QRunnable>
#include<QThread>
#include<QTimer>
#include<QObject>
#include"enum.h"
/*
 * ���ݿ������
 * ʵ�����ݿ��������
 * ��:addData(QString const& strSql)
 * ɾ:deleteData(QString const& strSql)
 * ��:changeData(QString const& strSql)
 * ��:selectData(QString const& strSql)
 *
 */
class MyDatabaseThread:public QThread
{
public:
    MyDatabaseThread();
    ~MyDatabaseThread()
    {
        int wtf = -1;
    }

    void run();

    void stop();

private:

volatile  bool    m_stop;

};


#define MAX_THREAD 5
class MyDatabase:public QObject
{
    Q_OBJECT
private:
    //QString m_strPath;  //�����ļ���·��
public:
    static MyDatabase*  m_singleton;
    static MyDatabase* GetInstance(void);
    ~MyDatabase();
    /*���������߳�*/
    void Start();

    /*destroy����*/
    void Destroy();

    /*�Ƿ�ֹͣ*/
    bool IsStop();

    /*ֹ֪ͨͣ*/
    void NotifyStop();

     bool AddData(const QString &strSql);
    bool DeleteData(QString const& strSql);
    bool ChangeData(QString const& strSql);
    bool SelectData(QString const& strSql, QStringList **&pList, int &count,int flag = connect_db_mydb);
    bool TableExists(const QString &strDBName, QString const& strTableName);

    /*
     * �ж����ݿ� �Ƿ��� ָ�����ݿ� �� ָ�����ݱ�
     */
    bool    FindDBTableAndCreate(int flag, QString const& strDBName, QString const& strTableName);
    /*
     * ����ָ���ı�
     */
    bool    CreateFlagTable(int flag,QString const& strDBName, QString const& strTableName);


private:
    MyDatabaseThread* thread_array[MAX_THREAD];
    MyDatabase();
    QString m_strDBName;
    QString m_strTableName[device_max];
   //  friend class Database_task;
     bool   m_stop;
     QTimer  m_examine_timer;
public slots:
     /*
      * ���� �ṩ ��� ��� ���ݿ� �� ��
      */
     void  Examine_DB_Table();
};

#endif // MYDATABASE_H
