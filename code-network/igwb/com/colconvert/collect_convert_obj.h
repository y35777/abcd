#ifndef __COLLECT_CONVERT_OBJ_H_
#define __COLLECT_CONVERT_OBJ_H_

#include "include/i_collect_convert.h"

class CCollectConvertObj 
{
public:
    CCollectConvertObj(int nConvertID, const char* const szCfgPath);
    virtual ~CCollectConvertObj();

    //初始化
    virtual int Init();

    //转换话单文件
    virtual int Process(const char* szBillFile, 
                        const int& nConvertInfo);


    //取任务号
    inline int  GetConvertID(){return m_nConvertID;};

protected:

   //本采集任务对应的配置文件全路径名
    char          m_szCfgPath[MAX_PATH];

    //采集任务对象接口指针
    ICollectConvert* m_pCollectConvert;

    //转换ID号，即交换机在配置文件中的编号
    int          m_nConvertID;

};

#endif //__COLLECT_CONVERT_OBJ_H_
