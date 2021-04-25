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

struct sBufferData                                   //��дbuffer�����ݽṹ
{
    BYTE *pData;
    int  nSize;
};

struct sCommData                                      //�շ��������ݽṹ
{
    int  nLen;
    BYTE Data[1024];
};
#define ALARMBOX_COMM_COMMAND_DATA_HEAD      0x7e      //����ͷ
#define M_ALM_SHAKE_HAND_FRAME               0xb7      //һ�ֽڵ�����֡
class  CDeviceIO
{
public:
    
    CDeviceIO( );                                                   //���캯��
    ~CDeviceIO ();                                                  //��������
    static CDeviceIO *Instance();                                   //ʵ��������
    virtual BOOL initComPort (const STRING &portName = "com2");     //��ʼ�����豸������ͨѶ��  
    virtual int send (BYTE *pBuf, int nLen);                        //���豸��������
    virtual int receive (sCommData& sReceiveData);                  //���豸���ջ�Ӧ������     
    BOOL isInited ();                                               //����Ƿ��Ѿ���ʼ���豸��                                    
    void Destroy();  
    
protected:     
    static CDeviceIO* s_pInstance;                                  //ʵ��������
    ACE_DEV_Connector  _connector;                                  // �豸������    
    ACE_TTY_IO         _devIO;                                      // IO��д��    
    int _initFlag;                                                  // �豸�Ƿ��Ѿ���ʼ���� 0:�� 1:��
    
};
//////////////////////////////////////////////////////////////////////
class CComBuffer;
class CCommThread 
{
public:   
    
    CCommThread(CComBuffer* pComBuffer);    
    virtual ~CCommThread ();
    BOOL Init(STRING szPortName);      
    BOOL IsRunning();                          //�߳��Ƿ���������
    const char* GetErrInfo();                  //�õ����̵߳Ĵ�����Ϣ   
    virtual BOOL Run() = 0;                    //�߳���������   
    int WaitThreadExit();                      //�ȴ��߳̽�������
    void Stop();                               //����ֹͣ��־Ϊֹͣ״̬����
    CDeviceIO    *m_pDeviceIO;                 //����IO����

protected:    
    ACE_Thread_Manager  m_tThreadMgr;        //�̹߳����Ա����
    BOOL                m_bRunning;          //���߳��Ƿ��������б�־
    char                m_szErrInfo[2000];   //���̵߳��쳣��Ϣ     
    
    CComBuffer          *m_pComBuffer;

};
//////////////////////////////////////////////////////////////////////
class CSendThread:public CCommThread
{
public:
    CSendThread(CComBuffer* pComBuffer):CCommThread(pComBuffer){};   
    BOOL Run();                                //�߳��������� 
    
protected: 
    static void ThreadProc(CSendThread *pSendThread);
    void WritePort();                          //�򴮿�д����

};

class CRecvThread:public CCommThread
{
public:
    CRecvThread(CComBuffer* pComBuffer):CCommThread(pComBuffer){};  
    BOOL Run();                                //�߳��������� 
    
protected: 
    static void ThreadProc(CRecvThread *pRecvThread);
    void ReadPort();                           //����������
    BOOL WriteBuff(BYTE *pBuf, int nLen);      //д������
};
//////////////////////////////////////////////////////////////////////
class CComBuffer
{
public:
    CComBuffer();
    ~CComBuffer();
    BOOL Put(BYTE *pBuf, int nLen);           //�ڻ������з���һ֡
    BOOL Get(BYTE **pBuf, int *nLen);         //�ӻ�������ȡһ֡
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
    BOOL Read(BYTE **ppBuf, int *nLen);           //��ȡ�澯������
    BOOL Write(BYTE *pBuf, int nLen);             //���͸澯������

protected:
    CRecvThread*      m_pRecvThread;               //���̶߳���
    CSendThread*      m_pSendThread;               //д�̶߳���
    CComBuffer*       m_InBuffer; 
    CComBuffer*       m_OutBuffer;                 //��������������

};

#endif
