#include "MemBlock.h"

MemBlock::MemBlock (unsigned int iSize)
{
    this->m_size = iSize;
    m_memPtr = malloc (iSize);
}

MemBlock::~MemBlock (void)
{
    if (m_memPtr)
    {
        free (m_memPtr);
        m_memPtr = NULL;
    }
}

void MemBlock::setSize (unsigned int i)
{
    m_size = i;
}

unsigned int MemBlock::getSize (void)
{
    return (m_size);
}

unsigned int MemBlock::getUsedSize (void)
{
    return (m_usedSize);
}

void MemBlock::reSet (void)
{
    (void) memset (m_memPtr, 0x00, m_size);
}

void* MemBlock::data (void)
{
    return (m_memPtr);
}

unsigned int MemBlock::getIndex (void)
{
    return (m_index);
}

bool MemBlock::isUsed (void)
{
    return (m_isUsed);
}

void MemBlock::setIndex (unsigned int i)
{
    m_index = i;
}

void MemBlock::setIsUsed (bool b)
{
    m_isUsed = b;
}

int MemBlock::getFd (void)
{
    return (m_fd);
}
void MemBlock::setFd(int fd)
{
    this->m_fd = fd; 
}
