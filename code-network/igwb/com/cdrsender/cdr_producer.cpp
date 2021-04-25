/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : cdr_producer.cpp
 *描述信息  ：本文件是类CCDRProducer的实现
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#include "cdr_producer.h"
#include "cdr_buffer.h"
#include "../include/toolbox.h"
#include "../frame/resource.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////
#define TMP_BUFFER_LEN    20480        //临时话单包缓存区20K
#define CDR_FILE_LEN      1572864      //话单文件大小1.5兆

BYTE*  CCDRProducer::g_pFileBuffer = NULL;
UINT4 CCDRProducer::g_nFileBuffSize = 0;
/**************************************************************************
 *函数原型  : CCDRProducer::CCDRProducer(CCDRBuffer* pBuffer)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : z30971
 *修改记录  : 
***************************************************************************/
CCDRProducer::CCDRProducer(CCDRBuffer* pBuffer, CSenderStatusFile* pStatusFile)
{
    ASSERT(pBuffer != NULL && pStatusFile);
    
    m_pCDRBuff = pBuffer;
    
    m_pStatusFile = pStatusFile;

    m_bIsExit = TRUE;

    m_nBillPacketID = 0;

    m_ThdExitEvt.reset();
}
/**************************************************************************
 *函数原型  : CCDRProducer::~CCDRProducer()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : z30971
 *修改记录  : 
***************************************************************************/
CCDRProducer::~CCDRProducer()
{
    m_pCDRBuff = NULL;

    m_pStatusFile = NULL;

    if (g_pFileBuffer != NULL)
    {
        delete[] g_pFileBuffer;
        g_pFileBuffer = NULL;
    }

    Clear();
}
/**************************************************************************
 *函数原型  : BOOL CCDRProducer::GetConfig()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : z30971
 *修改记录  : 
***************************************************************************/
BOOL CCDRProducer::GetConfig()
{
    CINIFile IniFile(GetCfgFilePath());
    if (IniFile.Open() != TRUE)
    {
        TRACE(MTS_CDRSENDER,S_PROCESS_OPEN_INIFAIL);
        return FALSE;
    }
    
    //读取配置任务个数信息
    m_nTaskCount = IniFile.GetInt(CFG_SEC_BS, 
                          CFG_BS_KEY_TASK_COUNT, 0);

    if(m_nTaskCount == 0)
    {
        return FALSE;
    }
        
    char szTaskKey[16];
    char szTaskPath[MAX_PATH];

    //读取任务路径信息
    for (int i = 1; i <= m_nTaskCount; i++)
    {
        SNPRINTF(szTaskKey, 16, "%s%d", CFG_BS_KEY_GTP_TASK, i);
        szTaskKey[15] = 0;

        IniFile.GetString(CFG_SEC_BS, szTaskKey, "", 
                          szTaskPath, MAX_PATH);
        szTaskPath[MAX_PATH - 1] = 0;
        
        if(szTaskPath[0] == 0)
        {
            TRACE(MTS_CDRSENDER, S_BS_GET_TASK_PATH_FAIL, i);
            return FALSE;
        }
        
        m_VecTaskPath.push_back(szTaskPath);
    }

    return TRUE;
}
/**************************************************************************
 *函数原型  : BOOL CCDRProducer::GetStatus()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
BOOL CCDRProducer::GetStatus()
{
    TStatusRecord* pRecord = NULL;
    TStatusRecord  Record;

    for (int id = 1; id <= m_nTaskCount; id++)
    {
        BOOL bRet = m_pStatusFile->GetRecord(id, &Record);
              
        pRecord = new TStatusRecord;
        if (bRet)
        {
            memcpy(pRecord, &Record, STATUSINFOLENGTH);
        }
        else
        {
            memset(pRecord, 0, STATUSINFOLENGTH);
            pRecord->m_nTaskID = id;
        }
        
        m_TaskStatusMap[id] = pRecord;
    }
    return TRUE;
}
/**************************************************************************
 *函数原型  : int CCDRProducer::open(void* args)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : z30971
 *修改记录  : 
***************************************************************************/
int CCDRProducer::open(void* args)
{
    if(!GetConfig())
    {
        TRACE(MTS_CDRSENDER, S_BS_GET_CONFIG_INFO_FAIL);
        return ERR_FAIL;
    }

    if(!GetStatus())
    {
        TRACE(MTS_CDRSENDER, S_BS_GET_STATUS_FAIL);
        return ERR_FAIL;
    }

    g_nFileBuffSize = CDR_FILE_LEN; 
    g_pFileBuffer = new BYTE[g_nFileBuffSize];

    if (g_pFileBuffer == NULL)
    {
        TRACE(MTS_CDRSENDER, S_BS_ALLOC_ERR);
        return ERR_FAIL;
    }

    //启动线程，如果失败返回ERR_FAIL
    if(this->activate())
        return ERR_FAIL;

    //设置m_bIsExit为FALSE
    m_bIsExit = FALSE;

    return ERR_SUCCESS;

}
/**************************************************************************
 *函数原型  : int CCDRProducer::svc(void)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : z30971
 *修改记录  : 
***************************************************************************/
int CCDRProducer::svc()
{
    ProduceCDR();
    return ERR_SUCCESS;
}
/**************************************************************************
 *函数原型  : void CCDRProducer::ProduceCDR()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 线程执行函数,将话单写入CCDRBuffer
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
void CCDRProducer::ProduceCDR()
{
    /*
    **  对每个任务轮询, 读取一定数目的话单,构成话单包,	
    **  向CDRBuffer写入数据
    */
    while (1)
    {
        for (int id = 1; id <= m_nTaskCount; id++)
        {
            if (!Sleep(1))
            {
                m_bIsExit = TRUE;
                return;
            }
            if (!PrepareTaskCurFiles(id))
            {
                m_bIsExit = TRUE;
                return;
            }
            if (!ProcessTaskCurFiles(id))
            {
                m_bIsExit = TRUE;
                return;
            }
        }
    }
}

/**************************************************************************
 *函数原型  : void CCDRProducer::stop()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
void CCDRProducer::stop()
{
    m_ThdExitEvt.signal();
    wait();
}
/**************************************************************************
 *函数原型  : void CCDRProducer::Clear()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : z30971
 *修改记录  : 
***************************************************************************/
void CCDRProducer::Clear()
{
    TASKSTATUSMAP::iterator it = m_TaskStatusMap.begin();

    for (; it != m_TaskStatusMap.end(); it++)
    {
        delete it->second;
        it->second = NULL;
    }

    m_TaskStatusMap.clear();

    m_VecTaskPath.clear();
}
/**************************************************************************
 *函数原型  : BOOL CCDRProducer::PrepareTaskCurFiles(UINT nTask)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
BOOL CCDRProducer::PrepareTaskCurFiles(UINT4 nTaskID)
{
    TStatusRecord* pRecord = m_TaskStatusMap[nTaskID];
    time_t   cTime = pRecord->m_tCreTime;
            
    LIST<STRING> subDirList;
    LIST<TFileNode>& FileList = m_TaskFilesMap[nTaskID];
    STRING szTaskPath = m_VecTaskPath[nTaskID-1];
            
    PrepareSubDirs(szTaskPath.c_str(), subDirList);

    //清空
    FileList.clear();            
    while(!subDirList.empty())
    {
         if (!Sleep(0))
         {
            return FALSE; 
         }
         STRING& szCurDir = subDirList.front();
         PrepareFiles(szCurDir.c_str(), cTime, FileList);
         subDirList.pop_front();
     }
     
     /*
     **	更新当前的读取状态记录, 如果准备的文件列表不为空
     ** 则链表的头记录文件一定和读记录一致
     */
     if (!FileList.empty())
     {
         FileList.sort();
         
         TFileNode& node = FileList.front();
         if (strcmp(node.m_szFileName, pRecord->m_szFileName) != 0)
         {
             STRNCPY(pRecord->m_szFileName, node.m_szFileName, MAX_PATH);
             pRecord->m_nCurPos = 0;
             pRecord->m_tCreTime = node.m_tFileTime;
         }
     }
     
     if (!Sleep(0))
     {
         return FALSE; 
     }

     return TRUE;
}
/**************************************************************************
 *函数原型  : void CCDRProducer::ProcessTaskCurFiles(UINT nTask)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
BOOL CCDRProducer::ProcessTaskCurFiles(UINT4 nTaskID)
{
    char tmpBuffer[TMP_BUFFER_LEN];
    
    //处理当前任务
    TStatusRecord* pRecord = m_TaskStatusMap[nTaskID];
    LIST<TFileNode>& fileList = m_TaskFilesMap[nTaskID];
 
    while (!fileList.empty())
    {
        if (!Sleep(0))
        {
            return FALSE; 
        }
        
        UINT2 nCdrCount = 0;
        UINT4 nDataLen = 0;

        TFileNode node = fileList.front();
        fileList.pop_front();

        if(strcmp(pRecord->m_szFileName, node.m_szFileName) != 0)
        {
            STRNCPY(pRecord->m_szFileName, node.m_szFileName, MAX_PATH);
            pRecord->m_nCurPos = 0;
            pRecord->m_tCreTime = node.m_tFileTime;
        }

        //文件的当前长度
        UINT4 uFileLen = FileSize(node.m_szFileName);

        if (g_nFileBuffSize < uFileLen)
        {
            delete g_pFileBuffer;
            g_pFileBuffer = new BYTE[uFileLen];

            if (g_pFileBuffer == NULL)
            {
                TRACE(MTS_CDRSENDER, S_BS_ALLOC_ERR);
                return FALSE;
            }
        }
        
        //已经到尾部或者当前无数据写入
        if (pRecord->m_nCurPos >= uFileLen)
        {
           continue;
        }

        FILE* fp = fopen(node.m_szFileName, "rb");
        
        if (fp == NULL)
        {
            TRACE(MTS_CDRSENDER, S_BS_OPEN_CDR_FILE_ERR, pRecord->m_szFileName);
            //sleep(60);
            //continue;
            return FALSE;
        }
        
        if (fseek(fp, pRecord->m_nCurPos, SEEK_SET) != 0)
        {
            TRACE(MTS_CDRSENDER, S_BS_FSEEK_ERR, pRecord->m_szFileName,
                  pRecord->m_nCurPos);
            fclose(fp);
            //sleep(60);
            //continue;

            return FALSE;
        }


        //将剩余的数据读出到内存中
        UINT4 nRet = fread(g_pFileBuffer, uFileLen - pRecord->m_nCurPos, 1, fp);
        fclose(fp);
        if (nRet != 1)
        {
            TRACE(MTS_CDRSENDER, S_BS_READ_FILE_ERR, pRecord->m_szFileName);
            return FALSE;
        }

        UINT4 nMemLen = uFileLen - pRecord->m_nCurPos;
        UINT4 nBillLen = 0;
        UINT4 nMemOffset = 0; 
        

        //处理单张话单 TBillPacket
        while (nMemOffset < nMemLen)
        {
            if (!Sleep(0))
            {
                return FALSE; 
            }
            
            //读入长度
            memcpy(&nBillLen, g_pFileBuffer + nMemOffset, sizeof(nBillLen));
            
            nBillLen = NTOHL(nBillLen);
            UINT2 nCdrLen = nBillLen;

            //不足一张话单
            if (nCdrLen > nMemLen - nMemOffset - 4)
            {
                break;
            }

            nCdrLen = HTONS(nCdrLen);
            memcpy(tmpBuffer + nDataLen, &nCdrLen, 2);
                       
            //拷贝数据
            memcpy(tmpBuffer + nDataLen + 2, 
               g_pFileBuffer + nMemOffset + 4, 
               nBillLen);
            nMemOffset += (nBillLen + 4);
            nDataLen += (nBillLen + 2);

            nCdrCount++;
            

            if ((nCdrCount >= CDRCOUNTPERPACKET) || (nMemOffset == nMemLen))
            {
                //此处的内存由由外部使用者释放
                TBillPacket* pPacket = new TBillPacket;

                pPacket->m_nTaskID = nTaskID;
                pPacket->m_nPacketID = m_nBillPacketID++;
                STRNCPY(pPacket->m_szFileName, node.m_szFileName, MAX_PATH);
                pPacket->m_tCreTime = node.m_tFileTime;
                pPacket->m_nCurReadPos = pRecord->m_nCurPos + nMemOffset;
                pPacket->m_nLength = nDataLen;
                pPacket->m_nCdrCount = nCdrCount;
                pPacket->m_pData = new BYTE[nDataLen];

                memcpy(pPacket->m_pData, tmpBuffer, nDataLen);

                m_pCDRBuff->PutPacket(pPacket);

                nCdrCount = 0;
                nDataLen = 0;
            }
       }//End whilen(MemOffset < nMemLen)

       //读记录状态
       pRecord->m_nCurPos = uFileLen;
    }

    return TRUE;
}
/**************************************************************************
 *函数原型  : BOOL PrepareSubDirs(const char* pszDir, LIST<STRING> &SubDirList)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
BOOL CCDRProducer::PrepareSubDirs(const char* pszDir, LIST<STRING> &SubDirList)
{
    PrepareSubDirs_Impl(pszDir,SubDirList);
 
    //对目录排序
    if(SubDirList.empty())
    {
        SubDirList.push_back(pszDir);
    }
    
    if (!Sleep(0))
    {
        return FALSE; 
    }
    SubDirList.sort();
    return TRUE;
}
/**************************************************************************
 *函数原型  : BOOL PrepareSubDirs_Impl(const char* pszDir, LIST<STRING>& SubDirList)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
void CCDRProducer::PrepareSubDirs_Impl(const char* pszDir, LIST<STRING>& SubDirList)
{
    //打开szPath目录
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;

    //打开文件目录
    pDir = ACE_OS::opendir(pszDir);
    if(pDir == NULL)
    {
        char szTmpBuf[MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_BS_OPEN_DIR_FAIL;
        sTmpInfo += pszDir;
        sTmpInfo += szTmpBuf;
        TRACE(MTS_CDRSENDER, sTmpInfo.c_str());

        return;
    }

    //遍历pszDir下文件(夹),如果是文件夹并且不是“.”或者“..”
    //(假设是tmpDir),将其加入SubDirList尾部,并且通过
    //SNPRINTF(pszDir, MAX_PATH,"s%s" psz, tmpDir)
    //更新pszDir后,递归调用PrepareSubDirs(pszDir,SubDirList)
    char szSrcFilePath[MAX_PATH];
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        ACE_Time_Value Seconds(0);
        Seconds += ACE_OS::gettimeofday ();
        int nRet = m_ThdExitEvt.wait(&Seconds);
        if(nRet == 0)
        {
            m_ThdExitEvt.signal();
            break;
        }

        SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",pszDir,pDirent->d_name);
        szSrcFilePath[MAX_PATH - 1] = 0;

        ACE_OS::stat(szSrcFilePath, &sDirStat);
        if(!S_ISDIR(sDirStat.st_mode)
            || strcmp(pDirent->d_name,".") == 0
            || strcmp(pDirent->d_name,"..") == 0
            || !IsDigitValue(pDirent->d_name))
        {
            continue;
        }
        SubDirList.push_back(szSrcFilePath);
        PrepareSubDirs_Impl(szSrcFilePath,SubDirList);
    }
    //关闭pszDir所在目录
    ACE_OS::closedir(pDir);
}
/**************************************************************************
 *函数原型  : int PrepareFiles(const char* szPath, time_t tFileTime
                               LIST<TFileNode>& FileList)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
int CCDRProducer::PrepareFiles(const char* szPath, time_t tFileTime,
                               LIST<TFileNode>& FileList)
{
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;

    //FileList.clear();
    
    //打开文件目录
    pDir = ACE_OS::opendir(szPath);
    if(pDir == NULL)
    {
        char szTmpBuf[MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_BS_OPEN_DIR_FAIL;
        sTmpInfo += szPath;
        sTmpInfo += szTmpBuf;
        TRACE(MTS_CDRSENDER, sTmpInfo.c_str());

        return ERR_FAIL;
    }

    char szSrcFilePath[MAX_PATH];
    
    TFileNode fileNode;
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        ACE_Time_Value Seconds(0);
        Seconds += ACE_OS::gettimeofday ();
        int nRet = m_ThdExitEvt.wait(&Seconds);
        if(nRet == 0)
        {
            m_ThdExitEvt.signal();
            break;
        }

        //形成源文件路径
        SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",szPath,pDirent->d_name);
        szSrcFilePath[MAX_PATH - 1] = 0;

        ACE_OS::stat(szSrcFilePath, &sDirStat);
        if(S_ISDIR(sDirStat.st_mode))
        {
            continue;
        }
        
        //判断是否传送过
        if(sDirStat.st_ctime >= tFileTime)
        {
            fileNode.m_tFileTime = sDirStat.st_ctime;
            SNPRINTF(fileNode.m_szFileName, MAX_PATH, "%s/%s", szPath, pDirent->d_name);
            fileNode.m_szFileName[MAX_PATH - 1] = 0;

            FileList.push_back(fileNode);
        }
    }

    ACE_OS::closedir(pDir);

    //FileList.sort();

    return ERR_SUCCESS;
}
/**************************************************************************
 *函数原型  : BOOL CCDRProducer::GetCreateTime(const char* szFilePath, 
                                                time_t t_cTime)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
void  CCDRProducer::GetCreateTime(const char* szPath, time_t st_cTime)
{
    ACE_stat sDirStat;      
    
    ACE_OS::stat(szPath, &sDirStat);

    st_cTime = sDirStat.st_ctime;
}
/**************************************************************************
 *函数原型  : BOOL CCDRProducer::Sleep(int nSeconds) 
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
BOOL CCDRProducer::Sleep(int nSeconds)
{
    ACE_Time_Value Seconds(nSeconds);
    Seconds += ACE_OS::gettimeofday ();

    int nRet = m_ThdExitEvt.wait(&Seconds);

    return (nRet != 0);
}