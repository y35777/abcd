#ifndef __QBREPORT_H__
#define __QBREPORT_H__

#include "../include/toolbox.h"

#define MAX_REPORT_SIZE (56 * 1024)    

#include "../include/resource.h"
#include "mml_report.h"

#define S_QB_MML_IGWB              S_IGATEWAY_BILL
#define S_QB_MML_TABLE_TITLE       "%s                PAGE %d"
#define S_QB_MML_TO_BE_CONTINUE    "TO BE CONTINUED\r\n"

class CQBMMLReport : public CMMLReport
{
public:
    CQBMMLReport();
    virtual ~CQBMMLReport();

    void MakeQBReportHead(const char* szCmd,
                          int nRetCode = 0,
                          int nSequ = 0);

    //�ڱ����м���һ������������ݣ����Ƿ�ҳ���͵�����
    void AddQBHoriTable(LIST<STRING>& TitleList,
                        LIST<STRING>& ContentList, 
                        const char* szHeadName,
                        int& nPageNo);

    //�ڱ����м���һ������������ݣ����Ƿ�ҳ���͵�����
    void AddQBVertTable(LIST<STRING>& TitleList,
                        LIST<STRING>& ContentList, 
                        LIST<STRING>& RecHeadList);

protected:
    //������Ĵ�С���ƣ����ھ����Ƿ���з�ҳ���ͣ�
    //��ʵ�ʵ�����£�������ܻ���Ϊ��������ߴ�
    int m_nSizeLimit;
	//������ 2002-07-30 ���ⵥD15553
	int *m_pLenArray;    //����ÿһ�еĳ���
	int m_nLineLen;      //����һ�еĳ���
	int m_nRecordLen;    //�洢һ����¼�ĳ���
    //����
};

#endif //__QBREPORT_H__
