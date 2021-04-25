#ifndef __BK_LINK_MGR_H__
#define __BK_LINK_MGR_H__

#include "link_mgr.h"

//�������ļ��������Ķ���
class  CBKLinkMgr : public CLinkMgr
{

public:

    CBKLinkMgr(CLinkGroup* pLinkGroup)
              :CLinkMgr(pLinkGroup)
    {
        //
    }

    virtual  ~CBKLinkMgr()
    {
        //
    }

    //��ʼ�����ط���, ���麯��, �����������ʵ�ָ÷���
    virtual int  Init(void *pPara = NULL){return ERR_SUCCESS;}

    //ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
    virtual int  GetFile(const SGetFileReq &pReq);

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    virtual int  GetFileList(const char* szSrcDir, SListOut &pOut);

    //ɾ���ļ�����˵�ָ���ļ�
    virtual int  DelFile(const char* szFileName);

    //ȡ�ļ�����˵�ָ���ļ��ĳ���
    virtual int  GetFileLen(const char* szFileName,
                            unsigned long& uFileLen);

    //������ָ���ļ�
    virtual int  RenameFile(const char *szOld, const char *szNew);

    //�����ļ������
    virtual int  Connect();

    //�Ͽ�����
    virtual int  Disconnect();

    //�����ļ�����ģʽ
    virtual int  SetMode(int nMode);

protected:

    //int m_nActiveLink;
};

#endif //__BK_LINK_MGR_H__
