#ifndef __SAVETRANSACTION_H__
#define __SAVETRANSACTION_H__

#include "../include/precom.h"
#include "info_file.h"
#include <algorithm>
#include <deque>

#define CHAR_DIR            '/' 

typedef struct _FilesListNode
{
    time_t tFileTime;               //文件修改时间
    char szFileName[MAX_PATH];      //话单文件名称(无路径信息)
} SFilesListNode, *PFilesListNode;
typedef std::deque<SFilesListNode *> LISTFILELISTNODE;

class CSaveTransaction
{
public:
/* Begin: WLAN移植需要严格控制文件输出，默认为0，表示不需按时间输出	Adder: Gan Zhiliang   Date: 2003-4-23*/
	UINT4 m_uResFileProductTime;

/* End  : Adder: Gan Zhiliang	   Date: 2003-4-23*/
    //构造、析构函数
    CSaveTransaction();
    virtual ~CSaveTransaction();

    //参数设置和获取函数
    UINT4 GetCurCsn(BOOL bDisplay = FALSE);

    //对外接口
    //秒事件处理函数
	virtual BOOL Timer1Sec();	
    //实现话单包保存的处理逻辑
	int WritePacket(BYTE* pPacketInfo, UINT4& uWriteTime);	
    //关闭当前正在操作的话单文件
    virtual BOOL CloseCurFile();

	//added by maimaoshi at 2002-09-23
	virtual void DelMoreFiles() { }         //删除多余文件
	//end

	void GetCurrentFileName(STRING& sFileName);

protected:
    //处理初始化时的工作
    BOOL Init();
    //计算下一个新话单文件的文件序列号
    void CalcNextCsn();
    //得到下一个话单文件的文件名
    void GetNextFileName();
    //得到下一个话单文件存放目录，如果目录不存在，则创建之
    virtual void GetNextFilePath() = 0;
    //删除最后一个临时文件，在进行状态文件校验时调用
    virtual BOOL DeleteTempFile() = 0;
    //根据收到的话单包，更新状态文件的内容以及各成员变量
    virtual void RefreshStatus(BYTE* pPacketInfo) = 0;
    //创建新的话单文件以及相关的状态文件刷新操作,
    //在基类中实现创建新文件的操作，在子类中负责实现状态文件的刷新
    virtual BOOL CreateNewFile();
    //关闭话单文件的后续处理
    virtual BOOL DoWhenFileClose() = 0;
    //在保存话单包事务中的写状态文件操作
    virtual BOOL WriteStatusFile(CInfoFile* pInfoFile, BOOL bFlush) = 0;
    //将一包话单写入到当前的话单文件中
    BOOL WritePacketData(BYTE* pPacket, UINT4 uPacketLen);
    //在保存话单包事务中的写话单文件操作
    virtual int WriteBillFile(BYTE* pPacketInfo, BOOL bFlush) = 0;

	//颜文远2002.05.18修改，问题单号D13767
    //处理启动校验时主备状态文件不一致的情况
    virtual BOOL DoWithMainDiffBak(BYTE* pMainHead, BYTE* pBakHead, 
								   BYTE* pMainLastUnit, 
								   BYTE* pBakLastUnit) = 0;
	//颜文远2002.05.18修改结束

    //根据状态文件里的信息对相关的成员变量进行初始化
    virtual BOOL InitByStatusFile() = 0;
    //对主备状态文件进行校验工作，相应调用其他处理函
    //数根据状态处理话单文件。在模块启动时调用
    BOOL StartCheck();
    //根据序列号得到文件序列号的字符串
    STRING GetFileCsnStr(UINT4 uCsn);
	//根据收到的包信息判断是否为空包
	virtual BOOL IsEmptyPacket(BYTE* pInfo) = 0;

	//added by maimaoshi at 2002-09-23
	//删除指定目录下过期话单文件处理函数
    int DelOneDirBillFiles(char* szPath);
	//创建路径
	STRING MakePath(const STRING &path, const char *filename);
	//准备文件列表
    virtual int PrepareFiles(const char* szPath, time_t tFileTime);
	//end 

protected:
    //成员变量
    CInfoFile m_InfoMain;         //主状态文件
    CInfoFile m_InfoBak;          //备状态文件
    BYTE*     m_pHead;            //当前的状态文件头，
                                  //由初始化函数和析构函数负责内存的申请和释放
    UINT4     m_uHeadLen;         //状态文件头的长度
    BYTE*     m_pLastInfoUnit;    //状态文件中最后一个状态记录，
                                  //由初始化函数和析构函数负责内存的申请和释放
    UINT4     m_uUnitLen;         //状态记录的长度
    FILE*     m_pBillFile;        //当前正在处理的话单文件对象指针
    STRING    m_szFilePath;       //当前的文件的存放路径
    STRING    m_szFileName;       //当前的文件名
    UINT4     m_uFileLen;         //话单文件的最大长度，单位为字节
    char      m_szPrefix[512];    //话单文件名的前缀
    int       m_nCsnLen;          //话单文件名的序列号长度
    UINT4     m_uFileCsnStart;    //话单文件起始序列号
    UINT4     m_uFileCsnEnd;      //话单文件最大序列号
    UINT4     m_uFileCsnStep;     //话单文件序列号递增步长
    char      m_szPostfix[512];   //话单文件名的后缀
    UINT4     m_uCurCsn;          //当前的话单文件序列号
    tm        m_tmToday;          //当前的日期，只有日期信息有用，
                                  //用于进行判断是否跨天
    tm        m_tmCreateTime;     //当前话单文件创建时间
    tm        m_tmLastFileTime;   //上一个文件的创建时间
    //进行Flush操作的最大包号间隔，当保存了m_uMaxFlushSpan个话单包后，则必
    //须进行一次状态文件的写操作以及状态文件和话单文件的Flush操作
    UINT4 m_uMaxFlushSpan;
    UINT4 m_uLastFlushPID;        //上次进行Flush操作时的话单包号
    UINT4 m_uPID;                 //当前处理的包号
	char  m_szModuName[200];      //用于写TRACE信息等时的模块名

	//added by maimaoshi at 2002-09-23 
	char m_szModuNameEx[200];      //用于写删除过时文件Log信息时的模块名
	char m_szSrcBasePath[MAX_PATH]; //备份的原始根路径
	UINT4 m_uBillSavedDays;         //话单文件保留天数
	LISTFILELISTNODE m_FileList;    //需要备份的话单文件列表

	STRING m_sCurFileFullPath;		//当前读写的文件名全路径
	
	BOOL   m_bAlwaysGenFile;      //是否不间断地生成文件

	class Compare
    {
    public:
        friend class CSaveTransaction;
        bool operator ()(SFilesListNode* x, SFilesListNode *y); 
    };
	//end
};

#endif
