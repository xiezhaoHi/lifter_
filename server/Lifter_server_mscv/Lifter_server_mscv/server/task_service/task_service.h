#ifndef _TASK_SERVICE_H_
#define _TASK_SERVICE_H_

#include <QRunnable>

#include "i_service.h"

class CTaskService : public IInternalService
{
public:
    static CTaskService * GetInstance();

    static void DestroyInstance();

    virtual int StartInternalService();

    virtual int StopInternalService();

    virtual int NotifyServiceToStop();

    virtual QString GetServiceName();

    void CommitTask(QRunnable *task);

private:

    CTaskService();

    ~CTaskService();

private:

    static CTaskService * m_pInstance;


};

#endif//!_TASK_SERVICE_H_
