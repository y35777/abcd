#include "com_collect_convert.h"
#include "com_dir_collect_convert.h"
#include "../../../include/format_out.h"


//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
CComCollectConvert::CComCollectConvert(int nSwitchIndex)
                        :CCollectConvertBase(nSwitchIndex) 
{
    //
}

//��������
CComCollectConvert::~CComCollectConvert()
{
    //
}


int CComCollectConvert::Init()
{
    
    int nRet = CCollectConvertBase::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //����S1240��������Ϣ
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(%s)ת��ģ��������ļ�(%s)ʧ��", 
              m_szSwitchName, 
              m_szCfgPath);
        return ERR_FAIL;
    }

    char szSecName[CFG_BUF_SIZE];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[CFG_BUF_SIZE - 1] = '\0';


    CCollectConvertSubObj* pConvert = NULL;

	int nDirCount = INIFile.GetInt(szSecName, 
                                  CFG_CO_COL_DIR_COUNT,
                                  CFG_CO_COL_DIR_COUNT_DEFAULT);
    for(int i = 1; i <= nDirCount; i++)
    {
        /*
            ȡ��DIR����·��Ϣ
        */
        if(StringiCmp(m_szCollectId, CO_TYPE_TELLIN))
        {
            pConvert = new CTellinDirConvert(m_nSwitchIndex,
                                             i,
                                             m_pIBillProcessor);
        }
        else if(StringiCmp(m_szCollectId, CO_TYPE_SJZ_TELLIN))
        {
            pConvert = new CSJZTellinDirConvert(m_nSwitchIndex,
												i,
												m_pIBillProcessor);
        }
        else if(StringiCmp(m_szCollectId,CO_TYPE_METER_CC08))
        {
            pConvert = new CMeterSumDirConvert(m_nSwitchIndex,
                                            i,
                                            m_pIBillProcessor);
            
        }
        else
        {
        
            pConvert = new CComDirCollectConvert(m_nSwitchIndex,
                                                i,
                                                m_pIBillProcessor);
        }

        pConvert->SetCfgPath(m_szCfgPath);
        pConvert->SetSwitchName(m_szSwitchName);

        if((NULL == pConvert) || (pConvert->Init() != ERR_SUCCESS))
        {
            COTRACE("�ɼ�����(%s)����Dirת���Ӷ���(ID=%d)ʧ��", 
                  m_szSwitchName,
                  i);
            delete pConvert;
            return ERR_FAIL;
        }
        m_SubObjMap[i] = pConvert;
    }

    return ERR_SUCCESS;
}

