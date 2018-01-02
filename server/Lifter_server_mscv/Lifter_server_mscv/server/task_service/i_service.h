#ifndef _I_SERVICE_H_
#define _I_SERVICE_H_

#include <QString>
#include <QMutex>
#include <QMutexLocker>

class IInternalService
{
public:

    IInternalService() 
    {
        m_bNotifyToStop = false;
        m_bWorking = false;
    }

    virtual ~IInternalService() {};

    virtual int StartInternalService() = 0;

    virtual int StopInternalService() = 0;

    virtual int NotifyServiceToStop() { m_bNotifyToStop = true; return 0; }

    virtual bool IsStopSingalTriggered() { return m_bNotifyToStop; }

    virtual QString GetServiceName() { return QStringLiteral("null service name"); }

private:
    QMutex  m_mutexForVal;
    bool    m_bNotifyToStop;// 通知
    bool    m_bWorking;     // 是否仍在工作
};

#endif//!_I_SERVICE_H_