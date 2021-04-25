/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:����̲���
    �ļ���:StaticMemMnger.h
    ������Ϣ:
    
    ����CStaticMemMnger��Ķ���,�����Զ�����ڴ�صĹ���
    ������߷����ڴ���ٶȡ�

    ���������������ʹ�ã����ڴ�����������ڴ治���ǹ����ڴ档
    �������ֿռ�ļ����˷ѻ���䲻�ɹ��������

    Ŀǰʹ����ASN.1����ǰ�ṹ������ÿ�η������һ����С���ڴ档

    �޸ļ�¼:
    ������,  2004-02 ,   ����.   
*/
//////////////////////////////////////////////////////////////////////

#ifndef __STATIC_MEM_MANAGER_H__
#define __STATIC_MEM_MANAGER_H__

#include "../include/base_type.h"

#define MIN_MEM_BLOCK_SIZE       1 * 1024
#define MAX_MEM_BLOCK_SIZE       100 * 1024 * 1024

class CStaticMemMnger  
{
public:
	CStaticMemMnger(size_t nBlockSize = 1024 * 1024);
	virtual ~CStaticMemMnger();

    void* Allocate(size_t nSize);
    void  Deallocate();
protected:

    //ÿ�η���һ���ڴ�Ĵ�С
    size_t          m_nBlockSize;   

    struct  SBlockInfo
    {
        size_t      nActSize;
        void*       pMemBlock;
    };
    
    //�洢�������ڴ�����Ϣ
    VECTOR<SBlockInfo>   m_vMemInfo;
    int                  m_nCurrentBlock;

};

void* operator new( size_t classSize, CStaticMemMnger* pMemMnger);

#endif // __STATIC_MEM_MANAGER_H__