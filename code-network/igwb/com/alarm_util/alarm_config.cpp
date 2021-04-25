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
//功能描述  构造CAlarmConfig对象
//          szCfgFile必须为一有效路径名
//          CAlarmConfig函数不做有效性判断
//输入参数  szCfgFile：告警参数配置文件的全路径名
//////////////////////////////////////////////////////////////////////
CAlarmConfig::CAlarmConfig(const char* szCfgFile)
{
    sprintf(m_szCfgFile,"%s",szCfgFile);
}

//////////////////////////////////////////////////////////////////////
//功能描述  析构CAlarmConfig对象
//输入参数  szCfgFile - 告警参数配置文件的全路径名
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
//功能描述  读入告警信息配置文件
//输入参数  无
//输出参数  无
//返回值    返回ERR_SUCCESS(0)表示成功;其它(非0)表示失败,返回值代表错误码
///////////////////////////////////////////////////////////////////////////
int CAlarmConfig::LoadConfig()
{
    //以只读方式打开m_szCfgFile，若文件打开失败，返回ERR_OPEN_ALM_CFG_FILE；
    FILE *pFile = fopen(m_szCfgFile,"rb");
    if(pFile == NULL)
    {
        return ERR_OPEN_ALM_CFG_FILE;
    }

    //将告警信息配置文件中所有告警配置读出，添加到m_AlarmExplainMap中：
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
        //张玉新 2002-06-07 D14379
        pAlarmExplain->uAlarmID = ntohs(pAlarmExplain->uAlarmID);
        //over
        if (m_AlarmExplainMap.find(pAlarmExplain->uAlarmID) == m_AlarmExplainMap.end())
        {
            m_AlarmExplainMap[pAlarmExplain->uAlarmID] = pAlarmExplain;
            pAlarmExplain = new SAlarmExplain;
        }
    }

    //关闭m_szCfgFile文件并返回ERR_SUCCESS
    delete pAlarmExplain;
    fclose(pFile);
    return ERR_SUCCESS;

}

///////////////////////////////////////////////////////////////////////////////////
//功能描述  根据uAlarmID查找相应的告警解释信息
//输入参数  告警ID
//输出参数  无
//返回值    详细告警解释结构指针
///////////////////////////////////////////////////////////////////////////////////
PAlarmExplain CAlarmConfig::GetAlarmExplain(UINT2 uAlarmID)
{
    //申明临时变量
    ALARM_EXPLAIN_MAP::iterator  it;

    // 以uAlarmID为Key值查找m_AlarmExplainMap,如果找到,则返回告警uAlarmID的详细解释
    //否则返回NULL
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
//功能描述  根据uAlarmID查找其对应的告警类型
//输入参数  告警ID
//输出参数  无
//返回值    该告警对应的告警类型
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