#ifndef __BILL_TASK_STRUCT_H__
#define __BILL_TASK_STRUCT_H__

//定义任务类型
enum TASK_TYPE
{
    TK_UNKNOWN = -1,        //未知任务
    TK_SCP_RVY              //SCP话单恢复任务
};

#define SCP_QUERY_DST_FMT   "X3KF_In_BSCP_D"

//SCP的话单恢复命令的关键数据
struct SSCPBillRcvrApply
{
	char szOp[17];		    //FTP用户名
	char szPwd[17];		    //FTP口令
	char szDir[33];		    //文件目录	
	char szFileName[33];	//文件名
	char szIP[17];		    //FTP的IP地址
	int  nNI;               //网标识，0国际; 1国际备用; 2国内; 3国内备用
	char szDpc[7];		    //本局对应的DPC码, 16进制的字符串, 如"AAAAAA"
};

//保存话单恢复命令的有关信息，返回报告时用
struct SRcvrCmd
{
	char* szCmdLine;	//MML命令串
	UINT2 uAppType;     //应用类型
	UINT2 uClientNo;    //某种类型的MML客户端的编号
	UINT4 uSerialNo;	//命令流水号
};

#define  MAX_NUM_LEN	22
//异常话单结构
struct SEcptBillRec
{
	int  nRecNo;		                //流水号
	int  nServKey;		                //业务键
	char szCallingNum[MAX_NUM_LEN + 2];	//主叫用户号码
	char szTransNum[MAX_NUM_LEN + 2];	//目标地址号码
	char szDateTime[15];			    //异常话单的起始时间串
	int  nDecCost;				        //异常话费修正值
	tm   tmStartTime;				    //由szDateTime转换的值
};

//网际标识及信令点号
struct SScpApplyKey
{
    int nNI;
    char szDpc[7];
    bool operator<(const struct SScpApplyKey& right) const
    {
        if(nNI == right.nNI)
        {
            return ACE_OS::strcasecmp(szDpc, right.szDpc) < 0; 
        }

        return (nNI < right.nNI);
    }
};

//接入码、业务键
struct SCodeKey
{
	char szAccCode[MAX_NUM_LEN + 1];
	int nServKey;
};

enum EBill_RVY_RETCODE
{	//此返回码定义对应资源中的定义, 不可随便改，增加时从最后加
	BR_SUCCESS   = 0,	    //成功，处理完毕。
	BR_CODEBEGIN = 18540,
	BR_ACCEPTED  = 18540,	//系统空闲，请求被接受。
	BR_PROCESSING,		    //该请求正在处理。
	BR_DPCERR,			    //目的信令点不符，请求被拒绝。
	BR_PROCPREV,		    //正在处理上一请求或者其它请求，请求被拒绝。
	BR_SYSECPT,			    //系统错误。//意外错误

	BR_FTPCOMFAIL,		    //FTP COM初始化失败。
	BR_FTPGETFAIL,		    //FTP从SCP取文件失败。
	BR_WRITEFILEFAIL,	    //异常话单文件写失败。
	BR_FTPPUTFAIL,		    //FTP送文件给SCP失败超过n(3)次。
	
	BR_GETDPCFAIL,		    //取目的信令点失败。
	BR_THDFAIL,			    //话单恢复工作线程创建失败。
	
	BR_FTPERRBEGIN,
	BR_FTP_SUCCESS = BR_FTPERRBEGIN,	//文件传输成功。
	BR_FTP_WAIT,			//正在进行传输,请稍候(初始化时使用)
	BR_FTP_CONNFAIL,		//连接到FTP Server失败。
	BR_FTP_FINDTRANEXCEP,	//查找或传输文件异常
	BR_FTP_FILENOTEXIST,	//文件不存在。
	BR_FTP_TRANNOFILE,		//所有文件都未传输成功
	BR_FTP_TRANSOMEFILE,	//部分文件未传输成功
	BR_FTP_FAIL,			//FTP操作失败。//与开发人员联系
    
	BR_CODEKEYFAIL,		    //接入码、业务键读入失败。
	BR_ECPTFILEOPENFAIL,    //异常话单文件打开错误。
	BR_RCVRFILECRTFAIL,	    //恢复话单文件创建失败。
	BR_NOBILLRECMATCH,	    //没有找到匹配的话单可恢复。

	//以下三个非返回码，保留用于LoadString
	BR_SUCCESSSTR,	        //success
	BR_RETCODESTR,	        //RETCODE = %d
	BR_DPCSTR,		        //\r\nDPC  =  %s
    
	BR_CODEEND
};

#endif //__BILL_TASK_STRUCT_H__
