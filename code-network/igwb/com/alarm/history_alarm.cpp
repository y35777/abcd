    // HistoryAlarm.cpp: implementation of the CHistoryAlarm class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/frame.h"
#include "../include/toolbox.h"
#include "../utility/config.h"
#include "history_alarm.h"
#include "resource.h"
#include "ace/ACE.h"
#include "ace/OS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//��������  CHistoryAlarm��Ĺ��캯���д����szAlarmFile���Ա����
//          m_szAlarmFile��Ӧ��ʾ��ʷ�澯�Ĵ洢·��
//�������
//*szAlarmFile      ����Ÿ澯�ļ���
// nAlarmUpperLimit ���澯��Ŀ����
// nAlarmDelNum     ���澯��Ŀ��������ʱ,һ����ɾ���ĸ澯����
//������2001-10-20
////////////////////////////////////////////////////////////////////////
CHistoryAlarm::CHistoryAlarm(const char* szAlarmFile,int nAlarmDelNum,int nAlarmUpperLimit)
{
    //�����ļ������޺�׺��Ϣ��
    sprintf(m_szAlarmFile,"%s", szAlarmFile);

    //��ʼ������Ա����
    m_nAlarmUpperLimit  = nAlarmUpperLimit;
    m_nAlarmDelNum      = nAlarmDelNum;
    m_nRecordNum = 0;
    //by ldf 2002.1.21��Ϊ��ʼ����ˮ��Ϊ100
    //by ldf 2004-01-31 OM���ģ������99����һ����Ҫ��CHK ALM�л��
    m_uCsn = 99;
    m_nReadIndex        = 0;
    m_pAlarmUtil        = NULL;
    m_uSyn = 99;
    m_uMMLAlmBaseID = 0;
}

CHistoryAlarm::~CHistoryAlarm()
{
    Close();
    if(NULL != m_pAlarmUtil)
    {
        delete m_pAlarmUtil;
        m_pAlarmUtil = NULL;
    #ifdef _OM_MODIFY
        //���ڿ���ɾ���澯��Ϣʱ����ļ�������ĸ澯����
        //����Ӧ����m_nRecordNum���Ƚض̸澯�ļ�
        //by ldf 2004-01-31 OM����
        ACE_OS::truncate(m_szAlarmFile,SIZEOF_ALARMHEAD*5+SIZEOF_HISTORYALARM*m_nRecordNum);	
        m_nRecordNum = 0;//by lx SWPD04555
    #endif

    }
}

//////////////////////////////////////////////////////
//��������  ����Open(void)����ʷ�澯���ļ�m_szAlarmFile
//�������  ��
//�������  ��
//����ֵ    ����ERR_SUCCESS(0)��ʾ�ļ��򿪳ɹ�,���ط�0��ʾ��ʧ��
//������ 2001-10-20
/////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::Open()
{
    // �����ÿ����ȡ�Ƿ���XML
    CINIFile ConfigFile(GetCfgFilePath());
    ConfigFile.Open();
    m_bUseXml = ConfigFile.GetInt(CFG_SEC_ALARM, 
                                  CFG_USE_XML, 
                                  CFG_USE_XML_DEFAULT);

    m_pAlarmUtil = CreateAlarmUtilObj();
    if(NULL == m_pAlarmUtil)
    {
        return ERR_FAIL;
    }

    STRING szFilePath = m_szAlarmFile;
    szFilePath       += "/";
    szFilePath       += GetAlarmFile();

    //�˺�m_szAlarmFile�д����ļ�����׺��Ϣ
    SNPRINTF(m_szAlarmFile, sizeof(m_szAlarmFile), "%s", szFilePath.c_str());
    m_szAlarmFile[sizeof(m_szAlarmFile) - 1] = '\0';

    //����ļ��Ѵ���ر�
    Close();

    //�ж��ļ��Ƿ����
    BOOL bFileExist = (access(m_szAlarmFile, 0) == 0);
    if (!bFileExist)
    {
        int nPos  = szFilePath.find_last_of('/');
#ifdef _PLATFORM_WIN32
        int nPos2 = szFilePath.find_last_of('\\');
#else //_PLATFORM_WIN32
        int nPos2 = -1;
#endif //_PLATFORM_WIN32
        nPos      = max(nPos, nPos2);
        if (nPos <= 0)
        {
            return ERR_FAIL;
        }

        STRING szDirPath = szFilePath.substr(0,nPos);

        //�����ļ�����Ŀ¼
        if (!CreateDeepDir(szDirPath.c_str()))
        {
            return ERR_FAIL;
        }
    }

    //��ʼ��XMLFile,��ָ����XML�ļ������ڣ����½�һ��������ָ��������XML�ļ�
    if (ERR_SUCCESS != m_pAlarmUtil->OpenFile(m_szAlarmFile))
    {
        return ERR_OPEN_HISTORY_ALARM;
    }

    if (bFileExist) //�ļ�ԭ���ʹ���
    {
        //��ȡ�澯��ˮ����Ϣ,����ȡ�����򷵻�ʧ��
        m_pAlarmUtil->ReadHeadElement(m_uCsn);
        if(ERR_SUCCESS != m_pAlarmUtil->ReadHeadElement(m_uCsn))
        {
            Close();
            TRACE(MTS_ALARM,S_ALARM_OPEN_HISTORY_FAIL);
            return ERR_OPEN_HISTORY_ALARM;
        }
#ifdef _OM_MODIFY
        //by ldf 2004-01-31 OM����
        //��ȡ�澯ͬ������Ϣ,����ȡ�����򷵻�ʧ��
        m_pAlarmUtil->ReadHeadElementSyn(m_uSyn);
        if(ERR_SUCCESS != m_pAlarmUtil->ReadHeadElementSyn(m_uSyn))
        {
            Close();
            TRACE(MTS_ALARM,S_ALARM_OPEN_HISTORY_FAIL);
            return ERR_OPEN_HISTORY_ALARM;
        }


        //��ȡ�澯ͬ����׼����Ϣ,����ȡ�����򷵻�ʧ��
        m_pAlarmUtil->ReadHeadElementMMLBaseID(m_uMMLAlmBaseID);
        if(ERR_SUCCESS != m_pAlarmUtil->ReadHeadElementMMLBaseID(m_uMMLAlmBaseID))
        {
            Close();
            TRACE(MTS_ALARM,S_ALARM_OPEN_HISTORY_FAIL);
            return ERR_OPEN_HISTORY_ALARM;
        }
        //end
#endif



        //���ļ��򿪳ɹ�ʱ,��õ�ǰ�澯���ݿ��ļ��и澯��¼��
        m_nRecordNum = m_pAlarmUtil->GetCount();
        
        return ERR_SUCCESS;
    }
    else//�ļ�������ʱ�����澯�ļ�
    {
        //д��澯��ˮ��
        //by ldf 2002.1.21,��Ϊ��ʼ��ˮ��Ϊ100
        m_uCsn = 99;
        if(WriteCsn(m_uCsn) != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }
//#ifdef _OM_MODIFY
        //by ldf 2004-01-31 OM����
        m_uSyn = 99;

        if(WriteSyn(m_uSyn) != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }

        m_uMMLAlmBaseID = 0;
        if(WriteMMLBaseID(m_uMMLAlmBaseID) != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }
        //end
//#endif
        return ERR_SUCCESS;
    }
}

int CHistoryAlarm::Close()
{
    //�رո澯�ļ�
    if(m_pAlarmUtil != NULL)
    {
        m_pAlarmUtil->CloseFile();
    }
    
    //by ldf 2002.1.21
    m_uCsn = 99;
    m_uSyn = 99;

    return ERR_SUCCESS;//self check add
}

UINT4 CHistoryAlarm::CreateCsn()
{
    //���ɸ澯��ˮ��,�����¸澯���ݿ��ļ�
    ++m_uCsn;
    WriteCsn(m_uCsn);

    //���ظ澯��ˮ��
    return m_uCsn;
}

//by ldf 2004-01-31 OM����
UINT4 CHistoryAlarm::CreateSyn()
{
    //���ɸ澯��ˮ��,�����¸澯���ݿ��ļ�
    ++m_uSyn;
    WriteSyn(m_uSyn);

    //���ظ澯��ˮ��
    return m_uSyn;
}

//////////////////////////////////////////////////
//��������  ���˸澯��ˮ��
//�������  ��
//�������  ��
//����ֵ    ��Ч��Csnֵ
//������2001-10-20
///////////////////////////////////////////////
UINT4 CHistoryAlarm::RollBackCsn()
{
    //���m_nCsnΪ100; ��Ϊ�澯����ʼ��ˮ����100
    //by ldf 2002.1.21
    if(100 > m_uCsn)
    {
        return 99;
    }

    //��m_nCsn��Ϊ1,�򷵻�--m_nCsn;
    --m_uCsn;

    //���¸澯���ݿ�
    WriteCsn (m_uCsn);

    return m_uCsn;
}

//by ldf 2004-01-31 OM����
UINT4 CHistoryAlarm::RollBackSyn()
{
    //���m_nSynΪ100; ��Ϊ�澯����ʼ��ˮ����100
    if(100 > m_uSyn)
    {
        return 99;
    }
    
    //��m_nSyn��Ϊ1,�򷵻�--m_nSyn;
    --m_uSyn;
    
    //���¸澯���ݿ�
    WriteSyn(m_uSyn);

    return m_uSyn;
}

///////////////////////////////////////////////////////////////
//��������  ��д�ļ�ͷ�ĸ澯��ˮ��
//�������  nCsn��Ҫд���ļ��ĸ澯��ˮ��
//�������  ��
//����ֵ    ERR_SUCCESS(0)��ʾ�ɹ�������Ϊʧ����
///////////////////////////////////////////////////////////////
int CHistoryAlarm::WriteCsn(UINT4 csn)
{
    
    //д�澯��ˮ��
    if(ERR_SUCCESS != m_pAlarmUtil->WriteHeadElement(csn))
    {
        TRACE(MTS_ALARM,S_ALARM_FILE_ERITECSN_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FAIL_FLUSH_CSN);
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

//by ldf 2004-01-31 OM����
int CHistoryAlarm::WriteSyn(UINT4 syn)
{
//#ifdef _OM_MODIFY
    //д�澯ͬ����
    if(ERR_SUCCESS != m_pAlarmUtil->WriteHeadElementSyn(syn))
    {
        TRACE(MTS_ALARM,S_ALARM_FILE_ERITESYN_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FAIL_FLUSH_SYN);
        return ERR_FAIL;
    }
//#endif
    return ERR_SUCCESS;

}

//by ldf 2004-01-31 OM����
int CHistoryAlarm::WriteMMLBaseID(UINT4 uMMLBaseID )
{
//#ifdef _OM_MODIFY
    //д�澯MML��׼��
    if(ERR_SUCCESS != m_pAlarmUtil->WriteHeadElementMMLBaseID(uMMLBaseID))
    {
        TRACE(MTS_ALARM,S_ALARM_FILE_ERITEMMLBASEID_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FAIL_FLUSH_MMLBASEID);
        return ERR_FAIL;
    }
//#endif

    return ERR_SUCCESS;

}
//end


UINT4 CHistoryAlarm::GetCsn()
{
    return m_uCsn;
}

//by ldf 2004-01-31 OM����
UINT4 CHistoryAlarm::GetSyn()
{
    return m_uSyn;
}

UINT4 CHistoryAlarm::GetMMLBaseID()
{
    return m_uMMLAlmBaseID;
}
//end
////////////////////////////////////////////////////////////////////////
//��������  ������һ���澯��¼pAlarm������ʷ�澯���β��,���ز������,
//          pAlarmָ��һ���澯��Ϣ�ṹ.
//�������  pAlarm - ��ʷ�澯�ṹ
//�������  ��
//����ֵ    ִ�гɹ�������0,ִ��ʧ�ܾ�����not 0ֵ
//////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::AddRecord(SHistoryAlarm *pAlarm)
{
    //���澯��¼��ӵ���m_nRecordNum���澯��¼��
    if(ERR_SUCCESS != m_pAlarmUtil->SaveHisAlarm(m_nRecordNum, *pAlarm))
    {
        TRACE(MTS_ALARM,S_ALARM_ADD_HIS_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    m_nRecordNum++;
    return ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////
//��������  ��������澯��¼���澯���ݿ��ļ���
//����      uNum��Ԥ׷�ӵ���ʷ�澯��β���ĸ澯��¼��
//      pAlarm��ָ��uNum���澯��Ϣ�ṹ
//�������  ��
//����ֵ    �ɹ���ӵ��澯�ļ��еĸ澯��¼��
////////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::AddRecord(SHistoryAlarm pAlarm[], UINT4 uNum)
{
    //��Ӹ澯���澯�ļ���,�����سɹ���ӵĸ澯��¼��
    int nWriteNum = 0;

    for(UINT4 i = 0; i<uNum; i++)
    {

        if(AddRecord(&pAlarm[i]) == ERR_SUCCESS)
        {
            nWriteNum++;
        }
        else
        {
            break;
        }

    }

    return nWriteNum;
}

//////////////////////////////////////////////////////////////////////
//��������  ���ݸ澯��ˮ�Ų��Ҹ澯��¼
//�������  uCsn - Ԥ׷�ӵ���ʷ�澯��β���ĸ澯��¼��
//�������  ��
//����ֵ    �ø澯�ڸ澯�ļ��е�λ��,�Ը澯��¼Ϊ��λ,Ϊ-1ʱ��ʾû���ҵ�
////////////////////////////////////////////////////////////////////////
//!!!!
//��OM���ģ�uCsn����ʷ�澯���в���Ψһ�ģ������뾡����Ҫʹ���������
//����ǲ�����ʷ���Ѿ��еĸ澯,�뿼��ʹ�øø澯��ͬ����,����FindRecordBySyn��
//uSyn�ڸ澯������Ψһ��
int CHistoryAlarm::FindRecord(UINT4 uCsn)
{
    //���ļ��ж�ȡ���и澯��¼��Ϣ,�����ȡ�����򷵻�-1;
    //���ڲ����Ǹ澯���кŻ�ת�����,��ʷ�澯���и澯��¼����ˮ���Ƿǵ������е�,
    //���Բ���������ҷ� ���ʼ����,�ҵ���һ����ˮ��ΪuCsn����ʷ�澯��¼
    int nRetPos = -1;
    int nLeft   = 0;
#ifdef _OM_MODIFY     //����OM���ĺ�����csn�������������������ƥ��ķ������в���
    int nRight  = m_nRecordNum - 1;
    SHistoryAlarm hisAlarm; 
    
    while(nLeft <= nRight)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nLeft, hisAlarm))
        {
            break;
        }
        
        if(uCsn == hisAlarm.uCsn)
        {
            nRetPos = nLeft;
            break;
        }
        
        nLeft++;
    }
#else         //���ڷ�OM���ĵİ汾���Բ��ö��ݲ��������Ч��

    int nRight  = m_nRecordNum - 1;
    int nMid    = (nRight-nLeft) / 2;

    UINT4 nLeftCsn,nMidCsn,nRightCsn;
    SHistoryAlarm hisAlarm;
    
    while(1)        
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nLeft, hisAlarm))
        {
            break;
        }
        nLeftCsn = hisAlarm.uCsn;
        
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nMid, hisAlarm))
        {
            break;
        }
        nMidCsn = hisAlarm.uCsn;
        
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nRight, hisAlarm))
        {
            break;
        }
        nRightCsn = hisAlarm.uCsn;
        
        if(nLeft == nMid)
        {
            if(uCsn == nLeftCsn)
                nRetPos = nLeft;
            else
                if(uCsn == nRightCsn)
                    nRetPos = nRight;
                break;
        }
        if(uCsn < nLeftCsn || uCsn > nRightCsn)
            break;
        if(uCsn == nMidCsn)
        {
            nRetPos = nMid;
            break;
        }
        else
        {
            if(uCsn < nMidCsn)
            {
                nRight = nMid;
            }
            else
            {
                nLeft = nMid;
            }
            //by lx 2003-09-28 SWPD01792
            nMid = nLeft + (nRight-nLeft)/2;
        }
    }
#endif
    return nRetPos;
}

////////////////////////////////////////////////////////////////////////////////////////
//��������  ������pAlarm������ʷ�澯������pAlarmӵ����ͬ�澯��ˮ��(CSN)����ʷ�澯��¼
//�������  pAlarmָ��һ����ʷ�澯��Ϣ
//�������  ��
//����ֵ    �ɹ�����0,ʧ�ܷ��ش�����
//////////////////////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::RewriteRecord(SHistoryAlarm *pAlarm)
{
    if(NULL == pAlarm)
    {
        return ERR_POINTER_NULL;
    }

    //���ݸ澯��ˮ�Ų��Ҹ澯���ö�Ӧ�澯��¼���ļ��е�λ�ã�
    int noffset;

    if((noffset = FindRecordBySyn(pAlarm->uSyn)) != -1)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->SaveHisAlarm(noffset, *pAlarm))
        {
            TRACE(MTS_ALARM,S_ALARM_ADD_HIS_FAIL);
            return ERR_WRITE_HISTORY_ALARM;
        }

        if(ERR_SUCCESS != m_pAlarmUtil->Flush())
        {
            TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
            return ERR_WRITE_HISTORY_ALARM;
        }

        return ERR_SUCCESS;
    }

    return ERR_WRITE_HISTORYALARM;
}

//////////////////////////////////////////////////////////////////////
//��������  ������pAlarmָ����ָ��uNum���澯��¼д����ʷ�澯���ͷ��
//      pAlarmָ��uNum���澯��Ϣ�ṹ
//�������  ��
//�������  ��
//����ֵ    ִ�гɹ�������0,ִ��ʧ�ܾ����ظ�ֵ
//////////////////////////////////////////////////////////////////////
int CHistoryAlarm::WriteRecord(SHistoryAlarm *pAlarm, UINT4 uNum)
{
    for(int i = 0; i < uNum; i++)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->SaveHisAlarm(i, *(pAlarm + i)))
        {
            TRACE(MTS_ALARM,S_ALARM_ADD_HIS_FAIL);
            return ERR_WRITE_HISTORY_ALARM;
        }
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    return ERR_SUCCESS;
}

/////////////////////////////////////////////////////////////////
//��������  ���ݸ澯��ˮ�Ų�����ʷ�澯��ɾ����Ӧ�ĸ澯��Ϣ
//�������  uCsn��Ҫɾ���澯��Ӧ�ĸ澯��ˮ��
//�������  ��
//����ֵ    ����ִ�гɹ�����0,ִ�д�����
///////////////////////////////////////////////////////////////////
int CHistoryAlarm::DeleteRecord(const UINT4 uSyn)
{
    //���Ҹ澯��ˮ��uSyn��Ӧ�澯����ʷ�澯���е�λ�ã�
    int noffset;
    SHistoryAlarm *pBuf = NULL;

#ifdef _OM_MODIFY
    noffset = FindRecordBySyn(uSyn);
#else
    noffset = FindRecord(uSyn);
#endif
    if(noffset  != -1)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->Delete(noffset))
        {
            return ERR_FAIL;
        }
        m_nRecordNum--;

        if(ERR_SUCCESS != m_pAlarmUtil->Flush())
        {
            TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
            return ERR_WRITE_HISTORY_ALARM;
        }

        return ERR_SUCCESS;
    }

    TRACE(MTS_ALARM,S_ALARM_WRITE_HIS_ERR);
    return ERR_WRITE_HISTORYALARM;
}

///////////////////////////////////////////////////////////
//��������  ����ɾ����ʷ�澯�������uRecordNum���澯��¼
//�������  ��
//�������  ��
//����ֵ    ����0��ʾɾ���ɹ�����ֵ��ʾɾ��ʧ��
///////////////////////////////////////////////////////////
int CHistoryAlarm::DeleteRecords(const UINT2 uRecordNum)
{
    if(0 == uRecordNum)
    {
        return ERR_SUCCESS;
    }

    int i = min(uRecordNum, m_nRecordNum) - 1;
    SHistoryAlarm hisAlarm;
    while(i >= 0)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(i, hisAlarm))
        {
            break;
        }

    #ifdef _OM_MODIFY
       if(ERR_SUCCESS == m_pAlarmUtil->Delete(i))
       {
            m_nRecordNum--;
            i--;
       }
    #else
       //�ǽ����¼��澯ֱ�ӱ��Ϊ��Ч
       if(hisAlarm.yAlarmType == AT_EVENT && hisAlarm.yAlarmLevel != AL_EMERGENCY)
       {
           //by ldf 2002.1.21
           if(ERR_SUCCESS == m_pAlarmUtil->Delete(i))
               m_nRecordNum--;
           i--;
           continue;
       }
       //ȷ�ϵĽ����¼��澯ֱ�ӱ��Ϊ��Ч
       if(hisAlarm.yAlarmType == AT_EVENT 
           && hisAlarm.yAlarmLevel == AL_EMERGENCY 
           && hisAlarm.szFAckOptr[0] != '\0')
       {
           if(ERR_SUCCESS == m_pAlarmUtil->Delete(i))
               m_nRecordNum--;
           i--;
           continue;
       }
       //�Ƿ�Ϊ���ϸ澯�������Ѿ��ָ���ȷ�ϣ����˼�¼���Ϊ��Ч
       if(hisAlarm.yAlarmType == AT_FAULT
           && hisAlarm.yResType != NOT_RECOVER 
           && hisAlarm.szFAckOptr[0] != '\0')
       {
           //by ldf 2002.1.21
           if(ERR_SUCCESS == m_pAlarmUtil->Delete(i))
               m_nRecordNum--;
       }
       i--;
    #endif
    }
    i = 0;
    
    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    return ERR_SUCCESS;
}

UINT4 CHistoryAlarm::GetRecordNum()
{
    return m_nRecordNum;
}

/////////////////////////////////////////////////////////////////////
//��������  ��ȡ��ʷ�澯���ļ�ָ�뵱ǰ��ָλ�ú��uNum����ʷ�澯��¼
//�������  uNum - ��Ҫ����ĸ澯��¼��
//�������  uNum - ʵ�ʶ���ĸ澯��¼��
//          pData - ���صļ�¼ͷָ��
//����ֵ    ����ִ�гɹ�����0,ִ��ʧ��ʱ���ش�����
//////////////////////////////////////////////////////////////////////
int CHistoryAlarm::GetRecord(SHistoryAlarm *pData, UINT4 &uNum)
{
    //��ȡ�澯���ݼ�¼
    if(0 == uNum || NULL == pData)
    {
        return ERR_SUCCESS;
    }

    //�����ļ�¼��
    int nMaxOffset = min(uNum + m_nReadIndex, m_nRecordNum);
    uNum = 0;
    while(m_nReadIndex < nMaxOffset)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(m_nReadIndex, *(pData + uNum)))
        {
            uNum = 0;
            return ERR_READ_HISTORY_ALARM;
        }
        m_nReadIndex++;
        uNum++;
    }

    return ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////
//��������  ���ļ���дָ�����origin�Ƶ���offset���澯��¼��
//�������  offset��Ҫ��λ�ĸ澯��¼ƫ��
//          origin����ʼλ��,ͬfseek,��������ֵ:
//                  SEEK_CUR��Current position of file pointer;
//                  SEEK_END��End of file;
//                  SEEK_SET��Beginning of file.
//                  Ĭ�������ΪSEEK_SET
//�������  ��
//����ֵ    ERR_SUCCESS��ʾ�ɹ�,����Ϊ�����룻
///////////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::Seek(long offset, int origin)
{
    //����offset��origin����Ӧ�ƶ���ƫ��
    switch(origin)
    {
    //��originΪSEEK_SETʱ,������offset>m_nRecordNum�����
    case SEEK_SET:
        m_nReadIndex = offset;
        break;

    //��originΪSEEK_CURʱ,����������ļ�ͷ��ƫ����
    case SEEK_CUR:
        //���㵱ǰ�ļ�ָ������λ��,�������Ա�֤tmpoffset�϶�ָ��һ���澯��¼��ͷλ��.
        m_nReadIndex += offset;
    break;

    //��originΪSEEK_ENDʱ,Ϊ�˱��ⲻ������¼���ƶ�ָ���Ӱ��
    //����offset������ļ�ͷ��ʼ��ƫ��ֵ
    case SEEK_END:
        if(offset > m_nRecordNum)
            offset = m_nRecordNum;
        m_nReadIndex = m_nRecordNum - offset;
        break;

    //Ĭ�������,�ƶ�����0����¼��
    default:
        m_nReadIndex = 0;
        break;
    }

    return 0;
}

/////////////////////////////////////////////////////////
//��������  ������ʷ�澯��,ɾ������ָ����Ŀ����ʷ�澯
//�������  ��
//�������  ��
//����ֵ    ����0��ʾɾ���ɹ�,��0��ʾɾ��ʧ��
/////////////////////////////////////////////////////////
int CHistoryAlarm::Sweep()
{
    //���Ƿ�澯��ﵽ������
    if(m_nRecordNum > m_nAlarmUpperLimit)
    //ɾ���澯��¼
    {
        DeleteRecords(m_nAlarmDelNum);
    }
    return 0;
}

//by ldf 2004-01-31 OM����
//////////////////////////////////////////////////////////////////////
//��������    ���ݸ澯ͬ���Ų��Ҹ澯��¼
//�������    uSyn - Ԥ׷�ӵ���ʷ�澯��β���ĸ澯��¼��
//�������    ��
//����ֵ    �ø澯�ڸ澯�ļ��е�λ��,�Ը澯��¼Ϊ��λ,Ϊ-1ʱ��ʾû���ҵ�
////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::FindRecordBySyn(UINT4 uSyn)
{

    //���ļ��ж�ȡ���и澯��¼��Ϣ,�����ȡ�����򷵻�-1;
    //���ڲ����Ǹ澯���кŻ�ת�����,��ʷ�澯���и澯��¼����ˮ���ǵ������е�,
    //���Բ��ö��ַ������㷨�ҵ���ˮ��ΪuSyn����ʷ�澯��¼

    int nLeft   = 0;
    int nRight  = m_nRecordNum - 1;
    int nMid    = (nRight-nLeft) / 2;
    int nRetPos = -1;
    UINT4 nLeftSyn,nMidSyn,nRightSyn;
    SHistoryAlarm hisAlarm;

    while(1)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nLeft, hisAlarm))
        {
            break;
        }
        nLeftSyn = hisAlarm.uSyn;

        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nMid, hisAlarm))
        {
            break;
        }
        nMidSyn = hisAlarm.uSyn;

        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nRight, hisAlarm))
        {
            break;
        }
        nRightSyn = hisAlarm.uSyn;

        if(nLeft == nMid)
        {
            if(uSyn == nLeftSyn)
                nRetPos = nLeft;
            else
                if(uSyn == nRightSyn)
                    nRetPos = nRight;
            break;
        }
        if(uSyn < nLeftSyn || uSyn > nRightSyn)
            break;
        if(uSyn == nMidSyn)
        {
            nRetPos = nMid;
            break;
        }
        else
        {
            if(uSyn < nMidSyn)
            {
                nRight = nMid;
            }
            else
            {
                nLeft = nMid;
            }
            //by lx 2003-09-28 SWPD01792
            nMid = nLeft + (nRight-nLeft)/2;
        }
    }

    return nRetPos;
}



