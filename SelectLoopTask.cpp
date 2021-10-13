#include "Define.h"
#include "Task.h"
#include "SelectLoopTask.h"

struct ev_io_info SelectLoopTask::m_ioArray[MAXFD];
TASKQUEUE SelectLoopTask::m_TaskQueue;
int SelectLoopTask::m_clientCount = 0;

SelectLoopTask::SelectLoopTask (string ip, int port)
{
    m_Ip   = ip;
    m_Port = port;

    if ((m_listenFd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "ERROR: in socket ()\n";
        return;
    }

    SelectLoopTask::setNonBlock (m_listenFd);
    SelectLoopTask::setNodelay  (m_listenFd);
    SelectLoopTask::setReuseAddr(m_listenFd);

    for (int i=0; i<MAXFD; ++i)
    {
        SelectLoopTask::m_ioArray[i].fd = -1;
    }
}

SelectLoopTask::~SelectLoopTask ()
{
    for (register int i=0; i<MAXFD; i++)
    {
        SelectLoopTask::closeFd (i);
    }

    close (m_listenFd);
}

int SelectLoopTask::InIoArray (int fd)
{
    if (SelectLoopTask::m_ioArray[fd].fd == fd)
        return (1);
    else
        return (0);
}

int SelectLoopTask::Execute (void* data)
{
    fd_set readfds, testfds;
    int    result, connfd, fd;
    // initialize read fds array and add server-fd to that..
    FD_ZERO (&readfds);
    FD_SET (m_listenFd, &readfds);

    // loop and accept request from clients
    while (1) {
        testfds = readfds;

        printf ("server waiting...\n");
        result = select (FD_SETSIZE, &testfds, (fd_set*)NULL, (fd_set*)NULL, (struct timeval*)0);
        if (result < 1) {
            perror ("FTP-Server: select");
            return (1);
        }

        // check fd, if already then goto accpet request from client
        for (fd=0; fd < FD_SETSIZE; fd++)
        {
            if (FD_ISSET (fd, &testfds))
            {
                // is listen-fd
                if (fd == m_listenFd)
                {
                    connfd = handleAccept (m_listenFd);
                    FD_SET (connfd, &readfds);
                }
                // is client-fd
                else if (InIoArray (fd))
                {
                    if (handleRecv (fd) == 1)
                    {
                        FD_CLR (fd, &readfds);
                    }
                }
            }
        }
    }
}

int SelectLoopTask::handleAccept (int listenFd)
{
    struct sockaddr_in clientAddr;
    socklen_t sockLen = 0;
    int newFd = 0;

    newFd = accept (listenFd, (struct sockaddr*)&clientAddr, &sockLen);
    if (newFd < 0)
    {
        cout << "ERROR: in accept ()\n";
        abort ();
        return (-1);
    }
    SelectLoopTask::setNonBlock (newFd);
    SelectLoopTask::setReuseAddr(newFd);
    SelectLoopTask::setNodelay  (newFd);

    SelectLoopTask::m_ioArray[newFd].fd = newFd;
    SelectLoopTask::m_ioArray[newFd].lasttime = time (NULL);

    SelectLoopTask::m_clientCount++;
    return (newFd);
}

int SelectLoopTask::handleRecv (int recvFd)
{
    MemBlock* block = SINGLEFACTORY->m_memZone.malloc ();
    if (block == NULL)
    {
        cout << "MALLOC-ERROR: out of memory" << endl;
        return (-1);
    }

#if 1
    int recv_len = 0;

    // packet header
    recv_len = SINGLEFACTORY->recvData (recvFd, block->data(), sizeof (int));
    if (recv_len != sizeof (int))
    {
        cout << "[Warning]: Packet header length-" << sizeof (int) \
             << ", actually received length-" << recv_len <<endl;
        block->reSet ();
        SINGLEFACTORY->m_memZone.free (block);
        SelectLoopTask::closeFd(recvFd);
        return (-1);
    }

    int *len = (int*)block->data();
    // packet body
    recv_len = SINGLEFACTORY->recvData (recvFd, (char*)block->data()+sizeof(int), *len-sizeof(int));
    if ((*len - sizeof (int)) != (unsigned int)recv_len)
    {
        cout << "FD=" << recvFd << ": length=" << *len - sizeof (int) << \
                ", actually recviced length=" << recv_len << endl;
        block->reSet ();
        SINGLEFACTORY->m_memZone.free (block);
        SelectLoopTask::closeFd (recvFd);
        return (-1);
    }

    SelectLoopTask::m_ioArray[recvFd].lasttime = time (NULL);
    block->setFd (recvFd);
    SINGLEFACTORY->m_recvQueue.inQueue (block);
#endif
    return (1);
}

void SelectLoopTask::handleTimeOut (int i)
{
    time_t now = time (NULL);
    for (register int i=0; i<MAXFD; ++i)
    {
        if (i == -1)
        {
            if (TIMEOUT < now - m_ioArray[i].lasttime)
                SelectLoopTask::closeFd (i);
        }
    }
    return;
}

void SelectLoopTask::setNonBlock (int fd)
{
    fcntl (fd, F_SETFL, fcntl (fd, F_GETFL, 0) | O_NONBLOCK);
}

void SelectLoopTask::setReuseAddr(int fd)
{
    int reuse = 1;
    setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse));
}

void SelectLoopTask::setNodelay (int fd)
{
    int nodelay = 1;
    setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof (nodelay));
}

void SelectLoopTask::closeFd (int fd)
{
    if (fd)
    {
        close (fd);
        SelectLoopTask::m_clientCount--;
    }
}

int SelectLoopTask::getClientCount (void)
{
    return SelectLoopTask::m_clientCount;
}

int SelectLoopTask::initInetAddr (void)
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
