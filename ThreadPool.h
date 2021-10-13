#ifndef	THREADPOOL_H
#define THREADPOOL_H

#include "Define.h"
#include "Queue.h"
#include "Task.h"
#include "MutexLock.h"

class ThreadPool
{

    typedef list <pthread_t> THREADLIST;
    typedef Queue<Task*> TASKQUEUE;

    public:
        ThreadPool (unsigned int num) : m_threadCount(num)
        {
            m_isExit = false;
        }

        ~ThreadPool ()
        {
        }

        bool initThreads (void);
        bool runAll (void);
        bool joinTask (Task* t);
        bool stopPool (void);
        bool reSize (unsigned int num);

        static void  sigHandler (int sig);
        static void* threadFunc (void* data);

        int killAll (void);
        int getThreadCount (void);
        int getTaskCount (void);

    private:
        THREADLIST    m_threadList;
        TASKQUEUE     m_taskQueue;
        unsigned int    m_threadCount;
        MutexLock     m_mutexLock;
        bool            m_isExit;
};

#endif  //THREADPOOL_H
