#if !defined(_FORMAT_IQUERY_BROWSE_IMPLEMENT_H_)
#define _FORMAT_IQUERY_BROWSE_IMPLEMENT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the IQueryBrowseImp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "iquery_browse.h"

class CFormat;
class CIGWB;

/****************** The IQueryBrowseImp Interface Defining **************/
// ����������������ѯ����ӿ�ʵ��
//
/************************************************************************/
class IGWB_Svc_Export IQueryBrowseImp: public IQueryBrowse
{
public:

    //���캯��
    IQueryBrowseImp(CIGWB* pIGWB);

    //��������
    virtual ~IQueryBrowseImp();

    //���������ѯ�ӿڳ�ʼ��
    virtual int Init(const UINT4 nAPID = 0);

    //��ø�ʽ�����õ����н��������
    virtual UINT4 GetAPNames(LIST<STRING>& APNames) const;

    //�����ΪsAPName�Ľ���������ͨ������
    virtual UINT4 GetChlNames(const STRING& sAPName, LIST<STRING>& ChlNames) const;

    //�������ΪsAPName�Ľ���������ԭʼ���������ʽ������Ӧ�ĸ�ʽ����
    virtual UINT4 GetOrigBrowseNames(const STRING& sAPName, LIST<STRING>& Names,
                                     LIST<STRING>& Descs) const;

    //����һ����������ƣ����ĳ��ͨ�����������ջ��������ʽ������Ӧ�ĸ�ʽ����
    virtual UINT4 GetFinalBrowseNames(const STRING& sAPName, const STRING& sChlName,
                                      LIST<STRING>&  Names, LIST<STRING>& Descs) const;

    //�������ΪsBrowseName�������ʽ�ĸ�ʽ�������
    virtual CFormat* GetBrowseFmt(const STRING& sBrowseName);

protected:

    CIGWB*   m_pIGWB;    //CIGWB����ָ��

};
/*************** End of The IQueryBrowseImp Interface Defining **********/

#endif  //!defined(_FORMAT_IQUERY_BROWSE_IMPLEMENT_H_)
