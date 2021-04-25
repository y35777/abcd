#include "net_status.h"
#include "resource.h"

FILE* CNetStatusInfo::m_pStatusFile = 0;

//��CNetWorkStatusInfo�ڹ��캯���н��������ָ�븳��
//m_pNetWorkStatusInfo�����������ط�����Ըñ�����ֵ
CNetStatusInfo* CNetStatusInfo::m_pNetworkStatusInfo = 0;

//CNetStatusInfo�����ָ���б�
CNetStatusInfo::LIST_PNETSTATUSINFO CNetStatusInfo::m_StackStatusInfoList;

//���л��������ܳ���
UINT4 CNetStatusInfo::m_uTotalBufLen = 0;

//��ʼ��״̬�ļ����ļ���
char CNetStatusInfo::m_szFileName[MAX_PATH] = "";

/******************************************************
// ������:  CNetStatusInfo::CNetStatusInfo
// ����:    Wangfeng
// ʱ��:    01-10-17
// ����:    ���캯��
// ����:
//       ����1 :UINT4 const uBufferLen
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
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
// ������:  CNetStatusInfo::~CNetStatusInfo
// ����:    Wangfeng
// ʱ��:    01-10-17
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
CNetStatusInfo::~CNetStatusInfo(void)
{
    //�ͷ�״̬��Ϣ������
    if (m_pBuffer != 0)
    {
        delete [] m_pBuffer;
        m_pBuffer = 0;
    }

    //�ͷ�m_pStatusFile
    if (m_StackStatusInfoList.empty() && !m_pNetworkStatusInfo)
    //���������һ��CNetStatusInfo����ʱ���ܹر�״̬�ļ�
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
// ������:  CNetStatusInfo::Init
// ����:    Wangfeng
// ʱ��:    01-10-17
// ����:    ��ʼ����״̬�ļ�
// ����:
//       ����1 :const char* const szStatusFileName--��״̬�ļ���
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
int CNetStatusInfo::Init(const char* const szStatusFileName)
{
    ASSERT(szStatusFileName != 0);

    //�Զ�/д��ʽ���ļ�������ļ��������򷵻�ʧ��
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
// ������:  CNetStatusInfo::Set
// ����:    Wangfeng
// ʱ��:    01-10-17
// ����:    ����״̬��Ϣ
// ����:
//       ����1 :const BYTE* const pBuf--ָ����״̬��
//                                      Ϣ�Ļ�����
//       ����2 :UINT4 const uLen--pBufָ��Ļ������ĳ���
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
void CNetStatusInfo::Set(const BYTE* const pBuf,
                         UINT4 const uLen)
{
    ASSERT(pBuf != 0);
    ASSERT(uLen != 0);

    //��pBuf�е�״̬��Ϣ�������ڲ�״̬��Ϣ������m_pBuffer��
    memcpy(m_pBuffer, pBuf, uLen);
}


/******************************************************
// ������:  CNetStatusInfo::Get
// ����:    Wangfeng
// ʱ��:    01-10-17
// ����:    
// ����:
//       ����1 :BYTE* const pBuf--
//                         ָ����״̬��Ϣ�Ļ�������pBuf
//                         ���ڴ�ռ��ڵ��øú���ǰӦ�ѷ���
//       ����2 :UINT4 uLen--ָʾpBuf����������

//       ����3 :BOOL const bReadFile = TRUE--
//                         ָʾ�Ƿ����״̬�ļ����±�����
//                         �еĻ���������
// ����ֵ: UINT4  -- ���ض������ֽ���
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
UINT4 CNetStatusInfo::Get(BYTE* const pBuf, 
                          UINT4 const uLen, 
                          BOOL const bReadFile)
{
    ASSERT(pBuf != 0);
    ASSERT(uLen != 0);
    //��bReadFileΪTRUE�������ReadFile��������״̬�ļ��ж�ȡ
    //�������ڲ�״̬��Ϣ���������ݡ�����ȡ״̬��Ϣʧ�ܣ��򷵻�
    //0����������ڲ�״̬��Ϣ�����������ݣ�����������ֽ���

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
// ������:  CNetStatusInfo::GetAllStatusData
// ����:    Wangfeng
// ʱ��:    01-11-15
// ����:    ��ȡ�ڴ�������״̬��Ϣ����
// ����:
//       ����1 :char* &pBuffer--��������ַ
//       ����2 :int nBufLen--����������
// ����ֵ: int -- ����ɹ���������ĳ��ȣ�
//                ���ʧ�ܷ���0
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-15      Wangfeng
******************************************************/
int CNetStatusInfo::GetAllStatusData(BYTE* &pBuffer, 
                                     int nBufLen)
{
    int nRet = 0;

    //����*m_pNetworkStatusInfo�����Get����������ڲ�״
    //̬��Ϣ���������ݣ���������ʱ��������
    if(nBufLen < m_uTotalBufLen) return nRet;

    BYTE *pBuf = pBuffer;
    int nBufLength = m_uTotalBufLen;
    UINT4 uGetBytes = m_pNetworkStatusInfo->Get(pBuf, 
                                                nBufLength, 
                                                FALSE);

    //����ָ�뼰ʣ��ռ䳤��
    pBuf += uGetBytes;
    nBufLength -= uGetBytes;

    //����m_StackStatusInfoList������CNetStatusInfo����
    //�ֱ������Get����������ڲ�״̬��Ϣ��������������
    //��������
    LIST_PNETSTATUSINFO::iterator i;
    for (i = m_StackStatusInfoList.begin(); 
         i != m_StackStatusInfoList.end(); 
         i++)
    {
        ASSERT(nBufLength > 0);

        //����״̬����׷�ӵ���������
        uGetBytes = (*i)->Get(pBuf, nBufLength, FALSE);

        //����ָ�뼰ʣ��ռ䳤��
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
// ������:  CNetStatusInfo::GetAllStatusDataLen
// ����:    Wangfeng
// ʱ��:    01-11-15
// ����:    ��ȡ�ڴ�������״̬��Ϣ���ݵĳ���
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-15      Wangfeng
******************************************************/
int CNetStatusInfo::GetAllStatusDataLen()
{
    return m_uTotalBufLen;
}


/******************************************************
// ������:  CNetStatusInfo::ReadFile
// ����:    Wangfeng
// ʱ��:    01-10-22
// ����:    ����״̬�ļ�m_StatusFile��ȡ״̬��Ϣ
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
int CNetStatusInfo::ReadFile(void)
{
    return ERR_FAIL;
}


/******************************************************
// ������:  CNetStatusInfo::WriteFile
// ����:    Wangfeng
// ʱ��:    01-10-17
// ����:    ��״̬��Ϣ�����������ݴ���״̬�ļ��ĵ�ǰλ��
//          д����״̬�ļ�m_StatusFile��
// ����:
//       ����1 :void
// ����ֵ: int ERR_SUCCESS�����ɹ�
//             ERR_FAIL����ʧ��
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-17      Wangfeng
******************************************************/
int CNetStatusInfo::WriteFile(void)
{
    //��״̬��Ϣ�����������ݴ���״̬�ļ��ĵ�ǰλ��д��
    //��״̬�ļ�m_pStatusFile��
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
// ������:  CNetStatusInfo::Flush
// ����:    Wangfeng
// ʱ��:    01-10-22
// ����:    ˢ�±����������״̬���ݵ���״̬�ļ���
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
int CNetStatusInfo::Flush(void)
{
    return ERR_FAIL;
}


/******************************************************
// ������:  CNetStatusInfo::EnumStackId
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ����״̬�ļ��л�õ�nNum��Э��ջ�ı��
// ����:
//       ����1 :int const nNum--SSlideWndStatusInfo��
//              GtpStatusInfo�ṹ�в�����������±�
//       ����2 :int const nStackInfoLen
//              ��ʾЭ������ͣ����ò�����ȡֵΪSTACK_SLIDE
//              �����ʾ��״̬�ļ��еĽṹΪSSlideWndStatusInfo��
//              ���ò�����ȡֵΪSTACK_GTP�����ʾ��״̬�ļ���
//              �ĽṹΪSGtpStatusInfo
// ����ֵ: UINT4 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
UINT4 CNetStatusInfo::EnumStackId(int const nNum, 
                                  int const nStackInfoType)
{
    ASSERT(m_pStatusFile != 0);

    //ȡ��״̬�ļ���С
    if(fseek(m_pStatusFile, 0, SEEK_END) != 0) 
        return INVALID_STACK_ID;
    UINT4 uFileLen = ftell(m_pStatusFile);

    //���״̬�ļ������Ƿ��㹻
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
        //����ļ����Ȳ��㣬������ЧID
        return INVALID_STACK_ID;
    }

    //��״̬�ļ���С���仺����
    BYTE* pBuf = new BYTE[uFileLen];

    //����״̬�ļ��ж������е�����
    if(fseek(m_pStatusFile, 0, SEEK_SET) != 0)
    {
        //������ЧID
        delete [] pBuf;
        return INVALID_STACK_ID;
    }
    UINT4 uRead = fread(pBuf, uFileLen, 1, m_pStatusFile);
    if(uRead != 1)
    //���ļ�����
    {
        TRACE(MTS_NETWORK, S_NET_READ_NET_STATUS_FILE, 
              errno, strerror(errno));
        MSGOUT(MTS_NETWORK,
               MSGOUT_LEVEL_URGENT,
               S_NET_READ_NET_STATUS_FILE, errno, strerror(errno));
        delete [] pBuf;
        //������ЧID
        return INVALID_STACK_ID;
    }

    UINT4 uRet = INVALID_STACK_ID;
    if (STACK_SLIDE == nStackInfoType)
    {
        //������������ǿ��ת����SSlideWndStatusInfo���ͣ�
        //���SSlideWndModInfo�е�nNum��Ԫ�ص�nModule��
        //�����ظ�ֵ
        uRet = ((SSlideWndStatusInfo*)pBuf)->SlideModInfo[nNum].yModule;
    }
    else if(STACK_GTP == nStackInfoType)
    {
        //������������ǿ��ת����SGtpStatusInfo���ͣ����
        //SGtpNodeInfo�е�nNum��Ԫ�ص�uStackID�������ظ�ֵ
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
// ������:  CNetStatusInfo::FlushAll
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ˢ������״̬���ݵ���״̬�ļ���
// ����:
//       ����1 :void
// ����ֵ: int ERR_SUCCESS�����ɹ�
//             ERR_FAIL����ʧ��
// ע��: 
// ����: 
// ����            ����                  ����     
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
        //�ƶ���״̬�ļ�m_pStatusFile��дָ�뵽�ļ�ͷ
        if(fseek(m_pStatusFile, 0, SEEK_SET) != 0)
        {
            nRet = ERR_FAIL;
        }
        else
        {
            //����ʱ������������д��m_pStatusFile
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
// ������:  CNetWorkStatusInfo::CNetWorkStatusInfo
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ���캯��
// ����:
//       ����1 :UINT4 const uBufferLen
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
CNetWorkStatusInfo::CNetWorkStatusInfo(UINT4 const uBufferLen)
                                      :CNetStatusInfo(uBufferLen)
{
    //���������ָ�븳ֵ��m_pNetworkStatusInfo
    ASSERT(m_pNetworkStatusInfo == 0);
    m_pNetworkStatusInfo = this;
}



/******************************************************
// ������:  CNetWorkStatusInfo::~CNetWorkStatusInfo
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
CNetWorkStatusInfo::~CNetWorkStatusInfo(void)
{
    m_pNetworkStatusInfo = 0;
}


/******************************************************
// ������:  CNetWorkStatusInfo::ReadFile
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ����״̬�ļ�m_pStatusFile��ȡ�ļ�ͷ��Ϣ
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CNetWorkStatusInfo::ReadFile(void)
{
    ASSERT(m_pStatusFile != 0);

    //�ƶ���״̬�ļ��Ķ�ָ�뵽�ļ�ͷ
    if(fseek(m_pStatusFile, 0, SEEK_SET) != 0) return ERR_FAIL;

    //��ȡSNetWorkStatusInfo�ṹ���ڲ�״̬��Ϣ������m_pBuffer��
    //���ݶ��ļ��Ƿ�ɹ���������Ӧ��ֵ
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
// ������:  CNetWorkStatusInfo::Flush
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ˢ�±����������״̬���ݵ���״̬�ļ���
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CNetWorkStatusInfo::Flush(void)
{
    ASSERT(m_pStatusFile != 0);

    //�ƶ���״̬�ļ���дָ�뵽�ļ�ͷ
    if(fseek(m_pStatusFile, 0, SEEK_SET) != 0) return ERR_FAIL;

    //���ڲ�״̬��Ϣ������m_pBuffer�е�����ǿ��ת����
    //SNetWorkStatusInfo�ṹ�����ýṹд�����ļ��в�ǿ�Ƹ���
    if((WriteFile() == 0) && (fflush(m_pStatusFile) == 0)) return ERR_SUCCESS;

    return ERR_FAIL;
}


/******************************************************
// ������:  CStackStatusInfo::CStackStatusInfo
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ���캯��
// ����:
//       ����1 :int const nStackStatusInfoType
//       ����2 :UINT4 uStackId
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
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

    //���������ָ����뵽m_StackStatusInfoList�Ķ�β
    m_StackStatusInfoList.push_back(this);
}


/******************************************************
// ������:  CStackStatusInfo::~CStackStatusInfo
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
CStackStatusInfo::~CStackStatusInfo(void)
{
    //���������ָ���m_StackStatusInfoList��ɾ��
    m_StackStatusInfoList.remove(this);
}


/******************************************************
// ������:  CStackStatusInfo::ReadFile
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ����״̬�ļ�m_pStatusFile��ȡ�������Ӧ��Э
//          ��ջ��Ϣ
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CStackStatusInfo::ReadFile(void)
{
    //������״̬�ļ�ͷ�ϵ�SNetWorkStatusInfo�ṹ
    if(fseek(m_pStatusFile, NET_STATUS_INFO_HEADLENGTH, SEEK_SET) != 0)
    {
        return ERR_FAIL;
    }

    //����һ���������������ڶ�״̬�ļ�����ʱ�ٵ�ԭ������
    BYTE* pBuffer = new BYTE[m_uBufferLen];
    int nRet = ERR_FAIL;
    if (STACK_SLIDE == m_nStackStatusInfoType)
    {
        while (!feof(m_pStatusFile))
        {
            //����״̬�ļ��ĵ�ǰλ�ö�ȡsizeof(SSlideWndModInfo)
            //���ֽڵ�m_pBuffer�У�����Щ����ǿ��ת����SSlideWndModInfo
            //�ṹ���жϸýṹ�е�nModule�Ƿ����m_uStackId�������ڣ�
            //�򷵻�ERR_SUCCESS
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
            //����״̬�ļ��ĵ�ǰλ�ö�ȡsizeof(SGtpNodeInfo)���ֽڵ�
            //m_pBuffer�У�����Щ����ǿ��ת����SGtpNodeInfo�ṹ���ж�
            //�ýṹ�е�nModule�Ƿ����m_uStackId�������ڣ��򷵻�
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
// ������:  CStackStatusInfo::Flush
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ˢ�±����������״̬���ݵ���״̬�ļ���
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CStackStatusInfo::Flush(void)
{
    ASSERT(m_pStatusFile != 0);

    //������״̬�ļ�ͷ�ϵ�SNetWorkStatusInfo�ṹ��
    if(fseek(m_pStatusFile, 
             NET_STATUS_INFO_HEADLENGTH, 
             SEEK_SET) != 0)
    {
        return ERR_FAIL;
    }

    //����m_StackStatusInfoList���ұ�������������λ�ã�
    //������λ���ƶ�״̬�ļ�ָ�룬д��״̬���ݲ�ˢ��
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

