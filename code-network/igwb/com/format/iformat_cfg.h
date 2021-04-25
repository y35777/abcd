#if !defined(_FORMAT_IFORMAT_CONFIG_H_)
#define _FORMAT_IFORMAT_CONFIG_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the IFormatCfg
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

/****************** The IFormatCfg Interface Defining ***************/
// ������������ʽ���ýӿ�
//
/********************************************************************/
class IGWB_Svc_Export IFormatCfg
{
public:

    //������������
    virtual ~IFormatCfg(){};

    //���ݸ�ʽ�������ļ���ʼ����ʽ���ýӿ�
    virtual int Init(const UINT4 nAPID = 0) = 0;

    //�ڸ�ʽ��������һ������ΪsFmtName�Ļ�����ʽ
    virtual int AddFormat(const STRING& sFmtName) = 0;

    //ָ��һ�ָ�ʽ��sFmtName�����û��޸����ʽ����sFmtDes
    //�Ͷ�Ӧ��Դ��ʽ��sSrcFmtName
    virtual int SetFormatGen(const STRING& sFmtName, const STRING& sFmtDes, 
                             const STRING& sSrcFmtName) = 0;

    //���ָ����ʽ��sFmtName�����еĻ�������ϢMAPָ��
    virtual const FIELD_MAP* GetFieldsMap(const STRING& sFmtName) = 0;

    //���ָ����ʽ��sFmtName�����еĻ�������ϢSFieldInfo*����ָ��
    virtual const SFieldInfo** GetFieldsArray(const STRING& sFmtName) = 0;

    //����ָ����ʽ��ΪsFmtName�ĸ�ʽ��ĸ�ʽʶ������sDistingCondition
    virtual void SetDistingCondition(const STRING& sFmtName, 
                                     const STRING& sDistingCondition) = 0;

    //���ָ����ʽ��ΪsFmtName�ĸ�ʽ��ĸ�ʽʶ��������
    //����ʶ����������sDistingCondition
    virtual void GetDistingCondition(const STRING& sFmtName, 
                                     STRING& sDistingCondition) = 0;

    //ɾ��ָ����ʽ��ΪsFmtName�ĸ�ʽ��
    virtual int DelFormat(const STRING& sFmtName) = 0;

    //��ø�ʽ�������и�ʽ��
    virtual UINT4 GetFormats(LIST<STRING>& Formats) = 0;

    //���ָ����ʽ���ĸ�ʽ���һ����Ϣ������øø�ʽ��������Դ��ʽ��
    virtual void GetFormatGen(const STRING& sFmtName, STRING& sFmtDes, 
                              STRING& sSrcFmtName) = 0;

    //ָ��������ID����������ƣ�����һ�������ĸ�ʽ������Ϣ
    virtual int AddAccessPoint(const UINT4 nAPID, const STRING& sAPName) = 0;

    //ָ��������ID��׷�Ӹý�����ԭʼ���������ʽ��
    virtual int AddOrgBrowseNames(const UINT4 nAPID, const UINT4 nCount, 
                                  const char* szFormats[]) = 0;

    //ָ��������ID����øý��������ԭʼ���������ʽ��
    virtual UINT4 GetOrgBrowseNames(const UINT4 nAPID, LIST<STRING>& Formats) = 0;

    //ָ��������ID����øý��������ԭʼ���������ʽ��
    virtual int AddChannel(const UINT4 nAPID, const UINT4 nChlID , 
                           const STRING& sChlName) = 0;

    //ָ��������ID��ͨ����ţ����ø�ͨ�������һ�����ջ���Դ��ʽ�ķּ�����
    virtual int SetDisCondition(const UINT4 nAPID, const UINT4 nChlID , 
                                const STRING& sDisFormat,
                                const STRING& sDisCondition) = 0;

    //ָ�������ID��ͨ����ţ���׷�ӵķ�ʽ���ø�ͨ�������ջ�����ʽ��
    //�����ջ��������ʽ��
    virtual int SetChlFormats(const UINT4 nAPID, const UINT4 nChlID , 
                              const UINT4 nFinalFmtCount, 
                              const char* szFinalFormats[], 
                              const UINT4 nFinalBrowseFmtCount, 
                              const char* szFinalBrowseFmts[]) = 0;

    //ָ�������ID��ͨ����ţ���ø�ͨ�����е����ջ�����ʽ�������ջ��������ʽ��
    virtual int GetChlFormats(const UINT4 nAPID, const UINT4 nChlID , 
                              LIST<STRING>& FinalFmts,
                              LIST<STRING>& FinalBrowseFormats) = 0;

    //ָ�������ID��ͨ����ţ�ɾ����ͨ����������Ϣ
    virtual int DelChannel(const UINT4 nAPID, const UINT4 nChlID) = 0;

    //ָ�������ID����øý���������ͨ������
    virtual UINT4 GetChannels(const UINT4 nAPID, LIST<STRING>& ChlNames) = 0;

    //��ø�ʽ�����н��������
    virtual UINT4 GetAPNames(LIST<STRING>& APNames) = 0;

    //ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ���浽������
    virtual int Commit(void) = 0;

    //ָ��������ID��׷�Ӹý�����ԭʼ������ʽ��
    virtual int AddOrgFmtNames(const UINT4 nAPID, const UINT4 nCount, 
                               const char* szFormats[]) = 0;

    //ָ��������ID����øý��������ԭʼ������ʽ��
    virtual UINT4 GetOrgFmtNames(const UINT4 nAPID, LIST<STRING>& Formats) = 0;

    //ɾ��ָ�������ĸ�ʽ������Ϣ
    virtual int DelAccessPoint(UINT4 nAPID) = 0;

};
/****************** End of The IFormatCfg Interface Defining ********/

#endif  //!defined(_FORMAT_IFORMAT_CONFIG_H_)
