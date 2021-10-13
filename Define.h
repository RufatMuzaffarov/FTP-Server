#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <queue>
#include <pthread.h>
#include <list>
#include <signal.h>
#include <ev.h>
#include <fcntl.h>
#include <sys/select.h>

#define MAXFD       1024
#define TIMEOUT     10
#define LISTEN_QMAX 10
#define SIGINTERNAL 0x11

using namespace std;

typedef struct sRecvData
{
    unsigned int iLen;
    void* pData ()
    {
        return (this + 1);
    };

}   MSG_HEAD;

#endif //DEFINE_H
