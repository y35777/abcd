/***************************************************************************
*��Ʒ��        ��iGateway Bill V200
*ģ����        ��agent
*�ļ���        ��cluster_alarm.cpp
*������Ϣ      ����CClusterAlarm��ʵ�֣������װ��SUN SC(SUN CLUSTER)���ϼ�
���Լ��澯����
*�汾˵��      ��V200R002i00M00B00D1130A
*��Ȩ��Ϣ      ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
*����          ������ƽ
*����ʱ��      ��2001-10-30
*�޸ļ�¼      ��
*****************************************************************************/

#include "cluster_alarm.h"
#include "cluster_agent.h"
#include "../include/frame.h"
#ifdef _PLATFORM_SOLARIS 
#include "../include/system1.h"
#endif

/********************************************************************
����ԭ��    public: CClusterAlarm(CCluster *ptheCluster);
��������    ���캯��
�������    ptheCluster��CCluster����ָ�룬CClusterAlarmͨ����ָ��
����ȡSC�ľ�̬�Ͷ�̬��Ϣ
�������    ��
����ֵ          ��
�׳��쳣    ��
*********************************************************************/
CClusterAlarm:: CClusterAlarm(CCluster* ptheCluster)
{
    //�ڵ��������£����캯���д����ptheClusterָ�����Ϊ��
    m_ptheCluster   = ptheCluster;
    m_bAlarmRestore = FALSE;
    
    char szFile[MAX_PATH]; 
    
    //�������ļ�
    CINIFile* pINIFile = NULL;
    pINIFile = new CINIFile((char *)::GetCfgFilePath());
    pINIFile->Open();
    
    //��������Դ����(FTP����FTAM)
    char szResType[64];
    
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_SERVICE_RES, 
        DEFAULT_SERVICE_RES, szResType, 64);
    StringToUpper(szResType);
    m_sResourceType = szResType;

    if (m_ptheCluster == NULL) //�����������������ļ��ж� 
    {
        m_uVMType = (VM_TYPE)pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
            CFG_VM_TYPE, VT_VXVM);
    } 
    else  //˫������
    {
        m_uVMType = m_ptheCluster->GetVMType();
    }
    

    
    //��ǰ��������ڵĴ��������;���
    char szOutput[MAX_PATH];
    
    //��ǰ�������ڵ��߼�����
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_FRONT_VOL, 
        DEFAULT_FRONT_VOL, szOutput, 254);
    m_sFrontVolume = szOutput;
    
    
    //��ǰ�������ڵĴ�������
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_FRONT_DG, 
        DEFAULT_FRONT_DG, szOutput, 254);
    m_sFrontDG = szOutput;
    
    //����������ڵ��߼����� 
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_BACK_VOL, 
        DEFAULT_BACK_VOL, szOutput, 254);
    m_sBackVolume = szOutput;
    
    //����������ڵĴ�������
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_BACK_DG, 
        DEFAULT_BACK_DG, szOutput, 254);
    m_sBackDG = szOutput;
    
    //�����澯�б�
    
    m_pAlarmList = new LIST<SInnerAlarm*>;
    
    //�������澯����澯����б�
    BYTE yAlarmPara[MAX_ALARM_PARA];
	memset(yAlarmPara, 0xFF, MAX_ALARM_PARA);

    GetAlarmItem(ALARM_ID_HEARTBEAT, yAlarmPara);
    
    //�������澯����澯����б�
    GetAlarmItem(ALARM_ID_FAILOVER, yAlarmPara);
    
    //��˫�������½�˽���澯����澯����б�
    if (m_ptheCluster)
    {
        int nPrvNetNum = m_ptheCluster->GetNumOfPrvNet();
        for (int i = 0; i < nPrvNetNum; i++)
        {
			yAlarmPara[0] = i;
            GetAlarmItem(ALARM_ID_PRVNET, yAlarmPara);
        }
    }
    
    //������ʧЧ�澯����澯����б� 
    if (m_sResourceType.find("FTP"))
        yAlarmPara[0] = (BYTE)0;                //FTP����
    else
        yAlarmPara[0] = (BYTE)1;                //FTAM����
    GetAlarmItem(ALARM_ID_SVCRES, yAlarmPara);
    
    //��ǰ���̾�ʧЧ�澯����澯����б�
	yAlarmPara[0] = FRONT_VOLUME;
    GetAlarmItem(ALARM_ID_VOLUME, yAlarmPara);
    
    //������̾�ʧЧ�澯����澯����б�
	yAlarmPara[0] = BACK_VOLUME;
    GetAlarmItem(ALARM_ID_VOLUME, yAlarmPara);
    
    //��Ӳ�̹��ϸ澯����澯����б� 
	yAlarmPara[0] = ALARM_PARA_NONE;
    GetAlarmItem(ALARM_ID_DISK, yAlarmPara);
    
    //��IP��ԴʧЧ�澯����澯����б�
	yAlarmPara[0] = ALARM_PARA_NONE;
    GetAlarmItem(ALARM_ID_IPRES, yAlarmPara);
    
    delete pINIFile;
}

/********************************************************************
����ԭ��    public: ~CClusterAgent(void);
��������    CClusterAgent�����������
�������    ��
�������    ��
����ֵ          ��
�׳��쳣    ��
*********************************************************************/
CClusterAlarm:: ~CClusterAlarm(void)
{
    LIST<SInnerAlarm*>::iterator it;
    
    if (m_pAlarmList)
    {
        //����ɾ���澯�б��еĸ澯Ԫ��
        for (it = m_pAlarmList->begin(); it != m_pAlarmList->end();
        it++)
        {
            delete (*it);
        }
        
        m_pAlarmList->clear();
        delete m_pAlarmList;
    }
}

/********************************************************************
����ԭ��    private: PInnerAlarm GetAlarmItem(UINT2 uAlarmID, 
BYTE yAlarmPara);
��������    ���ݸ澯�š��澯�����ڸ澯�б��в���ָ���ĸ澯����
û���򴴽�ָ���澯�����澯�б�
�������    uAlarmID  ���澯�� 
yAlarmPara���澯������0xFF��ʾ�޸澯������
�������    ��
����ֵ          ָ��ָ���澯���ָ��
�׳��쳣    ��
*********************************************************************/
PInnerAlarm CClusterAlarm:: GetAlarmItem(UINT2 uAlarmID, BYTE* yAlarmPara)
{
    LIST<SInnerAlarm*>::iterator it;
    SInnerAlarm* pAlarm = NULL;
    
    //�����澯�б�����ID�Ͳ�����ƥ��ĸ澯��
    for (it = m_pAlarmList->begin(); it != m_pAlarmList->end();
    it++)
    {
        pAlarm = (*it);
        //�澯ID���澯����ȫƥ��
        if ((pAlarm->uAlarmID == uAlarmID) && 
             memcmp(pAlarm->yAlarmPara, yAlarmPara, MAX_ALARM_PARA) == 0 )

        {
            return pAlarm;
        } 
    }
    
    //���û�в��ҵ����򴴽�һ����Ӧ�ĸ澯�� 
    pAlarm = new SInnerAlarm;
    pAlarm->uAlarmID   = uAlarmID;
    memcpy(pAlarm->yAlarmPara, yAlarmPara, MAX_ALARM_PARA);
    pAlarm->yAlarmType = AT_NOT_DEFINE;
    
    m_pAlarmList->push_back(pAlarm);
    
    return pAlarm;
}

/*********************************************************************
����ԭ��    private: void SetAlarmItem(PInnerAlarm pAlarmItem, 
BOOL bStatus);
��������    ����ָ���澯��ĸ澯���͡�
�������    bStatus���澯��״̬��FALSE��ʾ�����˸澯��TRUE��ʾ�澯
�ָ���
�������    ��
����ֵ  ��
�׳��쳣    ��
**********************************************************************/
void CClusterAlarm:: SetAlarmItem(PInnerAlarm pAlarmItem,
                                  BOOL bStatus)
{
    if (bStatus == FALSE)
    {
        //�������ϸ澯
        pAlarmItem->yAlarmType = AT_FAULT;
    }
    else
    {
        //�����ָ��澯
        switch(pAlarmItem->yAlarmType)
        {
        case AT_FAULT:
            pAlarmItem->yAlarmType = AT_RESTORE;
            break;
            
        case AT_RESTORE:
            pAlarmItem->yAlarmType = AT_NOT_DEFINE;
            break;
        }
    }
}

/*********************************************************************
����ԭ��    public: void AlarmPrvNet(void);
��������    ���ÿ��˽����״̬����������Ӧ���ϸ澯��ָ��澯�����
ȫ��˽���жϣ����������ȫ���жϸ澯���ָ��澯��
�������    ��
�������    ��
����ֵ          ��
�׳��쳣    ��
**********************************************************************/
void CClusterAlarm:: AlarmPrvNet(void)
{
    int nrOfAvailPrvNet = 0;    //��¼����˽������
    SInnerAlarm* pAlarm;
    
    //���˫�������±��ڵ��ϼ�Ⱥϵͳ�Ƿ�������
    //by ldf 2002.4.22 ��Ӧ���ⵥ D13065
    if ( (m_ptheCluster == NULL) || (!m_ptheCluster->IsClusterRunning()) )
    //�޸����
    {
        return;
    }
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, 0xff, MAX_ALARM_PARA);

    //��ü�Ⱥ��ǰ��Ա�������ֻ��һ���ڵ㼤���nrOfAvailPrvNet=0��
    //��Ӧ�����������жϸ澯
    if (m_ptheCluster->GetCurrMemNum() >= 2)
    {
    	

        int nNumOfPrvNet = m_ptheCluster->GetNumOfPrvNet();
        
        //���ÿ��˽����״̬
        for (int i = 0; i < nNumOfPrvNet; i++)
        {
            yAlarmPara[0] = (BYTE)i;
        	pAlarm = GetAlarmItem(ALARM_ID_PRVNET, yAlarmPara);
            
            //���˽��i�ĵ�ǰ״̬
            if (m_ptheCluster->GetPrvNetStatus(i))
            {
                SetAlarmItem(pAlarm, TRUE);
                nrOfAvailPrvNet++;
            }
            else //˽������
            {
                SetAlarmItem(pAlarm, FALSE);
            } 
        }
    }
    
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
    pAlarm = GetAlarmItem(ALARM_ID_HEARTBEAT, yAlarmPara);  //�����ж�

    if (nrOfAvailPrvNet == 0)
    {
        SetAlarmItem(pAlarm, FALSE);   //���������жϸ澯
    }
    else
    {
        SetAlarmItem(pAlarm, TRUE);    //��������
    }
}

/*********************************************************************
����ԭ��    public: void AlarmSwitch(void);
��������    ����˫�������澯(ע���ڵ��������²����ô˺���)��
˫�������ڼ��˫���Ƿ񵹻�ʱ�������·�������SC�Ĺ�����
��ϵͳ��Ŀ¼�´���һ��master�ļ����ļ��б��漤���SC��
�������κ�һ�˵�����������Լ������������ԭ�ļ��е�
���ݣ���������һ̨��������ʱ���������mater�ļ��е�����
������������������������жϷ����˵�����
�������    ��
�������    ��
����ֵ          ��
�׳��쳣    ��
**********************************************************************/
void CClusterAlarm:: AlarmSwitch(void)
{
    //�ڵ��������²����������
    if (m_ptheCluster == NULL)   return;
    
    char szMaster[MAX_PATH];
    SNPRINTF(szMaster, MAX_PATH, "/var/other/master");
    szMaster[254] = '\0';
    
    const STRING sLocalNodename = m_ptheCluster->GetLocalNodename();   //��ñ���������
    
    //��master�ļ�
    FILE* pMaster = fopen(szMaster, "r");
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
    
    if (pMaster != NULL)
    {
        char szLine[1024];
        fgets(szLine, 1023, pMaster);
        fclose(pMaster);   //�ر��ļ�
        
        StringLRTrim(szLine);
        //�Ƚ�master�ļ��еĽڵ����Ƿ���ڱ��ڵ���
        if (strcmp(sLocalNodename.c_str(), szLine) == 0)
        {
            return;    //��ȣ�û�е���
        }
        else           //����ȣ����͵����澯
        {
            SInnerAlarm* pAlarm;
            
            //��õ����澯
            pAlarm = GetAlarmItem(ALARM_ID_FAILOVER, yAlarmPara);
            SetAlarmItem(pAlarm, FALSE);

            //by ldf 2003-08-07,��Ӧ���ⵥSWPD00823
            CMsgProcessor::SendAlarm(pAlarm);
            //end			
        }
    }
    
    //��master�ļ���ɾ���ļ��е�����
    pMaster = fopen(szMaster, "wb");
    if ( NULL == pMaster )
    {
        return;
    }

    //�����ڵ���д��
    fwrite(sLocalNodename.c_str(), sLocalNodename.length(), 1, pMaster);
    fclose(pMaster);
}

/**********************************************************************
����ԭ��    public:  void AlarmSwitchRestore(void);
��������    ����˫�������ָ��澯��
�������    ��
�������    ��
����ֵ          ��
�׳��쳣    ��
***********************************************************************/
void CClusterAlarm:: AlarmSwitchRestore(void)
{
    if (m_bAlarmRestore)   // �����Ѿ��ָ�
    {
        return;
    }
    
    //��õ�ǰ��Ⱥ��Ա��
    if (m_ptheCluster->GetCurrMemNum() < 2)
    {
        return;    //ֻ��һ̨������
    }
    
    m_bAlarmRestore = TRUE;
    SInnerAlarm* pAlarm;
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);

    
    //���͵����ָ��澯
    pAlarm = GetAlarmItem(ALARM_ID_FAILOVER, yAlarmPara);
    SetAlarmItem(pAlarm, TRUE);
}

/***********************************************************************
����ԭ��    public:  void AlarmVolume(UINT2 uFlag);
��������    ���ǰ������߼���״̬����������Ӧ�Ĺ��ϸ澯�ͻָ��澯��
�������    uFlag��ָ��ǰ���̻������߼�������ȡ����ֵ�� 
            #define FRONT_VOLUME  0 
            #define BACK_VOLUME   1
�������    ��
����ֵ      ��
�׳��쳣    ��
************************************************************************/
void CClusterAlarm:: AlarmVolume(UINT2 uFlag)
{
    PInnerAlarm pAlarm = NULL;
    BOOL bStatus = TRUE;       //��¼�߼���ĵ�ǰ״̬
    
    //���˫�������±��ڵ��ϼ�Ⱥϵͳ�Ƿ�������
    //by ldf 2002.4.22 ��Ӧ���ⵥ D13065
    if ( (m_ptheCluster == NULL) || (!m_ptheCluster->IsClusterRunning()) )
    //�޸����
    {
        return;
    }    
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, 0xff, MAX_ALARM_PARA);

    STRING sDiskGroup;
    STRING sVolume;
    
    switch (uFlag)
    {
        //���ǰ�������ڵ��߼���
    case FRONT_VOLUME:
        yAlarmPara[0] = (BYTE)1;
        sDiskGroup = m_sFrontDG;
        sVolume    = m_sFrontVolume;
        break;
        
        //����������ڵ��߼���
    case BACK_VOLUME:
        yAlarmPara[0] = (BYTE)1;
        sDiskGroup = m_sBackDG;
        sVolume    = m_sBackVolume;
        break;
        
    default:
        return;
    }

    
    char szCmdLine[MAX_PATH];
    FILE* fp = NULL;
    char szLine[1024];
    STRING sLine;
    
    switch (m_uVMType)
    {
    case VT_SDS:
        sprintf(szCmdLine, "metastat %s/%s > agent.tmp", 
            sDiskGroup.c_str(), sVolume.c_str());
        
#ifdef _PLATFORM_SOLARIS 
        system1(szCmdLine);     //����system()�����������
#else
        system(szCmdLine);     //����system()�����������
#endif
        
        if ((fp = fopen("agent.tmp", "r")) == NULL)
        {
            return; 
        }
        
        //��������߼����״̬�Ƿ�Ϊ"Okay"
        while (!feof(fp))
        {
            if(fgets(szLine, 1023, fp) != NULL)
            {
                sLine = szLine;
                if ((sLine.find("State:") != -1) 
                    && (sLine.find("Okay") == -1))
                {
                    bStatus = FALSE;
                    fclose(fp);
                    break;
                }
            }
            strcpy (szLine,"");
        }
        
        fclose(fp); 
        break;
        
    case VT_VXVM:
        sprintf(szCmdLine, "vxinfo -g %s %s > vxinfo.tmp", 
            sDiskGroup.c_str(), sVolume.c_str());
#ifdef _PLATFORM_SOLARIS 
        system1(szCmdLine);     //����system()�����������
#else
        system(szCmdLine);     //����system()�����������
#endif
        
        if ((fp = fopen("vxinfo.tmp", "r")) == NULL)
        {
            return;
        }
        
        //��������߼����Ƿ�"Started"
        while (!feof(fp))
        {
            if(fgets(szLine, 1023, fp) !=NULL)
            {
                sLine = szLine;
                
                //��ŷ� ���������޸ģ���Ϊ������agent.tmp�ļ�û��State:����������ֻ��һ��
                //�磺front fsgen Started
                if (sLine.find("Started") == -1)
                {
                    bStatus = FALSE;
                    fclose(fp);
                    break;
                }
            }
            strcpy(szLine,"");
        }
        
        fclose(fp);
        remove("vxinfo.tmp");
        break;
        
    default:
        return;
    }
    
    //���͸澯���߻ָ��澯
    pAlarm = GetAlarmItem(ALARM_ID_VOLUME, yAlarmPara);
    SetAlarmItem(pAlarm, bStatus);
}

/***********************************************************************
����ԭ��    public:  void AlarmResource(void);
��������    ���FTP��FTAM��״̬����������Ӧ�Ĺ��ϸ澯�ͻָ��澯
�������    ��
�������    ��
����ֵ          ��
�׳��쳣    ��
************************************************************************/
void CClusterAlarm:: AlarmResource(void)
{
}

/***********************************************************************
����ԭ��    public:  void AlarmIPs(void);
��������    ������ж�̬IP��ַ��Դ��״̬�����������ϸ澯�ͻָ��澯
����SC�ṩ������"pnmstat -l"���Լ��SC˫������IP��״̬
����ִ��"pnmstat -l"���������
bkggrp  r_adp     status  fo_time live_adp
nafo0   hme0:hme3   OK      NEVER   hme0
����Ϣ��������IP nafo0��ǰ����"OK"״̬��

  �������  ��
  �������  ��
  ����ֵ            ��
  �׳��쳣  ��
***********************************************************************/
void CClusterAlarm:: AlarmIPs(void)
{
    PInnerAlarm  pAlarm = NULL;
    
    //���˫�������±��ڵ��ϼ�Ⱥϵͳ�Ƿ�������
    //by ldf 2002.4.22 ��Ӧ���ⵥ D13065
    if ( (m_ptheCluster == NULL) || (!m_ptheCluster->IsClusterRunning()) )
    //�޸����
    {
        return;
    }

    //*sc3.0* �˴���GetNAFO�����ؽ������Ϊqfe0��qfe1��qfe2��qfe3��Ȼ��������Щ�����жϣ������� nafo��nafo1...
    //*sc3.0* �����NAFO��ʵ�����˼����ã�SC3.0���ǿ���ֱ��ʹ�÷���ֵ��qfe0,qfe1,qfe2,qfe3...,
    //*sc3.0* ����˳��һ��������Ҳ������������

    LIST<STRING> pNAFO = m_ptheCluster->GetNAFO();
    
    //ע�������������е�"-l"����ĸl��������1��
    char szCmdLine[MAX_PATH] = "pnmstat -l > pnmstat.tmp";
    
#ifdef _PLATFORM_SOLARIS 
    system1(szCmdLine);     //����system()�����������
#else
    system(szCmdLine);     //����system()�����������
#endif
    FILE* fp = NULL;
    
    if ((fp = fopen("pnmstat.tmp", "r")) == NULL)
    {
        return;
    } 
    
    int i = 0;
    LIST<STRING>::iterator it;
    char szLine[1024];
    STRING sLine;
    BOOL bStatus = TRUE;
    char szchar = '0' ;
    STRING sNAFO;
    char szbuf[10];
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
    
    for (it = pNAFO.begin(); it != pNAFO.end(); it++)
    {
       //root@igwb2 # pnmstat -l
       //group   adapters        status  fo_time act_adp
       //nafo0   qfe0            OK      NEVER   qfe0
       //nafo1   qfe3            DOWN    NEVER   qfe3
       //nafo2   qfe1            OK      NEVER   qfe1
       //nafo3   qfe2            OK      NEVER   qfe2

        //�����ɵ��ļ�agent.tmp��Ϊnafe0,nafe1,nafe2,nafe3...

        sprintf(szbuf, "nafo%c",szchar);
        sNAFO = szbuf;
        rewind(fp);   //��ת�ļ�ָ��
        yAlarmPara[0] = (BYTE)i;
        pAlarm = GetAlarmItem(ALARM_ID_IPRES, yAlarmPara);
        
        while (!feof(fp))
        {
            if(fgets(szLine, 1023, fp) !=NULL)
            {
                sLine = szLine;
                //�������IP��״̬
                if ((sLine.find(sNAFO) != -1)
                    && (sLine.find("OK")) == -1)
                {
                    bStatus = FALSE;
                    break;
                }
            }
            strcpy (szLine,"");
        }
        
        szchar++;
        SetAlarmItem(pAlarm, bStatus);
        bStatus = TRUE;  //�����һ��
        i++;             //IP��Դ����
    } 
    fclose(fp);
    remove("pnmstat.tmp");
}

/************************************************************************
����ԭ��    public:  void AlarmDisks(void);
��������    ��鹲�����״̬����������Ӧ���ϸ澯�ͻָ��澯
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*************************************************************************/
void CClusterAlarm:: AlarmDisks(void)
{
    PInnerAlarm pAlarm = NULL;
    BOOL bStatus = TRUE;   //��¼Ӳ��״̬��TRUE��ʾӲ���޹���
    
    //by ldf 2002.4.22 ��Ӧ���ⵥ D13065
    if ( (m_ptheCluster == NULL) || (!m_ptheCluster->IsClusterRunning()) )
    //�޸����
    {
        return;
    }
    
    LIST<STRING> DGList;
    LIST<STRING>::iterator it;
    
    if (m_ptheCluster == NULL) 
    {
        DGList.push_back(m_sFrontDG);
        DGList.push_back(m_sBackDG);
    }
    else
    {
        LIST<STRING> pDG = m_ptheCluster->GetDiskGroup();
        
        for (it = pDG.begin(); it != pDG.end(); it++)
        {
            DGList.push_back(*it);
        } 
    }
    
    FILE* fp = NULL;
    char szCmdLine[MAX_PATH];
    char szLine[1024];
    STRING sLine;
    
    
    LIST<STRING>::iterator itCheck;
    LIST<STRING> DGListCheck;
    char temp[100];
    char buf[100];
    STRING sLineCheck;
    
    
    switch (m_uVMType)
    {
    case VT_SDS:
        for (it = DGList.begin(); it != DGList.end(); it++)
        {
            sprintf(szCmdLine, "metastat -s %s > agent.tmp", (*it).c_str());
#ifdef _PLATFORM_SOLARIS 
            system1(szCmdLine);     //����system()�����������
#else
            system(szCmdLine);     //����system()�����������
#endif
            
            fp = fopen("agent.tmp", "r");
            if (fp == NULL)
            {
                return;
            }
            
            while (!feof(fp))
            {
                if(fgets(szLine, 1023, fp) != NULL)
                {
                    sLine = szLine;
                    
                    //��������״̬����Okay�������Ӳ�̳�������
                    if ((sLine.find("State:") != -1)
                        && (sLine.find("Okay") == -1))
                    {
                        bStatus = FALSE;
                        
                        fclose(fp);
                        fp = NULL;
                        
                        remove("agent.tmp");
                        
                        break;
                    }
                }
                strcpy (szLine,"");
            }            
            
            if (bStatus == FALSE)
            {
                break;
            }
            
            if (fp != NULL)
            {
                fclose(fp);
                fp = NULL;
                
                remove("agent.tmp");
            }
        } 
        
        break;
        
    case VT_VXVM:
        //��ŷ� �������ʵ������������޸�
        for (it = DGList.begin(); it != DGList.end(); it++)
        {
            sprintf(szCmdLine, "vxdisk -g %s list > vxdisk.tmp", (*it).c_str());
#ifdef _PLATFORM_SOLARIS 
            system1(szCmdLine);     //����system()�����������
#else
            system(szCmdLine);     //����system()�����������
#endif
            
            fp = fopen("vxdisk.tmp", "r");
            if (fp == NULL)
            {
                return;
            }
            //�Թ���һ��DEVICE       TYPE      DISK         GROUP        STATUS
            fgets(szLine, 1023, fp);
            
            //�Ѹ���DISK����DGListCheck�б��У�DISK�ڵ�����λ��(�� igwbdg01)��
            //�еĸ�ʽ�磺c1t0d0s2     sliced    igwbdg01     igwbdg       online
            while (!feof(fp))
            {
                if(fgets(szLine, 1023, fp) != NULL)
                {
                    sscanf(szLine,"%s%s%s",temp,temp,buf);
                    sLineCheck = buf;
                    DGListCheck.push_back(sLineCheck);
                }
            }
            
            
            if (fp != NULL)
            {
                fclose(fp);
                fp = NULL;
                
                remove("vxdisk.tmp");
            }
            
            
            //�жϴ��̵�״̬�������Okay�������������
            for (itCheck = DGListCheck.begin(); itCheck != DGListCheck.end(); itCheck++)
            {
                sprintf(szCmdLine, "vxdisk check %s > vxdisk.tmp", (*itCheck).c_str());
#ifdef _PLATFORM_SOLARIS 
                system1(szCmdLine);     //����system()�����������
#else
                system(szCmdLine);     //����system()�����������
#endif
                
                fp = fopen("vxdisk.tmp", "r");
                if (fp == NULL)
                {
                    return;
                }
                
                
                while (!feof(fp))
                {
                    if(fgets(szLine, 1023, fp) !=NULL)
                    {
                        sLine = szLine;
                        
                        //��������״̬����Okay�������Ӳ�̳�������
                        if (sLine.find("Okay") == -1)
                        {
                            bStatus = FALSE;
                            
                            fclose(fp);
                            fp = NULL;
                            
                            remove("vxdisk.tmp");
                            
                            break;
                        }
                    }
                }            
                
                if (bStatus == FALSE)
                {
                    break;
                }
                
                if (fp != NULL)
                {
                    fclose(fp);
                    fp = NULL;
                    
                    remove("vxdisk.tmp");
                }
            } 
            if (bStatus == FALSE)
            {
                break;
            }
            
        }   
        break; 
        
        
    default:
        break;
    }    
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);    
    pAlarm = GetAlarmItem(ALARM_ID_DISK, yAlarmPara);

    SetAlarmItem(pAlarm, bStatus); 
}

/************************************************************************
����ԭ��    public:  void Alarm(void);
��������    �ڵ���˫����ͬ�����·ֱ������Ӧ�����������ϸ澯�ͻָ��澯
�������    ��
�������    ��
����ֵ          ��
�׳��쳣    ��
*************************************************************************/
void CClusterAlarm:: Alarm(void)
{
    //���¸澯ֻ��˫�������¼��
    if (m_ptheCluster)
    {
        AlarmPrvNet();              //˽���жϸ澯
        AlarmSwitchRestore();       //�����ָ��澯
        AlarmIPs();                 //IP��ԴʧЧ�澯
    }
    
    AlarmResource();                //��ԴʧЧ�澯
    AlarmVolume(FRONT_VOLUME);      //��ʧЧ�澯(ǰ�������ڵ��߼���)
    AlarmVolume(BACK_VOLUME);       //��ʧЧ�澯(��������ڵ��߼���)
    AlarmDisks();                   //Ӳ�̹��ϸ澯
    
    //���壺
    LIST<SInnerAlarm*>::iterator it;
    SInnerAlarm* pAlarm = NULL;
    
    //�����澯����͸澯/�ָ��澯
    for (it = m_pAlarmList->begin(); it != m_pAlarmList->end();
    it++)
    {
        pAlarm = (*it);
        if (pAlarm->yAlarmType != AT_NOT_DEFINE)
        {
            CMsgProcessor::SendAlarm(pAlarm);
        }
    } 
}
