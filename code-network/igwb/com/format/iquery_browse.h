#if !defined(_FORMAT_IQUERY_BROWSE_H_)
#define _FORMAT_IQUERY_BROWSE_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the IQueryBrowse
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CFormat;

/****************** The IQueryBrowse Interface Defining *****************/
// ����������������ѯ����ӿ�
//
/************************************************************************/
class IGWB_Svc_Export IQueryBrowse
{
public:

    //������������
    virtual ~IQueryBrowse(){};

    //���������ѯ�ӿڳ�ʼ��
    virtual int Init(const UINT4 nAPID = 0) = 0;

    //��ø�ʽ�����õ����н��������
    virtual UINT4 GetAPNames(LIST<STRING>& APNames) const = 0;

    //�����ΪsAPName�Ľ���������ͨ������
    virtual UINT4 GetChlNames(const STRING& sAPName, LIST<STRING>& ChlNames) const = 0;

    //�������ΪsAPName�Ľ���������ԭʼ���������ʽ������Ӧ�ĸ�ʽ����
    virtual UINT4 GetOrigBrowseNames(const STRING& sAPName, LIST<STRING>& Names,
                                     LIST<STRING>& Descs) const = 0;

    //����һ����������ƣ����ĳ��ͨ�����������ջ��������ʽ������Ӧ�ĸ�ʽ����
    virtual UINT4 GetFinalBrowseNames(const STRING& sAPName, const STRING& sChlName,
                                      LIST<STRING>&  Names, LIST<STRING>& Descs) const = 0;

    //�������ΪsBrowseName�������ʽ�ĸ�ʽ�������
    virtual CFormat* GetBrowseFmt(const STRING& sBrowseName) = 0;

};
/****************** End of The IQueryBrowse Interface Defining **********/

#endif  //!defined(_FORMAT_IQUERY_BROWSE_H_)
