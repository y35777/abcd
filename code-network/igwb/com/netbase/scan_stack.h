#ifndef __SCAN_STACK_H__
#define __SCAN_STACK_H__


struct SFileListInfo
{
    time_t tFileTime;
    STRING strFilePath;
    bool operator<(const struct SFileListInfo& right) const
    {
        return (tFileTime < right.tFileTime);
    }
};

class CProtocolStack;
struct SScanStackInfo;
class CScanStack : public CProtocolStack
{
public:
    CScanStack(const char* const szPath, 
               CNetApp* const pNetApp, 
               UINT4 const uStackId);
    virtual ~CScanStack(void);

    //返回本对象中当前已接收的且可以提交给前存盘模块
    //保存的话单数据的字节数
    virtual int GetReadyDataSize(void);

    //返回本对象中当前已接收的且可以提交给前存盘模块
    //保存的话单数据的记录数
    virtual int GetReadyRecordCount(void);

    //将本协议栈可提交给前存盘模块保存的话单数据放入*pDataBuf对象中
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //对上次提供给前存盘模块保存的话单数据进行后续的处理
    virtual void OnResponse(BOOL const bSaved);

    //从主状态文件中读取本协议栈的状态信息
    virtual int LoadStatus(void);

    //向主状态文件中写入本协议栈的状态信息
    virtual int SaveStatus(void);

    //秒事件处理函数
    virtual void Timer1Sec(void);

    //获取故障状态
    BOOL GetFailureStatus();

protected:

    //向对应的主机模块或GSN发送协议帧
    virtual int SendFrame2Ex(void* pFrame);

    //搜索szPath目录中所有文件
    int PrepareFiles(const char* const szPath);

    LIST<SFileListInfo> m_FileList;

    //扫描间隔，单位：秒
    int m_nScanInv;

    //扫描的秒计数器
    int m_nScanCounter;

    //是否是故障状态
    int m_bFailure;

    //是否已提交文件
    BOOL m_bSubmitFile;

    //提交数据缓冲区指针
    char* m_pSubmitBuffer;
    int   m_nSubmitBufLen;

    //提交数据缓冲区的有效长度
    int m_nSubmitLen;

    //协议栈信息
    SScanStackInfo m_ScanStackInfo;

    //最小可分割单元长度
    int m_nBillRecordSize;

    //低潮包大小
    int m_uLowAtSize;
};

#endif //__SCAN_STACK_H__
