#if !defined(_FORMAT_ACCESS_POINT_H_)
#define _FORMAT_ACCESS_POINT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CAccessPoint
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "channel.h"

class CFormatLib;
class CFormat;


//��ͨ��IDΪ������ͨ��MAP���Ͷ���
typedef MAP<UINT4, CChannel*> CHL_MAP;

typedef LIST<STRING> ORG_FMT_NAME_LIST;  //Added by ZhengYuqun 2004-01-18 SWPD03392

/****************** The CAccessPoint class Defining *****************/
// ������������ʽת����
//
/********************************************************************/
class CAccessPoint
{
public:

    //���캯��
    CAccessPoint(const STRING& sFmtRootDir, const UINT4 uAPID, 
                 CFormatLib* pFmtLib = NULL);

    //��������
    ~CAccessPoint(void);

    //��ʼ�������
    int Init(CFormatLib* pFmtLib = NULL);

    //��ý����ID
    UINT4 GetAPID(void) const;

    //��ý��������
    void GetAPName(STRING& sAPName) const;

    //�������������ļ��˺��������װ�˽���������ļ��ṹ
    int LoadConfig(void);

    //added by lucy 2004-06-29
    int LoadBinaryConfig(const STRING& sAPFile);
    int LoadAsciiConfig(const STRING& sAPFile);
    int VerifyFileFmt(const STRING& sAPFile);
    //end add

    
    //���ԭʼ������ʽ����
    UINT4 GetOrigLen(void) const;

    //��õ�ǰ������ָ��ͨ�������ջ�������
    UINT4 GetFinalLen(UINT4 uChlID) const;
    
    //��ñ������������ͨ�����ƣ�������ȱʡ��0ͨ��
    UINT4 GetChannelNames(LIST<STRING>& ChlNames) const;

    //��ñ�������ͨ����ΪsChlName��ͨ�����������ջ��������ʽ��
    UINT4 GetFinalBrowseNames(const STRING& sChlName, LIST<STRING>& Names, 
                              LIST<STRING>& Descs) const;

    //��ñ������������ԭʼ���������ʽ��
    UINT4 GetOrigBrowseNames(LIST<STRING>& Names, LIST<STRING>& Descs) const;    

    //�ּ�һ�Ż�������øû���������ͨ���͸�ʽ�������
    void Distribute(const void* pABill, const UINT4 uLen, UINT4& uChlID, 
                    CFormat*& pDestFmt);

    void SpecDistribute(const void* pABill, const UINT4 uLen, VECTOR<UINT4>& uChlID, 
					VECTOR<CFormat*>& pDestFmt);

    //���ý��������
    void SetAPName(const STRING& sAPName);

    //����һ�ֽ����
    int Create(void);

    //׷�ӱ�������ԭʼ���������ʽ��
    int AddBrowseNames(UINT4 nCount, const char* szFormats[]);

    //����ָ����ͨ��
    int AddChannel(UINT4 nChlID, const STRING& sChlName);

    //����ָ��ͨ�������һ�����ջ���Դ��ʽ�ķּ�����
    int SetDistingCondition(UINT4 nChlID, const STRING& sDisFormat,
                            const STRING& sDisCond);

    //��׷�ӵķ�ʽ����ָ��ͨ�������ջ�����ʽ�������ջ��������ʽ��
    int SetChlFormats(UINT4 nChlID, UINT4 nFinalFmtCount, const char* szFinalFormats[],
                      UINT4 nFinalBrowseFmtCount, const char* szFinalBrowseFmts[]);

    //ָ��ͨ����ţ���ø�ͨ�����е����ջ�����ʽ�������ջ��������ʽ��
    int GetChlFormats(const UINT4 nChlID, LIST<STRING>& FinalFmts,
                      LIST<STRING>& FinalBrowseFormats) const;

    //ɾ��ָ����ͨ��
    int DelChannel(UINT4 nChlID);

    //׷�ӱ�������ԭʼ������ʽ��
    int AddOrgFmtNames(UINT4 nCount, const char* szFormats[]);

    //��ñ������������ԭʼ������ʽ��
    UINT4 GetOrgFmtNames(LIST<STRING>& Names, LIST<STRING>& Descs) const;

    //ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ���浽������
    int Commit(void);

    //ɾ�������
    int Delete(void);

protected:

    UINT4        m_uAPID;               //�����ID
    STRING       m_sAPName;             //���������
    STRING       m_sFmtRootDir;         //��ʽ�����ø�Ŀ¼
    CHL_MAP*     m_pChannels;           //��ͨ��IDΪ������MAPָ��
    FMT_MAP*     m_pOrigFmtMaps;        //�Ը�ʽ��Ϊ������ԭʼ������ʽ�б�
    FMT_MAP*     m_pOrigBrowseFmtMaps;  //�Ը�ʽ��Ϊ������ֵ��ԭʼ���������ʽ�б�
    int          m_nAPType;             //���������
    CFormatLib*  m_pFmtLib;             //��ʽ����󣬲����ͷŸ�ָ��
    ORG_FMT_NAME_LIST* m_pOrgFmtNameList; //��������ʽ�������� Added by ZhengYuqun 2004-01-18 SWPD03392
};
/****************** End of The CAccessPoint class Defining **********/


#endif  //!defined(_FORMAT_ACCESS_POINT_H_)
