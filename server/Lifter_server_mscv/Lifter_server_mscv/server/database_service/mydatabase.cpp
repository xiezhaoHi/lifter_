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

    //CANģ��
    FindDBTableAndCreate(device_can,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMMddhh")));

    //Cgqģ��
    FindDBTableAndCreate(device_cgq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMMdd")));
    //Jdqģ��
    FindDBTableAndCreate(device_jdq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMM")));
    //Bmqģ��
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

 /*destroy����*/
 void MyDatabase::Destroy()
 {
    NotifyStop();
    if(m_singleton)
    {
        delete m_singleton;
        m_singleton = NULL;
    }
 }


/*���������߳�*/
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
 * �ж� ���Ƿ����
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


//�����ݿ��������
bool  MyDatabase::AddData(QString const& strSql)
{
    if(strSql.isEmpty())
        return false;
    QSqlDatabase db =  ConnectionPool::openConnection();
    //qDebug()<< QString("��ȡ���ݿ�����");
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
   // qDebug()<< QString("һ�δ洢���");
    return retFlag;
}
//ɾ��ָ����
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
//�������ݿ� ����
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

//��ѯ���ݿ�
bool MyDatabase::SelectData(QString const& strSql,QStringList**& pList, int& count,int flag)
{
    QSqlDatabase db;
    if(connect_db_mydb == flag) //���ӳ������ݿ�
         db =  ConnectionPool::openConnection();
    if(connect_db_gssdb == flag) //���Ӹ�˿�����ݿ�
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
            int columnNum = sql_query.record().count(); //����
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
    if(connect_db_mydb == flag) //���ӳ������ݿ�
          ConnectionPool::closeConnection(db);
    if(connect_db_gssdb == flag) //���Ӹ�˿�����ݿ�
         ConnectionPool_gss::closeConnection(db);
    return retFlag;
}



/*�Ƿ�ֹͣ*/
bool MyDatabase::IsStop()
{
    return m_stop;
}

/*ֹ֪ͨͣ*/
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
 * �ж����ݿ� �Ƿ��� ָ�����ݿ� �� ָ�����ݱ�
 */
bool    MyDatabase::FindDBTableAndCreate(int flag,QString const& strDBName,QString const& strTableName)
{

    QString strDB = QString("Create Database If Not Exists %1%2 Character Set UTF8")
            .arg(m_strDBName)
            .arg(strDBName);
    bool retFalg = AddData(strDB);
    if(!retFalg)
        return false;

    //������
    if(!TableExists(m_strDBName+strDBName
                                             ,m_strTableName[flag]+strTableName))
    {
        //������
       retFalg = CreateFlagTable(flag,m_strDBName+strDBName,m_strTableName[flag]+strTableName);
        if(!retFalg)
            return false;
        //��ʼ��sq buffer
        UserBuffer::GetInstance()->InitDBSqlBuff(flag,m_strDBName+strDBName,m_strTableName[flag]+strTableName);
    }
    return true;
}
/*
 * ����ָ���ı�
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
 * ���� �ṩ ��� ��� ���ݿ� �� ��
 */
void  MyDatabase::Examine_DB_Table()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString strDate = current_date_time.toString("yyyyMMdd");
//    QString current_time = current_date_time.toString("hh:mm:ss.zzz");

    //CANģ��
    FindDBTableAndCreate(device_can,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMMddhh")));

    //Cgqģ��
    FindDBTableAndCreate(device_cgq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMMdd")));
    //Jdqģ��
    FindDBTableAndCreate(device_jdq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMM")));
    //Bmqģ��
    FindDBTableAndCreate(device_bmq,current_date_time.toString(QString("yyyyMM"))
                  ,current_date_time.toString(QString("yyyyMM")));
    UserBuffer::GetInstance()->ClearBuff();  //���� ���ݿ� �洢����
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
      //  qDebug() << QString("���ݿ�洢�߳̿���!_") << currentID;
        while(m_stop)
        {


                QString strSql = UserBuffer::GetInstance()->PopDatabaseQueue();
                if(!strSql.isEmpty())
                    MyDatabase::GetInstance()->AddData(strSql);
            msleep(10);
        }
        qDebug() << QString("���ݿ�洢�߳̽���,��ʼ����������!_") ;



        UserBuffer::GetInstance()->ClearBuff(); //�������ݿ����

        while(!UserBuffer::GetInstance()->IsEmptyDatabaseQueue())
        {
            MyDatabase::GetInstance()->AddData(UserBuffer::GetInstance()->PopDatabaseQueue());
            msleep(10);
        }
        qDebug() << QString("���ݿ�洢�߳�,����������!_") ;

    }catch(...)
    {
        qDebug() << QString("�߳��쳣!_%1").arg(GetLastError());
    }
}

void MyDatabaseThread::stop()
{
    m_stop = false;
}
