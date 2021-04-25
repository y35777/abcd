#if !defined(_FORMAT_IFORMAT_CONFIG_IMPLEMENT_H_)
#define _FORMAT_IFORMAT_CONFIG_IMPLEMENT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the IFormatCfgImp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "iformat_cfg.h"

class CIGWB;

/****************** The IFormatCfgImp Interface Defining ************/
// ������������ʽ���ýӿ�ʵ��
//
/********************************************************************/
class IGWB_Svc_Export IFormatCfgImp: public IFormatCfg
{
public:

    //���캯��
    IFormatCfgImp(CIGWB  *pIGWB);

    //��������
    virtual ~IFormatCfgImp();

    //���ݸ�ʽ�������ļ���ʼ����ʽ���ýӿ�
    virtual int Init(const UINT4 nAPID = 0);

    //�ڸ�ʽ��������һ������ΪsFmtName�Ļ�����ʽ
    virtual int AddFormat(const STRING& sFmtName);

    //ָ��һ�ָ�ʽ��sFmtName�����û��޸����ʽ����sFmtDes
    //�Ͷ�Ӧ��Դ��ʽ��sSrcFmtName
    virtual int SetFormatGen(const STRING& sFmtName, const STRING& sFmtDes, 
                             const STRING& sSrcFmtName);

    //���ָ����ʽ��sFmtName�����еĻ�������ϢMAPָ��
    virtual const FIELD_MAP* GetFieldsMap(const STRING& sFmtName);

    //���ָ����ʽ��sFmtName�����еĻ�������ϢSFieldInfo*����ָ��
    virtual const SFieldInfo** GetFieldsArray(const STRING& sFmtName);

    //����ָ����ʽ��ΪsFmtName�ĸ�ʽ��ĸ�ʽʶ������sDistingCondition
    virtual void SetDistingCondition(const STRING& sFmtName, 
                                     const STRING& sDistingCondition);

    //���ָ����ʽ��ΪsFmtName�ĸ�ʽ��ĸ�ʽʶ��������
    //����ʶ����������sDistingCondition
    virtual void GetDistingCondition(const STRING& sFmtName, 
                                     STRING& sDistingCondition);

    //ɾ��ָ����ʽ��ΪsFmtName�ĸ�ʽ��
    virtual int DelFormat(const STRING& sFmtName);

    //��ø�ʽ�������и�ʽ��
    virtual UINT4 GetFormats(LIST<STRING>& Formats);

    //���ָ����ʽ���ĸ�ʽ���һ����Ϣ������øø�ʽ��������Դ��ʽ��
    virtual void GetFormatGen(const STRING& sFmtName, STRING& sFmtDes, 
                              STRING& sSrcFmtName);

    //ָ��������ID����������ƣ�����һ�������ĸ�ʽ������Ϣ
    virtual int AddAccessPoint(const UINT4 nAPID, const STRING& sAPName);

    //ָ��������ID��׷�Ӹý�����ԭʼ���������ʽ��
    virtual int AddOrgBrowseNames(const UINT4 nAPID, const UINT4 nCount, 
                                  const char* szFormats[]);

    //ָ��������ID����øý��������ԭʼ���������ʽ��
    virtual UINT4 GetOrgBrowseNames(const UINT4 nAPID, LIST<STRING>& Formats);

    //ָ��������ID��ͨ��ID�����ƣ�����ָ����ͨ��
    virtual int AddChannel(const UINT4 nAPID, const UINT4 nChlID , 
                           const STRING& sChlName);

    //ָ��������ID��ͨ����ţ����ø�ͨ�������һ�����ջ���Դ��ʽ�ķּ�����
    virtual int SetDisCondition(const UINT4 nAPID, const UINT4 nChlID , 
                                const STRING& sDisFormat,
                                const STRING& sDisCondition);

    //ָ�������ID��ͨ����ţ���׷�ӵķ�ʽ���ø�ͨ�������ջ�����ʽ��
    //�����ջ��������ʽ��
    virtual int SetChlFormats(const UINT4 nAPID, const UINT4 nChlID , 
                              const UINT4 nFinalFmtCount, 
                              const char* szFinalFormats[], 
                              const UINT4 nFinalBrowseFmtCount, 
                              const char* szFinalBrowseFmts[]);

    //ָ�������ID��ͨ����ţ���ø�ͨ�����е����ջ�����ʽ�������ջ��������ʽ��
    virtual int GetChlFormats(const UINT4 nAPID, const UINT4 nChlID,
                              LIST<STRING>& FinalFmts,
                              LIST<STRING>& FinalBrowseFormats);

    //ָ�������ID��ͨ����ţ�ɾ����ͨ����������Ϣ
    virtual int DelChannel(const UINT4 nAPID, const UINT4 nChlID);

    //ָ�������ID����øý���������ͨ������
    virtual UINT4 GetChannels(const UINT4 nAPID, LIST<STRING>& ChlNames);

    //��ø�ʽ�����н��������
    virtual UINT4 GetAPNames(LIST<STRING>& APNames);

    //ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ���浽������
    virtual int Commit(void);

    //ָ��������ID��׷�Ӹý�����ԭʼ������ʽ��
    virtual int AddOrgFmtNames(const UINT4 nAPID, const UINT4 nCount, 
                               const char* szFormats[]);

    //ָ��������ID����øý��������ԭʼ������ʽ��
    virtual UINT4 GetOrgFmtNames(const UINT4 nAPID, LIST<STRING>& Formats);

    //ɾ��ָ�������ĸ�ʽ������Ϣ
    virtual int DelAccessPoint(UINT4 nAPID);

protected:

    CIGWB*  m_pIGWB;		//CIGWB����ָ��

};
/*************** End of The IFormatCfgImp Interface Defining ********/

#endif  //!defined(_FORMAT_IFORMAT_CONFIG_IMPLEMENT_H_)
