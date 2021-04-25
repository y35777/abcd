#include "s1240_collect_convert.h"
#include "s1240_tp_collect_convert.h"

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
CS1240CollectConvert::CS1240CollectConvert(int nSwitchIndex)
                        :CCollectConvertBase(nSwitchIndex) 
{
    //
}

//��������
CS1240CollectConvert::~CS1240CollectConvert()
{
    //
}


int CS1240CollectConvert::Init()
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
             "%s%d", //"switch*"
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);

    CS1240TPCollectConvert* pConvert = NULL;

	int nTPCount = INIFile.GetInt(szSecName, 
                                  CFG_CO_TP_COUNT,
                                  CFG_CO_TP_COUNT_DEFAULT);
    for(int i = 1; i <= nTPCount; i++)
    {
        /*
            ȡ��TP����·��Ϣ
        */
        pConvert = new CS1240TPCollectConvert(m_nSwitchIndex,
                                                i,
                                                m_pIBillProcessor);
        pConvert->SetCfgPath(m_szCfgPath);
        pConvert->SetSwitchName(m_szSwitchName);

        if((NULL == pConvert) || (pConvert->Init() != ERR_SUCCESS))
        {
            COTRACE("�ɼ�����(%s)����TPת���Ӷ���(ID=%d)ʧ��", 
                  m_szSwitchName,
                  i);
            delete pConvert;
            return ERR_FAIL;
        }
        m_SubObjMap[i] = pConvert;
    }

    return ERR_SUCCESS;
}

