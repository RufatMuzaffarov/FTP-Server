#ifndef	EVLOOPTASK_H
#define EVLOOPTASK_H

#include "Define.h"
#include "Task.h"
#include "MemBlock.h"
#include "MemZone.h"
#include "SingleFactory.h"
#include "Queue.h"

// define libev io information
typedef struct ev_io_info
{
    public:
        struct ev_io*   fd;
        ev_tstamp       lasttime;
}   ev_io_info;

class EvLoopTask : public Task
{

    public:
         EvLoopTask (string p, int port);
        ~EvLoopTask ();

        virtual int Execute (void* data);
        int  getClientCount ();
        static void handleAccept (struct ev_loop* l, ev_io* w, int revents);
        static void handleRecv   (struct ev_loop* l, ev_io* w, int revents);
        static void handleTimeOut(struct ev_loop* l, struct ev_timer* t, int revents);

        static void setNonBlock (int fd);
        static void setNodelay  (int fd);
        static void setReuseAddr(int fd);
        static void closeFd     (int fd);

    public:
        static struct ev_loop* m_Loop;
        static struct ev_io_info m_ioArray[MAXFD];
        static TASKQUEUE m_TaskQueue;

    private:
        static int  m_clientCount;
        string m_Ip;
        int m_Port;
        int m_listenFd;
        int initInetAddr (void);
};


#endif	//EVLOOPTASK_H
