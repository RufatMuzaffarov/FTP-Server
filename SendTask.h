#ifndef	SENDTASK_H
#define	SENDTASK_H

#include "Define.h"
#include "Task.h"

class SendTask : public Task
{
    public:
        //SendTask (bool exit) : Task (exit)
        SendTask ()
        {
        }

        ~SendTask () {};
        virtual int Execute (void* data);

    public:
        static TASKQUEUE m_TaskQueue;
};

#endif	//SENDTASK_H
