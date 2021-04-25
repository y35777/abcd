// 5ess_collect.h: interface for the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _5ESS_COLLECT_H_
#define _5ESS_COLLECT_H_

#include "../collect_base.h"

//added by chenliangwei
#define PREFIX_AMA_FILE     "stream"
#define LEN_OF_AMA_FILE_SN  4

class CFileSNMgr;
//added end

class C5ESSCollect : public CCollectBase
{
public:
    C5ESSCollect(int nSwitchIndex, CLinkGroup* pLinkGroup);
    virtual ~C5ESSCollect();
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

    //added by chenliangwei
    //����ļ����к��Ƿ�������������©���ļ������麯��, �����������ʵ��
    virtual  BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);
    //added end

    //ָ�����ļ��Ƿ�ɲ�
    int CanGetSpecFile(const char* const szModule,
                       const char* const szSrcFile);
    //�ɼ�ָ�����ļ�
    virtual  int  GetSpecFile(const char* const szModule,
                              const char* const szSrcFile,
                              const char* const szDstFile);
    //��ȡ��������Ϣ
    virtual  int  GetSwitchInfo(LIST<STRING>& label,
                                LIST<STRING>& content);

    //��ȡģ����Ϣ
    virtual  int  GetModuleInfo(const char* const szModule,
                                LIST<STRING>& label,
                                LIST<STRING>& content);

    //��ȡ�ļ�����
    virtual int  GetFileLen(const char* const szModule,
                            const char* const szSrcFile);

    int MakeFileNameMsg(const char* const szSrcFile,
                        SGetFileReq& req);

    int GetFile(SGetFileReq& stReq);

    //�ļ��������Ļص�������ͨ���ļ������кż���©���ļ���
    static STRING GetFileNameBySN(const int   nFileSN,
                                  const char* szReferName,
                                  const int   nSNLen,
                                  const int   nPostfixLen);


protected:

    BOOL GetSubName(const char *sInfoLine,
                    SFileInfo& szFileInfo );

    int GetPrefixIndex(const char* const szSrcFile);
    int GetFileListIndex(const char* const szSrcFile);

    STRING GetDstFileName(const char* const szSrcFile);

    //���ֻ�����ŵ���ϸĿ¼��
    VECTOR<STRING> m_vBillSavePath;
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

    //��ʱ�����ļ���ŵ�Ŀ¼
    STRING m_strTmpPath;
    //Զ��5ESS��������ԴĿ¼
    STRING m_strRemotePath;

    //�ļ�ǰ׺������
    VECTOR<STRING> m_vFilePrefix;

    //������Ϣ
    STRING m_strError;

    //��ǰʱ��
    STRING m_strCurDate;

    VECTOR<STRING> m_vAreaCode; //����
    VECTOR<STRING> m_vBillType; //��������
    VECTOR<STRING> m_vLabel;    //��ʶ

    //added by chenliangwei
    int m_bCheckSN;

    CFileSNMgr* m_pFileSNMgr;

    //�ȽϺ����࣬���ڶ�fna�е��ļ���Ϣ��������
    class Compare
    {
    public:
        BOOL operator ()(const SFileInfo &lfileinfo, const SFileInfo &rfileinfo);
    };//�������

    //added end

    VECTOR <SFileInfo> m_FileList;

    ACE_Mutex          m_MutexForList;
};

#endif // _5ESS_COLLECT_H_
