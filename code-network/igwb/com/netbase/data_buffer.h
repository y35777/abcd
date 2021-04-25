#ifndef  __DATA_BUFFER_H__
#define  __DATA_BUFFER_H__

#include "../include/frame.h"

class CDataBuffer
{
public:
    CDataBuffer(UINT4 uBufLen = 0);
    virtual ~CDataBuffer(void);

    //重新分配缓冲区
    void Alloc(UINT4 uBufLen);

    //将从pBuf开始的uBufLen个字节拷贝到内部缓冲区中
    int Put(const char* const pBuf, UINT4 const uBufLen);

    //获得内部缓冲区的指针
    void Get(char*& pBuf, UINT4& uBufLen, BOOL const bReset = TRUE);

protected:

    //该指针指向用于临时存放将要打包发送给前存盘模块的数据的缓冲区
    //该缓冲区在构造函数或Alloc函数中创建，在析构函数中释放
    BYTE* m_pBuffer;

    //该成员变量表示m_pBuffer缓冲区的长度
    //该成员变量在构造函数或Alloc函数中设置
    UINT4 m_uBufLen;

    //该成员变量表示缓冲区m_pBuffer中已经写入的数据的字节数；
    UINT4 m_uWritten;

};

#endif  //__DATA_BUFFER_H__
