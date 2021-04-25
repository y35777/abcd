#include "net_status.h"
#include "resource.h"

FILE* CNetStatusInfo::m_pStatusFile = 0;

//由CNetWorkStatusInfo在构造函数中将自身对象指针赋给
//m_pNetWorkStatusInfo变量，其它地方不会对该变量赋值
CNetStatusInfo* CNetStatusInfo::m_pNetworkStatusInfo = 0;

//CNetStatusInfo对象的指针列表
CNetStatusInfo::LIST_PNETSTATUSINFO CNetStatusInfo::m_StackStatusInfoList;

//所有缓冲区的总长度
UINT4 CNetStatusInfo::m_uTotalBufLen = 0;

//初始化状态文件的文件名
char CNetStatusInfo::m_szFileName[MAX_PATH] = "";

/******************************************************
// 函数名:  CNetStatusInfo::CNetStatusInfo
// 作者:    Wangfeng
// 时期:    01-10-17
// 描述:    构造函数
// 输入:
//       参数1 :UINT4 const uBufferLen
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
CNetStatusInfo::CNetStatusInfo(UINT4 const uBufferLen)
{
    m_uBufferLen = uBufferLen;
    m_uTotalBufLen += uBufferLen;
    m_pBuffer = new BYTE[uBufferLen];
}


/******************************************************
// 函数名:  CNetStatusInfo::~CNetStatusInfo
// 作者:    Wangfeng
// 时期:    01-10-17
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
CNetStatusInfo::~CNetStatusInfo(void)
{
    //释放状态信息缓冲区
    if (m_pBuffer != 0)
    {
        delete [] m_pBuffer;
        m_pBuffer = 0;
    }

    //释放m_pStatusFile
    if (m_StackStatusInfoList.empty() && !m_pNetworkStatusInfo)
    //当析构最后一个CNetStatusInfo对象时才能关闭状态文件
    {
        if (m_pStatusFile != 0)
        {
            fclose(m_pStatusFile);
            m_pStatusFile = 0;
        }
    }

    m_uTotalBufLen -= m_uBufferLen;
}


/******************************************************
// 函数名:  CNetStatusInfo::Init
// 作者:    Wangfeng
// 时期:    01-10-17
// 描述:    初始化主状态文件
// 输入:
//       参数1 :const char* const szStatusFileName--主状态文件名
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
int CNetStatusInfo::Init(const char* const szStatusFileName)
{
    ASSERT(szStatusFileName != 0);

    //以读/写方式打开文件，如果文件不存在则返回失败
    if (m_pStatusFile) 
    {
        fclose(m_pStatusFile);
        m_pStatusFile = 0;
    }
    m_pStatusFile = fopen(szStatusFileName, "rb+");
    
    if(!m_pStatusFile)
    {
        TRACE(MTS_NETWORK, S_NET_OPEN_NET_STATUS_FILE, 
              errno, strerror(errno));
        MSGOUT(MTS_NETWORK,
               MSGOUT_LEVEL_URGENT,
               S_NET_OPEN_NET_STATUS_FILE, errno, strerror(errno));
        return errno;
    }

    strcpy((char*)m_szFileName, szStatusFileName);
    return ERR_SUCCESS;
}



/******************************************************
// 函数名:  CNetStatusInfo::Set
// 作者:    Wangfeng
// 时期:    01-10-17
// 描述:    设置状态信息
// 输入:
//       参数1 :const BYTE* const pBuf--指向存放状态信
//                                      息的缓冲区
//       参数2 :UINT4 const uLen--pBuf指向的缓冲区的长度
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
void CNetStatusInfo::Set(const BYTE* const pBuf,
                         UINT4 const uLen)
{
    ASSERT(pBuf != 0);
    ASSERT(uLen != 0);

    //将pBuf中的状态信息拷贝到内部状态信息缓冲区m_pBuffer中
    memcpy(m_pBuffer, pBuf, uLen);
}


/******************************************************
// 函数名:  CNetStatusInfo::Get
// 作者:    Wangfeng
// 时期:    01-10-17
// 描述:    
// 输入:
//       参数1 :BYTE* const pBuf--
//                         指向存放状态信息的缓冲区，pBuf
//                         的内存空间在调用该函数前应已分配
//       参数2 :UINT4 uLen--指示pBuf缓冲区长度

//       参数3 :BOOL const bReadFile = TRUE--
//                         指示是否从主状态文件更新本对象
//                         中的缓冲区数据
// 返回值: UINT4  -- 返回读出的字节数
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
UINT4 CNetStatusInfo::Get(BYTE* const pBuf, 
                          UINT4 const uLen, 
                          BOOL const bReadFile)
{
    ASSERT(pBuf != 0);
    ASSERT(uLen != 0);
    //若bReadFile为TRUE，则调用ReadFile函数从主状态文件中读取
    //并更新内部状态信息缓存区数据。若读取状态信息失败，则返回
    //0，否则输出内部状态信息缓冲区的数据，返回输出的字节数

    if (bReadFile)
    {
        int nRet = ReadFile();
        if (nRet != ERR_SUCCESS) return 0;
    }

    UINT4 uGetBytes = min(uLen, m_uBufferLen);
    memcpy(pBuf, m_pBuffer, uGetBytes);
    return uGetBytes;
}



/******************************************************
// 函数名:  CNetStatusInfo::GetAllStatusData
// 作者:    Wangfeng
// 时期:    01-11-15
// 描述:    获取内存中所有状态信息数据
// 输入:
//       参数1 :char* &pBuffer--缓冲区地址
//       参数2 :int nBufLen--缓冲区长度
// 返回值: int -- 如果成功返回输出的长度，
//                如果失败返回0
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-11-15      Wangfeng
******************************************************/
int CNetStatusInfo::GetAllStatusData(BYTE* &pBuffer, 
                                     int nBufLen)
{
    int nRet = 0;

    //调用*m_pNetworkStatusInfo对象的Get函数获得其内部状
    //态信息缓冲区数据，并放入临时缓冲区中
    if(nBufLen < m_uTotalBufLen) return nRet;

    BYTE *pBuf = pBuffer;
    int nBufLength = m_uTotalBufLen;
    UINT4 uGetBytes = m_pNetworkStatusInfo->Get(pBuf, 
                                                nBufLength, 
                                                FALSE);

    //更新指针及剩余空间长度
    pBuf += uGetBytes;
    nBufLength -= uGetBytes;

    //遍历m_StackStatusInfoList中所有CNetStatusInfo对象，
    //分别调用其Get函数获得其内部状态信息缓冲区，并放入
    //缓冲区中
    LIST_PNETSTATUSINFO::iterator i;
    for (i = m_StackStatusInfoList.begin(); 
         i != m_StackStatusInfoList.end(); 
         i++)
    {
        ASSERT(nBufLength > 0);

        //读出状态数据追加到缓冲区中
        uGetBytes = (*i)->Get(pBuf, nBufLength, FALSE);

        //更新指针及剩余空间长度
        pBuf += uGetBytes;
        nBufLength -= uGetBytes;
    }

    if(0 == nBufLength)
    {
        nRet = m_uTotalBufLen;
    }
    return nRet;
}



/******************************************************
// 函数名:  CNetStatusInfo::GetAllStatusDataLen
// 作者:    Wangfeng
// 时期:    01-11-15
// 描述:    获取内存中所有状态信息数据的长度
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-11-15      Wangfeng
******************************************************/
int CNetStatusInfo::GetAllStatusDataLen()
{
    return m_uTotalBufLen;
}


/******************************************************
// 函数名:  CNetStatusInfo::ReadFile
// 作者:    Wangfeng
// 时期:    01-10-22
// 描述:    从主状态文件m_StatusFile读取状态信息
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
int CNetStatusInfo::ReadFile(void)
{
    return ERR_FAIL;
}


/******************************************************
// 函数名:  CNetStatusInfo::WriteFile
// 作者:    Wangfeng
// 时期:    01-10-17
// 描述:    将状态信息缓存区中数据从主状态文件的当前位置
//          写入主状态文件m_StatusFile中
// 输入:
//       参数1 :void
// 返回值: int ERR_SUCCESS——成功
//             ERR_FAIL——失败
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
int CNetStatusInfo::WriteFile(void)
{
    //将状态信息缓存区中数据从主状态文件的当前位置写入
    //主状态文件m_pStatusFile中
    UINT4 uWrite = fwrite(m_pBuffer, m_uBufferLen, 1, 
                            m_pStatusFile);
    if (uWrite != 1)
    {
        TRACE(MTS_NETWORK, S_NET_WRITE_NET_STATUS_FILE, 
              errno, strerror(errno));
        MSGOUT(MTS_NETWORK,
               MSGOUT_LEVEL_URGENT,
               S_NET_WRITE_NET_STATUS_FILE, 
               errno, strerror(errno));
        return ERR_FAIL;
    }
    return ERR_SUCCESS;
}



/******************************************************
// 函数名:  CNetStatusInfo::Flush
// 作者:    Wangfeng
// 时期:    01-10-22
// 描述:    刷新本对象包含的状态数据到主状态文件中
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
int CNetStatusInfo::Flush(void)
{
    return ERR_FAIL;
}


/******************************************************
// 函数名:  CNetStatusInfo::EnumStackId
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    从主状态文件中获得第nNum个协议栈的编号
// 输入:
//       参数1 :int const nNum--SSlideWndStatusInfo或
//              GtpStatusInfo结构中不定长数组的下标
//       参数2 :int const nStackInfoLen
//              表示协议的类型；若该参数的取值为STACK_SLIDE
//              ，则表示主状态文件中的结构为SSlideWndStatusInfo；
//              若该参数的取值为STACK_GTP，则表示主状态文件中
//              的结构为SGtpStatusInfo
// 返回值: UINT4 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
UINT4 CNetStatusInfo::EnumStackId(int const nNum, 
                                  int const nStackInfoType)
{
    ASSERT(m_pStatusFile != 0);

    //取得状态文件大小
    if(fseek(m_pStatusFile, 0, SEEK_END) != 0) 
        return INVALID_STACK_ID;
    UINT4 uFileLen = ftell(m_pStatusFile);

    //检查状态文件长度是否足够
    UINT4 uChildStructLen = 0;
    if (STACK_SLIDE == nStackInfoType) 
    {
        uChildStructLen = SLIDE_MOD_INFO_LENGTH;
    }
    else if (STACK_GTP == nStackInfoType) 
    {
        uChildStructLen = GTP_NODE_INFO_LENGTH;
    }
    else if (STACK_SCAN == nStackInfoType) 
    {
        uChildStructLen = SCAN_STACK_INFO_LEN;
    }
    else
    {
        ASSERT(0);
        return INVALID_STACK_ID;
    }
    if(uFileLen < (nNum + 1) * uChildStructLen + NET_STATUS_INFO_HEADLENGTH)
    {
        //如果文件长度不足，返回无效ID
        return INVALID_STACK_ID;
    }

    //按状态文件大小分配缓冲区
    BYTE* pBuf = new BYTE[uFileLen];

    //从主状态文件中读出所有的数据
    if(fseek(m_pStatusFile, 0, SEEK_SET) != 0)
    {
        //返回无效ID
        delete [] pBuf;
        return INVALID_STACK_ID;
    }
    UINT4 uRead = fread(pBuf, uFileLen, 1, m_pStatusFile);
    if(uRead != 1)
    //读文件出错
    {
        TRACE(MTS_NETWORK, S_NET_READ_NET_STATUS_FILE, 
              errno, strerror(errno));
        MSGOUT(MTS_NETWORK,
               MSGOUT_LEVEL_URGENT,
               S_NET_READ_NET_STATUS_FILE, errno, strerror(errno));
        delete [] pBuf;
        //返回无效ID
        return INVALID_STACK_ID;
    }

    UINT4 uRet = INVALID_STACK_ID;
    if (STACK_SLIDE == nStackInfoType)
    {
        //将读出的数据强制转换成SSlideWndStatusInfo类型，
        //获得SSlideWndModInfo中第nNum个元素的nModule，
        //并返回该值
        uRet = ((SSlideWndStatusInfo*)pBuf)->SlideModInfo[nNum].yModule;
    }
    else if(STACK_GTP == nStackInfoType)
    {
        //将读出的数据强制转换成SGtpStatusInfo类型，获得
        //SGtpNodeInfo中第nNum个元素的uStackID，并返回该值
        uRet = ((SGtpStatusInfo*)pBuf)->GtpNodeInfo[nNum].uStackID;
    }
    else if(STACK_SCAN == nStackInfoType)
    {
        uRet = ((SScanStatusInfo*)pBuf)->ScanNodeInfo[nNum].nStackID;
    }

    delete [] pBuf;
    return uRet;
}


/******************************************************
// 函数名:  CNetStatusInfo::FlushAll
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    刷新所有状态数据到主状态文件中
// 输入:
//       参数1 :void
// 返回值: int ERR_SUCCESS——成功
//             ERR_FAIL——失败
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CNetStatusInfo::FlushAll(void)
{
    int nRet = ERR_SUCCESS;

    BYTE *pBuffer = new BYTE[m_uTotalBufLen];

    if(GetAllStatusData(pBuffer, m_uTotalBufLen) == 0)
    {
        nRet = ERR_FAIL;
    }
    else
    {
        //移动主状态文件m_pStatusFile的写指针到文件头
        if(fseek(m_pStatusFile, 0, SEEK_SET) != 0)
        {
            nRet = ERR_FAIL;
        }
        else
        {
            //将临时缓冲区中数据写入m_pStatusFile
            int nWriteBytes = fwrite(pBuffer, 
                                     m_uTotalBufLen,
                                     1,
                                     m_pStatusFile);
            if ((1 == nWriteBytes) && (fflush(m_pStatusFile) == 0))
            {
                ACE_OS::truncate(m_szFileName, m_uTotalBufLen);
                nRet = ERR_SUCCESS;
            }
            else
            {
                TRACE(MTS_NETWORK, S_NET_FLUSH_NET_STATUS, 
                      errno, strerror(errno));
                MSGOUT(MTS_NETWORK,
                       MSGOUT_LEVEL_URGENT,
                       S_NET_FLUSH_NET_STATUS, errno, strerror(errno));
                nRet = ERR_FAIL;
            }
        }
    }

    delete [] pBuffer;
    return nRet;
}




/******************************************************
// 函数名:  CNetWorkStatusInfo::CNetWorkStatusInfo
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    构造函数
// 输入:
//       参数1 :UINT4 const uBufferLen
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
CNetWorkStatusInfo::CNetWorkStatusInfo(UINT4 const uBufferLen)
                                      :CNetStatusInfo(uBufferLen)
{
    //将本对象的指针赋值给m_pNetworkStatusInfo
    ASSERT(m_pNetworkStatusInfo == 0);
    m_pNetworkStatusInfo = this;
}



/******************************************************
// 函数名:  CNetWorkStatusInfo::~CNetWorkStatusInfo
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
CNetWorkStatusInfo::~CNetWorkStatusInfo(void)
{
    m_pNetworkStatusInfo = 0;
}


/******************************************************
// 函数名:  CNetWorkStatusInfo::ReadFile
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    从主状态文件m_pStatusFile读取文件头信息
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CNetWorkStatusInfo::ReadFile(void)
{
    ASSERT(m_pStatusFile != 0);

    //移动主状态文件的读指针到文件头
    if(fseek(m_pStatusFile, 0, SEEK_SET) != 0) return ERR_FAIL;

    //读取SNetWorkStatusInfo结构到内部状态信息缓冲区m_pBuffer中
    //根据读文件是否成功，返回相应的值
    UINT4 uRead = fread(m_pBuffer, m_uBufferLen, 1, m_pStatusFile);
    if(uRead != 1)
    {
        TRACE(MTS_NETWORK, S_NET_READ_MAIN_NET_STATUS, 
              errno, strerror(errno));
        MSGOUT(MTS_NETWORK,
               MSGOUT_LEVEL_URGENT,
               S_NET_READ_MAIN_NET_STATUS, errno, strerror(errno));
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CNetWorkStatusInfo::Flush
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    刷新本对象包含的状态数据到主状态文件中
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CNetWorkStatusInfo::Flush(void)
{
    ASSERT(m_pStatusFile != 0);

    //移动主状态文件的写指针到文件头
    if(fseek(m_pStatusFile, 0, SEEK_SET) != 0) return ERR_FAIL;

    //将内部状态信息缓冲区m_pBuffer中的内容强制转换成
    //SNetWorkStatusInfo结构，将该结构写入主文件中并强制更新
    if((WriteFile() == 0) && (fflush(m_pStatusFile) == 0)) return ERR_SUCCESS;

    return ERR_FAIL;
}


/******************************************************
// 函数名:  CStackStatusInfo::CStackStatusInfo
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    构造函数
// 输入:
//       参数1 :int const nStackStatusInfoType
//       参数2 :UINT4 uStackId
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
CStackStatusInfo::CStackStatusInfo(int const nStackStatusInfoType, 
                                   UINT4 uStackId)
:CNetStatusInfo((STACK_SLIDE == nStackStatusInfoType) ? 
                SLIDE_MOD_INFO_LENGTH : 
                ((STACK_SCAN == nStackStatusInfoType) ? 
                SCAN_STATUS_INFO_LEN : GTP_NODE_INFO_LENGTH))
{
    m_nStackStatusInfoType = nStackStatusInfoType;
    m_uStackId = uStackId;

    //将本对象的指针加入到m_StackStatusInfoList的队尾
    m_StackStatusInfoList.push_back(this);
}


/******************************************************
// 函数名:  CStackStatusInfo::~CStackStatusInfo
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
CStackStatusInfo::~CStackStatusInfo(void)
{
    //将本对象的指针从m_StackStatusInfoList中删除
    m_StackStatusInfoList.remove(this);
}


/******************************************************
// 函数名:  CStackStatusInfo::ReadFile
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    从主状态文件m_pStatusFile读取本对象对应的协
//          议栈信息
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CStackStatusInfo::ReadFile(void)
{
    //跳过主状态文件头上的SNetWorkStatusInfo结构
    if(fseek(m_pStatusFile, NET_STATUS_INFO_HEADLENGTH, SEEK_SET) != 0)
    {
        return ERR_FAIL;
    }

    //创建一个缓冲区，不致在读状态文件出错时毁掉原有数据
    BYTE* pBuffer = new BYTE[m_uBufferLen];
    int nRet = ERR_FAIL;
    if (STACK_SLIDE == m_nStackStatusInfoType)
    {
        while (!feof(m_pStatusFile))
        {
            //从主状态文件的当前位置读取sizeof(SSlideWndModInfo)
            //个字节到m_pBuffer中，将这些数据强制转换成SSlideWndModInfo
            //结构，判断该结构中的nModule是否等于m_uStackId，若等于，
            //则返回ERR_SUCCESS
            if(fread(pBuffer, SLIDE_MOD_INFO_LENGTH, 1, m_pStatusFile) == 1)
            {
                if(m_uStackId == ((SSlideWndModInfo*)pBuffer)->yModule)
                {
                    memcpy(m_pBuffer, pBuffer, m_uBufferLen);
                    nRet = ERR_SUCCESS;
                    break;
                }
            }
        }
    }
    else if (STACK_SCAN == m_nStackStatusInfoType)
    {
        while (!feof(m_pStatusFile))
        {
            if(fread(pBuffer, SCAN_STACK_INFO_LEN, 1, m_pStatusFile) == 1)
            {
                if(m_uStackId == ((SScanStackInfo*)pBuffer)->nStackID)
                {
                    memcpy(m_pBuffer, pBuffer, m_uBufferLen);
                    nRet = ERR_SUCCESS;
                    break;
                }
            }
        }
    }
    else
    {
        while (!feof(m_pStatusFile))
        {
            //从主状态文件的当前位置读取sizeof(SGtpNodeInfo)个字节到
            //m_pBuffer中，将这些数据强制转换成SGtpNodeInfo结构，判断
            //该结构中的nModule是否等于m_uStackId，若等于，则返回
            //ERR_SUCCESS
            if(fread(pBuffer, GTP_NODE_INFO_LENGTH, 1, m_pStatusFile) == 1)
            {
                if(((SGtpNodeInfo*)pBuffer)->uStackID == m_uStackId)
                {
                    memcpy(m_pBuffer, pBuffer, m_uBufferLen);
                    nRet = ERR_SUCCESS;
                    break;
                }
            }
        }
    }
    delete [] pBuffer;
    return nRet;
}


/******************************************************
// 函数名:  CStackStatusInfo::Flush
// 作者:    Wangfeng
// 时期:    01-10-18
// 描述:    刷新本对象包含的状态数据到主状态文件中
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CStackStatusInfo::Flush(void)
{
    ASSERT(m_pStatusFile != 0);

    //跳过主状态文件头上的SNetWorkStatusInfo结构；
    if(fseek(m_pStatusFile, 
             NET_STATUS_INFO_HEADLENGTH, 
             SEEK_SET) != 0)
    {
        return ERR_FAIL;
    }

    //遍历m_StackStatusInfoList查找本对象在链表中位置，
    //并根据位置移动状态文件指针，写入状态数据并刷新
    LIST_PNETSTATUSINFO::iterator i;
    for (i = m_StackStatusInfoList.begin(); 
         i != m_StackStatusInfoList.end(); 
         i++)
    {
        if((*i) == this)
        {
            UINT4 uWrite = fwrite(m_pBuffer, 
                                  m_uBufferLen, 
                                  1, 
                                  m_pStatusFile);
            
            if((1 == uWrite) && (fflush(m_pStatusFile) == 0))
            {
                return ERR_SUCCESS;
            }
            return ERR_FAIL;
        }
        else
        {
            if(fseek(m_pStatusFile, m_uBufferLen, SEEK_CUR) != 0)
            {
                return ERR_FAIL;
            }
        }
    }

    return ERR_FAIL;
}

