/**************************************************************************
 *��Ʒ��    : iGateway Bill V200        
 *ģ����    : ����ģ��(Perf)
 *�ļ���    : os_perf_db.h
 *������Ϣ  : ���ļ�����COSPerfDB�Ķ��壻
              ������ṩ�˻�ȡ����ϵͳ״̬���麯���ӿ�
 *�汾˵��  : V200R001i00M00B00D1130A   
 *��Ȩ��Ϣ  : ��Ȩ����(c) 2001-2002 ��Ϊ�������޹�˾
 *����      : ������
 *����ʱ��  : 2001-11-05
 *�޸ļ�¼  : 
 **************************************************************************/

#ifndef __OS_PERF_DB__
#define __OS_PERF_DB__

#include "../include/base_type.h"

//by ldf 2002.6.7 ��Ӧ���ⵥD14393
#include "../include/toolbox.h"
//�޸����

// �߼�������Ϣ
typedef struct _LogicalDisk{
    char szName[MAX_PATH];  // �߼�������
    UINT4 uTot;         // �߼������ܿռ�
    UINT4 uAva;         // �߼��������ÿռ�
} SLogicalDisk, *PLogicalDisk;

// �����������ݽṹ��������Ҫ��õĲ���ϵͳ��������
typedef struct _OSPerfStat{
    BYTE nCPUs;         // CPU����
    BYTE nLogDisk;      // �߼���������
    BYTE* CPUPercent;   // ÿ��CPU��ռ����
    UINT4 uPhyMemTot;   // �����ڴ��ܿռ�
    UINT4 uPhyMemAva;   // �����ڴ���ÿռ�
    UINT4 uVirMemTot;   // �����ڴ��ܿռ�
    UINT4 uVirMemAva;   // �����ڴ���ÿռ�
    PLogicalDisk Disks;  // �����߼������ܿռ�����ÿռ�

	~_OSPerfStat()
	{
		delete CPUPercent;
		delete Disks;
	}

} SOSPerfStat, *POSPerfStat;

class COSPerfDB {
public:
    COSPerfDB()
	{
		m_szFrontSavePath[0] = 0;
		m_szBackSavePath[0] = 0;
	}

    COSPerfDB(const char *szFront,const char *szBack)
    {
        strncpy(m_szFrontSavePath,szFront,MAX_PATH);
		m_szFrontSavePath[MAX_PATH - 1] = 0;

        strncpy(m_szBackSavePath,szBack,MAX_PATH);
		m_szBackSavePath[MAX_PATH - 1] = 0;
    }

    virtual ~COSPerfDB(){};

    // ��ò���ϵͳ������������������������Ĳ���ϵͳ������������ַ�����ʽ
    // Ϊ��
    // "����������1��++����������1ֵ++......++����������N��++����������Nֵ"��
    // ����+�ű�ʾһ���ո��ַ�
    virtual void GetOSStatus(BYTE *pbPara,int nNum,char*& szAllPerf) = 0;

    // ��������������ݣ�������
    //      ÿ��CPU��ռ����
    //      �����ڴ�Ŀ��ÿռ�
    //      �����ڴ�Ŀ��ÿռ�
    //      ���߼������Ŀ��ÿռ�
    virtual void GetPerfStatistic(SOSPerfStat& OSPerfStat) = 0;

protected:
    char m_szFrontSavePath[MAX_PATH];
    char m_szBackSavePath[MAX_PATH];
};

#endif /* __OS_PERF_DB__ */


