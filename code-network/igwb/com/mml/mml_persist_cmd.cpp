/*
    ��Ʒ��:iGateway Bill V200
    ģ����:MML Server
    �ļ���:mml_persist_cmd.cpp
    ������Ϣ:
    
    ����CPersistCmd���ʵ��,�����װ��һ������Ľṹ.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/

#include "../include/frame.h"
#include "../include/toolbox.h"

#include "mml_persist_cmd.h"

CPersistCmd::CPersistCmd(_CmdDef* pData)
{
    m_uCmdCode = pData->uCmdCode;
    m_yCmdLevel =  pData->yLevel;
    m_uParaNum = pData->uParaNum;
    strcpy(m_szCmdName,pData->szCmdName);
    strcpy(m_szCmdDspName,pData->szDspName);
}

CPersistCmd::~CPersistCmd()
{
    VECTOR<CPersistPara*>::iterator i = m_ParaVector.begin(),
                                    e = m_ParaVector.end();
    while(i != e)
    {
        delete *i;
        i++;
    }
}

/*
    ��������:��ʼ������.

    ����:
    1.��ʼ���ɹ�,����ERR_SUCCESS.
    2.ʧ�ܷ��ؾ��������.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistCmd::Init()
{
    CPersistPara* pPara = NULL;
    int nResult = 0;

    for(int i = 0;i < m_uParaNum;i++)
    {
        /* �������1��ʼ */
        pPara = new CPersistPara(m_uCmdCode,i + 1,MAX_UINT2);
        if((nResult = pPara->Init()) == ERR_SUCCESS)
        {
            m_ParaVector.push_back(pPara);
        }
        else
        {
            delete pPara;
            return nResult;
        }
    }
    
    return ERR_SUCCESS;
}

char* CPersistCmd::GetCmdName()
{
    return m_szCmdName;
}

UINT2 CPersistCmd::GetCmdCode()
{
    return m_uCmdCode;
}

BYTE CPersistCmd::GetCmdLevel()
{
    return m_yCmdLevel;
}

UINT2 CPersistCmd::GetParaNum()
{
    return m_uParaNum;
}

/*
    ��������:���ݲ������ȡ��������ָ��.

    ����:
    uParaNo,������.
    һ�������������мȿ��Բ���������,�ֿ��Բ��������.

    ����:
    �ڲ������в��Ҳ�������ͬ�Ĳ���,������ƥ��Ĳ�������,
    ���ز��������ָ��,���򷵻�NULL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
CPersistPara* CPersistCmd::GetPara(UINT2 uParaNo)
{
    VECTOR<CPersistPara*>::iterator i = m_ParaVector.begin(),
                                    e = m_ParaVector.end();
    while(i != e)
    {
        if((*i)->GetParaCode() == uParaNo)
            return (*i);

        i++;
    }
    return NULL;
}

/*
    ��������:���ݲ�������ȡ��������ָ��.

    ����:
    pParaName,������.
    һ�������������мȿ��Բ���������,�ֿ��Բ��������.
    �����������ִ�Сд.

    ����:
    �ڲ������в��Ҳ�����ƥ��Ĳ���,������ƥ��Ĳ�������,
    ���ز��������ָ��,���򷵻�NULL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
CPersistPara* CPersistCmd::GetPara(const char* pParaName)
{
    VECTOR<CPersistPara*>::iterator i = m_ParaVector.begin(),
                                    e = m_ParaVector.end();
    while(i != e)
    {
        if(!ACE_OS::strcasecmp((*i)->GetParaName(),pParaName))
            return (*i);

        i++;
    }
    return NULL;
}


/*
    CPersistCmdMan���ʵ��.
*/
CPersistCmdMan* CPersistCmdMan::g_pCmdMan = NULL;

CPersistCmdMan::CPersistCmdMan()
{
    m_pParaDef = NULL;
    m_uParaNum = 0;
    m_pChildParaDef = NULL;
    m_uChildParaNum = 0;
    m_pParaEnumDef = NULL;
    m_uParaEnumNum = 0;
    m_pParaRangeDef = NULL;
    m_uParaRangeNum = 0;
}

CPersistCmdMan::~CPersistCmdMan()
{
    if(m_pParaDef)
        delete m_pParaDef;

    if(m_pChildParaDef)
        delete m_pChildParaDef;

    if(m_pParaEnumDef)
        delete m_pParaEnumDef;

    if(m_pParaRangeDef)
        delete m_pParaRangeDef;

    MAP<STRING,CPersistCmd*>::iterator i = m_CmdMap.begin(),
                                       e = m_CmdMap.end();
    while(i != e)
    {
        delete (*i).second;
        i++;
    }
}

CPersistCmd* CPersistCmdMan::GetCmd(const STRING strCmdName)
{
    /*
        YACC�ʷ��������ص��������Ǵ�д��ĸ.
        ����macro.ini�ļ�ת�����������ļ���,
        ������Ҳ�Ǵ�д��ĸ.
    */
    if(m_CmdMap.find(strCmdName) == m_CmdMap.end())
        return NULL;

    return m_CmdMap[strCmdName];
}

/*
    ��������:��ʼ�������������.

    ����:
    ��ʼ���ɹ�,����ERR_SUCCESS.
    ���ļ�������,����ERR_FILE_NOT_EXIST.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistCmdMan::Init()
{
    char szOut[MAX_PATH + 1];
    char szFileName[MAX_PATH+1];

    const char* szDataFile[] = {"paradef.dat",
                                "childparadef.dat",
                                "paraenumdef.dat",
                                "pararangedef.dat"};
    char* pBuf[] = {NULL,NULL,NULL,NULL};
    UINT4 nFileLen[] = {0,0,0,0};

    FILE* f = NULL;
    //����������·�����ٿ���
	sprintf(szOut,"%s/%s",GetAppPath(),CFG_MML_PERSIST_DATA_DEF);
    try
    {
        /* ��ȡ��������ļ� */
        for(int i = 0;i < 4;i++)
        {
            if((szOut[strlen(szOut) - 1] == '/')
                || (szOut[strlen(szOut) - 1] == '\\'))
            {
                sprintf(szFileName,"%s%s",szOut,szDataFile[i]);
            }
            else
            {
                sprintf(szFileName,"%s/%s",szOut,szDataFile[i]);
            }

            f = fopen(szFileName,"rb");
            if(f == NULL)
            {
                throw "Open file fail.";
            }

            nFileLen[i] = FileSize(f);
            pBuf[i] = new char[nFileLen[i]];

            if(fread(pBuf[i],nFileLen[i],1,f) != 1)
            {
                fclose(f);
                throw "Read file fail.";
            }

            fclose(f);
        }
    }
    catch(const char* e)
    {
        e++;//Ϊ������"����δ����"�ĸ澯.
        for(int i = 0;i < 4;i++)
        {
            delete[] pBuf[i];
        }
        return ERR_FAIL;
    }

    m_pParaDef = pBuf[0];
    m_uParaNum = nFileLen[0] / sizeof(_ParaDef);

    m_pChildParaDef = pBuf[1];
    m_uChildParaNum = nFileLen[1] / sizeof(_ChildParaDef);

    m_pParaEnumDef = pBuf[2];
    m_uParaEnumNum = nFileLen[2] / sizeof(_ParaEnumDef);

    m_pParaRangeDef = pBuf[3];
    m_uParaRangeNum = nFileLen[3] / sizeof(_ParaRangeDef);
    //ת���ֽ��������
    PParaDef pParaDef = (PParaDef)m_pParaDef;
	int j = 0;
	for(; j < m_uParaNum; j++)
	{
		pParaDef[j].uCmdCode = ntohs(pParaDef[j].uCmdCode);
		pParaDef[j].uParaCode = ntohs(pParaDef[j].uParaCode);
	}
	PChildParaDef pChildParaDef = (PChildParaDef)m_pChildParaDef;
	for(j = 0; j < m_uChildParaNum; j++)
	{
		pChildParaDef[j].uChildParaCode = ntohs(pChildParaDef[j].uChildParaCode);
		pChildParaDef[j].uCmdCode = ntohs(pChildParaDef[j].uCmdCode);
		pChildParaDef[j].uParaCode = ntohs(pChildParaDef[j].uParaCode);
	}
    PParaEnumDef pParaEnumDef = (PParaEnumDef)m_pParaEnumDef;
	for(j = 0; j < m_uParaEnumNum; j++)
	{
		pParaEnumDef[j].uChildParaCode = ntohs(pParaEnumDef[j].uChildParaCode);
		pParaEnumDef[j].uCmdCode = ntohs(pParaEnumDef[j].uCmdCode);
		pParaEnumDef[j].uParaCode = ntohs(pParaEnumDef[j].uParaCode);
		pParaEnumDef[j].uValue = ntohl(pParaEnumDef[j].uValue);
	}
	PParaRangeDef pParaRangeDef = (PParaRangeDef) m_pParaRangeDef;
	for(j = 0; j < m_uParaRangeNum; j++)
	{
        pParaRangeDef[j].nMaxValue = ntohl(pParaRangeDef[j].nMaxValue);
		pParaRangeDef[j].nMinValue = ntohl(pParaRangeDef[j].nMinValue);
		pParaRangeDef[j].uChildParaCode = ntohs(pParaRangeDef[j].uChildParaCode);
		pParaRangeDef[j].uCmdCode = ntohs(pParaRangeDef[j].uCmdCode);
		pParaRangeDef[j].uParaCode = ntohs(pParaRangeDef[j].uParaCode);
	}
	//over
    /* ��ʼ�������ļ��е�ÿһ������ */
    if((szOut[strlen(szOut) - 1] == '/')
        || (szOut[strlen(szOut) - 1] == '\\'))
    {
        sprintf(szFileName,"%scmddef.dat",szOut);
    }
    else
    {
        sprintf(szFileName,"%s/cmddef.dat",szOut);
    }

    f = fopen(szFileName,"rb");
    if(f == NULL) return ERR_FILE_NOT_EXIST;

    _CmdDef sCmd;
    CPersistCmd* pCmd = NULL;
    int nResult = ERR_SUCCESS;
	
    while(fread(&sCmd,sizeof(_CmdDef),1,f) == 1)
    {
		//ת���ֽ��������
		sCmd.uCmdCode = ntohs(sCmd.uCmdCode);
		sCmd.uParaNum = ntohs(sCmd.uParaNum);
		//over
        pCmd = new CPersistCmd(&sCmd);
        if((nResult = pCmd->Init()) != ERR_SUCCESS)
        {
            delete pCmd;
            break;
        }

        STRING strCmdName = sCmd.szCmdName;
        m_CmdMap[strCmdName] = pCmd;
    }

    /* ���ղ���ʹ�õ�ָ�� */

    delete m_pParaDef;
    delete m_pChildParaDef;
    delete m_pParaEnumDef;
    delete m_pParaRangeDef;

    m_pParaDef = NULL;
    m_uParaNum = 0;
    m_pChildParaDef = NULL;
    m_uChildParaNum = 0;
    m_pParaEnumDef = NULL;
    m_uParaEnumNum = 0;
    m_pParaRangeDef = NULL;
    m_uParaRangeNum = 0;

    if(ferror(f))   return ERR_FAIL;
    fclose(f);

    return nResult;
}


CPersistCmdMan* CPersistCmdMan::Instance()
{
    if(g_pCmdMan == NULL)
    {
        g_pCmdMan = new CPersistCmdMan();
        if(g_pCmdMan->Init() != ERR_SUCCESS)
        {
            delete g_pCmdMan;
            g_pCmdMan = NULL;
            return NULL;
        }
    }
    return g_pCmdMan;
}


void CPersistCmdMan::Destroy()
{
    if(g_pCmdMan)
    {
        delete g_pCmdMan;
        g_pCmdMan = NULL;
    }
}

void CPersistCmdMan::GetParaDef(_ParaDef*& pParaDef,UINT4& uParaNum)
{
    pParaDef = (_ParaDef*)m_pParaDef;
    uParaNum = m_uParaNum;
}

void CPersistCmdMan::GetChildParaDef(_ChildParaDef*& pParaDef,
                                  UINT4& uChildParaNum)
{
    pParaDef = (_ChildParaDef*)m_pChildParaDef;
    uChildParaNum = m_uChildParaNum;
}

void CPersistCmdMan::GetParaEnumDef(_ParaEnumDef*& pParaEnumDef,
                                 UINT4& uParaEnumNum)
{
    pParaEnumDef = (_ParaEnumDef*)m_pParaEnumDef;
    uParaEnumNum = m_uParaEnumNum;
}

void CPersistCmdMan::GetParaRangeDef(_ParaRangeDef*& pParaRangeDef,
                                  UINT4& uParaRangeNum)
{
    pParaRangeDef = (_ParaRangeDef*)m_pParaRangeDef;
    uParaRangeNum = m_uParaRangeNum;
}


const char* CPersistCmd::GetCmdDspName()
{
    return m_szCmdDspName;
}

