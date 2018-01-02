#include "connectionpool.h"
#include<QSettings>
#include <QApplication>
#include "config/config.h"
QMutex ConnectionPool::mutex;
QWaitCondition ConnectionPool::waitConnection;
ConnectionPool* ConnectionPool::instance = NULL;

ConnectionPool::ConnectionPool() {
    // �������ݿ����ӵ���Щ��Ϣ��ʵ�ʿ�����ʱ����Ҫͨ����ȡ�����ļ��õ���
    //


    QString strPath = QString(qApp->applicationDirPath()+"\\config\\config.ini");
    QSettings iniRead(strPath,QSettings::IniFormat);
    QString strHost = iniRead.value("SOCKET/host").toString();

//    hostName     = "127.0.0.1";
//    databaseName = "data_test";
//    username     = "root";
//    password     = "root";
//    databaseType = "QMYSQL";
//    testOnBorrow = true;
//    testOnBorrowSql = "SELECT 1";

//    maxWaitTime  = 1000;
//    waitInterval = 200;
//    maxConnectionCount  = 5;
    QMap<int,QString> map = Config::GetInstance()->GetMap(map_database);
    hostName     = map[0];
    databaseName = map[1];
    username     = map[2];
    password     = map[3];
    databaseType = map[4];
    testOnBorrow = map[5] =="1" ;
    testOnBorrowSql = map[6];

    maxWaitTime  = map[7].toInt();
    waitInterval = map[8].toInt();
    maxConnectionCount  = map[9].toInt();
}

ConnectionPool::~ConnectionPool() {
    // �������ӳص�ʱ��ɾ�����е�����
    foreach(QString connectionName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach(QString connectionName, unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

ConnectionPool& ConnectionPool::getInstance() {
    if (NULL == instance) {
        QMutexLocker locker(&mutex);

        if (NULL == instance) {
            instance = new ConnectionPool();
        }
    }

    return *instance;
}

void ConnectionPool::release() {
    QMutexLocker locker(&mutex);
    delete instance;
    instance = NULL;
}

QSqlDatabase ConnectionPool::openConnection() {

    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName;

    QMutexLocker locker(&mutex);

    // �Ѵ���������
    int connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();

    // ��������Ѿ����꣬�ȴ� waitInterval ���뿴���Ƿ��п������ӣ���ȴ� maxWaitTime ����
    for (int i = 0;
         i < pool.maxWaitTime
         && pool.unusedConnectionNames.size() == 0 && connectionCount == pool.maxConnectionCount;
         i += pool.waitInterval) {
        waitConnection.wait(&mutex, pool.waitInterval);

        // ���¼����Ѵ���������
        connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    }

    if (pool.unusedConnectionNames.size() > 0) {
        // ���Ѿ����յ����ӣ���������
        connectionName = pool.unusedConnectionNames.dequeue();
    } else if (connectionCount < pool.maxConnectionCount) {
        // û���Ѿ����յ����ӣ�����û�дﵽ������������򴴽��µ�����
        connectionName = QString("Connection-%1").arg(connectionCount + 1);
    } else {
        // �Ѿ��ﵽ���������
        qDebug() << "Cannot create more connections.";
        return QSqlDatabase();
    }

    // ��������
    QSqlDatabase db = pool.createConnection(connectionName);

    // ��Ч�����Ӳŷ��� usedConnectionNames
    if (db.isOpen()) {
        pool.usedConnectionNames.enqueue(connectionName);
    }

    return db;
}

void ConnectionPool::closeConnection(QSqlDatabase connection) {
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName = connection.connectionName();

    // ��������Ǵ��������ӣ��� used ��ɾ�������� unused ��
    if (pool.usedConnectionNames.contains(connectionName)) {
        QMutexLocker locker(&mutex);
        pool.usedConnectionNames.removeOne(connectionName);
        pool.unusedConnectionNames.enqueue(connectionName);
        waitConnection.wakeOne();
    }
}

QSqlDatabase ConnectionPool::createConnection(const QString &connectionName) {
    // �����Ѿ��������ˣ������������������´���
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);

        if (testOnBorrow) {
            // ��������ǰ�������ݿ⣬������ӶϿ������½�������
           // qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
            QSqlQuery query(testOnBorrowSql, db1);

            if (query.lastError().type() != QSqlError::NoError && !db1.open()) {
                qDebug() << "Open datatabase error:" << db1.lastError().text();
                return QSqlDatabase();
            }
        }

        return db1;
    }

    // ����һ���µ�����
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connectionName);
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(username);
    db.setPassword(password);

    if (!db.open()) {
        qDebug() << "Open datatabase error:" << db.lastError().text();
        return QSqlDatabase();
    }

    return db;
}
