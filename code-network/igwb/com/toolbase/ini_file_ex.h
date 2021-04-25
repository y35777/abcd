/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:
    �ļ���:ini_file_ex.h
    ������Ϣ:
    
    ����CINIFileEx��Ķ���, ������չ��CINIFile��Ĺ��ܣ�
    ʵ���˶�INI�ļ���ɾ���Ρ�ɾ�����Լ��ļ�������ʱ����һ���ļ��Ĺ��ܡ�


    �޸ļ�¼:
    ������,  2003-08 ,   ����.
    �������� 2003-11 ��  ��֮����ΪCINIFileEx�����Ż�����.
*/

#ifndef __INI_FILE_EX_H__
#define __INI_FILE_EX_H__

#include "../include/toolbox.h"

class IGWB_Svc_Export CINIFileEx : public CINIFile  
{
public:
    CINIFileEx(const char *szFileName);
    virtual ~CINIFileEx();

public:
    int DeleteKey(const char *szSection,const char *szKey);

    int DelSection(const char *szSection);
    
    int TruncateFile();
    int ReplaceSecWithStr(const char *szSection, const char *szSubstitution);
    int LoadFile();
    
public:
    BOOL Open();
    
protected:
    BYTE *m_pFileBuffer;
};

#endif //__INI_FILE_EX_H__
