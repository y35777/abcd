// AlarmMMLReport.cpp: implementation of the CAlarmMMLReport class.
//
//////////////////////////////////////////////////////////////////////

#include "alarm_mml_report.h"

#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlarmMMLReport::CAlarmMMLReport(const char* szName):CMMLReport(szName)
{
    //by ldf 2004-01-12 OM���ģ���ʼ���澯ͬ����Ϊ100
    m_uMMLBaseAlarmID = 100;
}   

CAlarmMMLReport::~CAlarmMMLReport()
{
}
////////////////////////////////////////////////////////
//��������    ����澯����Ƭ��ͷ
//        ��ʽΪ:
//        <�澯��־ ALARM><2SP><�澯��ˮ��'L10><2SP><�澯���'L8>
//        <2SP><�澯����'L12><4SP><�澯��Դ'L8><2SP><�澯���'L5><4SP>
//        <�澯���ܷ���'L16>\r\n
//�������    SAlarmDetail *pAlarm
//��ŷ� 2002-9-17
////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeAlarmHead(SAlarmDetail *pAlarm)
{
    if(pAlarm == NULL)
    {
        return;
    }
    //��Ӹ澯��־
    AddString("ALARM");
    FillSpace(2);

    //��Ӹ澯��ˮ��
    char tmp[16];
    sprintf(tmp,"%d",pAlarm->uCsn);
    AddString(tmp,10,ALIGN_LEFT);
    FillSpace(2);
    STRING szTmp = pAlarm->GetAlarmType();
    //��Ӹ澯���
    AddString(szTmp.c_str(), 8, ALIGN_LEFT);
    FillSpace(2);

    //��Ӹ澯����
    szTmp = pAlarm->GetAlarmLevel();
    AddString(szTmp.c_str(), 12, ALIGN_LEFT);
    FillSpace(4);

    //��Ӹ澯��Դ
    szTmp = pAlarm->GetAlarmSource();
    AddString(szTmp.c_str(), 8, ALIGN_LEFT);
    FillSpace(2);

    //��Ӹ澯���
    szTmp = pAlarm->GetAlarmID();
    AddString(szTmp.c_str(), 5, ALIGN_LEFT);
    FillSpace(4);

    //��Ӹ澯���ܷ���
    szTmp = pAlarm->GetAlarmSubSysID();
    AddString(szTmp.c_str(), 16, ALIGN_LEFT);
    //FillSpace(2);
}

///////////////////////////////////////////////////////////////
//��������    ����"�����ϱ��澯����". ����ĸ�ʽ����:
//<��ʼ����ʶ +++><4SP><Դ��ʶ��'L32><8SP><�澯��������'L10><1SP>
//<�澯����ʱ��'L8>\r\n
//ALARM  �澯��ˮ��   �澯���   �澯����   �澯��Դ  �澯��� �澯���ܷ���
//       ģ���    =  (ģ���)
//       �澯����  =  (�澯����)
//       ��λ��Ϣ  =  �澯���� ��������  �澯��ϸ���� �� ��ϸ����
//                      �澯������ϸ���� �� ��������ֵ
//<��������ʶ ---><4SP><END>
//�������
//��澯�����д洢�ĸ澯�ṹ -SActiveAlarm
//�澯������ָ��               -pConfig
//������2001-10-17
/////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeAlarmBroad(SActiveAlarm *pAlarm, CAlarmConfig *pConfig,int nRecoverType)
{
    //A ���챨��ͷ
    MakeReportHead2(pAlarm);
    //B ���CAlarmDetail�ṹָ��,����"�澯����Ƭ��ͷ"
    SAlarmDetail* pAlarmDetail = pAlarm->GetAlarmDetail();
    MakeAlarmHead(pAlarmDetail);
    //C ����<ģ���'L20A1><2SP><=><2SP><ģ���'L3A0>\r\n
    AddString(STR_NEWLINE);
	
    STRING szTmp;
#ifdef _OM_MODIFY
    //by ldf 2004-01-31 OM����
    UINT4 uSynInLog = pAlarmDetail->GetSYNNO();

    //�Ա��ĵ�ͬ���ţ�����M2000ͬ����׼ֵ����
    char tmp[8];
    sprintf(tmp,"%d",uSynInLog + (m_uMMLBaseAlarmID -100) );
    MakeReportItem(S_ALARM_SYN_NO, tmp, 20,ALIGN_RIGHT);
	
    szTmp = pAlarmDetail->GetAlarmTime();
    if(pAlarm->yAlarmType != AT_RESTORE)
    {
        MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
    }
    //end
#endif


    szTmp = pAlarmDetail->GetModule();
    MakeReportItem(S_ALARM_MODULENO, szTmp.c_str(),20,ALIGN_RIGHT);
    //D ����<�澯����'L20A1<2SP><=><2SP><�澯����'L55A0>\r\n
    szTmp = pAlarmDetail->GetAlarmName(pConfig);
    MakeReportItem(S_ALARM_ALARMNAME, szTmp.c_str(),20,ALIGN_RIGHT);
    //E ����<��λ��Ϣ'L20A1><2SP><=><2SP><�澯����=><����ֵ'M3A0><1SP>
    //                                     <�澯��ϸ����=><����ֵ'M6A0><1SP>
    //                                   <�澯������ϸ����=><�澯��������ֵ'M3>\r\n
    char szContent[1024];
    STRING szTmpPara = pAlarmDetail->GetAlarmParas();
    STRING szTmpExp = pAlarmDetail->GetExplain(pConfig);

    #ifdef _OM_MODIFY
        SNPRINTF(szContent,1024,"%s=%s",S_ALARM_ALARMPARAMETER , szTmpPara.c_str()); 
    #else 
        STRING szTmpParaExp = pAlarmDetail->GetParaExplain(pConfig);
        //Ϊ����ȥ���ո�
        SNPRINTF(szContent,1024,"%s=%s %s=%s %s=%s",
            S_ALARM_ALARMPARAMETER , szTmpPara.c_str() ,
            S_ALARM_ALARMEXPLAIN ,    szTmpExp.c_str() ,
            S_ALARM_ALARMPARAEXPLAIN, szTmpParaExp.c_str());
    #endif


    MakeReportItem(S_ALARM_ALARMPARA,szContent,20,ALIGN_RIGHT);

    #ifdef _OM_MODIFY
        //by ldf 2004-01-31 OM����
        if(pAlarm->yAlarmType == AT_RESTORE)
        {
            switch(nRecoverType)
            {
                //AUTO_RECOVER,		//1:�����ָ�
            case 1:
                MakeReportItem(S_ALARM_RECOVERSTATUS, S_ALARM_AUTO_RECOVER,20,ALIGN_RIGHT);
                break;
                //RESET_RECOVER,		//2:��λ�ָ�
            case 2:
                MakeReportItem(S_ALARM_RECOVERSTATUS, S_ALARM_RESET_RECOVER,20,ALIGN_RIGHT);
                break;
                //MANNUAL_RECOVER     //3:�ֹ��ָ�
            case 3:
                MakeReportItem(S_ALARM_RECOVERSTATUS, S_ALARM_MANNUAL_RECOVER,20,ALIGN_RIGHT);
                break;
            default:
                MakeReportItem(S_ALARM_RECOVERSTATUS, S_ALARM_AUTO_RECOVER,20,ALIGN_RIGHT);
            }

            //�õ��ָ��澯�Ļָ�ʱ�䣬��ʵ��ͷ��ʱ����ͬ��������Щ�ظ�
            char tmp[20];
            sprintf(tmp,"%04d-%02d-%02d %02d:%02d:%02d",pAlarm->uYear, pAlarm->yMonth,pAlarm->yDay,
                pAlarm->yHour,pAlarm->yMin,pAlarm->ySec);
            STRING szRecoverTime =tmp;		
            MakeReportItem(S_ALARM_ALARMRECOVERACKDATE, szRecoverTime.c_str(),20,ALIGN_RIGHT);
        }
        //end
    #endif


    //F ���챨��β
    MakeReportTail();
}
/////////////////////////////////////////////////////////////////////////
//��������    ����"�¼��澯ȷ���ϱ�����",
//��ʽ����:
//<�澯��־ ALARM     0          ȷ��     ��Ϣ      �澯��Դ>\r\n
//<ȷ�ϲ���Ա'L0A1<2SP><=><2SP><ȷ�ϲ���Ա'L0A0>\r\n
//<�澯����'L0A1<2SP><=><2SP><�澯����'L0A0>\r\n
//<�澯��ˮ��'L0A1<2SP><=><2SP><��ˮ��'L0A0>\r\n
//�������:
//szConfirmOpr -ȷ�ϲ���Ա
//szAlarmType  -�澯����
//szBeginCsn   -��ʼ��ˮ��
//szEndCsn     -������ˮ��
//������2001-10-17
////////////////////////////////////////////////////////////////////////////

//by ldf 2002.6.7 ��Ӧ���ⵥD14393
void CAlarmMMLReport::MakeAlarmEventAckBody(const char *szConfirmOpr, const char *szAlarmType, const char *szCsn)
//�޸����
{
    //A ����ALARM     0          ȷ��     ��Ϣ      �澯��Դ
    AddString(S_ALARM_ALARMACKHEAD);
    AddString(STR_NEWLINE); //���\r\n����
    //D ����<�澯��ˮ��'L0A1<2SP><=><2SP><��ˮ��'L0A0>\r\n
    MakeReportItem(S_ALARM_ALARMCSN, szCsn,20,ALIGN_RIGHT);
    //B ����<ȷ�ϲ���Ա'L0A1<2SP><=><2SP><ȷ�ϲ���Ա'L0A0>\r\n
    MakeReportItem(S_ALARM_CONFIRMOPR, szConfirmOpr,20,ALIGN_RIGHT);
    //C ����<�澯����'L0A1<2SP><=><2SP><�澯����'L0A0>\r\n
    //MakeReportItem(S_ALARM_ALARMTYPE, szAlarmType,16,ALIGN_RIGHT);
    //E ����<������ˮ��'L0A1<2SP><=><2SP><��ˮ��'L0A0>\r\n
    //MakeReportItem(S_ALARM_ENDCSN, szEndCsn,16,ALIGN_RIGHT);

}

/////////////////////////////////////////////////////////////////////////
//��������    ����"���ϸ澯ȷ���ϱ�����",
//��ʽ����:
//<�澯��־ ALARM     0          ȷ��     ��Ϣ      �澯��Դ>\r\n
//<�澯��ˮ��'L0A1<2SP><=><2SP><��ˮ��'L10A0><1SP><��ˮ��'L10A0><1SP><��ˮ��'L10A0>\r\n
//<ȷ�ϲ���Ա'L0A1<2SP><=><2SP><ȷ�ϲ���Ա'L0A0>\r\n
//�������:
//szConfirmOpr -ȷ�ϲ���Ա
//szAlarmType  -�澯����
//szBeginCsn   -��ʼ��ˮ��
//szEndCsn     -������ˮ��
//������2001-10-17
////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeAlarmFaultAckBody (const char* szConfirmOpr,
                                             const char* szAlarmType,
                                             LIST <STRING>& CsnAcked)
{
    //A ����ALARM     0          ȷ��     ��Ϣ      �澯��Դ
    AddString(S_ALARM_ALARMACKHEAD);
    AddString(STR_NEWLINE); //���\r\n����
    //B ����<�澯��ˮ��'L0A1<2SP><=><2SP><��ˮ��'L0A0>\r\n
    //ȡ��ȷ�ϳɹ��ĸ澯��ˮ��
    int uCsnNum = CsnAcked.size();
    STRING szCsn;
    const char* szSpace = "          " ;

    for (int nlist_count=0 ; nlist_count < uCsnNum ; nlist_count++)
    {
        STRING szCsnTmp = CsnAcked.front();
        CsnAcked.pop_front();
        int szCsnLen = szCsnTmp.length();
        szCsnTmp = szCsnTmp.append(szSpace,10-szCsnLen);
        szCsn = szCsn + szCsnTmp;
        szCsn = szCsn + " ";
    }

    MakeReportItem(S_ALARM_ALARMCSN, szCsn.c_str(),20,ALIGN_RIGHT);
    //C ����<ȷ�ϲ���Ա'L0A1<2SP><=><2SP><ȷ�ϲ���Ա'L0A0>\r\n
    MakeReportItem(S_ALARM_CONFIRMOPR, szConfirmOpr,20,ALIGN_RIGHT);
}

//////////////////////////////////////////////////////////////////////////
//��������    ���조�ֶβ�ѯ���ر��桱������.��ʽ����:
//ALARM  �澯��ˮ��   �澯���   �澯����    �澯��Դ  �澯���  �澯���ܷ���
//�澯����ʱ��  =  (�澯����ʱ��)
//ģ���        =  (ģ���)
//�澯����        =  (�澯����)
//��λ��Ϣ        =  �澯���� = ����ֵ  �澯��ϸ����  = ������Ϣ  �澯������ϸ����
//                =  ����������Ϣ  �޸����� �� �޸�������Ϣ      �ָ�״̬  = (�ָ�״̬)
//ȷ��״̬        =  (ȷ��״̬)
//�澯ȷ�ϲ���Ա=  (����Ա: ������|IP��ַ|�û���)
//�澯ȷ��ʱ��    =  (ȷ��ʱ��)
//�澯�ָ�ȷ�ϲ���Ա = (����Ա: ������|IP��ַ|�û���)
//�澯�ָ�ȷ��ʱ��   = (ȷ��ʱ��)
//�������:
//pAlarm  - ��ʷ�澯�ṹָ��
//pConfig - �澯�������ָ��
//������ 2001-10-17
///////////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeDetailAlarmBody(SHistoryAlarm *pAlarm, CAlarmConfig *pConfig)
{
    //A�����CAlarmDetail�ṹָ��,����"�澯����Ƭ��ͷ"
    SAlarmDetail* pAlarmDetail = pAlarm->GetAlarmDetail();
    MakeAlarmHead(pAlarmDetail);
    AddString(STR_NEWLINE);
    //����ȥ������
    //AddString(STR_NEWLINE);
    STRING szTmp = pAlarmDetail->GetAlarmTime();
    //B������<�澯����ʱ��'L20A1><2SP><=><2SP><�澯����ʱ��'L3A0>\r\n
    MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
    //C������<ģ���'L20A1><2SP><=><2SP><ģ���'L3A0>\r\n
    szTmp = pAlarmDetail->GetModule();
    MakeReportItem(S_ALARM_MODULENO,  szTmp.c_str(),20,ALIGN_RIGHT);
    //D������<�澯����'L20A1<2SP><=><2SP><�澯����'L55A0>\r\n
    szTmp = pAlarmDetail->GetAlarmName(pConfig);
    MakeReportItem(S_ALARM_ALARMNAME, szTmp.c_str(),20,ALIGN_RIGHT);
    //E������<��λ��Ϣ'L16A1><2SP><=><2SP><��λ��Ϣ1=><��λ��Ϣ1ֵ'M3A0><1SP>
    //                                    <��λ��Ϣ2=><��λ��Ϣ2ֵ'M3A0><1SP>
    //                                       ...
    //                                      <��λ��Ϣn=><��λ��Ϣnֵ'M3>\r\n
    char szContent[1024];
    szTmp = pAlarmDetail->GetFixExplain(pConfig);
    STRING szTmpExp = pAlarmDetail->GetExplain(pConfig);

    STRING szTmpPara = pAlarmDetail->GetAlarmParas();
#ifdef _OM_MODIFY
    SNPRINTF(szContent,1024,"%s=%s",S_ALARM_ALARMPARAMETER , szTmpPara.c_str());
#else    
    STRING szTmpParaExp = pAlarmDetail->GetParaExplain(pConfig);
    //����ȥ���ո�
    SNPRINTF(szContent,1024,"%s=%s %s=%s %s=%s",
        S_ALARM_ALARMPARAMETER , szTmpPara.c_str() ,
        S_ALARM_ALARMEXPLAIN , szTmpExp.c_str() ,
        S_ALARM_ALARMPARAEXPLAIN , szTmpParaExp.c_str());
#endif
    MakeReportItem(S_ALARM_ALARMPARA, szContent,20,ALIGN_RIGHT);
    
#ifndef _OM_MODIFY
    MakeReportItem(S_ALARM_ALARMFIX , szTmp.c_str(),20,ALIGN_RIGHT,201,ALIGN_LEFT);
    //F ����<ȷ��״̬'L20A1<2SP><=><2SP><ȷ��״̬'L16A0>\r\n
    szTmp = pAlarm->GetAckStatus();
    MakeReportItem(S_ALARM_ACKSTATUS , szTmp.c_str(),20,ALIGN_RIGHT,16,ALIGN_LEFT);
#endif

    //G ����<�ָ�״̬'L20A1<2SP><=><2SP><�ָ�״̬'L16A0>\r\n
    if(pAlarm->yAlarmType == AT_FAULT)
    {
        szTmp = pAlarm->GetRecoverAckStatus();
        MakeReportItem(S_ALARM_RECOVERSTATUS,szTmp.c_str(),20,ALIGN_RIGHT,16,ALIGN_RIGHT);
    }
    
#ifndef _OM_MODIFY
    //H������
    //        �澯ȷ�ϲ���Ա  =  (����Ա:������|IP��ַ|�û���)
    //        �澯ȷ��ʱ��   =   (ȷ��ʱ��)
    //        �澯�ָ�ȷ�ϲ���Ա  =  (����Ա:������|IP��ַ|�û���)
    //        �澯�ָ�ȷ��ʱ��  =  (ȷ��ʱ��)
    szTmp = pAlarm->GetFAckOptr();
    MakeReportItem(S_ALARM_ALARMACKOPR , szTmp.c_str(),20,ALIGN_RIGHT);
    szTmp = pAlarm->GetFAckTime();
    MakeReportItem(S_ALARM_ALARMACKDATE , szTmp.c_str(),20,ALIGN_RIGHT);
    if(pAlarm->yAlarmType == AT_FAULT)
    {
        szTmp = pAlarm->GetRAckOptr();
        MakeReportItem(S_ALARM_ALARMRECOVERACKOPR, szTmp.c_str(),20,ALIGN_RIGHT);
        szTmp = pAlarm->GetRecoverAckTime();
        MakeReportItem(S_ALARM_ALARMRECOVERACKDATE, szTmp.c_str(),20,ALIGN_RIGHT);
    }
#endif

}
///////////////////////////////////////////////////////////////////////////////////
//��������:
//            ALARM  �澯��ˮ��   �澯���   �澯����    �澯��Դ  �澯���  �澯���ܷ���
//            �澯����ʱ��  =  (�澯����ʱ��)
//            ģ���  =  (ģ���)
//            �澯����  =  (�澯����)
//            ��λ��Ϣ  =  (��λ��Ϣ)�澯���� = ����ֵ  �澯��ϸ����  = ������Ϣ
//                          �澯������ϸ���� = ����������Ϣ  �޸����� = (�޸�����)
//          �ָ�״̬  =  (�ָ�״̬)
//          ȷ��״̬  =  (ȷ��״̬)
//�������
//pAlarm    -��ʷ�澯�ṹָ��
//pConfig   -�澯�������ָ��
//������ 2001-10-17
/////////////////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeQueryAlarmBody(SHistoryAlarm *pAlarm, CAlarmConfig *pConfig)
{
    //A ���CAlarmDetail�ṹָ��,����"�澯����Ƭ��ͷ"
    SAlarmDetail* pAlarmDetail = pAlarm->GetAlarmDetail();
    MakeAlarmHead(pAlarmDetail);
    AddString(STR_NEWLINE);
    STRING szTmp = pAlarmDetail->GetAlarmTime();
    
#ifdef _OM_MODIFY
    //by ldf 2004-01-31 OM����
    UINT4 uSynInLog = pAlarmDetail->GetSYNNO();
    //�Ա��ĵ�ͬ���ţ�����M2000ͬ����׼ֵ����
    char tmp[8];
    sprintf(tmp,"%d",uSynInLog + (m_uMMLBaseAlarmID - 100) );
    MakeReportItem(S_ALARM_SYN_NO, tmp, 20,ALIGN_RIGHT);

    if(pAlarm->yResType == NOT_RECOVER)
    {
        MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
    }
#else
    MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
#endif

    //C ����<ģ���'L20A1><2SP><=><2SP><ģ���'L3A0>\r\n
    szTmp =  pAlarmDetail->GetModule();
    MakeReportItem(S_ALARM_MODULENO, szTmp.c_str(),20,ALIGN_RIGHT);
    //D ����<�澯����'L20A1<2SP><=><2SP><�澯����'L50A0>\r\n
    szTmp = pAlarmDetail->GetAlarmName(pConfig);
    MakeReportItem(S_ALARM_ALARMNAME, szTmp.c_str(),20,ALIGN_RIGHT);
    //E ����<��λ��Ϣ'L20A1><2SP><=><2SP><��λ��Ϣ1=><��λ��Ϣ1ֵ'M3A0><1SP>
    //                                   <��λ��Ϣ2=><��λ��Ϣ2ֵ'M6A0><1SP>
    //                                     ...
    //                                         <��λ��Ϣn=><��λ��Ϣnֵ'M3>\r\n
    
    char szContent[1024];
    STRING szTmpPara = pAlarmDetail->GetAlarmParas();
#ifdef _OM_MODIFY
    SNPRINTF(szContent,1024,"%s=%s",S_ALARM_ALARMPARAMETER ,szTmpPara.c_str());
#else
    STRING szTmpExp = pAlarmDetail->GetExplain(pConfig);
    STRING szTmpParaExp = pAlarmDetail->GetParaExplain(pConfig);
    szTmp = pAlarmDetail->GetFixExplain(pConfig);
    //����ȥ���ո�
    SNPRINTF(szContent,1024,"%s=%s %s=%s %s=%s",
        S_ALARM_ALARMPARAMETER ,szTmpPara.c_str() ,
        S_ALARM_ALARMEXPLAIN , szTmpExp.c_str() ,
        S_ALARM_ALARMPARAEXPLAIN , szTmpParaExp.c_str());
#endif
    
    MakeReportItem(S_ALARM_ALARMPARA,szContent,20,ALIGN_RIGHT);
#ifndef _OM_MODIFY
    MakeReportItem(S_ALARM_ALARMFIX , szTmp.c_str(),20,ALIGN_RIGHT,201,ALIGN_LEFT);
    //F ����<ȷ��״̬'L20A1<2SP><=><2SP><ȷ��״̬'L0A0>\r\n
    szTmp = pAlarm->GetAckStatus();
    MakeReportItem(S_ALARM_ACKSTATUS, szTmp.c_str(),20,ALIGN_LEFT);
#endif
    //G ����<�ָ�״̬'L20A1<2SP><=><2SP><�ָ�״̬'L50A0>\r\n
    szTmp = pAlarm->GetRecoverAckStatus();
    MakeReportItem(S_ALARM_RECOVERSTATUS, szTmp.c_str(),20,ALIGN_RIGHT);

    //H������
    //        �澯ȷ�ϲ���Ա  =  (����Ա:������|IP��ַ|�û���)
    //        �澯ȷ��ʱ��   =   (ȷ��ʱ��)
    //        �澯�ָ�ȷ�ϲ���Ա  =  (����Ա:������|IP��ַ|�û���)
    //        �澯�ָ�ȷ��ʱ��  =  (ȷ��ʱ��)
#ifdef _OM_MODIFY
    if(pAlarm->yResType != NOT_RECOVER)
    {
        szTmp = pAlarm->GetRevTime();
        MakeReportItem(S_ALARM_ALARMREVTIME , szTmp.c_str(),20,ALIGN_RIGHT);
    }
#else
    if(pAlarm->yAlarmType == AT_FAULT && pAlarm->yResType != NOT_RECOVER)
    {
        szTmp = pAlarm->GetRevTime();
        MakeReportItem(S_ALARM_ALARMREVTIME , szTmp.c_str(),20,ALIGN_RIGHT);
    }
    szTmp = pAlarm->GetFAckOptr();
    MakeReportItem(S_ALARM_ALARMACKOPR , szTmp.c_str(),20,ALIGN_RIGHT);
    szTmp = pAlarm->GetFAckTime();
    MakeReportItem(S_ALARM_ALARMACKDATE , szTmp.c_str(),20,ALIGN_RIGHT);
    
    szTmp = pAlarm->GetRAckOptr();
    MakeReportItem(S_ALARM_ALARMRECOVERACKOPR, szTmp.c_str(),20,ALIGN_RIGHT);
    szTmp = pAlarm->GetRecoverAckTime();
    MakeReportItem(S_ALARM_ALARMRECOVERACKDATE, szTmp.c_str(),20,ALIGN_RIGHT);
#endif

}

/////////////////////////////////////////////////////////////////////////////////
//��������:
//            ALARM  �澯��ˮ��   �澯���   �澯����    �澯��Դ  �澯���  �澯���ܷ���
//            �澯����ʱ��  =  (�澯����ʱ��)
//          ģ���  =  (ģ���)
//          �澯����  =  (�澯����)
//          ��λ��Ϣ  =     �澯����  =  ����ֵ
//                         �澯��ϸ����  =  ������Ϣ
//                         �澯������ϸ����  =  ����������Ϣ
//                         �޸�����  =  (�޸�����)
//�������:
//pAlarm  -�澯�ṹָ��
//pConfig -�澯�������ָ��
//������2001-10-17
//////////////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeQueryAlarmBody(SActiveAlarm *pAlarm, CAlarmConfig *pConfig)
{
    //A ���CAlarmDetail�ṹָ��,����"�澯����Ƭ��ͷ"
    SAlarmDetail *pAlarmDetail = pAlarm->GetAlarmDetail();
    MakeAlarmHead(pAlarmDetail);
    //B ����<�澯����ʱ��'L20A1><2SP><=><2SP><�澯����ʱ��'L3A0>\r\n
    STRING szTmp = pAlarmDetail->GetAlarmTime();
    MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
    //C ����<ģ���'L20A1><2SP><=><2SP><ģ���'L3A0>\r\n
    szTmp = pAlarmDetail->GetModule();
    MakeReportItem(S_ALARM_MODULENO, szTmp.c_str(),20,ALIGN_RIGHT);
    //D ����<�澯����'L20A1<2SP><=><2SP><�澯����'L50A0>\r\n
    szTmp = pAlarmDetail->GetAlarmName(pConfig);
    MakeReportItem(S_ALARM_ALARMNAME, szTmp.c_str(),20,ALIGN_RIGHT);
    //E ����<��λ��Ϣ'L20A1><2SP><=><2SP><��λ��Ϣ1=><��λ��Ϣ1ֵ'M3A0><1SP>
    //                                     <��λ��Ϣ2=><��λ��Ϣ2ֵ'M6A0><1SP>
    //                                     ...
    //                                     <��λ��Ϣn=><��λ��Ϣnֵ'M3>\r\n
    szTmp = pAlarmDetail->GetAlarmParas();
    MakeReportItem(S_ALARM_ALARMPARA, szTmp.c_str(),20,ALIGN_RIGHT);
    
    char szContent[1024];
    STRING szTmpPara = pAlarmDetail->GetAlarmParas();
#ifdef _OM_MODIFY
    SNPRINTF(szContent,1024,"%s=%s",S_ALARM_ALARMPARAMETER ,szTmpPara.c_str());
#else
    STRING szTmpExp = pAlarmDetail->GetExplain(pConfig);
    STRING szTmpParaExp = pAlarmDetail->GetParaExplain(pConfig);
    szTmp = pAlarmDetail->GetFixExplain(pConfig);
    ////����ȥ���ո�
    SNPRINTF(szContent,1024,"%s=%s %s=%s %s=%s %s=%s",
        S_ALARM_ALARMPARAMETER ,szTmpPara.c_str(),
        S_ALARM_ALARMEXPLAIN , szTmpExp.c_str(),
        S_ALARM_ALARMPARAEXPLAIN , szTmpParaExp.c_str(),
        S_ALARM_ALARMFIX ,szTmp.c_str());
#endif
    MakeReportItem(S_ALARM_ALARMPARA, szContent,20,ALIGN_RIGHT);
}
///////////////////////////////////////////////////////////////////////////////
//��������    ��������:
//< ALARM><2SP>\r\n
//<ģ���'L0A1<2SP><=><2SP><ģ���'L0A0>\r\n
//<��ʼ��ˮ��'L0A1<2SP><=><2SP><��ˮ��'L0A0>\r\n
//<������ˮ��'L0A1<2SP><=><2SP><��ˮ��'L0A0>\r\n
//�ı���
//�������
//start_csn    -��ʼ��ˮ��
//end_csn    -������ˮ��
//MH        -ģ���
//������2001-10-17
///////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeRecoverAlarmBody(int start_csn, int end_csn, int MH)
{
    //A ����<ģ���'L0A1<2SP><=><2SP><ģ���'L0A0>\r\n
    char tmp[32];
#ifndef _OM_MODIFY
    sprintf(tmp,"%d",MH);
    MakeReportItem(S_ALARM_MODULENO,tmp,20,ALIGN_RIGHT);
#endif

    //B ����<��ʼ��ˮ��'L0A1<2SP><=><2SP><��ˮ��'L0A0>\r\n
    sprintf(tmp,"%d",start_csn);
    MakeReportItem(S_ALARM_BEGINCSN,tmp,20,ALIGN_RIGHT);
    //C ����<������ˮ��'L0A1<2SP><=><2SP><��ˮ��'L0A0>\r\n
    sprintf(tmp,"%d",end_csn);
    MakeReportItem(S_ALARM_ENDCSN, tmp,20,ALIGN_RIGHT);
}

void CAlarmMMLReport::MakeDeleteAlarmBody(char *start_date, char *end_date, int MH)
{
    //A������<ALARM><2SP>\r\n
    AddString("ALARM");
    FillSpace(2);
    AddString(STR_NEWLINE);
    //B ����<ģ���'L0A1<2SP><=><2SP><ģ���'L0A0>\r\n
    char tmp[8];
    sprintf(tmp,"%d",MH);
    MakeReportItem(S_ALARM_MODULENO,tmp,20,ALIGN_RIGHT);
    //C ����<��ʼ����'L0A1<2SP><=><2SP><��ʼ����'L0A0>\r\n
    MakeReportItem(S_ALARM_BEGINDATE,start_date,20,ALIGN_RIGHT);
    //D ����<��������'L0A1<2SP><=><2SP><��������'L0A0>\r\n
    MakeReportItem(S_ALARM_ENDDATE, end_date,20,ALIGN_RIGHT);

}
/////////////////////////////////////////////////////
//��������    ���ݱ���ͷ��ʽ��װ����ͷ,��ʽΪ
//<��ʼ����ʶ +++><4SP><Դ��ʶ��'L32><8SP><�澯��������'L10><1SP><�澯����ʱ��'L8>\r\n
//������ 2001-10-13
/////////////////////////////////////////////////////
void CAlarmMMLReport::MakeReportHead2(SActiveAlarm *pActive)
{
    //A ��m_nReportLen = m_nHeadPosition,��ʾ��ʼ��m_nHeadPosition����ƴ�ӱ�������,������m_nHeadPosition��ʼ��������0
    m_nReportLen = m_nHeadPosition;
    memset(m_pszBuf + m_nHeadPosition, 0, m_nBufLen - m_nHeadPosition);
    //B ͨ��GetStrDate��õ�ǰ�����ַ���,����ӵ�������
    char szTime[20];
    sprintf(szTime, "%04d-%02d-%02d",pActive->uYear, pActive->yMonth, pActive->yDay);
    AddString(szTime, 10, ALIGN_LEFT);
    FillSpace(1);
    //C ͨ��GetStrTime��õ�ǰʱ���ַ���,����ӵ�������
    sprintf(szTime, "%02d:%02d:%02d", pActive->yHour,pActive->yMin,pActive->ySec);
    AddString(szTime, 8, ALIGN_LEFT);
    AddString(STR_NEWLINE);
}

//////////////////////////////////////////////////////////////////////////
//����utilityģ���е�mml_report�����еĺ���
//��������  ���ݱ���������������װ����
//          <�澯����'L16A1><2SP><=><2SP><�澯����'L50A0>\r\n�ı�������
//�������  STRING szTitle  -�Ⱥ���ߵ��ַ�
//      STRING szContent    -�Ⱥ��ұߵ��ַ�
//      int nFirstFormatLen -��һ���������ַ����
//      int nFirstAign      -��һ�������Ķ��뷽ʽ
//      int nSecondFormatLen-�ڶ����������ַ����
//      int nSecondAlign    -�ڶ��������Ķ��뷽ʽ
//��ŷ� 2002.1.15
/////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeReportItem(const char* szTitle,
                                     const char* szContent,
                                     int nFirstFormatLen,
                                     int nFirstAign,
                                     int nSecondFormatLen,
                                     int nSecondAlign)
{
    int nTitleLen = strlen(szTitle);
    int nConLen = strlen(szContent);
    int nContentLen = nConLen + nTitleLen + strlen(MML_FILL1);

    //�������̫��,��Ҫ������ʾ
    if(nContentLen >= nSecondFormatLen || nSecondFormatLen > 55)
    {
        char tempContent[1024];
        char tempTitle[512];
        BYTE nSecondSpace = 0;

        //��������ʾ���ͻ�����Ϣ���һ������,Ȼ������������µı���:
        //      ��λ��Ϣ ��  ��Ϊ��Ϣ1 = ��λ��Ϣ1��ֵ  ��λ��Ϣ2 = ��λ��Ϣ2
        //                   ֵ��λ��Ϣ  ��λ��Ϣ3 = ��λ��Ϣ3��ֵ

        if(nTitleLen >= nFirstFormatLen)
        {
            memcpy(tempTitle, szTitle, nFirstFormatLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else if(nFirstAign == ALIGN_LEFT)//����뷽ʽ
        {
            memcpy(tempTitle, szTitle, nTitleLen);
            memset(tempTitle + nTitleLen, CHAR_SPACE,nFirstFormatLen-nTitleLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else if(nFirstAign == ALIGN_RIGHT)
        {
            memset(tempTitle,CHAR_SPACE,nFirstFormatLen-nTitleLen);
            memcpy(tempTitle + nFirstFormatLen-nTitleLen, szTitle, nTitleLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else
        {
            strcpy(tempTitle, szTitle);
        }
        if(nSecondAlign == ALIGN_LEFT)
            sprintf(tempContent,"%s%s%s",tempTitle,MML_FILL1,szContent);
        else
        {
            char temp[128];
            memset(temp,CHAR_SPACE,nSecondFormatLen);
            temp[nSecondFormatLen] = '\0';
            nSecondSpace = nSecondFormatLen;
            sprintf(tempContent,"%s%s%s%s",tempTitle,MML_FILL1,temp,szContent);
        }
        STRING sOutput;
        MultiLine(tempContent,sOutput,strlen(tempTitle)+strlen(MML_FILL1)+nSecondSpace);
        AddString(sOutput.c_str());
    }
    //�����㹻�Ļ�,��ƴ��
    else
    {
    //ƴ��szTitle������
        AddString(szTitle,  nFirstFormatLen,nFirstAign);
    //���<2SP><=><2SP>����������
        AddString(MML_FILL1);
    //ƴ��szContent������,���ܶ���,���к��һ���Ⱥ��ұ������
        //by ldf 2003-03-06 ��Ӧ���ⵥ D20433
        if ( 0 == nConLen)
        {
            AddString(MML_NULL,nSecondFormatLen,nSecondAlign);
        }
        else
        {
            AddString(szContent,nSecondFormatLen,nSecondAlign);
        }
        //over

    }

    //��ӻس�����
    AddString(STR_NEWLINE);
}
////////////////////////////////////////////////////
//����utilityģ���е�mml_report�����еĺ���
//��������  �������һ�б��Ĳóɶ�������:
//    ��λ��Ϣ =  ��Ϊ��Ϣ1 = ��λ��Ϣ1��ֵ  ��λ��Ϣ2 = ��λ��Ϣ2
//             =  ֵ��λ��Ϣ  ��λ��Ϣ3 = ��λ��Ϣ3��ֵ
//�������
//      sInput      -����ı���
//      LEFT_MARGIN -������µĿո���
//      ROW_LENGTH  -ÿһ�еĳ���
//�������
//      sOutPut     -����ı���
//��ŷ� 2002.1.15
////////////////////////////////////////////////////////
void CAlarmMMLReport::MultiLine(const char* szInput,
                                STRING &sOutput,
                                int LEFT_MARGIN,
                                int ROW_LENGTH)
{
    int nLen = strlen(szInput);
    unsigned char szTmp[1024];
    unsigned char* pszOutput = szTmp;
    int nLineLen = 0;
    int nIndex = 0;
    int nCharNum;
    unsigned char* pszLast;

    unsigned char* pszInput = (unsigned char *)szInput;
    nLineLen = ROW_LENGTH;

    //�Ƿ�ʣ�µ��ַ� �� ���Ҫ��Ŀո� > �涨��һ�еĳ���
    while ((nLen + nIndex) > ROW_LENGTH)
    {
        pszLast = (unsigned char *)(pszInput + nLineLen);
        nCharNum = 0;
        if ((*pszLast) > 0xA0)   //���һ���ַ��Ǻ���
        {
            do
            {
                pszLast--;
                if (*pszLast > 0xA0)
                    nCharNum++;
                else
                    break;
            } while (pszLast > pszInput);

            if ((nCharNum % 2) != 0)
                nLineLen --;
        }
        else if (isalpha(*pszLast) || isdigit(*pszLast))
        {
            do
            {
                pszLast--;
                if (isalpha(*pszLast) || isdigit(*pszLast))
                    nCharNum++;
                else
                    break;
            } while (pszLast > pszInput);
            nLineLen -= nCharNum;
        }

        if (nLineLen == 0)
        {
            nLineLen = ROW_LENGTH - LEFT_MARGIN;
        }

        memcpy(pszOutput, pszInput, nLineLen);
        pszOutput += nLineLen;
        pszInput  += nLineLen;
        nLen      -= nLineLen;

        while (*pszInput == ' ')
        {
            pszInput++;
            nLen--;
        }

        if (nLen == 0)
            break;

        pszOutput += sprintf((char *)pszOutput, STR_NEWLINE);
        memset(pszOutput, ' ', LEFT_MARGIN);
        pszOutput += LEFT_MARGIN ;
        nIndex = LEFT_MARGIN;
        nLineLen = ROW_LENGTH - LEFT_MARGIN;
    }

    memcpy(pszOutput, pszInput, nLen);
    *(pszOutput + nLen) = '\0';
    sOutput = (char *)szTmp;
}

//by ldf 2004-01-31 OM����
void CAlarmMMLReport::SetMMLBaseAlarmID(UINT4 uMMLBaseID)
{
    m_uMMLBaseAlarmID = uMMLBaseID;
    return;
}
