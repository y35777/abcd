/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200
 *ģ����    ���ɼ�ת��ģ��(collect_convertor)
 *�ļ���    : colloect_convertor.h
 *������Ϣ  �����ļ�����CCollectConvertorModule��ʵ��
 *�汾˵��  ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ��������
 *����ʱ��  ��2003-03-19
 *�޸ļ�¼  ��
 **************************************************************************/

#ifndef __COLLECT_CONVERTOR_H__
#define __COLLECT_CONVERTOR_H__

#include "../include/frame.h"
#include "../include/toolbox.h"
#include "../include/collect_msg_def.h"

#include "collect_convert_obj_factory.h"
#include "collect_convert_obj.h"

class CCollectConvertorModule : public CWorkModule
{
public:
    // ���캯��
    CCollectConvertorModule(void);

    // ��������
    virtual ~CCollectConvertorModule();

    static char g_szColAppName[MAX_PATH];

protected:

    // ���������ַ�����
    virtual void ParseMsg (MSG_INTRA* pMsgParsed);

    // ��ʼ������
    virtual int InitInstance(void);

    // ��������������
    virtual void ExitInstance(void);

    // ��ȡ��ģ�����ͺ���
    virtual MODULE_TYPE GetModuleType(void);

    virtual void OnTimer(const long nTimerID);


    //�������ļ���ת��
    int ProcessBillConvert();

    //����ת��������ɼ�ģ��
    void SendConvertResult(int nCmdCode);

    int SaveStatus();

    int LoadStatus();

    VECTOR<CCollectConvertObj*>* m_pConvertObjList;

    enum CO_MD_STATUS
    {
        CM_FAILURE  = -1,
        CM_DEACTIVE = 0,
        CM_RESUME   = 1,
        CM_ACTIVE   = 2
    };

    CO_MD_STATUS  m_nStatus;

    enum FILE_CONVERT_STATUS
    {
        CS_NO_CONVERT      = 0,
        CS_ALREADY_CONVERT = 1
    };

    int m_nConvertStatus;

    int m_nConvertResult;

    int m_nFileNum;

    SSwitchFileInfo m_FileInfo;

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;

};

#endif //__COLLECT_CONVERTOR_H__


