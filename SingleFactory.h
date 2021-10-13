#ifndef SINGLEFACTORY
#define SINGLEFACTORY

#include <stdio.h>
#include "Task.h"
#include "Queue.h"
#include "MemZone.h"
#include "MemBlock.h"

// only instance (global)
class SingleFactory
{
    public:
        ~SingleFactory ();
        static class SingleFactory* instance();

    public:
        // receive queue (evLoop ----> FTPTask)
        TASKQUEUE m_recvQueue;
        // send queue (FTPTask ----> SendTask)
        TASKQUEUE m_sendQueue;
        // memory-manager
        MemZone <MemBlock > m_memZone;

        // receive data handler
        static int recvData (int fd, void* buffer, unsigned int len);
        // send data handler
        static int sendData (int fd, void* buffer, unsigned int len);

    private:
        SingleFactory ();
        // single pattern
        static class SingleFactory* m_staticObject;
};

#define SINGLEFACTORY SingleFactory::instance()

#endif  //SINGLEFACTORY_H
