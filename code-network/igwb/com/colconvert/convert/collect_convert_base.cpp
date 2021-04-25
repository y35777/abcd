#include "collect_convert_base.h"
#include <assert.h>

#include "../../include/toolbox.h"
#include "../include/collect_convert_include.h"
#include "include/collect_convert_define.h"
#include "../../include/format_out.h"

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
CCollectConvertBase::CCollectConvertBase(int nSwitchIndex) //���캯������Э�����ָ��
{
    m_nSwitchIndex = nSwitchIndex;
    m_pIGWB = NULL;

    m_szSwitchName[0] = '\0';
    m_szCollectId[0] = '\0';
    m_szCfgPath[0] = '\0';

    m_pIBillProcessor = NULL;
}

//��������
CCollectConvertBase::~CCollectConvertBase()
{
    //
    if (m_pIGWB != NULL)
    {
        delete m_pIGWB;
        m_pIGWB = NULL;
    }
}

int CCollectConvertBase::Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(%s)ת��ģ��������ļ�(%s)ʧ��", 
              m_szSwitchName, 
              m_szCfgPath);
        return ERR_FAIL;
    }


    char szSecName[CFG_BUF_SIZE];
    SNPRINTF(szSecName, 
             sizeof(szSecName), 
             "%s%d", 
             CFG_CO_SEC_SWITCH, 
             m_nSwitchIndex);
    szSecName[CFG_BUF_SIZE - 1] = '\0';

    //��ȡ��������
    INIFile.GetString(szSecName,
                      CFG_CO_NAME, 
                      CFG_CO_NAME_DEFAULT, 
                      m_szSwitchName, 
                      sizeof(m_szSwitchName));

    //��ȡ����������
    INIFile.GetString(szSecName,
                      CFG_CO_TYPE, 
                      CFG_CO_TYPE_DEFAULT, 
                      m_szCollectId, 
                      sizeof(m_szCollectId));

    //������
    m_nAPID = CCollectorAdapter::GetProcessType() - PT_CO_BASE + 1;

    //��ʼ��m_pIBillProcessor
    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "%s/config/format", GETAPPPATH);

    m_pIGWB = new CIGWB(szIGWBPath);

    if (m_pIGWB == NULL)
    {
        COTRACE("�ɼ�����(%s)����IGWB����ʧ��,�����ļ�·��(%s)",
            m_szSwitchName,
            szIGWBPath);

        return ERR_FAIL;
    }

    m_pIBillProcessor = (IBillProcessor* )m_pIGWB->QueryInterface(IID_FP_IBILLPROCESSOR);

	if (NULL == m_pIBillProcessor)
    {
        COTRACE("�ɼ�����(%s)����IBillProcessor����ʧ��,�����ļ�·��(%s)",
            m_szSwitchName,
            szIGWBPath);

        return ERR_FAIL;
    }

    int nResult = m_pIBillProcessor->Init(m_nAPID);     
    if (0 != nResult)
    {
        COTRACE("�ɼ�����(%s)��ʼ��IBillProcessor����(AP=%d)ʧ��,�����ļ�·��(%s)",
            m_szSwitchName,
            m_nAPID,
            szIGWBPath);

        return ERR_FAIL;
    }

    return ERR_SUCCESS;

}

int CCollectConvertBase::Process(const char* szBillFile, const int& nConvertInfo)
{
    int nRet = ERR_FAIL;

    //��������Ŀ¼
    nRet = CreateLocSubDir(); 

    //ת�������ļ�
    SConvertFileReq stConvertReq;
    memset(&stConvertReq, 0, sizeof(stConvertReq));

    strncpy(stConvertReq.szSrcName, szBillFile, sizeof(stConvertReq.szSrcName));
    stConvertReq.nConvertInfo = nConvertInfo;

    nRet = Convert(stConvertReq);

    if (ERR_SUCCESS != nRet)
    {
        SInnerAlarm Alarm;
        Alarm.yAlarmType    = AT_EVENT;
        Alarm.uAlarmID      = ALARM_ID_CONVERT_FILE_FAULT;
        Alarm.yAlarmPara[0] = ALARM_PARA_NONE;
        SENDALARM(&Alarm);
    }


    //��������ļ�
    if (ERR_SUCCESS != ClearOldFile())
    {
        WRITELOG_F("�ɼ�����(%s)ɾ�����ڵ����ջ����ļ�ʧ��",
                   m_szSwitchName);
        COMSGOUT(MSGOUT_LEVEL_URGENT, 
               "�ɼ�����(%s)ɾ�����ڵ����ջ����ļ�ʧ��",
               m_szSwitchName);
    }

    return nRet;

}

int CCollectConvertBase::Convert(SConvertFileReq& stReq)
{
    int nSubObjIndex = stReq.nConvertInfo;

    if (m_SubObjMap.find(nSubObjIndex) != m_SubObjMap.end())
    {
        return m_SubObjMap[nSubObjIndex]->Convert(stReq);
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
            "�ɼ�����(%s)ת�������ļ�ʱ�Ҳ�����ת���Ӷ���(%d)",
            m_szSwitchName,
            nSubObjIndex);

        COTRACE("�ɼ�����(%s)ת�������ļ�ʱ�Ҳ���ת���Ӷ���(%d)",
            m_szSwitchName,
            nSubObjIndex);

    }

    return ERR_FAIL;

}

int CCollectConvertBase::CreateLocSubDir()
{
    int nRet = ERR_SUCCESS;
    MAP<int, CCollectConvertSubObj*>::iterator i = m_SubObjMap.begin();
    for(; i != m_SubObjMap.end(); i++)
    {
        if(ERR_SUCCESS != i->second->CreateLocSubDir())
        {
            nRet = ERR_FAIL;
        }
    }

    return nRet;

}

int CCollectConvertBase::ClearOldFile()
{
    int nRet = ERR_SUCCESS;
    MAP<int, CCollectConvertSubObj*>::iterator i = m_SubObjMap.begin();
    for(; i != m_SubObjMap.end(); i++)
    {
        if(ERR_SUCCESS != i->second->ClearOldFile())
        {
            nRet = ERR_FAIL;
        }
    }

    return nRet;

}

//added by chenliangwei
//���ָ��Ŀ¼�µĹ�����Ŀ¼
int CCollectConvertBase::ClearOldDateSubDir(const char* szRootDir, 
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
        sprintf(szTmpBuf, 
            "��Ŀ¼(%s)ʧ��(Errno: %d)", 
            szRootDir, 
            errno);
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
                    WRITELOG_F("ɾ�����ڵ����ջ���Ŀ¼%sʧ��", 
                               szPath);
                    COMSGOUT(MSGOUT_LEVEL_URGENT, 
                           "ɾ�����ڵ����ջ���Ŀ¼%sʧ��", 
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
                        WRITELOG_C("ɾ�����ڵ����ջ����ļ�%s", 
                                   szFilePath);
                        COMSGOUT(MSGOUT_LEVEL_SIDLY, 
                               "ɾ�����ڵ����ջ����ļ�%s", 
                               szFilePath);
                    }
                    else
                    {
                        WRITELOG_F("ɾ�����ڵ����ջ����ļ�%sʧ��", 
                                   szFilePath);
                        COMSGOUT(MSGOUT_LEVEL_URGENT, 
                               "ɾ�����ڵ����ջ����ļ�%sʧ��", 
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
int  CCollectConvertBase::GetLocalFileList(const char* szSrcDir, SListOut &pOut)
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
        fi.nFileSize = 0;
        fi.nAttr     = 0;
        pOut.FileList.push_back(fi);
    }

    ACE_OS::closedir(pdir);

    return ERR_SUCCESS;
}
//added end


