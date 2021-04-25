// com_collect.h: interface for the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __EWSD_AMAS_COLLECT_H_
#define __EWSD_AMAS_COLLECT_H_

#include "../collect_base.h"

class CEWSD_AMACollect;
class CLinkMgr;

//���˽������Ĳɼ���
class CEWSD_AMASCollect : public CCollectBase
{
public:
    CEWSD_AMASCollect(int nSwitchIndex, CLinkGroup* pLinkGroup);
    virtual ~CEWSD_AMASCollect();

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

    CLinkMgr* m_pLinkMgr;

    //������Ϣ
    STRING m_strError;

    VECTOR<CEWSD_AMACollect*>   m_AMAColList;
};

#endif // __EWSD_AMAS_COLLECT_H_
