///////////////////////////////////////////////////////////////
// comport.cpp -- CCommPort类的实现                           //
// chenghui, 1999.4                                          //
//                                                           //
//                                                           //
//                                                           //
///////////////////////////////////////////////////////////////
#include <stdio.h>
//#include <windows.h>
#include <process.h>
#include "comm_port.h"

/////////////////////////////////////////////////////////////
// CCommPort类提供了类似于文件操作的Open、Close、Read和Write
// 方法。每一次Write被视为写入一个数据帧。Write首先对该数据
// 块添加帧头标志和变形，然后发送给串口。Read则通过检测帧头
// 标志以一次读出一个帧。因此，当线路出现故障或噪音时，正在
// 传送的帧可能被丢弃，但不会出现帧错位现象。
// CCommPort通过提供了类似于Socket的OnReceive虚函数来通知已收
// 到数据。要得到通知消息，必须重载OnReceive。
// 特别注意:OnReceive被CCommPort的中一独立的读线程调用。因此
// 在OnReceive中必须考虑同步问题。
/////////////////////////////////////////////////////////////

CCommPort::CCommPort()
{
    m_hPort = INVALID_HANDLE_VALUE;
    memset(m_Overlapped, 0, sizeof(m_Overlapped));
    m_Overlapped[0].hEvent =
        CreateEvent(NULL, TRUE, FALSE, NULL);
    m_Overlapped[1].hEvent =
        CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hThread[0] = m_hThread[1] = NULL;
}

CCommPort::~CCommPort()
{
    if (m_hPort != INVALID_HANDLE_VALUE) Close();
    CloseHandle(m_Overlapped[0].hEvent);
    CloseHandle(m_Overlapped[1].hEvent);
}

///////////////////////////////////////////////////////
// Open -- 打开串口
// 参数
//     nPort -- 端口号
//     nBaudRate -- 波特率
BOOL CCommPort::Open(int nPort, DWORD nBaudRate)
{
    char pPortName[10];

    sprintf(pPortName, "\\\\.\\COM%d", nPort);
    m_hPort = CreateFile(pPortName,
        GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_FLAG_OVERLAPPED,
        NULL);
    if (m_hPort == INVALID_HANDLE_VALUE)
        return FALSE;
    else
    {
        m_nBaudRate = nBaudRate;

        if (!SetupCommPort())
        {
            Close();
            return FALSE;
        }
        if (!CreateWatchThread())
        {
            Close();
            return FALSE;
        }
        ResumeThread(m_hThread[0]);
        ResumeThread(m_hThread[1]);
        return TRUE;
    }
}

///////////////////////////////////////////////
// Close -- 关闭串口
void CCommPort::Close()
{
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        //置m_hPort,准备关闭线程
        HANDLE hPort;
        hPort = m_hPort;
        m_hPort = INVALID_HANDLE_VALUE;

        //唤醒读写线程
        //由于m_hPort已经关闭，故读写线程重新
        //投入运行之后会立即结束。
        SetCommMask(hPort, 0);
        m_OutBuffer.CancelWait();

        //等待线程结束
        DWORD nThreadId = GetCurrentThreadId();
        if (nThreadId != m_nThreadId[0])
            WaitForSingleObject(m_hThread[0], INFINITE);
        if (nThreadId != m_nThreadId[1])
            WaitForSingleObject(m_hThread[1], INFINITE);
        m_hThread[0] = m_hThread[1] = NULL;

        //清除缓冲区
        m_InBuffer.Clear();
        m_OutBuffer.Clear();

        //关闭端口句柄
        CloseHandle(hPort);
    }
}

////////////////////////////////////////////////
// Flush -- 将写缓冲区中的内容写入串口
//参数：无
//返回：无
void CCommPort::Flush()
{
    BYTE *pFrame;
    DWORD nCount;
    while (m_hPort != INVALID_HANDLE_VALUE)
    {
        if (m_OutBuffer.Get(&pFrame, &nCount))
        {
            WriteFrame(pFrame, nCount);
            delete pFrame;
        } else
            return;
    }
}

/////////////////////////////////////////////////
// Write -- 往写缓冲区中添加一帧
// 参数
//     pBuf -- 需要写入的内容
//     nCount -- 需要写入的字节数
// 返回值表示是否成功
BOOL CCommPort::Write(BYTE *pBuf, DWORD nCount)
{
    BYTE *p = new BYTE[nCount];
    if (!p) return FALSE; else
    {
        memcpy(p, pBuf, nCount);
        if (!m_OutBuffer.Put(p, nCount))
        {
            delete p;
            return FALSE;
        }
        return TRUE;
    }
}

/////////////////////////////////////////////////////
// Read -- 从缓冲区中取出一帧
// 特别注意：调用程序应负责释放pBuf的空间
BOOL CCommPort::Read(BYTE **ppBuf, DWORD *pCount)
{
    return m_InBuffer.Get(ppBuf, pCount);
}

////////////////////////////////////////////////////
// WritePort -- 写串口
// 参数
//     pBuf -- 需要写入的内容
//     nCount -- 需要写入的字节熟
// 返回值表示是否成功
BOOL CCommPort::WritePort(LPCVOID pBuf, DWORD nCount)
{
    COMSTAT ComStat;
    DWORD nErrorFlags;
    DWORD nBytesWritten;
    DWORD nBytesSent = 0;

    if (!WriteFile(m_hPort, pBuf,
        nCount, &nBytesWritten, &m_Overlapped[0]))
    {
        if(GetLastError() == ERROR_IO_PENDING)
        {
            while(!GetOverlappedResult(m_hPort,
                &m_Overlapped[0], &nBytesWritten, TRUE ))
            {
                if(GetLastError() == ERROR_IO_INCOMPLETE)
                {
                    nBytesSent += nBytesWritten;
                    continue;
                }
                else
                {
                    ClearCommError(m_hPort, &nErrorFlags, &ComStat);
                    break;
                }
            }
            nBytesSent += nBytesWritten;
        }
        else
        {
            COMSTAT stat;
            ClearCommError(m_hPort, &nErrorFlags, &stat);
            return FALSE;
        }
    }
    return TRUE;
}

//////////////////////////////////////////////////////
// ReadPort -- 读串口
// 参数
//     pbuf -- 用于存放读取结果的缓冲区
//     nCount -- 需要读取的字节数
DWORD CCommPort::ReadPort(LPVOID pBuf, DWORD nCount)
{
    COMSTAT ComStat;
    DWORD nErrorFlags;
    DWORD nLength;

    ClearCommError(m_hPort, &nErrorFlags, &ComStat);
    nLength = min((DWORD)nCount, ComStat.cbInQue);

    if (nLength > 0)
    {
        if (!ReadFile(m_hPort, pBuf,
            nLength, &nLength, &m_Overlapped[1]))
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                while(!GetOverlappedResult(m_hPort,
                    &m_Overlapped[1], &nLength, TRUE))
                {
                    if(GetLastError() == ERROR_IO_INCOMPLETE)
                        continue;
                    else
                    {
                        ClearCommError(m_hPort, &nErrorFlags, &ComStat);
                        break;
                    }
                }
            }
            else
            {
                nLength = 0 ;
                ClearCommError(m_hPort, &nErrorFlags, &ComStat);
            }
        }
    }

    return nLength;
}

////////////////////////////////////////////////////////
// WriteFrame -- 发送一个数据帧
// 为了防止数据丢失，WriteFrame在发送之前对数据进行了
// 以下封装：
// 1. 开始标志四个字节，为COMM_FRAME_BEGIN
// 2. 数据长度四个字节，为实际数据长度
// 3. 实际数据开始标志四个字节，为COMM_DATA_BEGIN
// 3. 实际数据，以4个字节为单位，第一个字节为0xFF，其它
//    三个字节为实际数据
// 4. 结束标志四个字节，为COMM_FRAME_END
// 参数
//    pBuf -- 需要发送的数据
//    nCount -- 数据长度
// 返回值表示是否成功发送
BOOL CCommPort::WriteFrame(BYTE *pBuf, DWORD nCount)
{
    if (!pBuf) return FALSE;
    if (nCount == 0) return TRUE;
    if (m_hPort == INVALID_HANDLE_VALUE) return FALSE;

    //分配转换空间
    //nSize为nCount个字节转换后的大小(以DWORD为单位)
    DWORD nSize = (nCount + 2) / 3;
    DWORD *pSendBuf = new DWORD[nSize + 4];
    if (!pSendBuf) return FALSE;

    //转换数据
    memset(pSendBuf, COMM_FRAME_BEGIN, 4);      //开始标志
    pSendBuf[1] = nCount;                       //数据长度
    memset(pSendBuf + 2, COMM_DATA_BEGIN, 4);   //数据开始标志
    DWORD i = 0, j = 0;
    for (; i < nSize; i++)
    {
        BYTE *p = (BYTE *)(pSendBuf + i + 3);
        *(BYTE *)p = 0xFF;
        *(BYTE *)(++p) = pBuf[j++];
        if (j < nCount)
            *(BYTE *)(++p) = pBuf[j++];
        else
            *(BYTE *)(++p) = 0xFF;
        if (j < nCount)
            *(BYTE *)(++p) = pBuf[j++];
        else
            *(BYTE *)(++p) = 0xFF;
    }
    memset(pSendBuf + nSize + 3, COMM_FRAME_END, 4);    //结束标志

    //发送数据
    BOOL bRetVal = WritePort(
        pSendBuf, (nSize + 4) * 4);
    delete pSendBuf;
    return bRetVal;
}

////////////////////////////////////////////////////
// SetupComm -- 设置串口
// 返回值表示是否成功
//参数：无
//返回：无
BOOL CCommPort::SetupCommPort()
{
    DCB dcb;
    SetCommMask(m_hPort, EV_RXCHAR);
    SetupComm(m_hPort, 4096, 4096);

    dcb.DCBlength = sizeof(DCB);
    GetCommState(m_hPort, &dcb);
    dcb.BaudRate = m_nBaudRate;
    dcb.ByteSize = 8;
    dcb.Parity = 0;
    dcb.StopBits = 0;
    dcb.fBinary = TRUE ;
    dcb.fParity = TRUE ;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fDtrControl = RTS_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fInX = dcb.fOutX = FALSE;

    if (!SetCommState(m_hPort, &dcb)) return FALSE;

    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutConstant = 2000;
    timeouts.WriteTotalTimeoutMultiplier = 2 * CBR_9600 / m_nBaudRate;
    timeouts.WriteTotalTimeoutConstant = 0;
    if (!SetCommTimeouts(m_hPort, &timeouts)) return FALSE;

    if (!PurgeComm(m_hPort,
        PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR |
        PURGE_RXCLEAR)) return FALSE;

    return TRUE;
}

//////////////////////////////////////////////////////////////
// CCommPort::CComBuffer管理输入输出缓冲区
// CCommPort::CComBuffer维护一个Thread Safe的队列，队列中
// 的每一项代表一个帧
//参数：无
//返回：无
//////////////////////////////////////////////////////////////
CCommPort::CComBuffer::CComBuffer()
{
    for (int i = 0; i < COMM_BUFFER_SIZE; i++)
    {
        m_pBuffer[i].pData = NULL;
        m_pBuffer[i].nSize = 0;
    }
    m_hGetEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hMutex = CreateMutex(NULL, FALSE, NULL);
    m_nHead = m_nCount = 0;
}

CCommPort::CComBuffer::~CComBuffer()
{
    Clear();
    CloseHandle(m_hGetEvent);
    CloseHandle(m_hMutex);
}

/////////////////////////////////////////////////////////
// Put -- 向队列尾部添加一帧
// 参数
//     pBuf -- 帧数据
//     nCount -- 帧长度
// 返回值表示是否成功
BOOL CCommPort::CComBuffer::Put(BYTE *pBuf, DWORD nCount)
{
    if (m_nCount < COMM_BUFFER_SIZE)
    {
        WaitForSingleObject(m_hMutex, INFINITE);
        int i = m_nHead + m_nCount;
        if (i >= COMM_BUFFER_SIZE) i -= COMM_BUFFER_SIZE;
        m_pBuffer[i].pData = (BYTE *)pBuf;
        m_pBuffer[i].nSize = nCount;
        m_nCount++;
        SetEvent(m_hGetEvent);
        ReleaseMutex(m_hMutex);
        return TRUE;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////
// Get -- 从队列头部读取一帧
// 参数
//     ppBuf -- 用于返回帧数据
//     pCount -- 用于返回帧长度
// 返回值表示是否成功
BOOL CCommPort::CComBuffer::Get(BYTE **ppBuf, DWORD *pCount)
{
    if (m_nCount)
    {
        WaitForSingleObject(m_hMutex, INFINITE);
        *ppBuf = m_pBuffer[m_nHead].pData;
        if (pCount) *pCount = m_pBuffer[m_nHead].nSize;
        m_pBuffer[m_nHead].pData = NULL;
        m_pBuffer[m_nHead].nSize = 0;
        if (++m_nHead >= COMM_BUFFER_SIZE)
            m_nHead = 0;
        if (--m_nCount == 0)
            ResetEvent(m_hGetEvent);
        ReleaseMutex(m_hMutex);
        return TRUE;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////
// Clear -- 清除缓冲区
//参数：无
//返回：无
void CCommPort::CComBuffer::Clear()
{
    BYTE *pBuf;
    while (Get(&pBuf, NULL))
        delete pBuf;
}

/////////////////////////////////////////////////////////
// ReadThreadProc -- 读线程
// 参数
//     pParam -- 为this
//返回：无
#ifdef __BORLANDC__
void CCommPort::ReadThreadProc(LPVOID pParam)
#else
UINT WINAPI CCommPort::ReadThreadProc(LPVOID pParam)
#endif
{
    CCommPort *pThis;
    //nFrameState表示重组帧的状态
    // 0, 1, 2, 3 -- 等待帧头状态
    // 4, 5, 6, 7 -- 等待帧长度状态
    // 8, 9, 10, 11 -- 等待数据开始标志
    // 12, 13, 14, 15 -- 等待帧结束标志
    int nFrameState = 0;
    // pFrameBuf用于存放帧数据
    BYTE *pFrameBuf = NULL;
    // nBytesInBuf记录在pFrameBuf中的字节数
    DWORD nBytesInBuf = 0;
    // nRawFrameLen表示未还原帧数据段长度
    DWORD nRawFrameLen = 0;
    // nFrameLen表示已还原帧长度
    DWORD nFrameLen = 0;

    pThis = (CCommPort *)pParam;
    while (pThis->m_hPort != INVALID_HANDLE_VALUE)
    {
        DWORD nBytesRead;
        DWORD dwEvtMask = 0;

        WaitCommEvent(pThis->m_hPort, &dwEvtMask, NULL);

        if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
        {
            BYTE pReadBuf[4096];
            while (nBytesRead = pThis->ReadPort(pReadBuf, 4096))
            {
                DWORD i = 0;
                while (i < nBytesRead)
                {
                    if ((nFrameState >= 0) && (nFrameState < 4))    //接收帧头
                    {
                        if (pReadBuf[i] != COMM_FRAME_BEGIN)
                            nFrameState = 0;
                        else
                        {
                            if (++nFrameState == 4)
                                nFrameLen = 0;
                        }
                        i++;
                    }
                    else if ((nFrameState >= 4) && (nFrameState < 8))    //接收帧长度
                    {
                        ((BYTE *)&nFrameLen)[nFrameState - 4] = pReadBuf[i];
                        if (++nFrameState == 8)
                        {
                            nRawFrameLen = ((nFrameLen + 2) / 3) * 4;
                            if (pFrameBuf) delete pFrameBuf;
                            pFrameBuf = new BYTE[nRawFrameLen];
                            if (!pFrameBuf)
                                nFrameState = 0;
                            else
                                nBytesInBuf = 0;
                        }
                        i++;
                    }
                    else if ((nFrameState >= 8) && (nFrameState < 12))    //接收数据开始标志
                    {
                        if (pReadBuf[i] != COMM_DATA_BEGIN)
                            nFrameState = 0;
                        else
                            nFrameState++;
                        i++;
                    }
                    else if ((nFrameState >= 12) && (nFrameState < 16))   //接收数据和结束标志
                    {
                        if (nBytesInBuf < nRawFrameLen)
                        {
                            int nBytesLeft = nBytesRead - i;
                            int nBytesNeed = nRawFrameLen - nBytesInBuf;
                            int nBytesAccept = min(nBytesLeft, nBytesNeed);
                            memcpy(pFrameBuf + nBytesInBuf, pReadBuf + i, nBytesAccept);
                            nBytesInBuf += nBytesAccept;
                            i += nBytesAccept;
                        } else
                        {
                            if (pReadBuf[i] != COMM_FRAME_END)
                                nFrameState = 0;
                            else
                                nFrameState++;
                            if (nFrameState == 16)
                            {
                                nFrameState = 0;
                                //还原数据
                                BOOL bDecodeFailed = FALSE;
                                BYTE *p1 = (BYTE *)pFrameBuf;
                                BYTE *p2 = new BYTE[nFrameLen + 2];
                                BYTE *p3 = p2;
                                if (!p2) bDecodeFailed = TRUE; else
                                {
                                    DWORD nRawFrameLenInDWord = nRawFrameLen / 4;
                                    for (DWORD j = 0; j < nRawFrameLenInDWord; j++)
                                    {
                                        if (p1[0] != 0xFF)
                                            {
                                            bDecodeFailed = TRUE;
                                            break;
                                        }
                                        else
                                            p1++;
                                        for (int k = 0; k < 3; k++)
                                        {
                                            *p2 = *p1;
                                            p1++;
                                            p2++;
                                        }
                                    }
                                }
                                if (!bDecodeFailed)
                                {
                                    if (!pThis->m_InBuffer.Put(p3, nFrameLen)) delete p3;
                                    pThis->OnReceive(pThis->m_InBuffer.GetCount());
                                }
                            }
                            i++;
                        }
                    }
                }
            }
        }
    }
    if (pFrameBuf) delete pFrameBuf;
    _endthread();
#ifndef __BORLANDC__
    return 0;
#endif
}

//////////////////////////////////////////////////////////
// WriteThreadProc -- 写线程
// 参数
//     pParam -- 为this
#ifdef __BORLANDC__
void CCommPort::WriteThreadProc(LPVOID pParam)
#else
UINT WINAPI CCommPort::WriteThreadProc(LPVOID pParam)
#endif
{
    CCommPort *pThis = (CCommPort *)pParam;
    while (pThis->m_hPort != INVALID_HANDLE_VALUE)
    {
        pThis->m_OutBuffer.WaitForData();
        pThis->Flush();
    }
    _endthread();
#ifndef __BORLANDC__
    return 0;
#endif
}

//////////////////////////////////////////////////////////
// CreateWatchThread -- 创建守护线程
// 返回值表示是否成功
//参数：无
//返回：无
BOOL CCommPort::CreateWatchThread()
{
#ifdef __BORLANDC__
    m_hThread[0] = (HANDLE)_beginthreadNT(
        WriteThreadProc, 4096, this, NULL, CREATE_SUSPENDED,
        &m_nThreadId[0]);
    if (m_hThread[0] == INVALID_HANDLE_VALUE) m_hThread[0] = NULL;
    m_hThread[1] = (HANDLE)_beginthreadNT(
        ReadThreadProc, 4096, this, NULL, CREATE_SUSPENDED,
        &m_nThreadId[1]);
    if (m_hThread[1] == INVALID_HANDLE_VALUE) m_hThread[1] = NULL;
#else
    m_hThread[0] = (HANDLE)_beginthreadex(
        NULL, 4096, WriteThreadProc, this, CREATE_SUSPENDED,
        (unsigned *)&m_nThreadId[0]);
    m_hThread[1] = (HANDLE)_beginthreadex(
        NULL, 4096, ReadThreadProc, this, CREATE_SUSPENDED,
        (unsigned *)&m_nThreadId[1]);
#endif
    return (m_hThread[0] && m_hThread[1]);
}
