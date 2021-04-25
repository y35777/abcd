#if !defined(_FORMAT_FORMAT_H_)
#define _FORMAT_FORMAT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CFormat
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CCondition;
class CConvert;

/****************** The CFormat class Defining **********************/
// ����������������ʽ��
//
/********************************************************************/
class IGWB_Svc_Export CFormat
{
public:

    //���캯��
    CFormat(const STRING& sRootDir, const STRING& sFmtName);

    //����ʽ������Ϣ�����ڴ溯��
    int LoadConfig(void);

    //���㱾��ʽ�ֽڵĳ��ȣ��ú���Ŀǰ�ò�����
    UINT4  CaculateFmtLen(void);

    //����һ�ֻ�����ʽ
    int Create(void);

    //ɾ��һ�ֻ�����ʽ
    int Delete(void);

    //��ñ���ʽ��
    void GetFmtName(STRING& sFmtName) const;
    const char* GetFmtName() const;

    //���ñ���ʽ��Ӧ��Դ��ʽ����
    int SetSrcFmt(CFormat*  pSrcFmt);

    //��ñ���ʽ��Ӧ��Դ��ʽ����
    CFormat* GetSrcFmt(void) const;

    //��ñ���ʽ��Դ��ʽ��
    void GetSrcFmtName(STRING& sSrcFmtName) const;
    const char* GetSrcFmtName() const;


    //��ñ���ʽ��Ӧ�ĵ��Ż������ȣ��ֽ�����
    UINT4 GetFmtLen(void) const;
    
	//��ñ���ʽ��Ӧ��Դ��ʽ���ȵ��Ż������ȣ��ֽ�����
	UINT4 GetSrcFmtLen(void) const;

    //��ñ���ʽ�����
    UINT4 GetFieldNum(void) const;

    //��ñ���ʽ���л�����MAPָ��
    const FIELD_MAP* GetFieldsMap(void) const;

    //��ñ���ʽ���л���������ָ��
    const SFieldInfo** GetFieldsArray(void) const;

    //��ñ���ʽ�ĸ�ʽ������Դ��ʽ��
    void GetFormatGen(STRING& sFmtDesc, STRING&  sSrcFmtName) const;

    //���ñ���ʽ�ĸ�ʽ������Դ��ʽ��
    void SetFormatGen(const STRING& sFmtDesc,  const STRING& sSrcFmtName);

    //���ñ���ʽ��ʶ������
    void SetDistingCondition(const STRING& sDistingCondition);

    //��ñ���ʽ��ʶ������
    void GetDistingCondition(STRING& sDistingCondition) const;
    
    //���ݸ�ʽ��������һ�Ż����л�ø����ֵ
    //2004-02-18  SWPD03631 ���Ӳ���pDestFIʵ�ֶ�����ȡö��ֵ ZhengYuqun
    int GetFieldValueByIndex(const void* pABill, const int bThisFmt, 
                             const UINT4 uIndex,
                             void* pFieldValue, UINT4& uLen, SFieldInfo* pDestFI = NULL);

    //ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ���浽������
    int Commit(void);

    //���ò�ѯ����
    int SetQueryCond(const STRING& sQueryCond);

    //��ȡ���������͵ĳ���
    static int GetFieldTypeLen(const SFieldInfo* pFI);

	
	//�������ʽ����ȡ����
	int ParseExpress(const char* const  szExpress, LIST<STRING>& FieldList);

public:

    //��������
    virtual ~CFormat();

    //��ʼ������
    virtual int Init(void);

    //����һ����ʽ����������ֽ�ƫ�ơ�λƫ���Լ��ֽ��ܳ��Ⱥ���
    virtual int InitFormatInfo(void);
    
    //�жϴ����һ�Ż����Ƿ���ϱ���ʽ
    virtual int IsTheFormat(const void* pABill,  const UINT4 uLen);

    //�������һ�Ż���ת����Ŀ���ʽ
    virtual int ConvertABill(const void* pASrcBill, const UINT4 uSrcBillLen, 
                             void*& pADestBill, UINT4& uDestBillLen);

    //���ݻ���������һ�Ż����л�ø����ֵ
    virtual int GetFieldValueByName(const void* pABill, const int bThisFmt,
                                    const STRING& sFieldName,  
                                    void*& pFieldValue, UINT4& uLen,
                                    int nIsQuery = 0);

	//���ݻ�������Ŵ�һ�Ż����л�ø����ֵ
	virtual int GetFieldContentByIndex(const void* pABill, const int bThisFmt,
		                               UINT2 nFieldIndex,  
		                               void* pFieldValue, UINT4& uLen,
		                               int nIsQuery);
    //�ж�һ�Ż����Ƿ����ָ���Ĳ�ѯ����
    virtual int IsTheQueryCondition(const void* pASrcBill, const STRING& sQueryCond = "");

	//��Intel����Net�ֽ���תΪ�����ֽ���
    //Modified by XDB on 2002.10.28
	static int ConvertToHostOrder(const SFieldInfo* pFI, void* pFieldValue);

    void SetMustMerge(BOOL bMustMerge);

protected:
    STRING         m_sFmtRootDir;		//��ʽ��ĸ�·��
    STRING         m_sFmtName;	        //��ʽ����
    STRING         m_sFmtDesc;		    //��ʽ����
    STRING         m_sSrcFmtName;		//����ʽ��Ӧ��Դ��ʽ��
    STRING         m_sDistingCondition;	//��ʽʶ���������ַ�����ʽ����׺���ʽ��
                                        //�Ӹ�ʽ���ж���
    STRING         m_sQueryCond;        //��ѯ�������ַ�����ʽ����׺���ʽ

    FIELD_MAP*     m_pFieldsMap;		//�Ի���������Ϊ����MAP
    CCondition*    m_pDistingCondition;	//��Ϊʶ�������ĺ�׺���ʽ����
    CFormat*       m_pSrcFmt;		    //����ʽ��Դ��ʽ����
    CConvert*      m_pConvert;		    //��ʽת������
    CCondition*    m_pQueryCondition;	//��Ϊ��ѯ�����ĺ�׺���ʽ���� 
    UINT4          m_uFmtLen;		    //����ʽ���л������ֽ��ܳ���
    UINT4          m_uFieldNum;		    //����ʽ�������ܸ���
    SFieldInfo**   m_pFieldsArray;		//�������������飬������ž��ǻ��������ţ�
                                        //���鳤�Ⱦ��Ǹ�ʽ�����Ļ��������

	CCondition*    m_pStatCondition;	//��Ϊͳ�������ĺ�׺���ʽ����
    BOOL           m_bIsMustMerge;

	//����ƽ 2002��4��4����������������Ա����
	//����iGateway Bill V200�ĸ�ʽ�����ڱ�����������ļ�ʱ��ͳһ�������ֽ���1�ֽ�
	//���뷽ʽ���棬��NTƽ̨�£���ʽ����԰�1�ֽڶ�����룬����UNIXƽ̨�£�����ʹ��
	//1�ֽڶ�����룬��˸�ʽ���ڼ��������ļ�ʱ���봦���ֽ�����ֽڶ�������
private:
    void  SerialFieldInfo(SFieldInfo* pOut, BYTE* pIn); 
	void  SerialEnumInfo(SEnumInfo* pOut, BYTE* pIn);
};
/****************** End of The CFormat class Defining ***************/

#endif  //!defined(_FORMAT_FORMAT_H_)
