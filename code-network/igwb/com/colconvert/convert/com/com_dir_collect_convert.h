   #ifndef __COM_DIR_COLLECT_CONVERT_H__
#define __COM_DIR_COLLECT_CONVERT_H__

/*
CComDirCollectConvert定义了Com采集操作相关的基本属性和基本方法
*/
#include "../collect_convert_subobj.h"

//以下是采集操作的类CComDirCollectConvert的定义
class  CComDirCollectConvert : public CCollectConvertSubObj
{

public:
    CComDirCollectConvert(int nSwitchIndex, 
                          int nDirIndex,
                          IBillProcessor* pIBillProcessor = NULL); //构造函数传入协议对象指针
    virtual ~CComDirCollectConvert();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;

};

//以下是采集操作的类CTellinDirConvert的定义
class  CTellinDirConvert : public CComDirCollectConvert
{

public:
    CTellinDirConvert(int nSwitchIndex, 
                      int nDirIndex,
                      IBillProcessor* pIBillProcessor = NULL); //构造函数传入协议对象指针
    virtual ~CTellinDirConvert();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;
    
    //构建目标文件的路径
    virtual void ConstuctDestFileLst(SConvertFileReq& stReq);

    //拷贝第二份文件
    virtual int  CopySecondFile();

};

class  CSJZTellinDirConvert : public CTellinDirConvert
{
	
public:
    CSJZTellinDirConvert(int nSwitchIndex, 
		int nDirIndex,
		IBillProcessor* pIBillProcessor = NULL); //构造函数传入协议对象指针
    virtual ~CSJZTellinDirConvert();
	
    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;
    
    //构建目标文件的路径
    virtual void ConstuctDestFileLst(SConvertFileReq& stReq);
	virtual int  loadstatus(int nChannel);
	virtual int  SaveStatus(int nChlNo);

    virtual void OnFileConvertSucc(int nChannel);

    STRING GetFileCsnStr(UINT4 uCsn, int nChannel);

    //格式化字符串
    static STRING FormatCfgStr(const char* szFormat, 
                               const tm&   tmTime, 
                               const UINT4 uChannelID);

    virtual STRING GetBillFileInfo(const char* szFilePath, int nChannel);
    
    virtual BOOL IsRepeatLog(const char* szLogMsg, FILE* pFile);
    //保存稽核日志
    BOOL SaveCheckLog(const char* szLogMsg, int nChannel);

public:
    struct SChlInfo 
    {
        int    nCsnLen;          //话单文件名的序列号长度
        UINT4  uFileCsnStart;    //话单文件起始序列号
        UINT4  uFileCsnEnd;      //话单文件最大序列号
        UINT4  uFileCsnStep;     //话单文件序列号递增步长
        char   szFormalPrefix[512];        //正式文件的前缀
        char   szFormalMidfix[512];        //正式文件的中缀
        char   szFormalPostfix[512];       //正式文件的后缀
        time_t tFileCreateTime;            //文件创建时间

        char   szStatusFile[MAX_PATH];     //状态文件路径
        int    uCurCsn;                    //文件序列号

        BOOL   bCheckLog;                  //是否启动稽核日志特性
        char   szCheckedLogPath[MAX_PATH]; //稽核日志路径
        char   szCheckLogPrefix[512];      //稽核日志文件的前缀
        char   szCheckLogPostfix[512];     //稽核日志文件的后缀
        int    nBillLen;                   //最终话单长度

        
        SChlInfo()
        {
            memset(this, 0, sizeof(*this));
        }
    };

protected:
	VECTOR<SChlInfo>  m_ChlInfoList;          //通道信息列表

};

//以下是采集操作的类CMeterSumDirConvert的定义
class  CMeterSumDirConvert : public CComDirCollectConvert
{

public:
    CMeterSumDirConvert(int nSwitchIndex, 
                       int nDirIndex,
                       IBillProcessor* pIBillProcessor = NULL); //构造函数传入协议对象指针
    virtual ~CMeterSumDirConvert();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;

    //构建目标文件的路径
//    virtual void ConstuctDestFileLst(SConvertFileReq& stReq);

    //把临时目录下的文件移动到正式目录下
    virtual int RenameDestFile();

    //拷贝第二份文件
    virtual int  CopySecondFile();

    //转换话单文件
    virtual int ConvertFile(SConvertFileReq& stReq);
protected:

    virtual BOOL isLeapYear(int nYear);
    virtual int GetDaysOfMonth(int nYear, int nMonth);
    virtual int GetLastDate(int& nYear, int& nMonth, int& nDay);
    virtual int GetLastMonth(int& nYear, int& nMonth, int& nDay);
    virtual void initialize(STRING& sTodayMeterFile,
                            STRING& sLastMeterFile,
                            STRING& sLastMonthMeterFile,
                            const char* pszSrcFile);

    virtual int Process_cc08_special_meter_file(const char* pszSrcFile,
                                                VECTOR<STRING>& FinalFileNameList,
                                                VECTOR<int>&    CvtResultList,
                                                const int bFileLevelVerify,
                                                const int bBillLevelVerify,
                                                void* pIBillProcessor = NULL,
                                                const int nOrigBillLen = 0);

    virtual int process_and_build_finalfile(const char* pszSrcFile, //今天
                                            const char*   pszDestFile,//昨天
                                            VECTOR<STRING>& FinalFileNameList,
                                            VECTOR<int>&    CvtResultList,
                                            void* pIBillProcessor = NULL,
                                            const int nOrigBillLen = 0
                                            );
                                            
	virtual int Proce_Other_File(STRING& sSrcFile,
     				 			 VECTOR<STRING>& FinalFileNameList, 
    				 			 VECTOR<int>& CvtResultList,
    				 			 void* pIBillProcessor = NULL,
    				 			 const int	nOrigBillLen = 0);
    				 					                                              
    int m_BillDate,m_IsMonthBill,m_IsDayBill;
    unsigned long  m_nMeterUpLimit;
    struct SMetSumKey
    {
        BYTE byChgObj;
        char szCallNo[10];
        UINT2 uTrunkGrp;
        bool operator<(const struct SMetSumKey& right) const
        {
            char szLeft[13];
            memcpy(szLeft, &byChgObj, sizeof(byChgObj));
            memcpy(szLeft+1, szCallNo, sizeof(szCallNo));
            memcpy(szLeft+11, &uTrunkGrp, sizeof(uTrunkGrp));

            char szRight[13];
            memcpy(szRight, &right.byChgObj, sizeof(byChgObj));
            memcpy(szRight+1, right.szCallNo, sizeof(szCallNo));
            memcpy(szRight+11, &right.uTrunkGrp, sizeof(uTrunkGrp));
            return memcmp(szLeft,szRight,sizeof(szLeft)) < 0;
        }
    };
    
};


#endif //__COM_DIR_COLLECT_CONVERT_H__

