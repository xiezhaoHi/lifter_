#include "task_service.h"
#include <QThreadPool>
#include <QTime>

CTaskService * CTaskService::m_pInstance = NULL;


CTaskService::CTaskService()
{

}

CTaskService::~CTaskService()
{

}

CTaskService * CTaskService::GetInstance()
{
    if (!m_pInstance) {
        m_pInstance = new CTaskService();
    }
    return m_pInstance;
}

void CTaskService::DestroyInstance()
{
    if (m_pInstance) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

int CTaskService::StartInternalService()
{
    static int nMaxThreadCount = 30;
    QThreadPool::globalInstance()->setExpiryTimeout(60 * 3);// 3分钟没有可执行的任务，则销毁线程
    if (QThreadPool::globalInstance()->maxThreadCount() < nMaxThreadCount) {
        QThreadPool::globalInstance()->setMaxThreadCount(nMaxThreadCount);
    }

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);// 设置随机数种子

    return 0;
}

int CTaskService::StopInternalService()
{
    QThreadPool::globalInstance()->waitForDone();// 等待所有线程退出
    return 0;
}

int CTaskService::NotifyServiceToStop()
{
    return 0;
}

QString CTaskService::GetServiceName()
{
    return QStringLiteral("task service");
}

void CTaskService::CommitTask(QRunnable *task)
{
    task->setAutoDelete(true);
    QThreadPool::globalInstance()->start(task);
}


