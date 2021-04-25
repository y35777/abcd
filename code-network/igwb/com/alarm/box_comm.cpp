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
功能描述    初始化与设备相连的通讯口
输入参数    portName----短消息设备所连的通讯口，默认为串口2
输出参数    无
返回值      成功与否的标志
抛出异常    无
**********************************************************************/
BOOL CDeviceIO::initComPort (const STRING &portName) 
{   
    int retCode = 0;
    if (portName.size() < 1)
    {
        return FALSE;
    }

    // 如果在这之前已经初始化了，就必须清除以前的初始化信息
    if (_initFlag != 0)
    {
        _devIO.close();
        _initFlag = 0; //设为未初始化状态  
    }    
    
    ACE_Time_Value timeout(5);
    retCode = _connector.connect(_devIO, ACE_DEV_Addr(portName.c_str()), &timeout);                           
    if (retCode == -1)
    {        
        return FALSE;
    }                                             
    
    ACE_TTY_IO::Serial_Params  serialParam; // 参数 

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
    
    _initFlag = 1; //设为已初始化状态
                           
    return TRUE;
}                            
/**********************************************************************
功能描述    向串口写数据
输入参数    pBuf----待发送数据的指针
            nLen－－待发送数据的长度 
输出参数    无
返回值      发送的字节数
抛出异常    无
**********************************************************************/
int CDeviceIO::send (BYTE *pBuf, int nLen)
{
    //如果设备没有初始化成功或命令长度为0，则不执行发送命令    
    if (!_initFlag || nLen <= 0)
    {
        return -1;
    }    

    return _devIO.send( (void *)pBuf, nLen );

}   
/**********************************************************************
功能描述    从串口设备接收数据
输入参数    无 
输出参数    yRead----收到的字符
返回值      接收到的字节数
抛出异常    无
**********************************************************************/
int CDeviceIO::receive (sCommData& sReceiveData)
{    
    //如果设备没有初始化成功，则不执行接收命令    
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
功能描述    检查串口是否已经初始化
输入参数    无 
输出参数    无
返回值      是否初始化的标志
抛出异常    无
**********************************************************************/                       
//检查是否已经初始化参数了                  
BOOL CDeviceIO::isInited ()
{
    return (_initFlag==1) ? TRUE:FALSE;
}                          
/***************************************************************************************************/
/***************************************************************************************************/
//构造函数
CCommThread::CCommThread(CComBuffer* pComBuffer)
{
    m_bRunning      = FALSE;  
    m_pComBuffer    = pComBuffer;
    m_pDeviceIO     = NULL;
}

//析构函数
CCommThread::~CCommThread()
{
    if (NULL != m_pDeviceIO)
    {        
        m_pDeviceIO->Destroy();
        m_pDeviceIO = NULL;
    }    
}

//初始化函数
BOOL CCommThread::Init(STRING szPortName)
{
    m_pDeviceIO     = CDeviceIO::Instance();

    //初始化串口
    if(NULL != m_pDeviceIO)
    {
        if(!(m_pDeviceIO->isInited()))
        {
            return m_pDeviceIO->initComPort(szPortName);
        }        
    }
    return TRUE;
}

//线程是否正在运行
BOOL CCommThread::IsRunning()
{
    return m_bRunning;
}

//得到本线程的错误信息
const char* CCommThread::GetErrInfo()
{
    return m_szErrInfo;
}

//等待线程结束函数
int CCommThread::WaitThreadExit()
{
    return m_tThreadMgr.wait();
}

//设置停止标志为停止状态函数
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
// WritePort -- 从输出缓冲区中取一帧，写串口
// 输入参数  -- 无
// 输出参数  -- 无   
// 返回值    －－无
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
                //发送完毕，释放输出缓冲区的空间
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
// ReadPort -- 读串口
// 输入参数 -- 无
// 输出参数 -- 无
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
            //根据接收的数据分析完整的帧
            for(int nHasReadBytes = 0; nHasReadBytes < sRecvData.nLen; nHasReadBytes ++)
            {
                yRead = sRecvData.Data[nHasReadBytes];
                if( yRead == ALARMBOX_COMM_COMMAND_DATA_HEAD && bHasHead == FALSE)//收到数据头
                {
                    byteBuf[nAlreadyReadLen] = yRead;                
                    nAlreadyReadLen ++;
                    bHasHead = TRUE;
                    continue;
                }
                
                if( bHasHead == TRUE)//已经收到数据头
                {
                    if(yRead == ALARMBOX_COMM_COMMAND_DATA_HEAD || yRead == M_ALM_SHAKE_HAND_FRAME || yRead == -1 )//该帧数据收完成
                    {
                        WriteBuff(byteBuf, nAlreadyReadLen);   //将已收到的数据写入输入缓冲区
                        nAlreadyReadLen = 0;
                        
                        bHasHead = FALSE;
                        if (yRead == M_ALM_SHAKE_HAND_FRAME)//一个字节的握手帧b7
                        {                        
                            bHasHead = FALSE;
                            continue;
                        }
                        else if(yRead == ALARMBOX_COMM_COMMAND_DATA_HEAD)//标准帧头
                        {                    
                            byteBuf[nAlreadyReadLen] = yRead;                        
                            nAlreadyReadLen ++;
                            bHasHead = TRUE;
                            continue;
                        }
                        else//接收的数据为 -1,本次接收数据结束
                        {
                            bHasHead = FALSE;
                            //m_bRunning = FALSE;               //自动退出while
                        }
                    }
                    else//正在收数据
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
// WriteBuff -- 将收到的串口数据写入输入缓冲区
//参数：pBuf－－欲写入数据的指针
//        nlen--  数据长度
//返回：无
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
    //创建输出缓冲区以及写线程对象
    m_OutBuffer = new CComBuffer();
    m_pSendThread = new CSendThread(m_OutBuffer);

    //创建输入缓冲区以及读线程对象
    m_InBuffer = new CComBuffer();  
    m_pRecvThread = new CRecvThread(m_InBuffer);
}

CCommBox::~CCommBox()
{
    //停止读、写线程结束
    if(m_pSendThread->IsRunning())
    {
        m_pSendThread->Stop();          
    }    
    if(m_pRecvThread->IsRunning())
    {
        m_pRecvThread->Stop();        
    } 
    //等待读、写线程结束
    m_pRecvThread->WaitThreadExit();
    delete m_pRecvThread;
    m_pRecvThread = NULL; 
    m_pSendThread->WaitThreadExit();
    delete m_pSendThread;
    m_pSendThread = NULL;
    
    //清除缓冲区
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
    //初始化读写线程对象
    if(!(m_pSendThread->Init(szPortName)))
    {
        return FALSE;
    }
    if(!(m_pRecvThread->Init(szPortName)))
    {        
        return FALSE; 
    }    

    //启动读线程
    bSucc = m_pRecvThread->Run();
    if(!bSucc)
    {
        return FALSE;
    }   
    
    //启动写线程
    bSucc = m_pSendThread->Run();
    if(!bSucc)
    {
        return FALSE;
    }  

    return TRUE;
}
/////////////////////////////////////////////////////
// Read -- 从输入缓冲区中取出一帧
// 特别注意：调用程序应负责释放pBuf的空间
BOOL CCommBox::Read(BYTE **pBuf, int *nLen)
{
    return m_InBuffer->Get(pBuf, nLen);
}
/////////////////////////////////////////////////
// Write -- 往输出缓冲区中添加一帧
// 参数
//     pBuf -- 需要写入的内容
//     nCount -- 需要写入的字节数
// 返回值表示是否成功
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
// CCommBox::CComBuffer管理输入输出缓冲区
// CCommBox::CComBuffer维护一个Thread Safe的队列，队列中 的每一项代表一个帧
//参数：无
//返回：无
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
// Put -- 向队列尾部添加一帧
// 参数
//     pBuf -- 帧数据
//     nCount -- 帧长度
// 返回值表示是否成功
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
// Get -- 从队列头部读取一帧
// 参数
//     ppBuf -- 用于返回帧数据
//     pCount -- 用于返回帧长度
// 返回值表示是否成功
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
// Clear -- 清除缓冲区
//参数：无
//返回：无
void CComBuffer::Clear()
{
    BYTE *pBuf;
    while (Get(&pBuf, NULL))
        delete pBuf;
}











































