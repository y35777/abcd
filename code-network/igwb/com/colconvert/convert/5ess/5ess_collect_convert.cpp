#include "5ess_collect_convert.h"
#include "5ess_prefix_collect_convert.h"
#include "../../../include/format_out.h"


//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
C5essCollectConvert::C5essCollectConvert(int nSwitchIndex)
                        :CCollectConvertBase(nSwitchIndex) 
{
    //
}

//��������
C5essCollectConvert::~C5essCollectConvert()
{
    //
}


int C5essCollectConvert::Init()
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


    C5essPrefixCollectConvert* pConvert = NULL;

	int nPrefixCount = INIFile.GetInt(szSecName, 
                                  CFG_5ESS_PREFIX_NUM,
                                  CFG_5ESS_PREFIX_NUM_DEFAULT);
    for(int i = 1; i <= nPrefixCount; i++)
    {
        pConvert = new C5essPrefixCollectConvert(m_nSwitchIndex,
                                                i,
                                                m_pIBillProcessor);
        pConvert->SetCfgPath(m_szCfgPath);
        pConvert->SetSwitchName(m_szSwitchName);

        if((NULL == pConvert) || (pConvert->Init() != ERR_SUCCESS))
        {
            COTRACE("�ɼ�����(%s)����Prefixת���Ӷ���(ID=%d)ʧ��", 
                  m_szSwitchName,
                  i);
            delete pConvert;
            return ERR_FAIL;
        }
        m_SubObjMap[i] = pConvert;
    }

    return ERR_SUCCESS;
}

