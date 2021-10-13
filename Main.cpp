#include "Define.h"
#include "ThreadPool.h"
#include "FTPTask.h"
#include "SendTask.h"
#include "MutexLock.h"
#include "Task.h"
#include "EvLoopTask.h"
#include "Queue.h"
#include "MemZone.h"
#include "MemBlock.h"

void sig1 (int sig)
{
    printf ("get exit signal\n");
    raise (SIGKILL);
    return;
}

int main (int argc, char** argv)
{
    signal (SIGINT,  SIG_IGN);
    signal (SIGPIPE, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTERM, sig1);

#if 1
    ThreadPool* pool = new ThreadPool(3);
    pool->runAll ();

    EvLoopTask *    evLoopTask = new EvLoopTask ("127.0.0.1", 9999);
    FTPTask *       ftpTask    = new FTPTask ();
    SendTask *      sendTask   = new SendTask ();

    pool->joinTask (evLoopTask);
    pool->joinTask (ftpTask);
    pool->joinTask (sendTask);


    pool->reSize ((unsigned int)3);

    while (true)
    {
        cout << "<<<<<<< main loop >>>>>>>" << endl;
        sleep (5);
    }
    pool->killAll ();
#endif

    return (0);
}
