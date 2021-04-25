#include "../include/toolbox.h"
#include "../include/frame.h"
#include "cdr_stat_rst.h"


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
CCDRStatRst::CCDRStatRst()
{
    m_uAccessPoint  = 0;
    m_nMemDataLen   = 0;
    m_pMemData      = NULL;
    m_szFileName[0] = '\0';
}

/*******************************************************************
函数名称：~CCDRStatRst()
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
CCDRStatRst::~CCDRStatRst()
{
     if (m_pMemData != NULL)
    {
        delete[] m_pMemData;
        m_pMemData = NULL;
    }
    if (m_pHead != NULL)
    {
        delete[] m_pHead;
        m_pHead = NULL;
    }

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
int CCDRStatRst::Init(UINT4 uAccessPoint)
{
    m_uAccessPoint = uAccessPoint;

    CINIFile iniFile(GetCfgFilePath());
    char szPath[MAX_PATH];
    iniFile.Open();
    iniFile.GetString(CFG_SEC_CDRSTAT,
                      CFG_CDRSTAT_ROOT_DIR,
                      CFG_CDRSTAT_ROOT_DIR_DEFAULT,
                      szPath,
                      sizeof(szPath));
    szPath[sizeof(szPath) - 1] = '\0';

    //从配置文件中读取当前接入点名称
    char szAPSec[25];
    SNPRINTF(szAPSec,
             sizeof(szAPSec),
             "%s%d",
             CFG_SEC_AP,
             m_uAccessPoint);
    szAPSec[sizeof(szAPSec) - 1] = '\0';

    char szAPName[25];
    iniFile.GetString(szAPSec,
                      CFG_AP_NAME,
                      szAPSec,
                      szAPName,
                      sizeof(szAPName));
    szAPName[sizeof(szAPName) - 1] = '\0';
    m_sAPName = szAPName;

    if (!CreateDeepDir(szPath))
    {
        return ERR_FAIL;
    }

    //配置文件的文件名
    sprintf(m_szFileName, "%s/stat_rst_%d.dat", szPath, m_uAccessPoint);

    FILE* pFile = NULL;

    //如果文件不存在，要创建文件
    if (access(m_szFileName, 0) == -1)
    {
        pFile = fopen(m_szFileName, "wb+");
        //创建文件失败
        if (pFile == NULL)
        {
            return ERR_FAIL;
        }

        //把结果头信息写进文件中
        time_t curtime = ACE_OS::time(NULL);
        SStatRstHead head;
        head.uAccessPoint   = m_uAccessPoint;
        head.uVersion       = 0x00000001;
        head.tStatEndTime   = curtime;
        memset(head.yReserved, 0, 8);

        if (fwrite(&head, STAT_RST_HEAD_LENGTH, 1, pFile) != 1)
        {
            fclose(pFile);
            return ERR_FAIL;
        }

        fclose(pFile);
    }

    //打开文件
    if ((pFile = fopen(m_szFileName, "rb")) == NULL)
    {
        return ERR_FAIL;
    }

    //获得文件大小
    UINT4 nFileLen = FileSize(pFile);
    m_nMemDataLen  = nFileLen;
    m_pMemData     = new BYTE[m_nMemDataLen];
    memset(m_pMemData, 0, m_nMemDataLen);

    m_pHead = new BYTE[STAT_RST_HEAD_LENGTH];
    memset(m_pHead, 0, STAT_RST_HEAD_LENGTH);

    //把配置头信息读进内存
    if (fread(m_pHead, STAT_RST_HEAD_LENGTH, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    fseek(pFile, 0, SEEK_SET);

    //把配置信息读进内存
    if ( fread(m_pMemData, m_nMemDataLen, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    fclose(pFile);
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
void CCDRStatRst::GetStatRstHead(SStatRstHead& statRstHead)
{
    memcpy(&statRstHead, m_pHead, STAT_RST_HEAD_LENGTH);
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
SStatExpIndexList* CCDRStatRst::GetStatExpList(void)
{
    SStatExpIndexList* pTemp;
    SStatExpIndexList* pStatExpIndexList;

    pTemp = (SStatExpIndexList*)(m_pMemData + STAT_RST_HEAD_LENGTH);

    if (m_nMemDataLen <= STAT_RST_HEAD_LENGTH)
    {
        pStatExpIndexList = new SStatExpIndexList;
        pStatExpIndexList->uStatExpCount = 0;
        return pStatExpIndexList;
    }
    else
    {
        pStatExpIndexList = new (pTemp->uStatExpCount)SStatExpIndexList;
    }

    pStatExpIndexList->uStatExpCount = pTemp->uStatExpCount ;

    memcpy(pStatExpIndexList->expIndex,
           pTemp->expIndex,
           pTemp->uStatExpCount * sizeof(SStatExpIndexMsg));

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
int CCDRStatRst::GetStatExpRst(const char* const szStatExpress, time_t tBeginTime,
                        time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal)
{
    UINT4 nOffset = 0;
    UINT4 nCurrentIndex = 0;
    SStatExpIndexMsg*  pExpMsg  = NULL;
    if (FindStatExpress((char*)szStatExpress, nOffset))
    {
        pExpMsg = (SStatExpIndexMsg*)(m_pMemData + nOffset);
        nCurrentIndex = pExpMsg->nStatExpIndex;
    }
    else
    {
        return ERR_FAIL;
    }

    nOffset = STAT_RST_HEAD_LENGTH + 4;
    SStatExpIndexList* pExpList = (SStatExpIndexList*)(m_pMemData + STAT_RST_HEAD_LENGTH);

    nOffset = nOffset + pExpList->uStatExpCount * STATEXPINDEXMSGLENGTH;

    SAllStatExpRst*  pAllRst = NULL;
    SOneStatExpRst*  pOneRst = NULL;

    while (nOffset < m_nMemDataLen)
    {
        pAllRst = (SAllStatExpRst*)(m_pMemData + nOffset);

        if ((pAllRst->uStatEndTime >= tBeginTime) && (pAllRst->uStatEndTime <= tEndTime))
        {
            FindStatRst(pAllRst, nCurrentIndex, nCDRCount, nValueTotal);
        }
        nOffset = nOffset + pAllRst->uStatSubItemCount * ONESTATEXPRSTLENGTH + 8;
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
int CCDRStatRst::AddStatExpress(LIST<char* >& StatExpList)
{
    FILE* pFile = NULL;
    if ((pFile = fopen(m_szFileName, "rb+")) == NULL)
    {
        return ERR_FAIL;
    }

    //重新写头
    fseek(pFile, 0, SEEK_SET);

    if (fwrite(m_pHead, 20, 1, pFile) != 1)
    {
        fclose(pFile);
    }

    SStatExpIndexMsg  ExpMsg;
    UINT4 nStatExpCount = 0;

    //第一次写了头没有写数据的时候
    if (m_nMemDataLen <= STAT_RST_HEAD_LENGTH)
    {
        ExpMsg.nStatExpIndex  = 0;
    }
    else
    {
        if (m_nMemDataLen <= STAT_RST_HEAD_LENGTH)
        {
            nStatExpCount = 0;
        }
        else
        {
            SStatExpIndexList* pExpList;
            pExpList = (SStatExpIndexList*)(m_pMemData + STAT_RST_HEAD_LENGTH);

            nStatExpCount = pExpList->uStatExpCount;
        }
    }

    UINT4 nOffset = STAT_RST_HEAD_LENGTH + 4;
    //全部把原来的表达式置为不可用
    for (UINT4 i = 1; i <= nStatExpCount; i++)
    {
        memcpy(&ExpMsg, m_pMemData + nOffset, STATEXPINDEXMSGLENGTH);
        ExpMsg.bInUsed = FALSE;

        fseek(pFile, nOffset, SEEK_SET);

        //写进文件中
        if (fwrite(&ExpMsg, STATEXPINDEXMSGLENGTH, 1, pFile) != 1)
        {
            fclose(pFile);
            RollBack();
            return ERR_FAIL;
        }
        nOffset = nOffset + STATEXPINDEXMSGLENGTH;
    }

    //最后一个一个表达式的索引号，用于索引增加
    UINT4 nLastExpMsgIndex = ExpMsg.nStatExpIndex;

    LIST<char* >::iterator it = StatExpList.begin();

    UINT4 nPosition = 0, nAddExpLen = nOffset;
    while (it != StatExpList.end())
    {
        if (FindStatExpress(*it, nPosition))
        {
            //对于原来存在找到的，修改bInUsed为TRUE然后写文件，
            memcpy(&ExpMsg, m_pMemData + nPosition, STATEXPINDEXMSGLENGTH);
            ExpMsg.bInUsed = TRUE;

            fseek(pFile, nPosition, SEEK_SET);

            //写进文件中
            if (fwrite(&ExpMsg, STATEXPINDEXMSGLENGTH, 1, pFile) != 1)
            {
                fclose(pFile);
                RollBack();
                return ERR_FAIL;
            }
        }
        else
        {
            //对于原来不存在的，增加一个在nOffset后面递增写，
            ExpMsg.bInUsed = TRUE;
            nLastExpMsgIndex++;
            ExpMsg.nStatExpIndex = nLastExpMsgIndex;
            strcpy(ExpMsg.szStatExp, *it);

            fseek(pFile, nAddExpLen, SEEK_SET);

            //写进文件中
            if (fwrite(&ExpMsg, STATEXPINDEXMSGLENGTH, 1, pFile) != 1)
            {
                fclose(pFile);
                RollBack();
                return ERR_FAIL;
            }

            //文件位置后移
            nAddExpLen = nAddExpLen + STATEXPINDEXMSGLENGTH;
        }
        it++;
    }

    //改写表达式的总数,重新写进文件
    UINT4 nCount = (nAddExpLen - STAT_RST_HEAD_LENGTH - 4) / STATEXPINDEXMSGLENGTH;

    fseek(pFile, STAT_RST_HEAD_LENGTH, SEEK_SET);

    if (fwrite(&nCount, 4, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }


    if (m_nMemDataLen > nOffset)
    {
        fseek(pFile, nAddExpLen, SEEK_SET);
        //最后把统计结果主体再写一次,写进文件中
        if (fwrite(m_pMemData + nOffset, m_nMemDataLen - nOffset, 1, pFile) != 1)
        {
            fclose(pFile);
            RollBack();
            return ERR_FAIL;
        }
    }

    fclose(pFile);
    delete[] m_pMemData;
    m_pMemData = NULL;

    if ((pFile = fopen(m_szFileName, "rb")) == NULL)
    {
        return ERR_FAIL;
    }
    //获得文件大小
    UINT4 nFileLen = FileSize(pFile);
    m_nMemDataLen  = nFileLen;
    m_pMemData     = new BYTE[m_nMemDataLen];
    memset(m_pMemData, 0, m_nMemDataLen);

    //把配置信息读进内存
    if (fread(m_pMemData, m_nMemDataLen, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    fclose(pFile);
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
int CCDRStatRst::AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime)
{
    FILE* pFile = NULL;

    if ((pFile = fopen(m_szFileName, "rb+")) == NULL)
    {
        return ERR_FAIL;
    }

    SStatRstHead* pStatRstHead = (SStatRstHead*)m_pHead;
    pStatRstHead->tStatEndTime = statTime;

    //修改统计时间
    fseek(pFile, 0, SEEK_SET);
    int nWrite = fwrite(m_pHead, sizeof(SStatRstHead), 1, pFile);
    if(nWrite != 1)
    {
        return ERR_FAIL;
    }

    //增加统计内容
    UINT4 nAllRstLen = statExpRst.uStatSubItemCount * ONESTATEXPRSTLENGTH + 8;

    fseek(pFile, m_nMemDataLen, SEEK_SET);
    if (fwrite(&statExpRst, nAllRstLen, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    //获得文件大小
    UINT4 nFileLen = FileSize(pFile);
    m_nMemDataLen  = nFileLen;
    delete m_pMemData;
    m_pMemData     = new BYTE[m_nMemDataLen];
    memset(m_pMemData, 0, m_nMemDataLen);

    fseek(pFile, 0, SEEK_SET);
    //把配置信息读进内存
    if (fread(m_pMemData, m_nMemDataLen, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    fclose(pFile);
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
int CCDRStatRst::CleanStatExp(void)
{
    UINT4 nOffset = STAT_RST_HEAD_LENGTH + 4;
    SStatExpIndexMsg*  pExpMsg  = NULL;
    SStatExpIndexList* pExpList = NULL;
    pExpList = GetStatExpList();

    //历遍整个表达式缓冲查找bIsUsed为FALSE的表达式
    for (UINT4 i = 0; i < pExpList->uStatExpCount; i++)
    {
        pExpMsg = (SStatExpIndexMsg*)(m_pMemData + nOffset);

        if (!pExpMsg->bInUsed)
        {
            //找到bInUsed为FALSE的表达式,根据其索引找到其统计结果的位置
            //待完成
        }
        nOffset = nOffset +  STATEXPINDEXMSGLENGTH;
    }
    delete pExpList;

    return ERR_SUCCESS;
}

/*******************************************************************
函数名称：FindStatExpress(char* szExpress, UINT4& nPosition)
功能描述：查找表达式的位置
输入参数：char* szExpress, 待找的表达式
输出参数：UINT4& nPosition,表达式位置
返回内容：找到返回TRUE,否则返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatRst::FindStatExpress(char* szExpress, UINT4& nPosition)
{
    BOOL bIsFinded = FALSE;

    //第一次写了头没有写数据的时候
    if (m_nMemDataLen <= STAT_RST_HEAD_LENGTH)
    {
        return bIsFinded;
    }

    UINT4 nOffset = STAT_RST_HEAD_LENGTH + 4;

    SStatExpIndexMsg*  pExpMsg  = NULL;
    SStatExpIndexList* pExpList = NULL;

    pExpList = GetStatExpList();
    //历遍整个表达式缓冲查找szExpress
    for (UINT4 i = 0; i < pExpList->uStatExpCount; i++)
    {
        if (strcmp((pExpList->expIndex[i]).szStatExp, szExpress) == 0)
        {
            bIsFinded = TRUE;
            break;
        }
        nOffset = nOffset +  STATEXPINDEXMSGLENGTH;
    }

    nPosition = nOffset;
    delete pExpList;
    return bIsFinded;
}


/*******************************************************************
函数名称：RollBack(void)
功能描述：如果写失败，把m_pMemData数据回写
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatRst::RollBack(void)
{
    FILE* pFile = NULL;

    if ((pFile = fopen(m_szFileName, "wb")) == NULL)
    {
        return ERR_FAIL;
    }

    if (fwrite(m_pMemData, m_nMemDataLen, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    fclose(pFile);
    return ERR_SUCCESS;
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
void CCDRStatRst::FindStatRst(SAllStatExpRst* pAllRst,
                              UINT4 nIndex,
                              UINT4& nCDRCount,
                              UINT4& nValueTotal)
{
    SOneStatExpRst* pOneRst = NULL;

    //历遍整个表达式缓冲查找szExpress
    for (UINT4 i = 0; i < pAllRst->uStatSubItemCount; i++)
    {
        pOneRst = (SOneStatExpRst*)(&(pAllRst->ItemRst[i]));

        if (pOneRst->nStatExpIndex == nIndex)
        {
            nCDRCount   += pOneRst->nCDRCount;
            nValueTotal += pOneRst->nValueTotal;
            break;
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
UINT4 CCDRStatRst::GetAccessPoint()
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
const char* CCDRStatRst::GetAccessPointName()
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
void  CCDRStatRst::GetAllExpress(LIST<STRING>& ExpressList)
{
    SStatExpIndexList* pExpList = NULL;
    pExpList = GetStatExpList();

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
time_t CCDRStatRst::GetLastStatTime()
{
    SStatRstHead* pStatRstHead = (SStatRstHead*)m_pHead;

    return pStatRstHead->tStatEndTime;
}

//刷新已执行统计操作的时间
int CCDRStatRst::SetLastStatTime(time_t curtime)
{
    SStatRstHead* pStatRstHead = (SStatRstHead*)m_pHead;

    pStatRstHead->tStatEndTime = curtime;

    FILE* pFile = NULL;

    if ((pFile = fopen(m_szFileName, "rb+")) != NULL)
    {
        //修改统计时间
        fseek(pFile, 0, SEEK_SET);
        int nWrite = fwrite(m_pHead, sizeof(SStatRstHead), 1, pFile);
        fclose(pFile);

        if(nWrite != 1)
        {
            return ERR_FAIL;
        }
         
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}
