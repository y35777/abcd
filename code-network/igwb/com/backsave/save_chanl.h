#ifndef __SAVECHANNEL_H__
#define __SAVECHANNEL_H__

#include "../include/save_out.h"
#include "patch_encap.h"

//后存盘状态文件的文件头定义
struct  BS_STATUS_FILE_HEAD
{
    UINT4 uCheckSum;        //校验和
    UINT4 uRotateCount;     //状态文件回转的次数
    UINT4 uStatusFileWritePos;  //状态文件自身的写指针
    UINT4 uLastPID; //所属通道当前所保存的最后一包最终话单的包号
    UINT4 uFileCsn; //包号为uLastPid的最终话单包所在的最终话单文件的文件序列号
    UINT4 uFileWritePos; //包号为uLastPID的最终话单包所在的最终话单文件的写指针
    UINT4 uBillCsn;      //最后一条话单的话单处理模块中按通道处理的话单流水号
    UINT4 uaReserved[2];        //保留字节
};

//后存盘状态记录的定义
struct BS_STATUS_FILE_ELEMENT
{
    UINT4 uFileCsn;     //最终话单文件序列号
    UINT4 uFirstPID;    //该话单文件中的第一包话单的包号
    UINT4 uLastPID;     //该话单文件中的最后一包话单的包号，
						//本值目前不填
    UINT4 uBillCsn;     //前一个话单文件中最后话单记录的话单流水号
    UINT4 uReserved;    //保留字段
};

class CSaveChannel : public CSaveTransaction  
{
public:
    //构造、析构函数
    CSaveChannel(CPatchEncap* pPatchEncap);
    virtual ~CSaveChannel();

    //继承的接口
	BOOL Timer1Sec();   //秒事件处理函数	

    //新增加的接口
	//获取通道名
	const char* GetChlName();
    //初始化函数
    virtual BOOL Init(UINT4 uAPID, UINT4 uChannelID, STRING& szChannelName, int nBillLen = 0);
    //获得启动恢复时需要的状态参数
    void GetResumeParam(UINT4& uLastPID, UINT4& uLastBillCsn);
    //根据状态文件得到最后一个话单包的包号
    UINT4 GetLastPID();
    //根据状态文件得到话单序列号
    UINT4 GetBillCsn();
	//检查计费中心是否已经及时取走话单文件，每秒钟被调用一次
	void CheckFetchingFile();
	
	//by	杨善	2004-6-1	问题单SWPD05564
	BOOL CloseCurFile();
	//the end	问题单SWPD05564

protected:
    //得到下一个话单文件存放目录，如果目录不存在，则创建之
    void GetNextFilePath();
    //删除最后一个临时文件，在进行状态文件校验时调用
    BOOL DeleteTempFile();
    //根据收到的话单包，更新状态文件的内容及各成员变量
    void RefreshStatus(BYTE* pPacketInfo);
    BOOL DoWhenFileClose(); //关闭话单文件的后续处理
    //在保存话单包事务中的写状态文件操作
    BOOL WriteStatusFile(CInfoFile* pInfoFile, BOOL bFlush);
    //在保存话单包事务中的写话单文件操作
    int WriteBillFile(BYTE* pPacketInfo, BOOL bFlush); 

	//颜文远2002.05.18修改，问题单号D13767
    //处理启动校验时主备状态文件不一致的情况
    BOOL DoWithMainDiffBak(BYTE* pMainHead, BYTE* pBakHead, 
						   BYTE* pMainLastUnit, BYTE* pBakLastUnit);
	//颜文远2002.05.18修改结束

    //根据状态文件里的信息对相关的成员变量进行初始化
    BOOL InitByStatusFile();    
    //创建新的话单文件以及相关的状态文件刷新操作
    BOOL CreateNewFile();
	//根据收到的包信息判断是否为空包
	BOOL IsEmptyPacket(BYTE* pInfo);

	virtual void DelMoreFiles();

    //Added by ZhengYuqun SWPD03912 2004-03-03
    //查找第二份话单目录下的第一个文件
    STRING FindFirstFileInSecondPath();

    //格式化前缀字符串
    static STRING FormatCfgStr(const char* szFormat, 
                               const tm&   tmTime, 
                               const UINT4 uChannelID,
                               const INT4  nRecordCnt = 0);

    virtual STRING GetBillFileInfo(const char* szFilePath);
    
    virtual BOOL IsRepeatLog(const char* szLogMsg, FILE* pFile);
    //保存稽核日志
    BOOL SaveCheckLog(const char* szLogMsg);
	

protected:
    UINT4  m_uAPID;               //接入点的ID号
    UINT4  m_uChannelID;          //通道号	
    STRING m_szChannelName;       //通道名
    STRING m_szDatePath;          //通道的存盘目录
    STRING m_szFirstPath;         //第一份用于查询浏览的话单的存放路径
    STRING m_szFormalFileName;    //当前的正式文件名
    char   m_szFormalPrefix[512]; //正式文件的前缀
    char   m_szFormalMidfix[512]; //正式文件的中缀
    char   m_szFormalPostfix[512];//正式文件的后缀
    BOOL   m_bSecondCopy;         //是否使用第二存盘目录结构保存最终话单文件
    STRING m_szSecondPath;        //第二份送往计费中心的话单文件的存放路径
    STRING m_szSecondChannelPath; //第二份送往计费中心的通道路径
    
    //话单文件生成的时间限制，临时文件创建超过这个时间后，强制转换成正式文件
    UINT4  m_uFileTime;          
    UINT4  m_uFileExistTime;     //当前文件存在的时间，用于判断当前文件是否超时
	//以下变量用于管理“计费中心长期未取话单”的故障告警
	BOOL   m_bFetchFileAlarmSend;//是否发送过“计费中心长期未取话单”的故障告警
	STRING m_szCheckedFilePath;//被监视的文件，用于确定与计费中心的连接是否正常
	int    m_nCheckTimeCounter;//检查被监视文件的时间计数器
	UINT4    m_nFetchFileTimeOut;//计费中心取话单文件的超时时间，以分钟为单位
	int    m_nDelTimeCounter;  //检查文件是否过期的时间间隔

    int    m_nBillLen;               //最终话单长度

    BOOL   m_bCheckLog;              //是否启动稽核日志特性
    STRING m_szCheckedLogPath;      //稽核日志路径
    char   m_szCheckLogPrefix[512];  //稽核日志文件的前缀
    char   m_szCheckLogPostfix[512]; //稽核日志文件的后缀

    CPatchEncap* m_pPatchEncap;//后存盘适配器
};

#endif 
