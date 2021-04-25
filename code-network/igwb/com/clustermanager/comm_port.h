/////////////////////////////////////////////////////////////
// comport.h -- 串口支持模块
// chenghui, 1999.4
// 所定义的类 -- CCommPort
/////////////////////////////////////////////////////////////
#include <windows.h>

#ifndef _COMPORT_H
#define _COMPORT_H

#define COMM_FRAME_BEGIN    0x00
#define COMM_DATA_BEGIN     0x01
#define COMM_FRAME_END      0x02

#define COMM_BUFFER_SIZE    30
#define BAUDRATE            CBR_115200

class CCommPort
{
public:
    CCommPort();
    ~CCommPort();
    BOOL Open(int nPort, DWORD nBaudRate = CBR_115200);
    void Flush();
    void Close();
    BOOL Write(BYTE *pBuf, DWORD nCount);
    BOOL Read(BYTE **ppBuf, DWORD *pCount);
    virtual void OnReceive(int nFrames) {};

protected:
//与端口操作有关的成员
    HANDLE m_hPort;     //端口句柄
    DWORD m_nBaudRate;  //波特率
    OVERLAPPED m_Overlapped[2]; // 0 -- 写, 1 -- 读
    BOOL SetupCommPort(); //设置端口参数
    BOOL WritePort(LPCVOID pBuf, DWORD nCount);
    DWORD ReadPort(LPVOID pBuf, DWORD nCount);
    BOOL WriteFrame(BYTE *pBuf, DWORD nCount);
    BOOL ReadSyncFlag(); //等待同步标志

//与缓冲区有关的成员
    class CComBuffer
    {
    public:
        CComBuffer();
        ~CComBuffer();
        BOOL Put(BYTE *pBuf, DWORD nCount);
        BOOL Get(BYTE **pBuf, DWORD *pCount);
        void WaitForData()
        {
            WaitForSingleObject(m_hGetEvent, INFINITE);
        }
        void CancelWait()
        {
            SetEvent(m_hGetEvent);
        }
        DWORD GetCount() { return m_nCount; };
        VOID Clear();
    private:
        struct {
            BYTE *pData;
            DWORD nSize;
        } m_pBuffer[COMM_BUFFER_SIZE];
        int m_nHead, m_nCount;
        HANDLE m_hGetEvent;
        HANDLE m_hMutex;
    } m_InBuffer, m_OutBuffer;    //输入和输出缓冲区

//与读写线程有关的成员
    HANDLE m_hThread[2];    // 0 -- 写, 1 -- 读
    DWORD m_nThreadId[2];   // 0 -- 写, 1 -- 读
    HANDLE m_hEvent;
    BOOL CreateWatchThread();
#ifdef __BORLANDC__
    static void WriteThreadProc(LPVOID pParam);
    static void ReadThreadProc(LPVOID pParam);
#else
    static UINT WINAPI WriteThreadProc(LPVOID pParam);
    static UINT WINAPI ReadThreadProc(LPVOID pParam);
#endif
};

#endif