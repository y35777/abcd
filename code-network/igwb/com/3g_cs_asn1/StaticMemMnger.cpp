// StaticMemMnger.cpp: implementation of the CStaticMemMnger class.
//
//////////////////////////////////////////////////////////////////////

#include "StaticMemMnger.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticMemMnger::CStaticMemMnger(size_t nBlockSize)
{
    m_nBlockSize = nBlockSize;
    if(m_nBlockSize < MIN_MEM_BLOCK_SIZE)
    {
        m_nBlockSize = MIN_MEM_BLOCK_SIZE;
    }
    if(m_nBlockSize > MAX_MEM_BLOCK_SIZE)
    {
        m_nBlockSize = MAX_MEM_BLOCK_SIZE;
    }

    m_nCurrentBlock = -1;
}

CStaticMemMnger::~CStaticMemMnger()
{
    int nCount = m_vMemInfo.size();
    for(int i = 0; i < nCount; i++)
    {
        if(m_vMemInfo[i].pMemBlock != NULL)
        {
            delete []m_vMemInfo[i].pMemBlock;
            m_vMemInfo[i].pMemBlock = NULL;
        }
    }
}

void* CStaticMemMnger::Allocate(size_t nSize)
{
    if((nSize == 0) || (nSize > m_nBlockSize))
    {
        return NULL;
    }

    if ((m_nCurrentBlock == -1)
        ||(m_nBlockSize - m_vMemInfo[m_nCurrentBlock].nActSize < nSize))
    {  
        if(m_nCurrentBlock + 1 > (int)(m_vMemInfo.size()) - 1)
        {
            void* p = new BYTE[m_nBlockSize];
            if(p == NULL)
            {
                return NULL;
            }

            SBlockInfo stBlock;
            stBlock.nActSize = 0;
            stBlock.pMemBlock = p;
            m_vMemInfo.push_back(stBlock);
        }

        m_nCurrentBlock++;
    }


    BYTE* p = (BYTE*)m_vMemInfo[m_nCurrentBlock].pMemBlock;
    p += m_vMemInfo[m_nCurrentBlock].nActSize;

    m_vMemInfo[m_nCurrentBlock].nActSize += nSize;

    return (void*)p;
}

void CStaticMemMnger::Deallocate()
{
    int nCount = m_vMemInfo.size();
    for(int i = 0; i < nCount; i++)
    {
        m_vMemInfo[i].nActSize = 0;
    }
    
    m_nCurrentBlock = -1;
}

void* operator new( size_t classSize, CStaticMemMnger* pMemMnger)
{
    if(pMemMnger != NULL)
    {
        return pMemMnger->Allocate(classSize);
    }
    else
    {
        return NULL;
    }
}