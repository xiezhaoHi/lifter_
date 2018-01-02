#ifndef MYDATABASE_H
#define MYDATABASE_H

#include"connectionpool.h"
#include<QRunnable>
#include<QThread>
#include<QTimer>
#include<QObject>

/*
 * ���ݿ������
 * ʵ�����ݿ��������
 * ��:addData(QString const& strSql)
 * ɾ:deleteData(QString const& strSql)
 * ��:changeData(QString const& strSql)
 * ��:selectData(QString const& strSql)
 *
 */

class MyMoveToThread : public QObject
{

    Q_OBJECT

public:
public slots:
    //
    void MybeginThread();
signals:



};

class DataBase_thread : public QObject
{

    Q_OBJECT

public:
    DataBase_thread()
    {
        m_threadFun = new MyMoveToThread();
        m_threadFun->moveToThread(&m_thread);
        connect(&m_timer,&QTimer::timeout,m_threadFun,&MyMoveToThread::MybeginThread);
        connect(&m_thread,&QThread::finished,m_threadFun,&QObject::deleteLater);
    }
    ~DataBase_thread()
    {
        m_thread.quit();
        m_thread.wait();
    }

    void start(int time = 1000)
    {
         m_timer.start(time);
        m_thread.start();
    }

    QThread m_thread;
    MyMoveToThread* m_threadFun;
    QTimer  m_timer;
public slots:

signals:

};

#define MAX_THREAD 2
class MyDatabase
{
private:
    //QString m_strPath;  //�����ļ���·��
public:
    static MyDatabase*  m_singleton;
    static MyDatabase* GetInstance(void);
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
    bool SelectData(QString const& strSql, QStringList **&pList, int &count);
private:
    DataBase_thread* thread_array[MAX_THREAD];
    MyDatabase();
   //  friend class Database_task;
     bool   m_stop;

};

#endif // MYDATABASE_H
