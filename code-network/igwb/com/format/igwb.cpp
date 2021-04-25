/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CIGWB
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format_lib.h"
#include "iformat_cfg_imp.h"
#include "ibill_processor_imp.h"
#include "iquery_browse_imp.h"
#include "access_point.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "igwb.h"
/************ The CIGWB Defining *******************************/
// �������������캯��
//
// ���������
//   sCfgRoot����ʽ���·��
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CIGWB::CIGWB(const STRING& sCfgRoot): m_sFmtRootDir(sCfgRoot)
{
    //��ʼ����Ա����
    m_pIFormatCfgImp = NULL;
    m_pIBillProcessorImp = NULL;
    m_pIQueryBrowseImp = NULL;
    m_pFmtLib = NULL;
    m_pAPs = NULL;
    //m_sFmtRootDir = sCfgRoot;
}
/************ End of The CIGWB Defining ************************/


/************ The ~CIGWB Defining ******************************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CIGWB::~CIGWB()
{
    if(m_pAPs != NULL)
    {
        //ɾ�����������б������н�������
        LIST<CAccessPoint*>::iterator it = m_pAPs->begin();
        while(it != m_pAPs->end())
        {
            delete *it;
            it++;
        }

        //ɾ�����������б����
        delete m_pAPs;
        m_pAPs = NULL;
    }

    //ɾ����ʽ�����
    if(m_pFmtLib != NULL)
    {
        delete m_pFmtLib;
        m_pFmtLib = NULL;
    }

    //ɾ����ʽ���ýӿڶ���
    if(m_pIFormatCfgImp != NULL)
    {
        delete m_pIFormatCfgImp;
        m_pIFormatCfgImp = NULL;
    }

    //ɾ����������ӿڶ���
    if(m_pIBillProcessorImp != NULL)
    {
        delete m_pIBillProcessorImp;
        m_pIBillProcessorImp = NULL;
    }

    //ɾ����ѯ����ӿڶ���
    if(m_pIQueryBrowseImp != NULL)
    {
        delete m_pIQueryBrowseImp;
        m_pIQueryBrowseImp = NULL;
    }
}
/************ End of The ~CIGWB Defining ***********************/


/************ The Init Defining ********************************/
// ������������ʼ������
//
// ���������
//     nAPID����Ҫ��ʼ���Ľ����ID��Ϊ0��ʾ������н������г�ʼ��
//      bCfg��Ϊ0��ʾ��Ը�ʽ���ý��г�ʼ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CIGWB::Init(const UINT4 nAPID, const int bCfg)
{
    if(m_pFmtLib != NULL)
    {
         return 0;
    }

    //������ʽ�����
    m_pFmtLib = new CFormatLib(m_sFmtRootDir);
    if(m_pFmtLib->Init(bCfg) != 0)
    {
        //�����ʽ������ʼ��ʧ��
        TRACE(MTS_FORMAT, S_FMT_IGWB_INIT_FMTLIB_FAIL, 
              nAPID);
        delete m_pFmtLib;
        m_pFmtLib = NULL;
        return -1;
    }
    
    int nPos  = m_sFmtRootDir.find_last_of('/');
#ifdef _PLATFORM_WIN32
    int nPos2 = m_sFmtRootDir.find_last_of('\\');
#else //_PLATFORM_WIN32
    int nPos2 = -1;
#endif //_PLATFORM_WIN32
    nPos      = max(nPos, nPos2);
    if (nPos <= 0)
    {
        return ERR_FAIL;
    }
    STRING sConfigPath = m_sFmtRootDir.substr(0, nPos + 1);
	sConfigPath = sConfigPath + "ini/igwb.ini";
	
    //�������ļ�
    CINIFile Ini(sConfigPath.c_str());
    Ini.Open();

    //�������ļ��ж�ȡ��ǰ���������
    UINT4 nAPCount = Ini.GetInt(CFG_SEC_COMMON, 
                                CFG_COMMON_KEY_APCOUNT, 
                                CFG_COMMON_KEY_APCOUNT_DEFAULT);

    //�������������б�
    m_pAPs = new LIST<CAccessPoint*>;
    CAccessPoint* pAP = NULL;

    //������н������г�ʼ��
    if(nAPID == 0)
    {
        int nID = 1;  //�����ID������
        FILE* pAPFile = NULL;
        STRING sFilePath;
        char szTmp[20];

        while(nID <= nAPCount)
        {
            //��ȡ��ǰ����������ļ���ȫ·��
            sFilePath = m_sFmtRootDir + "/";
            sprintf(szTmp, "%d", nID);
            sFilePath += szTmp;
            sFilePath += ".apt";

            //��ֻ����ʽ�򿪵�ǰ�Ľ���������ļ�
            pAPFile = fopen(sFilePath.c_str(), "rb");
            if(pAPFile != NULL)
            {
                //����򿪵�ǰ����������ļ��ɹ����������ڵ�ǰ����������ļ���
                //������ǰ�������󣬲�������������б���
                pAP = new CAccessPoint(m_sFmtRootDir, nID, m_pFmtLib);
                m_pAPs->push_back(pAP);

                fclose(pAPFile);
                pAPFile = NULL;
            }

            //����򿪵�ǰ����������ļ�ʧ�ܣ����������ڵ�ǰ�����ID�Ժ��
            //����������ļ��ˣ�Ҳ���ǽ����ID�����Ǵ�С��������������
            else
            {
                TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
                      sFilePath.c_str());
                delete m_pFmtLib;
                m_pFmtLib = NULL;

                //ɾ�����������б������н�������
                LIST<CAccessPoint*>::iterator it = m_pAPs->begin();
                while(it != m_pAPs->end())
                {
                    delete *it;
                    it++;
                }

                //ɾ�����������б����
                delete m_pAPs;
                m_pAPs = NULL;
                
                return -1;
            }

            nID++;
        }
    }

    //ֻ���һ���ض��Ľ�����ʼ��
    else  //nAPIDΪ����Ľ����ID
    {
        //����ָ���Ľ������󣬲�������������б���
        pAP = new CAccessPoint(m_sFmtRootDir, nAPID, m_pFmtLib);
        m_pAPs->push_back(pAP);
    }

    //��ʼ�����������б������н�������
    LIST<CAccessPoint*>::iterator it = m_pAPs->begin();
    while(it != m_pAPs->end())
    {
        if((*it)->Init(m_pFmtLib) != 0)
        {
            //�����ʼ����ǰ��������ʧ��
            it = m_pAPs->begin();
            while(it != m_pAPs->end())
            {
                //ɾ�����н�������
                delete *it;
                it++;
            }
            
            //ɾ�����������б����
            delete m_pAPs;
            m_pAPs = NULL;

            //ɾ����ʽ�����
            delete m_pFmtLib;
            m_pFmtLib = NULL;

            return -1;
        }

        it++;
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The QueryInterface Defining **********************/
// ���������������Ӧ�Ľӿ�ָ��
//
// ���������
//   iid����Ҫ��õĽӿ�ID��ΪIIDö����
//
// �����������
//
//   ����ֵ����Ӧ�Ľӿ�ָ�룬������ͨ��ǿ������ת���������ص�void*
//           ת���ɶ�Ӧ�Ĵ���ָ�룬�������ͷŸ�ָ��
//
/***************************************************************/
void* CIGWB::QueryInterface(const IID iid)
{
    switch(iid)
    {
    //��ʽ���ýӿ�
    case IID_FP_IFORMATCFG:
        if(m_pIFormatCfgImp == NULL)
        {
            m_pIFormatCfgImp = new IFormatCfgImp(this);
        }
        return m_pIFormatCfgImp;

    //��ʽ����ӿ�
    case IID_FP_IBILLPROCESSOR:
        if(m_pIBillProcessorImp == NULL)
        {
            m_pIBillProcessorImp = new IBillProcessorImp(this);
        }
        return m_pIBillProcessorImp;

    //��ѯ����ӿ�
    case IID_FP_IQUERYBROWSE:
        if(m_pIQueryBrowseImp == NULL)
        {
            m_pIQueryBrowseImp = new IQueryBrowseImp(this);
        }
        return m_pIQueryBrowseImp;

    default:
        return NULL;
    }
}
/************ End of The QueryInterface Defining ***************/
