#ifndef  __GTP_RELATED_H__
#define  __GTP_RELATED_H__

#include "../include/frame.h"
#include "../comm/comm_struct.h"
#include "protocol_stack.h"
#include "protocol_track.h"
#include "net_app.h"
#include "resource.h"

//支持GTP'协议的应用层每打包一包原始话单，都会生成一个对应的
//状态文件，该状态文件中包含各个协议栈提交到该包原始话单中的
//话单序列号。CGtpSNFileRef抽象了对该状态文件的引用
class CGtpSNFileRef
{
public:
    CGtpSNFileRef(unsigned long const uFileNumber);
    virtual ~CGtpSNFileRef();

    //获得本对象对应的状态文件的编号
    unsigned long GetFileNumber(void);

    //本对象的引用计数加一
    unsigned long AddRef(void);

    //本对象的引用计数减一
    unsigned long Release(BOOL bDeleteFile = TRUE);

protected:

    //该成员变量表示对相应状态文件的引用计数；
    unsigned long m_uRefCount;

    //该成员变量表示相应状态文件的编号，即为对应的原始话单包的包号
    unsigned long m_uFileNumber;

};

typedef struct _GtpSNFile
{
    //描述状态文件的结构
    FILE *pStatusFile;

    //状态文件引用
    CGtpSNFileRef *pFileRef;

}SGtpSNFile;

typedef struct _GtpSNListInFile 
{   
    //序列号状态文件引用
    CGtpSNFileRef *pFileRef;

    //序列号数组
    LIST<unsigned long> *pListSeqNum;

}SGtpSNListInFile;


//以下状态表示帧的状态
#define FS_NULL         0   //帧不存在
#define FS_NEW          1   //新收到
#define FS_SUBMITTED    2   //已提交给存盘模块
#define FS_SAVED        3   //已成功保存

//装入方式常数
#define LA_DISCARD      0   //读入内容丢弃
#define LA_SAVED        1   //装入内容为已保存
#define LA_SUBMITTED    2   //读入内容为已提交

//CSNList抽象了GTP'协议栈中的“序列号列表”
class CSNList
{
public:
    CSNList(void);
    virtual ~CSNList(void);

    //从状态文件中的当前位置装载序列号
    int Load(SGtpSNFile* const pSNFile, int const nAction);

    //将已经提交的序列号保存到状态文件中
    int Save(SGtpSNFile* const pSNFile);

    //添加新的序列号
    void Add(unsigned long const uSeqNum);

    //提交nCount个新的序列号
    void Submit(int const nCount);

    //获得已经提交的序列号列表
    LIST<unsigned long>* const GetSubmitted(void);

    //获得第一个文件号
    unsigned long GetFirstFileNumber(void);

    //判断nSeqNumber是否是已经在本“序列号列表”中
    int Lookup(unsigned long const uSeqNum);

    //前存盘模块应答后，对序列号列表进行相应的处理
    void OnResponse(BOOL const bSaved);

    //清除本对象中的所有序列号
    void Clear(BOOL bDeleteFile);

protected:

    //该成员变量表示删除过时序列号时，允许保留的历史序列号的个数
    //即m_nAllFrameCount大于m_nHisFrameCount时要清除一些记录
    //该成员变量的缺省值为3000，但可以配置；
    int m_nHisFrameCount;

    //该成员变量表示“序列号列表”中当前各种状态（已经被前存盘模
    //块保存的、已经提交但前存盘模块还没有保存的、还没有提交的）
    //序列号的总数
    int m_nAllFrameCount;

    typedef LIST<SGtpSNListInFile*> LIST_PGTPSNLISTINFILE;
    //该链表中的元素为SGtpSNListInFile结构的指针
    LIST_PGTPSNLISTINFILE m_Saved;

    //该链表中的元素为已经提交但前存盘模块还没有成功保存的序列号；
    LIST<unsigned long>* m_pSubmitted;

    //该链表中的元素为还没有提交给前存盘模块进行保存的序列号；
    LIST<unsigned long> m_New;

    //该MAP的键值为序列号，被索引的值为序列号的状态，
    //状态的取值如下： FS_SAVED——已经被前存盘模块保存 
    //                 FS_SUMITTED——已经提交但前存盘模块还没有成功保存
    //                 FS_NEW——还没有提交给前存盘模块进行保存
    MAP<unsigned long, char> m_SeqMap;

    //该指针指向最后一个访问的序列号状态文件的引用对象
    CGtpSNFileRef* m_pLastSNFileRef;

};

//无效的重起计数
#define INVALID_RESTARTCOUNT    0xFFFF

//CGtpStack派生于CProtocolStack，抽象了GTP'协议栈
class CGtpStack:public CProtocolStack
{
public:
    CGtpStack(CNetApp* const pNetApp, 
              unsigned long const uStackId);
    virtual ~CGtpStack();

    //该函数清除历史记录，未应答帧和新数据帧
    void Clear(BOOL bDeleteFile);

    //返回本对象中当前已接收的且可以提交给前存盘模块
    //保存的话单数据的字节数
    virtual int GetReadyDataSize(void);

    //返回本对象中当前已接收的且可以提交给前存盘模块
    //保存的话单数据的记录数
    virtual int GetReadyRecordCount(void);

    //将本协议栈可提交给前存盘模块保存的话单数据放入*pDataBuf对象中
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //处理从与本协议栈对应的GSN发送过来的一帧数据 
    virtual void OnReceive(void* pFrame);

    //对上次提供给前存盘模块保存的话单数据进行后续的处理
    virtual void OnResponse(BOOL const bSaved);

    //从主状态文件中读取本协议栈的状态信息
    virtual int LoadStatus(void);

    //向主状态文件中写入本协议栈的状态信息
    virtual int SaveStatus(void);

    //装入序列号列表
    int LoadSNList(SGtpSNFile *pSNFile);

    //保存序列号列表
    int SaveSNList(SGtpSNFile *pSNFile);
    
    //秒事件处理函数
    virtual void Timer1Sec(void);

    //向对应的主机模块或GSN发送协议帧
    int SendFrame2Ex(void* pFrame);

    //发送Echo Request
    void SendEchoRequest();

    //发送Node Alive Request
    void SendNodeAliveRequest(UINT4 uLocalAddr);

    //发送GTP请求/应答帧
    void SendGTPRequest(SGtpFrame *pFrame);
    void SendGTPResponse(SGtpFrame *pFrame);

    //创建一个GTP帧
    SGtpFrame* CreateGTPFrame(UINT2 uLen);

    //复制一个GTP帧
    SGtpFrame* CopyGTPFrame(SGtpFrame *pFrame);

    //发送版本不支持的GTP帧
    void SendVersionNotSupported(SGtpFrame *pFrame);

    //本函数向GSN发送RedirectionRequest帧
    void SendRedirectionRequest(char* szIpAddress, 
                                BYTE yCause = CAUSE_SYSTEM_FAILURE);

    //退出时是否删除文件
    BOOL m_bDeleteFileOnDestroy;

    //更新本协议栈的GSN的IP地址表
    void UpdateGsnIPTable(VECTOR<UINT4>& ip_table);

    //获得本协议栈的GSN的IP地址表
    void GetGsnIPTable(VECTOR<UINT4>& ip_table);

    //删除一个GSN的IP地址
    void DelGsnIp(UINT4 uIPAddr);

    void SetGsnName(const char* szGsnName)
    {
        strncpy(m_szGsnName, szGsnName, sizeof(m_szGsnName));
        m_szGsnName[sizeof(m_szGsnName) - 1] = '\0';

        strncpy(m_szName, szGsnName, sizeof(m_szName));
        m_szName[sizeof(m_szName) - 1] = '\0';
    }

protected:

    //清除历史记录
    void ResetHisRecord();

    //处理Data Record Transfer消息，处理数据传输请求
    void OnGTPDataTransferRequest(SGtpFrame *pFrame);
    void OnGTPEchoRequest(SGtpFrame *pFrame);
    void OnGTPEchoResponse(SGtpFrame *pFrame);
    void OnGTPResponse(SGtpFrame *pFrame);
    void OnGTPSendData(SGtpFrame *pFrame, BOOL bMaybeDup);

    //处理测试帧
    void OnGTPTestEmptyFrame(SGtpFrame *pFrame);
    void OnGTPMaybeDupData(SGtpFrame *pFrame);
    void OnGTPNormalData(SGtpFrame *pFrame);
    void OnGTPCancelData(SGtpFrame *pFrame);
    void OnGTPReleaseData(SGtpFrame *pFrame);
    void OnGTPNodeAliveRequest(SGtpFrame *pFrame);
    
    //创建/删除数据传输应答帧
    SGtpFrame *CreateTransferResponse(BYTE nCause, 
                                      UINT2 nFrameCount);
    void SendTransferResponse(UINT2 uSeqNum, 
                              BYTE bCause = CAUSE_REQUEST_ACCEPTED);
    void DeleteTransferResponse(SGtpFrame *pFrame);
    //发送重复发来的已保存的帧序列号
    void SendAleadyFulfill();

    //需要保存的状态信息
    SGtpNodeInfo m_GtpNodeInfo;

    //该成员表示本GTP'协议栈的“序列号列表”
    CSNList m_SNList;

    //该链表中的元素为SGtpFrame结构的指针
    //该链表中的协议帧为都是还没有提交给前存盘模块进行保存的
    LIST<SGtpFrame*> m_ListNewFrames;

    //该成员变量表示新接收到的协议帧的个数
    int m_nNewRecordCount;

    //该成员变量表示新接收到的所有协议帧的长度的总和
    int m_nNewDataSize;

    //该链表中的协议帧为已经释放的协议帧
    LIST<SGtpFrame*> m_ListReleasedFrames;

    //该链表中的元素为已经释放的协议帧的序列号
    LIST<unsigned long> m_ListReleasedSN;

    //该链表中的元素为已保存的协议帧的序列号
    LIST<unsigned long> m_ListSavedSN;

    //该指针指向预先分配的应答帧,该帧在构造函数中创建，
    //该帧数据区的缺省大小为10KB,该帧在析构函数中释放
    SGtpFrame* m_pAckFrame;

    //该MAP的键值为请求帧的序列号，元素为SGtpReqFrameInfo结构的指针
    //SGtpReqFrameInfo描述了等待GSN应答的请求，其定义如下：
    typedef MAP<unsigned long, SGtpReqFrameInfo*> MAP_SN_TO_GTPREQ;
    MAP_SN_TO_GTPREQ m_MapReqFrames;
    
    //该成员变量表示向GSN发送请求帧时，下一个可用的序列号
    //该成员变量在构造函数中初始化为1
    unsigned long m_nGTPSeqNumber;

    //发送Echo Request帧的时间计数器
    int m_nEchoReqInvCounter;

    //发送Node Alive Request帧的时间计数器
    int m_nNodeAliveReqInvCounter;

    //工作目录,如"d:\\statusfile\\network\\10.105.32.24\\"
    char m_szWorkDir[MAX_PATH];

    //协议帧头长
    int m_nGtpHeadLen;

    //GTP'协议帧长度限制
    int m_nBufferLimit;

    //GSN名称
    char m_szGsnName[20];
    
    //上次调用SendAleadyFulfill的时间
    ACE_Time_Value  m_tLastSendAdyFulfillTime;

	LIST<time_t>	m_ResTime_new;
	LIST<time_t>	m_ResTime_old;
};

//CGtpNetApp派生于CNetApp，抽象了支持GTP'协议的应用层
class CPerfItem;
class CGtpNetApp:public CNetApp
{
public:
    CGtpNetApp(CNetWork* const pNetWork);
    virtual ~CGtpNetApp();

    //初始化函数
    virtual int Init(void);

    //从主状态文件中读取所有协议栈的状态信息
    virtual int LoadStatus(void);

    //向主状态文件中写入所有协议栈的状态信息
    virtual int SaveStatus(void);

    //搜索工作目录下的所有状态文件并读入其内容
    int LoadPacketSNStatus();

    //保存当前包的序列号列表
    int SavePacketSNStatus();

    //处理来自网络层的协议帧
    virtual void OnReceive(SNetFrame* pFrame);

    //返回新收到帧队列最大长度
    int GetMaxNewFrameCount();

    //返回发送Echo Request帧的时间间隔，单位为分钟
    int GetEchoReqInv();

    //返回REQUEST消息发送超时时间
    int GetRequestTimeOut();

    //返回超时重发次数
    int GetTimeOutRepeat();

    //发送Node Alive Request帧的时间间隔，单位为分钟
    int GetNodeAliveReqInv();

    //获得本机地址
    const char* GetLocalAddress();

    //当前存盘模块通知网络模块启动恢复已经完成后，本函数被调用
    virtual void Active(void);

    //网络层去激活
    virtual void DeActive(void);

    //向所有GSN发重定向信息
    void SendRedirection(void);

    //秒事件处理函数
    virtual void Timer1Sec(void);

    //向对应的主机模块或GSN发送协议帧
    virtual int SendFrame2Ex(MSG_INTRA* pMsg, CProtocolStack* pStack);

    //统计GTP'重复话单数据帧数(GTP_REPEATDATA_FRAME)
    CPerfItem* m_pRptBillPerf;

    //统计GTP'正常话单数据帧数(GTP_NORMALDATA_FRAME)
    CPerfItem* m_pNmlBillPerf;

    //统计GTP'所有重复数据帧数(GTP_ALL_FRAME)
    CPerfItem* m_pAllFrmPerf;

    //统计与所有GSN的连接数(GTP_ALLGSN_CONNECT)
    CPerfItem* m_pGSNConnectPerf;

    //统计网络的队列长度(PERF_GTP_QUEUE_SIZE)
    CPerfItem* m_pGtpQueueSize;

    //统计GTP'帧的吞吐量(PERF_GTP_THROUGHPUT)
    CPerfItem* m_pGtpThroughPut;

	//统计Ga接口平均响应时间
	CPerfItem* m_pGtpResTime;

protected:
    //新话单队列长度
    int m_nMaxNewFrameCount;

    //发送Echo Request帧的时间间隔，单位为分钟
    int m_nEchoReqInv;

    //REQUEST消息发送超时时间
    int m_nRequestTimeOut;

    //超时重发次数
    int m_nTimeOutRepeat;

    //发送Node Alive Request帧的时间间隔，单位为分钟(这是主机要求的)
    int m_nNodeAliveReqInv;

    //本机地址
    char m_szLocalAddr[20];

    //重定向推荐地址
    char m_szRedirectionAddr[20];

    //各IP地址对应于m_GsnMultiIPTable中GSN的索引号
    MAP<UINT4, int>        m_GsnMultiIPMap;

    //所有多地址GSN的IP地址表
    VECTOR<VECTOR<UINT4> > m_GsnMultiIPTable;

    //从IP地址到CGtpStack指针的MAP
    MAP<UINT4, CGtpStack*> m_IPToStackMap;

    //从IP地址到协议栈名的MAP
    MAP<UINT4, STRING>     m_IPToStackName;

    //当前的协议栈ID
    UINT4                  m_uCurrentStackID;

    //待删除的GSN的IP地址，只对单IP地址的GSN有效
    VECTOR<UINT4>          m_DelGsnIPTable;
};

//CGtpTrack派生于CProtocolTrack，实现了GTP'协议数据帧的分析
class CGtpTrack:public CProtocolTrack
{
public:
    CGtpTrack();
    virtual ~CGtpTrack();

    //实施对GTP'协议帧的分析,由函数内部分配内存，调用者不需要释放
    virtual const char* Analysis(void* const pFrame, int nProcRst = 0);

protected:

    //处理Data Record Transfer消息，处理数据传输请求
    void OnGTPDataTransferRequest(SGtpFrame *pFrame);
    void OnGTPDataTransferResponse(SGtpFrame *pFrame);

    void OnGTPEchoResponse(SGtpFrame *pFrame);
    void OnGTPSendData(SGtpFrame *pFrame);

    //处理测试帧
    void OnGTPCancelData(SGtpFrame *pFrame);
    void OnGTPReleaseData(SGtpFrame *pFrame);
    void OnGTPNodeAliveRequest(SGtpFrame *pFrame);
    void OnGTPRedirectionRequest(SGtpFrame *pFrame);
    void OnGTPRedirectionResponse(SGtpFrame *pFrame);

    //获得应答原因值的描述
    const char* GetResponseCauseDesp(int nCause);

};

#endif //__GTP_RELATED_H__
