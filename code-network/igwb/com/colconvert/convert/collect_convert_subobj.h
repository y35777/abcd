#ifndef __COLLECT_CONVERT_SUBOBJ_H__
#define __COLLECT_CONVERT_SUBOBJ_H__

/*
CCollectConvertSubObj�ǲɼ������Ӷ���Ļ���, �������˲ɼ�������صĻ������Ժͻ�������
*/
#include "../include/collect_convert_include.h"
#include "include/collect_convert_struct.h"
#include "include/collect_convert_define.h"

class IBillProcessor;

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
class  CCollectConvertSubObj 
{

public:
    CCollectConvertSubObj(int nSwitchIndex, 
                          int nSubObjIndex,
                          IBillProcessor* pIBillProcessor = NULL); //���캯������Э�����ָ��

    virtual ~CCollectConvertSubObj();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;


    virtual  int  Convert(SConvertFileReq& stReq);

    //��̬������Ŀ¼, ���麯��, �����������ʵ��
    virtual  int CreateLocSubDir();


    //��������ļ�, ���麯��, �����������ʵ��
    virtual  int ClearOldFile();


    //����Ŀ���ļ���·��
    virtual void ConstuctDestFileLst(SConvertFileReq& stReq);

    //����ʱĿ¼�µ��ļ��ƶ�����ʽĿ¼��
    virtual int RenameDestFile();

    //�����ڶ����ļ�
    virtual int CopySecondFile();

    virtual int ConvertFile(SConvertFileReq& stReq);

    virtual void OnFileConvertSucc(int nChannel);

    inline void SetCfgPath(const char* const szCfgPath)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }


    inline void SetSwitchName(const char* const szSwitchName)
    {
        strncpy(m_szSwitchName, szSwitchName, sizeof(m_szSwitchName));
        m_szSwitchName[sizeof(m_szSwitchName) - 1] = '\0';
    }


protected:

    virtual  STRING GetDstFileName(const char* szSrcName);

    virtual  STRING GetErrFileName(const char* szSrcName);

    CONVERTFILEFUN  m_pConvertFun;  //ת������ָ��



    char  m_szSwitchName[40];       //��������
    char  m_szCollectId[60];        //ָ������Ĳɼ�������"5ESS"�����ڴ���C5ESSCollect����
    int   m_nSwitchIndex;           //����������
    int   m_nSubObjIndex;           //�Ӷ�������
    char  m_szSubObjName[40];       //�Ӷ�������

    char  m_szCfgPath[MAX_PATH];    //��ǰ�����ļ�������·����Ϣ

    
    char m_szFinalRootPath[MAX_PATH];      //���ջ�����Ŀ¼
    char m_szFinalPath[MAX_PATH];          //���ջ���Ŀ¼
    char m_szSecondFinalPath[MAX_PATH];    //���ջ����ڶ��ݵ�Ŀ¼


    char m_szBillType[CFG_BUF_SIZE];       //��������

    int   m_nFileSaveDays;        //�����ļ���������


    int  m_bFileLevelVerify;       //�Ƿ�����ļ���У��
    int  m_bFileLevelFailReCol;    //�ļ���У��ʧ���Ƿ��ز�
    int  m_bBillLevelVerify;       //�Ƿ���л�����У��
    int  m_bBillLevelFailReCol;    //������У��ʧ���Ƿ��ز�

    int  m_bCrtZeroFnlFile;        //�Ƿ�����0�ֽڵ����ջ����ļ�
    int  m_nOrigBillLen;           //ԭʼ�����ĳ���

    char m_szCurDate[24];
    IBillProcessor* m_pIBillProcessor;
    LIST<STRING> m_ChlNames;

    VECTOR<STRING> m_FinalDirList;
    VECTOR<STRING> m_SecondDirList;

    //�Ƿ���Ҫ���Ƶڶ������ջ����ļ�
    BOOL m_bSecondCopy;

    //�Ƿ���Ҫ�ڶ��ݻ�������
    BOOL m_bNeedSecondAdapter;

    VECTOR<STRING> m_FinalFileList;
    VECTOR<int>    m_CvtResultList;

};

#endif //__S1240_TP_COLLECT_CONVERT_H__

