#ifndef TASK_H
#define TASK_H

#include "Define.h"
#include "Queue.h"
#include "MemBlock.h"

typedef Queue <MemBlock*> TASKQUEUE;

class Task
{
    public:
        //Task (bool isExit) : m_AutoExit (isExit)
        Task () : m_AutoExit (true)
        {
        }

        virtual ~Task () {};
        virtual int Execute (void* data) = 0;
        bool isAutoExit (void)
        {
            return m_AutoExit;
        }

    private:
        bool m_AutoExit;
        
};

#endif  //TASK_H
