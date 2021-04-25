#if !defined(_FORMAT_IGWB_H_)
#define _FORMAT_IGWB_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CIGWB
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CFormatLib;
class IFormatCfgImp;
class IBillProcessorImp;
class IQueryBrowseImp;
class CAccessPoint;

/****************** The CIGWB class Defining ************************/
// ������������ʽת����
//
/********************************************************************/
class IGWB_Svc_Export CIGWB
{
public:

    //���캯��
    CIGWB(const STRING& sCfgRoot);

    //��������
    ~CIGWB();

    //��ʼ������
    int Init(const UINT4 nAPID = 0, const int bCfg = -1);

    //�����Ӧ�Ľӿ�ָ��
    void* QueryInterface(const IID iid);

    //��Ԫ��
    friend class IBillProcessorImp;
    friend class IQueryBrowseImp;
    friend class IFormatCfgImp;

protected:

    IFormatCfgImp*        m_pIFormatCfgImp;      //��ʽ���ýӿ�ָ��
    IBillProcessorImp*    m_pIBillProcessorImp;  //��������ӿ�ָ��
    IQueryBrowseImp*      m_pIQueryBrowseImp;    //�����ѯ�ӿ�ָ��
    CFormatLib*           m_pFmtLib;             //��ʽ�����ָ��
    LIST<CAccessPoint*>*  m_pAPs;                //���������б�
    STRING                m_sFmtRootDir;         //��ʽ���ŵĸ�·��

};
/****************** End of The CIGWB class Defining *****************/

#endif  //!defined(_FORMAT_IGWB_H_)
