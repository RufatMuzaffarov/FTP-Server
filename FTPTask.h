#ifndef	FTPTASK_H
#define	FTPTASK_H

#include "Define.h"
#include "Task.h"

class FTPTask : public Task
{
    public:
        //FTPTask (bool exit) : Task (exit)
        FTPTask ()
        {
        }

        ~FTPTask () {};
        virtual int Execute (void* data);

    public:
        static TASKQUEUE m_TaskQueue;
};

#endif	//FTPTASK_H
