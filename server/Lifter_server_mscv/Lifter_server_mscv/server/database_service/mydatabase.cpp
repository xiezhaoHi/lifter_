#include "mydatabase.h"
#include"buffer/usebuffer.h"
#include<QTime>
#include<QThread>
#include<Qtsql/QSqlRecord>
#include<QTextCodec>
#include<QCoreApplication>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
MyDatabase::MyDatabase()
{
	qDebug() << QSqlDatabase::drivers();

	qDebug() << QCoreApplication::libraryPaths();

    m_strTableName[device_can] = QString("structure_stress_");
    m_strTableName[device_cgq] = QString("sensor_");
    m_strTableName[device_jdq] = QString("relay_");
    m_strTableName[device_bmq] = QString("encoder_");
    m_strDBName = QString("Lifter_");

    QDateTime current_date_time = QDateTime::currentDateTime();
    //QString strDate = current_date_time.toString("yy/mm/dd");
    //QString current_time = current_date_time.toString("hh:mm:ss.zzz");

    //CAN模块
    FindDBTableAndCreate(device_can,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMMddhh")));

    //Cgq模块
    FindDBTableAndCreate(device_cgq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMMdd")));
    //Jdq模块
    FindDBTableAndCreate(device_jdq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMM")));
    //Bmq模块
    FindDBTableAndCreate(device_bmq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMM")));


    m_stop = false;
    for(int index = 0; index < MAX_THREAD; ++index)
        thread_array[index]= new MyDatabaseThread();

    connect(&m_examine_timer,&QTimer::timeout,this,&MyDatabase::Examine_DB_Table);
    m_examine_timer.start(360000);

}

MyDatabase::~MyDatabase()
{

}


 MyDatabase*   MyDatabase::m_singleton = NULL;

 /*destroy服务*/
 void MyDatabase::Destroy()
 {
    NotifyStop();
    if(m_singleton)
    {
        delete m_singleton;
        m_singleton = NULL;
    }
 }


/*开启服务线程*/
void  MyDatabase::Start()
{


    for(int index =0; index < MAX_THREAD; ++index )
    {
        if(thread_array[index])
            thread_array[index]->start();
    }
}
  MyDatabase* MyDatabase::GetInstance(void)
 {
    if(NULL == m_singleton)
        m_singleton = new MyDatabase();
    return m_singleton;
 }

/*
 * 判断 表是否存在
 */
   bool MyDatabase::TableExists(QString const& strDBName,QString const& strTableName)
   {
	   QString strSql = QString("select * from %1.%2;").arg(strDBName).arg(strTableName);
       if(strSql.isEmpty())
           return false;
       QSqlDatabase db =  ConnectionPool::openConnection();
       QSqlQuery sql_query(db);
       bool retFlag = false;

        sql_query.prepare(strSql);

       if(!sql_query.exec())
       {
            qDebug()<<strSql << "##"<<sql_query.lastError();
       }
       else
       {
           retFlag = true;
       }
       ConnectionPool::closeConnection(db);
       return retFlag;
   }


//给数据库添加数据
bool  MyDatabase::AddData(QString const& strSql)
{
    if(strSql.isEmpty())
        return false;
    QSqlDatabase db =  ConnectionPool::openConnection();
    //qDebug()<< QString("获取数据库连接");
    QSqlQuery sql_query(db);
    bool retFlag = false;

     sql_query.prepare(strSql);

    if(!sql_query.exec())
    {
         qDebug()<<strSql << "##"<<sql_query.lastError();
    }
    else
    {
        retFlag = true;
    }
    ConnectionPool::closeConnection(db);
   // qDebug()<< QString("一次存储完成");
    return retFlag;
}
//删除指定的
bool MyDatabase::DeleteData(QString const& strSql)
{
    QSqlDatabase db =  ConnectionPool::openConnection();

    QSqlQuery sql_query(db);
    bool retFlag = false;
    sql_query.prepare(strSql);

    if(!sql_query.exec())
    {
         qDebug()<<sql_query.lastError();
    }
    else
    {
        retFlag = true;
    }
    ConnectionPool::closeConnection(db);
    return retFlag;
}
//更改数据库 数据
bool MyDatabase::ChangeData(QString const& strSql)
{
    QSqlDatabase db =  ConnectionPool::openConnection();

    QSqlQuery sql_query(db);
    bool retFlag = false;
    sql_query.prepare(strSql);

    if(!sql_query.exec())
    {
         qDebug()<<sql_query.lastError();
    }
    else
    {
        retFlag = true;
    }
    ConnectionPool::closeConnection(db);
    return retFlag;
}

//查询数据库
bool MyDatabase::SelectData(QString const& strSql,QStringList**& pList, int& count,int flag)
{
    QSqlDatabase db;
    if(connect_db_mydb == flag) //连接程序数据库
         db =  ConnectionPool::openConnection();
    if(connect_db_gssdb == flag) //连接钢丝绳数据库
        db = ConnectionPool_gss::openConnection();

    QSqlQuery sql_query(db);

    bool retFlag = false;
    sql_query.prepare(strSql);

    if(!sql_query.exec())
    {
         qDebug()<<sql_query.lastError();
    }
    else
    {
        retFlag = true;
        count = sql_query.size();
        if(count >0)
        {
            int columnNum = sql_query.record().count(); //列数
            pList = new QStringList*[count];
            int index = 0;
            while(sql_query.next())
            {
                    pList[index] = new QStringList;
                    for(int col = 0; col < columnNum; ++col)
                    {
                        pList[index]->append(sql_query.value(col).toString().toLocal8Bit());


                    }
                      ++index;
              }
        }
    }
    if(connect_db_mydb == flag) //连接程序数据库
          ConnectionPool::closeConnection(db);
    if(connect_db_gssdb == flag) //连接钢丝绳数据库
         ConnectionPool_gss::closeConnection(db);
    return retFlag;
}



/*是否停止*/
bool MyDatabase::IsStop()
{
    return m_stop;
}

/*通知停止*/
void MyDatabase::NotifyStop()
{
    m_stop = true;
    for(int index = 0; index < MAX_THREAD; ++index)
    {
        if(thread_array[index])
        {
            thread_array[index]->stop();
            thread_array[index]->wait();
            delete thread_array[index];
        }
    }
}
/*
 * 判断数据库 是否有 指定数据库 和 指定数据表
 */
bool    MyDatabase::FindDBTableAndCreate(int flag,QString const& strDBName,QString const& strTableName)
{

    QString strDB = QString("Create Database If Not Exists %1%2 Character Set UTF8")
            .arg(m_strDBName)
            .arg(strDBName);
    bool retFalg = AddData(strDB);
    if(!retFalg)
        return false;

    //表不存在
    if(!TableExists(m_strDBName+strDBName
                                             ,m_strTableName[flag]+strTableName))
    {
        //创建表
       retFalg = CreateFlagTable(flag,m_strDBName+strDBName,m_strTableName[flag]+strTableName);
        if(!retFalg)
            return false;
        //初始化sq buffer
        UserBuffer::GetInstance()->InitDBSqlBuff(flag,m_strDBName+strDBName,m_strTableName[flag]+strTableName);
    }
    return true;
}
/*
 * 创建指定的表
 */
bool    MyDatabase::CreateFlagTable(int flag,QString const& strDBName, QString const& strTableName)
{
    QString strSql;
    switch (flag) {
    case device_can:
            strSql = QString("CREATE TABLE  If Not Exists  `%1`.`%2` (\
                             `structure_stress_id` varchar(10) NOT NULL,\
                            `structure_stress_date_id` varchar(45) DEFAULT NULL,\
                             `structure_stress_date` varchar(45) DEFAULT NULL,\
                             `structure_stress_time` varchar(45) DEFAULT NULL,\
                             `structure_stress_ms` varchar(45) DEFAULT NULL,\
                             `structure_stress_data` varchar(45) DEFAULT NULL,\
                             `save_date` varchar(45) DEFAULT NULL,\
                             `save_time` varchar(45) DEFAULT NULL\
                           ) ENGINE=MyISAM DEFAULT CHARSET=utf8;")
            .arg(strDBName)
            .arg(strTableName)
           ;
        break;

   case device_cgq:
            strSql = QString("CREATE TABLE  If Not Exists  `%1`.`%2` (\
                             `sensor_id` varchar(45) DEFAULT NULL,\
                            `sensor_date` varchar(45) DEFAULT NULL,\
                            `sensor_time` varchar(45) DEFAULT NULL,\
                            `sensor_jd_x` varchar(45) DEFAULT NULL,\
                            `sensor_jd_y` varchar(45) DEFAULT NULL,\
                            `sensor_jd_z` varchar(45) DEFAULT NULL,\
                            `sensor_jsd_x` varchar(45) DEFAULT NULL,\
                            `sensor_jsd_y` varchar(45) DEFAULT NULL,\
                            `sensor_jsd_z` varchar(45) DEFAULT NULL\
                          ) ENGINE=MyISAM DEFAULT CHARSET=utf8;")
            .arg(strDBName)
            .arg(strTableName);
        break;
   case device_jdq:
            strSql = QString("CREATE TABLE  If Not Exists  `%1`.`%2` (\
                             `relay_id` varchar(45) DEFAULT NULL,\
                            `relay_date` varchar(45) DEFAULT NULL,\
                            `relay_time` varchar(45) DEFAULT NULL,\
                            `relay_state` varchar(8192) DEFAULT NULL\
                          ) ENGINE=MyISAM DEFAULT CHARSET=utf8;")
            .arg(strDBName)
            .arg(strTableName);
        break;
    case device_bmq:
            strSql = QString("CREATE TABLE  If Not Exists  `%1`.`%2` (\
                             `encoder_id` varchar(45) DEFAULT NULL,\
                            `encoder_date` varchar(45) DEFAULT NULL,\
                            `encoder_time` varchar(45) DEFAULT NULL,\
                            `encoder_dir` varchar(45) DEFAULT NULL,\
                            `encoder_jd` varchar(45) DEFAULT NULL,\
                            `encoder_zs` varchar(45) DEFAULT NULL,\
                            `encoder_jsz` varchar(45) DEFAULT NULL\
                          ) ENGINE=MyISAM DEFAULT CHARSET=utf8;")
            .arg(strDBName)
            .arg(strTableName);
        break;
    default:
        break;
    }
    return AddData(strSql);
}

/*
 * 对外 提供 借口 检查 数据库 和 表
 */
void  MyDatabase::Examine_DB_Table()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString strDate = current_date_time.toString("yyyyMMdd");
//    QString current_time = current_date_time.toString("hh:mm:ss.zzz");

    //CAN模块
    FindDBTableAndCreate(device_can,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMMddhh")));

    //Cgq模块
    FindDBTableAndCreate(device_cgq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMMdd")));
    //Jdq模块
    FindDBTableAndCreate(device_jdq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMM")));
    //Bmq模块
    FindDBTableAndCreate(device_bmq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMM")));
    UserBuffer::GetInstance()->ClearBuff();  //清理 数据库 存储队列
}

MyDatabaseThread::MyDatabaseThread()
{
    m_stop = true;
}

void MyDatabaseThread::run()
{
    try
    {
       // HANDLE currentID = currentThreadId();
      //  qDebug() << QString("数据库存储线程开启!_") << currentID;
        while(m_stop)
        {


                QString strSql = UserBuffer::GetInstance()->PopDatabaseQueue();
                if(!strSql.isEmpty())
                    MyDatabase::GetInstance()->AddData(strSql);
            msleep(10);
        }
        qDebug() << QString("数据库存储线程结束,开始最后的清理工作!_") ;



        UserBuffer::GetInstance()->ClearBuff(); //清理数据库队列

        while(!UserBuffer::GetInstance()->IsEmptyDatabaseQueue())
        {
            MyDatabase::GetInstance()->AddData(UserBuffer::GetInstance()->PopDatabaseQueue());
            msleep(10);
        }
        qDebug() << QString("数据库存储线程,结束清理工作!_") ;

    }catch(...)
    {
        qDebug() << QString("线程异常!_%1").arg(GetLastError());
    }
}

void MyDatabaseThread::stop()
{
    m_stop = false;
}
