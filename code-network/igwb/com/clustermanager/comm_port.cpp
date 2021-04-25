///////////////////////////////////////////////////////////////
// comport.cpp -- CCommPort���ʵ��                           //
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
// CCommPort���ṩ���������ļ�������Open��Close��Read��Write
// ������ÿһ��Write����Ϊд��һ������֡��Write���ȶԸ�����
// �����֡ͷ��־�ͱ��Σ�Ȼ���͸����ڡ�Read��ͨ�����֡ͷ
// ��־��һ�ζ���һ��֡����ˣ�����·���ֹ��ϻ�����ʱ������
// ���͵�֡���ܱ����������������֡��λ����
// CCommPortͨ���ṩ��������Socket��OnReceive�麯����֪ͨ����
// �����ݡ�Ҫ�õ�֪ͨ��Ϣ����������OnReceive��
// �ر�ע��:OnReceive��CCommPort����һ�����Ķ��̵߳��á����
// ��OnReceive�б��뿼��ͬ�����⡣
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
// Open -- �򿪴���
// ����
//     nPort -- �˿ں�
//     nBaudRate -- ������
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
// Close -- �رմ���
void CCommPort::Close()
{
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        //��m_hPort,׼���ر��߳�
        HANDLE hPort;
        hPort = m_hPort;
        m_hPort = INVALID_HANDLE_VALUE;

        //���Ѷ�д�߳�
        //����m_hPort�Ѿ��رգ��ʶ�д�߳�����
        //Ͷ������֮�������������
        SetCommMask(hPort, 0);
        m_OutBuffer.CancelWait();

        //�ȴ��߳̽���
        DWORD nThreadId = GetCurrentThreadId();
        if (nThreadId != m_nThreadId[0])
            WaitForSingleObject(m_hThread[0], INFINITE);
        if (nThreadId != m_nThreadId[1])
            WaitForSingleObject(m_hThread[1], INFINITE);
        m_hThread[0] = m_hThread[1] = NULL;

        //���������
        m_InBuffer.Clear();
        m_OutBuffer.Clear();

        //�رն˿ھ��
        CloseHandle(hPort);
    }
}

////////////////////////////////////////////////
// Flush -- ��д�������е�����д�봮��
//��������
//���أ���
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
// Write -- ��д�����������һ֡
// ����
//     pBuf -- ��Ҫд�������
//     nCount -- ��Ҫд����ֽ���
// ����ֵ��ʾ�Ƿ�ɹ�
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
// Read -- �ӻ�������ȡ��һ֡
// �ر�ע�⣺���ó���Ӧ�����ͷ�pBuf�Ŀռ�
BOOL CCommPort::Read(BYTE **ppBuf, DWORD *pCount)
{
    return m_InBuffer.Get(ppBuf, pCount);
}

////////////////////////////////////////////////////
// WritePort -- д����
// ����
//     pBuf -- ��Ҫд�������
//     nCount -- ��Ҫд����ֽ���
// ����ֵ��ʾ�Ƿ�ɹ�
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
// ReadPort -- ������
// ����
//     pbuf -- ���ڴ�Ŷ�ȡ����Ļ�����
//     nCount -- ��Ҫ��ȡ���ֽ���
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
// WriteFrame -- ����һ������֡
// Ϊ�˷�ֹ���ݶ�ʧ��WriteFrame�ڷ���֮ǰ�����ݽ�����
// ���·�װ��
// 1. ��ʼ��־�ĸ��ֽڣ�ΪCOMM_FRAME_BEGIN
// 2. ���ݳ����ĸ��ֽڣ�Ϊʵ�����ݳ���
// 3. ʵ�����ݿ�ʼ��־�ĸ��ֽڣ�ΪCOMM_DATA_BEGIN
// 3. ʵ�����ݣ���4���ֽ�Ϊ��λ����һ���ֽ�Ϊ0xFF������
//    �����ֽ�Ϊʵ������
// 4. ������־�ĸ��ֽڣ�ΪCOMM_FRAME_END
// ����
//    pBuf -- ��Ҫ���͵�����
//    nCount -- ���ݳ���
// ����ֵ��ʾ�Ƿ�ɹ�����
BOOL CCommPort::WriteFrame(BYTE *pBuf, DWORD nCount)
{
    if (!pBuf) return FALSE;
    if (nCount == 0) return TRUE;
    if (m_hPort == INVALID_HANDLE_VALUE) return FALSE;

    //����ת���ռ�
    //nSizeΪnCount���ֽ�ת����Ĵ�С(��DWORDΪ��λ)
    DWORD nSize = (nCount + 2) / 3;
    DWORD *pSendBuf = new DWORD[nSize + 4];
    if (!pSendBuf) return FALSE;

    //ת������
    memset(pSendBuf, COMM_FRAME_BEGIN, 4);      //��ʼ��־
    pSendBuf[1] = nCount;                       //���ݳ���
    memset(pSendBuf + 2, COMM_DATA_BEGIN, 4);   //���ݿ�ʼ��־
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
    memset(pSendBuf + nSize + 3, COMM_FRAME_END, 4);    //������־

    //��������
    BOOL bRetVal = WritePort(
        pSendBuf, (nSize + 4) * 4);
    delete pSendBuf;
    return bRetVal;
}

////////////////////////////////////////////////////
// SetupComm -- ���ô���
// ����ֵ��ʾ�Ƿ�ɹ�
//��������
//���أ���
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
// CCommPort::CComBuffer�����������������
// CCommPort::CComBufferά��һ��Thread Safe�Ķ��У�������
// ��ÿһ�����һ��֡
//��������
//���أ���
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
// Put -- �����β�����һ֡
// ����
//     pBuf -- ֡����
//     nCount -- ֡����
// ����ֵ��ʾ�Ƿ�ɹ�
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
// Get -- �Ӷ���ͷ����ȡһ֡
// ����
//     ppBuf -- ���ڷ���֡����
//     pCount -- ���ڷ���֡����
// ����ֵ��ʾ�Ƿ�ɹ�
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
// Clear -- ���������
//��������
//���أ���
void CCommPort::CComBuffer::Clear()
{
    BYTE *pBuf;
    while (Get(&pBuf, NULL))
        delete pBuf;
}

/////////////////////////////////////////////////////////
// ReadThreadProc -- ���߳�
// ����
//     pParam -- Ϊthis
//���أ���
#ifdef __BORLANDC__
void CCommPort::ReadThreadProc(LPVOID pParam)
#else
UINT WINAPI CCommPort::ReadThreadProc(LPVOID pParam)
#endif
{
    CCommPort *pThis;
    //nFrameState��ʾ����֡��״̬
    // 0, 1, 2, 3 -- �ȴ�֡ͷ״̬
    // 4, 5, 6, 7 -- �ȴ�֡����״̬
    // 8, 9, 10, 11 -- �ȴ����ݿ�ʼ��־
    // 12, 13, 14, 15 -- �ȴ�֡������־
    int nFrameState = 0;
    // pFrameBuf���ڴ��֡����
    BYTE *pFrameBuf = NULL;
    // nBytesInBuf��¼��pFrameBuf�е��ֽ���
    DWORD nBytesInBuf = 0;
    // nRawFrameLen��ʾδ��ԭ֡���ݶγ���
    DWORD nRawFrameLen = 0;
    // nFrameLen��ʾ�ѻ�ԭ֡����
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
                    if ((nFrameState >= 0) && (nFrameState < 4))    //����֡ͷ
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
                    else if ((nFrameState >= 4) && (nFrameState < 8))    //����֡����
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
                    else if ((nFrameState >= 8) && (nFrameState < 12))    //�������ݿ�ʼ��־
                    {
                        if (pReadBuf[i] != COMM_DATA_BEGIN)
                            nFrameState = 0;
                        else
                            nFrameState++;
                        i++;
                    }
                    else if ((nFrameState >= 12) && (nFrameState < 16))   //�������ݺͽ�����־
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
                                //��ԭ����
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
// WriteThreadProc -- д�߳�
// ����
//     pParam -- Ϊthis
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
// CreateWatchThread -- �����ػ��߳�
// ����ֵ��ʾ�Ƿ�ɹ�
//��������
//���أ���
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
