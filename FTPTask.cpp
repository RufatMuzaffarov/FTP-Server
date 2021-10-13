#include "Define.h"
#include "FTPTask.h"
#include "MemZone.h"
#include "MemBlock.h"
#include "SingleFactory.h"

TASKQUEUE FTPTask::m_TaskQueue;

int FTPTask::Execute (void* data)
{
    while (1)
    {
        MemBlock* block = NULL;

        if (SINGLEFACTORY->m_recvQueue.outQueue (block, 2) == false)
        {
            continue;
        }

        cout << "---------------- Task Execute ----------------" << endl;
        cout << "RecvData: " << (char* )((char *)block->data() + sizeof (int)) << endl;
        cout << "RecvLen : " <<  (((MSG_HEAD*) block->data())->iLen) << endl << endl;

#ifdef _ONLY_TEST
        block->reSet ();
        SINGLEFACTORY->m_memZone.free (block);

#if 0
        MemBlock* pblock = SINGLEFACTORY->m_memZone.malloc ();
        if (pblock == NULL)
        {
            cout << "FTPTask::Execute: out of memory\n";
            return -1;
        }
        pblock->setFd (block->getFd ());
        MSG_HEAD* sendData = (MSG_HEAD*) pblock->data();
        sendData->iLen = sizeof (MSG_HEAD) + 10;
        char      buffer[11] = "0987654321";
        memcpy (sendData->pData(), &buffer, sizeof (buffer));

        pblock->setSize (sendData->iLen);
        SINGLEFACTORY->m_sendQueue.inQueue (pblock);
#endif
        usleep (100);
#endif
    }
    return 0;
}
