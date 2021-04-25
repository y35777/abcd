#ifndef __I_WLAN_AS_OCI_H__
#define __I_WLAN_AS_OCI_H__



class IWlanAsOci
{
public:
    IWlanAsOci(void){};
    virtual ~IWlanAsOci(void){};

    //初始化
    virtual int Init(int nRecordSize,
                     int nLowAtSize,
                     int nMaxPackageRecord) = 0;

    //返回本对象中当前已接收的且可以提交给前存盘模块
    //保存的话单数据的字节数
    virtual int GetReadyDataSize(void) = 0;

    //返回本对象中当前已接收的且可以提交给前存盘模块
    //保存的话单数据的记录数
    virtual int GetReadyRecordCount(void) = 0;

    //将本协议栈可提交给前存盘模块保存的话单数据放入pDataBuf中
    virtual int GetReadyData(char*& pDataBuf, int& nLen) = 0;

    //对上次提供给前存盘模块保存的话单数据进行后续的处理
    virtual void OnResponse(BOOL const bSaved) = 0;

    //秒事件处理函数
    virtual void Timer1Sec(void) = 0;
    
};

#endif //__I_WLAN_AS_OCI_H__
