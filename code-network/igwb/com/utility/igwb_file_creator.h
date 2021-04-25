// igwb_file_creator.h: interface for the CIGWBFileCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__IGWB_FILE_CREATOR_H__)
#define __IGWB_FILE_CREATOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "igwb_file.h"

class CIGWBFileCreator
{
public:
	//构造函数
	CIGWBFileCreator();

	//析构函数
	~CIGWBFileCreator();

    static CIGWBFile* CreateIGWBFile(UINT4 uBillType, 
                                     UINT4 uAPType,
                                     UINT4 uBillLen,
                                     int nAPID);
};

#endif // !defined(__IGWB_FILE_CREATOR_H__)
