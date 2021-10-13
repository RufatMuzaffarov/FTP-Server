#include "SendTask.h"
#include "MemZone.h"
#include "MemBlock.h"
#include "SingleFactory.h"

int SendTask::Execute (void*)
{
    while (true)
    {
        MemBlock* block;
        if (SINGLEFACTORY->m_sendQueue.outQueue (block, 2) == false)
        {
            continue;
        }

        int fd = block->getFd ();

        unsigned int written_bytes;
        unsigned int left_bytes = block->getSize();
        char* ptr = (char*) block->data();

        while (left_bytes > 0)
        {
            written_bytes = send (fd, ptr, left_bytes, 0);
            if (errno == EINTR)
            {
                if (written_bytes < 0) written_bytes = 0;
                continue;
            }

            if (errno == EAGAIN)
            {
                if (written_bytes < 0) written_bytes = 0;
                usleep (200);
                continue;
            }

            else
            {
                break;
            }

            ptr += written_bytes;
            left_bytes -= written_bytes;
        }

        cout << "FD=" << fd << " -- send success" << endl;

        block->reSet();
        SINGLEFACTORY->m_memZone.free (block);
    }

    return (0);
}
