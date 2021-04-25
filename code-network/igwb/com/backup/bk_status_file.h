#ifndef _BK_STATUS_FILE
#define _BK_STATUS_FILE

#include "../include/base_type.h"   // Added by ClassView
#include "../include/frame.h"
#include "../include/resource.h"
#include "../include/toolbox.h" 
#include "resource.h"

//ʱ����tFileTime��ͬ������ļ���
#define MAX_FILE_COUNT  20

typedef struct _StatusNode
{
    char szPath[20];                            //ͨ�����ƻ���������
    time_t tFileTime;                           //���һ���ļ��޸�ʱ��
    int  nFileCount;                            //ʱ����tFileTime��ͬ���ļ�����
    char szFileName[MAX_FILE_COUNT][MAX_PATH];  //�����ļ�����(��·����Ϣ)
} SStatusNode, *PStatusNode;


class CBKStatusFile
{
public:
    char m_szPathFile[MAX_PATH];    //��ǰ�򿪵��ļ����ƺ�·��
    int GetStatInfo (const char* szChlName, SStatusNode *pStatInfo);
    BOOL Write(void* pData, UINT4 uLen);
    BOOL Read(void* pData, UINT4 uLen);
    long GetCurOffset();
    BOOL SetOffset(int nOffset, int POS = SEEK_SET);
    void Close();
    BOOL Open(const char* szPathFile);
    CBKStatusFile();
    ~CBKStatusFile();

protected:
    FILE *m_pFile;   //״̬�ļ�FILEָ��
};

#endif  //#ifndef _BK_STATUS_FILE


