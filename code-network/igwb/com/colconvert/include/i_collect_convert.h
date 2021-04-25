#ifndef __I_COLLECT_CONVERT_H__
#define __I_COLLECT_CONVERT_H__

class ICollectConvert
{
public:
    ICollectConvert(){};
    virtual ~ICollectConvert(){};

    //设置配置文件全路径名
    virtual void SetCfgPath(const char* const szCfgPath) = 0;

    //初始化采集任务
    virtual int Init() = 0;

    //转换话单文件
    virtual int Process(const char* szBillFile, 
                        const int& nConvertInfo) = 0;

};

#endif //__I_COLLECT_CONVERT_H__
