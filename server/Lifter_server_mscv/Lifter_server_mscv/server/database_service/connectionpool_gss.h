#ifndef ConnectionPool_gss_H
#define ConnectionPool_gss_H


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
单利模式
数据库连接池
数据库连接池的特点
    获取连接时不需要了解连接的名字
    支持多线程，保证获取到的连接一定是没有被其他线程正在使用
    按需创建连接
    可以创建多个连接
    可以控制连接的数量
    连接被复用，不是每次都重新创建一个新的连接
    连接断开了后会自动重连
    当无可用连接时，获取连接的线程会等待一定时间尝试继续获取，直到超时才会返回一个无效的连接
    关闭连接很简单
*/
class ConnectionPool_gss {
public:
    static void release(); // 关闭所有的数据库连接
    static QSqlDatabase openConnection();                 // 获取数据库连接
    static void closeConnection(QSqlDatabase connection); // 释放数据库连接回连接池

    ~ConnectionPool_gss();

private:

    static ConnectionPool_gss& getInstance(); //实例化一个连接池对象

    ConnectionPool_gss();
    ConnectionPool_gss(const ConnectionPool_gss &other);
    ConnectionPool_gss& operator=(const ConnectionPool_gss &other);
    QSqlDatabase createConnection(const QString &connectionName); // 创建数据库连接

    QQueue<QString> usedConnectionNames;   // 已使用的数据库连接名
    QQueue<QString> unusedConnectionNames; // 未使用的数据库连接名


    QString m_strPath; //记录配置文件的 绝对路径
    // 数据库信息
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;

    bool    testOnBorrow;    // 取得连接的时候验证连接是否有效
    QString testOnBorrowSql; // 测试访问数据库的 SQL

    int maxWaitTime;  // 获取连接最大等待时间
    int waitInterval; // 尝试获取连接时等待间隔时间
    int maxConnectionCount; // 最大连接数

    static QMutex mutex;
    static QWaitCondition waitConnection;
    static ConnectionPool_gss *instance;
};



#endif // ConnectionPool_gss_H
