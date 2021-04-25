#ifndef __INFOFILE_H__
#define __INFOFILE_H__

#include "../include/precom.h"
#include "../include/frame.h"

#define MAX_UNIT_NUM 150000

struct SInfoHead
{
    UINT4 uCheckSum;            //用于文件头校验的校验和
    UINT4 uRotateCount;         //状态文件回转的次数
    UINT4 uStatusFileWritePos;  //当前状态记录的写指针
    UINT4 Data[1];              //业务中的具体结构，由不同的模块各自决定
};

//为指定的文件申请指定大小的空间
BOOL AskFileSpace(FILE* f, int nFileSize);

class CInfoFile
{
public:
    //构造、析构函数
    CInfoFile();
    virtual ~CInfoFile();

    //参数设置和获取函数
    STRING GetFileFullPath();
    UINT4 GetUnitNum();
    UINT4 GetWriteIndex();
    //设置状态文件名和存放路径
    void SetInfoFilePath(const STRING& szFileName, 
                         const STRING& szFilePath);   
    //设置状态备份文件名和存放路径
    void SetBackupFilePath(const STRING& szFileName, 
                           const STRING& szFilePath);
    //设置文件头长度，记录单元长度和记录单元个数
    void SetFileStruct(const UINT4 uHeadLen, const UINT4 uUnitLen, 
                              const UINT4 uUnitNum);

    //对外接口
    //对指定的文件头计算校验和
    void CalcCheckSum(BYTE* pHead);
    //使pHead中的uStatusFileWritePos递增一个状态记录的长度
    void IncreaseIndex(BYTE* pHead);
    //根据属性中指定的路径、文件名和长度创建或打开状态文件
    BOOL OpenInfoFile(BOOL bNeedCreate = FALSE, BYTE* pDefaultHead = NULL);
    //关闭状态文件
    BOOL Close();
    //删除状态文件
    BOOL Delete();
    //将未写入到硬盘的状态数据写入到硬盘中
    BOOL Flush();
    //该函数实现将状态文件拷贝到相应的备份目录下
    BOOL CopyFileForBackup();
    //该函数实现写文件头的功能
    BOOL WriteHead(const BYTE* pData, const UINT4 uLen);
    //写第m_InfoWriteIndex个记录单元，也就是最后一个记录单元
    BOOL WriteUnit(const BYTE* pData, const UINT4 uLen);
    //函数实现读取文件头的功能
    BOOL ReadHead(BYTE* pData);
    //该函数实现读取指定记录单元的功能
    int ReadUnit(UINT4 uIndex, BYTE* pData);
    //该函数实现读取最后一个记录单元的功能
    int ReadLastUnit(BYTE* pData);
    //校验文件头是否有效
    BOOL CheckHead();

protected:
    //提供状态文件写入的接口
    BOOL WriteInfo(const UINT4 uOffset, const BYTE* pData, 
                   const UINT4 uLen);    
    //提供读取状态文件数据的接口
    BOOL ReadInfo(const UINT4 uOffset, const BYTE* pData, 
                  const UINT4 uLen);

protected:
    //成员变量
    FILE*  m_pInfoFile;           //对应文件对象，管理相关文件读写
    STRING m_szInfoFileName;      //状态文件名
    STRING m_szInfoFilePath;      //文件存放路径
    STRING m_szBackupName;        //备份文件名
    STRING m_szBackupPath;        //备份文件存放路径
    UINT4  m_uHeadLen;            //文件头长度，以字节为单位
    UINT4  m_uUnitLen;            //状态记录单元长度，以字节为单位
    UINT4  m_uUnitNum;            //状态记录个数，默认取值为MAX_UNIT_NUM
    UINT4  m_uFileLen;            //文件总长度
    UINT4  m_uInfoWriteIndex;     //表示当前写到的状态记录索引，
                                  //取值为1到m_uUnitNum
    int    m_nRotateCount;        //状态文件的回绕次数
};

#endif
