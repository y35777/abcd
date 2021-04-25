#include "../../include/base_type.h"
#include "collect_base.h"
#include <assert.h>
#include "../resource.h"

#include "../../include/toolbox.h"
#include "../include/adapter_include.h"

//�����ǲɼ������Ļ���CCollectBase�Ķ���
//���캯��
/********************************************************************
Function:        CCollectBase(CLinkMgr *pLinkMgr);
Description:     ��������CCollectBase�Ĺ��캯�����ڴ˴����ɾ�����ļ��������
Calls:           ��
Called By:       ���ʹ����
Table Accessed:  ��
Table Updated:   ��
Input:           szProtocolId���ַ���ָ�룬����ͬ��Э�飬��"Vertel NT"��������CVertelForNTFtam����
Output:          ��
Return:          ��
Others:          ��
*********************************************************************/
CCollectBase::CCollectBase(int nSwitchIndex, CLinkGroup* pLinkGroup) //���캯������Э�����ָ��
{
    assert(NULL != pLinkGroup);

    m_pLinkMgr       = NULL;
    m_pLinkGroup     = pLinkGroup;
    m_nSwitchIndex   = nSwitchIndex;
    m_bFileLenVerify = FALSE;
}

//��������
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
//���ָ��Ŀ¼�µĹ�����Ŀ¼
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

    //��szRootDirĿ¼
    pDir = ACE_OS::opendir(szRootDir);
    if(pDir == NULL)
    {
        char szTmpBuf[500];
        sprintf(szTmpBuf, S_FAILED_TO_OPEN_DIR, szRootDir, errno);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTmpBuf);
        COTRACE(szTmpBuf);

        return ERR_FAIL;
    }

    //����ָ����Ŀ¼�µ���Ŀ¼����ɾ�����ڵ�Ŀ¼
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
            //��Ŀ¼ת��Ϊʱ��
			time_t curTime,DirTime;
			int nDateNum = ACE_OS::atoi(pDirent->d_name);
			tm tTmp;
			memset(&tTmp,0,sizeof(tm));
		    tTmp.tm_year = (nDateNum / 10000) - 1900;
            tTmp.tm_mon = (nDateNum % 10000)/100 - 1;
			tTmp.tm_mday = nDateNum %100;
			DirTime = mktime(&tTmp); //�õ�Ŀ¼ʱ��Ļ�׼ֵ

			//ȡ��ǰʱ��
			time(&curTime);
			tTmp = *localtime(&curTime);
			tTmp.tm_hour = 0;
			tTmp.tm_min = 0;
			tTmp.tm_sec = 0;
			curTime = mktime(&tTmp); //�õ���ǰ�����ڣ�ʱ���붼Ϊ0

            //�ж�Ŀ¼ʱ���Ƿ��ѹ���
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

	//�ر�Ŀ¼
	ACE_OS::closedir(pDir);
	return nRes;
}
//added end

//added by chenliangwei
//�õ�����ָ��Ŀ¼�µ��ļ��б�
int  CCollectBase::GetLocalFileList(const char* szSrcDir, SListOut &pOut)
{
    //�õ�ָ��Ŀ¼�������ļ�
    ACE_DIR *pdir = ACE_OS::opendir(szSrcDir);
    dirent *pdirent;
    
    //��ŷ� �޸� 12.17
    if (pdir==NULL)
    {
        return ERR_FAIL;
    }
    //�޸����
    
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

//ָ�����ļ��Ƿ�ɲ�
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

//��ȡ��������Ϣ
int CCollectBase::GetSwitchInfo(LIST<STRING>& label,
                                LIST<STRING>& content)
{
    return ERR_FAIL;
}

//��ȡģ����Ϣ
int CCollectBase::GetModuleInfo(const char* const szModule,
                                LIST<STRING>& label, 
                                LIST<STRING>& content)
{
    return ERR_FAIL;
}

//��ȡ�ļ�����
int CCollectBase::GetFileLen(const char* const szModule,
                             const char* const szSrcFile)
{
    return 0;
}

