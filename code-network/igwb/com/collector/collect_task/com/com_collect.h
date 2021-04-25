// com_collect.h: interface for the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __COM_COLLECT_H_
#define __COM_COLLECT_H_

#include "../collect_base.h"

class CComDirCollect;
class CLinkMgr;

//���˽������Ĳɼ���
class CComCollect : public CCollectBase
{
public:
    CComCollect(int nSwitchIndex, CLinkGroup* pLinkGroup);
    virtual ~CComCollect();

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

    //����ļ����к��Ƿ�������������©���ļ������麯��, �����������ʵ��
    virtual  BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);

    //���Ӳɼ�������
    virtual  int  Connect();

    //�Ͽ�����
    virtual  int  Disconnect();

    //ָ�����ļ��Ƿ�ɲ�
    virtual int  CanGetSpecFile(const char* const szModule,
                                const char* const szSrcFile);

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

    CLinkMgr* m_pLinkMgr;

    //������Ϣ
    STRING m_strError;

    VECTOR<CComDirCollect*>   m_DirColList;
};

#endif // __COM_COLLECT_H_
