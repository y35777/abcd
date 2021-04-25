#ifndef _MML_REPORT_H_
#define _MML_REPORT_H_

#include "../include/precom.h"
#include "../include/resource.h"
//by ldf 2002.6.7 对应问题单D14393
#include "../include/toolbox.h"
//修改完毕

#define MML_HEAD				"+++"
#define MML_END					"---    END"  //<结束符标识 ---><4SP><END>
#define MML_FILL1				"  =  "       //字符串：<2SP><=><2SP>
//by ldf 2003-03-06对应问题单D20433
#define MML_NULL                "NULL"
//over
//张玉新 2002-05-25 问题单D13958
#define MML_FILL2               "     "       //<5SP> 对应于 MML_FILL1
//结束
#define STR_NEWLINE				"\r\n"	      //回车换行字符串
#define ALIGN_LEFT				0	      //左对齐方式
#define ALIGN_RIGHT				1	      //右对齐方式
#define ALIGN_MID				2	      //中间对齐方式
#define CHAR_SPACE				' ' 	      //空格
#define ALIGN_NONE				3

#define STR_NOUSE				"---"
#define STR_TITLE_SPIN          "----------------------------------------------------------"
//该类用于生成发送到MML客户端的文本格式的MML报文。该类采用Builder模式设计，
//封装了生成报文的算法。用户使用如下：
//(1)构造CMMLReport或其派生类的对象。
//(2)调用MakeReportHead构造报文头。该函数将清空以前生成的报文数据；
//(3)调用MakeReportItem、AddString等函数构造报文体；
//(4)调用MakeReportTail构造报文尾；
//(5)调用GetReport获得已生成的报文，并处理该报文。注意：该函数返回的内存
//空间使用者不得释放；
//注意事项：
//(1)步骤(2)-(5)构成一个闭环操作，用户可在初始化一个CMMLReport或其派生类
//对象后循环使用该对象;


class CMMLReport  
{
public:
    CMMLReport(const char* szName);
	virtual ~CMMLReport();

	int GetReportLen();

    void MakeCmdDisplay(const char* szCmd,
                        UINT4 uCsn, 
                        int ret            = 0,
                        const char* szhead = "ALARM");

    void Clear(const char* szName);

	void AddString(const char* pString, int nWidth, int alignment);

	void AddString(const char* pString);

	void FillSpace(int nSp);

	virtual void MakeReportItem(const char* szTitle,
                                const char* szContent,
                                int nFirstFormatLen  = 24,
		                        int nFirstAign       = ALIGN_LEFT,
                                int nSecondFormatLen = 50,
                                int nSecondAlign     = ALIGN_LEFT,
							    BYTE bNormal         = 0);

	//张玉新 2002-05-25 问题单D13958
	void MakeReportItem2(const char* szTitle,
                         const char* szContent,
                         int nFirstFormatLen  = 24,
                         int nFirstAign       = ALIGN_LEFT,
                         int nSecondFormatLen = 50,
                         int nSecondAlign     = ALIGN_LEFT);

	virtual void MakeReportHead();

	virtual void MakeReportTail();

	//张玉新 2002-07-30 问题单D15553，增加一个长度数组的参数
	void AddHoriTable(LIST<STRING>& szTitleList,
                      LIST<STRING>& szRecContentList,
                      const char* szReportTitle = "",  
			          int *pLenArray            = NULL,
                      int bLineWrap             = 1,
                      int LineLen               = 250);

	void AddVertTable(LIST<STRING>& szTitleList,
                      LIST<STRING>& szRecContentList, 
                      LIST<STRING>& szRecTitle,
                      int bLineWrap = 1,
                      int LineLen   = 80);

	const char *GetReport();

protected:
	//张玉新 2002-05-25 问题单D13958
	virtual void MultiLine(const char* sInput,
                           STRING &sOutput, 
                           int LEFT_MARGIN = 10,
                           int ROW_LENGTH  = 80,
						   BYTE bNormal    = 0);

	//张玉新 2002-07-30 问题单D15553
	int  CalcHoriTableFieldLens(LIST<STRING>& szTitleList, 
		                        LIST<STRING>& szRecContentList, 
								int *pLenArray);

	int  CalcVertTableRecordLen(LIST<STRING>& szTitleList, 
		                        LIST<STRING>& szRecContentList, 
								LIST<STRING>& szRecTitle);

protected:
	char *m_pszBuf;
	int m_nBufLen;
	int m_nHeadPosition;
	int m_nReportLen;
};

#endif
