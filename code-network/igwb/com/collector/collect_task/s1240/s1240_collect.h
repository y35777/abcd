#ifndef __S1240_COLLECT_H__
#define __S1240_COLLECT_H__

#include "../collect_base.h"
#include "../../include/i_cmise_fun.h"

class CS1240TPCollect;

class CS1240Collect : public CCollectBase
{
public:
    CS1240Collect(int nSwitchIndex, 
                  CLinkGroup* pLinkGroup,
                  CS1240CmiseGroup* pCmiseGroup); //���캯������Э�����ָ��
    virtual ~CS1240Collect();

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

    //����ļ����к��Ƿ����������麯��, �����������ʵ��
    virtual  BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);


    //�������ӷ����
    virtual  int  TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus);

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

protected:
    MAP<int, CS1240TPCollect*>  m_TPMap;
    CS1240CmiseGroup*           m_pCmiseGroup;
    SS1240SwitchInfo            m_SwitchInfo;          //������б�
};


#endif //__S1240_COLLECT_H__
