#ifndef	SELECTLOOPTASK_H
#define SELECTLOOPTASK_H

#include "Define.h"
#include "Task.h"
#include "MemBlock.h"
#include "MemZone.h"
#include "SingleFactory.h"
#include "Queue.h"

// define select io information
typedef struct ev_io_info
{
    public:
        int              fd;
        time_t           lasttime;
}   ev_io_info;

class SelectLoopTask : public Task
{

    public:
         SelectLoopTask (string p, int port);
        ~SelectLoopTask ();

        virtual int Execute (void* data);
        int InIoArray (int fd);
        int getClientCount ();
        static int  handleAccept (int listenFd);
        static int  handleRecv   (int recvFd);
        static void handleTimeOut(int recvFd);

        static void setNonBlock  (int fd);
        static void setNodelay   (int fd);
        static void setReuseAddr (int fd);
        static void closeFd      (int fd);

    public:
        static struct ev_io_info m_ioArray[MAXFD];
        static TASKQUEUE m_TaskQueue;

    private:
        static int  m_clientCount;
        string m_Ip;
        int    m_Port;
        int    m_listenFd;

        int    initInetAddr (void);
};


#endif	//SELECTLOOPTASK_H
