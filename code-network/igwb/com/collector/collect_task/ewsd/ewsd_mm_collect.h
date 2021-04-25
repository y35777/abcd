// ewsd_mm_collect.h: interface for the CEWSD_MMCollect class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _EWSD_MM_COLLECT_H_
#define _EWSD_MM_COLLECT_H_

#include "../collect_base.h"

class CEWSD_MMCollect : public CCollectBase
{
public:
    CEWSD_MMCollect(int nSwitchIndex, CLinkGroup* pLinkGroup);
    virtual ~CEWSD_MMCollect();
    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Col_Init();

    //�����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��;
    virtual  int  Col_GetFile(SColFileReq& stReq);

    //����ļ��б�, ���麯��, �����������ʵ��
    virtual  int  Col_GetList(SListOut& pOut);

    //��̬������Ŀ¼, ���麯��, �����������ʵ��
    virtual  BOOL CreateLocSubDir();

    //��������ļ�, ���麯��, �����������ʵ��
    virtual  BOOL ClearOldFile();

    int GetFileListIndex(const char* const szSrcFile);

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

protected:
    BOOL GetSubName(const char *sInfoLine,
                    SFileInfo& szFileInfo );

    STRING GetDstFileName(const char* const szSrcFile);

    //������ŵĸ�Ŀ¼
    STRING m_strLocalRootPath;
    //������ŵĵ�ǰĿ¼
    STRING m_strLocalPath;

    //ָʾ�ļ���ŵĸ�Ŀ¼
    STRING m_strFNARootPath;
    //ָʾ�ļ���ŵĵ�ǰĿ¼
    STRING m_strFNAPath;
    STRING m_strFnaFile;
    STRING m_strFnaIndex;
    STRING m_strBillIndex;

    //��ʱ�����ļ���ŵ�Ŀ¼
    STRING m_strTmpPath;
    //Զ��5ESS��������ԴĿ¼
    STRING m_strRemotePath;

    //������Ϣ
    STRING m_strError;

    //��ǰʱ��
    STRING m_strCurDate;

    char m_szAreaCode[50]; //����
    char m_szBillType[50]; //��������

    VECTOR <SFileInfo> m_FileList;

    ACE_Mutex          m_MutexForList;
};

#endif // _EWSD_MM_COLLECT_H_
