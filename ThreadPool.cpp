#include "Define.h"
#include "ThreadPool.h"

using namespace std;

bool ThreadPool::initThreads ()
{
    for (unsigned int i=0; i<m_threadCount; i++)
    {
        pthread_t id = 0;
        pthread_create (&id, 0, ThreadPool::threadFunc, this);
        m_threadList.push_back (id);
    }

    return (true);
}

bool ThreadPool::reSize (unsigned int num)
{
    MutexLockGuard guard (m_mutexLock);
    int count = (int) (num-m_threadCount);
    if (count > 0)
    {
        for (int i=0; i<count; ++i)
        {
            cout << i << ":::reSize of thread list:::" << count << endl;
            pthread_t id = 0;
            pthread_create (&id, 0, ThreadPool::threadFunc, this);
            m_threadList.push_back (id);
        }
    }
    m_threadCount = num;

    return (true);
}

bool ThreadPool::runAll ()
{
    return (initThreads ());
}

bool ThreadPool::joinTask (Task* t)
{
    return (this->m_taskQueue.inQueue (t));
}

void* ThreadPool::threadFunc (void* data)
{
    cout << "threadfunc\n";
    (void)signal (SIGINTERNAL, ThreadPool::sigHandler);
    ThreadPool* tp = static_cast<ThreadPool*> (data);
    Task* t = NULL;
    while (!tp->m_isExit)
    {
        if (tp->m_taskQueue.outQueue (t, 2) == true && t)
        {
            t->Execute (NULL);
            if (t->isAutoExit ())
            {
                delete t;
                t = NULL;
            }
        }
#if 1
        else
        {
            MutexLockGuard* guard = new MutexLockGuard (tp->m_mutexLock);
            if (tp->m_threadCount < tp->m_threadList.size ())
            {
                cout << "...........................................:" << pthread_self () << endl;
                tp->m_threadList.remove (pthread_self ());
                delete guard;
                return (NULL);
            }
            delete guard;
        }
#endif
    }
    MutexLockGuard guard (tp->m_mutexLock);
    tp->m_threadList.remove (pthread_self ());
    return (NULL);
}

bool ThreadPool::stopPool (void)
{
    m_isExit = true;
    while (this->getThreadCount() != 0)
    {
        usleep (100);
    }

    return (false);
}

int ThreadPool::killAll (void)
{
    MutexLockGuard guard (this->m_mutexLock);
    while (getThreadCount() != 0)
    {
        pthread_kill (m_threadList.front (), SIGINTERNAL);
        m_threadList.pop_front ();
    }

    return (0);
}

void ThreadPool::sigHandler (int sig)
{
    printf ("pthread id=[%ld] exit\n", pthread_self ());
    pthread_exit (NULL);

    return;
}

int ThreadPool::getThreadCount ()
{
    MutexLockGuard guard (m_mutexLock);
    return (m_threadList.size ());
}

int ThreadPool::getTaskCount ()
{
    return (m_taskQueue.getCount ());
}
