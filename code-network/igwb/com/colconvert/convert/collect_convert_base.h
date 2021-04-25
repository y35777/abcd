#ifndef __COLLECT_CONVERT_BASE_H__
#define __COLLECT_CONVERT_BASE_H__

#include "include/collect_convert_struct.h"
#include "../include/i_collect_convert.h"
#include "../include/collect_convert_include.h"
#include "collect_convert_subobj.h"

/*
CCollectConvertBase�ǲɼ������Ļ���, �������˲ɼ�������صĻ������Ժͻ�������
*/
class CIGWB;
class IBillProcessor;

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
class  CCollectConvertBase : public ICollectConvert
{

public:
    CCollectConvertBase(int nSwitchIndex); //���캯������Э�����ָ��
    virtual ~CCollectConvertBase();

    //��ʼ��, ���麯��, �����������ʵ��
    virtual  int  Init();

    virtual  int  Process(const char* szBillFile, 
                          const int& nConvertInfo);

    virtual  int  Convert(SConvertFileReq& stReq);


    //��̬������Ŀ¼, ���麯��, �����������ʵ��
    virtual  int CreateLocSubDir();

    //added by chenliangwei
    //���ָ��Ŀ¼�µĹ�����Ŀ¼����������ʹ�õĹ��ߺ���
    static int ClearOldDateSubDir(const char* szRootDir, const int nSaveDays);

    //�õ�����ָ��Ŀ¼�µ��ļ��б�
    static int GetLocalFileList(const char* szSrcDir, SListOut &pOut);
    //added end

    //��������ļ�, ���麯��, �����������ʵ��
    virtual  int ClearOldFile();

    inline void SetCfgPath(const char* const szCfgPath)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }

protected:

    MAP<int, CCollectConvertSubObj*>  m_SubObjMap;


    int   m_nSwitchIndex;
    char  m_szSwitchName[40];       //��������
    char  m_szCollectId[40];        //ָ������Ĳɼ�������"5ESS"�����ڴ���C5ESSCollect����

    char  m_szCfgPath[MAX_PATH];    //��ǰ�����ļ�������·����Ϣ

    UINT4  m_nAPID;

    CIGWB* m_pIGWB;
    IBillProcessor* m_pIBillProcessor ;

};

#endif //__COLLECT_CONVERT_BASE_H__

