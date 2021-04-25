#ifndef __EWSD_MM_COLLECT_CONVERT_H__
#define __EWSD_MM_COLLECT_CONVERT_H__

/*
CEWSDAMASCollectConvert��ewsdama�ɼ�������, �������˲ɼ�������صĻ������Ժͻ�������
*/
#include "../../include/collect_convert_include.h"
#include "../include/collect_convert_struct.h"

#include "../collect_convert_base.h"

//�����ǲɼ���������CEWSDAMASCollectConvert�Ķ���
class  CEWSDMMCollectConvert : public CCollectConvertBase
{

public:
    CEWSDMMCollectConvert(int nSwitchIndex); //���캯������Э�����ָ��
    virtual ~CEWSDMMCollectConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;

    virtual  int  Convert(SConvertFileReq& stReq);


    //��̬������Ŀ¼, ���麯��, �����������ʵ��
    virtual  int CreateLocSubDir();

    //��������ļ�, ���麯��, �����������ʵ��
    virtual  int ClearOldFile();

protected:
    virtual  STRING GetDstFileName(const char* szSrcName);

    virtual  STRING GetErrFileName(const char* szSrcName);

    CONVERTFILEFUN  m_pConvertFun;  //ת������ָ��


    char m_szFinalRootPath[MAX_PATH];      //���ջ�����Ŀ¼
    char m_szFinalPath[MAX_PATH];          //���ջ���Ŀ¼
    char m_szSecondFinalPath[MAX_PATH];    //���ջ����ڶ��ݵ�Ŀ¼


    char m_szBillType[CFG_BUF_SIZE];       //��������

    int   m_nFileSaveDays;        //�����ļ���������


    int  m_bFileLevelVerify;       //�Ƿ�����ļ���У��
    int  m_bFileLevelFailReCol;    //�ļ���У��ʧ���Ƿ��ز�
    int  m_bBillLevelVerify;       //�Ƿ���л�����У��
    int  m_bBillLevelFailReCol;    //������У��ʧ���Ƿ��ز�

};

#endif //__EWSD_MM_COLLECT_CONVERT_H__

