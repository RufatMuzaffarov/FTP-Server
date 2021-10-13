#ifndef MEMZONE_H
#define MEMZONE_H

#include "Define.h"
#include "MutexLock.h"

#define MEMBLOCKNUM     2048

template <typename T>
class MemZone
{
    typedef queue<T*> QUEUEBLOCK;

    public:
        MemZone (int iCount = MEMBLOCKNUM)
        {
            MutexLockGuard guard (m_mutexLock);
            this->m_blockCount = iCount;
            for (int i=0; i<this->m_blockCount; i++)
            {
                T* t = new T;
                m_queueBlock.push (t);
            }
        }

        ~MemZone (void)
        {
            MutexLockGuard guard (m_mutexLock);
            for (int i=0; i<this->m_blockCount; i++)
            {
                T* t = m_queueBlock.front ();
                m_queueBlock.pop ();
                delete t;
            }
        }

        T* malloc (void)
        {
            MutexLockGuard guard (m_mutexLock);
            if (m_queueBlock.size () == 0)
            {
                return (NULL);
            }
            T* p = m_queueBlock.front ();
            m_queueBlock.pop ();
            return (p);
        }

        bool free (T* t)
        {
            MutexLockGuard guard (m_mutexLock);
            if (t == NULL) {
                return (false);
            }
            m_queueBlock.push (t);
            return (true);
        }

        int getCount (void)
        {
            return (m_blockCount);
        }

        int getUsedCount (void)
        {
            MutexLockGuard guard (m_mutexLock);
            return (m_blockCount - \
                    m_queueBlock.size ());
        }

    private:
        QUEUEBLOCK   m_queueBlock;
        MutexLock  m_mutexLock;
        int          m_blockCount;
};

#endif  //MEMZONE_H
