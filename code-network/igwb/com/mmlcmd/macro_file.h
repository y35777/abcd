/******************************************************************************************************
 *产品名	：iGateway Bill V200		
 *模块名	：公用功能
 *文件名	：MacroFile.h	
 *描述信息	：本文件是类CMacroFile的定义部分，在继承CINIFile类的功能的基础上，封装了访问MML访问MACRO.INI
 *            文件的功能。CMacroFile的一个特点是利用纯C++实现跨平台，可以作为技术货架提供。
 *版本说明	：V200R001i00M00B00D1130A	
 *版权信息	：版权所有（C）2001-2002 华为技术有限公司
 *作者		：邱意平
 *创建时间	：2001-07-30
 *修改记录	：
 ******************************************************************************************************/
#ifndef  __MACRO_FILE__
#define  __MACRO_FILE__

#include "../include/toolbox.h"
#include "../include/mmlcmd_def.h"

class CINIFile;

class CMacroFile : public CINIFile
{
public:                  
	CMacroFile(char *pszFileName);
    virtual ~CMacroFile();

public:
	BOOL GetCmdDef(char *szCmdName, SCmdDef& CmdDef);				
	BYTE GetParaDataType(char *szCmdName, UINT2 nParaNo);

	UINT2 GetNextWord(char *szWord, char *szLine, UINT2 uWordSize);
	BOOL GetCommonValue(char *szInput, char *szOutput, char *pszType);
	int GetParaMaxDataLen(char *szCmdName, UINT2 nParaNO);
	BYTE IsParaMustGive(char *szCmdName, int nParaCode);
	BOOL GetParaAttribute(char* szParaLine, int& nParaCode, char *szParaName, 
                          char *szDspName, char* szDefault, int& nMoveLength); 

	BOOL GetEnumValue(char *szEnumString, char *szEnumName, UINT4& uEnumValue);
	BYTE GetRangeType(char *pszDesc);

	BOOL ConvertChildPara(char *szCmdName, int nCmdCode, int nParaSeq, int nParaCode, int nChildNum);
	BOOL ConvertParaDef(char *szCmdName, UINT2 nCmdCode, UINT2 nParaNum);
	BOOL WriteParaRangeTbl(char *pszBuf, int nCmdCode, int nParaCode, int nChildParaCode);
	BOOL WriteParaEnumTbl(char *pszBuf, int nCmdCode, int nParaCode, int nChildParaCode);

    //每一个MML命令包含一个"CONFIRM"参数，参数码统一为1000
    void WriteConfirmPara(const int nCmdCode, const int nParaNum);

	BOOL ConvertAllCmdToTbl(const int nMaxCmdCode);

private:
	void CloseAllTbl();

private:
	FILE    *m_fpCmd;
	FILE    *m_fpPara;
	FILE    *m_fpParaRange;
	FILE    *m_fpChildPara;
	FILE    *m_fpParaEnum;
};


#endif //__MACRO_FILE__
