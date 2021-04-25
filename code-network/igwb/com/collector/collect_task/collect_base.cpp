#include "../../include/base_type.h"
#include "collect_base.h"
#include <assert.h>
#include "../resource.h"

#include "../../include/toolbox.h"
#include "../include/adapter_include.h"

//以下是采集操作的基类CCollectBase的定义
//构造函数
/********************************************************************
Function:        CCollectBase(CLinkMgr *pLinkMgr);
Description:     本方法是CCollectBase的构造函数，在此处生成具体的文件传输对象
Calls:           无
Called By:       类的使用者
Table Accessed:  无
Table Updated:   无
Input:           szProtocolId，字符串指针，代表不同的协议，如"Vertel NT"代表生成CVertelForNTFtam对象；
Output:          无
Return:          无
Others:          无
*********************************************************************/
CCollectBase::CCollectBase(int nSwitchIndex, CLinkGroup* pLinkGroup) //构造函数传入协议对象指针
{
    assert(NULL != pLinkGroup);

    m_pLinkMgr       = NULL;
    m_pLinkGroup     = pLinkGroup;
    m_nSwitchIndex   = nSwitchIndex;
    m_bFileLenVerify = FALSE;
}

//析构函数
CCollectBase::~CCollectBase()
{
    while(m_LinkMgrMap.size() > 0)
    {
        delete m_LinkMgrMap.begin()->second;
        m_LinkMgrMap.erase(m_LinkMgrMap.begin());
    }

    m_pLinkMgr = NULL;

    if(NULL != m_pLinkGroup)
    {
        delete m_pLinkGroup;
        m_pLinkGroup = NULL;
    }
}

//added by chenliangwei
//清除指定目录下的过期子目录
int CCollectBase::ClearOldDateSubDir(const char* szRootDir, 
                                      const int nSaveDays)
{
	if(nSaveDays <= 0)
    {
		return ERR_SUCCESS;
    }

	
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;

    //打开szRootDir目录
    pDir = ACE_OS::opendir(szRootDir);
    if(pDir == NULL)
    {
        char szTmpBuf[500];
        sprintf(szTmpBuf, S_FAILED_TO_OPEN_DIR, szRootDir, errno);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTmpBuf);
        COTRACE(szTmpBuf);

        return ERR_FAIL;
    }

    //遍历指定根目录下的子目录，并删除过期的目录
    int nRes = ERR_SUCCESS;
	char szPath[MAX_PATH];

	while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
		SNPRINTF(szPath,MAX_PATH,"%s/%s",szRootDir, pDirent->d_name);
		szPath[MAX_PATH - 1] = 0;
	
		if(strcmp(pDirent->d_name,".") == 0
		   || strcmp(pDirent->d_name,"..") == 0)
        {
            continue;
        }

		ACE_OS::stat(szPath, &sDirStat);
		int nElapsed = 0;
		if(S_IFDIR == (sDirStat.st_mode & S_IFDIR) && IsDigitValue(pDirent->d_name))
		{
            //将目录转换为时间
			time_t curTime,DirTime;
			int nDateNum = ACE_OS::atoi(pDirent->d_name);
			tm tTmp;
			memset(&tTmp,0,sizeof(tm));
		    tTmp.tm_year = (nDateNum / 10000) - 1900;
            tTmp.tm_mon = (nDateNum % 10000)/100 - 1;
			tTmp.tm_mday = nDateNum %100;
			DirTime = mktime(&tTmp); //得到目录时间的基准值

			//取当前时间
			time(&curTime);
			tTmp = *localtime(&curTime);
			tTmp.tm_hour = 0;
			tTmp.tm_min = 0;
			tTmp.tm_sec = 0;
			curTime = mktime(&tTmp); //得到当前的日期，时分秒都为0

            //判断目录时间是否已过期
			nElapsed = difftime(curTime,DirTime);
			if(nElapsed >= (nSaveDays * 24 * 3600))
			{
                SListOut listout;
                nRes = GetLocalFileList(szPath, listout);

                if (nRes != ERR_SUCCESS)
                {
                    WRITELOG_F(S_FAILED_TO_REMOVE_OUTDATED_DIR, 
                               szPath);
                    COMSGOUT(MSGOUT_LEVEL_URGENT, 
                           S_FAILED_TO_REMOVE_OUTDATED_DIR, 
                           szPath);

              		nRes = ERR_FAIL;
                }

                while(listout.FileList.size() > 0)
                {
                    char szFilePath[MAX_PATH];
                    SNPRINTF(szFilePath, 
                             MAX_PATH, 
                             "%s/%s", 
                             szPath, 
                             listout.FileList.front().szFileName);

                    if (FileDel(szFilePath))
                    {
                        WRITELOG_C(S_REMOVE_OUTDATED_CDRS, 
                                   szFilePath);
                        COMSGOUT(MSGOUT_LEVEL_SIDLY, 
                               S_REMOVE_OUTDATED_CDRS, 
                               szFilePath);
                    }
                    else
                    {
                        WRITELOG_F(S_FAILED_TO_RMV_OUTDATED_CDRS, 
                                   szFilePath);
                        COMSGOUT(MSGOUT_LEVEL_URGENT, 
                               S_FAILED_TO_RMV_OUTDATED_CDRS, 
                               szFilePath);
              		    nRes = ERR_FAIL;
                    }

                    listout.FileList.erase(listout.FileList.begin());
                }
 
				rmdir(szPath);
			}
		}
	}

	//关闭目录
	ACE_OS::closedir(pDir);
	return nRes;
}
//added end

//added by chenliangwei
//得到本地指定目录下的文件列表
int  CCollectBase::GetLocalFileList(const char* szSrcDir, SListOut &pOut)
{
    //得到指定目录下所有文件
    ACE_DIR *pdir = ACE_OS::opendir(szSrcDir);
    dirent *pdirent;
    
    //李杜芳 修改 12.17
    if (pdir==NULL)
    {
        return ERR_FAIL;
    }
    //修改完毕
    
    while (pdirent = ACE_OS::readdir(pdir))
    {
		if(strcmp(pdirent->d_name,".") == 0
		   || strcmp(pdirent->d_name,"..") == 0)
        {
            continue;
        }

        SFileInfo fi;
        strncpy(fi.szFileName, pdirent->d_name, sizeof(fi.szFileName));
        fi.szFileName[sizeof(fi.szFileName) - 1] = '\0';
        fi.nFileSize        = 0;
        fi.nAttr            = 0;
        fi.nCollectStatus   = CS_NOT_COLLECTED;
        pOut.FileList.push_back(fi);
    }

    ACE_OS::closedir(pdir);

    return ERR_SUCCESS;
}
//added end

int CCollectBase::SetAlarmLimit(const char* const szTP, 
                                int nAlarmLimit)
{
    return ERR_FAIL;
}

//指定的文件是否可采
int CCollectBase::CanGetSpecFile(const char* const szModule,
                                 const char* const szSrcFile)
{
    return ERR_FAIL;
}

int CCollectBase::GetSpecFile(const char* const szModule,
                              const char* const szSrcFile, 
                              const char* const szDstFile)
{
    return ERR_FAIL;
}

//获取交换机信息
int CCollectBase::GetSwitchInfo(LIST<STRING>& label,
                                LIST<STRING>& content)
{
    return ERR_FAIL;
}

//获取模块信息
int CCollectBase::GetModuleInfo(const char* const szModule,
                                LIST<STRING>& label, 
                                LIST<STRING>& content)
{
    return ERR_FAIL;
}

//获取文件长度
int CCollectBase::GetFileLen(const char* const szModule,
                             const char* const szSrcFile)
{
    return 0;
}

