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

    //在报告中加入一个横向表格的内容，考虑分页传送的问题
    void AddQBHoriTable(LIST<STRING>& TitleList,
                        LIST<STRING>& ContentList, 
                        const char* szHeadName,
                        int& nPageNo);

    //在报告中加入一个横向表格的内容，考虑分页传送的问题
    void AddQBVertTable(LIST<STRING>& TitleList,
                        LIST<STRING>& ContentList, 
                        LIST<STRING>& RecHeadList);

protected:
    //报文体的大小限制，用于决定是否进行分页传送，
    //在实际的情况下，报告可能会略为超过这个尺寸
    int m_nSizeLimit;
	//张玉新 2002-07-30 问题单D15553
	int *m_pLenArray;    //存贮每一列的长度
	int m_nLineLen;      //存贮一行的长度
	int m_nRecordLen;    //存储一个记录的长度
    //结束
};

#endif //__QBREPORT_H__
