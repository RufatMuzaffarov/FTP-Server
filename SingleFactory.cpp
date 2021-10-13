#include "Define.h"
#include "SingleFactory.h"

class SingleFactory* SingleFactory::m_staticObject = NULL;

class SingleFactory* SingleFactory::instance ()
{
    if (m_staticObject == NULL)
    {
        m_staticObject = new SingleFactory ();
    }

    return m_staticObject;
}

SingleFactory::SingleFactory ()
{
}

SingleFactory::~SingleFactory ()
{
}

int SingleFactory::recvData (int fd, void* buffer, unsigned int len)
{
    if (buffer == NULL)
        return (-1);

    int recv_len = 0;
    int l = 0;
    while (len != 0)
    {
        l = recv (fd, (char*)buffer + recv_len, len, 0);
        if (errno == EINTR)
        {
            usleep (100);
            continue;
        }
        else if (errno == EAGAIN)
        {
            usleep (100);
            continue;
        }

        if (l == 0)
        {
            break;
        }
        else if (l == -1)
        {
            return (-1);
        }

        len -= l;
        recv_len += l;
    }

    return (recv_len);
}

int SingleFactory::sendData (int fd, void* buffer, unsigned int len)
{
    if (buffer == NULL)
        return (-1);

    int bytes_left = len;
    int written_bytes;
    char* ptr = (char*) buffer;

    while (bytes_left > 0)
    {
        written_bytes = send (fd, ptr, bytes_left, 0);
        if (written_bytes <= 0)
        {
            if (errno == EINTR)
            {
                if (written_bytes < 0)
                {
                    written_bytes = 0;
                    usleep (100);
                    continue;
                }
            }
            else if (errno == EAGAIN)
            {
                if (written_bytes < 0)
                {
                    written_bytes = 0;
                    usleep (100);
                    continue;
                }
            }
            else
            {
                break;
            }
        }

        bytes_left -= written_bytes;
        ptr += written_bytes;
    }

    return (0);
}
