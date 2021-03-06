#include "../include/toolbox.h"
#include "alarm_xml_util.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export IAlarmUtil* CreateAlarmUtil()
{
    return new CAlarmXmlUtil();
}

CAlarmXmlUtil::CAlarmXmlUtil()
{
    //
}

CAlarmXmlUtil::~CAlarmXmlUtil()
{

}

/***********************************************************************************
                          历史告警文件alarmhistory.xml格式
 -----------------------------------------------------------------------------------
<?xml version="1.0" encoding="ISO8859-1" standalone="no" ?>
<AlarmHistory>
  <Head CSN="11" SYN="11" BASEID="11"/>
  <Comment CSN="10" SYN="10" ModuleNO="1" AlarmID="2" AlarmType="3" FuncID="4" AlarmLevel="5" SubSysID="6" SDate="2002-01-01 12:12:12" CPUPos="1" PID="7" Para="010BAB" ResType="8" RDate="2002-01-02 12:12:12" FAckOptr="A" FAckDate="2002-01-03 12:12:12" RAckOptr="B" RAckDate="2002-01-04 12:12:12"/>
  <Comment CSN="11" SYN="11" ModuleNO="1" AlarmID="2" AlarmType="3" FuncID="4" AlarmLevel="5" SubSysID="6" SDate="2003-01-01 12:12:12" CPUPos="1" PID="7" Para="010BAB" ResType="8" RDate="2003-01-02 12:12:12" FAckOptr="A" FAckDate="2003-01-03 12:12:12" RAckOptr="B" RAckDate="2003-01-04 12:12:12"/>
</AlarmHistory>
 -----------------------------------------------------------------------------------

***********************************************************************************/
#define XMLTAG_ALARM_ALARMHISTORY           "AlarmHistory"
#define XMLTAG_ALARM_HEAD                   "Head"
#define XMLTAG_ALARM_HEAD_CSN               "CSN"
#define XMLTAG_ALARM_HEAD_SYN               "SYN"
#define XMLTAG_ALARM_HEAD_BASEID            "BASEID"
#define XMLTAG_ALARM_CONTENT                "Comment"
#define XMLTAG_ALARM_CONTENT_CSN            "CSN"
#define XMLTAG_ALARM_CONTENT_SYN            "SYN"
#define XMLTAG_ALARM_CONTENT_MODULENO       "ModuleNO"
#define XMLTAG_ALARM_CONTENT_ALARMID        "AlarmID"
#define XMLTAG_ALARM_CONTENT_ALARMTYPE      "AlarmType"
#define XMLTAG_ALARM_CONTENT_FUNCID         "FuncID"
#define XMLTAG_ALARM_CONTENT_ALARMLEVEL     "AlarmLevel"
#define XMLTAG_ALARM_CONTENT_SUBSYSID       "SubSysID"
#define XMLTAG_ALARM_CONTENT_SDATE          "SDate"
#define XMLTAG_ALARM_CONTENT_CPUPOS         "CPUPos"
#define XMLTAG_ALARM_CONTENT_PID            "PID"
#define XMLTAG_ALARM_CONTENT_PARA           "Para"
#define XMLTAG_ALARM_CONTENT_RESTYPE        "ResType"
#define XMLTAG_ALARM_CONTENT_RDATE          "RDate"
#define XMLTAG_ALARM_CONTENT_FACKOPTR       "FAckOptr"
#define XMLTAG_ALARM_CONTENT_FACKDATE       "FAckDate"
#define XMLTAG_ALARM_CONTENT_RACKOPTR       "RAckOptr"
#define XMLTAG_ALARM_CONTENT_RACKDATE       "RAckDate"

//剖析并生成DOM文档树，文件不存在则生成指定文件
int CAlarmXmlUtil::OpenFile(const char * const szFilePath)
{
    if(szFilePath != NULL)
    {
        //初始化xml文件，并建立相应的DOM结点树
        BOOL bRet = m_xmlFileAlarm.InitXmlFile(szFilePath, XMLTAG_ALARM_ALARMHISTORY);

        return bRet ? 0 : -1;
    }

    return -1;
}

//关闭文件
int CAlarmXmlUtil::CloseFile(void)
{
    return m_xmlFileAlarm.ClearDOMDocument() ? 0 : -1;
}

//获得剩余的告警数
int CAlarmXmlUtil::GetCount(void)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    DOMNodeList *pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle,XMLTAG_ALARM_CONTENT);
    if (NULL == pNodeList)
    {
        return 0;
    }

    int nCount = pNodeList->getLength();

    return nCount;
}

//保存文件
int CAlarmXmlUtil::Flush(void)
{
    //写xml文件
    if (!m_xmlFileAlarm.WriteXmlFile())
    {
        return -1;
    }

    return 0;
}

//删除某条告警记录
int CAlarmXmlUtil::Delete(int nIndex)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    DOMNodeList *pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle,XMLTAG_ALARM_CONTENT);
    if (NULL == pNodeList)
    {
        return 0;
    }

    DOMNode* pNode = pNodeList->item(nIndex);
    //删除记录
    if (!m_xmlFileAlarm.DeleteNode(pNode))
    {
        //删除失败
        return ERR_FAIL;
    }

    return 0;
}

//获得文件头记录
int CAlarmXmlUtil::ReadHeadElement(UINT4& uCSN)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //按头信息关键字查找
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);
        const XMLCh* chr = NULL;
        chr = m_xmlFileAlarm.GetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_CSN);
        if (!chr)
        {
            return ERR_FAIL;
        }
        uCSN = atoi(S(chr));
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

//添加/写文件头记录
int CAlarmXmlUtil::WriteHeadElement(UINT4 uCSN)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //按头信息关键字查找
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (NULL == pNodeList || pNodeList->getLength() == 0)
    {
        //生成头结点
        DOMElement* pHeadNode = m_xmlFileAlarm.CreateNewElement(pDoc, XMLTAG_ALARM_HEAD);
        if (pHeadNode == NULL)
        {
            return ERR_FAIL;
        }
        //在头结点中加入属性
        if (!m_xmlFileAlarm.SetNodeAttrValue(pHeadNode, XMLTAG_ALARM_HEAD_CSN, "99"))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }

        //添加头节点
        if (!m_xmlFileAlarm.AddChildToNode(pRootEle, pHeadNode))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }
        pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    }

    if (NULL != pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);

        //设置新告警流水号
        char szBuf[1024];
        sprintf(szBuf, "%ld", uCSN);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_CSN, szBuf))
        {
            return ERR_FAIL;
        }

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

//写一条日志信息
int CAlarmXmlUtil::SaveHisAlarm(int            nIndex,
                                SHistoryAlarm& hisAlarm)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //返回文件内容列表
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_CONTENT);
    if (pNodeList == NULL)
    {
        return ERR_FAIL;
    }

    DOMElement* pEleAlarm = NULL;
    if (nIndex > pNodeList->getLength())
    {
        return ERR_FAIL;
    }
    else if (nIndex == pNodeList->getLength())
    {
        //创建新的记录对象
        pEleAlarm = m_xmlFileAlarm.CreateNewElement(pDoc, XMLTAG_ALARM_CONTENT);
        if (pEleAlarm == NULL)
        {
            return ERR_FAIL;
        }
        //添加节点
        if (!m_xmlFileAlarm.AddChildToNode(pRootEle, pEleAlarm))
        {
            pEleAlarm->release();
            return ERR_FAIL;
        }
    }
    else
    {
        pEleAlarm = (DOMElement*)pNodeList->item(nIndex);
    }

    if(NULL != pEleAlarm)
    {
        char szBuf[1024];
        char szDate[20];

        //开始设置值
        //告警流水号,顺序分配,恢复告警与其对应的故障告警uCsn相同
        sprintf(szBuf, "%ld", hisAlarm.uCsn);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_CSN, szBuf))
        {
            return ERR_FAIL;
        }

        //告警同步号
        sprintf(szBuf, "%ld", hisAlarm.uSyn);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SYN, szBuf))
        {
            return ERR_FAIL;
        }

        //发送告警的主机模块号,对于iGateway bill,主为0,备为1
        sprintf(szBuf, "%ld", hisAlarm.yModuleNO);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_MODULENO, szBuf))
        {
            return ERR_FAIL;
        }

        //告警编号
        sprintf(szBuf, "%ld", hisAlarm.uAlarmID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMID, szBuf))
        {
            return ERR_FAIL;
        }

        //告警类型,ALARM_TYPE枚举型
        sprintf(szBuf, "%ld", hisAlarm.yAlarmType);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMTYPE, szBuf))
        {
            return ERR_FAIL;
        }

        //告警箱告警分类,ALARM_FUNCTION_ID枚举型
        sprintf(szBuf, "%ld", hisAlarm.yFuncID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FUNCID, szBuf))
        {
            return ERR_FAIL;
        }

        //告警级别,ALARM_LEVEL枚举型
        sprintf(szBuf, "%ld", hisAlarm.yAlarmLevel);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMLEVEL, szBuf))
        {
            return ERR_FAIL;
        }

        //网管告警分类,ALARM_SUBSYSTEM枚举型
        sprintf(szBuf, "%ld", hisAlarm.ySubSysID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SUBSYSID, szBuf))
        {
            return ERR_FAIL;
        }

        //告警产生的日期,时间
        szDate[0] = '\0';
        if (hisAlarm.uSYear + hisAlarm.ySMonth + hisAlarm.ySDay
            + hisAlarm.ySHour + hisAlarm.ySMin + hisAlarm.ySSec != 0)
        {
            sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
                hisAlarm.uSYear, hisAlarm.ySMonth, hisAlarm.ySDay
                , hisAlarm.ySHour, hisAlarm.ySMin, hisAlarm.ySSec);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SDATE, szDate))
        {
            return ERR_FAIL;
        }

        //A(0),B(1)机指示,对于iGateway bill,主为0,备为1
        sprintf(szBuf, "%ld", hisAlarm.yCPUPos);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_CPUPOS, szBuf))
        {
            return ERR_FAIL;
        }

        //告警产生的进程标识
        sprintf(szBuf, "%ld", hisAlarm.yPID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_PID, szBuf))
        {
            return ERR_FAIL;
        }

        //告警参数
        for (int nLoop = 0; nLoop < MAX_ALARM_PARA; nLoop++)
        {
            sprintf(szBuf + nLoop*2, "%02X", hisAlarm.yPara[nLoop]);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_PARA, szBuf))
        {
            return ERR_FAIL;
        }

        //告警恢复标志,ALARM_RECOVER_TYPE枚举型
        sprintf(szBuf, "%ld", hisAlarm.yResType);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RESTYPE, szBuf))
        {
            return ERR_FAIL;
        }

        //告警恢复的日期,时间
        szDate[0] = '\0';
        if (hisAlarm.uRYear + hisAlarm.yRMonth + hisAlarm.yRDay
            + hisAlarm.yRHour + hisAlarm.yRMin + hisAlarm.yRSec != 0)
        {
            sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
                hisAlarm.uRYear, hisAlarm.yRMonth, hisAlarm.yRDay
                , hisAlarm.yRHour, hisAlarm.yRMin, hisAlarm.yRSec);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RDATE, szDate))
        {
            return ERR_FAIL;
        }

        //告警确认的操作员,日期,时间
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FACKOPTR, hisAlarm.szFAckOptr))
        {
            return ERR_FAIL;
        }

        szDate[0] = '\0';
        if (hisAlarm.uFAckYear + hisAlarm.yFAckMonth + hisAlarm.yFAckDay
            + hisAlarm.yFAckHour + hisAlarm.yFAckMin + hisAlarm.yFAckSec != 0)
        {
            sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
                hisAlarm.uFAckYear, hisAlarm.yFAckMonth, hisAlarm.yFAckDay
                , hisAlarm.yFAckHour, hisAlarm.yFAckMin, hisAlarm.yFAckSec);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FACKDATE, szDate))
        {
            return ERR_FAIL;
        }

        //告警恢复确认的操作员,日期,时间
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RACKOPTR, hisAlarm.szRAckOptr))
        {
            return ERR_FAIL;
        }

        szDate[0] = '\0';
        if (hisAlarm.uRAckYear + hisAlarm.yRAckMonth + hisAlarm.yRAckDay
            + hisAlarm.yRAckHour + hisAlarm.yRAckMin + hisAlarm.yRAckSec != 0)
        {
            sprintf(szDate,"%04d-%02d-%02d %02d:%02d:%02d",
                hisAlarm.uRAckYear, hisAlarm.yRAckMonth, hisAlarm.yRAckDay
                , hisAlarm.yRAckHour, hisAlarm.yRAckMin, hisAlarm.yRAckSec);
        }
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RACKDATE, szDate))
        {
            return ERR_FAIL;
        }

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

//读一条告警信息
int CAlarmXmlUtil::ReadOneAlarm(int            nIndex,
                                SHistoryAlarm& hisAlarm)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //返回文件内容列表
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_CONTENT);
    if (pNodeList == NULL)
    {
        return ERR_FAIL;
    }

    if (nIndex >= pNodeList->getLength())
    {
        return ERR_FAIL;
    }

    DOMElement* pEleAlarm = (DOMElement*)pNodeList->item(nIndex);
    if(NULL != pEleAlarm)
    {
        const XMLCh* pChr = NULL;
        UINT2 uYear = 0;        //4位年
        BYTE yMonth = 0;        //2位月
        BYTE yDay   = 0;        //2位日
        BYTE yHour  = 0;        //2位时
        BYTE yMin   = 0;        //2位分
        BYTE ySec   = 0;        //2位秒

        //开始获取数据

        //告警流水号,顺序分配,恢复告警与其对应的故障告警uCsn相同
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_CSN);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.uCsn = atoi(S(pChr));

        //告警同步号
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SYN);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.uSyn = atoi(S(pChr));

        //发送告警的主机模块号,对于iGateway bill,主为0,备为1
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_MODULENO);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yModuleNO = atoi(S(pChr));

        //告警编号
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMID);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.uAlarmID = atoi(S(pChr));

        //告警类型,ALARM_TYPE枚举型
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMTYPE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yAlarmType = atoi(S(pChr));

        //告警箱告警分类,ALARM_FUNCTION_ID枚举型
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FUNCID);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yFuncID = atoi(S(pChr));

        //告警级别,ALARM_LEVEL枚举型
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_ALARMLEVEL);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yAlarmLevel = atoi(S(pChr));

        //网管告警分类,ALARM_SUBSYSTEM枚举型
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SUBSYSID);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.ySubSysID = atoi(S(pChr));

        //告警产生的日期,时间
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_SDATE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        //格式转换
        ProcessDate(pChr, uYear, yMonth, yDay, yHour, yMin, ySec);
        hisAlarm.uSYear  = uYear;
        hisAlarm.ySMonth = yMonth;
        hisAlarm.ySDay   = yDay;
        hisAlarm.ySHour  = yHour;
        hisAlarm.ySMin   = yMin;
        hisAlarm.ySSec   = ySec;

        //A(0),B(1)机指示,对于iGateway bill,主为0,备为1
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_CPUPOS);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yCPUPos = atoi(S(pChr));

        //告警产生的进程标识
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_PID);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yPID = atoi(S(pChr));

        //告警参数
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_PARA);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        BYTE byValue = 0;
        char chr[3];
        for (int nLoop = 0, nCount = 0; nLoop < MAX_ALARM_PARA; nLoop++)
        {
            memcpy(chr, S(pChr) + nCount, 2);
            if (chr[0] == '\0' || chr[1] == '\0')
            {
                break;
            }
            chr[2] = '\0';
            //获取一个BYTE的数值
            byValue = GetHexCharValue(chr);
            hisAlarm.yPara[nLoop] = byValue;
            //每次处理2个字符，即一个BYTE
            nCount += 2;
        }

        //告警恢复标志,ALARM_RECOVER_TYPE枚举型
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RESTYPE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        hisAlarm.yResType = atoi(S(pChr));

        //告警恢复的日期,时间
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RDATE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        //格式转换
        ProcessDate(pChr, uYear, yMonth, yDay, yHour, yMin, ySec);
        hisAlarm.uRYear  = uYear;
        hisAlarm.yRMonth = yMonth;
        hisAlarm.yRDay   = yDay;
        hisAlarm.yRHour  = yHour;
        hisAlarm.yRMin   = yMin;
        hisAlarm.yRSec   = ySec;

        //告警确认的操作员,日期,时间
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FACKOPTR);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        memcpy(hisAlarm.szFAckOptr, S(pChr), 78);
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_FACKDATE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        //格式转换
        ProcessDate(pChr, uYear, yMonth, yDay, yHour, yMin, ySec);
        hisAlarm.uFAckYear  = uYear;
        hisAlarm.yFAckMonth = yMonth;
        hisAlarm.yFAckDay   = yDay;
        hisAlarm.yFAckHour  = yHour;
        hisAlarm.yFAckMin   = yMin;
        hisAlarm.yFAckSec   = ySec;

        //告警恢复确认的操作员,日期,时间
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RACKOPTR);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        memcpy(hisAlarm.szRAckOptr, S(pChr), 78);
        pChr = m_xmlFileAlarm.GetNodeAttrValue(pEleAlarm, XMLTAG_ALARM_CONTENT_RACKDATE);
        if (!pChr)
        {
            return ERR_FAIL;
        }
        //格式转换
        ProcessDate(pChr, uYear, yMonth, yDay, yHour, yMin, ySec);
        hisAlarm.uRAckYear  = uYear;
        hisAlarm.yRAckMonth = yMonth;
        hisAlarm.yRAckDay   = yDay;
        hisAlarm.yRAckHour  = yHour;
        hisAlarm.yRAckMin   = yMin;
        hisAlarm.yRAckSec   = ySec;

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

/*******************************************************************
函数名称：ProcessDate(const XMLCh* pChr, UINT2& uYear, BYTE& yMonth, BYTE& yDay
                                , BYTE& yHour, BYTE& yMin, BYTE& ySec);
功能描述：处理年月日时分秒格式
输入参数：pChr待处理的日期字符串
输出参数：uYear年，yMonth月，yDay日，yHour时，yMin分，ySec秒
返回内容：无
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CAlarmXmlUtil::ProcessDate(const XMLCh* pChr,
                                UINT2& uYear,
                                BYTE& yMonth,
                                BYTE& yDay,
                                BYTE& yHour,
                                BYTE& yMin,
                                BYTE& ySec)
{
    if (pChr == NULL || pChr[0] == '\0')    //空串
    {
        uYear = 0;
        yMonth = 0;
        yDay = 0;
        yHour = 0;
        yMin = 0;
        ySec = 0;
    }
    else    //按照 yyyy-mm-dd hh:mm:ss格式分解
    {
        char pTemp[5];
        memcpy(pTemp, S(pChr), 4);
        pTemp[4] = '\0';
        uYear = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 5, 2);
        pTemp[2] = '\0';
        yMonth = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 8, 2);
        pTemp[2] = '\0';
        yDay = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 11, 2);
        pTemp[2] = '\0';
        yHour = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 14, 2);
        pTemp[2] = '\0';
        yMin = atoi(pTemp);

        memcpy(pTemp, S(pChr) + 17, 2);
        pTemp[2] = '\0';
        ySec = atoi(pTemp);
    }
}

/*******************************************************************
函数名称：GetHexCharValue(char chr[2])
功能描述：转换一个十六进制的字符串为数值，如把"AC"转换成172
输入参数：chr待处理的十六进制字符串
输出参数：无
返回内容：转换后的数值
调用函数：
被调函数：
作者时间：
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BYTE CAlarmXmlUtil::GetHexCharValue(char chr[2])
{
    BYTE byValue = 0;
    int nValueTemp = 0;
    for(int i = 0, j = 1; i < 2; i++, j--)
    {
        switch(chr[i])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                nValueTemp = chr[i] - '0';
                break;
            }
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            {
                nValueTemp = chr[i] - 'A' + 10;
                break;
            }
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            {
                nValueTemp = chr[i] - 'a' + 10;
                break;
            }
        default://非十六进制字符
            {
                return 255;
            }
        }

        if (j == 1)
        {
            byValue += nValueTemp*16;
        }
        else
        {
            byValue += nValueTemp;
        }
    }

    return byValue;
}

int CAlarmXmlUtil::ReadHeadElementSyn(UINT4& uSYN)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //按头信息关键字查找
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);
        const XMLCh* chr = NULL;
        chr = m_xmlFileAlarm.GetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_SYN);
        if (!chr)
        {
            return ERR_FAIL;
        }
        uSYN = atoi(S(chr));
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

int CAlarmXmlUtil::ReadHeadElementMMLBaseID(UINT4& uMMLBaseID)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //按头信息关键字查找
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);
        const XMLCh* chr = NULL;
        chr = m_xmlFileAlarm.GetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_BASEID);
        if (!chr)
        {
            return ERR_FAIL;
        }
        uMMLBaseID = atoi(S(chr));
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

int CAlarmXmlUtil::WriteHeadElementSyn(UINT4 uSYN)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //按头信息关键字查找
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (NULL == pNodeList || pNodeList->getLength() == 0)
    {
        //生成头结点
        DOMElement* pHeadNode = m_xmlFileAlarm.CreateNewElement(pDoc, XMLTAG_ALARM_HEAD);
        if (pHeadNode == NULL)
        {
            return ERR_FAIL;
        }
        //在头结点中加入属性
        if (!m_xmlFileAlarm.SetNodeAttrValue(pHeadNode, XMLTAG_ALARM_HEAD_SYN, "0"))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }

        //添加头节点
        if (!m_xmlFileAlarm.AddChildToNode(pRootEle, pHeadNode))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }
        pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    }

    if (NULL != pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);

        //设置新告警流水号
        char szBuf[1024];
        sprintf(szBuf, "%ld", uSYN);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_SYN, szBuf))
        {
            return ERR_FAIL;
        }

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

int CAlarmXmlUtil::WriteHeadElementMMLBaseID(UINT4 uBaseID)
{
    //获取DOC对象
    DOMDocument* pDoc = m_xmlFileAlarm.GetDOMDocument();
    if (NULL == pDoc)
    {
        return ERR_FAIL;
    }
    //获取XML文件的顶层元素对象
    DOMElement* pRootEle = pDoc->getDocumentElement();
    if (NULL == pRootEle)
    {
        return ERR_FAIL;
    }

    //按头信息关键字查找
    DOMNodeList* pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    if (NULL == pNodeList || pNodeList->getLength() == 0)
    {
        //生成头结点
        DOMElement* pHeadNode = m_xmlFileAlarm.CreateNewElement(pDoc, XMLTAG_ALARM_HEAD);
        if (pHeadNode == NULL)
        {
            return ERR_FAIL;
        }
        //在头结点中加入属性
        if (!m_xmlFileAlarm.SetNodeAttrValue(pHeadNode, XMLTAG_ALARM_HEAD_BASEID, "0"))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }

        //添加头节点
        if (!m_xmlFileAlarm.AddChildToNode(pRootEle, pHeadNode))
        {
            pHeadNode->release();
            return ERR_FAIL;
        }
        pNodeList = m_xmlFileAlarm.GetChildListByTagName(pRootEle, XMLTAG_ALARM_HEAD);
    }

    if (NULL != pNodeList && pNodeList->getLength() > 0)
    {
        DOMElement* pEleHead = (DOMElement*)pNodeList->item(0);

        //设置新告警流水号
        char szBuf[1024];
        sprintf(szBuf, "%ld", uBaseID);
        if (!m_xmlFileAlarm.SetNodeAttrValue(pEleHead, XMLTAG_ALARM_HEAD_BASEID, szBuf))
        {
            return ERR_FAIL;
        }

        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}

