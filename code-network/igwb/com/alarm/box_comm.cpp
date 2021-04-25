#include "box_comm.h"

CDeviceIO *CDeviceIO::s_pInstance = NULL;
CDeviceIO::CDeviceIO(  ): _initFlag(0)
{
}            

CDeviceIO::~CDeviceIO()
{  
    if (_initFlag != 0)
    {
        _devIO.close();
        _initFlag = 0;
    }    
} 

CDeviceIO* CDeviceIO::Instance()
{
    if(s_pInstance == NULL)
    {
        s_pInstance = new CDeviceIO();
    }
    
    return s_pInstance;
}
void CDeviceIO::Destroy()
{
    if(s_pInstance != NULL)
    {
        delete this;
        s_pInstance = NULL;
    }    
}
/**********************************************************************
��������    ��ʼ�����豸������ͨѶ��
�������    portName----����Ϣ�豸������ͨѶ�ڣ�Ĭ��Ϊ����2
�������    ��
����ֵ      �ɹ����ı�־
�׳��쳣    ��
**********************************************************************/
BOOL CDeviceIO::initComPort (const STRING &portName) 
{   
    int retCode = 0;
    if (portName.size() < 1)
    {
        return FALSE;
    }

    // �������֮ǰ�Ѿ���ʼ���ˣ��ͱ��������ǰ�ĳ�ʼ����Ϣ
    if (_initFlag != 0)
    {
        _devIO.close();
        _initFlag = 0; //��Ϊδ��ʼ��״̬  
    }    
    
    ACE_Time_Value timeout(5);
    retCode = _connector.connect(_devIO, ACE_DEV_Addr(portName.c_str()), &timeout);                           
    if (retCode == -1)
    {        
        return FALSE;
    }                                             
    
    ACE_TTY_IO::Serial_Params  serialParam; // ���� 

  	serialParam.baudrate        = 19200;
	serialParam.parityenb       = 0;
	serialParam.paritymode      = "NO";
	serialParam.databits        = 8;
	serialParam.stopbits        = 1;
	serialParam.readtimeoutmsec = 10000;
	serialParam.ctsenb          = 0;
	serialParam.rcvenb          = 1;

    serialParam.xinenb          = 0;
    serialParam.xoutenb         = 0;
    serialParam.dsrenb          = 0;
    serialParam.xonlim          = -1;
    serialParam.xofflim         = -1;
    serialParam.modem           = 0;    
    serialParam.rtsenb          = 1;

    if (_devIO.control (ACE_TTY_IO::SETPARAMS, &serialParam) == -1)
    {
        return FALSE;
    }
    
    _initFlag = 1; //��Ϊ�ѳ�ʼ��״̬
                           
    return TRUE;
}                            
/**********************************************************************
��������    �򴮿�д����
�������    pBuf----���������ݵ�ָ��
            nLen�������������ݵĳ��� 
�������    ��
����ֵ      ���͵��ֽ���
�׳��쳣    ��
**********************************************************************/
int CDeviceIO::send (BYTE *pBuf, int nLen)
{
    //����豸û�г�ʼ���ɹ��������Ϊ0����ִ�з�������    
    if (!_initFlag || nLen <= 0)
    {
        return -1;
    }    

    return _devIO.send( (void *)pBuf, nLen );

}   
/**********************************************************************
��������    �Ӵ����豸��������
�������    �� 
�������    yRead----�յ����ַ�
����ֵ      ���յ����ֽ���
�׳��쳣    ��
**********************************************************************/
int CDeviceIO::receive (sCommData& sReceiveData)
{    
    //����豸û�г�ʼ���ɹ�����ִ�н�������    
    if (!_initFlag)
    {
        TRACE(MTS_ALARM, "receive(): device hasn't been inited!\n");
        return -1; 
    } 

    sReceiveData.nLen = 0;
    memset(sReceiveData.Data, 0, sizeof(sReceiveData.Data));
    	
    ssize_t    nReadBytes;    
    nReadBytes = _devIO.recv ((void *) sReceiveData.Data, sizeof(sReceiveData.Data));

    sReceiveData.nLen = nReadBytes; 
    return nReadBytes;    
    
}
/**********************************************************************
��������    ��鴮���Ƿ��Ѿ���ʼ��
�������    �� 
�������    ��
����ֵ      �Ƿ��ʼ���ı�־
�׳��쳣    ��
**********************************************************************/                       
//����Ƿ��Ѿ���ʼ��������                  
BOOL CDeviceIO::isInited ()
{
    return (_initFlag==1) ? TRUE:FALSE;
}                          
/***************************************************************************************************/
/***************************************************************************************************/
//���캯��
CCommThread::CCommThread(CComBuffer* pComBuffer)
{
    m_bRunning      = FALSE;  
    m_pComBuffer    = pComBuffer;
    m_pDeviceIO     = NULL;
}

//��������
CCommThread::~CCommThread()
{
    if (NULL != m_pDeviceIO)
    {        
        m_pDeviceIO->Destroy();
        m_pDeviceIO = NULL;
    }    
}

//��ʼ������
BOOL CCommThread::Init(STRING szPortName)
{
    m_pDeviceIO     = CDeviceIO::Instance();

    //��ʼ������
    if(NULL != m_pDeviceIO)
    {
        if(!(m_pDeviceIO->isInited()))
        {
            return m_pDeviceIO->initComPort(szPortName);
        }        
    }
    return TRUE;
}

//�߳��Ƿ���������
BOOL CCommThread::IsRunning()
{
    return m_bRunning;
}

//�õ����̵߳Ĵ�����Ϣ
const char* CCommThread::GetErrInfo()
{
    return m_szErrInfo;
}

//�ȴ��߳̽�������
int CCommThread::WaitThreadExit()
{
    return m_tThreadMgr.wait();
}

//����ֹͣ��־Ϊֹͣ״̬����
void CCommThread::Stop()
{
    m_bRunning = FALSE;
}
/***************************************************************************************************/
/***************************************************************************************************/
BOOL CSendThread::Run()
{
    m_bRunning = TRUE;
    int nResult = m_tThreadMgr.spawn((ACE_THR_FUNC)ThreadProc, this);
    return (nResult != -1);
}

void CSendThread::ThreadProc(CSendThread *pSendThread)
{    
    pSendThread->WritePort(); 
}
////////////////////////////////////////////////////
// WritePort -- �������������ȡһ֡��д����
// �������  -- ��
// �������  -- ��   
// ����ֵ    ������
void CSendThread::WritePort()
{
    BYTE *pFrame = NULL;
    int nLen;
    while(m_bRunning)
    {
        if (m_pComBuffer->Get(&pFrame, &nLen))
        {   
            if(m_pDeviceIO != NULL)
            {
                m_pDeviceIO->send(pFrame, nLen);
                //������ϣ��ͷ�����������Ŀռ�
                delete pFrame;
            }      
        }
        ACE_OS::sleep(1);
    }
}
/***************************************************************************************************/
/***************************************************************************************************/
BOOL CRecvThread::Run()
{
    m_bRunning = TRUE;
    int nResult = m_tThreadMgr.spawn((ACE_THR_FUNC)ThreadProc, this);
    return (nResult != -1);
}
void CRecvThread::ThreadProc(CRecvThread *pRecvThread)
{    
    pRecvThread->ReadPort();
}
//////////////////////////////////////////////////////
// ReadPort -- ������
// ������� -- ��
// ������� -- ��
void CRecvThread::ReadPort()
{    
    int nReadBytes = 0, nAlreadyReadLen = 0;
    BYTE yRead;
    BYTE byteBuf[4096];
    BOOL bHasHead = FALSE;
    sCommData sRecvData;
    while(m_bRunning)
    {        
        if(m_pDeviceIO != NULL)
        {
            nReadBytes = m_pDeviceIO->receive(sRecvData);
        }        
        
        if(nReadBytes > 0)
        {        
            //���ݽ��յ����ݷ���������֡
            for(int nHasReadBytes = 0; nHasReadBytes < sRecvData.nLen; nHasReadBytes ++)
            {
                yRead = sRecvData.Data[nHasReadBytes];
                if( yRead == ALARMBOX_COMM_COMMAND_DATA_HEAD && bHasHead == FALSE)//�յ�����ͷ
                {
                    byteBuf[nAlreadyReadLen] = yRead;                
                    nAlreadyReadLen ++;
                    bHasHead = TRUE;
                    continue;
                }
                
                if( bHasHead == TRUE)//�Ѿ��յ�����ͷ
                {
                    if(yRead == ALARMBOX_COMM_COMMAND_DATA_HEAD || yRead == M_ALM_SHAKE_HAND_FRAME || yRead == -1 )//��֡���������
                    {
                        WriteBuff(byteBuf, nAlreadyReadLen);   //�����յ�������д�����뻺����
                        nAlreadyReadLen = 0;
                        
                        bHasHead = FALSE;
                        if (yRead == M_ALM_SHAKE_HAND_FRAME)//һ���ֽڵ�����֡b7
                        {                        
                            bHasHead = FALSE;
                            continue;
                        }
                        else if(yRead == ALARMBOX_COMM_COMMAND_DATA_HEAD)//��׼֡ͷ
                        {                    
                            byteBuf[nAlreadyReadLen] = yRead;                        
                            nAlreadyReadLen ++;
                            bHasHead = TRUE;
                            continue;
                        }
                        else//���յ�����Ϊ -1,���ν������ݽ���
                        {
                            bHasHead = FALSE;
                            //m_bRunning = FALSE;               //�Զ��˳�while
                        }
                    }
                    else//����������
                    {
                        byteBuf[nAlreadyReadLen] = yRead;
                        nAlreadyReadLen ++;
                    }
                }            
            }//end for            
        }        
        ACE_OS::sleep(1);
    }   
}
////////////////////////////////////////////////
// WriteBuff -- ���յ��Ĵ�������д�����뻺����
//������pBuf������д�����ݵ�ָ��
//        nlen--  ���ݳ���
//���أ���
BOOL CRecvThread::WriteBuff(BYTE *pBuf, int nLen)
{

    BYTE *p = new BYTE[nLen];
    if (!p)
    {
        return FALSE; 
    }
    else
    {
        memcpy(p, pBuf, nLen);
        if (!m_pComBuffer->Put(p, nLen))
        {
            delete p;
            return FALSE;
        }
        return TRUE;
    }
}
/***************************************************************************************************/
/***************************************************************************************************/
CCommBox::CCommBox()
{
    //��������������Լ�д�̶߳���
    m_OutBuffer = new CComBuffer();
    m_pSendThread = new CSendThread(m_OutBuffer);

    //�������뻺�����Լ����̶߳���
    m_InBuffer = new CComBuffer();  
    m_pRecvThread = new CRecvThread(m_InBuffer);
}

CCommBox::~CCommBox()
{
    //ֹͣ����д�߳̽���
    if(m_pSendThread->IsRunning())
    {
        m_pSendThread->Stop();          
    }    
    if(m_pRecvThread->IsRunning())
    {
        m_pRecvThread->Stop();        
    } 
    //�ȴ�����д�߳̽���
    m_pRecvThread->WaitThreadExit();
    delete m_pRecvThread;
    m_pRecvThread = NULL; 
    m_pSendThread->WaitThreadExit();
    delete m_pSendThread;
    m_pSendThread = NULL;
    
    //���������
    if(m_InBuffer != NULL)
    {
        delete m_InBuffer;
        m_InBuffer = NULL;
    }
    if(m_OutBuffer != NULL)
    {
        delete m_OutBuffer;
        m_OutBuffer = NULL;
    }    
}

BOOL CCommBox::Init(STRING szPortName)
{
    BOOL bSucc = FALSE;
    //��ʼ����д�̶߳���
    if(!(m_pSendThread->Init(szPortName)))
    {
        return FALSE;
    }
    if(!(m_pRecvThread->Init(szPortName)))
    {        
        return FALSE; 
    }    

    //�������߳�
    bSucc = m_pRecvThread->Run();
    if(!bSucc)
    {
        return FALSE;
    }   
    
    //����д�߳�
    bSucc = m_pSendThread->Run();
    if(!bSucc)
    {
        return FALSE;
    }  

    return TRUE;
}
/////////////////////////////////////////////////////
// Read -- �����뻺������ȡ��һ֡
// �ر�ע�⣺���ó���Ӧ�����ͷ�pBuf�Ŀռ�
BOOL CCommBox::Read(BYTE **pBuf, int *nLen)
{
    return m_InBuffer->Get(pBuf, nLen);
}
/////////////////////////////////////////////////
// Write -- ����������������һ֡
// ����
//     pBuf -- ��Ҫд�������
//     nCount -- ��Ҫд����ֽ���
// ����ֵ��ʾ�Ƿ�ɹ�
BOOL CCommBox::Write(BYTE *pBuf, int nLen)
{
    BYTE *p = new BYTE[nLen];
    if (!p)
    {
        return FALSE; 
    }
    else
    {
        memcpy(p, pBuf, nLen);
        if (!m_OutBuffer->Put(p, nLen))
        {
            delete p;
            return FALSE;
        }
        return TRUE;
    }
}
//////////////////////////////////////////////////////////////
// CCommBox::CComBuffer�����������������
// CCommBox::CComBufferά��һ��Thread Safe�Ķ��У������� ��ÿһ�����һ��֡
//��������
//���أ���
//////////////////////////////////////////////////////////////
CComBuffer::CComBuffer()
{
    for (int i = 0; i < BOX_COMM_BUFFER_SIZE; i++)
    {
        m_pBuffer[i].pData = NULL;
        m_pBuffer[i].nSize = 0;
    }

    m_nHead = m_nCount = 0;
}

CComBuffer::~CComBuffer()
{
    Clear();
}
/////////////////////////////////////////////////////////
// Put -- �����β�����һ֡
// ����
//     pBuf -- ֡����
//     nCount -- ֡����
// ����ֵ��ʾ�Ƿ�ɹ�
BOOL CComBuffer::Put(BYTE *pBuf, int nLen)
{
    if (m_nCount < BOX_COMM_BUFFER_SIZE)
    {
        m_Mutex.acquire();
        int i = m_nHead + m_nCount;
        if (i >= BOX_COMM_BUFFER_SIZE) i -= BOX_COMM_BUFFER_SIZE;
        m_pBuffer[i].pData = (BYTE *)pBuf;
        m_pBuffer[i].nSize = nLen;
        m_nCount++;
        m_Mutex.release();

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
BOOL CComBuffer::Get(BYTE **ppBuf, int *nLen)
{
    if (m_nCount)
    {
        m_Mutex.acquire();
        *ppBuf = m_pBuffer[m_nHead].pData;
        if (nLen) *nLen = m_pBuffer[m_nHead].nSize;
        m_pBuffer[m_nHead].pData = NULL;
        m_pBuffer[m_nHead].nSize = 0;
        if (++m_nHead >= BOX_COMM_BUFFER_SIZE)
            m_nHead = 0;
        m_nCount-- ;
        m_Mutex.release();

        return TRUE;
    }
    return FALSE;
}
/////////////////////////////////////////////////////////
// Clear -- ���������
//��������
//���أ���
void CComBuffer::Clear()
{
    BYTE *pBuf;
    while (Get(&pBuf, NULL))
        delete pBuf;
}











































