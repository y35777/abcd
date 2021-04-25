/*     
    产品名:iGateway Bill V200
    模块名:后存盘补丁
    文件名:StaticMemMnger.h
    描述信息:
    
    包含CStaticMemMnger类的定义,该类自定义对内存池的管理，
    用于提高分配内存的速度。

    必须在这个条件下使用：向内存池请求分配的内存不能是过大内存。
    否则会出现空间的极度浪费或分配不成功的情况。

    目前使用于ASN.1编码前结构，其中每次分配的是一个较小的内存。

    修改记录:
    陈凉威,  2004-02 ,   创建.   
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

    //每次分配一块内存的大小
    size_t          m_nBlockSize;   

    struct  SBlockInfo
    {
        size_t      nActSize;
        void*       pMemBlock;
    };
    
    //存储了所有内存块的信息
    VECTOR<SBlockInfo>   m_vMemInfo;
    int                  m_nCurrentBlock;

};

void* operator new( size_t classSize, CStaticMemMnger* pMemMnger);

#endif // __STATIC_MEM_MANAGER_H__