/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CFormatLib
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "format.h"
#include "format_global.h"
#include "format_lib.h"

/************ The CFormatLib Defining **************************/
// 功能描述：构造函数
//
// 输入参数：
//   sRootDir：格式库根目录路径式
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CFormatLib::CFormatLib(const STRING& sRootDir): m_sFmtRootDir(sRootDir)
{
    //初始化成员变量
    //m_sFmtRootDir = sRootDir;
    m_pFmtsMap = NULL;
    m_bIsMustMerge = FALSE;
}
/************ End of The CFormatLib Defining *******************/


/************ The ~CFormatLib Defining *************************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CFormatLib::~CFormatLib()
{
    FORMAT_MAP::iterator item;

    if(m_pFmtsMap != NULL)
    {
        //删除所有的格式库对象
        item = m_pFmtsMap->begin();
        while(item != m_pFmtsMap->end())
        {
            delete item->second;
            item->second = NULL;

            item++;
        }

        //删除格式库名与格式库对象的映射关系对象
        delete m_pFmtsMap;
        m_pFmtsMap = NULL;
    }

    while(m_mapAPToFmtSpDll.size())
    {
        ACE_DLL* pDll = m_mapAPToFmtSpDll.begin()->second;
        pDll->close();
        delete pDll;
        m_mapAPToFmtSpDll.erase(m_mapAPToFmtSpDll.begin());
    }
}
/************ End of The ~CFormatLib Defining ******************/

extern "C" {
typedef CFormat* (*CREATEFORMATOBJECT)(const char* szRootDir, const char* szFmtName);
}
CFormat* CFormatLib::CreateFormatObject(const char* szDllName,
                                        const char* szRootDir, 
                                        const char* szFmtName,
                                        int nAP)
{
    ACE_DLL* pDll = NULL;
    if(m_mapAPToFmtSpDll.find(nAP) == m_mapAPToFmtSpDll.end())
    {
        pDll = new ACE_DLL;
        int nRet = pDll->open(szDllName);
        if(0 != nRet)
        {
            return NULL;
        }
    }
    else
    {
        pDll = m_mapAPToFmtSpDll[nAP];
    }

    CREATEFORMATOBJECT f = (CREATEFORMATOBJECT)pDll->symbol("CreateFormatObject");
    if(NULL == f)
    {
        return NULL;
    }

    return f(szRootDir, szFmtName);
}

/************ The Init Defining ********************************/
// 功能描述：初始化函数
//
// 输入参数：
//       bCfg：为0表示是针对IFormatCfg接口进行初始化
//
// 输出参数：无
//
//   返回值：初始化成功返回0；否则返回-1
//
/***************************************************************/
int CFormatLib::Init(int bCfg)
{
    //打开配置文件
    CINIFile Ini(GetCfgFilePath());
    Ini.Open();

    //从配置文件中读取当前接入点类型
    UINT4 nAPCount = Ini.GetInt(CFG_SEC_COMMON, 
                                CFG_COMMON_KEY_APCOUNT, 
                                CFG_COMMON_KEY_APCOUNT_DEFAULT);
    int i = 0 ;
    char szFmtDll[40];
    for(i = 1; i <= nAPCount; i++)
    {
        char strSec[20];
        sprintf(strSec, "%s%d", CFG_SEC_AP, i);
        Ini.GetString(strSec,
                      CFG_AP_KEY_FMT_DLL,
                      CFG_AP_KEY_FMT_DLL_DEFAULT,
                      szFmtDll,
                      sizeof(szFmtDll));
        if(szFmtDll[0] != '\0')
        {
            m_mapAPToFmtSpName[i] = szFmtDll;
        }
    }
    
    if(m_pFmtsMap == NULL)
    {
        m_pFmtsMap = new FORMAT_MAP;
    }

    //如果不针对格式配置进行初始化，则不创建任何CFormat对象
    if(bCfg != 0)
    {
        return 0;
    }

    //否则针对每一个格式配置文件，创建一个CFormat*对象---->
    STRING sFmtLibDir = m_sFmtRootDir;
    
    ACE_DIR *pDir = NULL;
    dirent* pDirent =  NULL;

    //打开格式配置文件目录
    pDir = ACE_OS::opendir(sFmtLibDir.c_str());

    //如果打开目录失败，失败原因是目录不存在
    if((pDir == NULL) && (errno == ENOENT))
    {
        //则创建该配置文件的目录
        if(!CreateDeepDir(m_sFmtRootDir.c_str()))
        {
            return -1;  //创建目录失败
        }

        return 0;
    }

    //如果打开目录失败，失败原因不是目录不存在
    else if(pDir == NULL)
    {
        return -1;
    }
    
    //如文件已打开, 将之关闭
    if(pDir != NULL)
    {
        ACE_OS::closedir(pDir);
    }
    
    
    return 0;
}


/************ The GetFormats Defining **************************/
// 功能描述：获得当前格式库中所有格式名
//
// 输入参数：无
//
// 输出参数：
//  FmtNamesList：获得的所有格式名列表
//
//   返回值：格式名总数
//
/***************************************************************/
UINT4 CFormatLib::GetFormats(LIST<STRING>& FmtNamesList) const
{
    FORMAT_MAP::iterator item;  //格式名与格式对象映射关系迭代子


    //初始化输出参数
    FmtNamesList.clear();
    if(m_pFmtsMap == NULL)
    {
        return 0;
    }

    //遍列格式名与格式对象映射，获取其格式名称
    item = m_pFmtsMap->begin();
    while(item != m_pFmtsMap->end())
    {
        //将获取的格式名加入到输出参数列表中去
        FmtNamesList.push_back(item->first);
        item++;
    }

    //返回格式名个数
    return FmtNamesList.size();
}
/************ End of The GetFormats Defining *******************/


/************ The GetFormat Defining ***************************/
// 功能描述：获得指定格式名的格式对象
//
// 输入参数：
//   sFmtName：格式名
//
// 输出参数：
//
//   返回值：指定格式名的格式对象
//
/***************************************************************/
CFormat* CFormatLib::GetFormat(const STRING& sFmtName, int nAP)
{
    CFormat*  pFmt = NULL;      //格式对象指针
    FORMAT_MAP::iterator item;  //指定格式名在格式对象映射关系中的节点

    //确保输入的参数正确性
    if(sFmtName.length() == 0)
    {
        return NULL;
    }

    //确保格式名与格式对象映射关系存在
    if(m_pFmtsMap == NULL)
    {
        m_pFmtsMap = new FORMAT_MAP;
    }
    else
    {
        //查找指定格式名的格式对象
        item = m_pFmtsMap->find(sFmtName);
        if(item != m_pFmtsMap->end())
        {
            //找到指定格式名的格式对象
            pFmt = item->second;
            return pFmt;
        }
    }

    if(nAP > 0)
    {	
        
        if(m_mapAPToFmtSpName.find(nAP) != m_mapAPToFmtSpName.end())
        {
            STRING& strFmtDll = m_mapAPToFmtSpName[nAP];
            if (WLAN_DLL_NAME == strFmtDll) 
            {
                pFmt = new CFormat(m_sFmtRootDir, sFmtName);
            }
            else
            {            
                //创建当前格式对象
                pFmt = CreateFormatObject(strFmtDll.c_str(), 
                                          m_sFmtRootDir.c_str(),
                                          sFmtName.c_str(),
                                          nAP);
            }
        }
        else        
        {
            pFmt = new CFormat(m_sFmtRootDir, sFmtName);
        }       


        if(NULL == pFmt)
        {
            //如果初始化当前格式对象失败
            return NULL;
        }
        pFmt->SetMustMerge(m_bIsMustMerge);

        if(pFmt->Init() != 0)
        {
            //如果初始化当前格式对象失败
            delete pFmt;
            return NULL;
        }

        STRING  sSrcFmtName;
        pFmt->GetSrcFmtName(sSrcFmtName);

        //如果本格式名不等于源格式名，且原格式不为空
        if((sSrcFmtName.length() != 0) && (sSrcFmtName != sFmtName))
        {
            //获取本格式对象的原格式对象
            CFormat* pSrcFmt = GetFormat(sSrcFmtName, nAP);
            if(pSrcFmt == NULL)
            {
                delete pFmt;
                return NULL;
            }

            //设置本格式的原格式对象，并把本格式对象加入到格式对象映射关系中
            if(pFmt->SetSrcFmt(pSrcFmt) != 0)
            {
                delete pFmt;
                return NULL;
            }
        }

        //本格式的源格式为自己
        else if(sSrcFmtName == sFmtName)
        {
            //设置本格式的原格式对象，并把本格式对象加入到格式对象映射关系中
            if(pFmt->SetSrcFmt(pFmt) != 0)
            {
                delete pFmt;
                return NULL;
            }
        }
        //如果本格式的原格式为空
/*
        else
        {
            //
        }
*/
        (*m_pFmtsMap)[sFmtName] = pFmt;

        return pFmt;
    }

    //指定格式名的格式对象不存在
    return NULL;
}
/************ End of The GetFormat Defining ********************/


/************ The AddFormat Defining ***************************/
// 功能描述：增加一种新话单格式配置
//
// 输入参数：
//   sFmtName：新增加的格式配置名称
//
// 输出参数：无
//
//   返回值：增加成功返回0，否则返回-1
//
/***************************************************************/
int CFormatLib::AddFormat(const STRING& sFmtName)
{
/*
    FORMAT_MAP::iterator item;  //指定格式名在格式对象映射关系中的节点

    //确保格式名与格式对象映射关系存在
    if(m_pFmtsMap == NULL)
    {
        m_pFmtsMap = new FORMAT_MAP;
    }

    //查找指定格式名的格式对象
    item = m_pFmtsMap->find(sFmtName);
    if(item != m_pFmtsMap->end())
    {
        //如果指定格式名的格式对象已经存在
        return -1;
    }

    //指定格式名的格式对象不存在
    else
    {
        //创建一个指定格式名的新格式对象
        CFormat* pFmt = NULL;
        if(!Is3GFormat(sFmtName))
        {
            //如果是3G话单格式
/ *
            pFmt = new CFormatR99(m_sFmtRootDir, sFmtName);
* /
        }
		else if(!Is3GR4Format(sFmtName))
        {
            //如果是3GR4话单格式
/ *
            pFmt = new CFormatR4(m_sFmtRootDir, sFmtName);
* /
        }
		else if(!IsGPRSFormat(sFmtName))
        {
            //如果是GPRS话单格式
/ *
            pFmt = new CFormatR98(m_sFmtRootDir, sFmtName);
* /
        }
        else
        {
            //非3G话单格式
            pFmt = new CFormat(m_sFmtRootDir, sFmtName);
        }

        int nRet = pFmt->Create();
        if(nRet != 0)
        {
            //创建格式配置文件失败
            delete pFmt;
            return -1;
        }

        m_pFmtsMap->insert(FORMAT_MAP::value_type(sFmtName, pFmt));
        
        return 0;
    }*/
       return 0;
} 

/************ End of The AddFormat Defining ********************/


/************ The DelFormat Defining ***************************/
// 功能描述：删除一种话单格式配置
//
// 输入参数：
//   sFmtName：待删除的的格式配置名称
//
// 输出参数：无
//
//   返回值：成功返回0，否则返回-1
//
/***************************************************************/
int CFormatLib::DelFormat(const STRING& sFmtName)
{
    CFormat*  pFmt = NULL;      //格式对象指针
    FORMAT_MAP::iterator item;  //指定格式名在格式对象映射关系中的节点


    //确保格式名与格式对象映射关系存在
    if(m_pFmtsMap == NULL)
    {
        return -1;
    }

    //查找指定格式名的格式对象
    item = m_pFmtsMap->find(sFmtName);
    if(item != m_pFmtsMap->end())
    {
        //如果指定格式名的格式对象存在
        pFmt = item->second;
        if(pFmt->Delete() != 0)
        {
            return -1;
        }

        //从格式对象映射关系中删除指定的格式对象
        delete pFmt;
        m_pFmtsMap->erase(item);
        return 0;
    }

    //指定格式名的格式对象不存在
    else
    {
        //指定格式名的格式对象不存在
        return -1;
    }
}
/************ End of The DelFormat Defining ********************/


/************ The Commit Defining ******************************/
// 功能描述：使上一次Commit到本次Commit调用之间所修改的配置信息
//           保存到磁盘中
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormatLib::Commit(void)
{
    CFormat*  pFmt = NULL;      //格式对象指针
    FORMAT_MAP::iterator item;  //指定格式名在格式对象映射关系中的节点


    if(m_pFmtsMap != NULL)
    {
        //遍历所有格式对象
        item = m_pFmtsMap->begin();
        while(item != m_pFmtsMap->end())
        {
            //提交当前格式的配置信息
            pFmt = item->second;
            if(pFmt->Commit() != 0)
            {
                return -1;
            }

            item++;
        }
    }

    return 0;
}
/************ End of The Commit Defining ***********************/

void CFormatLib::SetMustMerge(BOOL bMustMerge)
{
     m_bIsMustMerge = bMustMerge;	
}
