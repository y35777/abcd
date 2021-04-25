#ifndef __METER_ACCUMULATION_INDEX__
#define __METER_ACCUMULATION_INDEX__

#include "../include/base_type.h"

#define SUBSCRIBER_NUM_LEN      10      //用户号码(BCD)长度

#pragma pack(1)
//用户计次表在表底文件中的位置索引
struct SMeterLocation
{
    long            nLocation;                          //位置偏移量
    unsigned char   ySubscriber[SUBSCRIBER_NUM_LEN + 1];    //用户号码//加一位time_id 防止一个用户改变提交时间
};
#pragma pack()

typedef LIST<SMeterLocation> METER_LOCATION_INDEX;

class CMeterAccIndex
{
public:
    CMeterAccIndex();
    ~CMeterAccIndex();

public:
    int Init(const char* pszFileName);
    int GetIndex(const unsigned char* pSubscriber, const int nLength,
                 long& nLocation);
    int AddIndex(const unsigned char* pSubscriber, const int nLength,
                 const long nLocation);

    void Close(void);

private:
    FILE*   m_pFile;                            //状态文件句柄
    char    m_szFileName[MAX_PATH];             //状态文件名
    METER_LOCATION_INDEX    m_lstMeterLoc;      //用户计次表在表底文件中的位置索引
};

#endif //__METER_ACCUMULATION_INDEX__
