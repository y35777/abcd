#include "ace/INET_Addr.h"
#include "../include/toolbox.h"
#include "../include/perf_item.h"
#include "data_buffer.h"
#include "net_status.h"
#include "network.h"
#include "gtp_related.h"
#include "../utility/mml_report.h"


template<class Tframe, class Tietype>
inline int CHECK_TV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset)
{
    if (pframe->uLen() < noffset + ielen)
    {/*��鱾֡�Ƿ�����*/ 
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
               S_NET_CHECK_TV_SIZE, 
               pframe->yType(), pframe->uLen(), 
               szTietype, noffset); 
        return -1; 
    } 
    else
    {
        pie = (Tietype *)(pframe->Data() + noffset);
        if (pie->yType() != Tietype::type)
        {/*���ietype�������Ƿ���ȷ*/
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TV_IE_TYPE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, noffset);
            return -2;
        }
    }

    return 0;
}

//CHECK_TLV���һ��TLV�ṹ���ú����Ƚ���TV�ṹ��顣Ȼ���
//��֡�����Ƿ��㹻��
template<class Tframe, class Tietype>
inline int CHECK_TLV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset)
{
    if (pframe->uLen() < noffset + ielen)
    {/*���ietype֡ͷ�Ƿ�����*/
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
               S_NET_CHECK_TV_SIZE,
               pframe->yType(), pframe->uLen(),
               szTietype, noffset);
        return -1;
    }
    else
    {
        pie = (Tietype *)(pframe->Data() + noffset);
        if (pie->yType() != Tietype::type)
        {/*���ietype�������Ƿ���ȷ*/
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TV_IE_TYPE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, noffset);
            return -2;
        }
        if (pframe->uLen() < noffset + IETLV_HEAD_LENGTH + pie->uLen())
        {/*��鱾֡�Ƿ�����*/
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TLV_SIZE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, pie->uLen(), noffset);
            return -3;
        }
    }

    return 0;
}

/******************************************************
// ������:  CGtpSNFileRef::CGtpSNFileRef
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���캯��
// ����:
//       ����1 :unsigned long const nFileNumber����������
//              ��Ӧ��״̬�ļ��ı�ţ�����Ӧ��ԭʼ�������İ���
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpSNFileRef::CGtpSNFileRef(unsigned long const uFileNumber)
{
    m_uRefCount = 1;
    m_uFileNumber = uFileNumber;
}


/******************************************************
// ������:  CGtpSNFileRef::~CGtpSNFileRef
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpSNFileRef::~CGtpSNFileRef()
{
    //
}


/******************************************************
// ������:  CGtpSNFileRef::GetFileNumber
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��ñ������Ӧ��״̬�ļ��ı��
// ����:
//       ����1 :void
// ����ֵ: unsigned long 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
unsigned long CGtpSNFileRef::GetFileNumber(void)
{
    return m_uFileNumber;
}


/******************************************************
// ������:  CGtpSNFileRef::AddRef
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����������ü�����һ
// ����:
//       ����1 :void
// ����ֵ: unsigned long 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
unsigned long CGtpSNFileRef::AddRef(void)
{
    return ++m_uRefCount;
}


/******************************************************
// ������:  CGtpSNFileRef::Release
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����������ü�����һ
// ����:
//       ����1 :BOOL bDeleteFile
// ����ֵ: unsigned long 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
unsigned long CGtpSNFileRef::Release(BOOL bDeleteFile)
{
    if (m_uRefCount > 0)
    {
        if (--m_uRefCount == 0)
        {
            //ɾ���ļ�
            if (bDeleteFile)
            {
                char szPath[MAX_PATH];

                sprintf(szPath, 
                        "%s%u.sn", 
                        CNetWork::m_szWorkDir,
                        m_uFileNumber);
                remove(szPath);
            }

            //ɾ��������
            delete this;
            return 0;
        }
    }
    return m_uRefCount;
}


/******************************************************
// ������:  CSNList::CSNList
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���캯��
// ����:
//       ����1 :void
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CSNList::CSNList(void)
{
    //��ȡ�����ļ�����á�Ĭ�ϵ���ʷ��¼���ȡ���ȱʡֵΪ3000��
    //��ֵ��m_nHisFrameCount
    //�������ļ�
    CINIFile *pIniFile;
    pIniFile = new CINIFile((char *)GetCfgFilePath());
    pIniFile->Open();

    //��ȡ�����̶�Ӧ��Ĭ�ϵ���ʷ��¼����
    char strSec[20];
    sprintf(strSec, 
            "%s%d",
            CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);
    m_nHisFrameCount = pIniFile->GetInt(strSec,
                                        CFG_AP_KEY_HIS_FRM_COUNT,
                                        CFG_AP_KEY_HIS_FRM_COUNT_DEFAULT);
    pIniFile->Close();
    delete pIniFile;

    m_nAllFrameCount = 0;
    m_pLastSNFileRef = 0;
    m_pSubmitted = new LIST<unsigned long>;
}


/******************************************************
// ������:  CSNList::~CSNList
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CSNList::~CSNList(void)
{
    //����Clear(FALSE)��������ɱ�Ҫ��������
    Clear(FALSE);

    if(m_pSubmitted != 0)
    {
        delete m_pSubmitted;
        m_pSubmitted = 0;
    }
}


/******************************************************
// ������:  CSNList::Load
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��״̬�ļ��еĵ�ǰλ��װ�����к�
// ����:
//       ����1 :SGtpSNFile* const pSNFile
//       ����2 :int const nAction--
//                  1��LA_DISCARD������װ�ص����кŶ�����
//                  2��LA_SAVED������װ�ص����к���Ϊ�Ѿ�
//                                 ��ǰ����ģ�鱣��
//                  3��LA_SUBMITTED������װ�ص����к���Ϊ
//                                     �Ѿ��ύ��ǰ����ģ
//                                     �黹û�гɹ�����
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CSNList::Load(SGtpSNFile* const pSNFile, int const nAction)
{
    int nSNLen;
    UINT4 uSN;

    //��pSNFile->m_pStatusFile�ļ��ĵ�ǰλ�ý�ָ��Э��ջ�����к�
    //���뵽*pSNArray��

    //�������к��б�ĳ���
    if(fread(&nSNLen, 4, 1, pSNFile->pStatusFile) != 1) return ERR_FAIL;

    //����һ������ΪLIST<unsigned long>*�Ķ��󣬽��ö���ֵ��pSNArray
    LIST<unsigned long>* pSNArray = new LIST<unsigned long>;
    //�����������кţ�����ӵ��б���
    for(int j = 0; j < nSNLen; j++)
    {
        if(fread(&uSN, 4, 1, pSNFile->pStatusFile) != 1)
        {
            delete pSNArray;
            return ERR_FAIL;
        }
        pSNArray->push_back(uSN);
    }

    if ((nAction != LA_DISCARD) && (pSNArray->size() > 0))
    {
        //���ñ���װ�ص����кŵ�״̬
        LIST<unsigned long>::iterator i;
        for (i = pSNArray->begin(); i != pSNArray->end(); i++)
        {
            unsigned long uSeqNum = *i;
            switch (nAction)
            {
            case LA_SAVED:
                //��ӳ���ϵ<nSeqNum, FS_SAVED>���뵽m_SeqMap��
                m_SeqMap[uSeqNum] = FS_SAVED;
                break;

            case LA_SUBMITTED:
                //��ӳ���ϵ<nSeqNum, FS_SUBMITTED>���뵽m_SeqMap��
                m_SeqMap[uSeqNum] = FS_SUBMITTED;
                break;
            //by ldf 2002.4.17��Ӧ���ⵥD12742
            default:
              {
                  break;
              }
            //�޸����
            }
        }

        //�������кŵ�����
        m_nAllFrameCount += pSNArray->size();

        //��*pSNArray�е����кŷ���m_Saved�л�m_Submitted��
        if (LA_SAVED == nAction)
        {
            SGtpSNListInFile* pListInFile = new SGtpSNListInFile;
            pListInFile->pFileRef = pSNFile->pFileRef;
            pListInFile->pListSeqNum = pSNArray;

            //��pListInFile���뵽m_Saved�Ķ�β
            m_Saved.push_back(pListInFile);
            pListInFile->pFileRef->AddRef();
            pSNArray = 0;
        }
        else if (LA_SUBMITTED == nAction)
        {
            if (m_pSubmitted != 0)
            {
                delete m_pSubmitted;
                m_pSubmitted = 0;
            }
            m_pSubmitted = pSNArray;
            m_pLastSNFileRef = pSNFile->pFileRef;
            m_pLastSNFileRef->AddRef();
            pSNArray = 0;
        }
    }

    if (pSNArray != 0)
    {
        delete pSNArray;
        pSNArray = 0;
    }
    return ERR_SUCCESS;
}


/******************************************************
// ������:  CSNList::Save
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���Ѿ��ύ�����кű��浽״̬�ļ���
// ����:
//       ����1 :SGtpSNFile* const pSNFile
// ����ֵ: int -- �Ƿ�ɹ� 
// ע��:    ע�⣺���Բ��������ύ��δӦ�������½���
//          ��������״̬�����������ͬһ���ύ����ͬʱ
//          �����������������к��б��ļ��С�
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CSNList::Save(SGtpSNFile* const pSNFile)
{
    //��pSNFile->m_pStatutsFile�ļ��ĵ�ǰλ��˳��д��*m_pSubmitted��
    //�ĳ��ȼ����к�
    ASSERT(pSNFile != 0);

    //���仺����
    int nSize = m_pSubmitted->size();
    int nBufLen = nSize + 1;//��4���ֽ�Ϊ��λ�ĳ���
    UINT4* pBuf = new UINT4[nBufLen];

    //�򻺳���˳��д��m_pSubmitted�еĳ��ȼ����к�
    pBuf[0] = nSize;
    LIST<unsigned long>::iterator i;
    int j = 1;
    for (i = m_pSubmitted->begin(); i != m_pSubmitted->end(); i++)
    {
        pBuf[j++] = *i;
    }
    ASSERT(j == nBufLen);
    
    //���������ڵ�����д���ļ���
    int nWrite = fwrite((char*)pBuf, 
                        sizeof(UINT4) * nBufLen, 
                        1, 
                        pSNFile->pStatusFile);
    delete [] pBuf;
    if(nWrite != 1) return ERR_FAIL;

    if (m_pSubmitted->size() > 0)
    {
        //���m_pLastSNFileRef != 0��˵����ǰ���ύ��δӦ��
        //��ʱ�ٴ�����߱������к��б����߼��������������ͬһ
        //���ύ����ͬʱ�����������������к��б��ļ��С�
        ASSERT(m_pLastSNFileRef == 0);

        m_pLastSNFileRef = pSNFile->pFileRef;
        m_pLastSNFileRef->AddRef();
    }
    return ERR_SUCCESS;
}


/******************************************************
// ������:  CSNList::Add
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����µ����кţ��ڵ��øú���ǰ����Ҫ�ȵ���
//          Lookup()����������Ƿ�Ϊ�µĻ���������ǲ���
//          ���øú���
// ����:
//       ����1 :unsigned long const nSeqNum
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CSNList::Add(unsigned long const uSeqNum)
{
    //��nSeqNum���뵽m_New�Ķ�β
    m_New.push_back(uSeqNum);

    //��ӳ���ϵ<nSeqNum, FS_NEW>���뵽m_SeqMap��
    m_SeqMap[uSeqNum] = FS_NEW;

    m_nAllFrameCount++;
}


/******************************************************
// ������:  CSNList::Submit
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    �ύnCount���µ����к�
// ����:
//       ����1 :int const nCount
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CSNList::Submit(int const nCount)
{
    //��֤���ύ���б�ĳ��ȵ���0������˵����ǰ���ύ����δӦ��
    ASSERT(m_pSubmitted->size() == 0);

    //��m_New��ǰnCount�����к��ƶ���*m_pSubmitted��
    for(int i = 0; i < nCount; i++)
    {
        UINT4 uSeqNum = m_New.front();
        m_New.pop_front();
        m_pSubmitted->push_back(uSeqNum);

        //���ÿ�����ƶ������кţ���ӳ���ϵ<���ƶ������к�, 
        //FS_SUBMITTED>���뵽m_SeqMap��
        m_SeqMap[uSeqNum] = FS_SUBMITTED;
    }
}


/******************************************************
// ������:  const CSNList::GetSubmitted
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����Ѿ��ύ�����к��б�
// ����:
//       ����1 :void
// ����ֵ: LIST<unsigned long>* 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
LIST<unsigned long>* const CSNList::GetSubmitted(void)
{
    return m_pSubmitted;
}


/******************************************************
// ������:  CSNList::GetFirstFileNumber
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��õ�һ���ļ���
// ����:
//       ����1 :void
// ����ֵ: unsigned long 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
unsigned long CSNList::GetFirstFileNumber(void)
{
    if (m_Saved.size() > 0)
    {
        SGtpSNListInFile* pListInFile = m_Saved.front();
        return pListInFile->pFileRef->GetFileNumber();
    }
    return 0;
}


/******************************************************
// ������:  CSNList::Lookup
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    �ж�nSeqNumber�Ƿ����Ѿ��ڱ������к��б���
// ����:
//       ����1 :unsigned long const nSeqNum
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CSNList::Lookup(unsigned long const uSeqNum)
{
    //��m_SeqMap�в��Ҽ�ֵΪnSeqNum��Ԫ�أ������ڣ�
    //�򷵻ظ�Ԫ�ص�ȡֵ�����򷵻�FS_NULL
    if(m_SeqMap.find(uSeqNum) != m_SeqMap.end())
    {
        return m_SeqMap[uSeqNum];
    }
    return FS_NULL;
}


/******************************************************
// ������:  CSNList::OnResponse
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ǰ����ģ��Ӧ��󣬶����к��б������Ӧ�Ĵ���
// ����:
//       ����1 :bool const bSaved--�Ƿ񱣴�ɹ�
// ����ֵ: void
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CSNList::OnResponse(BOOL const bSaved)
{
    unsigned long uSeqNum;

    //����Ƿ��й�ʱ�����к���Ҫɾ��
    //ɾ��m_Saved��ǰnCountRedundant�����кŶ�Ӧ��Ԫ��
    int nCountRedundant = m_nAllFrameCount - m_nHisFrameCount;

    //������m_Saved�����ٱ���һ�����������ڻ�õ�һ���ļ���ʱ���ܵõ�0
    //��m_nHisFrameCount�Ƚ�Сʱ
    while ((nCountRedundant > 0) && (m_Saved.size() > 1))
    {
        SGtpSNListInFile* pListInFile = m_Saved.front();
        if (pListInFile->pListSeqNum->size() <= nCountRedundant)
        {
            m_nAllFrameCount -= pListInFile->pListSeqNum->size();
            nCountRedundant -= pListInFile->pListSeqNum->size();
            while (pListInFile->pListSeqNum->size() > 0)
            {
                uSeqNum = pListInFile->pListSeqNum->front();
                m_SeqMap.erase(uSeqNum);
                pListInFile->pListSeqNum->pop_front();
            }
            //��SN�ļ������һ��Э��ջ�ڴ˴�ɾ��
            pListInFile->pFileRef->Release();
            m_Saved.pop_front();
            delete pListInFile->pListSeqNum;
            delete pListInFile;
        }
        else
        {
            break;
        }
    }

    //���Ѿ��ύ�����кŵ�״̬�ı�ΪFS_SAVED��������Щ���кż��뵽
    //m_Saved�У�ͬʱ��m_pLastSNFileRef���и�ֵ
    if (m_pSubmitted->size() > 0)
    {   
        ASSERT(m_pLastSNFileRef != 0);
        if (bSaved)
        {
            //���*m_pSubmitted��nCountSubmitted�����кţ�
            //��ӳ���ϵ<���к�, FS_SAVED>���뵽m_SeqMap��
            LIST<unsigned long>::iterator i;
            for(i = m_pSubmitted->begin(); i != m_pSubmitted->end(); i++)
            {
                m_SeqMap[*i] = FS_SAVED;
            }

            SGtpSNListInFile* pListInFile = new SGtpSNListInFile;
            pListInFile->pFileRef = m_pLastSNFileRef;
            pListInFile->pListSeqNum = m_pSubmitted;

            //��pListInFile���뵽m_Saved�Ķ�β
            m_Saved.push_back(pListInFile);

            //����һ���¶��У������ö��е�ָ�븳ֵ��m_pSubmitted
            m_pSubmitted = new LIST<unsigned long>;
        }
        else
        {
            //�������ʧ�ܣ���ɾ��m_pSubmitted�ļ�¼
            //��SN�ļ������һ��Э��ջ�ڴ˴�ɾ�����˴�һ��Ҫɾ����SN�ļ���
            //��������һ��Loadʱ�����ļ��еİ��Żᶼ��Ϊ���ѳɹ�����İ���
            m_pLastSNFileRef->Release();
            LIST<unsigned long>::iterator i;
            for(i = m_pSubmitted->begin(); i != m_pSubmitted->end(); i++)
            {
                m_SeqMap.erase(*i);
            }

            //by ldf 2002.4.3 ��Ӧ���ⵥ D12381
            m_nAllFrameCount -= m_pSubmitted->size();
            //�޸����

            m_pSubmitted->clear();
        }
        m_pLastSNFileRef = 0;
    }
}


/******************************************************
// ������:  CSNList::Clear
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����������е��������к�
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CSNList::Clear(BOOL bDeleteFile)
{
    //ɾ��m_SeqMap�е�����ӳ���ϵ
    m_SeqMap.clear();

    //ɾ��m_Saved�����е�Ԫ��
    SGtpSNListInFile* pListInFile = 0;
    while (m_Saved.size() > 0)
    {
        pListInFile = m_Saved.front();
        m_Saved.pop_front();
        pListInFile->pFileRef->Release(bDeleteFile);
        delete pListInFile->pListSeqNum;
        delete pListInFile;
    }

    //ɾ��*m_pSubmitted�����е�Ԫ��
    m_pSubmitted->clear();

    //ɾ��m_New�����е�Ԫ��
    m_New.clear();

    if (m_pLastSNFileRef != 0)
    {
        m_pLastSNFileRef->Release(bDeleteFile);
        m_pLastSNFileRef = 0;
    }
    m_nAllFrameCount = 0;
}


/******************************************************
// ������:  CGtpStack::CGtpStack
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���캯��
// ����:
//       ����1 :CNetApp* const pNetApp����ָ����Ϊ����
//                                        ģ��Ӧ�ò�Ķ���
//       ����2 :unsigned long const nStackId--��GTP'Э��ջ�ı��
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpStack::CGtpStack(CNetApp* const pNetApp, 
                     unsigned long const uStackId)
                     :CProtocolStack(pNetApp, uStackId)
{
    m_szGsnName[0]                  = '\0';
    m_nNewDataSize                  = 0;
    m_nNewRecordCount               = 0;
    m_nGTPSeqNumber                 = 1;
    m_nEchoReqInvCounter            = 0;
    m_nNodeAliveReqInvCounter       = 0;
    m_bDeleteFileOnDestroy          = FALSE;
    memset(&m_GtpNodeInfo, 0, GTP_NODE_INFO_LENGTH);
    m_GtpNodeInfo.uGtpVersion       = GTP_V2_TAG1;
    m_nGtpHeadLen                   = GTP_FRAME_HEAD_LENGTH_V2;
    m_GtpNodeInfo.uGsnRestartCount  = INVALID_RESTARTCOUNT;
    m_GtpNodeInfo.uLocalIp          = ACE_OS::inet_addr(
                                      ((CGtpNetApp*)m_pNetApp)->
                                      GetLocalAddress());
    //�������ļ�
    CINIFile IniFile((char *)GetCfgFilePath());
    IniFile.Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);

    //��ȡ��GSN�Ķ˿ں�
    m_GtpNodeInfo.uLocalPort = IniFile.GetInt(strSec,
                                      CFG_AP_KEY_LOCAL_PORT_TO_PM,
                                      CFG_AP_KEY_LOCAL_PORT_TO_PM_GSN_DEFAULT);
    m_nBufferLimit = IniFile.GetInt(strSec,
                                    CFG_AP_KEY_GTP_BUFFER_LIMIT,
                                    CFG_AP_KEY_GTP_BUFFER_LIMIT_DEFAULT);
    if(m_nBufferLimit < 2048) m_nBufferLimit = 2048;
    if(m_nBufferLimit > 65535) m_nBufferLimit = 65535;
    IniFile.Close();

    m_GtpNodeInfo.uStackID = uStackId;

    //����һ������������Ϊ10KB��SGtpStack��ʵ��������ʵ����ָ�븳ֵ
    //��m_pAckFrame,ע�⣺ͷ��ҪΪ��ļ��㣬��20���ֽ�
    m_pAckFrame = (SGtpFrame*) new char[GTP_FRAME_HEAD_LENGTH_V1 + 
                                        PREPARED_ACKFRAME_SIZE];
    m_pAckFrame->yTag(m_GtpNodeInfo.uGtpVersion);
    m_pAckFrame->FillV0HeadEx();

    m_pStatusInfo = new CStackStatusInfo(STACK_GTP, uStackId);
    sprintf(m_szWorkDir, 
            "%s%d%s", 
            CNetWork::m_szWorkDir, 
            uStackId, 
            ACE_DIRECTORY_SEPARATOR_STR);

    //��������Ŀ¼
    CreateDeepDir(m_szWorkDir);       

    m_tLastSendAdyFulfillTime = ACE_OS::gettimeofday();

	m_ResTime_new.clear();
	m_ResTime_old.clear();
}


/******************************************************
// ������:  CGtpStack::~CGtpStack
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpStack::~CGtpStack(void)
{
    Clear(m_bDeleteFileOnDestroy);

    //ɾ������Ŀ¼����������
    if (m_bDeleteFileOnDestroy)
    {
        SweepDirectory(m_szWorkDir);
    }

    //�ͷ�m_pAckFrameָ���ʵ��
    if(m_pAckFrame != 0)
    {
        delete [] m_pAckFrame;
        m_pAckFrame = 0;
    }
}

/******************************************************
// ������:  CGtpStack::DelGsnIp
// ����:    Wangfeng
// ʱ��:    03-07-30
// ����:    ɾ��һ��GSN��IP��ַ
// ����:
//       ����1 :uIPAddr ��ɾ����IP��ַ
// ����ֵ: ��
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//03-07-30      Wangfeng
******************************************************/
void CGtpStack::DelGsnIp(UINT4 uIPAddr)
{
    int nGsnIpCount = sizeof(m_GtpNodeInfo.uGsnIp) / sizeof(m_GtpNodeInfo.uGsnIp[0]);
    BOOL bFound = FALSE;

    for(int i = 0; i < nGsnIpCount; i++)
    {
        if(0 != m_GtpNodeInfo.uGsnIp[i])
        {
            break;
        }
        
        if(m_GtpNodeInfo.uGsnIp[i] == uIPAddr)
        {
            for(int j = i; j < nGsnIpCount - 1; j++)
            {
                m_GtpNodeInfo.uGsnIp[j] = m_GtpNodeInfo.uGsnIp[j + 1];
            }
            m_GtpNodeInfo.uGsnIp[nGsnIpCount - 1] = 0;
        }
        
        if(m_GtpNodeInfo.uGsnIp[i] == m_GtpNodeInfo.nActivedGsnIP)
        {
            bFound = TRUE;
        }
    }

    if(!bFound)
    {
        m_GtpNodeInfo.nActivedGsnIP = m_GtpNodeInfo.uGsnIp[0];

        if(m_szGsnName[0] == '\0')
        {
            struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
            Addr.s_addr = m_GtpNodeInfo.nActivedGsnIP;
#else //_PLATFORM_HPUX
            Addr.S_un.S_addr = m_GtpNodeInfo.nActivedGsnIP;
#endif//_PLATFORM_HPUX
            strncpy(m_szName, ACE_OS::inet_ntoa(Addr), sizeof(m_szName));
            m_szName[sizeof(m_szName) - 1] = '\0';
        }
    }
}

/******************************************************
// ������:  CGtpStack::GetGsnIPTable
// ����:    Wangfeng
// ʱ��:    03-06-27
// ����:    ��ñ�Э��ջ��GSN��IP��ַ��
// ����:
//       ����1 :ip_table   ��ЧIP��ַ��
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//03-06-27      Wangfeng
******************************************************/
void CGtpStack::GetGsnIPTable(VECTOR<UINT4>& ip_table)
{
    int nGsnIpCount = sizeof(m_GtpNodeInfo.uGsnIp) / sizeof(m_GtpNodeInfo.uGsnIp[0]);

    for(int i = 0; i < nGsnIpCount; i++)
    {
        if(m_GtpNodeInfo.uGsnIp[i] == 0)
        {
            break;
        }
        ip_table.push_back(m_GtpNodeInfo.uGsnIp[i]);
    }
}


/******************************************************
// ������:  CGtpStack::UpdateGsnIPTable
// ����:    Wangfeng
// ʱ��:    03-06-27
// ����:    ���±�Э��ջ��GSN��IP��ַ��
// ����:
//       ����1 :ip_table   GSN��IP��ַ��
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//03-06-27      Wangfeng
******************************************************/
void CGtpStack::UpdateGsnIPTable(VECTOR<UINT4>& ip_table)
{
    int nGsnIpCount = sizeof(m_GtpNodeInfo.uGsnIp) / sizeof(m_GtpNodeInfo.uGsnIp[0]);
    BOOL bFound = FALSE;

    for(int i = 0; i < nGsnIpCount; i++)
    {
        if(i < ip_table.size())
        {
            m_GtpNodeInfo.uGsnIp[i] = ip_table[i];
        }
        else
        {
            m_GtpNodeInfo.uGsnIp[i] = 0;
        }

        if(0 != m_GtpNodeInfo.uGsnIp[i] && 
            m_GtpNodeInfo.uGsnIp[i] == m_GtpNodeInfo.nActivedGsnIP)
        {
            bFound = TRUE;
        }
    }

    if(!bFound)
    {
        m_GtpNodeInfo.nActivedGsnIP = m_GtpNodeInfo.uGsnIp[0];

        if(m_szGsnName[0] == '\0')
        {
            struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
            Addr.s_addr = m_GtpNodeInfo.nActivedGsnIP;
#else //_PLATFORM_HPUX
            Addr.S_un.S_addr = m_GtpNodeInfo.nActivedGsnIP;
#endif//_PLATFORM_HPUX
            strncpy(m_szName, ACE_OS::inet_ntoa(Addr), sizeof(m_szName));
            m_szName[sizeof(m_szName) - 1] = '\0';
        }
    }
}


/******************************************************
// ������:  CGtpStack::Clear
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    �ú��������ʷ��¼��δӦ��֡��������֡
// ����:
//       ����1 :BOOL bDeleteFile
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CGtpStack::Clear(BOOL bDeleteFile)
{
    SGtpReqFrameInfo *pInfo;
    SGtpFrame *pFrame;

    m_SNList.Clear(bDeleteFile);
    m_ListReleasedSN.clear();

    //�ͷ�δ�ɹ��յ�Ӧ�������֡
    MAP_SN_TO_GTPREQ::iterator i;
    for (i = m_MapReqFrames.begin(); i != m_MapReqFrames.end(); i++)
    {
        pInfo = (*i).second;
        delete pInfo->pFrame;
        delete pInfo;
    }
    m_MapReqFrames.clear();

    //�ͷ�δ�ύ���µ�����֡
    while (m_ListNewFrames.size() > 0)
    {
        pFrame = m_ListNewFrames.back();
        m_ListNewFrames.pop_back();
        delete pFrame;
    }
    
    //�ͷ�δ�ύ���ͷŵ�����֡
    while (m_ListReleasedFrames.size() > 0)
    {
        pFrame = m_ListReleasedFrames.back();
        m_ListReleasedFrames.pop_back();
        delete pFrame;
    }

    m_nNewDataSize = 0;
    m_nNewRecordCount = 0;
}


/******************************************************
// ������:  CGtpStack::GetReadyDataSize
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����
//          ģ�鱣��Ļ������ݵ��ֽ���
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CGtpStack::GetReadyDataSize(void)
{
    return m_nNewDataSize;
}


/******************************************************
// ������:  CGtpStack::GetReadyRecordCount
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����
//          ģ�鱣��Ļ������ݵļ�¼��
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CGtpStack::GetReadyRecordCount(void)
{
    return m_nNewRecordCount;
}


/******************************************************
// ������:  CGtpStack::GetReadyData
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����Э��ջ���ύ��ǰ����ģ�鱣��Ļ�������
//          ����*pDataBuf������
// ����:
//       ����1 :CDataBuffer* const pDataBuf
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CGtpStack::GetReadyData(CDataBuffer* const pDataBuf)
{
    while (m_ListReleasedFrames.size() > 0)
    {
        //ȡ�����ͷŵ�����֡�б���ͷ��ָ��
        SGtpFrame* pFrame = m_ListReleasedFrames.front();

        //ȡ����֡�����ĳ��ȼ���¼��
        //2002-11-23 ��ŷ� ��Ӧ���ⵥ D19425
        TIEBillPackage* pPackage = NULL;
        if((pFrame->yTag() == GTP_V0_TAG1) || 
           (pFrame->yTag() == GTP_V06_TAG1))
        {
            pPackage = (TIEBillPackage *)pFrame->Data();
        }
        else
        {
            pPackage = (TIEBillPackage *)(pFrame->Data() + 
                                    TIETRANSFER_CMD_LENGTH);
        }
        //�޸����

        int nDataSize = DATASIZE_OF_IEBILLPACKAGE(pPackage);
        int nDataCount = pPackage->yCount();

        //�ŵ��������У�ʧ�����˳�������
        if(pDataBuf->Put((char*)pPackage->Data(), nDataSize) != ERR_SUCCESS)
        {
            return;
        }

        //���µ�ǰ���յ������ݳ��Ⱥͼ�¼��
        m_nNewDataSize -= nDataSize;
        m_nNewRecordCount -= nDataCount;
        
        //��pFrame->uSeqNum��ȡֵ���뵽m_ListReleasedSN��
        m_ListReleasedSN.push_back(pFrame->uSeqNum());

        //��pFrame��m_ListReleasedFrames��ɾ��
        m_ListReleasedFrames.pop_front();
        delete pFrame;
    }

    int nFrameCount = 0;
    while (m_ListNewFrames.size() > 0)
    {
        //ȡ���µ�����֡�б���ͷ��ָ��
        SGtpFrame* pFrame = m_ListNewFrames.front();

        //ȡ����֡�����ĳ��ȼ���¼��
        //2002-11-23 ��ŷ� ��Ӧ���ⵥ D19425
        TIEBillPackage* pPackage = NULL;
        if((pFrame->yTag() == GTP_V0_TAG1) || 
           (pFrame->yTag() == GTP_V06_TAG1))
        {
            pPackage = (TIEBillPackage *)pFrame->Data();
        }
        else
        {
            pPackage = (TIEBillPackage *)(pFrame->Data() + 
                                    TIETRANSFER_CMD_LENGTH);
        }
        //�޸����

        int nDataSize = DATASIZE_OF_IEBILLPACKAGE(pPackage);
        int nDataCount = pPackage->yCount();

        //�ŵ��������У�ʧ����������ѭ��
        if(pDataBuf->Put((char*)pPackage->Data(), nDataSize) != ERR_SUCCESS)
        {
            break;
        }

        //���µ�ǰ���յ������ݳ��Ⱥͼ�¼��
        m_nNewDataSize -= nDataSize;
        m_nNewRecordCount -= nDataCount;

        //��pFrame��m_ListNewFrames��ɾ��
        m_ListNewFrames.pop_front();
        delete pFrame;
        nFrameCount++;
    }

    if (nFrameCount > 0)
    {
        m_SNList.Submit(nFrameCount);
    }
}


/******************************************************
// ������:  CGtpStack::OnReceive
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ������뱾Э��ջ��Ӧ��GSN���͹�����һ֡����
// ����:
//       ����1 :void* pFrame--����SNetFrame�ṹ����Ϣ֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CGtpStack::OnReceive(void* pFrame)
{
    SNetFrame* pNetFrame = (SNetFrame *)pFrame;
    SGtpFrame* pGTPFrame = (SGtpFrame *)(pNetFrame->Data);

    //�����Ϣ�����������Ȳ���һ��GTP'��֡ͷ���ȣ����˳�
    if(pNetFrame->Len < GTP_FRAME_HEAD_LENGTH_V2)
    {
        return;
    }

	//ͳ��ga�ӿ�ƽ����Ӧʱ�� yg
	time_t ResTime;
	ACE_OS::time(&ResTime);
	m_ResTime_new.push_back(ResTime);
	
    //��¼Э��ջ��Ϣ��GsnIp�ڹ��캯��������
    m_GtpNodeInfo.nActivedGsnIP = pNetFrame->SNetAddrPair.ClientIp;
    m_GtpNodeInfo.uGsnPort      = pNetFrame->SNetAddrPair.ClientPort;
    m_GtpNodeInfo.uLocalIp      = pNetFrame->SNetAddrPair.ServerIp;
    m_GtpNodeInfo.uLocalPort    = pNetFrame->SNetAddrPair.ServerPort;
    m_GtpNodeInfo.uAppType      = pNetFrame->AppType;

    //�жϰ汾�Ƿ�֧��
    if (!pGTPFrame->CheckGTPFrame())
    {
        SendVersionNotSupported(pGTPFrame);
        return;
    }

    if(m_szGsnName[0] == '\0')
    {
        struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        Addr.s_addr = m_GtpNodeInfo.nActivedGsnIP;
#else //_PLATFORM_HPUX
        Addr.S_un.S_addr = m_GtpNodeInfo.nActivedGsnIP;
#endif//_PLATFORM_HPUX
        strncpy(m_szName, ACE_OS::inet_ntoa(Addr), sizeof(m_szName));
        m_szName[sizeof(m_szName) - 1] = '\0';
    }

    if(pNetFrame->Len > m_nBufferLimit)
    {
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
               S_GTP_BUFFER_EXCEED, pNetFrame->Len, m_nBufferLimit);
        m_nBufferLimit = pNetFrame->Len;
    }
    ((CGtpNetApp *)m_pNetApp)->m_pAllFrmPerf->Add(1);

    if(pGTPFrame->yTag() != m_GtpNodeInfo.uGtpVersion)
    {
        m_GtpNodeInfo.uGtpVersion = pGTPFrame->yTag();
        m_nGtpHeadLen = SGtpFrame::GetHeadLen(m_GtpNodeInfo.uGtpVersion);
        m_pAckFrame->yTag(m_GtpNodeInfo.uGtpVersion);
        m_pAckFrame->FillV0HeadEx();
    }

    switch (pGTPFrame->yType())
    {
    //��������GSN���ܻᷢ�͸����CGF�Բ�ѯCGF�Ƿ���Ч��
    //CGFֱ��Ӧ�����������������
    case ECHO_REQUEST:
        OnGTPEchoRequest(pGTPFrame);
        break;

    //����Ӧ��CGF�����Ե���GSN����ECHO_REQUEST��GSN�յ�������
    //֮�󽫷���ECHO_RESPONSE��CGFӦ������ȡ�������������Ӧ����
    case ECHO_RESPONSE:
        OnGTPEchoResponse(pGTPFrame);
        break;

    //���ݴ�������GSNʹ�ø�����ͻ��������ݲ�ͬ����в�ͬ����
    case DATA_TRANSFER_REQUEST:
        OnGTPDataTransferRequest(pGTPFrame);
        break;

    //�ڵ�����Ӧ�𣬲���Ҫ�����κ��ر���
    case NODE_ALIVE_RESPONSE:
        OnGTPResponse(pGTPFrame);
        break;

    //�ڵ�����
    case NODE_ALIVE_REQUEST:
        OnGTPNodeAliveRequest(pGTPFrame);
        break;

    //�ض���Ӧ��
    case REDIRECTION_RESPONSE:
        OnGTPResponse(pGTPFrame);
        break;

    //�汾��֧��
    case VERSION_NOT_SUPPORTED:
        break;

    default:
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
               S_NET_UNRECOGNIZED_FRAME,
               pGTPFrame->yType());
    }

    CProtocolStack::OnReceive(pFrame);
}


/******************************************************
// ������:  CGtpStack::SendVersionNotSupported
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ��������GSN����VersionNotSupported֡
// ����:    SGtpFrame *pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::SendVersionNotSupported(SGtpFrame *pFrame)
{
    SGtpFrame* pGtpFrame = CreateGTPFrame(0);
    ASSERT(NULL != pGtpFrame);

    //���GTPͷ
    pGtpFrame->yType(VERSION_NOT_SUPPORTED);
    pGtpFrame->uSeqNum(pFrame->uSeqNum());

    SendGTPResponse(pGtpFrame);
}


/******************************************************
// ������:  CGtpStack::SendFrame2Ex
// ����:    Wangfeng
// ʱ��:    01-11-6
// ����:    ���Ӧ������ģ���GSN����Э��֡
// ����:
//       ����1 :void* pFrame
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-6       Wangfeng
******************************************************/
int CGtpStack::SendFrame2Ex(void* pFrame)
{
    //����һ��SNetFrame�ṹ��ʵ��*pMsg
    MSG_INTRA* pMsg = new(((SGtpFrame *)pFrame)->GetSize()) MSG_INTRA;
    if (!pMsg) return ERR_FAIL;

    //��*pFrame�е����ݿ�����*pMsg����������
    memcpy(pMsg->Data, pFrame, pMsg->Len);
    pMsg->ReceiverPid = PT_OUTSIDE;
    pMsg->ReceiverMid = MT_UNKNOWN;
    pMsg->SenderPid   = CMsgProcessor::GetProcessType();
    pMsg->SenderMid   = MT_NETWORK;
    pMsg->AppType     = m_GtpNodeInfo.uAppType;
    ((SNetFrame*)pMsg)->SNetAddrPair.ClientIp   = m_GtpNodeInfo.nActivedGsnIP;
    ((SNetFrame*)pMsg)->SNetAddrPair.ClientPort = m_GtpNodeInfo.uGsnPort;
    ((SNetFrame*)pMsg)->SNetAddrPair.ServerIp   = m_GtpNodeInfo.uLocalIp;
    ((SNetFrame*)pMsg)->SNetAddrPair.ServerPort = m_GtpNodeInfo.uLocalPort;

    return m_pNetApp->SendFrame2Ex(pMsg, this);
}


/******************************************************
// ������:  CGtpStack::SendRedirectionRequest
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ��������GSN����RedirectionRequest֡
// ����:
//       ����1 :char* szIpAddress---�Ƽ���GSN����һ̨CG��IP��ַ
//       ����2 :BYTE yCause
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CGtpStack::SendRedirectionRequest(char* szIpAddress, BYTE yCause)
{
    SGtpFrame *pFrame = NULL;

    UINT4 uAddr = ACE_OS::inet_addr(szIpAddress);
    if(uAddr != 0)
    {
        pFrame = CreateGTPFrame(TIECAUSE_LENGTH +
                                TIERECMD_NODE_ADDR_LENGTH);
    }
    else
    {
        pFrame = CreateGTPFrame(TIECAUSE_LENGTH);
    }

    //���GTPͷ
    pFrame->yType(REDIRECTION_REQUEST);
    pFrame->uSeqNum(m_nGTPSeqNumber++);

    //��дTIECause�ṹ
    TIECause* pCause = (TIECause* )pFrame->Data();
    pCause->yType(TIECause::type);
    pCause->yCause(yCause);

    if(uAddr != 0)
    {
        //��дTIERecommendNodeAddress
        TIERecommendNodeAddress* pNodeAddress = (TIERecommendNodeAddress* )
                                        (pFrame->Data() + TIECAUSE_LENGTH);
        pNodeAddress->yType(TIERecommendNodeAddress::type);
        pNodeAddress->uLen(4);   //IPV4����ʽ
        pNodeAddress->uAddress(uAddr);
    }

    SendGTPRequest(pFrame);
}


/******************************************************
// ������:  CGtpStack::OnResponse
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
// ����:
//       ����1 :bool const bSaved--�Ƿ񱣴�ɹ�
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CGtpStack::OnResponse(BOOL const bSaved)
{
//******************************************************************
////ͳ��Ga�ӿ�ƽ����Ӧʱ��
	m_ResTime_old = m_ResTime_new;
	m_ResTime_new.clear();
	time_t ResTime;
	ACE_OS::time(&ResTime);
	int AverageTime = 0;
	int TotalTime = 0;
	int ListSize = m_ResTime_old.size();
	
	while(m_ResTime_old.size() > 0)
	{
		TotalTime += ResTime - m_ResTime_old.front();
		m_ResTime_old.pop_front();
	}
	if (ListSize != 0)
	{
		AverageTime = TotalTime/ListSize;
		((CGtpNetApp *)m_pNetApp)->m_pGtpResTime->Set(AverageTime);
	}
	else
	{
		((CGtpNetApp *)m_pNetApp)->m_pGtpResTime->Set(0);
	}
//***********************************************************************	

	
    SGtpFrame *pFrame;
    LIST<unsigned long> *pSNArrays[] = {m_SNList.GetSubmitted(), 
                                        &m_ListReleasedSN};
    LIST<unsigned long> *pSNArray;
    TIESNResponse *pSNResponse;

    //���Ӧ����ǿհ�����ֱ���˳�
    if((m_SNList.GetSubmitted()->size() == 0) && 
       (m_ListReleasedSN.size() == 0))
    {
        return;
    }

    //����Ӧ��֡
    if (bSaved)
    {
        for (int i = 0; i < 2; i++)
        {
            pSNArray = pSNArrays[i];
            if (pSNArray->size() == 0) continue;

            //����Ӧ��֡
            pFrame = CreateTransferResponse(CAUSE_REQUEST_ACCEPTED,
                                            pSNArray->size());
            ASSERT(pFrame != NULL);

            pSNResponse = (TIESNResponse *)(pFrame->Data() + 
                                            TIECAUSE_LENGTH);
            int t = 0;
            LIST<unsigned long>::iterator j;
            for (j = pSNArray->begin(); j != pSNArray->end(); j++)
            {
                int nSeqNumber = *j;
                pSNResponse->uSeqNums(t++, nSeqNumber);
                if (1 == i)
                {
                    char szPath[MAX_PATH];

                    //ɾ���ѳɹ�����Ŀ����ظ����ļ�
                    sprintf(szPath, "%smd%u.dat", 
                            m_szWorkDir, 
                            nSeqNumber);
                    remove(szPath);
                }
            }
            SendGTPResponse(pFrame);
            DeleteTransferResponse(pFrame);
        }
    }

    //���������������к��б�
    m_SNList.OnResponse(bSaved);
    m_GtpNodeInfo.uFirstSNFile = m_SNList.GetFirstFileNumber();
    m_ListReleasedSN.clear();

    //�����ظ��������ѱ����֡���к�
    SendAleadyFulfill();
}

//�����ظ��������ѱ����֡���к�
void CGtpStack::SendAleadyFulfill()
{
    SGtpFrame *pFrame;
    TIESNResponse *pSNResponse;

    m_tLastSendAdyFulfillTime = ACE_OS::gettimeofday();

    //���Ӧ����ǿհ�����ֱ���˳�
    if(m_ListSavedSN.size() == 0)
    {
        return;
    }

    //����Ӧ��֡
    pFrame = CreateTransferResponse(CAUSE_REQUEST_ACCEPTED,
                                    m_ListSavedSN.size());
    ASSERT(pFrame != NULL);

    pSNResponse = (TIESNResponse *)(pFrame->Data() + 
                                    TIECAUSE_LENGTH);
    int t = 0;
    LIST<unsigned long>::iterator j;
    for (j = m_ListSavedSN.begin(); j != m_ListSavedSN.end(); j++)
    {
        int nSeqNumber = *j;
        pSNResponse->uSeqNums(t++, nSeqNumber);
    }

    //����Ӧ��֡
    SendGTPResponse(pFrame);
    DeleteTransferResponse(pFrame);
    m_ListSavedSN.clear();
}


/******************************************************
// ������:  CGtpStack::LoadStatus
// ����:    Wangfeng
// ʱ��:    01-10-22
// ����:    ����״̬�ļ��ж�ȡ��Э��ջ��״̬��Ϣ
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
int CGtpStack::LoadStatus(void)
{
    unsigned long uLocalIp   = m_GtpNodeInfo.uLocalIp;
    unsigned long uLocalPort = m_GtpNodeInfo.uLocalPort;

    //����*m_pStatusInfo�����Get����
    UINT4 uInputLen = m_pStatusInfo->Get((BYTE*)(&m_GtpNodeInfo), 
                                         GTP_NODE_INFO_LENGTH);

    m_GtpNodeInfo.uLocalIp   = uLocalIp;
    m_GtpNodeInfo.uLocalPort = uLocalPort;

    m_nGtpHeadLen = SGtpFrame::GetHeadLen(m_GtpNodeInfo.uGtpVersion);
    m_pAckFrame->yTag(m_GtpNodeInfo.uGtpVersion);
    m_pAckFrame->FillV0HeadEx();

    //��״̬�ļ����ɹ��򷵻�ʧ��
    if(uInputLen != GTP_NODE_INFO_LENGTH) return ERR_FAIL;

    if(m_szGsnName[0] == '\0')
    {
        struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        Addr.s_addr = m_GtpNodeInfo.nActivedGsnIP;
#else //_PLATFORM_HPUX
        Addr.S_un.S_addr = m_GtpNodeInfo.nActivedGsnIP;
#endif//_PLATFORM_HPUX
        strncpy(m_szName, ACE_OS::inet_ntoa(Addr), sizeof(m_szName));
        m_szName[sizeof(m_szName) - 1] = '\0';
    }

    return ERR_SUCCESS;
}


/******************************************************
// ������:  CGtpStack::LoadSNList
// ����:    Wangfeng
// ʱ��:    01-10-26
// ����:    װ�����к��б�
// ����:
//       ����1 :SGtpSNFile *pSNFile
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
int CGtpStack::LoadSNList(SGtpSNFile *pSNFile)
{
    int nRet = ERR_FAIL;    //�Ƿ�װ��ɹ�
    UINT4 uFileNumber = pSNFile->pFileRef->GetFileNumber();
    UINT4 uCurPacketId = m_pNetApp->GetNetWork()->GetPacketId();

    //���ļ���С�����һ��GSN����ʱ�İ��Ż���С����Ч�ļ���ʱӦ����
    if ((uFileNumber < m_GtpNodeInfo.uFirstSNFile) ||
        (uFileNumber < m_GtpNodeInfo.uPIDAtGSNRestart) ||
        (uFileNumber > uCurPacketId))
    {
        nRet = m_SNList.Load(pSNFile, LA_DISCARD);
    }
    else if (uFileNumber < uCurPacketId)
    //���ļ���С�ڵ�ǰ��IdʱӦ�����ѱ���
    {
        nRet = m_SNList.Load(pSNFile, LA_SAVED);
    }
    else// if (uFileNumber == uCurPacketId)
    {
        nRet = m_SNList.Load(pSNFile, 
                  m_pNetApp->GetNetWork()->IsWaitForResponse() ? 
                  LA_SUBMITTED : LA_SAVED);             
    }

    if (nRet != ERR_SUCCESS) return nRet;

    //���뱻�ͷ�����б������һ�εı��ͷ��б���Ч
    int nSize;
    UINT4 uSeqNum;
    m_ListReleasedSN.clear();

    //��ȡ��Ÿ���
    if (1 == fread(&nSize, sizeof(nSize), 1, pSNFile->pStatusFile))
    {
        for (int i = 0; i < nSize; i++)
        {
            if (fread(&uSeqNum, sizeof(UINT4), 1, 
                      pSNFile->pStatusFile) != 1)
            {
                nRet = ERR_FAIL;
                break;
            }
            m_ListReleasedSN.push_back(uSeqNum);
        }
    }
    else
    {
        nRet = ERR_FAIL;
    }

    if ((nRet != ERR_SUCCESS) || 
        !m_pNetApp->GetNetWork()->IsWaitForResponse() ||
        (uFileNumber != uCurPacketId))
    {
        m_ListReleasedSN.clear();
    }

    return nRet;
}


/******************************************************
// ������:  CGtpStack::SaveSNList
// ����:    Wangfeng
// ʱ��:    01-10-26
// ����:    �������к��б�
// ����:
//       ����1 :SGtpSNFile *pSNFile
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
int CGtpStack::SaveSNList(SGtpSNFile *pSNFile)
{
    //д�Ѿ��ύ��Э��֡�����к�
    int nRet = m_SNList.Save(pSNFile);
    if(nRet != ERR_SUCCESS) return nRet;

    //д�Ѿ��ͷŵ�Э��֡�����к�
    int nSize = m_ListReleasedSN.size();
    int nBufLen = nSize + 1;
    UINT4* pBuf = new UINT4[nBufLen];
    pBuf[0] = nSize;

    int j = 1;
    LIST<unsigned long>::iterator i;
    for(i = m_ListReleasedSN.begin(); i != m_ListReleasedSN.end(); i++)
    {
        pBuf[j++] = *i;
    }
    ASSERT(j == nBufLen);

    if(fwrite((char*)pBuf, 
              sizeof(UINT4) * nBufLen, 
              1, 
              pSNFile->pStatusFile) != 1)
    {
        nRet = ERR_FAIL;
    }
    delete [] pBuf;
    return nRet;
}


/******************************************************
// ������:  CGtpStack::SaveStatus
// ����:    Wangfeng
// ʱ��:    01-10-22
// ����:    ����״̬�ļ���д�뱾Э��ջ��״̬��Ϣ
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
int CGtpStack::SaveStatus(void)
{
    //����*m_pStatusInfo�����Set����
    m_pStatusInfo->Set((BYTE*)(&m_GtpNodeInfo), GTP_NODE_INFO_LENGTH);
    return ERR_SUCCESS;
}


/******************************************************
// ������:  CGtpStack::Timer1Sec
// ����:    Wangfeng
// ʱ��:    01-10-22
// ����:    ���¼�������
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpStack::Timer1Sec(void)
{
    UINT4 uSeqNum;
    int nRequestTimeOut  = ((CGtpNetApp*)m_pNetApp)->GetRequestTimeOut();
    int nTimeoutRepeat   = ((CGtpNetApp*)m_pNetApp)->GetTimeOutRepeat();
    int nEchoReqInv      = ((CGtpNetApp*)m_pNetApp)->GetEchoReqInv();

    if(m_GtpNodeInfo.uAppType == APP_GTP_UDP)
    {
        //��ʱ��Echo Reqest��Ϣ֡
        //ZYS�޸����ⵥ��D19464 
        if(nEchoReqInv >= 60 && nEchoReqInv <= 300
           && ++m_nEchoReqInvCounter >= nEchoReqInv)
        {
            SendEchoRequest();
            m_nEchoReqInvCounter = 0;
        }
    }

    /*int nNodeAliveReqInv = ((CGtpNetApp*)m_pNetApp)->GetNodeAliveReqInv();

    //��ʱ����Node Alive Request֡��ʱ�������(��������Ҫ���)
    if(m_nNodeAliveReqInvCounter++ >= nNodeAliveReqInv * ONE_MINUTE_IN_SECOND)
    {
        SendNodeAliveRequest(m_GtpNodeInfo.uLocalIp);
        m_nNodeAliveReqInvCounter = 0;
    }��������ʱȥ������Ҫ��*/

    //���m_MapReqFrames���Ƿ���ڳ�ʱ��Request
    LIST <UINT4> ListWillBeRemovedSeqNum;
    MAP_SN_TO_GTPREQ::iterator i;
    for (i = m_MapReqFrames.begin(); i != m_MapReqFrames.end(); i++)
    {
        uSeqNum = (*i).first;
        SGtpReqFrameInfo *pInfo = (*i).second;

        if (++(pInfo->yTimeOut) >= nRequestTimeOut)
        {
            pInfo->yTimeOut = 0;
            pInfo->yRepeatTimes++;
            SendFrame2Ex(pInfo->pFrame);
            if (pInfo->yRepeatTimes >= nTimeoutRepeat)
            {
				// ������GSN�����ϸ澯��Ϣ
				SInnerAlarm alarm;
				alarm.uAlarmID      = ALARM_ID_REQUSET_TIMEOUT;  // GSN����Ӧ
				alarm.yAlarmType    = AT_FAULT;                // ���ϸ澯
				alarm.yAlarmPara[0] = ALARM_PATA_GSN_NO_REQUEST;

				//��������дIP
				ACE_INET_Addr AddrClient;				
				AddrClient.set((u_short)0, NTOHL(m_GtpNodeInfo.nActivedGsnIP));
				sprintf((char*)&alarm.yAlarmPara[1], "%s", AddrClient.get_host_addr());
				alarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
				CMsgProcessor::SendAlarm(&alarm);            	
                ListWillBeRemovedSeqNum.push_back(uSeqNum);
                delete pInfo->pFrame;
                delete pInfo;
            }
        }
    }

    //ɾ��m_MapReqFrames���ѳ�ʱ�����к�
    while (ListWillBeRemovedSeqNum.size() > 0)
    {
        uSeqNum = ListWillBeRemovedSeqNum.front();
        ListWillBeRemovedSeqNum.pop_front();
        m_MapReqFrames.erase(uSeqNum);
    }

    ACE_Time_Value tNow = ACE_OS::gettimeofday() - m_tLastSendAdyFulfillTime;
    if(m_ListSavedSN.size() > 0 && tNow >= ACE_Time_Value(2))
    {
        //�����ظ��������ѱ����֡���к�
        SendAleadyFulfill();
    }

    CProtocolStack::Timer1Sec();
}


/******************************************************
// ������:  CGtpStack::SendEchoRequest
// ����:    Wangfeng
// ʱ��:    01-10-24
// ����:    ����Echo Request
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CGtpStack::SendEchoRequest()
{
    SGtpFrame *pFrame = CreateGTPFrame(0);
    if (pFrame != NULL)
    {
        //���GTPͷ
        pFrame->yType(ECHO_REQUEST);
        pFrame->uSeqNum(m_nGTPSeqNumber++);

        SendGTPRequest(pFrame);
    }
}


/******************************************************
// ������:  CGtpStack::SendGTPRequest
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ����GTP'����֡,�ú�����������֡���������δӦ��֡����
// ����:
//       ����1 :SGtpFrame *pFrame -- ��Ҫ���͵�֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::SendGTPRequest(SGtpFrame *pFrame)
{
    UINT4 uSeqNum;
    SGtpReqFrameInfo *pInfo = NULL;

    //ZYS�޸����ⵥ��D19464 ECHO_REQUEST����
	//if(pFrame->yType() != ECHO_REQUEST)
	{	
		uSeqNum = pFrame->uSeqNum();
		if (m_MapReqFrames.find(uSeqNum) != m_MapReqFrames.end())
		{
			pInfo = m_MapReqFrames[uSeqNum];
			delete pInfo->pFrame;
		}
		else
		{
			pInfo = new SGtpReqFrameInfo;
		}
		
		pInfo->pFrame = pFrame;
		pInfo->yTimeOut = 0;
		pInfo->yRepeatTimes = 0;
		m_MapReqFrames[uSeqNum] = pInfo;
	}

    SendFrame2Ex(pFrame);
}


/******************************************************
// ������:  CGtpStack::SendGTPResponse
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ����GTPӦ��֡
// ����:
//       ����1 :SGtpFrame *pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::SendGTPResponse(SGtpFrame *pFrame)
{
    SendFrame2Ex(pFrame);
}


/******************************************************
// ������:  CGtpStack::SendNodeAliveRequest
// ����:    Wangfeng
// ʱ��:    01-10-25
// ����:    ����Node Alive Request
// ����:
//       ����1 :UINT4 uLocalAddr -- ָ�����˵�ַ
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
void CGtpStack::SendNodeAliveRequest(UINT4 uLocalAddr)
{
/*
    //�������UDP��ʽ��������NodeAlive��Ϣ
    if(m_GtpNodeInfo.uAppType != APP_GTP_UDP)
    {
        return;
    }
*/

    SGtpFrame *pFrame;
    TIENodeAddress *pAddress;

    pFrame = CreateGTPFrame(TIENODE_ADDRESS_LENGTH);
    if (pFrame != NULL)
    {
        //���GTPͷ
        //2002-12-09 ��ŷ� ��Ӧ���ⵥ D19626
        pFrame->yType(NODE_ALIVE_REQUEST);
        //�޸����
        pFrame->uSeqNum(m_nGTPSeqNumber++);

        //���IE�ṹ
        pAddress = (TIENodeAddress *)pFrame->Data();
        pAddress->yType(TIENodeAddress::type);
        pAddress->uLen(4);
        pAddress->uAddress(uLocalAddr);
        SendGTPRequest(pFrame);
        MSGOUT(MTS_NETWORK, 
               MSGOUT_LEVEL_IMPORTANT, 
               S_NET_SEND_NODE_ALIVE_REQ);
    }
}


/******************************************************
// ������:  CGtpStack::CreateGTPFrame
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ����һ��GTP֡
// ����:
//       ����1 :UINT2 uLen -- ��������֡�����ݶγ���
// ����ֵ: SGtpFrame* 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
SGtpFrame* CGtpStack::CreateGTPFrame(UINT2 uLen)
{
    int nSize = m_nGtpHeadLen + uLen;
    SGtpFrame *pFrame = (SGtpFrame *)new BYTE[nSize];

    if (pFrame)
    {
        memset(pFrame, 0, nSize);
        pFrame->yTag(m_GtpNodeInfo.uGtpVersion);
        pFrame->FillV0HeadEx();
        pFrame->uLen(uLen);
    }
    return pFrame;
}


/******************************************************
// ������:  CGtpStack::CopyGTPFrame
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ����һ��GTP֡
// ����:
//       ����1 :SGtpFrame *pFrame
// ����ֵ: SGtpFrame* 
// ע��:
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
SGtpFrame* CGtpStack::CopyGTPFrame(SGtpFrame *pFrame)
{
    SGtpFrame *pResult;
    pResult = (SGtpFrame *)new BYTE[pFrame->GetSize()];
    if (pResult != NULL)
    {
        memcpy(pResult, pFrame, pFrame->GetSize());
    }

    return pResult;
}


/******************************************************
// ������:  CGtpStack::CreateTransferResponse
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    �ú�������һ��ָ����С�����ݴ���Ӧ��֡����дһ
//   ����֡���ݡ����ڸú����ڴ��������½�ֱ��ʹ��Ԥ��
//   ����õ�m_pAckFrame����������µ��ڴ棬��Ӧ�ͺ���
//   eleteTransferResponse���ʹ�ã�����ȷ���ͷ��ڴ�
// ����:
//       ����1 :BYTE nCause--Ӧ��ԭ����
//       ����2 :WORD nFrameCount--Ӧ�����кŵĸ���
// ����ֵ: SGtpFrame* --�ѳ�ʼ����Ӧ��֡
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
SGtpFrame* CGtpStack::CreateTransferResponse(BYTE nCause, 
                                             UINT2 uFrameCount)
{
    SGtpFrame *pRespFrame;
    UINT2 uSize = TIECAUSE_LENGTH + 
                  TIESN_RESPONSE_HEAD_LENGTH + 
                  uFrameCount * 2;

    //��m_pAckFrame�ռ��㹻����ʹ��m_pAckFrame�������һ���µ�
    if (uSize <= PREPARED_ACKFRAME_SIZE)
    {
        pRespFrame = m_pAckFrame;
    }
    else
    {
        pRespFrame = CreateGTPFrame(uSize);
    }

    //���֡����
    if (pRespFrame != NULL)
    {
        TIECause *pCause;
        TIESNResponse *pResponse;

        //���GTPЭ��ͷ
        pRespFrame->yTag(m_GtpNodeInfo.uGtpVersion);
        pRespFrame->FillV0HeadEx();
        pRespFrame->yType(DATA_TRANSFER_RESPONSE);
        pRespFrame->uSeqNum(0);
        pRespFrame->uLen(uSize);

        //���cause�ṹ
        pCause = (TIECause *)(pRespFrame->Data());
        pCause->yType(TIECause::type);
        pCause->yCause(nCause); 

        //���request reponse�ṹͷ
        pResponse = (TIESNResponse *)
            (pRespFrame->Data() + TIECAUSE_LENGTH);
        pResponse->yType(TIESNResponse::type);
        pResponse->uLen(uFrameCount * 2);
    };

    return pRespFrame;
}


/******************************************************
// ������:  CGtpStack::SendTransferResponse
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ����������һ��Ӧ��֡Ӧ��������֡�����Կ���
//   �ظ�������ȡ�����������Ӧ����һ֡һӦ�𣬸ú�����
//   ����ָ������Ӧ����
// ����:
//       ����1 :UINT2 uSeqNum -- ��ҪӦ���֡�����к�
//       ����2 :BYTE bCause -- Ӧ��ԭ��ֵ
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::SendTransferResponse(UINT2 uSeqNum, BYTE bCause)
{
    SGtpFrame *pRespFrame = CreateTransferResponse(bCause, 1);
    if (pRespFrame != NULL)
    {
        TIESNResponse *pSNResponse =(TIESNResponse *)
                (pRespFrame->Data() + TIECAUSE_LENGTH);
        pSNResponse->uSeqNums(0, uSeqNum);
        SendGTPResponse(pRespFrame);
        DeleteTransferResponse(pRespFrame);
    }
}


/******************************************************
// ������:  CGtpStack::DeleteTransferResponse
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ɾ����CreateTransferResponse��������֡
// ����:
//       ����1 :SGtpFrame *pFrame��CreateTransferResponse
//                               ������Ӧ��֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::DeleteTransferResponse(SGtpFrame *pFrame)
{
    if (pFrame != m_pAckFrame)
    {
        delete pFrame;
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPCancelData
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ����ȡ��������֡,�ú�������Ӧ�����ļ�ɾ��֮��Ӧ��
// ����:
//       ����1 :SGtpFrame *pFrame--���������ظ�������֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPCancelData(SGtpFrame *pFrame)
{
    SGtpFrame *pRespFrame;
    TIESNCanceled *pSNCanceled;
    TIESNResponse *pSNResponse;

    int nChkRst = CHECK_TLV(pFrame, "TIESNCanceled", 
                            TIESN_CANCELED_HEAD_LENGTH,
                            pSNCanceled, 
                            TIETRANSFER_CMD_LENGTH);
    if(nChkRst != 0)
    {
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_OPT_IE_INCRT);
        return;
    }

    //ɾ����Ӧ���ļ�
    //pSNCanceled->uLen�д����Ӧɾ���������к��б���ֽ���
    //��һ���������кŴ���������ֽ��У����Ի����������ֽ�����2
    for (int i = 0; i < pSNCanceled->uLen() / 2; i++)
    {
        char szPath[MAX_PATH];

        sprintf(szPath, 
                "%smd%u.dat", 
                m_szWorkDir, 
                pSNCanceled->uSeqNums(i));
        remove(szPath);
    }

    //����Ӧ��֡
    pRespFrame = CreateTransferResponse(CAUSE_REQUEST_ACCEPTED, 
                                        pSNCanceled->uLen() / 2);
    if (pRespFrame != NULL)
    {
        pSNResponse = (TIESNResponse *)
                      (pRespFrame->Data() + TIECAUSE_LENGTH);
        memcpy(pSNResponse->Data(), 
               pSNCanceled->Data(), 
               pSNCanceled->uLen());
        pRespFrame->uSeqNum(pFrame->uSeqNum());
        SendGTPResponse(pRespFrame);
        DeleteTransferResponse(pRespFrame);
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPDataTransferRequest
// ����:    Wangfeng
// ʱ��:    01-10-22
// ����:    ���ݴ�����������
// ����:
//       ����1 :SGtpFrame *pFrame ���յ���֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpStack::OnGTPDataTransferRequest(SGtpFrame *pFrame)
{
    //���TIETransferCommand��ѡ��δ��
    if(pFrame->uLen() == 0)
    {
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_MAND_IE_MISSING);

        return;
    }

    //2002-11-23 ��ŷ� ��Ӧ���ⵥ D19425
    if((pFrame->yTag() == GTP_V0_TAG1) || 
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        ((CGtpNetApp *)m_pNetApp)->m_pNmlBillPerf->Add(1);
        OnGTPSendData(pFrame, FALSE);
        return;
    }
    //�޸����

    TIETransferCommand *pCommand;
    int nChkRst = CHECK_TV(pFrame, "TIETransferCommand", 
                           TIETRANSFER_CMD_LENGTH,
                           pCommand, 0);
    //���TIETransferCommand��ѡ����д����
    if(nChkRst != 0)
    {
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_MAND_IE_INCRT);
        return;
    }

    switch (pCommand->yCmd())
    {
    //�յ�����֡
    case TC_SENDDATA:
        ((CGtpNetApp *)m_pNetApp)->m_pNmlBillPerf->Add(1);
        OnGTPSendData(pFrame, FALSE);
        break;

    //�յ������ظ�������֡
    case TC_SENDDATAMAYBEDUP:
        ((CGtpNetApp *)m_pNetApp)->m_pRptBillPerf->Add(1);
        OnGTPSendData(pFrame, TRUE);
        break;

    //�����ϴ������Ŀ����ظ�������֡��Ч
    case TC_CANCELDATA:
        OnGTPCancelData(pFrame);
        break;

    //�����ϴ������Ŀ����ظ�������֡��Ч
    case TC_RELEASEDATA:
        OnGTPReleaseData(pFrame);
        break;

    default:
        MSGOUT(MTS_NETWORK,
               MSGOUT_LEVEL_IMPORTANT,
               S_NET_UNRECOGNIZED_FRAME, 
               pCommand->yCmd());
        break;
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPEchoRequest
// ����:    Wangfeng
// ʱ��:    01-10-22
// ����:    ����Echo Request֡
// ����:
//       ����1 :SGtpFrame *pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpStack::OnGTPEchoRequest(SGtpFrame *pFrame)
{
    SGtpFrame *pRespFrame;
    TIERecovery *pRecovery;

    pRespFrame = CreateGTPFrame(TIERECOVERY_LENGTH);
    if (pRespFrame != NULL)
    {
        //���GTPͷ
        pRespFrame->yType(ECHO_RESPONSE);
        pRespFrame->uSeqNum(pFrame->uSeqNum());

        //���IE�ṹ
        pRecovery = (TIERecovery *)pRespFrame->Data();
        pRecovery->yType(TIERecovery::type);
        pRecovery->yRestartCount(0);
        SendGTPResponse(pRespFrame);
        delete pRespFrame;
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPEchoResponse
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    �ú����Ƚ�GSN��������Լ��GSN�Ƿ�������
//   ����GSN�������������״̬
// ����:
//       ����1 :SGtpFrame *pFrame -- ���յ���֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPEchoResponse(SGtpFrame *pFrame)
{
    TIERecovery *pRecovery;
    int nChkRst = CHECK_TV(pFrame, "TIERecovery", 
                           TIERECOVERY_LENGTH,
                           pRecovery, 0);
    if(nChkRst != 0)
    {
        return;
    }

    if (INVALID_RESTARTCOUNT == m_GtpNodeInfo.uGsnRestartCount) 
        m_GtpNodeInfo.uGsnRestartCount = pRecovery->yRestartCount();

    if (m_GtpNodeInfo.uGsnRestartCount != pRecovery->yRestartCount())
    {
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
               S_NET_GSN_RESTART, m_szName, pRecovery->yRestartCount());
        TRACE(MTS_NETWORK, S_NET_GSN_RESTART,
              m_szName, pRecovery->yRestartCount());

        //�����ʷ��¼
        ResetHisRecord();

        //����״̬
        m_GtpNodeInfo.uGsnRestartCount = pRecovery->yRestartCount();
        m_GtpNodeInfo.uPIDAtGSNRestart = m_pNetApp->GetNetWork()->GetPacketId();
        SaveStatus();
        m_pStatusInfo->Flush();
    }

    //�����ʷ����֡
    OnGTPResponse(pFrame);
}


/******************************************************
// ������:  CGtpStack::ResetHisRecord
// ����:    Wangfeng
// ʱ��:    01-11-5
// ����:    �����ʷ��¼
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-5       Wangfeng
******************************************************/
void CGtpStack::ResetHisRecord()
{
    //ɾ����ʷ��¼
    Clear(TRUE);

    //ɾ�����п����ظ�����
    ACE_DIR *pdir = ACE_OS::opendir(m_szWorkDir);
    if(NULL == pdir) return;

    dirent *pdirent;
    char szPath[MAX_PATH];
    const char *szFileName;
    while(pdirent = ACE_OS::readdir(pdir))
    {
        szFileName = pdirent->d_name;
        if(strstr(szFileName, "md") == szFileName)
        {
            sprintf(szPath, 
                    "%s%s", 
                    m_szWorkDir, 
                    szFileName);
            ACE_stat subStat;
            ACE_OS::stat(szPath, &subStat);
            if((subStat.st_mode & S_IFDIR) == 0)
            {
                remove(szPath);
            }
        }
        
    }
    ACE_OS::closedir(pdir);
}


/******************************************************
// ������:  CGtpStack::OnGTPMaybeDupData
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ��������ظ���֡���ú����������ظ���֡�����
//          ͨ��˽��Ŀ¼�µ���֡��Ϊ�����ļ��в�Ӧ��
// ����:
//       ����1 :SGtpFrame *pFrame -- ���������ظ�������֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPMaybeDupData(SGtpFrame *pFrame)
{
    //������֡���浽��֡��Ϊ�����ļ���
    char szPath[MAX_PATH];
    sprintf(szPath, 
            "%smd%u.dat", 
            m_szWorkDir,
            pFrame->uSeqNum()); 
    FILE *pFile = fopen(szPath, "wb");
    if (pFile != NULL)
    {
        int nSize = m_nGtpHeadLen + pFrame->uLen();
        int nWrite = fwrite(pFrame, nSize, 1, pFile);
        if (nWrite != 1)
        {
            fclose(pFile);
            remove(szPath);
            return;
        }
        fflush(pFile);
        fclose(pFile);

        //����Ӧ��֡
        SendTransferResponse(pFrame->uSeqNum());
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPNodeAliveRequest
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    Node Alive Request������
// ����:
//       ����1 :SGtpFrame *pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPNodeAliveRequest(SGtpFrame *pFrame)
{
    //���IP��ַ��ʽΪipv6���򷵻�Versoin Not Supported��Ϣ
    TIENodeAddress* pIpAddress = (TIENodeAddress* )(pFrame->Data());
    if (pFrame->uLen() < TIENODE_ADDRESS_LENGTH || pIpAddress->uLen() != 4)
    {
        SendVersionNotSupported(pFrame);
        return;
    }

    MSGOUT(MTS_NETWORK, 
           MSGOUT_LEVEL_IMPORTANT, 
           S_NET_RECV_NODE_ALIVE_REQ,
           m_szName);

    //���յ���GSN��NodeAliveRequest֡��������������������������е���ʷ��¼
    ResetHisRecord();

    //����״̬
    m_GtpNodeInfo.uGsnRestartCount = INVALID_RESTARTCOUNT;
    m_GtpNodeInfo.uPIDAtGSNRestart = m_pNetApp->GetNetWork()->GetPacketId();
    SaveStatus();
    m_pStatusInfo->Flush();

    //����Ӧ��
    SGtpFrame *pRespFrame;
    pRespFrame = CreateGTPFrame(0);
    if (pRespFrame != NULL)
    {
        //���GTPͷ
        pRespFrame->yType(NODE_ALIVE_RESPONSE);
        pRespFrame->uSeqNum(pFrame->uSeqNum());

        SendGTPResponse(pRespFrame);
        delete pRespFrame;
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPNormalData
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ��������������֡,�ú������Ȳ������յ���֡��
//          ���Ѿ����棬���Ѿ������򲹷�Ӧ�𣬷�������
//          ���ݵȴ�����
// ����:
//       ����1 :SGtpFrame *pFrame -- ���������ظ�������֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPNormalData(SGtpFrame *pFrame)
{
    TIEBillPackage *pPackage;

    //2002-11-23 ��ŷ� ��Ӧ���ⵥ D19425
    if((pFrame->yTag() == GTP_V0_TAG1) || 
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        pPackage = (TIEBillPackage *)pFrame->Data();
    }
    else
    {
        pPackage = (TIEBillPackage *)(pFrame->Data() + 
                                TIETRANSFER_CMD_LENGTH);
    }
    //�޸����

    switch (m_SNList.Lookup(pFrame->uSeqNum()))
    {
    //����ʷ��¼��δ���ָ����кţ���Ϊ�»�����Ӧ�����»������в�����
    case FS_NULL:
        {
            if (m_ListNewFrames.size() < 
                ((CGtpNetApp*)m_pNetApp)->GetMaxNewFrameCount())
            {
                SGtpFrame *pNewFrame = CopyGTPFrame(pFrame);
                if (pNewFrame != NULL)
                {
                    m_ListNewFrames.push_back(pNewFrame);
                    m_nNewDataSize += DATASIZE_OF_BILLPACKAGE(pFrame);
                    m_nNewRecordCount += pPackage->yCount();
                    m_SNList.Add(pFrame->uSeqNum());
                    //������ 2003-05-16
                    m_pNetApp->m_pReceiveBillCnt->Add(pPackage->yCount());                    
                }
            }
        }
        break;

    //����ʷ��¼�з��ָ����кŵ�֡�ѱ��ɹ�
    //���棬Ӧ����Ӧ��
    case FS_SAVED:
        if(std::find(m_ListSavedSN.begin(),
                     m_ListSavedSN.end(),
                     pFrame->uSeqNum()) == m_ListSavedSN.end())
        {
            m_ListSavedSN.push_back(pFrame->uSeqNum());
        }

        {
            ACE_Time_Value tNow = ACE_OS::gettimeofday() - m_tLastSendAdyFulfillTime;
            if(m_ListSavedSN.size() >= 100 || tNow >= ACE_Time_Value(2))
            {
                //�����ظ��������ѱ����֡���к�
                SendAleadyFulfill();
            }
        }
        break;

    //���ܳ��ֵ�����ֵΪFS_NEW��FS_SUBMITTED
    //�ֱ��ʾ���յ���֡�����ȴ�Ӧ���֡��Ӧ
    //ֱ�Ӷ���
    default:
        break;
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPReleaseData
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    �����ͷŵ�����֡,�ú�������Ӧ�ı��ͷŵ��ļ�
//          �����ڴ�ȴ���һ�α��ύ��Ȼ��ɾ����Ӧ���ļ�
// ����:
//       ����1 :SGtpFrame *pFrame -- ���������ظ�������֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPReleaseData(SGtpFrame *pFrame)
{
    TIESNReleased *pSNReleased;

    int nChkRst = CHECK_TLV(pFrame, "TIESNReleased",
                            TIESN_RELEASED_HEAD_LENGTH,
                            pSNReleased,
                            TIETRANSFER_CMD_LENGTH);
    if(nChkRst != 0)
    {
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_OPT_IE_INCRT);
        return;
    }

    for (int i = 0; i < pSNReleased->uLen() / 2; i++)
    {
        char szPath[MAX_PATH];
        UINT2 uSeqNum = pSNReleased->uSeqNums(i);

        //������֡���浽��֡��Ϊ�����ļ���
        sprintf(szPath, 
                "%smd%u.dat", 
                m_szWorkDir, 
                uSeqNum);
        FILE *pFile = fopen(szPath, "rb");
        if (pFile != NULL)
        {
            fseek(pFile, 0, SEEK_END);
            int nLen = ftell(pFile);
            if (nLen > m_nGtpHeadLen)
            {
                SGtpFrame *pGtpFrame = CreateGTPFrame(nLen - m_nGtpHeadLen);
                if (pGtpFrame != NULL)
                {
                    fseek(pFile, 0, SEEK_SET);
                    //�����ȷ�����ҳ�����ȷ������ͷŶ���׼������
                    if ((1 == fread(pGtpFrame, nLen, 1, pFile)) &&
                        ((nLen - m_nGtpHeadLen) == pGtpFrame->uLen()))
                    {
                        m_ListReleasedFrames.push_back(pGtpFrame);

                        TIEBillPackage *pPackage;

                        //2002-11-23 ��ŷ� ��Ӧ���ⵥ D19425
                        if((pGtpFrame->yTag() == GTP_V0_TAG1) || 
                           (pGtpFrame->yTag() == GTP_V06_TAG1))
                        {
                            pPackage = (TIEBillPackage *)pGtpFrame->Data();
                        }
                        else
                        {
                            pPackage = (TIEBillPackage *)(pGtpFrame->Data() + 
                                                    TIETRANSFER_CMD_LENGTH);
                        }
                        //�޸����

                        m_nNewRecordCount += pPackage->yCount();
                        m_nNewDataSize += DATASIZE_OF_BILLPACKAGE(pGtpFrame);
                    }
                    else
                    {
                        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
                               S_NET_READ_ERR_IN_RELEASE, szPath);
                    }
                }
            }
            fclose(pFile);
        }
        //����Ҳ����ļ�����ֱ�ӷ�����Ӧ֡
        else
        {
            SendTransferResponse(uSeqNum);
        }
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPResponse
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    GTP'Ӧ������, 
//          �ú�����������֡���������δӦ��֡����
//          �ú�������δӦ��֡�б����ҵ���ɾ����δӦ��֡
// ����:
//       ����1 :SGtpFrame *pGTPFrame -- ���յ���Ӧ��֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPResponse(SGtpFrame *pGTPFrame)
{
    UINT4 uSeqNum;
    SGtpReqFrameInfo *pInfo;
    
    uSeqNum = pGTPFrame->uSeqNum();
    if (m_MapReqFrames.find(uSeqNum) != m_MapReqFrames.end())
    {
        pInfo = (SGtpReqFrameInfo*)m_MapReqFrames[uSeqNum];
        delete pInfo->pFrame;
        delete pInfo;
        m_MapReqFrames.erase(uSeqNum);
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPSendData
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    �������ݽ��մ���
//          ����������ݰ�����ȡ�ʵ�����
//          ��������������:
//          1. �������ݣ�Ӧ�ж������Ƿ��ظ��������������Ӧ���
//             ����ȴ�������У�
//          2. test empty frame��Ӧ�����ѱ��滰�����У�����Ӧ��
//          3. �����ظ����ݣ�Ӧ���沢����Ӧ��
// ����:
//       ����1 :SGtpFrame *pFrame
//       ����2 :BOOL bMaybeDup
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPSendData(SGtpFrame *pFrame, BOOL bMaybeDup)
{
    TIEBillPackage *pPackage;

    //2002-11-23 ��ŷ� ��Ӧ���ⵥ D19425
    if((pFrame->yTag() == GTP_V0_TAG1) ||
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        int nChkRst = CHECK_TLV(pFrame, "TIEBillPackage", 
                                TIEBILL_BACKAGE_HEAD_LENGTH,
                                pPackage, 0);
        if(nChkRst != 0)
        {
            SendTransferResponse(pFrame->uSeqNum(), 
                                 CAUSE_MAND_IE_INCRT);
            return;
        }
    }
    else
    {
        int nChkRst = CHECK_TLV(pFrame, "TIEBillPackage", 
                                TIEBILL_BACKAGE_HEAD_LENGTH,
                                pPackage,
                                TIETRANSFER_CMD_LENGTH);
        if(nChkRst != 0)
        {
            if(pFrame->uLen() >= TIETRANSFER_CMD_LENGTH + IETLV_HEAD_LENGTH)
            {
                pPackage = (TIEBillPackage*) (pFrame->Data() + TIETRANSFER_CMD_LENGTH);
                if(pPackage->yType() == TIEBillPackage::type && pPackage->uLen() == 0)
                {
                    OnGTPTestEmptyFrame(pFrame);
                    return;
                }
            }
            SendTransferResponse(pFrame->uSeqNum(), 
                                 CAUSE_OPT_IE_INCRT);
            return;
        }
    }
    //�޸����

    //��¼��Ϊ0Ϊtest empty frame
    if (0 == pPackage->yCount())
    {
        OnGTPTestEmptyFrame(pFrame);
        return;
    }

    //��鳤���Ƿ��㹻
    if (DATASIZE_OF_IEBILLPACKAGE(pPackage) < 0)
    {
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
               S_NET_WRONG_IEBILLPACKAGE,
               pPackage->yCount(), pPackage->uLen());
        if((pFrame->yTag() == GTP_V0_TAG1) ||
           (pFrame->yTag() == GTP_V06_TAG1))
        {
            SendTransferResponse(pFrame->uSeqNum(), 
                                 CAUSE_MAND_IE_INCRT);
        }
        else
        {
            SendTransferResponse(pFrame->uSeqNum(), 
                                 CAUSE_OPT_IE_INCRT);
        }
        return;
    }

    //�������յ��Ļ�������
    if (bMaybeDup)
    {
        OnGTPMaybeDupData(pFrame);
    }
    else
    {
        OnGTPNormalData(pFrame);
    }
}


/******************************************************
// ������:  CGtpStack::OnGTPTestEmptyFrame
// ����:    Wangfeng
// ʱ��:    01-10-22
// ����:    �������֡
// ����:
//       ����1 :SGtpFrame *pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpStack::OnGTPTestEmptyFrame(SGtpFrame *pFrame)
{
    //test empty frameֻ������״��FS_NULL��FS_SAVED���������������
    switch (m_SNList.Lookup(pFrame->uSeqNum()))
    {
    //������ʷ��¼��δ���֣��������CGFû���յ�����ʱ��ΪCGF
    //δ����û�����ӦӦ��accept
    case FS_NULL:
        SendTransferResponse(pFrame->uSeqNum());   
        break;

    //���Ѿ�������Ӧ��Ӧ��fulfilled
    case FS_SAVED:
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_REQUEST_DUPFULFILLED);
        break;

    //�������Ϊ���ڵȴ����棬��ӦӦ��
    default:
        break;
    }
}


/******************************************************
// ������:  CGtpNetApp::CGtpNetApp
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���캯��
// ����:
//       ����1 :CNetWork* const pNetWork
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpNetApp::CGtpNetApp(CNetWork* const pNetWork)
                       :CNetApp(pNetWork)
{
    m_nStackStatusInfoType = STACK_GTP;

    //�������ļ�
    CINIFile *pIniFile;
    pIniFile = new CINIFile((char *)GetCfgFilePath());
    pIniFile->Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);

    //��ȡ�»������г���
    m_nMaxNewFrameCount = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_MAX_NEW_FRM_COUNT,
                                      CFG_AP_KEY_MAX_NEW_FRM_COUNT_DEFAULT);

    //��ȡ����Echo Request֡��ʱ��������λΪ��
    m_nEchoReqInv = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_ECHO_REQ_INV,
                                      CFG_AP_KEY_ECHO_REQ_INV_DEFAULT);
    //ZYS�޸����ⵥ��D19464
    if(m_nEchoReqInv < 0 || m_nEchoReqInv > 300)
    {
        m_nEchoReqInv = 60;
    }

    //��ȡREQUEST��Ϣ���ͳ�ʱʱ�䣬��λΪ��
    m_nRequestTimeOut = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_REQ_TIMEOUT,
                                      CFG_AP_KEY_REQ_TIMEOUT_DEFAULT);
	
	if (m_nRequestTimeOut !=1 && m_nRequestTimeOut !=2 && m_nRequestTimeOut!=4 && m_nRequestTimeOut !=8)
	{
		m_nRequestTimeOut = CFG_AP_KEY_REQ_TIMEOUT_DEFAULT;
	}

    //��ȡ��ʱ�ط�����
    m_nTimeOutRepeat = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_TIMEOUT_RPT,
                                      CFG_AP_KEY_TIMEOUT_RPT_DEFAULT);
	if (m_nTimeOutRepeat < 1 || m_nTimeOutRepeat > 6)
	{
		m_nTimeOutRepeat = CFG_AP_KEY_TIMEOUT_RPT_DEFAULT;
	}

    //��ȡ����Node Alive֡��ʱ��������λΪ��
    m_nNodeAliveReqInv = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_NODE_ALIVE_REQ_INV,
                                      CFG_AP_KEY_NODE_ALIVE_REQ_INV_DEFAULT);

    //��ȡ������ַ
    pIniFile->GetString(strSec,
                       CFG_AP_KEY_LOCAL_IP_TO_PM,
                       CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT,
                       m_szLocalAddr,
                       20);

    //��ȡ�ض����ַ
    pIniFile->GetString(strSec,
                       CFG_AP_KEY_REDIRECTION_ADDR,
                       CFG_AP_KEY_REDIRECTION_ADDR_DEFAULT,
                       m_szRedirectionAddr,
                       20);

    pIniFile->Close();
    delete pIniFile;

    m_pRptBillPerf    = NULL;
    m_pNmlBillPerf    = NULL;
    m_pAllFrmPerf     = NULL;
    m_pGSNConnectPerf = NULL;
    m_pGtpQueueSize   = NULL;
    m_pGtpThroughPut  = NULL;
    m_uCurrentStackID = 0;
	//ͳ��Ga�ӿ�ƽ����Ӧʱ��
	m_pGtpResTime = 0 ;
}


/******************************************************
// ������:  CGtpNetApp::~CGtpNetApp
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpNetApp::~CGtpNetApp()
{
    m_pRptBillPerf    = 0;
    m_pNmlBillPerf    = 0;
    m_pAllFrmPerf     = 0;
    m_pGSNConnectPerf = 0;
}


/******************************************************
// ������:  CGtpNetApp::Init
// ����:    Wangfeng
// ʱ��:    03-06-27
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//03-06-27      Wangfeng
******************************************************/
int CGtpNetApp::Init(void)
{
    int nRet = CNetApp::Init();
    if(ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    //��ȡ�����̶�Ӧ�Ľ�����
    char strSec[20];
    sprintf(strSec, 
            "%s%d", 
            CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);
    /*
        ����ЧIP��ַ��	
    */
    //����ЧIP��ַ��ĸ��������м���GSN����˫ƽ�����
    INT4 nMultiIPGsnCount = IniFile.GetInt(strSec,
                                           CFG_AP_KEY_MULTI_IP_GSN_COUNT,
                                           CFG_AP_KEY_MULTI_IP_GSN_COUNT_DEFAULT);

    UINT4 uIPAddr;
    char szLineContent[1024];
    for(int i = 0; i < nMultiIPGsnCount; i++)
    {
        char szIPAddrSec[50];
        SNPRINTF(szIPAddrSec,
                 sizeof(szIPAddrSec),
                 "%s%d",
                 CFG_AP_KEY_GSN_IP_TABLE,
                 i + 1);
        szIPAddrSec[sizeof(szIPAddrSec) - 1] = '\0';

        IniFile.GetString(strSec,
                          szIPAddrSec,
                          "",
                          szLineContent,
                          sizeof(szLineContent));

        char szGsnName[1024];
        const char *p = strchr(szLineContent, ':');
        if(p != NULL)
        {
            strncpy(szGsnName, 
                    szLineContent, 
                    min(sizeof(szGsnName), (UINT4)p - (UINT4)&szLineContent[0]));
            szGsnName[min(sizeof(szGsnName), (UINT4)p - (UINT4)&szLineContent[0])] = '\0';
            memmove(szLineContent, p + 1, strlen(p + 1) + 1);
        }
        else
        {
            szGsnName[0] = '\0';
        }

        VECTOR<UINT4> ip_table;
        if (szLineContent[0] != '\0')
        {
            char *pToken = strtok(szLineContent, ",");
            while(pToken)
            {
                uIPAddr = inet_addr(pToken);

                if(0 == uIPAddr)
                {
                    TRACE(MTS_NETWORK, S_IP_ADDRESS_ERROR, pToken);
                    return ERR_FAIL;
                }

                if(m_GsnMultiIPMap.find(uIPAddr) != m_GsnMultiIPMap.end())
                {
                    TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
                    return ERR_FAIL;
                }
                
                bool bFind = false;
                int j ;
                for(j = 0; j < ip_table.size(); j++)
                {
                    if(uIPAddr == ip_table[j])
                    {
                        bFind = true;
                        break;
                    }
                }
                if(bFind)
                {
                    TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
                    return ERR_FAIL;
                }
                ip_table.push_back(uIPAddr);

                pToken = strtok(NULL, ",");
            }

            if(ip_table.size() > GSN_IP_TABLE_IPCOUNT_MAX)
            {
                TRACE(MTS_NETWORK, 
                      S_GSN_IP_TOO_MANY,
                      szIPAddrSec,
                      GSN_IP_TABLE_IPCOUNT_MAX);
                return ERR_FAIL;
            }
            else if(ip_table.size() > 0)
            {
                m_GsnMultiIPTable.push_back(ip_table);
            }

            int j ;
            for(j = 0; j < ip_table.size(); j++)
            {
                uIPAddr = ip_table[j];
                m_GsnMultiIPMap[uIPAddr] = i;
                if(szGsnName[0] != '\0')
                {
                    m_IPToStackName[uIPAddr] = szGsnName;
                }
            }
        }
    }

    /*
        ����ɾ����IP��ַ��
    */
    IniFile.GetString(strSec,
                      CFG_AP_KEY_DEL_GSN_IP,
                      CFG_AP_KEY_DEL_GSN_IP_DEFAULT,
                      szLineContent,
                      sizeof(szLineContent));
    char *pToken = strtok(szLineContent, ",");
    while(pToken)
    {
        uIPAddr = inet_addr(pToken);

        if(0 == uIPAddr)
        {
            TRACE(MTS_NETWORK, S_IP_ADDRESS_ERROR, pToken);
            return ERR_FAIL;
        }

        bool bFind = false;
        int j ;
        for(j = 0; j < m_DelGsnIPTable.size(); j++)
        {
            if(uIPAddr == m_DelGsnIPTable[j])
            {
                bFind = true;
                break;
            }
        }
        if(bFind)
        {
            TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
            return ERR_FAIL;
        }

        if(m_GsnMultiIPMap.find(uIPAddr) != m_GsnMultiIPMap.end())
        {
            TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
            return ERR_FAIL;
        }
        m_DelGsnIPTable.push_back(uIPAddr);
    }
    
    //ͳ��GTP'�ظ���������֡��(GTP_REPEATDATA_FRAME)
    BYTE yAPType = (BYTE)CMsgProcessor::GetProcessType() - PT_AP_BASE + 1;
    m_pRptBillPerf = CPerfItem::GetItem(yAPType, 
                                        MT_NETWORK,
                                        GTP_REPEATDATA_FRAME);

    //ͳ��GTP'������������֡��(GTP_DATA_FRAME)
    m_pNmlBillPerf = CPerfItem::GetItem(yAPType, 
                                        MT_NETWORK,
                                        GTP_NORMALDATA_FRAME);

    //ͳ��GTP'���л�������֡��(GTP_ALL_FRAME)
    m_pAllFrmPerf = CPerfItem::GetItem(yAPType, 
                                       MT_NETWORK,
                                       GTP_ALL_FRAME);

    //ͳ��������GSN��������(GTP_ALLGSN_CONNECT)
    m_pGSNConnectPerf = CPerfItem::GetItem(yAPType, 
                                           MT_NETWORK,
                                           GTP_ALLGSN_CONNECT);
    //ͳ��GTP'�Ķ��г���(PERF_GTP_QUEUE_SIZE)
    m_pGtpQueueSize = CPerfItem::GetItem(yAPType, 
                                         MT_NETWORK,
                                         PERF_GTP_QUEUE_SIZE,
                                         PERF_STATS_TOTAL_VALUE);


    //ͳ��GTP'֡��������(PERF_GTP_THROUGHPUT)
    m_pGtpThroughPut = CPerfItem::GetItem(yAPType, 
                                          MT_NETWORK,
                                          PERF_GTP_THROUGHPUT);

     //ͳ��Ga�ӿ�ƽ����Ӧʱ��
	m_pGtpResTime = CPerfItem::GetItem(yAPType,
										MT_NETWORK,
										PERF_GTP_RESTIME);
    return ERR_SUCCESS;
}


/******************************************************
// ������:  CGtpNetApp::Active
// ����:    Wangfeng
// ʱ��:    01-10-29
// ����:    ����GTP����Ӧ�ò�
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-29      Wangfeng
******************************************************/
void CGtpNetApp::Active(void)
{
    if(!m_bActive)
    {
        //���ε���ÿ��CGtpStack�����SendNodeAliveRequest����
        //��SendEchoRequest����
        MAP_ID_TO_PS::iterator i;
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            UINT4 uLocalAddr = ACE_OS::inet_addr(m_szLocalAddr);
            ((CGtpStack*)(*i).second)->SendNodeAliveRequest(uLocalAddr);
            
			//D19464 Z30971ɾ������ʱ����EchoRequest��Ϣ
			// ((CGtpStack*)(*i).second)->SendEchoRequest();
        }

        CNetApp::Active();

        m_pGSNConnectPerf->Set(m_StackMan.size());
    }
}


/******************************************************
// ������: 	CGtpNetApp::DeActive
// ����:    Wangfeng
// ʱ��:    02-2-4
// ����:    �����ȥ����
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//02-2-4        Wangfeng
******************************************************/
void CGtpNetApp::DeActive(void)
{
    if(m_bActive)
    {
        //�����ض�����Ϣ
        SendRedirection();

        CNetApp::DeActive();
    }
}

/******************************************************
// ������:  CGtpNetApp::SendRedirection
// ����:    Wangfeng
// ʱ��:    01-11-1
// ����:    ������GSN���ض�����Ϣ
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-1       Wangfeng
******************************************************/
void CGtpNetApp::SendRedirection(void)
{
    BYTE  yCause = CAUSE_SYSTEM_FAILURE;
    if(CNetWork::NWSTATUS_AUXUPGRADE == m_pNetWork->GetCurStatus())
    {
        yCause = CAUSE_OWN_GODOWN;
    }
    
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        ((CGtpStack*)(*i).second)->SendRedirectionRequest(m_szRedirectionAddr, yCause);
    }
}

int CGtpNetApp::SendFrame2Ex(MSG_INTRA* pMsg, CProtocolStack* pStack)
{
    m_pGtpThroughPut->Add(pMsg->Len);

    return CNetApp::SendFrame2Ex(pMsg, pStack);
}

/******************************************************
// ������:  CGtpNetApp::LoadStatus
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����״̬�ļ��ж�ȡ����Э��ջ��״̬��Ϣ
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CGtpNetApp::LoadStatus(void)
{
    //����CNetStatusInfo::EnumStackId�������������Э��ջ�ı��

    //Ϊÿ����Ŵ���һ��CGtpStack����
    //��ӳ���ϵ<���, ��Ӧ��CGtpStack�����ָ��>���뵽m_StackMan�У�
    //���ε���ÿ��CGtpStack����LoadStatus����
    int nNum = 0;
    UINT4 uStackId;
    int i, j, k;
    while((uStackId = CNetStatusInfo::EnumStackId(nNum++, STACK_GTP)) 
          != INVALID_STACK_ID)
    {
        if(m_uCurrentStackID < uStackId)
        {
            m_uCurrentStackID = uStackId;
        }

        if(m_StackMan.find(uStackId) == m_StackMan.end())
        {
            //����һ��GTPЭ��ջ����
            CGtpStack* pGtpStack = new CGtpStack(this, uStackId);
            if(!pGtpStack) return ERR_FAIL;

            //װ���Э��ջ��״̬�ļ�
            if(pGtpStack->LoadStatus() != ERR_SUCCESS)
            {
                delete pGtpStack;
                TRACE(MTS_NETWORK, S_NET_LOAD_GTP_STACK, uStackId);
                return ERR_FAIL;
            }
            
            VECTOR<UINT4> out_ip_table;
            pGtpStack->GetGsnIPTable(out_ip_table);

            /*
                ����ǵ�IP��ַ��GSN,��IP��ַ������m_DelGsnIPTable��,��ɾ����GSN��Э��ջ
            */
            if(out_ip_table.size() == 1)
            {
                BOOL bFound = FALSE;
                VECTOR<UINT4>::iterator iter = m_DelGsnIPTable.begin();
                for(; iter != m_DelGsnIPTable.end(); iter++)
                {
                    if(*iter == out_ip_table[0])
                    {
                        m_DelGsnIPTable.erase(iter);
                        bFound = TRUE;
                        break;
                    }
                }

                if(bFound)
                {
                    out_ip_table.clear();
                }
            }
            /*
                ����Ƕ�IP��ַ��GSN,������û��һ��IP��ַ������
                ���õ�IP��ַ����,��ɾ����GSN��Э��ջ
            */
            else if(out_ip_table.size() > 1)
            {
                BOOL bFound = FALSE;
                for(j = 0; j < out_ip_table.size(); j++)
                {
                    UINT4 uIPAddr = out_ip_table[j];
                    if(m_GsnMultiIPMap.find(uIPAddr) != m_GsnMultiIPMap.end())
                    {
                        bFound = TRUE;
                        break;
                    }
                }

                if(!bFound)
                {
                    out_ip_table.clear();
                }
            }

            //ɾ��IP��ַ��Ϊ�յ�Э��ջ����
            if(out_ip_table.size() == 0)
            {
                char szMsg[100];
                sprintf(szMsg, 
                        S_NET_REMOVE_GTP_STACK,
                        pGtpStack->GetStackId());
                MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
                TRACE(MTS_NETWORK, szMsg);
                CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);

                pGtpStack->m_bDeleteFileOnDestroy = TRUE;
                delete pGtpStack;
                pGtpStack = NULL;
            }

            if(NULL != pGtpStack)
            {
                m_StackMan[uStackId] = pGtpStack;
                for(j = 0; j < out_ip_table.size(); j++)
                {
                    UINT4 uIPAddr = out_ip_table[j];
                    if(m_IPToStackMap.find(uIPAddr) == m_IPToStackMap.end())
                    {
                        m_IPToStackMap[uIPAddr] = pGtpStack;
                    }
                    else
                    {
                        //˵�����GSN����ͬһ��IP��������ƴ���
                        ASSERT(0);
                    }
                }
            }
        }
        else
        //˵��״̬�ļ��д�������ID��ͬ��Э��ջ�����Ǳ�̴���
        {
            ASSERT(0);
        }
    }
    
    //�������к��б�
    int nRet = LoadPacketSNStatus();
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }
    
    /*
     	��ʱm_uCurrentStackID�ܱ�֤��������״̬�ļ��м�¼�����Э��ջ��
        ˳�����m_GsnMultiIPTable�и���GSN��IP��ַ�����³�T1����
        ����ÿһ��GSN��IP��ַ�����³�T2�����ڸ���Э��ջ������ҵ�һ��
        IP��ַ������T1��Э��ջ����������ҵ�������T1�����¸�Э��ջ��
        IP��ַ��T2����������Э��ջ������T1��IP��ַɾ���������ĳЭ��ջ
        ��IP��ַ��ȫ��ɾ������ɾ����Э��ջ
    */
    for(i = 0; i < m_GsnMultiIPTable.size(); i++)
    {
        VECTOR<UINT4>& ip_table = m_GsnMultiIPTable[i];

        UINT4 uIPAddr = 0;
        CGtpStack* pGtpStack = NULL;
        for (j = 0; j < ip_table.size(); j++)
        {
            uIPAddr = ip_table[j];
            if(m_IPToStackMap.find(uIPAddr) != m_IPToStackMap.end())
            {
                pGtpStack = m_IPToStackMap[uIPAddr];
                break;
            }
        }

        if(NULL != pGtpStack)
        //�ҵ��˵�i��IP��ַ������Ӧ��Э��ջ
        {
            //����IP��ַ������ip_table�е�GSN��IP��ַ��
            for (j = 0; j < ip_table.size(); j++)
            {
                UINT4 uIPAddr_k = ip_table[j];
                if(m_IPToStackMap.find(uIPAddr_k) != m_IPToStackMap.end())
                {
                    CGtpStack* pGtpStack_k = m_IPToStackMap[uIPAddr_k];
                    pGtpStack_k->DelGsnIp(uIPAddr_k);
                    m_IPToStackMap.erase(uIPAddr_k);
                }
            }

            //���ԭЭ��ջIP��ַ��m_IPToStackMap�е�ӳ���ϵ
            VECTOR<UINT4> out_ip_table;
            pGtpStack->GetGsnIPTable(out_ip_table);
            for (k = 0; k < out_ip_table.size(); k++)
            {
                UINT4 uIPAddr_k = out_ip_table[k];
                if(m_IPToStackMap.find(uIPAddr_k) != m_IPToStackMap.end())
                {
                    m_IPToStackMap.erase(uIPAddr_k);
                }
            }

            pGtpStack->UpdateGsnIPTable(ip_table);
            for(j = 0; j < ip_table.size(); j++)
            {
                UINT4 uIPAddr = ip_table[j];
                if(m_IPToStackMap.find(uIPAddr) == m_IPToStackMap.end())
                {
                    m_IPToStackMap[uIPAddr] = pGtpStack;
                }
                else
                {
                    //˵�����GSN����ͬһ��IP��������ƴ���
                    ASSERT(0);
                }
            }
        }
    }

    MAP_ID_TO_PS::iterator iter = m_StackMan.begin();
    for (; iter != m_StackMan.end(); iter++)
    {
        CGtpStack* pGtpStack = (CGtpStack*)iter->second;
        VECTOR<UINT4> out_ip_table;
        pGtpStack->GetGsnIPTable(out_ip_table);

        //���û�����GSN��IP��ַɾ��ʱ������Ҳ����Э��ջɾ��
        if(out_ip_table.size() == 0)
        {
            char szMsg[100];
            sprintf(szMsg, 
                    S_NET_REMOVE_GTP_STACK,
                    pGtpStack->GetStackId());
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
            TRACE(MTS_NETWORK, szMsg);
            CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);

            pGtpStack->m_bDeleteFileOnDestroy = TRUE;
            delete pGtpStack;

            m_StackMan.erase(iter);
            iter = m_StackMan.begin();
        }
        else
        {
            if(m_IPToStackName.find(out_ip_table[0]) != m_IPToStackName.end())
            {
                pGtpStack->SetGsnName(m_IPToStackName[out_ip_table[0]].c_str());
            }
        }
    }

    return nRet;
}


/******************************************************
// ������:  CGtpNetApp::SaveStatus
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����״̬�ļ���д������Э��ջ��״̬��Ϣ
// ����:
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CGtpNetApp::SaveStatus(void)
{
    int nRet = ERR_SUCCESS;
    
    //�ȱ������к��б��ļ�
    if(GetNetWork()->IsWaitForResponse())
    {
        nRet = SavePacketSNStatus();
        if (nRet != ERR_SUCCESS) return nRet;
    }

    //���ε���ÿ��CGtpStack�����SaveStatus������
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        nRet = (*i).second->SaveStatus();
        if (nRet != ERR_SUCCESS)
        {
            TRACE(MTS_NETWORK, S_NET_SAVE_GTP_STATUS);
            return nRet;
        }
    }

    //��մ�ɾ��IP��ַ�������
    if(m_DelGsnIPTable.size() == 0)
    {
        CINIFile IniFile(GetCfgFilePath());
        IniFile.Open();

        //��ȡ�����̶�Ӧ�Ľ�����
        char strSec[20];
        sprintf(strSec, 
                "%s%d", 
                CFG_SEC_AP, 
                CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);

        char szLineContent[1024];
        IniFile.GetString(strSec,
                          CFG_AP_KEY_DEL_GSN_IP,
                          CFG_AP_KEY_DEL_GSN_IP_DEFAULT,
                          szLineContent,
                          sizeof(szLineContent));
        StringLRTrim(szLineContent);
        if(szLineContent[0] != '\0')
        {
            IniFile.WriteString(strSec,
                                CFG_AP_KEY_DEL_GSN_IP,
                                CFG_AP_KEY_DEL_GSN_IP_DEFAULT);
        }
    }

    m_pGSNConnectPerf->Set(m_StackMan.size());
    
    return nRet;
}


/******************************************************
// ������:  CGtpNetApp::LoadPacketSNStatus
// ����:    Wangfeng
// ʱ��:    01-10-26
// ����:    ��������Ŀ¼�µ�����״̬�ļ�������������
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
int CGtpNetApp::LoadPacketSNStatus()
{
    LIST <UINT4> files;
    UINT4 uFileNumber;
    int nRet = ERR_SUCCESS;
    char szPath[MAX_PATH];

    //������������ʷ���ż�¼�ļ��б�
    ACE_DIR *pdir = ACE_OS::opendir(CNetWork::m_szWorkDir);
    if(NULL == pdir) return ERR_FAIL;

    dirent *pdirent;
    while(pdirent = ACE_OS::readdir(pdir))
    {
        const char *szFileName = pdirent->d_name;
        if(strstr(szFileName, ".sn"))
        //����ļ����д����ַ���".sn"
        {
            //����Ŀ¼�ļ�
            sprintf(szPath, 
                    "%s%s", 
                    CNetWork::m_szWorkDir, 
                    szFileName);
            ACE_stat subStat;
            ACE_OS::stat(szPath, &subStat);
            if(subStat.st_mode & S_IFDIR)
            {
                continue;
            }

            //�����ļ���ȡ���к�
            char *pFileName = new char[strlen(szFileName) + 1];
            strcpy(pFileName, szFileName);
            if (char *pDot = strchr(pFileName, '.')) 
            {
                *pDot = '\0';
            }
            uFileNumber = atoi(pFileName);
            delete [] pFileName;

            //���ļ�����Ϊ���ֻ�Ϊ�㣬����atoi���ص�ֵΪ0����
            //Ϊ����˵�����ļ�����״̬�ļ�����Ӧװ�룻��Ϊ0.sn��
            //�����������Ŵ�1��ʼ���ʸ��ļ�Ҳ����װ��
            if ((uFileNumber > 0) && 
                (uFileNumber <= m_pNetWork->GetPacketId()))
            {
                files.push_back(uFileNumber);
            }
            else
            {
                remove(szPath);
            }
        }
    }
    ACE_OS::closedir(pdir);

    //�����Ŵ�С��������
    if (files.size() > 0) files.sort();

    //װ���¼
    CGtpStack *pTempStack = new CGtpStack(this, INVALID_STACK_ID);
    pTempStack->m_bDeleteFileOnDestroy = TRUE;
    LIST <UINT4>::iterator i = files.begin();
    while ((i != files.end()) && (nRet == ERR_SUCCESS))
    {

        //��ȡ�ļ���
        uFileNumber = *i;
        i++;

        sprintf(szPath, 
                "%s%d.sn", 
                CNetWork::m_szWorkDir, 
                uFileNumber);

        //�򿪲������ļ�
        FILE* file = fopen(szPath, "rb");
        if (!file)
        //���ļ�ʧ��
        {
            TRACE(MTS_NETWORK,
                   S_NET_OPEN_SEQ_FILE_ERR, 
                   szPath, errno, strerror(errno));
            MSGOUT(MTS_NETWORK,
                   MSGOUT_LEVEL_URGENT,
                   S_NET_OPEN_SEQ_FILE_ERR, 
                   szPath, errno, strerror(errno));
            continue;
        }

        UINT4 uStackId;
        SGtpSNFile snFile;
        snFile.pStatusFile = file;
        snFile.pFileRef = new CGtpSNFileRef(uFileNumber);

        //״̬�ļ��п��ܴ����Ѿ���ɾ������Э��ջ��״̬����
        //��״̬ʱ����������Щ���ݡ����ﴴ��һ��ʱЭ��ջ��
        //�ڶ�����Щ������Ҫ��״̬���ݡ�
        pTempStack->Clear(TRUE);
        while (1 == fread(&uStackId, 
               sizeof(uStackId), 1, 
               snFile.pStatusFile))
        {
            if(m_uCurrentStackID < uStackId)
            {
                m_uCurrentStackID = uStackId;
            }

            if(m_StackMan.find(uStackId) != m_StackMan.end())
            {
                nRet = ((CGtpStack *)m_StackMan[uStackId])->
                                     LoadSNList(&snFile);
            }
            else
            {
                nRet = pTempStack->LoadSNList(&snFile);
            }
            if (nRet != ERR_SUCCESS)
            {
                TRACE(MTS_NETWORK, S_NET_READ_SEQ_FILE, uFileNumber);
                MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_URGENT,
                       S_NET_READ_SEQ_FILE, uFileNumber);
                break;
            }
        }

        fclose(snFile.pStatusFile); 

        //������ļ��еİ�������Э��ջ��DISCARD�����ļ��ڴ˴�ɾ��
        snFile.pFileRef->Release();
    }
    delete pTempStack;

    return ERR_SUCCESS;
}


/******************************************************
// ������:  CGtpNetApp::SavePacketSNStatus
// ����:    Wangfeng
// ʱ��:    01-10-26
// ����:    ����ǰ���и�Э��ջ�����к��б������Ե�ǰ
//          ֡��Ϊ�����ļ��С��ú�����ÿ�����ݷ���֮���
//          �յ�Ӧ��֮�󱻵��á����ڽ���Ҫ�����ѷ���(�ύ)
//          �����кţ����ѷ������к��б���ÿ�η���ʱ�γɣ�
//          Ӧ��ʱ��գ����ֻ�ڷ���֮�����������
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
int CGtpNetApp::SavePacketSNStatus()
{
    FILE* file;
    char szPath[MAX_PATH];
    int nRet = ERR_SUCCESS;

    //��״̬�ļ����������򴴽�
    sprintf(szPath, "%s%u.sn", 
            CNetWork::m_szWorkDir, 
            m_pNetWork->GetPacketId());
    file = fopen(szPath, "wb");
    if (!file)
    //���ļ�ʧ��
    {
        TRACE(MTS_NETWORK, S_NET_CREATE_SEQ_FILE, 
              szPath, errno, strerror(errno));
        return ERR_FAIL;
    }

    SGtpSNFile snFile;
    snFile.pStatusFile = file;
    snFile.pFileRef = new CGtpSNFileRef(m_pNetWork->GetPacketId());

    //���α�����������Э��ջ��ID�͵�����SaveSNList����
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        //дһ��Э��ջ�����к��б���Ϣ
        CGtpStack *pStack = (CGtpStack *)((*i).second);
        UINT4 uStackId = (*i).first;
        if((fwrite(&uStackId, sizeof(uStackId), 1, file) != 1) || 
           (pStack->SaveSNList(&snFile) != ERR_SUCCESS))
        {
            nRet = ERR_FAIL;
            break;
        }
    }
    fflush(file);
    fclose(file);
    snFile.pFileRef->Release();

    //���д���к��б��ļ�ʧ�ܣ���ɾ�����ļ�
    if(nRet != ERR_SUCCESS)
    {
        remove(szPath);
    }

    return nRet;
}

void CGtpNetApp::Timer1Sec(void)
{
    CNetApp::Timer1Sec();

    m_pGtpQueueSize->Set(GetReadyDataSize());
}

/******************************************************
// ������:  CGtpNetApp::OnReceive
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������������Э��֡
// ����:
//       ����1 :SNetFrame* pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CGtpNetApp::OnReceive(SNetFrame* pFrame)
{
    m_pGtpThroughPut->Add(pFrame->Len);

    if (m_bActive)
    {
        //���ⴴ��һ��0.0.0.0��Э��ջ
        UINT4 uClientIp = pFrame->SNetAddrPair.ClientIp;
        if(uClientIp == 0)
        {
            MSGOUT(MTS_NETWORK, 
                   MSGOUT_LEVEL_NORMAL,
                   S_RECV_GSN_IP_0);
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame);
            return;
        }
        
        //��m_IPToStackMap���ҵ���ֵΪpFrame->SNetAddrPair.ClientIp
        //��Ԫ�أ����ø�Ԫ����ָ���CGtpStack�����OnReceive����
        if(m_IPToStackMap.find(uClientIp) == m_IPToStackMap.end())
        {
            CGtpStack* pGtpStack = new CGtpStack(this, ++m_uCurrentStackID);
            if(NULL == pGtpStack)
            {
                m_uCurrentStackID--;
                m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_FAIL);
                return;
            }

            VECTOR<UINT4> ip_table;
            if(m_GsnMultiIPMap.find(uClientIp) != m_GsnMultiIPMap.end())
            {
                int nGsnIndex = m_GsnMultiIPMap[uClientIp];
                if(nGsnIndex < m_GsnMultiIPTable.size())
                {
                    ip_table = m_GsnMultiIPTable[nGsnIndex];
                }
                else
                {
                    //��ƴ���
                    m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_FAIL);
                    ASSERT(0);
                    return;
                }
            }
            else
            {
                ip_table.push_back(uClientIp);
            }

            for(int i = 0; i < ip_table.size(); i++)
            {
                m_IPToStackMap[ip_table[i]] = pGtpStack;
            }
            if(m_IPToStackMap.find(uClientIp) == m_IPToStackMap.end())
            {
                //��ƴ���
                m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_FAIL);
                delete pGtpStack;
                m_uCurrentStackID--;
                ASSERT(0);
                return;
            }

            //����GSN��
            if(m_IPToStackName.find(uClientIp) != m_IPToStackName.end())
            {
                pGtpStack->SetGsnName(m_IPToStackName[uClientIp].c_str());
            }

            pGtpStack->UpdateGsnIPTable(ip_table);
            m_StackMan[m_uCurrentStackID] = pGtpStack;

            char szMsg[100];
            struct in_addr addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
            addr.s_addr = uClientIp;
#else //_PLATFORM_HPUX
            addr.S_un.S_addr = uClientIp;
#endif//_PLATFORM_HPUX
            sprintf(szMsg, S_NET_CREATE_GTP_STACK,
                   ACE_OS::inet_ntoa(addr),
                   pFrame->SNetAddrPair.ClientPort);
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
            TRACE (MTS_NETWORK, szMsg);
            CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);

            m_pGSNConnectPerf->Set(m_StackMan.size());
        }

        if((m_Stack_To_RecvInv.size() < m_pNetWork->GetLicenseCount())
            || (m_Stack_To_RecvInv.find((UINT4)m_IPToStackMap[uClientIp]) 
                != m_Stack_To_RecvInv.end()))
        {
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame);
            m_IPToStackMap[uClientIp]->OnReceive(pFrame);
        }
        else
        {
            //ģ����������Ȩ��
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_LICENSE_EXPIRED);
        }

		CNetApp::OnReceive(pFrame);
        return;
	}

    m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_APP_NOT_ACTIVED);
}


/******************************************************
// ������:  CGtpNetApp::GetMaxNewFrameCount
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    �������յ�֡������󳤶�
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
int CGtpNetApp::GetMaxNewFrameCount()
{
    return m_nMaxNewFrameCount;
}


/******************************************************
// ������:  CGtpNetApp::GetEchoReqInterval
// ����:    Wangfeng
// ʱ��:    01-10-25
// ����:    ���ط���Echo Request֡��ʱ��������λΪ����
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
int CGtpNetApp::GetEchoReqInv()
{
    return m_nEchoReqInv;
}


/******************************************************
// ������:  CGtpNetApp::GetRequestTimeOut
// ����:    Wangfeng
// ʱ��:    01-10-25
// ����:    ����REQUEST��Ϣ���ͳ�ʱʱ��
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
int CGtpNetApp::GetRequestTimeOut()
{
    return m_nRequestTimeOut;
}


/******************************************************
// ������:  CGtpNetApp::GetTimeOutRepeat
// ����:    Wangfeng
// ʱ��:    01-10-25
// ����:    ���س�ʱ�ط�����
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
int CGtpNetApp::GetTimeOutRepeat()
{
    return m_nTimeOutRepeat;
}


/******************************************************
// ������:  CGtpNetApp::GetNodeAliveReqInv
// ����:    Wangfeng
// ʱ��:    01-10-25
// ����:    ����Node Alive Request֡��ʱ��������λΪ
//          ����(��������Ҫ���)
// ����:
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
int CGtpNetApp::GetNodeAliveReqInv()
{
    return m_nNodeAliveReqInv;
}


/******************************************************
// ������:  CGtpNetApp::GetLocalAddress
// ����:    Wangfeng
// ʱ��:    01-10-25
// ����:    ��ñ�����ַ
// ����:
// ����ֵ: const char* 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
const char* CGtpNetApp::GetLocalAddress()
{
    return m_szLocalAddr;
}


/******************************************************
// ������:  CGtpTrack::CGtpTrack
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���캯��
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpTrack::CGtpTrack()
{
    m_pMmlReport = new CMMLReport("CG");
}


/******************************************************
// ������:  CGtpTrack::~CGtpTrack
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpTrack::~CGtpTrack()
{
    if (m_pMmlReport)
    {
        delete m_pMmlReport;
        m_pMmlReport = 0;
    }
}


/******************************************************
// ������:  CGtpTrack::Analysis
// ����:    Wangfeng
// ʱ��:    01-10-30
// ����:    ʵʩ��GTP'Э��֡�ķ���,�ɺ����ڲ������ڴ棬
//          �����߲���Ҫ�ͷ�
// ����:
//       ����1 :void* const pFrame--��������Э��֡
//                        nProcRst--������
// ����ֵ: const char* -- ���ط������õ��ַ���
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-30      Wangfeng
******************************************************/
const char* CGtpTrack::Analysis(void* const pFrame, int nProcRst)
{

    char szBuffer[1024];

    m_pMmlReport->MakeReportHead();
    m_pMmlReport->AddString(S_PROTOCOL_TRACK_HEAD);
    m_pMmlReport->AddString(STR_NEWLINE);

    //��pFrameǿ��ת����SNetFrame*����
    SNetFrame* pMsg = (SNetFrame*) pFrame;
    struct in_addr SvrAddr;
    struct in_addr CltAddr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
    SvrAddr.s_addr = pMsg->SNetAddrPair.ServerIp;
    CltAddr.s_addr = pMsg->SNetAddrPair.ClientIp;
#else //_PLATFORM_HPUX
    SvrAddr.S_un.S_addr = pMsg->SNetAddrPair.ServerIp;
    CltAddr.S_un.S_addr = pMsg->SNetAddrPair.ClientIp;
#endif//_PLATFORM_HPUX
    if (MT_NETWORK == pMsg->ReceiverMid)
    //��ʾ����������������Ϣ
    {
        //���ͷ�
        sprintf(szBuffer, "GSN (%s %s:%d)", CMsgProcessor::g_szPtDes, 
                ACE_OS::inet_ntoa(CltAddr), 
                pMsg->SNetAddrPair.ClientPort);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_SENDER, szBuffer);

        //���շ�
        sprintf(szBuffer, "CG (%s:%d)",
                ACE_OS::inet_ntoa(SvrAddr), 
                pMsg->SNetAddrPair.ServerPort);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_RECEIVER, szBuffer);
    }
    else
    //��ʾ����CG�����������Ϣ
    {
        //���ͷ�
        sprintf(szBuffer, "CG (%s %s:%d)", CMsgProcessor::g_szPtDes, 
                ACE_OS::inet_ntoa(SvrAddr), 
                pMsg->SNetAddrPair.ServerPort);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_SENDER, szBuffer);

        //���շ�
        sprintf(szBuffer, "GSN (%s:%d)",
                ACE_OS::inet_ntoa(CltAddr), 
                pMsg->SNetAddrPair.ClientPort);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_RECEIVER, szBuffer);
    }

    //�������кŶ�
    SGtpFrame* pGTPFrame = (SGtpFrame *)pMsg->Data;
    sprintf(szBuffer, "%d", pGTPFrame->uSeqNum());
    m_pMmlReport->MakeReportItem2(S_TRACK_SEQ_NUM, szBuffer);

    //�����Ϣ�����������Ȳ���һ��GTP'��֡ͷ����
    if(pMsg->Len < 1)
    {
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                      S_UNRECOGNIZED);
        m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                      S_FRAME_NOT_CORRECT);
    }
    //����汾��֧��
    else if (!pGTPFrame->CheckGTPFrame())
    {
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                      S_UNRECOGNIZED);
        m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                      S_VERSION_NOT_SUPPORTED);
    }
    //�����Ϣ�����������Ȳ�����д��Ϣ����
    else if(pMsg->Len < 2)
    {
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                      S_UNRECOGNIZED);
        m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                      S_FRAME_NOT_CORRECT);
    }
    else
    {
        m_strTrackMsg = "";
        switch (pGTPFrame->yType())
        {
        //��������
        case ECHO_REQUEST:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_ECHO_REQUEST);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_ECHO_REQUEST);
                if(nProcRst != ERR_SUCCESS)
                {
                    m_strTrackMsg += GetProcRstExplain(nProcRst);
                    m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                                  m_strTrackMsg.c_str());
                }
            }
            break;

        //����Ӧ��
        case ECHO_RESPONSE:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_ECHO_RESPONSE);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_ECHO_RESPONSE);
                OnGTPEchoResponse(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //���ݴ�������
        case DATA_TRANSFER_REQUEST:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_DATA_TRANSFER_REQUEST);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_DATA_TRANSFER_REQUEST);
                OnGTPDataTransferRequest(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //���ݴ���Ӧ��
        case DATA_TRANSFER_RESPONSE:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_DATA_TRANSFER_RESPONSE);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_DATA_TRANSFER_RESPONSE);
                OnGTPDataTransferResponse(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //�ڵ�Ӧ��
        case NODE_ALIVE_RESPONSE:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_NODE_ALIVE_RESPONSE);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_NODE_ALIVE_RESPONSE);
                if(nProcRst != ERR_SUCCESS)
                {
                    m_strTrackMsg += GetProcRstExplain(nProcRst);
                    m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                                  m_strTrackMsg.c_str());
                }
            }
            break;

        //�ڵ�����
        case NODE_ALIVE_REQUEST:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_NODE_ALIVE_REQUEST);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_NODE_ALIVE_REQUEST);
                OnGTPNodeAliveRequest(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //�ض�������
        case REDIRECTION_REQUEST:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_REDIRECTION_REQUEST);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_REDIRECTION_REQUEST);
                OnGTPRedirectionRequest(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //�ض���Ӧ��
        case REDIRECTION_RESPONSE:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_REDIRECTION_RESPONSE);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_REDIRECTION_RESPONSE);
                OnGTPRedirectionResponse(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //�汾��֧��
        case VERSION_NOT_SUPPORTED:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_VERSION_NOT_SUPPORTED);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_VERSION_NOT_SUPPORTED);
                if(nProcRst != ERR_SUCCESS)
                {
                    m_strTrackMsg += GetProcRstExplain(nProcRst);
                    m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                                  m_strTrackMsg.c_str());
                }
            }
            break;

        default:
            sprintf(szBuffer, "%d(%s)",
                    pGTPFrame->yType(),
                    S_UNRECOGNIZED);
            m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, szBuffer);
            break;
        }
    }

    m_pMmlReport->MakeReportTail();
    return m_pMmlReport->GetReport();
}


/******************************************************
// ������:  CGtpTrack::OnGTPCancelData
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ����ȡ��������֡
// ����:
//       ����1 :SGtpFrame *pFrame--���������ظ�������֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPCancelData(SGtpFrame *pFrame)
{
    TIESNCanceled *pSNCanceled;
    int nChkRst = CHECK_TLV(pFrame, "TIESNCanceled",
                            TIESN_CANCELED_HEAD_LENGTH,
                            pSNCanceled,
                            TIETRANSFER_CMD_LENGTH);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_OPT_IE_INCRT;
        return;
    }

    char szBuf[20];
    sprintf(szBuf, " = %d;", pSNCanceled->uLen() / 2);
    m_strTrackMsg += S_RECORD_COUNT;
    m_strTrackMsg += szBuf;
}


/******************************************************
// ������:  CGtpTrack::OnGTPDataTransferRequest
// ����:    Wangfeng
// ʱ��:    01-10-22
// ����:    �������ݴ�������֡
// ����:
//       ����1 :SGtpFrame *pFrame ���յ���֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpTrack::OnGTPDataTransferRequest(SGtpFrame *pFrame)
{
    //���TIETransferCommand��ѡ��δ��
    if(pFrame->uLen() == 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_MISSING;
        return;
    }
    
    //2002-11-23 ��ŷ� ��Ӧ���ⵥ D19425
    if((pFrame->yTag() == GTP_V0_TAG1) || 
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        OnGTPSendData(pFrame);
        return;
    }
    //�޸����
    
    TIETransferCommand *pCommand;

    int nChkRst = CHECK_TV(pFrame, "TIETransferCommand", 
                           TIETRANSFER_CMD_LENGTH,
                           pCommand, 0);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }

    switch (pCommand->yCmd())
    {
    //�յ�����֡
    case TC_SENDDATA:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_IE_SEND_DATA;
        m_strTrackMsg += "; ";
        OnGTPSendData(pFrame);
        break;

    //�յ������ظ�������֡
    case TC_SENDDATAMAYBEDUP:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_IE_SEND_DATA_MAYBE_DUP;
        m_strTrackMsg += "; ";
        OnGTPSendData(pFrame);
        break;

    //�����ϴ������Ŀ����ظ�������֡��Ч
    case TC_CANCELDATA:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_IE_CANCEL_DATA;
        m_strTrackMsg += "; ";
        OnGTPCancelData(pFrame);
        break;

    //�����ϴ������Ŀ����ظ�������֡��Ч
    case TC_RELEASEDATA:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_IE_RELEASE_DATA;
        m_strTrackMsg += "; ";
        OnGTPReleaseData(pFrame);
        break;

    default:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_UNRECOGNIZED;
        m_strTrackMsg += ";";
        break;
    }
}


/******************************************************
// ������:  CGtpTrack::OnGTPDataTransferResponse
// ����:    Wangfeng
// ʱ��:    01-10-30
// ����:    ����CG��GSN���͵����ݴ���Ӧ��
// ����:
//       ����1 :SGtpFrame *pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-30      Wangfeng
******************************************************/
void CGtpTrack::OnGTPDataTransferResponse(SGtpFrame *pFrame)
{//�յ���pFrameΪ�����ֽ���
    
    TIECause *pCause;
    TIESNResponse *pResponse;
    char szBuf[20];

    //����cause�ṹ
    pCause = (TIECause *)(pFrame->Data());
    m_strTrackMsg += S_REPONSE_CAUSE;
    m_strTrackMsg += " = ";
    m_strTrackMsg += GetResponseCauseDesp(pCause->yCause());
    m_strTrackMsg += "; ";

    //Transfer Reponse�ṹͷ
    pResponse = (TIESNResponse *) (pFrame->Data() + TIECAUSE_LENGTH);
    sprintf(szBuf, " = %d;", pResponse->uLen() / 2);
    m_strTrackMsg += S_RECORD_COUNT;
    m_strTrackMsg += szBuf;

    sprintf(szBuf, " = %d;", pResponse->uSeqNums(0));
    m_strTrackMsg += S_FIRST_FRAME_SEQ;
    m_strTrackMsg += szBuf;
}


/******************************************************
// ������:  CGtpTrack::GetResponseCauseDesp
// ����:    Wangfeng
// ʱ��:    01-10-30
// ����:    ���Ӧ��ԭ��ֵ������
// ����:
//       ����1 :int nCause
// ����ֵ: const char* 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-30      Wangfeng
******************************************************/
const char* CGtpTrack::GetResponseCauseDesp(int nCause)
{
    switch(nCause)
    {
    case CAUSE_REQUEST_ACCEPTED:
        return S_CAUSE_REQUEST_ACCEPTED;

    case CAUSE_REQUEST_DUPFULFILLED:
        return S_CAUSE_REQUEST_DUPFULFILLED;

    case CAUSE_SYSTEM_FAILURE:
        return S_CAUSE_SYSTEM_FAILURE;

    case CAUSE_TRANS_BUF_FULL:
        return S_CAUSE_TRANS_BUF_FULL;

    case CAUSE_RECEIVE_BUF_FULL:
        return S_CAUSE_RECEIVE_BUF_FULL;

    case CAUSE_ANOTHER_GODOWN:
        return S_CAUSE_ANOTHER_GODOWN;

    case CAUSE_OWN_GODOWN:
        return S_CAUSE_OWN_GODOWN;

    case CAUSE_CDR_DECODE_ERR:
    //177 -- CDR decoding error
        return S_CAUSE_CDR_DECODE_ERR;

    case CAUSE_INVALID_MSG_FMT:
    //193 -- Invalid message format
        return S_CAUSE_INVALID_MSG_FMT;

    case CAUSE_VER_NOT_SUP:
    //198 -- Version not supported
        return S_CAUSE_VER_NOT_SUP;

    case CAUSE_NO_RES_AVAILABLE:
    //199 -- No resources available
        return S_CAUSE_NO_RES_AVAILABLE;

    case CAUSE_SERVICE_NOT_SUP:
    //200 -- Service not supported
        return S_CAUSE_SERVICE_NOT_SUP;

    case CAUSE_MAND_IE_INCRT:
    //201 -- Mandatory IE incorrect
        return S_CAUSE_MAND_IE_INCRT;

    case CAUSE_MAND_IE_MISSING:
    //202 -- Mandatory IE missing
        return S_CAUSE_MAND_IE_MISSING;

    case CAUSE_OPT_IE_INCRT:
    //203 -- Optional IE incorrect
        return S_CAUSE_OPT_IE_INCRT;

    case CAUSE_SYSTEM_FAILURE1:
    //204 -- System failure
        return S_CAUSE_SYSTEM_FAILURE;

    case CAUSE_REQUEST_FULFILLED:
    //253 -- Request already fulfilled
        return S_CAUSE_REQUEST_FULFILLED;

    case CAUSE_REQ_NOT_FULFILL:
    //255 -- Request not fulfilled
        return S_CAUSE_REQ_NOT_FULFILL;
    
    //by ldf 2002.4.17��Ӧ���ⵥD12742
    default:
        {
             break;
        }
    //�޸����

    }

    return S_CAUSE_UNRECOGNIZED;
}


/******************************************************
// ������:  CGtpTrack::OnGTPEchoResponse
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ��¼��������
// ����:
//       ����1 :SGtpFrame *pFrame -- ���յ���֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPEchoResponse(SGtpFrame *pFrame)
{
    TIERecovery *pRecovery;

    int nChkRst = CHECK_TV(pFrame, "TIERecovery", 
                           TIERECOVERY_LENGTH,
                           pRecovery, 0);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }

    char szBuf[20];
    sprintf(szBuf, " = %d;", pRecovery->yRestartCount());
    m_strTrackMsg += S_TRACK_RESTART_COUNT;
    m_strTrackMsg += szBuf;
}


/******************************************************
// ������:  CGtpTrack::OnGTPNodeAliveRequest
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    ����Node Alive Request֡
// ����:
//       ����1 :SGtpFrame *pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPNodeAliveRequest(SGtpFrame *pFrame)
{
    //���IP��ַ��ʽΪipv6���򷵻�Versoin Not Supported��Ϣ
    TIENodeAddress* pIpAddress = (TIENodeAddress* )(pFrame->Data());

    char szBuf[100];
    if (pFrame->uLen() < TIENODE_ADDRESS_LENGTH)
    {
        m_strTrackMsg += S_FRAME_NOT_CORRECT;
        return;
    }
    else if (pIpAddress->uLen() != 4)
    {
        sprintf(szBuf, " = %s;", S_IP_ADDRESS_V6);
    }
    else
    {
        struct in_addr addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        addr.s_addr = pIpAddress->uAddress();
#else //_PLATFORM_HPUX
        addr.S_un.S_addr = pIpAddress->uAddress();
#endif//_PLATFORM_HPUX
        sprintf(szBuf, " = %s;", ACE_OS::inet_ntoa(addr));
    }
    m_strTrackMsg += S_NODE_ALIVE_ADDRESS;
    m_strTrackMsg += szBuf;
}


/******************************************************
// ������:  CGtpTrack::OnGTPReleaseData
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    �����ͷŵ�����֡
// ����:
//       ����1 :SGtpFrame *pFrame -- ���������ظ�������֡
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPReleaseData(SGtpFrame *pFrame)
{
    TIESNReleased *pSNReleased;

    int nChkRst = CHECK_TLV(pFrame, "TIESNReleased", 
                            TIESN_RELEASED_HEAD_LENGTH,
                            pSNReleased,
                            TIETRANSFER_CMD_LENGTH);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_OPT_IE_INCRT;
        return;
    }

    char szBuf[20];
    sprintf(szBuf, " = %d;", pSNReleased->uLen() / 2);
    m_strTrackMsg += S_RECORD_COUNT;
    m_strTrackMsg += szBuf;
}


/******************************************************
// ������:  CGtpTrack::OnGTPSendData
// ����:    Wangfeng
// ʱ��:    01-10-23
// ����:    �����������ݽ���֡
// ����:
//       ����1 :SGtpFrame *pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPSendData(SGtpFrame *pFrame)
{
    TIEBillPackage *pPackage;

    //2002-11-23 ��ŷ� ��Ӧ���ⵥ D19425
    if((pFrame->yTag() == GTP_V0_TAG1) || 
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        int nChkRst = CHECK_TLV(pFrame, "TIEBillPackage",
                                TIEBILL_BACKAGE_HEAD_LENGTH,
                                pPackage, 0);
        if(nChkRst != 0)
        {
            m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
            return;
        }
    }
    else
    {
        int nChkRst = CHECK_TLV(pFrame, "TIEBillPackage",
                                TIEBILL_BACKAGE_HEAD_LENGTH,
                                pPackage, 
                                TIETRANSFER_CMD_LENGTH);
        if(nChkRst != 0)
        {
            m_strTrackMsg += S_CAUSE_OPT_IE_INCRT;
            return;
        }
    }
    //�޸����

    char szBuf[20];
    sprintf(szBuf, " = %d; ", pPackage->yCount());
    m_strTrackMsg += S_RECORD_COUNT;
    m_strTrackMsg += szBuf;

    sprintf(szBuf, " = %d;", DATASIZE_OF_IEBILLPACKAGE(pPackage));
    m_strTrackMsg += S_RECORD_SIZE;
    m_strTrackMsg += szBuf;
}


/******************************************************
// ������:  CGtpTrack::OnGTPRedirectionRequest
// ����:    Wangfeng
// ʱ��:    01-10-31
// ����:    �����ض�������
// ����:
//       ����1 :SGtpFrame *pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-31      Wangfeng
******************************************************/
void CGtpTrack::OnGTPRedirectionRequest(SGtpFrame *pFrame)
{
    TIECause* pCause;
    int nChkRst = CHECK_TV(pFrame, "TIECause",
                           TIECAUSE_LENGTH,
                           pCause, 0);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }
    //��дTIECause�ṹ
    m_strTrackMsg += S_REDIRECTION_CAUSE;
    m_strTrackMsg += " = ";
    m_strTrackMsg += GetResponseCauseDesp(pCause->yCause());
    m_strTrackMsg += "; ";

    //�����Ƿ���û��TIERecommendNodeAddress��
    if(pFrame->uLen() == TIECAUSE_LENGTH)
    {
        return;
    }

    TIERecommendNodeAddress* pNodeAddress;
    nChkRst = CHECK_TLV(pFrame, "TIERecommendNodeAddress",
                        IETLV_HEAD_LENGTH,
                        pNodeAddress, TIECAUSE_LENGTH);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }

    char szBuf[100];
    if (pNodeAddress->uLen() != 4)
    {
        sprintf(szBuf, " = %s;", S_IP_ADDRESS_V6);
    }
    else
    {
        struct in_addr addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        addr.s_addr = pNodeAddress->uAddress();
#else //_PLATFORM_HPUX
        addr.S_un.S_addr = pNodeAddress->uAddress();
#endif//_PLATFORM_HPUX
        sprintf(szBuf, " = %s;", ACE_OS::inet_ntoa(addr));
    }
    m_strTrackMsg += S_REDIRECTION_ADDRESS;
    m_strTrackMsg += szBuf;
}

void CGtpTrack::OnGTPRedirectionResponse(SGtpFrame *pFrame)
{
    TIECause* pCause;
    int nChkRst = CHECK_TV(pFrame, "TIECause",
                           TIECAUSE_LENGTH,
                           pCause, 0);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }

    m_strTrackMsg    = S_REDIRECTION_CAUSE;
    m_strTrackMsg   += " = ";
    m_strTrackMsg   += GetResponseCauseDesp(pCause->yCause());
}
