#ifndef __ALARM_BOX_PRO_H__
#define __ALARM_BOX_PRO_H__

#include "../include/precom.h"
#include "../include/base_type.h"
#include "../include/frame.h"
#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"
#include "resource.h"
#include "ace/DEV_Connector.h"
#include "ace/TTY_IO.h"

struct sBufferData                                   //读写buffer的数据结构
{
    BYTE *pData;
    int  nSize;
};

struct sCommData                                      //收发串口数据结构
{
    int  nLen;
    BYTE Data[1024];
};
#define ALARMBOX_COMM_COMMAND_DATA_HEAD      0x7e      //数据头
#define M_ALM_SHAKE_HAND_FRAME               0xb7      //一字节的握手帧
class  CDeviceIO
{
public:
    
    CDeviceIO( );                                                   //构造函数
    ~CDeviceIO ();                                                  //析构函数
    static CDeviceIO *Instance();                                   //实例化函数
    virtual BOOL initComPort (const STRING &portName = "com2");     //初始化与设备相连的通讯口  
    virtual int send (BYTE *pBuf, int nLen);                        //向设备发送命令
    virtual int receive (sCommData& sReceiveData);                  //从设备接收回应的数据     
    BOOL isInited ();                                               //检查是否已经初始化设备了                                    
    void Destroy();  
    
protected:     
    static CDeviceIO* s_pInstance;                                  //实例化对象
    ACE_DEV_Connector  _connector;                                  // 设备连接器    
    ACE_TTY_IO         _devIO;                                      // IO读写器    
    int _initFlag;                                                  // 设备是否已经初始化。 0:否 1:是
    
};
//////////////////////////////////////////////////////////////////////
class CComBuffer;
class CCommThread 
{
public:   
    
    CCommThread(CComBuffer* pComBuffer);    
    virtual ~CCommThread ();
    BOOL Init(STRING szPortName);      
    BOOL IsRunning();                          //线程是否正在运行
    const char* GetErrInfo();                  //得到本线程的错误信息   
    virtual BOOL Run() = 0;                    //线程启动函数   
    int WaitThreadExit();                      //等待线程结束函数
    void Stop();                               //设置停止标志为停止状态函数
    CDeviceIO    *m_pDeviceIO;                 //串口IO对象

protected:    
    ACE_Thread_Manager  m_tThreadMgr;        //线程管理成员变量
    BOOL                m_bRunning;          //本线程是否正在运行标志
    char                m_szErrInfo[2000];   //本线程的异常信息     
    
    CComBuffer          *m_pComBuffer;

};
//////////////////////////////////////////////////////////////////////
class CSendThread:public CCommThread
{
public:
    CSendThread(CComBuffer* pComBuffer):CCommThread(pComBuffer){};   
    BOOL Run();                                //线程启动函数 
    
protected: 
    static void ThreadProc(CSendThread *pSendThread);
    void WritePort();                          //向串口写数据

};

class CRecvThread:public CCommThread
{
public:
    CRecvThread(CComBuffer* pComBuffer):CCommThread(pComBuffer){};  
    BOOL Run();                                //线程启动函数 
    
protected: 
    static void ThreadProc(CRecvThread *pRecvThread);
    void ReadPort();                           //读串口数据
    BOOL WriteBuff(BYTE *pBuf, int nLen);      //写缓冲区
};
//////////////////////////////////////////////////////////////////////
class CComBuffer
{
public:
    CComBuffer();
    ~CComBuffer();
    BOOL Put(BYTE *pBuf, int nLen);           //在缓冲区中放入一帧
    BOOL Get(BYTE **pBuf, int *nLen);         //从缓冲区中取一帧
    int GetCount() { return m_nCount; };
    void Clear();
    
private:
    sBufferData m_pBuffer[BOX_COMM_BUFFER_SIZE];
    int m_nHead, m_nCount;   
    ACE_Thread_Mutex m_Mutex;    
};
//////////////////////////////////////////////////////////////////////
class CCommBox
{
public:
    CCommBox();
    ~CCommBox();
    BOOL Init(STRING szPortName);    
    BOOL Read(BYTE **ppBuf, int *nLen);           //读取告警箱命令
    BOOL Write(BYTE *pBuf, int nLen);             //发送告警箱命令

protected:
    CRecvThread*      m_pRecvThread;               //读线程对象
    CSendThread*      m_pSendThread;               //写线程对象
    CComBuffer*       m_InBuffer; 
    CComBuffer*       m_OutBuffer;                 //输入和输出缓冲区

};

#endif
