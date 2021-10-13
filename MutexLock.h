#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include "Define.h"
#include <pthread.h>

class MutexLock
{
    public:
        MutexLock ()
        {
            pthread_mutex_init (&m_mutex, NULL);
        }

        ~MutexLock ()
        {
            pthread_mutex_destroy (&m_mutex);
        }

        void enLock (void)
        {
            pthread_mutex_lock (&m_mutex);
        }

        void deLock (void)
        {
            pthread_mutex_unlock (&m_mutex);
        }

    private:
        MutexLock (const MutexLock&);
        const MutexLock& operator=(const MutexLock&);
        pthread_mutex_t m_mutex;
};


class MutexLockGuard
{
    public:
        explicit MutexLockGuard (MutexLock& lock) : m_lock(lock)
        {
            m_lock.enLock ();
        }

        ~MutexLockGuard (void)
        {
            m_lock.deLock ();
        }

    private:
        MutexLockGuard (const MutexLockGuard&);
        const MutexLockGuard& operator=(const MutexLockGuard&);
        MutexLock& m_lock;
};

#endif //MUTEXLOCK_H
