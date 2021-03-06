#include "../include/toolbox.h"
#include "../include/frame.h"
#include "xml_cdr_stat_rst.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export ICDRStatRst* CreateCDRStatRst()
{
    return new CXmlCDRStatRst();
}

/***********************************************************************************
                          统计结果文件stat_rst_x.xml格式
 -----------------------------------------------------------------------------------
<?xml version="0x00000001" encoding="ISO8859-1" standalone="no" ?>
<CDRStatRst>
  <StatRstHead AccessPoint="16" Reserved="00000000" StatEndTime="2003-12-05" StatExpCount="4" version="0x00000001">
	<StatExpIndexMsg InUsed="1" StatExp="express1" StatExpIndex="1"/>
	<StatExpIndexMsg InUsed="1" StatExp="express2" StatExpIndex="2"/>
	<StatExpIndexMsg InUsed="1" StatExp="express3" StatExpIndex="3"/>
	<StatExpIndexMsg InUsed="1" StatExp="xyz > 2003-01-01 12:00" StatExpIndex="4"/>
  </StatRstHead>

  <StatRstContent ItemStatEndTime="2003-12-05" StatSubItemCount="2">
	<OneStatExpRst CDRCount="4" ItemStatExpIndex="1" ValueTotal="5"/>
	<OneStatExpRst CDRCount="7" ItemStatExpIndex="2" ValueTotal="8"/>
  </StatRstContent>
</CDRStatRst>
 -----------------------------------------------------------------------------------
************************************************************************************/
//定义stat_rst_xxx.xml文件(统计结果)中使用的XML TAG标记
#define XMLTAG_STATRST_ROOT             "CDRStatRst"
#define XMLTAG_STATRST_HEAD             "StatRstHead"
#define XMLTAG_STATRST_ENDTIME          "StatEndTime"
#define XMLTAG_STATRST_RESERVED         "Reserved"
#define XMLTAG_STATRST_EXPCOUNT         "StatExpCount"
#define XMLTAG_STATRST_VERSION          "version"
#define XMLTAG_STATRST_AP               "AccessPoint"
#define XMLTAG_STATRST_EXPINDEXMSG      "StatExpIndexMsg"
#define XMLTAG_STATRST_EXP              "StatExp"
#define XMLTAG_STATRST_INURSED          "InUsed"
#define XMLTAG_STATRST_EXPINDEX         "StatExpIndex"
#define XMLTAG_STATRST_CONTENT          "StatRstContent"
#define XMLTAG_STATRST_STATENDTIME      "ItemStatEndTime"
#define XMLTAG_STATRST_SUBITEMCOUT      "StatSubItemCount"
#define XMLTAG_STATRST_ONESTATEXPRST    "OneStatExpRst"
#define XMLTAG_STATRST_ITEMSTATEXPINDEX "ItemStatExpIndex"
#define XMLTAG_STATRST_CDRCOUNT         "CDRCount"
#define XMLTAG_STATRST_VALUETOTAL       "ValueTotal"

/*******************************************************************
函数名称：CCDRStatRst()
功能描述：构造函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CXmlCDRStatRst::CXmlCDRStatRst()
{
	m_uAccessPoint  = 0;
    m_szFileName[0] = '\0';    
    m_pDoc = NULL;
}

/*******************************************************************
函数名称：~CXmlCDRStatRst()
功能描述：析构函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CXmlCDRStatRst::~CXmlCDRStatRst()
{
}

/*******************************************************************
函数名称：Init(UINT4 uAccessPoint)
功能描述：设置参数，进行初始化操作
输入参数：UINT4 uAccessPoint,接入点号
输出参数：无
返回内容：成功返回ERR_SUCCESS,否则返回ERR_FAIL
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
int CXmlCDRStatRst::Init(UINT4 uAccessPoint)
{
	m_uAccessPoint = uAccessPoint;

    CINIFile iniFile(GetCfgFilePath());
    char szPath[MAX_PATH];
    iniFile.Open();
    iniFile.GetString(CFG_SEC_CDRSTAT, CFG_CDRSTAT_ROOT_DIR, 
                            CFG_CDRSTAT_ROOT_DIR_DEFAULT, szPath, MAX_PATH);

    szPath[MAX_PATH -1] = '\0';

	//从配置文件中读取当前接入点名称
	char szAPSec[25];
    SNPRINTF(szAPSec, 25, "%s%d", CFG_SEC_AP, m_uAccessPoint);
    szAPSec[24] = '\0';

	char szAPName[255];
    iniFile.GetString(szAPSec, CFG_AP_NAME, szAPSec, szAPName, 255);
	m_sAPName = szAPName;

	if (!CreateDeepDir(szPath))
	{
		return ERR_FAIL;
	}

    //配置文件的文件名
    sprintf(m_szFileName, "%s/%s%d%s", szPath, "stat_rst_", m_uAccessPoint, ".xml");

	//判断文件是否存在
	BOOL bFileExist = (access(m_szFileName, 0) == 0);

	//初始化XMLFile,如指定的XML文件不存在，则新建一个内容为空且不含头信息的XML文件
	if (!m_xmlFileRst.InitXmlFile(m_szFileName, XMLTAG_STATRST_ROOT))
	{
		return ERR_FAIL;
	}

	//XML文件是一新建且不含头信息文件
	if (!bFileExist)
	{
		DOMDocument *pDOMDoc = m_xmlFileRst.GetDOMDocument();
		if (pDOMDoc == 0)
		{
            FileDel(m_szFileName);
			return ERR_FAIL;
		}

		//加入头信息
		if (!AddHeadInfo(pDOMDoc->getDocumentElement()))
		{
            //添加头信息失败后删除文件
            FileDel(m_szFileName);
			return ERR_FAIL;
		}
	}

	return ERR_SUCCESS;
}


/*******************************************************************
函数名称：GetStatRstHead(SStatRstHead& statRstHead)
功能描述：获得统计结果文件头信息
输入参数：无
输出参数：SStatRstHead& statRstHead,统计结果结构头信息
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CXmlCDRStatRst::GetStatRstHead(SStatRstHead& statRstHead)
{
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        return;
    }

    const XMLCh* chr = NULL;

    //版本
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_VERSION);
    statRstHead.uVersion = atoi(S(chr));

    //接入点
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_AP);
    statRstHead.uAccessPoint = atoi(S(chr));

    //统计时间        
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_ENDTIME);
    const char* pszTime = XMLString::transcode(chr);
    statRstHead.tStatEndTime = ConvertTimeToTimeT_YMD(pszTime);

    //保留字节
    chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_RESERVED);
    strncpy((char*)statRstHead.yReserved, S(chr), 8);
}


/*******************************************************************
函数名称：GetStatExpList(SStatExpIndexList& statExpIndexList)
功能描述：获得所有统计条件表达式
输入参数：无
输出参数：SStatExpIndexList& statExpIndexList,所有统计条件表达式
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
SStatExpIndexList* CXmlCDRStatRst::GetStatExpList(void)
{    
	SStatExpIndexList* pStatExpIndexList = NULL;

    //获取XML文件中的头信息
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        return NULL;
    }

    //获取条件表达式个数
    const XMLCh* chr = m_xmlFileRst.GetNodeAttrValue(pEleHead, XMLTAG_STATRST_EXPCOUNT);
    int nCount = atoi(S(chr));
    if (nCount == 0)
    {
        //个数为0
		pStatExpIndexList = new SStatExpIndexList;
		pStatExpIndexList->uStatExpCount = 0;
		return pStatExpIndexList;
    }
    else
    {
        //分配指定个数大小的内存
		pStatExpIndexList = new (nCount)SStatExpIndexList;
    }

    //设置条件表达式个数
	pStatExpIndexList->uStatExpCount = nCount;

    //获取XML中条件表达式对象的节点列表
	DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG);
    DOMElement* pEle = NULL;
    if (pNodeList)
    {
        int nLoop = min(nCount, pNodeList->getLength());
        //循环赋值
        for (int i = 0; i < nLoop; i++)
        {
            pEle = (DOMElement*)pNodeList->item(i);
            if (!pEle)
            {
                continue;
            }

            //表达式
            chr = m_xmlFileRst.GetNodeAttrValue(pEle, XMLTAG_STATRST_EXP);
            strncpy(pStatExpIndexList->expIndex[i].szStatExp, S(chr), 512);

            //是否在用
            chr = m_xmlFileRst.GetNodeAttrValue(pEle, XMLTAG_STATRST_INURSED);
            pStatExpIndexList->expIndex[i].bInUsed = (BOOL)(atoi(S(chr)));

            //序号
            chr = m_xmlFileRst.GetNodeAttrValue(pEle, XMLTAG_STATRST_EXPINDEX);
            pStatExpIndexList->expIndex[i].nStatExpIndex = atoi(S(chr));
        }
    }

	return pStatExpIndexList; 
}


/*******************************************************************
函数名称：GetStatExpRst(const char* const szStatExpress, time_t tBeginTime, 
						time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal)
功能描述：获得指定时间范围内统计条件表达式的结果
输入参数：const char* const szStatExpress,表达式 
		  time_t tBeginTime, 开始时间
		  time_t tEndTime, 结束时间
输出参数：UINT4& nCDRCount, 话单张数
          UINT4& nValueTotal,累加值
返回内容：成功返回ERR_SUCCESS,否则返回ERR_FAIL
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
int CXmlCDRStatRst::GetStatExpRst(const char* const szStatExpress, time_t tBeginTime, 
						time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal)
{
    //定义指定表达式对应的序号
	UINT4 nCurrentIndex = 0;
	if (!FindStatExpress((char*)szStatExpress, nCurrentIndex))
	{
		return ERR_FAIL;
	}

    //获取XML文件内容对象的节点列表
    DOMNodeList* pNodeList = GetContentList();
    if (!pNodeList)
	{
		return ERR_FAIL;
	}

    int nListCount = pNodeList->getLength();
    DOMNode* pNode = NULL;
    UINT4 nEndTime = 0;
    const XMLCh* chr = NULL;
    const char* pszTime = NULL;

    //循环每一个结果列表对象
    for (int i = 0; i < nListCount; i++)
	{
        pNode = pNodeList->item(i);
        if (!pNode)
        {
            continue;
        }

        chr = m_xmlFileRst.GetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_STATENDTIME);
        pszTime = XMLString::transcode(chr);
        nEndTime = ConvertTimeToTimeT_YMD(pszTime);

        //判断该节点的时间是否相符
		if (nEndTime >= tBeginTime && nEndTime <= tEndTime)
		{
            //符合条件的节点对象，累加数据
			FindStatRst(pNode, nCurrentIndex, nCDRCount, nValueTotal);
		}
	}

	return ERR_SUCCESS;
}


/*******************************************************************
函数名称：AddStatExpress(LIST<char* >& StatExpList)
功能描述：添加统计条件表达式
输入参数：LIST<char* >& StatExpList,表达式列表
输出参数：无
返回内容：成功返回ERR_SUCCESS,否则返回ERR_FAIL
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
int CXmlCDRStatRst::AddStatExpress(LIST<char* >& StatExpList)
{
	DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return ERR_FAIL;
    }
    m_pDoc = (DOMDocument*)pDoc->cloneNode(true);

    //获取XML文件头信息对象
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        m_pDoc->release();
        return ERR_FAIL;
    }
    
    //目前条件表达式个数
    UINT4 nStatExpCount = 0;
    //最后一个索引号，用于索引增加
    UINT4 nLastExpMsgIndex = 0;
    UINT4 nTemp = 0;
    char chr[64] ;
    
    //获取头信息中目前条件表达式对象列表
    DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG); 
    if (pNodeList)
    {
        //存在表达式，把所有InUsed置为0
        nStatExpCount = pNodeList->getLength();
        DOMNode* pNode = NULL;
        for (int i = 0; i < nStatExpCount; i++)
        {
            pNode = pNodeList->item(i);
            if (!pNode)
            {
                continue;
            }
            
            //置为0
            if (!m_xmlFileRst.SetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_INURSED, "0"))
            {
                RollBack();
                return ERR_FAIL;
            }
                //最后一个一个表达式的索引号
            nTemp = atoi(S(m_xmlFileRst.GetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_EXPINDEX)));
            nLastExpMsgIndex = max(nLastExpMsgIndex, nTemp);
        }
    }
    
    UINT4 nIndex = 0;
    DOMNode* pNode = NULL;
    char chrExp[512];
    
    LIST<char* >::iterator it = StatExpList.begin();
    while(it != StatExpList.end())
    {
        strcpy(chrExp, *it);
        chrExp[511] = '\0';
        pNode = FindStatExpress(*it, nIndex);
        if (pNode)
        {
            //对于原来存在找到的，修改bInUsed为1
            if (!m_xmlFileRst.SetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_INURSED, "1"))
            {
                RollBack();
                return ERR_FAIL;
            }
        }
        else
        {
            //对于原来不存在的，增加一个            
            DOMElement* pElement = m_xmlFileRst.CreateNewElement(pDoc, XMLTAG_STATRST_EXPINDEXMSG);
            if (!pElement)
            {
                RollBack();
                return ERR_FAIL;
            }
            
            //表达式内容
            if (!m_xmlFileRst.SetNodeAttrValue(pElement, XMLTAG_STATRST_EXP, *it))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }
            //是否在用
            if (!m_xmlFileRst.SetNodeAttrValue(pElement, XMLTAG_STATRST_INURSED, "1"))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }
            //序号
            nLastExpMsgIndex++;
            sprintf(chr, "%d", nLastExpMsgIndex);
            if (!m_xmlFileRst.SetNodeAttrValue(pElement, XMLTAG_STATRST_EXPINDEX, chr))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }

            //添加
            if (!m_xmlFileRst.AddChildToNode(pEleHead, pElement))
            {
                pElement->release();
                RollBack();
                return ERR_FAIL;
            }            
        }

        it++;
    }
    
    //改写表达式的总数,重新写进文件    
    UINT4 nCount = 0;
    pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG); 
    if (pNodeList)
    {
        nCount = pNodeList->getLength();
    }
    sprintf(chr, "%d", nCount);
    if (!m_xmlFileRst.SetNodeAttrValue(pEleHead, XMLTAG_STATRST_EXPCOUNT, chr))
    {
        RollBack();
        return ERR_FAIL;
    }
        
    //释放备份资源
    m_pDoc->release();

    //提交到文件中
	if (!m_xmlFileRst.WriteXmlFile())
	{
		return ERR_FAIL;
    }
    
    return ERR_SUCCESS;
}


/*******************************************************************
函数名称：AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime)
功能描述：添加一天的所有统计表达式的统计结果
输入参数：const SAllStatExpRst& statExpRst,一天的统计结果
输出参数：无
返回内容：成功返回ERR_SUCCESS,否则返回ERR_FAIL
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
int CXmlCDRStatRst::AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime)
{
	DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return ERR_FAIL;
    }
	DOMElement* pRootEle = pDoc->getDocumentElement();
	if (pRootEle == 0)
	{
		return ERR_FAIL;
	}
    m_pDoc = (DOMDocument*)pDoc->cloneNode(true);

	//获取头信息对象
	DOMElement* pHeadEle = GetHeadElement();
	if (pHeadEle == 0)
	{
        m_pDoc->release();
		return ERR_FAIL;
	}
	char szBuf[1024];

    STRING strTime = ConvertTimeToString_YMD(statTime);
    //设置统计时间
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadEle, XMLTAG_STATRST_ENDTIME, strTime.c_str()))
	{
        RollBack();
		return ERR_FAIL;
	}
	
	//增加统计内容
    //创建新的统计结果对象
	DOMElement* pNewEle = m_xmlFileRst.CreateNewElement(pDoc, XMLTAG_STATRST_CONTENT);
	if (pNewEle == 0)
	{
        RollBack();
		return ERR_FAIL;
	}
	//统计截止时间
	//sprintf(szBuf, "%ld", statExpRst.uStatEndTime);
    strTime = ConvertTimeToString_YMD(statExpRst.uStatEndTime);
	if (!m_xmlFileRst.SetNodeAttrValue(pNewEle, XMLTAG_STATRST_STATENDTIME, strTime.c_str()))
	{
        RollBack();
		pNewEle->release();
		return ERR_FAIL;
	}
	//后续统计子项数
	UINT4 nSubCnt = statExpRst.uStatSubItemCount;
	//UINT4 nSubCnt = sizeof(statExpRst)/sizeof(SOneStatExpRst);
	sprintf(szBuf, "%ld", nSubCnt);
	if (!m_xmlFileRst.SetNodeAttrValue(pNewEle, XMLTAG_STATRST_SUBITEMCOUT, szBuf))
	{
        RollBack();
		pNewEle->release();
		return ERR_FAIL;
    }
    //取实际数组个数
    //nSubCnt = sizeof(statExpRst.ItemRst)/sizeof(SOneStatExpRst);
    //增加统计子项
    DOMElement* pSubEle = 0;
    for(UINT4 i = 0; i < nSubCnt; i++)
    {
        pSubEle = m_xmlFileRst.CreateNewElement(pDoc, XMLTAG_STATRST_ONESTATEXPRST);
        if (pSubEle == 0)
        {
            RollBack();
            pNewEle->release();
            return ERR_FAIL;
        }
        //统计表达式索引号
        sprintf(szBuf, "%ld", statExpRst.ItemRst[i].nStatExpIndex);
        if (!m_xmlFileRst.SetNodeAttrValue(pSubEle, XMLTAG_STATRST_ITEMSTATEXPINDEX, szBuf))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
        //话单张数
        sprintf(szBuf, "%ld", statExpRst.ItemRst[i].nCDRCount);
        if (!m_xmlFileRst.SetNodeAttrValue(pSubEle, XMLTAG_STATRST_CDRCOUNT, szBuf))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
        //域的累计值
        sprintf(szBuf, "%ld", statExpRst.ItemRst[i].nValueTotal);
        if (!m_xmlFileRst.SetNodeAttrValue(pSubEle, XMLTAG_STATRST_VALUETOTAL, szBuf))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
        
        //增加这个统计子节点
        if (!m_xmlFileRst.AddChildToNode(pNewEle, pSubEle))
        {
            RollBack();
            pSubEle->release();
            pNewEle->release();
            return ERR_FAIL;
        }
    }
    
    //增加这个统计节点
    if (!m_xmlFileRst.AddChildToNode(pRootEle, pNewEle))
    {
        RollBack();
        pNewEle->release();
		return ERR_FAIL;
	}

    //释放备份资源
    m_pDoc->release();

    //写入到XML文件
	if (!m_xmlFileRst.WriteXmlFile())
	{
		return ERR_FAIL;
	}
	
	return ERR_SUCCESS;
}

/*******************************************************************
函数名称：CleanStatExp(void)
功能描述：清理统计结果文件中不再使用的条件表达式
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
int CXmlCDRStatRst::CleanStatExp(void)
{
    //该函数目前没有应用
	return ERR_SUCCESS;
}

/*******************************************************************
函数名称：FindStatExpress(char* szExpress, UINT4& nIndex)
功能描述：根据表达式查找其序号
输入参数：char* szExpress, 待找的表达式
输出参数：UINT4& nIndex,表达式序号值
返回内容：找到返回NODE对象,否则返回NULL
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
DOMNode* CXmlCDRStatRst::FindStatExpress(char* szExpress, UINT4& nIndex)
{
    BOOL bReturn = FALSE;
	SStatExpIndexList* pStatExpIndexList = NULL;

    //获取头节点
    DOMElement* pEleHead = GetHeadElement();
    if (!pEleHead)
    {
        return NULL;
    }

    //获取条件表达式列表
    DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEleHead, XMLTAG_STATRST_EXPINDEXMSG); 
    if (!pNodeList)
    {
        return NULL;
    }

    int nListCount = pNodeList->getLength();
    const XMLCh* pCh = NULL;
    DOMNode* pNode = NULL;
    //循环查找每一个条件表达式
    for (int i = 0; i < nListCount; i++)
    {
        pNode = pNodeList->item(i);
        pCh = m_xmlFileRst.GetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_EXP);
        if (strcmp(S(pCh), szExpress) == 0)
        {
            nIndex = atoi(S(m_xmlFileRst.GetNodeAttrValue((DOMElement*)pNode, XMLTAG_STATRST_EXPINDEX)));
            return pNode;
        }
    }

    return NULL;
}

/*******************************************************************
函数名称：RollBack(void)
功能描述：如果写失败，回滚数据
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
BOOL CXmlCDRStatRst::RollBack(void)
{    
    if (!m_pDoc)
    {
        return FALSE;
    }
	
	return m_xmlFileRst.SetDOMDocument(m_pDoc);
}


/*******************************************************************
函数名称：FindStatRst(SAllStatExpRst* pAllRst, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal)
功能描述：在所有的统计结果中根据索引号查找其统计结果值
输入参数：SAllStatExpRst* pAllRst, 所有统计结果结构
          UINT4 nIndex, 索引号
输出参数：UINT4& nCDRCount, 话单张数
          UINT4& nValueTotal,累加值
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CXmlCDRStatRst::FindStatRst(DOMNode* pNode, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal)
{
	if (pNode == 0)
	{
		return;
	}
	
    //获取结果列表
	DOMNodeList* pDOMChilds = m_xmlFileRst.GetChildListByTagName((DOMElement*)pNode, XMLTAG_STATRST_ONESTATEXPRST);	
	if (!pDOMChilds)
	{
		return;
	}
    
	DOMElement* pChild = NULL;
    const XMLCh* chExpIndex = NULL;

    int nCount = pDOMChilds->getLength();
    //循环查找每一个结果对象
	for(int i = 0; i < nCount; i++)
	{
		pChild = (DOMElement*)pDOMChilds->item(i);
		if (!pChild)
		{
            continue;
        }
        
        chExpIndex = m_xmlFileRst.GetNodeAttrValue(pChild,XMLTAG_STATRST_ITEMSTATEXPINDEX);
        //序号相同，找到对应结果记录
        if (atoi(S(chExpIndex)) == nIndex)
        {
            nCDRCount += atoi(S(m_xmlFileRst.GetNodeAttrValue(pChild, XMLTAG_STATRST_CDRCOUNT)));
            nValueTotal += atoi(S(m_xmlFileRst.GetNodeAttrValue(pChild, XMLTAG_STATRST_VALUETOTAL)));
            return;
        }
    }	
}



/*******************************************************************
函数名称：GetAccessPoint()
功能描述：返回接入点号
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
UINT4 CXmlCDRStatRst::GetAccessPoint()
{
	return m_uAccessPoint;
}


/*******************************************************************
函数名称：GetAccessPointName()
功能描述：返回接入点名称
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
const char* CXmlCDRStatRst::GetAccessPointName()
{
	return m_sAPName.c_str();
}

/*******************************************************************
函数名称：GetAllExpress(LIST<STRING>& ExpressList)
功能描述：根据接入点号返回所有表达式
输入参数：无
输出参数：LIST<STRING>& ExpressList,获得所有表达式列表
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void  CXmlCDRStatRst::GetAllExpress(LIST<STRING>& ExpressList)
{
	SStatExpIndexList* pExpList = NULL;
	pExpList = GetStatExpList();

    if (!pExpList)
    {
        return;
    }
	for( UINT4 i = 0; i < pExpList->uStatExpCount; i++)
	{
        if((pExpList->expIndex[i]).bInUsed)
        {
            ExpressList.push_back((pExpList->expIndex[i]).szStatExp);
        }
	}
	delete pExpList;
}

/*******************************************************************
函数名称：GetLastStatTime()
功能描述：获得上次统计时间
输入参数：无
输出参数：无
返回内容：上次统计时间
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
time_t CXmlCDRStatRst::GetLastStatTime()
{
    //获取头信息对象
	DOMElement* pHeadEle = GetHeadElement();
	if (!pHeadEle)
	{
	    return 0;
	}
	else
	{
        //返回上次统计时间
        const XMLCh* chr = m_xmlFileRst.GetNodeAttrValue(pHeadEle, XMLTAG_STATRST_ENDTIME);
        const char* pszTime = XMLString::transcode(chr);
		return ConvertTimeToTimeT_YMD(pszTime);
	}
}

/*******************************************************************
函数名称：AddHeadInfo(DOMElement *pRootEle)
功能描述：加入文件头信息到XML文件中
输入参数：pRootEle，DOM文件对象
输出参数：无
返回内容：DOMElement添加成功返回头信息对象，否则返回NULL
调用函数：
被调函数：
作者时间： 
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
DOMElement * CXmlCDRStatRst::AddHeadInfo(DOMElement *pRootEle)
{
	if (pRootEle == 0)
	{
		return NULL;
	}
	DOMDocument* pDOMDoc = m_xmlFileRst.GetDOMDocument();
	if (pDOMDoc == 0)
	{
		return NULL;
	}
    m_pDoc = (DOMDocument*)pDOMDoc->cloneNode(true);

	//生成头结点
	DOMElement* pHeadNode = m_xmlFileRst.CreateNewElement(pDOMDoc, XMLTAG_STATRST_HEAD);
	if (pHeadNode == 0)
	{
        m_pDoc->release();
		return NULL;
	}
	//在头结点中加入属性
	//版本属性
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_VERSION, "0x00000001"))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
	//接入点号
	char szTmp[256];
	sprintf(szTmp, "%d", m_uAccessPoint);
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_AP, szTmp))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
    //统计截止时间
    time_t curtime = ACE_OS::time(NULL);
    STRING strTime = ConvertTimeToString_YMD(curtime);
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_ENDTIME, strTime.c_str()))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
	//保留字节
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_RESERVED, "00000000"))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}
    //统计表达式个数
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadNode, XMLTAG_STATRST_EXPCOUNT, "0"))
	{
        RollBack();
        pHeadNode->release();
		return NULL; 
	}

	//加入头结点到根结点下
	if (!m_xmlFileRst.AddChildToNode(pRootEle, pHeadNode))
	{
        RollBack();
		pHeadNode->release();
		return NULL; 
	}

    //提交到文件
    if (!m_xmlFileRst.WriteXmlFile())
    {
		pHeadNode->release();
		return NULL; 
    }
	return pHeadNode;
}

/*******************************************************************
函数名称：GetHeadElement()
功能描述：获取头节点
输入参数：无
输出参数：无
返回内容：DOMElement获取成功返回头信息对象，否则返回NULL
调用函数：
被调函数：
作者时间： 
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
DOMElement* CXmlCDRStatRst::GetHeadElement()
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return NULL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pEle = pDoc->getDocumentElement();
    if (!pEle)
    {
        return NULL;
    }

    //按头信息关键字查找
    DOMNodeList* pNodeList = m_xmlFileRst.GetChildListByTagName(pEle, XMLTAG_STATRST_HEAD);
    if (pNodeList && pNodeList->getLength() > 0)
    {
        return (DOMElement*)pNodeList->item(0);
    }

    return NULL;
}

/*******************************************************************
函数名称：GetContentList()
功能描述：获取文件内容列表
输入参数：无
输出参数：无
返回内容：DOMNodeList获取成功返回文件内容列表指针，否则返回NULL
调用函数：
被调函数：
作者时间： 
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
DOMNodeList* CXmlCDRStatRst::GetContentList()
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return NULL;
    }
    DOMElement* pEle = pDoc->getDocumentElement();
    if (!pEle)
    {
        return NULL;
    }

    //返回文件内容列表
    return m_xmlFileRst.GetChildListByTagName(pEle, XMLTAG_STATRST_CONTENT);
}

//刷新已执行统计操作的时间
int CXmlCDRStatRst::SetLastStatTime(time_t curtime)
{
	DOMDocument* pDoc = m_xmlFileRst.GetDOMDocument();
    if (!pDoc)
    {
        return ERR_FAIL;
    }
	DOMElement* pRootEle = pDoc->getDocumentElement();
	if (pRootEle == 0)
	{
		return ERR_FAIL;
	}
    m_pDoc = (DOMDocument*)pDoc->cloneNode(true);

	//获取头信息对象
	DOMElement* pHeadEle = GetHeadElement();
	if (pHeadEle == 0)
	{
        m_pDoc->release();
		return ERR_FAIL;
	}

    STRING strTime = ConvertTimeToString_YMD(curtime);
    //设置统计时间
	if (!m_xmlFileRst.SetNodeAttrValue(pHeadEle, XMLTAG_STATRST_ENDTIME, strTime.c_str()))
	{
        RollBack();
		return ERR_FAIL;
	}
	
    //释放备份资源
    m_pDoc->release();

    //写入到XML文件
	if (!m_xmlFileRst.WriteXmlFile())
	{
		return ERR_FAIL;
	}

    return ERR_SUCCESS;
}
