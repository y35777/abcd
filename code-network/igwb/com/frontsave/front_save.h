#ifndef __FRONTSAVE_H__
#define __FRONTSAVE_H__

#include "save.h"

//前存盘状态文件的文件头定义
struct FS_STATUS_FILE_HEAD
{
    UINT4 uCheckSum;            //校验和
    UINT4 uRotateCount;         //状态文件回转的次数
    UINT4 uStatusFileWritePos;  //状态文件自身的写指针
    UINT4 uLastPID;             //前存盘模块当前需要保存的原始话单包的包号
    UINT4 uFileCsn;             //包号为uLastPid的原始话单包所在的原始话
                                //单文件的文件序列号
    UINT4 uFileWritePos;        //包号为uLastPid的原始话单包所在的
                                //原始话单文件的写指针
    UINT4 uaReserved[2];        //保留字节
};

//前存盘状态记录的定义
struct FS_STATUS_FILE_ELEMENT
{
    UINT4 uPID;         //前存盘模块所保存的原始话单的包号
    UINT4 uFileCsn;     //包号为uPID的原始话单包所在的原始话单文件的文件序列号
    UINT4 uStartPos;    //包号为uPID的原始话单包在文件中的起始位置
    UINT4 uPacketLen;   //包号为uPID的原始话单包的长度
    UINT2 nYear;        //包号为uPID的原始话单包所在的原始话单文件的创建年份
    BYTE  yMonth;       //包号为uPID的原始话单包所在的原始话单文件的创建月份
    BYTE  yDay;         //包号为uPID的原始话单包所在的原始话单文件的创建日期
    UINT4 uReserved;    //保留字段
};

class CFrontSave : public CSaveTransaction
{
public:
    //构造、析构函数
    CFrontSave();
    virtual ~CFrontSave();

    //新增加的接口
    BOOL Init(UINT4 uAPID); //处理初始化时的工作
    //读取指定的话单包范围中的第一包能够读到的话单
    int GetFirstBillPacket(const UINT4 uFirstPID, const UINT4 uLastPID, 
                           UINT4& uPID, BYTE*& pPacket, UINT4&  uPacketLen, 
                           STRING& szFileFullPath, UINT4& uStartPos);
    //读取指定的话单包范围中的下一包能够读到的话单
    int GetNextBillPacket(const UINT4 uLastPID,
                          UINT4& uPID, 
                          BYTE*& pPacket, 
                          UINT4& uPacketLen, 
                          STRING& szFileFullPath, 
                          UINT4& uStartPos);
    //根据状态文件得到最后一个话单包的包号
    UINT4 GetLastPID();
    //得到当前话单包的相关参数，这些参数用于向缓冲区传送话单包的相关信息
    void GetCurPacketParam(STRING& szFileFullPath, UINT4& uStartPos);
protected:
    //从基类继承的函数
    //得到下一个话单文件存放目录，如果目录不存在，则创建之
    void GetNextFilePath();
    //删除最后一个临时文件，在进行状态文件校验时调用
    BOOL DeleteTempFile();
    //根据收到的话单包，更新状态文件的内容以及各成员变量
    void RefreshStatus(BYTE* pPacketInfo);
    //创建新的话单文件以及相关的状态文件刷新操作
    BOOL CreateNewFile();
    //关闭话单文件的后续处理
    BOOL DoWhenFileClose();
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
	//根据收到的包信息判断是否为空包
	BOOL IsEmptyPacket(BYTE* pInfo);

    //新增加的函数
    //根据指定的状态记录获得话单文件的全路径
    STRING GetFileFullPath(FS_STATUS_FILE_ELEMENT* pInfoUnit);
    //根据指定的状态记录获得话单文件的路径和文件名
    void GetFilePathAndName(FS_STATUS_FILE_ELEMENT* pInfoUnit, 
                            STRING& szFilePath, STRING& szFileName);
    //根据话单包号得到状态文件中相应的状态记录，如果找不到相关的记录，则得
    //到符合以下条件的包号对应的状态记录----在状态文件中存在，且为所有大
    //于uPID的包号中最小的包号
    int GetInfoUnit(const UINT4 uPID, 
                    FS_STATUS_FILE_ELEMENT* pInfoUnit, INT4& nIndex);
    //从指定的文件中读取一包话单数据
    BOOL ReadPacket(const char* szFileFullPath, const BYTE* pPacket, 
                    const UINT4 uLen, const UINT4 uStartPos);

	virtual void DelMoreFiles();

protected:
    UINT4  m_uAPID;            //所在的接入点的ID号
    STRING m_szDatePath;       //日期目录存放的路径

    //以下的变量用于进行读取某个包号范围内的话单包的操作
    //包号范围中的最小包号
    UINT4  m_uFirstPID;        
    //包号范围中的最大包号
    UINT4  m_uLastPID;
    //读取指定范围的话单包的操作是否已经结束
    BOOL   m_bReadComplete;    
    //当前正在读取的状态记录的索引号
    INT4   m_nCurIndex;        
};

#endif
