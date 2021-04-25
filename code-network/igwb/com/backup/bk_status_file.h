#ifndef _BK_STATUS_FILE
#define _BK_STATUS_FILE

#include "../include/base_type.h"   // Added by ClassView
#include "../include/frame.h"
#include "../include/resource.h"
#include "../include/toolbox.h" 
#include "resource.h"

//时间与tFileTime相同的最大文件数
#define MAX_FILE_COUNT  20

typedef struct _StatusNode
{
    char szPath[20];                            //通道名称或接入点名称
    time_t tFileTime;                           //最后一个文件修改时间
    int  nFileCount;                            //时间与tFileTime相同的文件个数
    char szFileName[MAX_FILE_COUNT][MAX_PATH];  //话单文件名称(无路径信息)
} SStatusNode, *PStatusNode;


class CBKStatusFile
{
public:
    char m_szPathFile[MAX_PATH];    //当前打开的文件名称和路径
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
    FILE *m_pFile;   //状态文件FILE指针
};

#endif  //#ifndef _BK_STATUS_FILE


