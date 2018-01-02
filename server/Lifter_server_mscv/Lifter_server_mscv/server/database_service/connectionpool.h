#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H


#include <QtSql/QSqlDatabase>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include<QWaitCondition>
#include<QDebug>
#include"enum.h"
/*
����ģʽ
���ݿ����ӳ�
���ݿ����ӳص��ص�
    ��ȡ����ʱ����Ҫ�˽����ӵ�����
    ֧�ֶ��̣߳���֤��ȡ��������һ����û�б������߳�����ʹ��
    ���贴������
    ���Դ����������
    ���Կ������ӵ�����
    ���ӱ����ã�����ÿ�ζ����´���һ���µ�����
    ���ӶϿ��˺���Զ�����
    ���޿�������ʱ����ȡ���ӵ��̻߳�ȴ�һ��ʱ�䳢�Լ�����ȡ��ֱ����ʱ�Ż᷵��һ����Ч������
    �ر����Ӻܼ�
*/
class ConnectionPool {
public:
    static void release(); // �ر����е����ݿ�����
    static QSqlDatabase openConnection();                 // ��ȡ���ݿ�����
    static void closeConnection(QSqlDatabase connection); // �ͷ����ݿ����ӻ����ӳ�

    ~ConnectionPool();

private:

    static ConnectionPool& getInstance(); //ʵ����һ�����ӳض���

    ConnectionPool();
    ConnectionPool(const ConnectionPool &other);
    ConnectionPool& operator=(const ConnectionPool &other);
    QSqlDatabase createConnection(const QString &connectionName); // �������ݿ�����

    QQueue<QString> usedConnectionNames;   // ��ʹ�õ����ݿ�������
    QQueue<QString> unusedConnectionNames; // δʹ�õ����ݿ�������


    QString m_strPath; //��¼�����ļ��� ����·��
    // ���ݿ���Ϣ
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;

    bool    testOnBorrow;    // ȡ�����ӵ�ʱ����֤�����Ƿ���Ч
    QString testOnBorrowSql; // ���Է������ݿ�� SQL

    int maxWaitTime;  // ��ȡ�������ȴ�ʱ��
    int waitInterval; // ���Ի�ȡ����ʱ�ȴ����ʱ��
    int maxConnectionCount; // ���������

    static QMutex mutex;
    static QWaitCondition waitConnection;
    static ConnectionPool *instance;
};



#endif // CONNECTIONPOOL_H
