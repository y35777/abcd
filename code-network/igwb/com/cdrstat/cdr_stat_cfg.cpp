#include "../include/toolbox.h"
#include "../include/frame.h"
#include "cdr_stat_cfg.h"

/*******************************************************************
函数名称：CCDRStatCfg()
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
CCDRStatCfg::CCDRStatCfg()
{
    m_uAccessPoint  = 0;
    m_nMemDataLen   = 0;
    m_pMemData      = NULL;
    m_szFileName[0] = '\0';

}
/*******************************************************************
函数名称：~CCDRStatCfg()
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
CCDRStatCfg::~CCDRStatCfg()
{
    if (m_pMemData != NULL)
    {
        delete m_pMemData;
        m_pMemData = NULL;
    }
}


/*******************************************************************
函数名称：Init(UINT4 uAccessPoint)
功能描述：设置参数，进行初始化操作
输入参数：UINT4 uAccessPoint,接入点号
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatCfg::Init(UINT4 uAccessPoint)
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
    sprintf(m_szFileName, "%s/cdr_stat_%d.dat", szPath, m_uAccessPoint);

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

        //把配置头信息写进文件中
        SStatCfgHead head;
        head.uAccessPoint   = m_uAccessPoint;
        head.uStatItemCount = 0;
        head.uVersion       = 0x00000001;
        memset(head.yReserved, 0, 8);

        if (fwrite(&head, STAT_CFG_HEAD_LENGTH, 1, pFile) != 1)
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
函数名称：GetStatItemNameList(LIST<STRING>& listStatItemName)
功能描述：获得所有统计项的项目
输入参数：LIST<char* >& listStatItemName,统计项名称列表
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatCfg::GetStatItemNameList(LIST<STRING>& listStatItemName)
{
    //还没有写入任何统计项目,返回空
    if (m_nMemDataLen <= STAT_CFG_HEAD_LENGTH)
    {
        return;
    }

    UINT4 nOffset = STAT_CFG_HEAD_LENGTH;
    SStatCfgHead* pHead = (SStatCfgHead*)m_pMemData;

    for (UINT4 i = 0; i < pHead->uStatItemCount; i++)
    {
        SStatItemCfg* pStatItemCfg = (SStatItemCfg*)(m_pMemData + nOffset);
        nOffset = nOffset + pStatItemCfg->uStatItemCount * SUBITEMCFGLENGTH + STATITEMCFGLENGTH;
        listStatItemName.push_back(pStatItemCfg->szStatItemName);
    }

}

/*******************************************************************
函数名称：GetStatItem(const char* const szStatItemName)
功能描述：获得指定统计项目名的统计配置信息
输入参数：const char* const szStatItemName,统计项名称
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
SStatItemCfg* CCDRStatCfg::GetStatItem(const char* const szStatItemName)
{
    SStatItemCfg* pStatItemCfg = NULL;

    UINT4 nOffset = STAT_CFG_HEAD_LENGTH;
    SStatCfgHead* pHead = (SStatCfgHead*)m_pMemData;

    for (UINT4 i = 0; i < pHead->uStatItemCount; i++)
    {
        SStatItemCfg* pCfg = NULL;
        pCfg = (SStatItemCfg*)(m_pMemData + nOffset);
        nOffset = nOffset + pCfg->uStatItemCount * SUBITEMCFGLENGTH + STATITEMCFGLENGTH;

        if (strcmp(pCfg->szStatItemName, szStatItemName) == 0)
        {
            pStatItemCfg = new (pCfg->uStatItemCount)SStatItemCfg;
            sprintf((char*)pStatItemCfg->szStatItemName, "%s", pCfg->szStatItemName);
            pStatItemCfg->uStatItemCount = pCfg->uStatItemCount;

            memcpy(pStatItemCfg->subItemCfg,
                   pCfg->subItemCfg,
                   pStatItemCfg->uStatItemCount * sizeof(SStatSubItemCfg));

            break;
        }
    }

    return pStatItemCfg;

}

/*******************************************************************
函数名称：GetStatItem(const char* const szStatItemName, LIST<SOneStatSubItemCfg*>& subItemList)
功能描述：获得指定统计项目名的统计配置信息
输入参数：const char* const szStatItemName,统计项名称
          LIST<SOneStatSubItemCfg*>& subItemList,统计项列表
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatCfg::GetStatItem(const char* const szStatItemName,
                              LIST<SOneStatSubItemCfg*>& subItemList)
{
    const SStatItemCfg* pStatItemCfg = NULL;

    pStatItemCfg = GetStatItem(szStatItemName);

    if (pStatItemCfg == NULL)
    {
        return;
    }

    UINT4 nCount = pStatItemCfg->uStatItemCount;

    //填充返回列表subItemList
    const SStatSubItemCfg* pTmp;
    for( UINT4 i = 0; i < nCount; i++)
    {
        pTmp = &(pStatItemCfg->subItemCfg[i]);

        SOneStatSubItemCfg* pOneCfg = new SOneStatSubItemCfg;

        memcpy(pOneCfg->szStatItemName,
               pStatItemCfg->szStatItemName,
               sizeof(pOneCfg->szStatItemName));

        memcpy(pOneCfg->szStatSubItemName,
               pTmp->szStatSubItemName,
               sizeof(pOneCfg->szStatSubItemName));

        memcpy(pOneCfg->szStatSubItemCond,
               pTmp->szStatSubItemCond,
               sizeof(pOneCfg->szStatSubItemCond));

        subItemList.push_back(pOneCfg);
    }
}


/*******************************************************************
函数名称：SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
功能描述：设置所有统计子项的配置信息
输入参数：LIST<SOneStatSubItemCfg*> subItemList,统计项列表
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatCfg::SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
{
    SOneStatSubItemCfg* pTemp = NULL;
    char  szItemName[64];
    szItemName[63] = '\0';

    UINT4 nItemCount  = 0, nSubItemCount = 0;
    UINT4 nItemOffSet = 0, nSubItemOffSet= 0;
    SStatItemCfg    StatItemCfg;
    SStatSubItemCfg StatSubItemCfg;

    LIST<SOneStatSubItemCfg*>::iterator it = subItemList.begin();

    BOOL bFlag = FALSE;
    BOOL bIsTheSameStatItem = TRUE;

    //第一个统计项的位置是20
    nItemOffSet = STAT_CFG_HEAD_LENGTH;
    //第一个子项的位置是88(20+68)
    nSubItemOffSet = STAT_CFG_HEAD_LENGTH + STATITEMCFGLENGTH;

    while (it != subItemList.end())
    {
        pTemp = (SOneStatSubItemCfg*)(*it);

        if (bIsTheSameStatItem)
        {
            SNPRINTF(szItemName, sizeof(szItemName), "%s", pTemp->szStatItemName);
            szItemName[sizeof(szItemName) - 1] = '\0';
            bIsTheSameStatItem = FALSE;
        }

        //如果统计项名称不相等，统计项的个数要增加
        if ((strcmp(pTemp->szStatItemName, szItemName) != 0) && bFlag)
        {
            nItemCount++;

            //把一个统计项写到信息缓冲区中
            memcpy(StatItemCfg.szStatItemName,
                   szItemName,
                   sizeof(StatItemCfg.szStatItemName));
            StatItemCfg.uStatItemCount = nSubItemCount;

            memcpy(m_pMemData + nItemOffSet, &StatItemCfg, STATITEMCFGLENGTH);

            nSubItemCount = 0;

            SNPRINTF(szItemName, sizeof(szItemName), "%s", pTemp->szStatItemName);
            szItemName[sizeof(szItemName) - 1] = '\0';
            

            //调整下一个统计项和下一个统计子项的位置
            nItemOffSet = nSubItemOffSet;
            nSubItemOffSet = nSubItemOffSet + STATITEMCFGLENGTH;
        }


        //把每一个子项写进内存
        memcpy(StatSubItemCfg.szStatSubItemName,
               pTemp->szStatSubItemName,
               sizeof(StatSubItemCfg.szStatSubItemName));
        memcpy(StatSubItemCfg.szStatSubItemCond,
               pTemp->szStatSubItemCond,
               sizeof(StatSubItemCfg.szStatSubItemCond));

        //假如缓冲区不够
        if (nSubItemOffSet + SUBITEMCFGLENGTH > m_nMemDataLen)
        {
            BYTE* pMemData = m_pMemData;
            m_nMemDataLen  = m_nMemDataLen + INCMEMLENGTH;
            m_pMemData     = new BYTE[m_nMemDataLen];
            memcpy(m_pMemData, pMemData, nSubItemOffSet);
            memset(&m_pMemData[nSubItemOffSet], 0, m_nMemDataLen - nSubItemOffSet);
            delete pMemData, pMemData = NULL;
        }

        memcpy(m_pMemData + nSubItemOffSet, &StatSubItemCfg, SUBITEMCFGLENGTH);

        nSubItemOffSet = nSubItemOffSet + SUBITEMCFGLENGTH;

        nSubItemCount++;
        bFlag = TRUE;
        it++;
    }

    //修改最后一个统计项的头信息
    if (bFlag)
    {
        nItemCount++;
        //把一个统计项写到信息缓冲区中
        memcpy(StatItemCfg.szStatItemName,
               pTemp->szStatItemName,
               sizeof(StatItemCfg.szStatItemName));
        StatItemCfg.uStatItemCount = nSubItemCount;
        memcpy(m_pMemData + nItemOffSet, &StatItemCfg, STATITEMCFGLENGTH);
    }

    //修改配置信息头的所有统计项数目
    SStatCfgHead head;
    memcpy(&head, m_pMemData, STAT_CFG_HEAD_LENGTH);
    head.uStatItemCount = nItemCount;
    memcpy(m_pMemData, &head, STAT_CFG_HEAD_LENGTH);

    //把m_pMemData的内容写进文件中
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
UINT4 CCDRStatCfg::GetAccessPoint()
{
    return m_uAccessPoint;
}


/*******************************************************************
函数名称：获得接入点名称
功能描述：GetAccessPointName()
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
const char* CCDRStatCfg::GetAccessPointName()
{
    return m_sAPName.c_str();
}
