#include "Define.h"
#include "Task.h"
#include "EvLoopTask.h"

struct ev_io_info EvLoopTask::m_ioArray[MAXFD];
struct ev_loop* EvLoopTask::m_Loop = NULL;
TASKQUEUE EvLoopTask::m_TaskQueue;
int EvLoopTask::m_clientCount = 0;

EvLoopTask::EvLoopTask (string ip, int port)
{
    m_Ip   = ip;
    m_Port = port;

    if ((m_listenFd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "ERROR: in socket ()\n";
        return;
    }

    EvLoopTask::setNonBlock (m_listenFd);
    EvLoopTask::setNodelay  (m_listenFd);
    EvLoopTask::setReuseAddr(m_listenFd);

    for (int i=0; i<MAXFD; ++i)
    {
        EvLoopTask::m_ioArray[i].fd = NULL;
    }
}

EvLoopTask::~EvLoopTask ()
{
    for (register int i=0; i<MAXFD; i++)
    {
        EvLoopTask::closeFd (i);
    }

    close (m_listenFd);
}

int EvLoopTask::Execute (void* data)
{
    initInetAddr ();
    ev_io  watcher;
    ev_timer timer;

    EvLoopTask::m_Loop = ev_loop_new (EVBACKEND_EPOLL);

    ev_io_init  (&watcher, handleAccept, m_listenFd, EV_READ);
    ev_io_start (EvLoopTask::m_Loop, &watcher);

    ev_timer_init  (&timer, handleTimeOut, 5, 5);
    ev_timer_start (EvLoopTask::m_Loop, &timer);

    ev_loop (EvLoopTask::m_Loop, 0);

    ev_loop_destroy (EvLoopTask::m_Loop);

    return (0);
}

void EvLoopTask::handleAccept (struct ev_loop* l, ev_io* w, int revents)
{
    struct sockaddr_in clientAddr;
    socklen_t sockLen = 0;
    int newFd = 0;

    newFd = accept (w->fd, (struct sockaddr*)&clientAddr, &sockLen);
    if (newFd < 0)
    {
        cout << "ERROR: in accept ()\n";
        return;
    }
    EvLoopTask::setNonBlock (newFd);
    EvLoopTask::setReuseAddr(newFd);
    EvLoopTask::setNodelay  (newFd);

    EvLoopTask::m_ioArray[newFd].fd = (ev_io*) malloc (sizeof (ev_io));
    EvLoopTask::m_ioArray[newFd].lasttime = ev_time();

    ev_io_init  (EvLoopTask::m_ioArray[newFd].fd, handleRecv, newFd, EV_READ);
    ev_io_start (EvLoopTask::m_Loop, EvLoopTask::m_ioArray[newFd].fd);

    EvLoopTask::m_clientCount++;
    return;
}

void EvLoopTask::handleRecv (struct ev_loop* l, ev_io* w, int revents)
{
    MemBlock* block = SINGLEFACTORY->m_memZone.malloc ();
    if (block == NULL)
    {
        cout << "MALLOC-ERROR: out of memory" << endl;
        return;
    }

#if 1
    int recv_len = 0;

    // packet header
    recv_len = SINGLEFACTORY->recvData (w->fd, block->data(), sizeof (int));
    if (recv_len != sizeof (int))
    {
        cout << "[Warning]: Packet header length-" << sizeof (int) \
             << ", actually received length-" << recv_len <<endl;
        block->reSet ();
        SINGLEFACTORY->m_memZone.free (block);
        EvLoopTask::closeFd(w->fd);
        return;
    }

    int *len = (int*)block->data();
    // packet body
    recv_len = SINGLEFACTORY->recvData (w->fd, (char*)block->data()+sizeof(int), *len-sizeof(int));
    if ((*len - sizeof (int)) != (unsigned int)recv_len)
    {
        cout << "FD=" << w->fd << ": length=" << *len - sizeof (int) << \
                ", actually recviced length=" << recv_len << endl;
        block->reSet ();
        SINGLEFACTORY->m_memZone.free (block);
        EvLoopTask::closeFd (w->fd);
        return;
    }

    EvLoopTask::m_ioArray[w->fd].lasttime = ev_time ();
    block->setFd (w->fd);
    SINGLEFACTORY->m_recvQueue.inQueue (block);
#endif
    return;
}


void EvLoopTask::handleTimeOut(struct ev_loop* l, struct ev_timer* t, int revents)
{
    ev_tstamp now = ev_time ();
    for (register int i=0; i<MAXFD; ++i)
    {
        if (m_ioArray[i].fd != NULL)
        {
            if (TIMEOUT < now - m_ioArray[i].lasttime)
            {
                EvLoopTask::closeFd (i);
            }
        }
    }
    return;
}

void EvLoopTask::setNonBlock (int fd)
{
    fcntl (fd, F_SETFL, fcntl (fd, F_GETFL, 0) | O_NONBLOCK);
}

void EvLoopTask::setReuseAddr(int fd)
{
    int reuse = 1;
    setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse));
}

void EvLoopTask::setNodelay (int fd)
{
    int nodelay = 1;
    setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof (nodelay));
}

void EvLoopTask::closeFd (int fd)
{
    if (fd)
    {
        close (fd);
        ev_io_stop (m_Loop, EvLoopTask::m_ioArray[fd].fd);
        free (EvLoopTask::m_ioArray[fd].fd);
        EvLoopTask::m_ioArray[fd].fd = NULL;
        EvLoopTask::m_clientCount--;
    }
}

int EvLoopTask::getClientCount (void)
{
    return EvLoopTask::m_clientCount;
}

int EvLoopTask::initInetAddr (void)
{
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons (m_Port);
    servAddr.sin_addr.s_addr = inet_addr (m_Ip.c_str());

    if (bind (m_listenFd, (struct sockaddr*)&servAddr, \
        sizeof (struct sockaddr)) < 0)
    {
        abort ();
        return (-1);
    }
    listen (m_listenFd, LISTEN_QMAX);
    return (m_listenFd);
}
