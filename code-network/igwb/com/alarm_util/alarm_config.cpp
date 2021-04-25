// AlarmConfig.cpp: implementation of the CAlarmConfig class.
//
//////////////////////////////////////////////////////////////////////
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "alarm_config.h"
#include "alarm_struct.h"
#include "../alarm/bin_alarm_handler.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//��������  ����CAlarmConfig����
//          szCfgFile����Ϊһ��Ч·����
//          CAlarmConfig����������Ч���ж�
//�������  szCfgFile���澯���������ļ���ȫ·����
//////////////////////////////////////////////////////////////////////
CAlarmConfig::CAlarmConfig(const char* szCfgFile)
{
    sprintf(m_szCfgFile,"%s",szCfgFile);
}

//////////////////////////////////////////////////////////////////////
//��������  ����CAlarmConfig����
//�������  szCfgFile - �澯���������ļ���ȫ·����
//////////////////////////////////////////////////////////////////////
CAlarmConfig::~CAlarmConfig()
{
    ALARM_EXPLAIN_MAP::iterator i;

    for(i = m_AlarmExplainMap.begin();i != m_AlarmExplainMap.end(); i++)
    {
        delete (*i).second;
    }
    m_AlarmExplainMap.clear();

}

///////////////////////////////////////////////////////////////////////
//��������  ����澯��Ϣ�����ļ�
//�������  ��
//�������  ��
//����ֵ    ����ERR_SUCCESS(0)��ʾ�ɹ�;����(��0)��ʾʧ��,����ֵ���������
///////////////////////////////////////////////////////////////////////////
int CAlarmConfig::LoadConfig()
{
    //��ֻ����ʽ��m_szCfgFile�����ļ���ʧ�ܣ�����ERR_OPEN_ALM_CFG_FILE��
    FILE *pFile = fopen(m_szCfgFile,"rb");
    if(pFile == NULL)
    {
        return ERR_OPEN_ALM_CFG_FILE;
    }

    //���澯��Ϣ�����ļ������и澯���ö�������ӵ�m_AlarmExplainMap�У�
    SAlarmExplain *pAlarmExplain;

    pAlarmExplain = new SAlarmExplain;
    if(pAlarmExplain == NULL)
    {
        return ERR_FAIL;
    }

    int nread;
    UINT4 nVersion;
    nread = fread(&nVersion, 1, 4, pFile);
    nVersion = ntohl(nVersion);    
    while(feof(pFile) == 0)
    {
        nread = fread(pAlarmExplain,sizeof(SAlarmExplain),1,pFile);
        if(nread != 1)
        {
            if(feof(pFile)!=0)
            {
                break;
            }
        }
        //������ 2002-06-07 D14379
        pAlarmExplain->uAlarmID = ntohs(pAlarmExplain->uAlarmID);
        //over
        if (m_AlarmExplainMap.find(pAlarmExplain->uAlarmID) == m_AlarmExplainMap.end())
        {
            m_AlarmExplainMap[pAlarmExplain->uAlarmID] = pAlarmExplain;
            pAlarmExplain = new SAlarmExplain;
        }
    }

    //�ر�m_szCfgFile�ļ�������ERR_SUCCESS
    delete pAlarmExplain;
    fclose(pFile);
    return ERR_SUCCESS;

}

///////////////////////////////////////////////////////////////////////////////////
//��������  ����uAlarmID������Ӧ�ĸ澯������Ϣ
//�������  �澯ID
//�������  ��
//����ֵ    ��ϸ�澯���ͽṹָ��
///////////////////////////////////////////////////////////////////////////////////
PAlarmExplain CAlarmConfig::GetAlarmExplain(UINT2 uAlarmID)
{
    //������ʱ����
    ALARM_EXPLAIN_MAP::iterator  it;

    // ��uAlarmIDΪKeyֵ����m_AlarmExplainMap,����ҵ�,�򷵻ظ澯uAlarmID����ϸ����
    //���򷵻�NULL
    it = m_AlarmExplainMap.find(uAlarmID);

    if(it == m_AlarmExplainMap.end())
    {
        return NULL;
    }
    else
    {
        return (*it).second;
    }
}

void CAlarmConfig::GetAlarmExplainList(LIST<SAlarmExplain>& AlarmExplainList)
{
    ALARM_EXPLAIN_MAP::iterator  it;
    
    SAlarmExplain* pAlarmExplain = new SAlarmExplain;
    for(it = m_AlarmExplainMap.begin(); it != m_AlarmExplainMap.end(); it++)
    {
        memcpy(pAlarmExplain, it->second, sizeof(SAlarmExplain));
        AlarmExplainList.push_back(*pAlarmExplain);
        pAlarmExplain = new SAlarmExplain;
    }
    delete pAlarmExplain;
    
    return ;
}

////////////////////////////////////////////////////////////////////////////
//��������  ����uAlarmID�������Ӧ�ĸ澯����
//�������  �澯ID
//�������  ��
//����ֵ    �ø澯��Ӧ�ĸ澯����
////////////////////////////////////////////////////////////////////////////
BYTE CAlarmConfig::GetAlarmType(UINT2 uAlarmID)
{
    SAlarmExplain *pExplain;
    pExplain = (SAlarmExplain *)GetAlarmExplain(uAlarmID);

    if(pExplain != NULL)
    {
        return pExplain->yAlarmType;
    }
    else
    {
        return AT_NOT_DEFINE;
    }
}
////////////////////////////////////////////////////////////////////////////
//by lx SWPD04479
void CAlarmConfig::AlarmVisit(CAlarmCfgVisitor visitor)
{
    ALARM_EXPLAIN_MAP::iterator  it;
    
    for(it = m_AlarmExplainMap.begin(); it != m_AlarmExplainMap.end(); it++)
    {
        visitor.operator() (it->second);
    }
    return;
}
//end by lx SWPD04479