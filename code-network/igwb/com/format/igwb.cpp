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
// 功能描述：构造函数
//
// 输入参数：
//   sCfgRoot：格式库根路径
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CIGWB::CIGWB(const STRING& sCfgRoot): m_sFmtRootDir(sCfgRoot)
{
    //初始化成员变量
    m_pIFormatCfgImp = NULL;
    m_pIBillProcessorImp = NULL;
    m_pIQueryBrowseImp = NULL;
    m_pFmtLib = NULL;
    m_pAPs = NULL;
    //m_sFmtRootDir = sCfgRoot;
}
/************ End of The CIGWB Defining ************************/


/************ The ~CIGWB Defining ******************************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CIGWB::~CIGWB()
{
    if(m_pAPs != NULL)
    {
        //删除接入点对象列表中所有接入点对象
        LIST<CAccessPoint*>::iterator it = m_pAPs->begin();
        while(it != m_pAPs->end())
        {
            delete *it;
            it++;
        }

        //删除接入点对象列表对象
        delete m_pAPs;
        m_pAPs = NULL;
    }

    //删除格式库对象
    if(m_pFmtLib != NULL)
    {
        delete m_pFmtLib;
        m_pFmtLib = NULL;
    }

    //删除格式配置接口对象
    if(m_pIFormatCfgImp != NULL)
    {
        delete m_pIFormatCfgImp;
        m_pIFormatCfgImp = NULL;
    }

    //删除话单处理接口对象
    if(m_pIBillProcessorImp != NULL)
    {
        delete m_pIBillProcessorImp;
        m_pIBillProcessorImp = NULL;
    }

    //删除查询浏览接口对象
    if(m_pIQueryBrowseImp != NULL)
    {
        delete m_pIQueryBrowseImp;
        m_pIQueryBrowseImp = NULL;
    }
}
/************ End of The ~CIGWB Defining ***********************/


/************ The Init Defining ********************************/
// 功能描述：初始化函数
//
// 输入参数：
//     nAPID：需要初始化的接入点ID，为0表示针对所有接入点进行初始化
//      bCfg：为0表示针对格式配置进行初始化
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CIGWB::Init(const UINT4 nAPID, const int bCfg)
{
    if(m_pFmtLib != NULL)
    {
         return 0;
    }

    //创建格式库对象
    m_pFmtLib = new CFormatLib(m_sFmtRootDir);
    if(m_pFmtLib->Init(bCfg) != 0)
    {
        //如果格式库对象初始化失败
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
	
    //打开配置文件
    CINIFile Ini(sConfigPath.c_str());
    Ini.Open();

    //从配置文件中读取当前接入点类型
    UINT4 nAPCount = Ini.GetInt(CFG_SEC_COMMON, 
                                CFG_COMMON_KEY_APCOUNT, 
                                CFG_COMMON_KEY_APCOUNT_DEFAULT);

    //创建接入点对象列表
    m_pAPs = new LIST<CAccessPoint*>;
    CAccessPoint* pAP = NULL;

    //针对所有接入点进行初始化
    if(nAPID == 0)
    {
        int nID = 1;  //接入点ID计数器
        FILE* pAPFile = NULL;
        STRING sFilePath;
        char szTmp[20];

        while(nID <= nAPCount)
        {
            //获取当前接入点配置文件的全路径
            sFilePath = m_sFmtRootDir + "/";
            sprintf(szTmp, "%d", nID);
            sFilePath += szTmp;
            sFilePath += ".apt";

            //以只读方式打开当前的接入点配置文件
            pAPFile = fopen(sFilePath.c_str(), "rb");
            if(pAPFile != NULL)
            {
                //如果打开当前接入点配置文件成功，表明存在当前接入点配置文件，
                //创建当前接入点对象，并放入接入点对象列表中
                pAP = new CAccessPoint(m_sFmtRootDir, nID, m_pFmtLib);
                m_pAPs->push_back(pAP);

                fclose(pAPFile);
                pAPFile = NULL;
            }

            //如果打开当前接入点配置文件失败，表明不存在当前接入点ID以后的
            //接入点配置文件了，也就是接入点ID必须是从小到大连续递增的
            else
            {
                TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
                      sFilePath.c_str());
                delete m_pFmtLib;
                m_pFmtLib = NULL;

                //删除接入点对象列表中所有接入点对象
                LIST<CAccessPoint*>::iterator it = m_pAPs->begin();
                while(it != m_pAPs->end())
                {
                    delete *it;
                    it++;
                }

                //删除接入点对象列表对象
                delete m_pAPs;
                m_pAPs = NULL;
                
                return -1;
            }

            nID++;
        }
    }

    //只针对一个特定的接入点初始化
    else  //nAPID为非零的接入点ID
    {
        //创建指定的接入点对象，并放入接入点对象列表中
        pAP = new CAccessPoint(m_sFmtRootDir, nAPID, m_pFmtLib);
        m_pAPs->push_back(pAP);
    }

    //初始化接入点对象列表中所有接入点对象
    LIST<CAccessPoint*>::iterator it = m_pAPs->begin();
    while(it != m_pAPs->end())
    {
        if((*it)->Init(m_pFmtLib) != 0)
        {
            //如果初始化当前接入点对象失败
            it = m_pAPs->begin();
            while(it != m_pAPs->end())
            {
                //删除所有接入点对象
                delete *it;
                it++;
            }
            
            //删除接入点对象列表对象
            delete m_pAPs;
            m_pAPs = NULL;

            //删除格式库对象
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
// 功能描述：获得相应的接口指针
//
// 输入参数：
//   iid：需要获得的接口ID，为IID枚举型
//
// 输出参数：无
//
//   返回值：相应的接口指针，调用者通过强制类型转换，将返回的void*
//           转换成对应的处理指针，但不得释放该指针
//
/***************************************************************/
void* CIGWB::QueryInterface(const IID iid)
{
    switch(iid)
    {
    //格式配置接口
    case IID_FP_IFORMATCFG:
        if(m_pIFormatCfgImp == NULL)
        {
            m_pIFormatCfgImp = new IFormatCfgImp(this);
        }
        return m_pIFormatCfgImp;

    //格式处理接口
    case IID_FP_IBILLPROCESSOR:
        if(m_pIBillProcessorImp == NULL)
        {
            m_pIBillProcessorImp = new IBillProcessorImp(this);
        }
        return m_pIBillProcessorImp;

    //查询浏览接口
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
