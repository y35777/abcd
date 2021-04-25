#include "data_buffer.h"
#include "resource.h"

/******************************************************
// 函数名:  CDataBuffer::CDataBuffer
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    构造函数
// 输入:
//       参数1 :UINT4 uBufLen
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
CDataBuffer::CDataBuffer(UINT4 uBufLen)
{
    m_pBuffer = 0;
    m_uBufLen = uBufLen;
    m_uWritten = 0;
    if (m_uBufLen > 0)  //分配缓冲区
    {
        m_pBuffer = new BYTE[m_uBufLen];
    }
}


/******************************************************
// 函数名:  CDataBuffer::~CDataBuffer
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
CDataBuffer::~CDataBuffer(void)
{
    //释放缓冲区m_pBuffer
    if(m_pBuffer != 0)
    {
        delete [] m_pBuffer;
        m_pBuffer = 0;
    }
}


/******************************************************
// 函数名:  CDataBuffer::Alloc
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    重新分配缓冲区
// 输入:
//       参数1 :UINT4 uBufLen
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
void CDataBuffer::Alloc(UINT4 uBufLen)
{
    ASSERT(uBufLen > 0);

    //释放原来的缓冲区
    if (m_pBuffer != 0)
    {
        delete [] m_pBuffer;
        m_pBuffer = 0;
    }

    m_uBufLen = uBufLen;
    m_uWritten = 0;

    //分配新的缓冲区
    m_pBuffer = new BYTE[m_uBufLen];
}


/******************************************************
// 函数名:  CDataBuffer::Put
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    将从pBuf开始的uBufLen个字节拷贝到内部缓冲区中
// 输入:
//       参数1 :const char* const pBuf
//       参数2 :UINT4 const uBufLen
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CDataBuffer::Put(const char* const pBuf, UINT4 const uBufLen)
{
    if ((m_uBufLen - m_uWritten >= uBufLen) && m_pBuffer)
    //内部缓冲区的剩余长度足够
    {
        //将从pBuf开始的uBufLen个字节拷贝到m_pBuffer+m_uWritten
        //开始缓冲区中
        memcpy(m_pBuffer + m_uWritten, pBuf, uBufLen);
        m_uWritten += uBufLen;
        return ERR_SUCCESS;
    }
    else
    {
        //调用框架提供的MSGOUT函数向MML运行调试台输出“打包原始话
        //单时，内部缓冲区不足”的错误信息
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
               S_NET_MAKE_BILL_PACKAGE);
        return ERR_FAIL;
    }
}



/******************************************************
// 函数名:  CDataBuffer::Get
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    获得内部缓冲区的指针
// 输入:
//       参数1 :char*& pBuf--(输出)内部缓冲区的指针
//       参数2 :UINT4& uBufLen--(输出)内部缓冲区中已经写
//              入的数据的字节数
//       参数3 :BOOL const bReset--若该参数等于TRUE，则
//              表示调用者负责释放所获得的缓冲区，否则本
//              对象负责释放内部缓冲区
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CDataBuffer::Get(char*& pBuf, UINT4& uBufLen, BOOL const bReset)
{
    pBuf = (char*)m_pBuffer;
    uBufLen = m_uWritten;
    if (bReset)
    {
        m_pBuffer = 0;
        m_uBufLen = 0;
        m_uWritten = 0;
    }
}