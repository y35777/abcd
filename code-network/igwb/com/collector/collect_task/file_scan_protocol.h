 #ifndef __FILE_SCAN_PROTOCOL_H__
#define __FILE_SCAN_PROTOCOL_H__

#include "file_protocol.h"

//�������ļ��������Ķ���
class  CFileScanProtocol : public CFileProtocol
{

public:

    CFileScanProtocol();
    virtual  ~CFileScanProtocol();

    //ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
    virtual  int  GetFile(const SGetFileReq &pReq);

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    virtual  int  GetFileList( const  char* szSrcDir, SListOut &pOut );

    //ɾ���ļ�����˵�ָ���ļ�
    virtual  int  DelFile( const char* szFileName );

    //������ָ���ļ�
    virtual  int  RenameFile( const char *szOld, const char *szNew );

protected:
};

#endif //__FILE_SCAN_PROTOCOL_H__
