// ewsd_file.cpp: implementation of the CEwsdFile class.
//
//////////////////////////////////////////////////////////////////////

#include "ewsd_file.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEwsdFile::CEwsdFile(UINT4 uBillType, UINT4 uBillLen, int nAPID)
                    :CIGWBFile(uBillType,uBillLen,nAPID)
{
	//
}

CEwsdFile::~CEwsdFile()
{

}
/**********************************************************************
功能描述    读取一张EWSD的原始话单记录数据
输入参数    无
输出参数    pData----话单记录数据的指针，使用固定内存
            uDataLen----话单记录的长度
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pData的值无效
返回值      成功与否
抛出异常    无
修改记录
maimaoshi,2003-06-03
**********************************************************************/
BOOL CEwsdFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    
    bRemain = FALSE;

    return TRUE;
}