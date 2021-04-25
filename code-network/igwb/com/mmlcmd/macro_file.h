/******************************************************************************************************
 *��Ʒ��	��iGateway Bill V200		
 *ģ����	�����ù���
 *�ļ���	��MacroFile.h	
 *������Ϣ	�����ļ�����CMacroFile�Ķ��岿�֣��ڼ̳�CINIFile��Ĺ��ܵĻ����ϣ���װ�˷���MML����MACRO.INI
 *            �ļ��Ĺ��ܡ�CMacroFile��һ���ص������ô�C++ʵ�ֿ�ƽ̨��������Ϊ���������ṩ��
 *�汾˵��	��V200R001i00M00B00D1130A	
 *��Ȩ��Ϣ	����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����		������ƽ
 *����ʱ��	��2001-07-30
 *�޸ļ�¼	��
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

    //ÿһ��MML�������һ��"CONFIRM"������������ͳһΪ1000
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
