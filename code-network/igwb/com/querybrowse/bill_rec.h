#ifndef __BILLRECORD_H__
#define __BILLRECORD_H__

#include "../include/precom.h"
#include "../include/format_out.h"

class CBillRecord
{
public:
    CBillRecord();   //缺省构造函数
    virtual ~CBillRecord();  //析构函数

    //设置话单记录数据内容函数
    BOOL SetContent(BYTE* pData, UINT4 uLen, CFormat* pFormat, ACE_Mutex* pMutex = NULL);
    int GetContent(LIST<STRING>& ContentList);  //获取话单记录内容

//颜文远2002.3.30添加，问题单号D12265
protected:
    void SetErrorContent(CFormat* pFormat);     //在话单内容中设置错误信息
//颜文远2002.3.30添加结束

    BOOL IsValidStr(const char* szStr, int nStrLen);

protected:
    LIST<STRING> m_ContentList; //话单中的域的内容的列表（文本内容）
};

#endif
