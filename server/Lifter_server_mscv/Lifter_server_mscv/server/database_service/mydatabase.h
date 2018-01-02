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
 * 数据库操作库
 * 实现数据库基本操作
 * 增:addData(QString const& strSql)
 * 删:deleteData(QString const& strSql)
 * 改:changeData(QString const& strSql)
 * 查:selectData(QString const& strSql)
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
    //QString m_strPath;  //配置文件的路径
public:
    static MyDatabase*  m_singleton;
    static MyDatabase* GetInstance(void);
    ~MyDatabase();
    /*开启服务线程*/
    void Start();

    /*destroy服务*/
    void Destroy();

    /*是否停止*/
    bool IsStop();

    /*通知停止*/
    void NotifyStop();

     bool AddData(const QString &strSql);
    bool DeleteData(QString const& strSql);
    bool ChangeData(QString const& strSql);
    bool SelectData(QString const& strSql, QStringList **&pList, int &count,int flag = connect_db_mydb);
    bool TableExists(const QString &strDBName, QString const& strTableName);

    /*
     * 判断数据库 是否有 指定数据库 和 指定数据表
     */
    bool    FindDBTableAndCreate(int flag, QString const& strDBName, QString const& strTableName);
    /*
     * 创建指定的表
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
      * 对外 提供 借口 检查 数据库 和 表
      */
     void  Examine_DB_Table();
};

#endif // MYDATABASE_H
