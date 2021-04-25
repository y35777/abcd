#if !defined(_FORMAT_CHANNEL_H_)
#define _FORMAT_CHANNEL_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CChannel
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CFormat;
class CDisCondition;
class CFormatLib;

//��ʽ�����ʽ�����Ӧ��ϵ�ṹ����
typedef struct _FmtMap
{
	STRING      sFmtName;	 //��ʽ��
	CFormat*    pFmt;		 //��ʽ����
} SFmtMap, *PFmtMap;

//��ʽ�����ʽ�����Ӧ��ϵ���Ͷ���
typedef MAP<STRING, SFmtMap*> FMT_MAP;

typedef LIST<STRING> FINAL_FMT_NAME_LIST; //Added by ZhengYuqun 2004-01-18 SWPD03392

/****************** The CChannel class Defining *********************/
// ����������ͨ����
//
/********************************************************************/
class CChannel
{
public:

    //���캯��
    CChannel(const STRING& sFmtRootDir, const UINT4 uAPID, const UINT4 uChlID,
             CFormatLib *pFmtLib = NULL);

    //��������
    ~CChannel();

    //��ʼ��
    int Init(CFormatLib *pFmtLib = NULL);

    //����ͨ�������������ڴ�
    int LoadConfig(void);

    //��õ�ǰͨ�������ջ�������
    UINT4 GetFinalLen() const;

	int LoadBinaryConfig(const STRING& sChlFile);
	int LoadAsciiConfig(const STRING& sChlFile);
	int VerifyFileFmt(const STRING& sChlFile);

    //���ͨ��������
    void GetChlName(STRING&  sChlName) const;

    //���ͨ����ID
    UINT4 GetChlID(void) const;

    //��ñ�ͨ���������ջ��������ʽ��
    UINT4 GetFinalBrowseNames(LIST<STRING>& Names, LIST<STRING>& Descs) const;

    //�жϴ����һ�Ż���pABill�������ڱ�ͨ��
    int IsTheChannel(const void* pABill, const UINT4 uLen, CFormat*& pDestFmt);

    //����ͨ������
    void SetChlName(const STRING& sChlName);

    //����һ���µ�ͨ��
    int Create(void);

    //���ñ�ͨ�������һ�����ջ���Դ��ʽ�ķּ�����
    int SetDistingCondition(const STRING& sDisFormat, const STRING& sDisCond);

    //��׷�ӵķ�ʽ���ñ�ͨ�������ջ�����ʽ�������ջ��������ʽ��
    int AddChlFormats(UINT4 nFinalFmtCount, const char* szFinalFormats[],
                      UINT4 nFinalBrowseFmtCount, const char* szFinalBrowseFmts[]);

    //��ñ�ͨ�����е����ջ�����ʽ�������ջ��������ʽ��
    int GetChlFormats(LIST<STRING>& FinalFmts, LIST<STRING>& FinalBrowseFormats) const;

    //ɾ��ͨ��
    int Delete(void);

    //ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ���浽������
    int Commit(void);


protected:

    UINT4           m_uChlID;		        //ͨ��ID
    UINT4           m_uAPID;		        //��ͨ�������Ľ����ID
    STRING          m_sChlName;		        //ͨ������
    STRING          m_sFmtRootDir;          //��ʽ���Ŀ¼·��
    CDisCondition*  m_pDisCondition;        //��ͨ����Ӧ�ķּ���������
    CFormatLib*     m_pFmtLib;              //��ʽ�����
    FMT_MAP*        m_pFinalFmtMaps;        //��ͨ����Ӧ�����ջ�����ʽ�����б�
    FMT_MAP*        m_pFinalBrowseFmtMaps;  //��ͨ����Ӧ�����ջ��������ʽ�б�
    
    FINAL_FMT_NAME_LIST* m_pFinalFmtNameList; //��������ʽ�������� Added by ZhengYuqun 2004-01-18 SWPD03392    
};

/****************** End of The CChannel class Defining **************/


#endif  //!defined(_FORMAT_CHANNEL_H_)
