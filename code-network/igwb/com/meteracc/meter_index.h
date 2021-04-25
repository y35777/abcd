#ifndef __METER_ACCUMULATION_INDEX__
#define __METER_ACCUMULATION_INDEX__

#include "../include/base_type.h"

#define SUBSCRIBER_NUM_LEN      10      //�û�����(BCD)����

#pragma pack(1)
//�û��ƴα��ڱ���ļ��е�λ������
struct SMeterLocation
{
    long            nLocation;                          //λ��ƫ����
    unsigned char   ySubscriber[SUBSCRIBER_NUM_LEN + 1];    //�û�����//��һλtime_id ��ֹһ���û��ı��ύʱ��
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
    FILE*   m_pFile;                            //״̬�ļ����
    char    m_szFileName[MAX_PATH];             //״̬�ļ���
    METER_LOCATION_INDEX    m_lstMeterLoc;      //�û��ƴα��ڱ���ļ��е�λ������
};

#endif //__METER_ACCUMULATION_INDEX__
