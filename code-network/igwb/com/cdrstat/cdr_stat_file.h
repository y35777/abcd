#ifndef __CDR_STAT_FILE_H__
#define __CDR_STAT_FILE_H__

#include "../include/toolbox.h"

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

class CIGWBFile;
class CCDRStatFile
{
public:
	CCDRStatFile();
	virtual ~CCDRStatFile();

public:

	//设置参数，进行初始化操作
    int Init(UINT4 uAPType,
             UINT4 uBillType,
             BOOL  bMustMerge, 
             BOOL  bFinalSPFileFmt, 
		     UINT4 uBillLen,
             int nAPID);

    //打开指定的话单文件
    BOOL Open(const char* const szPathFile);    

	//关闭文件
	BOOL Close();

	//获取下一张话单记录数据
    int  GetNextBill(BYTE*& pCDRRecord, UINT4& nCDRLen, BOOL&  bRemain);
	
protected:
	
	//话单文件类型的枚举定义
	enum BillType
	{
		ORIG_BILL = 0,      //原始话单
		FINAL_BILL = 1      //最终话单
	};

    //IGWB文件指针
    CIGWBFile*   m_pIGWBFile;
};
#endif //__CDR_STAT_FILE_H__

