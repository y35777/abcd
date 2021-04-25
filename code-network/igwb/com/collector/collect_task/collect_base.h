#ifndef __COLLECT_BASE_H__
#define __COLLECT_BASE_H__

/*
CCollectBase�ǲɼ������Ļ���, �������˲ɼ�������صĻ������Ժͻ�������
*/
#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "file_protocol.h"
#include "link_mgr.h"


//�����ǲɼ������Ļ���CCollectBase�Ķ���
class  CCollectBase
{

public:
    CCollectBase(int nSwitchIndex, CLinkGroup* pLinkGroup); //���캯������Э�����ָ��
    virtual ~CCollectBase();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Col_Init() = 0;

    //�����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��;
    virtual  int  Col_GetFile(SColFileReq& stReq) = 0;

    //����ļ��б�, ���麯��, �����������ʵ��
    virtual  int  Col_GetList(SListOut& pOut) = 0;

    //��̬������Ŀ¼, ���麯��, �����������ʵ��
    virtual  BOOL CreateLocSubDir() = 0;

    //added by chenliangwei
    //���ָ��Ŀ¼�µĹ�����Ŀ¼����������ʹ�õĹ��ߺ���
    static int ClearOldDateSubDir(const char* szRootDir, const int nSaveDays);

    //�õ�����ָ��Ŀ¼�µ��ļ��б�
    static int GetLocalFileList(const char* szSrcDir, SListOut &pOut);
    //added end

    //��������ļ�, ���麯��, �����������ʵ��
    virtual  BOOL ClearOldFile()
    {
        return TRUE;
    }

    //added by chenliangwei
    //����ļ����к��Ƿ����������麯��, �����������ʵ��
    virtual  BOOL CheckFileSN(const char* szFileName, char* szPreviousFile)
    {
        return TRUE;
    }
    //added end

    //added by chenliangwei, 2003-08-26
    //�õ���·����Ϣ
    int GetLinkInfo(MAP<STRING, SLinkStatusMsg>& LinkStatus)
    {
        if(m_pLinkGroup != NULL)
        {
            return m_pLinkGroup->GetLinkInfo(LinkStatus);
        }

        return ERR_SUCCESS;
    }
    //added end

    //�������ӷ����
    virtual  int  TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus)
    {
        if(m_pLinkGroup != NULL)
        {
            return m_pLinkGroup->TestLink(LinkStatus);
        }

        return ERR_SUCCESS;
    }

    //���Ӳɼ�������
    virtual  int  Connect()
    {
        return  ERR_SUCCESS;
    }

    //�Ͽ�����
    virtual  int  Disconnect()
    {
        return  ERR_SUCCESS;
    }

    inline void SetCfgPath(const char* const szCfgPath)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }
    inline void RegisterLinkMgr(const char* const szType, CLinkMgr* pLinkMgr)
    {
        STRING strType = szType;
        if(m_LinkMgrMap.find(strType) == m_LinkMgrMap.end())
        {
            m_LinkMgrMap[strType] = pLinkMgr;
        }
    }
    inline CLinkMgr* GetLinkMgr(const STRING& strType)
    {
        if(m_LinkMgrMap.find(strType) != m_LinkMgrMap.end())
        {
            return m_LinkMgrMap[strType];
        }
        return NULL;
    }

    //���ø澯����
    virtual int  SetAlarmLimit(const char* const szTP, int nAlarmLimit);

    //ָ�����ļ��Ƿ�ɲ�
    virtual int  CanGetSpecFile(const char* const szModule,
                                const char* const szSrcFile);

    //�ɼ�ָ�����ļ�
    virtual int  GetSpecFile(const char* const szModule,
                             const char* const szSrcFile, 
                             const char* const szDstFile);
    //��ȡ��������Ϣ
    virtual int  GetSwitchInfo(LIST<STRING>& label,
                               LIST<STRING>& content);

    //��ȡģ����Ϣ
    virtual int  GetModuleInfo(const char* const szModule,
                               LIST<STRING>& label,
                               LIST<STRING>& content);

    //��ȡ�ļ�����
    virtual int  GetFileLen(const char* const szModule,
                            const char* const szSrcFile);

    inline void EnableFileLenVerify()
    {
        m_bFileLenVerify = TRUE;
    }

    inline void DisableFileLenVerify()
    {
        m_bFileLenVerify = FALSE;
    }

protected:

    int   m_nSwitchIndex;
    char  m_szSwitchName[40];       //��������
    char  m_szCollectId[40];        //ָ������Ĳɼ�������"5ESS"�����ڴ���C5ESSCollect����
    char  m_szProtocolId[40];       //ָ��������ļ��ɼ�Э�飬��"Vertel NT"�����ڴ���CVertelForNT����

    char  m_szCfgPath[MAX_PATH];    //��ǰ�����ļ�������·����Ϣ
    CLinkGroup* m_pLinkGroup;
    CLinkMgr* m_pLinkMgr;           //�ļ��������ָ�룬��ָ���ļ�������CFileProtocol��������Ķ���
    MAP<STRING, CLinkMgr*>  m_LinkMgrMap;

    //�����ļ���������
    int   m_nFileSaveDays;          //added by chenliangwei
    BOOL  m_bFileLenVerify;         //�Ƿ�����ļ�����У��
};

#endif //__COLLECT_BASE_H__
