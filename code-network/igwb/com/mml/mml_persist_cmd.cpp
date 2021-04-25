/*
    产品名:iGateway Bill V200
    模块名:MML Server
    文件名:mml_persist_cmd.cpp
    描述信息:
    
    包含CPersistCmd类的实现,该类封装了一条命令的结构.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
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
    函数功能:初始化函数.

    返回:
    1.初始化成功,返回ERR_SUCCESS.
    2.失败返回具体错误码.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
int CPersistCmd::Init()
{
    CPersistPara* pPara = NULL;
    int nResult = 0;

    for(int i = 0;i < m_uParaNum;i++)
    {
        /* 参数码从1开始 */
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
    函数功能:根据参数码获取参数对象指针.

    参数:
    uParaNo,参数码.
    一个参数在命令中既可以参数名出现,又可以参数码出现.

    返回:
    在参数表中查找参数码相同的参数,若存在匹配的参数对象,
    返回参数对象的指针,否则返回NULL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
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
    函数功能:根据参数名获取参数对象指针.

    参数:
    pParaName,参数名.
    一个参数在命令中既可以参数名出现,又可以参数码出现.
    命令名不区分大小写.

    返回:
    在参数表中查找参数名匹配的参数,若存在匹配的参数对象,
    返回参数对象的指针,否则返回NULL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
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
    CPersistCmdMan类的实现.
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
        YACC词法分析返回的命令名是大写字母.
        并且macro.ini文件转换出的命令文件中,
        命令名也是大写字母.
    */
    if(m_CmdMap.find(strCmdName) == m_CmdMap.end())
        return NULL;

    return m_CmdMap[strCmdName];
}

/*
    函数功能:初始化各个命令参数.

    返回:
    初始化成功,返回ERR_SUCCESS.
    若文件不存在,返回ERR_FILE_NOT_EXIST.

    修改记录:
    zhouyonghui,2001-10-24,创建.
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
    //命令表的配置路径不再可配
	sprintf(szOut,"%s/%s",GetAppPath(),CFG_MML_PERSIST_DATA_DEF);
    try
    {
        /* 读取命令参数文件 */
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
        e++;//为了消除"变量未引用"的告警.
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
    //转化字节序的问题
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
    /* 初始化命令文件中的每一条命令 */
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
		//转换字节序的问题
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

    /* 回收不再使用的指针 */

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

