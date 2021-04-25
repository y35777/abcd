#ifndef _MML_REPORT_H_
#define _MML_REPORT_H_

#include "../include/precom.h"
#include "../include/resource.h"
//by ldf 2002.6.7 ��Ӧ���ⵥD14393
#include "../include/toolbox.h"
//�޸����

#define MML_HEAD				"+++"
#define MML_END					"---    END"  //<��������ʶ ---><4SP><END>
#define MML_FILL1				"  =  "       //�ַ�����<2SP><=><2SP>
//by ldf 2003-03-06��Ӧ���ⵥD20433
#define MML_NULL                "NULL"
//over
//������ 2002-05-25 ���ⵥD13958
#define MML_FILL2               "     "       //<5SP> ��Ӧ�� MML_FILL1
//����
#define STR_NEWLINE				"\r\n"	      //�س������ַ���
#define ALIGN_LEFT				0	      //����뷽ʽ
#define ALIGN_RIGHT				1	      //�Ҷ��뷽ʽ
#define ALIGN_MID				2	      //�м���뷽ʽ
#define CHAR_SPACE				' ' 	      //�ո�
#define ALIGN_NONE				3

#define STR_NOUSE				"---"
#define STR_TITLE_SPIN          "----------------------------------------------------------"
//�����������ɷ��͵�MML�ͻ��˵��ı���ʽ��MML���ġ��������Builderģʽ��ƣ�
//��װ�����ɱ��ĵ��㷨���û�ʹ�����£�
//(1)����CMMLReport����������Ķ���
//(2)����MakeReportHead���챨��ͷ���ú����������ǰ���ɵı������ݣ�
//(3)����MakeReportItem��AddString�Ⱥ������챨���壻
//(4)����MakeReportTail���챨��β��
//(5)����GetReport��������ɵı��ģ�������ñ��ġ�ע�⣺�ú������ص��ڴ�
//�ռ�ʹ���߲����ͷţ�
//ע�����
//(1)����(2)-(5)����һ���ջ��������û����ڳ�ʼ��һ��CMMLReport����������
//�����ѭ��ʹ�øö���;


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

	//������ 2002-05-25 ���ⵥD13958
	void MakeReportItem2(const char* szTitle,
                         const char* szContent,
                         int nFirstFormatLen  = 24,
                         int nFirstAign       = ALIGN_LEFT,
                         int nSecondFormatLen = 50,
                         int nSecondAlign     = ALIGN_LEFT);

	virtual void MakeReportHead();

	virtual void MakeReportTail();

	//������ 2002-07-30 ���ⵥD15553������һ����������Ĳ���
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
	//������ 2002-05-25 ���ⵥD13958
	virtual void MultiLine(const char* sInput,
                           STRING &sOutput, 
                           int LEFT_MARGIN = 10,
                           int ROW_LENGTH  = 80,
						   BYTE bNormal    = 0);

	//������ 2002-07-30 ���ⵥD15553
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
