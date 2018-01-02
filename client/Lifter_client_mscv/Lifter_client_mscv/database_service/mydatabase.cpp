#include "mydatabase.h"
#include"usebuffer.h"
#include<QTime>
#include<QThread>
#include<QSqlRecord>
#include<QTextCodec>
MyDatabase::MyDatabase()
{
    m_stop = false;
    for(int index = 0; index < MAX_THREAD; ++index)
        thread_array[index]= new DataBase_thread();
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
            thread_array[index]->start(100);
    }
}
  MyDatabase* MyDatabase::GetInstance(void)
 {
    if(NULL == m_singleton)
        m_singleton = new MyDatabase();
    return m_singleton;
 }

//�����ݿ��������
bool  MyDatabase::AddData(QString const& strSql)
{
    if(strSql.isEmpty())
        return false;
    QSqlDatabase db =  ConnectionPool::openConnection();
    QSqlQuery sql_query(db);
    bool retFlag = false;
//    QList<QString>::const_iterator it = strSql.begin(),itend = strSql.end();
//    int i = 0;
//    QVariantList list;
//    for (;it != itend; it++,i++)
//    {
     sql_query.prepare(strSql);
//    }
// qDebug()<<sql_query.lastError();

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
//ɾ��ָ����
bool DeleteData(QString const& strSql)
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
bool ChangeData(QString const& strSql)
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
bool MyDatabase::SelectData(QString const& strSql,QStringList**& pList, int& count)
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
    ConnectionPool::closeConnection(db);
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
}

void MyMoveToThread::MybeginThread()
{
    qDebug() << QThread::currentThreadId();
    QTime timeOld,timeNew;
    timeOld= QTime::currentTime();


        //����
         //QString str = QString("INSERT INTO `data_test`.`monitordate` (`MonitorCode`, `MonitorName`, `MonitorType`, `MonitorDesc`, `MonitorResult`, `MonitorFromElevator`) VALUES ('1', 'jsd', '08:58:39', 'sm', '5551aaffc4ffec07f61f1a', '1');");

         QStringList strList;
         int iCount = 0;
        while(UserBuffer::GetInstance()->ReturnDatabaseQueueSize()<100 && iCount++<10)
        {
              ::Sleep(100);
        }
//             strList.append(UserBuffer::GetInstance()->PopDatabaseQueue());
//             for(int index = 0;index < UserBuffer::GetInstance()->ReturnDatabaseQueueSize(); ++index)
//             {
//             strList.append(UserBuffer::GetInstance()->PopDatabaseQueue());
//            }
              MyDatabase::GetInstance()->AddData(UserBuffer::GetInstance()->PopDatabaseQueue());


}
