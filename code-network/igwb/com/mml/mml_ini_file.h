// mml_ini_file.h: interface for the CMmlIniFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MML_INI_FILE_H
#define MML_INI_FILE_H
#include "../include/toolbox.h"
class CMmlIniFile : public CINIFile  
{
public:
    //��������и���ͬ�������ͻὫ���и����ͬ����������
    using CINIFile::Open;
    int WriteInt(const char *szSectionName,
                 const char *szKeyName,
                 UINT4 uKeyValue);
    int WriteString(const char *szSectionName,
                    const char *szKeyName,
                    const char *szKeyValue);        
	int TruncateFile();
	int ReplaceSecWithStr(const char *szSection, const char *szSubstitution);
	int DeleteKey(const char *szSection,const char *szKey);
	int DelSection(const char *szSection);
	int Open(const char *pszFileName);
	CMmlIniFile();
    CMmlIniFile(const char *szFileName);
	virtual ~CMmlIniFile();
protected:
	int ReadFileToBuffer();
    BYTE *m_pFileBuffer;
};

#endif //MML_INI_FILE_H
