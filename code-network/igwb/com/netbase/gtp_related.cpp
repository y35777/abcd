#include "ace/INET_Addr.h"
#include "../include/toolbox.h"
#include "../include/perf_item.h"
#include "data_buffer.h"
#include "net_status.h"
#include "network.h"
#include "gtp_related.h"
#include "../utility/mml_report.h"


template<class Tframe, class Tietype>
inline int CHECK_TV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset)
{
    if (pframe->uLen() < noffset + ielen)
    {/*检查本帧是否完整*/ 
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
               S_NET_CHECK_TV_SIZE, 
               pframe->yType(), pframe->uLen(), 
               szTietype, noffset); 
        return -1; 
    } 
    else
    {
        pie = (Tietype *)(pframe->Data() + noffset);
        if (pie->yType() != Tietype::type)
        {/*检查ietype的类型是否正确*/
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TV_IE_TYPE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, noffset);
            return -2;
        }
    }

    return 0;
}

//CHECK_TLV检查一个TLV结构。该宏首先进行TV结构检查。然后检
//查帧长度是否足够。
template<class Tframe, class Tietype>
inline int CHECK_TLV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset)
{
    if (pframe->uLen() < noffset + ielen)
    {/*检查ietype帧头是否完整*/
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
               S_NET_CHECK_TV_SIZE,
               pframe->yType(), pframe->uLen(),
               szTietype, noffset);
        return -1;
    }
    else
    {
        pie = (Tietype *)(pframe->Data() + noffset);
        if (pie->yType() != Tietype::type)
        {/*检查ietype的类型是否正确*/
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TV_IE_TYPE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, noffset);
            return -2;
        }
        if (pframe->uLen() < noffset + IETLV_HEAD_LENGTH + pie->uLen())
        {/*检查本帧是否完整*/
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
                   S_NET_CHECK_TLV_SIZE,
                   pframe->yType(), pframe->uLen(),
                   szTietype, pie->uLen(), noffset);
            return -3;
        }
    }

    return 0;
}

/******************************************************
// 函数名:  CGtpSNFileRef::CGtpSNFileRef
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    构造函数
// 输入:
//       参数1 :unsigned long const nFileNumber——本对象
//              对应的状态文件的编号，即对应的原始话单包的包号
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpSNFileRef::CGtpSNFileRef(unsigned long const uFileNumber)
{
    m_uRefCount = 1;
    m_uFileNumber = uFileNumber;
}


/******************************************************
// 函数名:  CGtpSNFileRef::~CGtpSNFileRef
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpSNFileRef::~CGtpSNFileRef()
{
    //
}


/******************************************************
// 函数名:  CGtpSNFileRef::GetFileNumber
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    获得本对象对应的状态文件的编号
// 输入:
//       参数1 :void
// 返回值: unsigned long 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
unsigned long CGtpSNFileRef::GetFileNumber(void)
{
    return m_uFileNumber;
}


/******************************************************
// 函数名:  CGtpSNFileRef::AddRef
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    本对象的引用计数加一
// 输入:
//       参数1 :void
// 返回值: unsigned long 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
unsigned long CGtpSNFileRef::AddRef(void)
{
    return ++m_uRefCount;
}


/******************************************************
// 函数名:  CGtpSNFileRef::Release
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    本对象的引用计数减一
// 输入:
//       参数1 :BOOL bDeleteFile
// 返回值: unsigned long 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
unsigned long CGtpSNFileRef::Release(BOOL bDeleteFile)
{
    if (m_uRefCount > 0)
    {
        if (--m_uRefCount == 0)
        {
            //删除文件
            if (bDeleteFile)
            {
                char szPath[MAX_PATH];

                sprintf(szPath, 
                        "%s%u.sn", 
                        CNetWork::m_szWorkDir,
                        m_uFileNumber);
                remove(szPath);
            }

            //删除对象本身
            delete this;
            return 0;
        }
    }
    return m_uRefCount;
}


/******************************************************
// 函数名:  CSNList::CSNList
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    构造函数
// 输入:
//       参数1 :void
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CSNList::CSNList(void)
{
    //读取配置文件，获得“默认的历史记录长度”（缺省值为3000）
    //赋值给m_nHisFrameCount
    //打开配置文件
    CINIFile *pIniFile;
    pIniFile = new CINIFile((char *)GetCfgFilePath());
    pIniFile->Open();

    //读取本进程对应的默认的历史记录长度
    char strSec[20];
    sprintf(strSec, 
            "%s%d",
            CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);
    m_nHisFrameCount = pIniFile->GetInt(strSec,
                                        CFG_AP_KEY_HIS_FRM_COUNT,
                                        CFG_AP_KEY_HIS_FRM_COUNT_DEFAULT);
    pIniFile->Close();
    delete pIniFile;

    m_nAllFrameCount = 0;
    m_pLastSNFileRef = 0;
    m_pSubmitted = new LIST<unsigned long>;
}


/******************************************************
// 函数名:  CSNList::~CSNList
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CSNList::~CSNList(void)
{
    //调用Clear(FALSE)函数，完成必要的清理工作
    Clear(FALSE);

    if(m_pSubmitted != 0)
    {
        delete m_pSubmitted;
        m_pSubmitted = 0;
    }
}


/******************************************************
// 函数名:  CSNList::Load
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    从状态文件中的当前位置装载序列号
// 输入:
//       参数1 :SGtpSNFile* const pSNFile
//       参数2 :int const nAction--
//                  1、LA_DISCARD——将装载的序列号都丢弃
//                  2、LA_SAVED——将装载的序列号视为已经
//                                 被前存盘模块保存
//                  3、LA_SUBMITTED——将装载的序列号视为
//                                     已经提交但前存盘模
//                                     块还没有成功保存
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CSNList::Load(SGtpSNFile* const pSNFile, int const nAction)
{
    int nSNLen;
    UINT4 uSN;

    //从pSNFile->m_pStatusFile文件的当前位置将指定协议栈的序列号
    //读入到*pSNArray中

    //读出序列号列表的长度
    if(fread(&nSNLen, 4, 1, pSNFile->pStatusFile) != 1) return ERR_FAIL;

    //创建一个类型为LIST<unsigned long>*的对象，将该对象赋值给pSNArray
    LIST<unsigned long>* pSNArray = new LIST<unsigned long>;
    //读出所有序列号，并添加到列表中
    for(int j = 0; j < nSNLen; j++)
    {
        if(fread(&uSN, 4, 1, pSNFile->pStatusFile) != 1)
        {
            delete pSNArray;
            return ERR_FAIL;
        }
        pSNArray->push_back(uSN);
    }

    if ((nAction != LA_DISCARD) && (pSNArray->size() > 0))
    {
        //设置本次装载的序列号的状态
        LIST<unsigned long>::iterator i;
        for (i = pSNArray->begin(); i != pSNArray->end(); i++)
        {
            unsigned long uSeqNum = *i;
            switch (nAction)
            {
            case LA_SAVED:
                //将映射关系<nSeqNum, FS_SAVED>加入到m_SeqMap中
                m_SeqMap[uSeqNum] = FS_SAVED;
                break;

            case LA_SUBMITTED:
                //将映射关系<nSeqNum, FS_SUBMITTED>加入到m_SeqMap中
                m_SeqMap[uSeqNum] = FS_SUBMITTED;
                break;
            //by ldf 2002.4.17对应问题单D12742
            default:
              {
                  break;
              }
            //修改完毕
            }
        }

        //更新序列号的总数
        m_nAllFrameCount += pSNArray->size();

        //将*pSNArray中的序列号放入m_Saved中或m_Submitted中
        if (LA_SAVED == nAction)
        {
            SGtpSNListInFile* pListInFile = new SGtpSNListInFile;
            pListInFile->pFileRef = pSNFile->pFileRef;
            pListInFile->pListSeqNum = pSNArray;

            //将pListInFile加入到m_Saved的队尾
            m_Saved.push_back(pListInFile);
            pListInFile->pFileRef->AddRef();
            pSNArray = 0;
        }
        else if (LA_SUBMITTED == nAction)
        {
            if (m_pSubmitted != 0)
            {
                delete m_pSubmitted;
                m_pSubmitted = 0;
            }
            m_pSubmitted = pSNArray;
            m_pLastSNFileRef = pSNFile->pFileRef;
            m_pLastSNFileRef->AddRef();
            pSNArray = 0;
        }
    }

    if (pSNArray != 0)
    {
        delete pSNArray;
        pSNArray = 0;
    }
    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CSNList::Save
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    将已经提交的序列号保存到状态文件中
// 输入:
//       参数1 :SGtpSNFile* const pSNFile
// 返回值: int -- 是否成功 
// 注释:    注意：绝对不能在已提交包未应答的情况下进行
//          连续保存状态，这样会造成同一已提交包被同时
//          保存在至少两个序列号列表文件中。
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CSNList::Save(SGtpSNFile* const pSNFile)
{
    //从pSNFile->m_pStatutsFile文件的当前位置顺序写入*m_pSubmitted中
    //的长度及序列号
    ASSERT(pSNFile != 0);

    //分配缓冲区
    int nSize = m_pSubmitted->size();
    int nBufLen = nSize + 1;//以4个字节为单位的长度
    UINT4* pBuf = new UINT4[nBufLen];

    //向缓冲区顺序写入m_pSubmitted中的长度及序列号
    pBuf[0] = nSize;
    LIST<unsigned long>::iterator i;
    int j = 1;
    for (i = m_pSubmitted->begin(); i != m_pSubmitted->end(); i++)
    {
        pBuf[j++] = *i;
    }
    ASSERT(j == nBufLen);
    
    //将缓冲区内的数据写到文件中
    int nWrite = fwrite((char*)pBuf, 
                        sizeof(UINT4) * nBufLen, 
                        1, 
                        pSNFile->pStatusFile);
    delete [] pBuf;
    if(nWrite != 1) return ERR_FAIL;

    if (m_pSubmitted->size() > 0)
    {
        //如果m_pLastSNFileRef != 0，说明当前已提交包未应答，
        //此时再打包或者保存序列号列表都是逻辑错误，这样会造成同一
        //已提交包被同时保存在至少两个序列号列表文件中。
        ASSERT(m_pLastSNFileRef == 0);

        m_pLastSNFileRef = pSNFile->pFileRef;
        m_pLastSNFileRef->AddRef();
    }
    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CSNList::Add
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    添加新的序列号，在调用该函数前，需要先调用
//          Lookup()函数，检查是否为新的话单，如果是才能
//          调用该函数
// 输入:
//       参数1 :unsigned long const nSeqNum
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CSNList::Add(unsigned long const uSeqNum)
{
    //将nSeqNum加入到m_New的队尾
    m_New.push_back(uSeqNum);

    //将映射关系<nSeqNum, FS_NEW>加入到m_SeqMap中
    m_SeqMap[uSeqNum] = FS_NEW;

    m_nAllFrameCount++;
}


/******************************************************
// 函数名:  CSNList::Submit
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    提交nCount个新的序列号
// 输入:
//       参数1 :int const nCount
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CSNList::Submit(int const nCount)
{
    //验证已提交包列表的长度等于0，否则说明当前已提交包还未应答
    ASSERT(m_pSubmitted->size() == 0);

    //将m_New中前nCount个序列号移动到*m_pSubmitted中
    for(int i = 0; i < nCount; i++)
    {
        UINT4 uSeqNum = m_New.front();
        m_New.pop_front();
        m_pSubmitted->push_back(uSeqNum);

        //针对每个被移动的序列号，将映射关系<被移动的序列号, 
        //FS_SUBMITTED>加入到m_SeqMap中
        m_SeqMap[uSeqNum] = FS_SUBMITTED;
    }
}


/******************************************************
// 函数名:  const CSNList::GetSubmitted
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    获得已经提交的序列号列表
// 输入:
//       参数1 :void
// 返回值: LIST<unsigned long>* 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
LIST<unsigned long>* const CSNList::GetSubmitted(void)
{
    return m_pSubmitted;
}


/******************************************************
// 函数名:  CSNList::GetFirstFileNumber
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    获得第一个文件号
// 输入:
//       参数1 :void
// 返回值: unsigned long 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
unsigned long CSNList::GetFirstFileNumber(void)
{
    if (m_Saved.size() > 0)
    {
        SGtpSNListInFile* pListInFile = m_Saved.front();
        return pListInFile->pFileRef->GetFileNumber();
    }
    return 0;
}


/******************************************************
// 函数名:  CSNList::Lookup
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    判断nSeqNumber是否是已经在本“序列号列表”中
// 输入:
//       参数1 :unsigned long const nSeqNum
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CSNList::Lookup(unsigned long const uSeqNum)
{
    //在m_SeqMap中查找键值为nSeqNum的元素，若存在，
    //则返回该元素的取值，否则返回FS_NULL
    if(m_SeqMap.find(uSeqNum) != m_SeqMap.end())
    {
        return m_SeqMap[uSeqNum];
    }
    return FS_NULL;
}


/******************************************************
// 函数名:  CSNList::OnResponse
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    前存盘模块应答后，对序列号列表进行相应的处理
// 输入:
//       参数1 :bool const bSaved--是否保存成功
// 返回值: void
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CSNList::OnResponse(BOOL const bSaved)
{
    unsigned long uSeqNum;

    //检查是否有过时的序列号需要删除
    //删除m_Saved中前nCountRedundant个序列号对应的元素
    int nCountRedundant = m_nAllFrameCount - m_nHisFrameCount;

    //必须在m_Saved中至少保存一个包，否则在获得第一个文件号时可能得到0
    //如m_nHisFrameCount比较小时
    while ((nCountRedundant > 0) && (m_Saved.size() > 1))
    {
        SGtpSNListInFile* pListInFile = m_Saved.front();
        if (pListInFile->pListSeqNum->size() <= nCountRedundant)
        {
            m_nAllFrameCount -= pListInFile->pListSeqNum->size();
            nCountRedundant -= pListInFile->pListSeqNum->size();
            while (pListInFile->pListSeqNum->size() > 0)
            {
                uSeqNum = pListInFile->pListSeqNum->front();
                m_SeqMap.erase(uSeqNum);
                pListInFile->pListSeqNum->pop_front();
            }
            //该SN文件被最后一个协议栈在此处删除
            pListInFile->pFileRef->Release();
            m_Saved.pop_front();
            delete pListInFile->pListSeqNum;
            delete pListInFile;
        }
        else
        {
            break;
        }
    }

    //将已经提交的序列号的状态改变为FS_SAVED，并将这些序列号加入到
    //m_Saved中，同时对m_pLastSNFileRef进行赋值
    if (m_pSubmitted->size() > 0)
    {   
        ASSERT(m_pLastSNFileRef != 0);
        if (bSaved)
        {
            //针对*m_pSubmitted中nCountSubmitted个序列号，
            //将映射关系<序列号, FS_SAVED>加入到m_SeqMap中
            LIST<unsigned long>::iterator i;
            for(i = m_pSubmitted->begin(); i != m_pSubmitted->end(); i++)
            {
                m_SeqMap[*i] = FS_SAVED;
            }

            SGtpSNListInFile* pListInFile = new SGtpSNListInFile;
            pListInFile->pFileRef = m_pLastSNFileRef;
            pListInFile->pListSeqNum = m_pSubmitted;

            //将pListInFile加入到m_Saved的队尾
            m_Saved.push_back(pListInFile);

            //创建一个新队列，并将该队列的指针赋值给m_pSubmitted
            m_pSubmitted = new LIST<unsigned long>;
        }
        else
        {
            //如果保存失败，则删除m_pSubmitted的记录
            //该SN文件被最后一个协议栈在此处删除，此处一定要删除该SN文件，
            //否则在下一次Load时，该文件中的包号会都认为是已成功保存的包号
            m_pLastSNFileRef->Release();
            LIST<unsigned long>::iterator i;
            for(i = m_pSubmitted->begin(); i != m_pSubmitted->end(); i++)
            {
                m_SeqMap.erase(*i);
            }

            //by ldf 2002.4.3 对应问题单 D12381
            m_nAllFrameCount -= m_pSubmitted->size();
            //修改完毕

            m_pSubmitted->clear();
        }
        m_pLastSNFileRef = 0;
    }
}


/******************************************************
// 函数名:  CSNList::Clear
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    清除本对象中的所有序列号
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CSNList::Clear(BOOL bDeleteFile)
{
    //删除m_SeqMap中的所有映射关系
    m_SeqMap.clear();

    //删除m_Saved中所有的元素
    SGtpSNListInFile* pListInFile = 0;
    while (m_Saved.size() > 0)
    {
        pListInFile = m_Saved.front();
        m_Saved.pop_front();
        pListInFile->pFileRef->Release(bDeleteFile);
        delete pListInFile->pListSeqNum;
        delete pListInFile;
    }

    //删除*m_pSubmitted中所有的元素
    m_pSubmitted->clear();

    //删除m_New中所有的元素
    m_New.clear();

    if (m_pLastSNFileRef != 0)
    {
        m_pLastSNFileRef->Release(bDeleteFile);
        m_pLastSNFileRef = 0;
    }
    m_nAllFrameCount = 0;
}


/******************************************************
// 函数名:  CGtpStack::CGtpStack
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    构造函数
// 输入:
//       参数1 :CNetApp* const pNetApp——指向作为网络
//                                        模块应用层的对象
//       参数2 :unsigned long const nStackId--本GTP'协议栈的编号
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpStack::CGtpStack(CNetApp* const pNetApp, 
                     unsigned long const uStackId)
                     :CProtocolStack(pNetApp, uStackId)
{
    m_szGsnName[0]                  = '\0';
    m_nNewDataSize                  = 0;
    m_nNewRecordCount               = 0;
    m_nGTPSeqNumber                 = 1;
    m_nEchoReqInvCounter            = 0;
    m_nNodeAliveReqInvCounter       = 0;
    m_bDeleteFileOnDestroy          = FALSE;
    memset(&m_GtpNodeInfo, 0, GTP_NODE_INFO_LENGTH);
    m_GtpNodeInfo.uGtpVersion       = GTP_V2_TAG1;
    m_nGtpHeadLen                   = GTP_FRAME_HEAD_LENGTH_V2;
    m_GtpNodeInfo.uGsnRestartCount  = INVALID_RESTARTCOUNT;
    m_GtpNodeInfo.uLocalIp          = ACE_OS::inet_addr(
                                      ((CGtpNetApp*)m_pNetApp)->
                                      GetLocalAddress());
    //打开配置文件
    CINIFile IniFile((char *)GetCfgFilePath());
    IniFile.Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);

    //读取连GSN的端口号
    m_GtpNodeInfo.uLocalPort = IniFile.GetInt(strSec,
                                      CFG_AP_KEY_LOCAL_PORT_TO_PM,
                                      CFG_AP_KEY_LOCAL_PORT_TO_PM_GSN_DEFAULT);
    m_nBufferLimit = IniFile.GetInt(strSec,
                                    CFG_AP_KEY_GTP_BUFFER_LIMIT,
                                    CFG_AP_KEY_GTP_BUFFER_LIMIT_DEFAULT);
    if(m_nBufferLimit < 2048) m_nBufferLimit = 2048;
    if(m_nBufferLimit > 65535) m_nBufferLimit = 65535;
    IniFile.Close();

    m_GtpNodeInfo.uStackID = uStackId;

    //创建一个数据区长度为10KB的SGtpStack的实例，将该实例的指针赋值
    //给m_pAckFrame,注意：头长要为最长的计算，即20个字节
    m_pAckFrame = (SGtpFrame*) new char[GTP_FRAME_HEAD_LENGTH_V1 + 
                                        PREPARED_ACKFRAME_SIZE];
    m_pAckFrame->yTag(m_GtpNodeInfo.uGtpVersion);
    m_pAckFrame->FillV0HeadEx();

    m_pStatusInfo = new CStackStatusInfo(STACK_GTP, uStackId);
    sprintf(m_szWorkDir, 
            "%s%d%s", 
            CNetWork::m_szWorkDir, 
            uStackId, 
            ACE_DIRECTORY_SEPARATOR_STR);

    //创建工作目录
    CreateDeepDir(m_szWorkDir);       

    m_tLastSendAdyFulfillTime = ACE_OS::gettimeofday();

	m_ResTime_new.clear();
	m_ResTime_old.clear();
}


/******************************************************
// 函数名:  CGtpStack::~CGtpStack
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    析构函数
// 输入:
//       参数1 :void
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpStack::~CGtpStack(void)
{
    Clear(m_bDeleteFileOnDestroy);

    //删除工作目录中所有内容
    if (m_bDeleteFileOnDestroy)
    {
        SweepDirectory(m_szWorkDir);
    }

    //释放m_pAckFrame指向的实例
    if(m_pAckFrame != 0)
    {
        delete [] m_pAckFrame;
        m_pAckFrame = 0;
    }
}

/******************************************************
// 函数名:  CGtpStack::DelGsnIp
// 作者:    Wangfeng
// 时期:    03-07-30
// 描述:    删除一个GSN的IP地址
// 输入:
//       参数1 :uIPAddr 待删除的IP地址
// 返回值: 无
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//03-07-30      Wangfeng
******************************************************/
void CGtpStack::DelGsnIp(UINT4 uIPAddr)
{
    int nGsnIpCount = sizeof(m_GtpNodeInfo.uGsnIp) / sizeof(m_GtpNodeInfo.uGsnIp[0]);
    BOOL bFound = FALSE;

    for(int i = 0; i < nGsnIpCount; i++)
    {
        if(0 != m_GtpNodeInfo.uGsnIp[i])
        {
            break;
        }
        
        if(m_GtpNodeInfo.uGsnIp[i] == uIPAddr)
        {
            for(int j = i; j < nGsnIpCount - 1; j++)
            {
                m_GtpNodeInfo.uGsnIp[j] = m_GtpNodeInfo.uGsnIp[j + 1];
            }
            m_GtpNodeInfo.uGsnIp[nGsnIpCount - 1] = 0;
        }
        
        if(m_GtpNodeInfo.uGsnIp[i] == m_GtpNodeInfo.nActivedGsnIP)
        {
            bFound = TRUE;
        }
    }

    if(!bFound)
    {
        m_GtpNodeInfo.nActivedGsnIP = m_GtpNodeInfo.uGsnIp[0];

        if(m_szGsnName[0] == '\0')
        {
            struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
            Addr.s_addr = m_GtpNodeInfo.nActivedGsnIP;
#else //_PLATFORM_HPUX
            Addr.S_un.S_addr = m_GtpNodeInfo.nActivedGsnIP;
#endif//_PLATFORM_HPUX
            strncpy(m_szName, ACE_OS::inet_ntoa(Addr), sizeof(m_szName));
            m_szName[sizeof(m_szName) - 1] = '\0';
        }
    }
}

/******************************************************
// 函数名:  CGtpStack::GetGsnIPTable
// 作者:    Wangfeng
// 时期:    03-06-27
// 描述:    获得本协议栈的GSN的IP地址表
// 输入:
//       参数1 :ip_table   等效IP地址表
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//03-06-27      Wangfeng
******************************************************/
void CGtpStack::GetGsnIPTable(VECTOR<UINT4>& ip_table)
{
    int nGsnIpCount = sizeof(m_GtpNodeInfo.uGsnIp) / sizeof(m_GtpNodeInfo.uGsnIp[0]);

    for(int i = 0; i < nGsnIpCount; i++)
    {
        if(m_GtpNodeInfo.uGsnIp[i] == 0)
        {
            break;
        }
        ip_table.push_back(m_GtpNodeInfo.uGsnIp[i]);
    }
}


/******************************************************
// 函数名:  CGtpStack::UpdateGsnIPTable
// 作者:    Wangfeng
// 时期:    03-06-27
// 描述:    更新本协议栈的GSN的IP地址表
// 输入:
//       参数1 :ip_table   GSN的IP地址表
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//03-06-27      Wangfeng
******************************************************/
void CGtpStack::UpdateGsnIPTable(VECTOR<UINT4>& ip_table)
{
    int nGsnIpCount = sizeof(m_GtpNodeInfo.uGsnIp) / sizeof(m_GtpNodeInfo.uGsnIp[0]);
    BOOL bFound = FALSE;

    for(int i = 0; i < nGsnIpCount; i++)
    {
        if(i < ip_table.size())
        {
            m_GtpNodeInfo.uGsnIp[i] = ip_table[i];
        }
        else
        {
            m_GtpNodeInfo.uGsnIp[i] = 0;
        }

        if(0 != m_GtpNodeInfo.uGsnIp[i] && 
            m_GtpNodeInfo.uGsnIp[i] == m_GtpNodeInfo.nActivedGsnIP)
        {
            bFound = TRUE;
        }
    }

    if(!bFound)
    {
        m_GtpNodeInfo.nActivedGsnIP = m_GtpNodeInfo.uGsnIp[0];

        if(m_szGsnName[0] == '\0')
        {
            struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
            Addr.s_addr = m_GtpNodeInfo.nActivedGsnIP;
#else //_PLATFORM_HPUX
            Addr.S_un.S_addr = m_GtpNodeInfo.nActivedGsnIP;
#endif//_PLATFORM_HPUX
            strncpy(m_szName, ACE_OS::inet_ntoa(Addr), sizeof(m_szName));
            m_szName[sizeof(m_szName) - 1] = '\0';
        }
    }
}


/******************************************************
// 函数名:  CGtpStack::Clear
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    该函数清除历史记录，未应答帧和新数据帧
// 输入:
//       参数1 :BOOL bDeleteFile
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CGtpStack::Clear(BOOL bDeleteFile)
{
    SGtpReqFrameInfo *pInfo;
    SGtpFrame *pFrame;

    m_SNList.Clear(bDeleteFile);
    m_ListReleasedSN.clear();

    //释放未成功收到应答的请求帧
    MAP_SN_TO_GTPREQ::iterator i;
    for (i = m_MapReqFrames.begin(); i != m_MapReqFrames.end(); i++)
    {
        pInfo = (*i).second;
        delete pInfo->pFrame;
        delete pInfo;
    }
    m_MapReqFrames.clear();

    //释放未提交的新的数据帧
    while (m_ListNewFrames.size() > 0)
    {
        pFrame = m_ListNewFrames.back();
        m_ListNewFrames.pop_back();
        delete pFrame;
    }
    
    //释放未提交的释放的数据帧
    while (m_ListReleasedFrames.size() > 0)
    {
        pFrame = m_ListReleasedFrames.back();
        m_ListReleasedFrames.pop_back();
        delete pFrame;
    }

    m_nNewDataSize = 0;
    m_nNewRecordCount = 0;
}


/******************************************************
// 函数名:  CGtpStack::GetReadyDataSize
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    返回本对象中当前已接收的且可以提交给前存盘
//          模块保存的话单数据的字节数
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CGtpStack::GetReadyDataSize(void)
{
    return m_nNewDataSize;
}


/******************************************************
// 函数名:  CGtpStack::GetReadyRecordCount
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    返回本对象中当前已接收的且可以提交给前存盘
//          模块保存的话单数据的记录数
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CGtpStack::GetReadyRecordCount(void)
{
    return m_nNewRecordCount;
}


/******************************************************
// 函数名:  CGtpStack::GetReadyData
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    将本协议栈可提交给前存盘模块保存的话单数据
//          放入*pDataBuf对象中
// 输入:
//       参数1 :CDataBuffer* const pDataBuf
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CGtpStack::GetReadyData(CDataBuffer* const pDataBuf)
{
    while (m_ListReleasedFrames.size() > 0)
    {
        //取出已释放的数据帧列表中头部指针
        SGtpFrame* pFrame = m_ListReleasedFrames.front();

        //取出该帧话单的长度及记录数
        //2002-11-23 李杜芳 对应问题单 D19425
        TIEBillPackage* pPackage = NULL;
        if((pFrame->yTag() == GTP_V0_TAG1) || 
           (pFrame->yTag() == GTP_V06_TAG1))
        {
            pPackage = (TIEBillPackage *)pFrame->Data();
        }
        else
        {
            pPackage = (TIEBillPackage *)(pFrame->Data() + 
                                    TIETRANSFER_CMD_LENGTH);
        }
        //修改完毕

        int nDataSize = DATASIZE_OF_IEBILLPACKAGE(pPackage);
        int nDataCount = pPackage->yCount();

        //放到缓冲区中，失败则退出本函数
        if(pDataBuf->Put((char*)pPackage->Data(), nDataSize) != ERR_SUCCESS)
        {
            return;
        }

        //更新当前新收到的数据长度和记录数
        m_nNewDataSize -= nDataSize;
        m_nNewRecordCount -= nDataCount;
        
        //将pFrame->uSeqNum的取值加入到m_ListReleasedSN中
        m_ListReleasedSN.push_back(pFrame->uSeqNum());

        //将pFrame从m_ListReleasedFrames中删除
        m_ListReleasedFrames.pop_front();
        delete pFrame;
    }

    int nFrameCount = 0;
    while (m_ListNewFrames.size() > 0)
    {
        //取出新到数据帧列表中头部指针
        SGtpFrame* pFrame = m_ListNewFrames.front();

        //取出该帧话单的长度及记录数
        //2002-11-23 李杜芳 对应问题单 D19425
        TIEBillPackage* pPackage = NULL;
        if((pFrame->yTag() == GTP_V0_TAG1) || 
           (pFrame->yTag() == GTP_V06_TAG1))
        {
            pPackage = (TIEBillPackage *)pFrame->Data();
        }
        else
        {
            pPackage = (TIEBillPackage *)(pFrame->Data() + 
                                    TIETRANSFER_CMD_LENGTH);
        }
        //修改完毕

        int nDataSize = DATASIZE_OF_IEBILLPACKAGE(pPackage);
        int nDataCount = pPackage->yCount();

        //放到缓冲区中，失败则跳出本循环
        if(pDataBuf->Put((char*)pPackage->Data(), nDataSize) != ERR_SUCCESS)
        {
            break;
        }

        //更新当前新收到的数据长度和记录数
        m_nNewDataSize -= nDataSize;
        m_nNewRecordCount -= nDataCount;

        //将pFrame从m_ListNewFrames中删除
        m_ListNewFrames.pop_front();
        delete pFrame;
        nFrameCount++;
    }

    if (nFrameCount > 0)
    {
        m_SNList.Submit(nFrameCount);
    }
}


/******************************************************
// 函数名:  CGtpStack::OnReceive
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    处理从与本协议栈对应的GSN发送过来的一帧数据
// 输入:
//       参数1 :void* pFrame--接收SNetFrame结构的消息帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CGtpStack::OnReceive(void* pFrame)
{
    SNetFrame* pNetFrame = (SNetFrame *)pFrame;
    SGtpFrame* pGTPFrame = (SGtpFrame *)(pNetFrame->Data);

    //如果消息的数据区长度不足一个GTP'的帧头长度，则退出
    if(pNetFrame->Len < GTP_FRAME_HEAD_LENGTH_V2)
    {
        return;
    }

	//统计ga接口平均响应时间 yg
	time_t ResTime;
	ACE_OS::time(&ResTime);
	m_ResTime_new.push_back(ResTime);
	
    //记录协议栈信息，GsnIp在构造函数中设置
    m_GtpNodeInfo.nActivedGsnIP = pNetFrame->SNetAddrPair.ClientIp;
    m_GtpNodeInfo.uGsnPort      = pNetFrame->SNetAddrPair.ClientPort;
    m_GtpNodeInfo.uLocalIp      = pNetFrame->SNetAddrPair.ServerIp;
    m_GtpNodeInfo.uLocalPort    = pNetFrame->SNetAddrPair.ServerPort;
    m_GtpNodeInfo.uAppType      = pNetFrame->AppType;

    //判断版本是否支持
    if (!pGTPFrame->CheckGTPFrame())
    {
        SendVersionNotSupported(pGTPFrame);
        return;
    }

    if(m_szGsnName[0] == '\0')
    {
        struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        Addr.s_addr = m_GtpNodeInfo.nActivedGsnIP;
#else //_PLATFORM_HPUX
        Addr.S_un.S_addr = m_GtpNodeInfo.nActivedGsnIP;
#endif//_PLATFORM_HPUX
        strncpy(m_szName, ACE_OS::inet_ntoa(Addr), sizeof(m_szName));
        m_szName[sizeof(m_szName) - 1] = '\0';
    }

    if(pNetFrame->Len > m_nBufferLimit)
    {
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
               S_GTP_BUFFER_EXCEED, pNetFrame->Len, m_nBufferLimit);
        m_nBufferLimit = pNetFrame->Len;
    }
    ((CGtpNetApp *)m_pNetApp)->m_pAllFrmPerf->Add(1);

    if(pGTPFrame->yTag() != m_GtpNodeInfo.uGtpVersion)
    {
        m_GtpNodeInfo.uGtpVersion = pGTPFrame->yTag();
        m_nGtpHeadLen = SGtpFrame::GetHeadLen(m_GtpNodeInfo.uGtpVersion);
        m_pAckFrame->yTag(m_GtpNodeInfo.uGtpVersion);
        m_pAckFrame->FillV0HeadEx();
    }

    switch (pGTPFrame->yType())
    {
    //回响请求。GSN可能会发送该命令到CGF以查询CGF是否有效。
    //CGF直接应答该命令并返回重起计数
    case ECHO_REQUEST:
        OnGTPEchoRequest(pGTPFrame);
        break;

    //回响应答。CGF周期性的向GSN发送ECHO_REQUEST，GSN收到该命令
    //之后将发送ECHO_RESPONSE。CGF应从中提取重起计数并做相应处理
    case ECHO_RESPONSE:
        OnGTPEchoResponse(pGTPFrame);
        break;

    //数据传输请求。GSN使用该命令传送话单。根据不同情况有不同处理
    case DATA_TRANSFER_REQUEST:
        OnGTPDataTransferRequest(pGTPFrame);
        break;

    //节点活动命令应答，不需要进行任何特别处理
    case NODE_ALIVE_RESPONSE:
        OnGTPResponse(pGTPFrame);
        break;

    //节点活动请求，
    case NODE_ALIVE_REQUEST:
        OnGTPNodeAliveRequest(pGTPFrame);
        break;

    //重定向应答
    case REDIRECTION_RESPONSE:
        OnGTPResponse(pGTPFrame);
        break;

    //版本不支持
    case VERSION_NOT_SUPPORTED:
        break;

    default:
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
               S_NET_UNRECOGNIZED_FRAME,
               pGTPFrame->yType());
    }

    CProtocolStack::OnReceive(pFrame);
}


/******************************************************
// 函数名:  CGtpStack::SendVersionNotSupported
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    本函数向GSN发送VersionNotSupported帧
// 输入:    SGtpFrame *pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::SendVersionNotSupported(SGtpFrame *pFrame)
{
    SGtpFrame* pGtpFrame = CreateGTPFrame(0);
    ASSERT(NULL != pGtpFrame);

    //填充GTP头
    pGtpFrame->yType(VERSION_NOT_SUPPORTED);
    pGtpFrame->uSeqNum(pFrame->uSeqNum());

    SendGTPResponse(pGtpFrame);
}


/******************************************************
// 函数名:  CGtpStack::SendFrame2Ex
// 作者:    Wangfeng
// 时期:    01-11-6
// 描述:    向对应的主机模块或GSN发送协议帧
// 输入:
//       参数1 :void* pFrame
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-11-6       Wangfeng
******************************************************/
int CGtpStack::SendFrame2Ex(void* pFrame)
{
    //创建一个SNetFrame结构的实例*pMsg
    MSG_INTRA* pMsg = new(((SGtpFrame *)pFrame)->GetSize()) MSG_INTRA;
    if (!pMsg) return ERR_FAIL;

    //将*pFrame中的数据拷贝到*pMsg的数据区中
    memcpy(pMsg->Data, pFrame, pMsg->Len);
    pMsg->ReceiverPid = PT_OUTSIDE;
    pMsg->ReceiverMid = MT_UNKNOWN;
    pMsg->SenderPid   = CMsgProcessor::GetProcessType();
    pMsg->SenderMid   = MT_NETWORK;
    pMsg->AppType     = m_GtpNodeInfo.uAppType;
    ((SNetFrame*)pMsg)->SNetAddrPair.ClientIp   = m_GtpNodeInfo.nActivedGsnIP;
    ((SNetFrame*)pMsg)->SNetAddrPair.ClientPort = m_GtpNodeInfo.uGsnPort;
    ((SNetFrame*)pMsg)->SNetAddrPair.ServerIp   = m_GtpNodeInfo.uLocalIp;
    ((SNetFrame*)pMsg)->SNetAddrPair.ServerPort = m_GtpNodeInfo.uLocalPort;

    return m_pNetApp->SendFrame2Ex(pMsg, this);
}


/******************************************************
// 函数名:  CGtpStack::SendRedirectionRequest
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    本函数向GSN发送RedirectionRequest帧
// 输入:
//       参数1 :char* szIpAddress---推荐给GSN的另一台CG的IP地址
//       参数2 :BYTE yCause
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CGtpStack::SendRedirectionRequest(char* szIpAddress, BYTE yCause)
{
    SGtpFrame *pFrame = NULL;

    UINT4 uAddr = ACE_OS::inet_addr(szIpAddress);
    if(uAddr != 0)
    {
        pFrame = CreateGTPFrame(TIECAUSE_LENGTH +
                                TIERECMD_NODE_ADDR_LENGTH);
    }
    else
    {
        pFrame = CreateGTPFrame(TIECAUSE_LENGTH);
    }

    //填充GTP头
    pFrame->yType(REDIRECTION_REQUEST);
    pFrame->uSeqNum(m_nGTPSeqNumber++);

    //填写TIECause结构
    TIECause* pCause = (TIECause* )pFrame->Data();
    pCause->yType(TIECause::type);
    pCause->yCause(yCause);

    if(uAddr != 0)
    {
        //填写TIERecommendNodeAddress
        TIERecommendNodeAddress* pNodeAddress = (TIERecommendNodeAddress* )
                                        (pFrame->Data() + TIECAUSE_LENGTH);
        pNodeAddress->yType(TIERecommendNodeAddress::type);
        pNodeAddress->uLen(4);   //IPV4的形式
        pNodeAddress->uAddress(uAddr);
    }

    SendGTPRequest(pFrame);
}


/******************************************************
// 函数名:  CGtpStack::OnResponse
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    对上次提供给前存盘模块保存的话单数据进行后续的处理
// 输入:
//       参数1 :bool const bSaved--是否保存成功
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CGtpStack::OnResponse(BOOL const bSaved)
{
//******************************************************************
////统计Ga接口平均响应时间
	m_ResTime_old = m_ResTime_new;
	m_ResTime_new.clear();
	time_t ResTime;
	ACE_OS::time(&ResTime);
	int AverageTime = 0;
	int TotalTime = 0;
	int ListSize = m_ResTime_old.size();
	
	while(m_ResTime_old.size() > 0)
	{
		TotalTime += ResTime - m_ResTime_old.front();
		m_ResTime_old.pop_front();
	}
	if (ListSize != 0)
	{
		AverageTime = TotalTime/ListSize;
		((CGtpNetApp *)m_pNetApp)->m_pGtpResTime->Set(AverageTime);
	}
	else
	{
		((CGtpNetApp *)m_pNetApp)->m_pGtpResTime->Set(0);
	}
//***********************************************************************	

	
    SGtpFrame *pFrame;
    LIST<unsigned long> *pSNArrays[] = {m_SNList.GetSubmitted(), 
                                        &m_ListReleasedSN};
    LIST<unsigned long> *pSNArray;
    TIESNResponse *pSNResponse;

    //如果应答的是空包，则直接退出
    if((m_SNList.GetSubmitted()->size() == 0) && 
       (m_ListReleasedSN.size() == 0))
    {
        return;
    }

    //发送应答帧
    if (bSaved)
    {
        for (int i = 0; i < 2; i++)
        {
            pSNArray = pSNArrays[i];
            if (pSNArray->size() == 0) continue;

            //创建应答帧
            pFrame = CreateTransferResponse(CAUSE_REQUEST_ACCEPTED,
                                            pSNArray->size());
            ASSERT(pFrame != NULL);

            pSNResponse = (TIESNResponse *)(pFrame->Data() + 
                                            TIECAUSE_LENGTH);
            int t = 0;
            LIST<unsigned long>::iterator j;
            for (j = pSNArray->begin(); j != pSNArray->end(); j++)
            {
                int nSeqNumber = *j;
                pSNResponse->uSeqNums(t++, nSeqNumber);
                if (1 == i)
                {
                    char szPath[MAX_PATH];

                    //删除已成功保存的可能重复的文件
                    sprintf(szPath, "%smd%u.dat", 
                            m_szWorkDir, 
                            nSeqNumber);
                    remove(szPath);
                }
            }
            SendGTPResponse(pFrame);
            DeleteTransferResponse(pFrame);
        }
    }

    //处理正常话单序列号列表
    m_SNList.OnResponse(bSaved);
    m_GtpNodeInfo.uFirstSNFile = m_SNList.GetFirstFileNumber();
    m_ListReleasedSN.clear();

    //发送重复发来的已保存的帧序列号
    SendAleadyFulfill();
}

//发送重复发来的已保存的帧序列号
void CGtpStack::SendAleadyFulfill()
{
    SGtpFrame *pFrame;
    TIESNResponse *pSNResponse;

    m_tLastSendAdyFulfillTime = ACE_OS::gettimeofday();

    //如果应答的是空包，则直接退出
    if(m_ListSavedSN.size() == 0)
    {
        return;
    }

    //创建应答帧
    pFrame = CreateTransferResponse(CAUSE_REQUEST_ACCEPTED,
                                    m_ListSavedSN.size());
    ASSERT(pFrame != NULL);

    pSNResponse = (TIESNResponse *)(pFrame->Data() + 
                                    TIECAUSE_LENGTH);
    int t = 0;
    LIST<unsigned long>::iterator j;
    for (j = m_ListSavedSN.begin(); j != m_ListSavedSN.end(); j++)
    {
        int nSeqNumber = *j;
        pSNResponse->uSeqNums(t++, nSeqNumber);
    }

    //发送应答帧
    SendGTPResponse(pFrame);
    DeleteTransferResponse(pFrame);
    m_ListSavedSN.clear();
}


/******************************************************
// 函数名:  CGtpStack::LoadStatus
// 作者:    Wangfeng
// 时期:    01-10-22
// 描述:    从主状态文件中读取本协议栈的状态信息
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
int CGtpStack::LoadStatus(void)
{
    unsigned long uLocalIp   = m_GtpNodeInfo.uLocalIp;
    unsigned long uLocalPort = m_GtpNodeInfo.uLocalPort;

    //调用*m_pStatusInfo对象的Get函数
    UINT4 uInputLen = m_pStatusInfo->Get((BYTE*)(&m_GtpNodeInfo), 
                                         GTP_NODE_INFO_LENGTH);

    m_GtpNodeInfo.uLocalIp   = uLocalIp;
    m_GtpNodeInfo.uLocalPort = uLocalPort;

    m_nGtpHeadLen = SGtpFrame::GetHeadLen(m_GtpNodeInfo.uGtpVersion);
    m_pAckFrame->yTag(m_GtpNodeInfo.uGtpVersion);
    m_pAckFrame->FillV0HeadEx();

    //读状态文件不成功则返回失败
    if(uInputLen != GTP_NODE_INFO_LENGTH) return ERR_FAIL;

    if(m_szGsnName[0] == '\0')
    {
        struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        Addr.s_addr = m_GtpNodeInfo.nActivedGsnIP;
#else //_PLATFORM_HPUX
        Addr.S_un.S_addr = m_GtpNodeInfo.nActivedGsnIP;
#endif//_PLATFORM_HPUX
        strncpy(m_szName, ACE_OS::inet_ntoa(Addr), sizeof(m_szName));
        m_szName[sizeof(m_szName) - 1] = '\0';
    }

    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CGtpStack::LoadSNList
// 作者:    Wangfeng
// 时期:    01-10-26
// 描述:    装入序列号列表
// 输入:
//       参数1 :SGtpSNFile *pSNFile
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
int CGtpStack::LoadSNList(SGtpSNFile *pSNFile)
{
    int nRet = ERR_FAIL;    //是否装入成功
    UINT4 uFileNumber = pSNFile->pFileRef->GetFileNumber();
    UINT4 uCurPacketId = m_pNetApp->GetNetWork()->GetPacketId();

    //当文件号小于最后一次GSN重启时的包号或者小于有效文件号时应丢弃
    if ((uFileNumber < m_GtpNodeInfo.uFirstSNFile) ||
        (uFileNumber < m_GtpNodeInfo.uPIDAtGSNRestart) ||
        (uFileNumber > uCurPacketId))
    {
        nRet = m_SNList.Load(pSNFile, LA_DISCARD);
    }
    else if (uFileNumber < uCurPacketId)
    //当文件号小于当前包Id时应视做已保存
    {
        nRet = m_SNList.Load(pSNFile, LA_SAVED);
    }
    else// if (uFileNumber == uCurPacketId)
    {
        nRet = m_SNList.Load(pSNFile, 
                  m_pNetApp->GetNetWork()->IsWaitForResponse() ? 
                  LA_SUBMITTED : LA_SAVED);             
    }

    if (nRet != ERR_SUCCESS) return nRet;

    //读入被释放序号列表。仅最后一次的被释放列表有效
    int nSize;
    UINT4 uSeqNum;
    m_ListReleasedSN.clear();

    //读取序号个数
    if (1 == fread(&nSize, sizeof(nSize), 1, pSNFile->pStatusFile))
    {
        for (int i = 0; i < nSize; i++)
        {
            if (fread(&uSeqNum, sizeof(UINT4), 1, 
                      pSNFile->pStatusFile) != 1)
            {
                nRet = ERR_FAIL;
                break;
            }
            m_ListReleasedSN.push_back(uSeqNum);
        }
    }
    else
    {
        nRet = ERR_FAIL;
    }

    if ((nRet != ERR_SUCCESS) || 
        !m_pNetApp->GetNetWork()->IsWaitForResponse() ||
        (uFileNumber != uCurPacketId))
    {
        m_ListReleasedSN.clear();
    }

    return nRet;
}


/******************************************************
// 函数名:  CGtpStack::SaveSNList
// 作者:    Wangfeng
// 时期:    01-10-26
// 描述:    保存序列号列表
// 输入:
//       参数1 :SGtpSNFile *pSNFile
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
int CGtpStack::SaveSNList(SGtpSNFile *pSNFile)
{
    //写已经提交的协议帧的序列号
    int nRet = m_SNList.Save(pSNFile);
    if(nRet != ERR_SUCCESS) return nRet;

    //写已经释放的协议帧的序列号
    int nSize = m_ListReleasedSN.size();
    int nBufLen = nSize + 1;
    UINT4* pBuf = new UINT4[nBufLen];
    pBuf[0] = nSize;

    int j = 1;
    LIST<unsigned long>::iterator i;
    for(i = m_ListReleasedSN.begin(); i != m_ListReleasedSN.end(); i++)
    {
        pBuf[j++] = *i;
    }
    ASSERT(j == nBufLen);

    if(fwrite((char*)pBuf, 
              sizeof(UINT4) * nBufLen, 
              1, 
              pSNFile->pStatusFile) != 1)
    {
        nRet = ERR_FAIL;
    }
    delete [] pBuf;
    return nRet;
}


/******************************************************
// 函数名:  CGtpStack::SaveStatus
// 作者:    Wangfeng
// 时期:    01-10-22
// 描述:    向主状态文件中写入本协议栈的状态信息
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
int CGtpStack::SaveStatus(void)
{
    //调用*m_pStatusInfo对象的Set函数
    m_pStatusInfo->Set((BYTE*)(&m_GtpNodeInfo), GTP_NODE_INFO_LENGTH);
    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CGtpStack::Timer1Sec
// 作者:    Wangfeng
// 时期:    01-10-22
// 描述:    秒事件处理函数
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpStack::Timer1Sec(void)
{
    UINT4 uSeqNum;
    int nRequestTimeOut  = ((CGtpNetApp*)m_pNetApp)->GetRequestTimeOut();
    int nTimeoutRepeat   = ((CGtpNetApp*)m_pNetApp)->GetTimeOutRepeat();
    int nEchoReqInv      = ((CGtpNetApp*)m_pNetApp)->GetEchoReqInv();

    if(m_GtpNodeInfo.uAppType == APP_GTP_UDP)
    {
        //定时发Echo Reqest消息帧
        //ZYS修改问题单：D19464 
        if(nEchoReqInv >= 60 && nEchoReqInv <= 300
           && ++m_nEchoReqInvCounter >= nEchoReqInv)
        {
            SendEchoRequest();
            m_nEchoReqInvCounter = 0;
        }
    }

    /*int nNodeAliveReqInv = ((CGtpNetApp*)m_pNetApp)->GetNodeAliveReqInv();

    //定时发送Node Alive Request帧的时间计数器(这是主机要求的)
    if(m_nNodeAliveReqInvCounter++ >= nNodeAliveReqInv * ONE_MINUTE_IN_SECOND)
    {
        SendNodeAliveRequest(m_GtpNodeInfo.uLocalIp);
        m_nNodeAliveReqInvCounter = 0;
    }主机方暂时去掉该需要求*/

    //检查m_MapReqFrames中是否存在超时的Request
    LIST <UINT4> ListWillBeRemovedSeqNum;
    MAP_SN_TO_GTPREQ::iterator i;
    for (i = m_MapReqFrames.begin(); i != m_MapReqFrames.end(); i++)
    {
        uSeqNum = (*i).first;
        SGtpReqFrameInfo *pInfo = (*i).second;

        if (++(pInfo->yTimeOut) >= nRequestTimeOut)
        {
            pInfo->yTimeOut = 0;
            pInfo->yRepeatTimes++;
            SendFrame2Ex(pInfo->pFrame);
            if (pInfo->yRepeatTimes >= nTimeoutRepeat)
            {
				// 产生“GSN”故障告警消息
				SInnerAlarm alarm;
				alarm.uAlarmID      = ALARM_ID_REQUSET_TIMEOUT;  // GSN无响应
				alarm.yAlarmType    = AT_FAULT;                // 故障告警
				alarm.yAlarmPara[0] = ALARM_PATA_GSN_NO_REQUEST;

				//参数中填写IP
				ACE_INET_Addr AddrClient;				
				AddrClient.set((u_short)0, NTOHL(m_GtpNodeInfo.nActivedGsnIP));
				sprintf((char*)&alarm.yAlarmPara[1], "%s", AddrClient.get_host_addr());
				alarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
				CMsgProcessor::SendAlarm(&alarm);            	
                ListWillBeRemovedSeqNum.push_back(uSeqNum);
                delete pInfo->pFrame;
                delete pInfo;
            }
        }
    }

    //删除m_MapReqFrames中已超时的序列号
    while (ListWillBeRemovedSeqNum.size() > 0)
    {
        uSeqNum = ListWillBeRemovedSeqNum.front();
        ListWillBeRemovedSeqNum.pop_front();
        m_MapReqFrames.erase(uSeqNum);
    }

    ACE_Time_Value tNow = ACE_OS::gettimeofday() - m_tLastSendAdyFulfillTime;
    if(m_ListSavedSN.size() > 0 && tNow >= ACE_Time_Value(2))
    {
        //发送重复发来的已保存的帧序列号
        SendAleadyFulfill();
    }

    CProtocolStack::Timer1Sec();
}


/******************************************************
// 函数名:  CGtpStack::SendEchoRequest
// 作者:    Wangfeng
// 时期:    01-10-24
// 描述:    发送Echo Request
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-24      Wangfeng
******************************************************/
void CGtpStack::SendEchoRequest()
{
    SGtpFrame *pFrame = CreateGTPFrame(0);
    if (pFrame != NULL)
    {
        //填充GTP头
        pFrame->yType(ECHO_REQUEST);
        pFrame->uSeqNum(m_nGTPSeqNumber++);

        SendGTPRequest(pFrame);
    }
}


/******************************************************
// 函数名:  CGtpStack::SendGTPRequest
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    发送GTP'请求帧,该函数发送请求帧并将其放入未应答帧队列
// 输入:
//       参数1 :SGtpFrame *pFrame -- 需要发送的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::SendGTPRequest(SGtpFrame *pFrame)
{
    UINT4 uSeqNum;
    SGtpReqFrameInfo *pInfo = NULL;

    //ZYS修改问题单：D19464 ECHO_REQUEST特殊
	//if(pFrame->yType() != ECHO_REQUEST)
	{	
		uSeqNum = pFrame->uSeqNum();
		if (m_MapReqFrames.find(uSeqNum) != m_MapReqFrames.end())
		{
			pInfo = m_MapReqFrames[uSeqNum];
			delete pInfo->pFrame;
		}
		else
		{
			pInfo = new SGtpReqFrameInfo;
		}
		
		pInfo->pFrame = pFrame;
		pInfo->yTimeOut = 0;
		pInfo->yRepeatTimes = 0;
		m_MapReqFrames[uSeqNum] = pInfo;
	}

    SendFrame2Ex(pFrame);
}


/******************************************************
// 函数名:  CGtpStack::SendGTPResponse
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    发送GTP应答帧
// 输入:
//       参数1 :SGtpFrame *pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::SendGTPResponse(SGtpFrame *pFrame)
{
    SendFrame2Ex(pFrame);
}


/******************************************************
// 函数名:  CGtpStack::SendNodeAliveRequest
// 作者:    Wangfeng
// 时期:    01-10-25
// 描述:    发送Node Alive Request
// 输入:
//       参数1 :UINT4 uLocalAddr -- 指定本端地址
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
void CGtpStack::SendNodeAliveRequest(UINT4 uLocalAddr)
{
/*
    //如果不是UDP方式，则不用用NodeAlive消息
    if(m_GtpNodeInfo.uAppType != APP_GTP_UDP)
    {
        return;
    }
*/

    SGtpFrame *pFrame;
    TIENodeAddress *pAddress;

    pFrame = CreateGTPFrame(TIENODE_ADDRESS_LENGTH);
    if (pFrame != NULL)
    {
        //填充GTP头
        //2002-12-09 李杜芳 对应问题单 D19626
        pFrame->yType(NODE_ALIVE_REQUEST);
        //修改完毕
        pFrame->uSeqNum(m_nGTPSeqNumber++);

        //填充IE结构
        pAddress = (TIENodeAddress *)pFrame->Data();
        pAddress->yType(TIENodeAddress::type);
        pAddress->uLen(4);
        pAddress->uAddress(uLocalAddr);
        SendGTPRequest(pFrame);
        MSGOUT(MTS_NETWORK, 
               MSGOUT_LEVEL_IMPORTANT, 
               S_NET_SEND_NODE_ALIVE_REQ);
    }
}


/******************************************************
// 函数名:  CGtpStack::CreateGTPFrame
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    创建一个GTP帧
// 输入:
//       参数1 :UINT2 uLen -- 所创建的帧的数据段长度
// 返回值: SGtpFrame* 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
SGtpFrame* CGtpStack::CreateGTPFrame(UINT2 uLen)
{
    int nSize = m_nGtpHeadLen + uLen;
    SGtpFrame *pFrame = (SGtpFrame *)new BYTE[nSize];

    if (pFrame)
    {
        memset(pFrame, 0, nSize);
        pFrame->yTag(m_GtpNodeInfo.uGtpVersion);
        pFrame->FillV0HeadEx();
        pFrame->uLen(uLen);
    }
    return pFrame;
}


/******************************************************
// 函数名:  CGtpStack::CopyGTPFrame
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    复制一个GTP帧
// 输入:
//       参数1 :SGtpFrame *pFrame
// 返回值: SGtpFrame* 
// 注释:
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
SGtpFrame* CGtpStack::CopyGTPFrame(SGtpFrame *pFrame)
{
    SGtpFrame *pResult;
    pResult = (SGtpFrame *)new BYTE[pFrame->GetSize()];
    if (pResult != NULL)
    {
        memcpy(pResult, pFrame, pFrame->GetSize());
    }

    return pResult;
}


/******************************************************
// 函数名:  CGtpStack::CreateTransferResponse
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    该函数创建一个指定大小的数据传输应答帧并填写一
//   部分帧内容。由于该函数在大多数情况下将直接使用预先
//   分配好的m_pAckFrame而不会分配新的内存，故应和函数
//   eleteTransferResponse配合使用，以正确的释放内存
// 输入:
//       参数1 :BYTE nCause--应答原因码
//       参数2 :WORD nFrameCount--应达序列号的个数
// 返回值: SGtpFrame* --已初始化的应答帧
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
SGtpFrame* CGtpStack::CreateTransferResponse(BYTE nCause, 
                                             UINT2 uFrameCount)
{
    SGtpFrame *pRespFrame;
    UINT2 uSize = TIECAUSE_LENGTH + 
                  TIESN_RESPONSE_HEAD_LENGTH + 
                  uFrameCount * 2;

    //若m_pAckFrame空间足够，则使用m_pAckFrame否则分配一个新的
    if (uSize <= PREPARED_ACKFRAME_SIZE)
    {
        pRespFrame = m_pAckFrame;
    }
    else
    {
        pRespFrame = CreateGTPFrame(uSize);
    }

    //填充帧内容
    if (pRespFrame != NULL)
    {
        TIECause *pCause;
        TIESNResponse *pResponse;

        //填充GTP协议头
        pRespFrame->yTag(m_GtpNodeInfo.uGtpVersion);
        pRespFrame->FillV0HeadEx();
        pRespFrame->yType(DATA_TRANSFER_RESPONSE);
        pRespFrame->uSeqNum(0);
        pRespFrame->uLen(uSize);

        //填充cause结构
        pCause = (TIECause *)(pRespFrame->Data());
        pCause->yType(TIECause::type);
        pCause->yCause(nCause); 

        //填充request reponse结构头
        pResponse = (TIESNResponse *)
            (pRespFrame->Data() + TIECAUSE_LENGTH);
        pResponse->yType(TIESNResponse::type);
        pResponse->uLen(uFrameCount * 2);
    };

    return pRespFrame;
}


/******************************************************
// 函数名:  CGtpStack::SendTransferResponse
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    大多数情况下一个应答帧应答多个数据帧，但对可能
//   重复话单和取消数据命令的应答是一帧一应答，该函数提
//   供对指定包号应答功能
// 输入:
//       参数1 :UINT2 uSeqNum -- 需要应答的帧的序列号
//       参数2 :BYTE bCause -- 应答原因值
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::SendTransferResponse(UINT2 uSeqNum, BYTE bCause)
{
    SGtpFrame *pRespFrame = CreateTransferResponse(bCause, 1);
    if (pRespFrame != NULL)
    {
        TIESNResponse *pSNResponse =(TIESNResponse *)
                (pRespFrame->Data() + TIECAUSE_LENGTH);
        pSNResponse->uSeqNums(0, uSeqNum);
        SendGTPResponse(pRespFrame);
        DeleteTransferResponse(pRespFrame);
    }
}


/******************************************************
// 函数名:  CGtpStack::DeleteTransferResponse
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    删除由CreateTransferResponse所创建的帧
// 输入:
//       参数1 :SGtpFrame *pFrame由CreateTransferResponse
//                               创建的应答帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::DeleteTransferResponse(SGtpFrame *pFrame)
{
    if (pFrame != m_pAckFrame)
    {
        delete pFrame;
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPCancelData
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    处理取消的数据帧,该函数将相应包的文件删除之后并应答
// 输入:
//       参数1 :SGtpFrame *pFrame--包含可能重复话单的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPCancelData(SGtpFrame *pFrame)
{
    SGtpFrame *pRespFrame;
    TIESNCanceled *pSNCanceled;
    TIESNResponse *pSNResponse;

    int nChkRst = CHECK_TLV(pFrame, "TIESNCanceled", 
                            TIESN_CANCELED_HEAD_LENGTH,
                            pSNCanceled, 
                            TIETRANSFER_CMD_LENGTH);
    if(nChkRst != 0)
    {
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_OPT_IE_INCRT);
        return;
    }

    //删除相应的文件
    //pSNCanceled->uLen中存放了应删除话单序列号列表的字节数
    //因一个话单序列号存放在两个字节中，所以话单数等于字节数除2
    for (int i = 0; i < pSNCanceled->uLen() / 2; i++)
    {
        char szPath[MAX_PATH];

        sprintf(szPath, 
                "%smd%u.dat", 
                m_szWorkDir, 
                pSNCanceled->uSeqNums(i));
        remove(szPath);
    }

    //发送应答帧
    pRespFrame = CreateTransferResponse(CAUSE_REQUEST_ACCEPTED, 
                                        pSNCanceled->uLen() / 2);
    if (pRespFrame != NULL)
    {
        pSNResponse = (TIESNResponse *)
                      (pRespFrame->Data() + TIECAUSE_LENGTH);
        memcpy(pSNResponse->Data(), 
               pSNCanceled->Data(), 
               pSNCanceled->uLen());
        pRespFrame->uSeqNum(pFrame->uSeqNum());
        SendGTPResponse(pRespFrame);
        DeleteTransferResponse(pRespFrame);
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPDataTransferRequest
// 作者:    Wangfeng
// 时期:    01-10-22
// 描述:    数据传输请求处理函数
// 输入:
//       参数1 :SGtpFrame *pFrame 所收到的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpStack::OnGTPDataTransferRequest(SGtpFrame *pFrame)
{
    //如果TIETransferCommand必选项未填
    if(pFrame->uLen() == 0)
    {
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_MAND_IE_MISSING);

        return;
    }

    //2002-11-23 李杜芳 对应问题单 D19425
    if((pFrame->yTag() == GTP_V0_TAG1) || 
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        ((CGtpNetApp *)m_pNetApp)->m_pNmlBillPerf->Add(1);
        OnGTPSendData(pFrame, FALSE);
        return;
    }
    //修改完毕

    TIETransferCommand *pCommand;
    int nChkRst = CHECK_TV(pFrame, "TIETransferCommand", 
                           TIETRANSFER_CMD_LENGTH,
                           pCommand, 0);
    //如果TIETransferCommand必选项填写错误
    if(nChkRst != 0)
    {
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_MAND_IE_INCRT);
        return;
    }

    switch (pCommand->yCmd())
    {
    //收到数据帧
    case TC_SENDDATA:
        ((CGtpNetApp *)m_pNetApp)->m_pNmlBillPerf->Add(1);
        OnGTPSendData(pFrame, FALSE);
        break;

    //收到可能重复的数据帧
    case TC_SENDDATAMAYBEDUP:
        ((CGtpNetApp *)m_pNetApp)->m_pRptBillPerf->Add(1);
        OnGTPSendData(pFrame, TRUE);
        break;

    //申明上次送来的可能重复的数据帧无效
    case TC_CANCELDATA:
        OnGTPCancelData(pFrame);
        break;

    //申明上次送来的可能重复的数据帧有效
    case TC_RELEASEDATA:
        OnGTPReleaseData(pFrame);
        break;

    default:
        MSGOUT(MTS_NETWORK,
               MSGOUT_LEVEL_IMPORTANT,
               S_NET_UNRECOGNIZED_FRAME, 
               pCommand->yCmd());
        break;
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPEchoRequest
// 作者:    Wangfeng
// 时期:    01-10-22
// 描述:    处理Echo Request帧
// 输入:
//       参数1 :SGtpFrame *pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpStack::OnGTPEchoRequest(SGtpFrame *pFrame)
{
    SGtpFrame *pRespFrame;
    TIERecovery *pRecovery;

    pRespFrame = CreateGTPFrame(TIERECOVERY_LENGTH);
    if (pRespFrame != NULL)
    {
        //填充GTP头
        pRespFrame->yType(ECHO_RESPONSE);
        pRespFrame->uSeqNum(pFrame->uSeqNum());

        //填充IE结构
        pRecovery = (TIERecovery *)pRespFrame->Data();
        pRecovery->yType(TIERecovery::type);
        pRecovery->yRestartCount(0);
        SendGTPResponse(pRespFrame);
        delete pRespFrame;
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPEchoResponse
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    该函数比较GSN重起计数以检查GSN是否重起。若
//   发现GSN重起则清除所有状态
// 输入:
//       参数1 :SGtpFrame *pFrame -- 所收到的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPEchoResponse(SGtpFrame *pFrame)
{
    TIERecovery *pRecovery;
    int nChkRst = CHECK_TV(pFrame, "TIERecovery", 
                           TIERECOVERY_LENGTH,
                           pRecovery, 0);
    if(nChkRst != 0)
    {
        return;
    }

    if (INVALID_RESTARTCOUNT == m_GtpNodeInfo.uGsnRestartCount) 
        m_GtpNodeInfo.uGsnRestartCount = pRecovery->yRestartCount();

    if (m_GtpNodeInfo.uGsnRestartCount != pRecovery->yRestartCount())
    {
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
               S_NET_GSN_RESTART, m_szName, pRecovery->yRestartCount());
        TRACE(MTS_NETWORK, S_NET_GSN_RESTART,
              m_szName, pRecovery->yRestartCount());

        //清除历史记录
        ResetHisRecord();

        //保存状态
        m_GtpNodeInfo.uGsnRestartCount = pRecovery->yRestartCount();
        m_GtpNodeInfo.uPIDAtGSNRestart = m_pNetApp->GetNetWork()->GetPacketId();
        SaveStatus();
        m_pStatusInfo->Flush();
    }

    //清除历史请求帧
    OnGTPResponse(pFrame);
}


/******************************************************
// 函数名:  CGtpStack::ResetHisRecord
// 作者:    Wangfeng
// 时期:    01-11-5
// 描述:    清除历史记录
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-11-5       Wangfeng
******************************************************/
void CGtpStack::ResetHisRecord()
{
    //删除历史记录
    Clear(TRUE);

    //删除所有可能重复话单
    ACE_DIR *pdir = ACE_OS::opendir(m_szWorkDir);
    if(NULL == pdir) return;

    dirent *pdirent;
    char szPath[MAX_PATH];
    const char *szFileName;
    while(pdirent = ACE_OS::readdir(pdir))
    {
        szFileName = pdirent->d_name;
        if(strstr(szFileName, "md") == szFileName)
        {
            sprintf(szPath, 
                    "%s%s", 
                    m_szWorkDir, 
                    szFileName);
            ACE_stat subStat;
            ACE_OS::stat(szPath, &subStat);
            if((subStat.st_mode & S_IFDIR) == 0)
            {
                remove(szPath);
            }
        }
        
    }
    ACE_OS::closedir(pdir);
}


/******************************************************
// 函数名:  CGtpStack::OnGTPMaybeDupData
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    处理可能重复的帧，该函数将可能重复的帧存入该
//          通道私有目录下的以帧号为名的文件中并应答
// 输入:
//       参数1 :SGtpFrame *pFrame -- 包含可能重复话单的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPMaybeDupData(SGtpFrame *pFrame)
{
    //将数据帧保存到以帧号为名的文件中
    char szPath[MAX_PATH];
    sprintf(szPath, 
            "%smd%u.dat", 
            m_szWorkDir,
            pFrame->uSeqNum()); 
    FILE *pFile = fopen(szPath, "wb");
    if (pFile != NULL)
    {
        int nSize = m_nGtpHeadLen + pFrame->uLen();
        int nWrite = fwrite(pFrame, nSize, 1, pFile);
        if (nWrite != 1)
        {
            fclose(pFile);
            remove(szPath);
            return;
        }
        fflush(pFile);
        fclose(pFile);

        //发送应答帧
        SendTransferResponse(pFrame->uSeqNum());
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPNodeAliveRequest
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    Node Alive Request处理函数
// 输入:
//       参数1 :SGtpFrame *pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPNodeAliveRequest(SGtpFrame *pFrame)
{
    //如果IP地址形式为ipv6，则返回Versoin Not Supported消息
    TIENodeAddress* pIpAddress = (TIENodeAddress* )(pFrame->Data());
    if (pFrame->uLen() < TIENODE_ADDRESS_LENGTH || pIpAddress->uLen() != 4)
    {
        SendVersionNotSupported(pFrame);
        return;
    }

    MSGOUT(MTS_NETWORK, 
           MSGOUT_LEVEL_IMPORTANT, 
           S_NET_RECV_NODE_ALIVE_REQ,
           m_szName);

    //在收到了GSN的NodeAliveRequest帧后，由于主机重启，必须清空所有的历史记录
    ResetHisRecord();

    //保存状态
    m_GtpNodeInfo.uGsnRestartCount = INVALID_RESTARTCOUNT;
    m_GtpNodeInfo.uPIDAtGSNRestart = m_pNetApp->GetNetWork()->GetPacketId();
    SaveStatus();
    m_pStatusInfo->Flush();

    //发送应答
    SGtpFrame *pRespFrame;
    pRespFrame = CreateGTPFrame(0);
    if (pRespFrame != NULL)
    {
        //填充GTP头
        pRespFrame->yType(NODE_ALIVE_RESPONSE);
        pRespFrame->uSeqNum(pFrame->uSeqNum());

        SendGTPResponse(pRespFrame);
        delete pRespFrame;
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPNormalData
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    处理正常的数据帧,该函数首先查找所收到的帧是
//          否已经保存，若已经保存则补发应答，否则保留该
//          数据等待保存
// 输入:
//       参数1 :SGtpFrame *pFrame -- 包含可能重复话单的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPNormalData(SGtpFrame *pFrame)
{
    TIEBillPackage *pPackage;

    //2002-11-23 李杜芳 对应问题单 D19425
    if((pFrame->yTag() == GTP_V0_TAG1) || 
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        pPackage = (TIEBillPackage *)pFrame->Data();
    }
    else
    {
        pPackage = (TIEBillPackage *)(pFrame->Data() + 
                                TIETRANSFER_CMD_LENGTH);
    }
    //修改完毕

    switch (m_SNList.Lookup(pFrame->uSeqNum()))
    {
    //在历史记录中未发现该序列号，故为新话单，应加入新话单队列并保存
    case FS_NULL:
        {
            if (m_ListNewFrames.size() < 
                ((CGtpNetApp*)m_pNetApp)->GetMaxNewFrameCount())
            {
                SGtpFrame *pNewFrame = CopyGTPFrame(pFrame);
                if (pNewFrame != NULL)
                {
                    m_ListNewFrames.push_back(pNewFrame);
                    m_nNewDataSize += DATASIZE_OF_BILLPACKAGE(pFrame);
                    m_nNewRecordCount += pPackage->yCount();
                    m_SNList.Add(pFrame->uSeqNum());
                    //张玉新 2003-05-16
                    m_pNetApp->m_pReceiveBillCnt->Add(pPackage->yCount());                    
                }
            }
        }
        break;

    //在历史记录中发现该序列号的帧已被成功
    //保存，应补发应答
    case FS_SAVED:
        if(std::find(m_ListSavedSN.begin(),
                     m_ListSavedSN.end(),
                     pFrame->uSeqNum()) == m_ListSavedSN.end())
        {
            m_ListSavedSN.push_back(pFrame->uSeqNum());
        }

        {
            ACE_Time_Value tNow = ACE_OS::gettimeofday() - m_tLastSendAdyFulfillTime;
            if(m_ListSavedSN.size() >= 100 || tNow >= ACE_Time_Value(2))
            {
                //发送重复发来的已保存的帧序列号
                SendAleadyFulfill();
            }
        }
        break;

    //可能出现的其它值为FS_NEW和FS_SUBMITTED
    //分别表示新收到的帧和正等待应答的帧，应
    //直接丢弃
    default:
        break;
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPReleaseData
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    处理释放的数据帧,该函数将相应的被释放的文件
//          读入内存等待下一次被提交，然后删除相应的文件
// 输入:
//       参数1 :SGtpFrame *pFrame -- 包含可能重复话单的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPReleaseData(SGtpFrame *pFrame)
{
    TIESNReleased *pSNReleased;

    int nChkRst = CHECK_TLV(pFrame, "TIESNReleased",
                            TIESN_RELEASED_HEAD_LENGTH,
                            pSNReleased,
                            TIETRANSFER_CMD_LENGTH);
    if(nChkRst != 0)
    {
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_OPT_IE_INCRT);
        return;
    }

    for (int i = 0; i < pSNReleased->uLen() / 2; i++)
    {
        char szPath[MAX_PATH];
        UINT2 uSeqNum = pSNReleased->uSeqNums(i);

        //将数据帧保存到以帧号为名的文件中
        sprintf(szPath, 
                "%smd%u.dat", 
                m_szWorkDir, 
                uSeqNum);
        FILE *pFile = fopen(szPath, "rb");
        if (pFile != NULL)
        {
            fseek(pFile, 0, SEEK_END);
            int nLen = ftell(pFile);
            if (nLen > m_nGtpHeadLen)
            {
                SGtpFrame *pGtpFrame = CreateGTPFrame(nLen - m_nGtpHeadLen);
                if (pGtpFrame != NULL)
                {
                    fseek(pFile, 0, SEEK_SET);
                    //如果正确读出且长度正确则放入释放队列准备发送
                    if ((1 == fread(pGtpFrame, nLen, 1, pFile)) &&
                        ((nLen - m_nGtpHeadLen) == pGtpFrame->uLen()))
                    {
                        m_ListReleasedFrames.push_back(pGtpFrame);

                        TIEBillPackage *pPackage;

                        //2002-11-23 李杜芳 对应问题单 D19425
                        if((pGtpFrame->yTag() == GTP_V0_TAG1) || 
                           (pGtpFrame->yTag() == GTP_V06_TAG1))
                        {
                            pPackage = (TIEBillPackage *)pGtpFrame->Data();
                        }
                        else
                        {
                            pPackage = (TIEBillPackage *)(pGtpFrame->Data() + 
                                                    TIETRANSFER_CMD_LENGTH);
                        }
                        //修改完毕

                        m_nNewRecordCount += pPackage->yCount();
                        m_nNewDataSize += DATASIZE_OF_BILLPACKAGE(pGtpFrame);
                    }
                    else
                    {
                        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
                               S_NET_READ_ERR_IN_RELEASE, szPath);
                    }
                }
            }
            fclose(pFile);
        }
        //如果找不到文件，则直接返回响应帧
        else
        {
            SendTransferResponse(uSeqNum);
        }
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPResponse
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    GTP'应答处理函数, 
//          该函数发送请求帧并将其放入未应答帧队列
//          该函数查找未应答帧列表，若找到则删除该未应答帧
// 输入:
//       参数1 :SGtpFrame *pGTPFrame -- 所收到的应答帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPResponse(SGtpFrame *pGTPFrame)
{
    UINT4 uSeqNum;
    SGtpReqFrameInfo *pInfo;
    
    uSeqNum = pGTPFrame->uSeqNum();
    if (m_MapReqFrames.find(uSeqNum) != m_MapReqFrames.end())
    {
        pInfo = (SGtpReqFrameInfo*)m_MapReqFrames[uSeqNum];
        delete pInfo->pFrame;
        delete pInfo;
        m_MapReqFrames.erase(uSeqNum);
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPSendData
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    话单数据接收处理
//          函数检查数据包并采取适当处理
//          可能有三种数据:
//          1. 正常数据，应判断数据是否重复并根据情况发送应答或
//             加入等待保存队列；
//          2. test empty frame，应查找已保存话单队列，立即应答；
//          3. 可能重复数据，应保存并立即应答
// 输入:
//       参数1 :SGtpFrame *pFrame
//       参数2 :BOOL bMaybeDup
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpStack::OnGTPSendData(SGtpFrame *pFrame, BOOL bMaybeDup)
{
    TIEBillPackage *pPackage;

    //2002-11-23 李杜芳 对应问题单 D19425
    if((pFrame->yTag() == GTP_V0_TAG1) ||
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        int nChkRst = CHECK_TLV(pFrame, "TIEBillPackage", 
                                TIEBILL_BACKAGE_HEAD_LENGTH,
                                pPackage, 0);
        if(nChkRst != 0)
        {
            SendTransferResponse(pFrame->uSeqNum(), 
                                 CAUSE_MAND_IE_INCRT);
            return;
        }
    }
    else
    {
        int nChkRst = CHECK_TLV(pFrame, "TIEBillPackage", 
                                TIEBILL_BACKAGE_HEAD_LENGTH,
                                pPackage,
                                TIETRANSFER_CMD_LENGTH);
        if(nChkRst != 0)
        {
            if(pFrame->uLen() >= TIETRANSFER_CMD_LENGTH + IETLV_HEAD_LENGTH)
            {
                pPackage = (TIEBillPackage*) (pFrame->Data() + TIETRANSFER_CMD_LENGTH);
                if(pPackage->yType() == TIEBillPackage::type && pPackage->uLen() == 0)
                {
                    OnGTPTestEmptyFrame(pFrame);
                    return;
                }
            }
            SendTransferResponse(pFrame->uSeqNum(), 
                                 CAUSE_OPT_IE_INCRT);
            return;
        }
    }
    //修改完毕

    //记录数为0为test empty frame
    if (0 == pPackage->yCount())
    {
        OnGTPTestEmptyFrame(pFrame);
        return;
    }

    //检查长度是否足够
    if (DATASIZE_OF_IEBILLPACKAGE(pPackage) < 0)
    {
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, 
               S_NET_WRONG_IEBILLPACKAGE,
               pPackage->yCount(), pPackage->uLen());
        if((pFrame->yTag() == GTP_V0_TAG1) ||
           (pFrame->yTag() == GTP_V06_TAG1))
        {
            SendTransferResponse(pFrame->uSeqNum(), 
                                 CAUSE_MAND_IE_INCRT);
        }
        else
        {
            SendTransferResponse(pFrame->uSeqNum(), 
                                 CAUSE_OPT_IE_INCRT);
        }
        return;
    }

    //处理所收到的话单数据
    if (bMaybeDup)
    {
        OnGTPMaybeDupData(pFrame);
    }
    else
    {
        OnGTPNormalData(pFrame);
    }
}


/******************************************************
// 函数名:  CGtpStack::OnGTPTestEmptyFrame
// 作者:    Wangfeng
// 时期:    01-10-22
// 描述:    处理测试帧
// 输入:
//       参数1 :SGtpFrame *pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpStack::OnGTPTestEmptyFrame(SGtpFrame *pFrame)
{
    //test empty frame只有两种状况FS_NULL和FS_SAVED，不会有其它情况
    switch (m_SNList.Lookup(pFrame->uSeqNum()))
    {
    //若在历史记录中未发现，则可能是CGF没有收到，此时认为CGF
    //未保存该话单，应应答accept
    case FS_NULL:
        SendTransferResponse(pFrame->uSeqNum());   
        break;

    //若已经保存则应该应答fulfilled
    case FS_SAVED:
        SendTransferResponse(pFrame->uSeqNum(), 
                             CAUSE_REQUEST_DUPFULFILLED);
        break;

    //其它情况为正在等待保存，不应应答
    default:
        break;
    }
}


/******************************************************
// 函数名:  CGtpNetApp::CGtpNetApp
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    构造函数
// 输入:
//       参数1 :CNetWork* const pNetWork
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpNetApp::CGtpNetApp(CNetWork* const pNetWork)
                       :CNetApp(pNetWork)
{
    m_nStackStatusInfoType = STACK_GTP;

    //打开配置文件
    CINIFile *pIniFile;
    pIniFile = new CINIFile((char *)GetCfgFilePath());
    pIniFile->Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);

    //读取新话单队列长度
    m_nMaxNewFrameCount = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_MAX_NEW_FRM_COUNT,
                                      CFG_AP_KEY_MAX_NEW_FRM_COUNT_DEFAULT);

    //读取发送Echo Request帧的时间间隔，单位为秒
    m_nEchoReqInv = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_ECHO_REQ_INV,
                                      CFG_AP_KEY_ECHO_REQ_INV_DEFAULT);
    //ZYS修改问题单：D19464
    if(m_nEchoReqInv < 0 || m_nEchoReqInv > 300)
    {
        m_nEchoReqInv = 60;
    }

    //读取REQUEST消息发送超时时间，单位为秒
    m_nRequestTimeOut = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_REQ_TIMEOUT,
                                      CFG_AP_KEY_REQ_TIMEOUT_DEFAULT);
	
	if (m_nRequestTimeOut !=1 && m_nRequestTimeOut !=2 && m_nRequestTimeOut!=4 && m_nRequestTimeOut !=8)
	{
		m_nRequestTimeOut = CFG_AP_KEY_REQ_TIMEOUT_DEFAULT;
	}

    //读取超时重发次数
    m_nTimeOutRepeat = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_TIMEOUT_RPT,
                                      CFG_AP_KEY_TIMEOUT_RPT_DEFAULT);
	if (m_nTimeOutRepeat < 1 || m_nTimeOutRepeat > 6)
	{
		m_nTimeOutRepeat = CFG_AP_KEY_TIMEOUT_RPT_DEFAULT;
	}

    //读取发送Node Alive帧的时间间隔，单位为秒
    m_nNodeAliveReqInv = pIniFile->GetInt(strSec,
                                      CFG_AP_KEY_NODE_ALIVE_REQ_INV,
                                      CFG_AP_KEY_NODE_ALIVE_REQ_INV_DEFAULT);

    //获取本机地址
    pIniFile->GetString(strSec,
                       CFG_AP_KEY_LOCAL_IP_TO_PM,
                       CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT,
                       m_szLocalAddr,
                       20);

    //获取重定向地址
    pIniFile->GetString(strSec,
                       CFG_AP_KEY_REDIRECTION_ADDR,
                       CFG_AP_KEY_REDIRECTION_ADDR_DEFAULT,
                       m_szRedirectionAddr,
                       20);

    pIniFile->Close();
    delete pIniFile;

    m_pRptBillPerf    = NULL;
    m_pNmlBillPerf    = NULL;
    m_pAllFrmPerf     = NULL;
    m_pGSNConnectPerf = NULL;
    m_pGtpQueueSize   = NULL;
    m_pGtpThroughPut  = NULL;
    m_uCurrentStackID = 0;
	//统计Ga接口平均响应时间
	m_pGtpResTime = 0 ;
}


/******************************************************
// 函数名:  CGtpNetApp::~CGtpNetApp
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpNetApp::~CGtpNetApp()
{
    m_pRptBillPerf    = 0;
    m_pNmlBillPerf    = 0;
    m_pAllFrmPerf     = 0;
    m_pGSNConnectPerf = 0;
}


/******************************************************
// 函数名:  CGtpNetApp::Init
// 作者:    Wangfeng
// 时期:    03-06-27
// 描述:    初始化函数
// 输入:
//       参数1 :void
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//03-06-27      Wangfeng
******************************************************/
int CGtpNetApp::Init(void)
{
    int nRet = CNetApp::Init();
    if(ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    //读取本进程对应的接入点号
    char strSec[20];
    sprintf(strSec, 
            "%s%d", 
            CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);
    /*
        读等效IP地址表	
    */
    //读等效IP地址表的个数，即有几个GSN采用双平面机制
    INT4 nMultiIPGsnCount = IniFile.GetInt(strSec,
                                           CFG_AP_KEY_MULTI_IP_GSN_COUNT,
                                           CFG_AP_KEY_MULTI_IP_GSN_COUNT_DEFAULT);

    UINT4 uIPAddr;
    char szLineContent[1024];
    for(int i = 0; i < nMultiIPGsnCount; i++)
    {
        char szIPAddrSec[50];
        SNPRINTF(szIPAddrSec,
                 sizeof(szIPAddrSec),
                 "%s%d",
                 CFG_AP_KEY_GSN_IP_TABLE,
                 i + 1);
        szIPAddrSec[sizeof(szIPAddrSec) - 1] = '\0';

        IniFile.GetString(strSec,
                          szIPAddrSec,
                          "",
                          szLineContent,
                          sizeof(szLineContent));

        char szGsnName[1024];
        const char *p = strchr(szLineContent, ':');
        if(p != NULL)
        {
            strncpy(szGsnName, 
                    szLineContent, 
                    min(sizeof(szGsnName), (UINT4)p - (UINT4)&szLineContent[0]));
            szGsnName[min(sizeof(szGsnName), (UINT4)p - (UINT4)&szLineContent[0])] = '\0';
            memmove(szLineContent, p + 1, strlen(p + 1) + 1);
        }
        else
        {
            szGsnName[0] = '\0';
        }

        VECTOR<UINT4> ip_table;
        if (szLineContent[0] != '\0')
        {
            char *pToken = strtok(szLineContent, ",");
            while(pToken)
            {
                uIPAddr = inet_addr(pToken);

                if(0 == uIPAddr)
                {
                    TRACE(MTS_NETWORK, S_IP_ADDRESS_ERROR, pToken);
                    return ERR_FAIL;
                }

                if(m_GsnMultiIPMap.find(uIPAddr) != m_GsnMultiIPMap.end())
                {
                    TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
                    return ERR_FAIL;
                }
                
                bool bFind = false;
                int j ;
                for(j = 0; j < ip_table.size(); j++)
                {
                    if(uIPAddr == ip_table[j])
                    {
                        bFind = true;
                        break;
                    }
                }
                if(bFind)
                {
                    TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
                    return ERR_FAIL;
                }
                ip_table.push_back(uIPAddr);

                pToken = strtok(NULL, ",");
            }

            if(ip_table.size() > GSN_IP_TABLE_IPCOUNT_MAX)
            {
                TRACE(MTS_NETWORK, 
                      S_GSN_IP_TOO_MANY,
                      szIPAddrSec,
                      GSN_IP_TABLE_IPCOUNT_MAX);
                return ERR_FAIL;
            }
            else if(ip_table.size() > 0)
            {
                m_GsnMultiIPTable.push_back(ip_table);
            }

            int j ;
            for(j = 0; j < ip_table.size(); j++)
            {
                uIPAddr = ip_table[j];
                m_GsnMultiIPMap[uIPAddr] = i;
                if(szGsnName[0] != '\0')
                {
                    m_IPToStackName[uIPAddr] = szGsnName;
                }
            }
        }
    }

    /*
        读待删除的IP地址表
    */
    IniFile.GetString(strSec,
                      CFG_AP_KEY_DEL_GSN_IP,
                      CFG_AP_KEY_DEL_GSN_IP_DEFAULT,
                      szLineContent,
                      sizeof(szLineContent));
    char *pToken = strtok(szLineContent, ",");
    while(pToken)
    {
        uIPAddr = inet_addr(pToken);

        if(0 == uIPAddr)
        {
            TRACE(MTS_NETWORK, S_IP_ADDRESS_ERROR, pToken);
            return ERR_FAIL;
        }

        bool bFind = false;
        int j ;
        for(j = 0; j < m_DelGsnIPTable.size(); j++)
        {
            if(uIPAddr == m_DelGsnIPTable[j])
            {
                bFind = true;
                break;
            }
        }
        if(bFind)
        {
            TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
            return ERR_FAIL;
        }

        if(m_GsnMultiIPMap.find(uIPAddr) != m_GsnMultiIPMap.end())
        {
            TRACE(MTS_NETWORK, S_REDUP_IP_ADDRESS, pToken);
            return ERR_FAIL;
        }
        m_DelGsnIPTable.push_back(uIPAddr);
    }
    
    //统计GTP'重复话单数据帧数(GTP_REPEATDATA_FRAME)
    BYTE yAPType = (BYTE)CMsgProcessor::GetProcessType() - PT_AP_BASE + 1;
    m_pRptBillPerf = CPerfItem::GetItem(yAPType, 
                                        MT_NETWORK,
                                        GTP_REPEATDATA_FRAME);

    //统计GTP'正常话单数据帧数(GTP_DATA_FRAME)
    m_pNmlBillPerf = CPerfItem::GetItem(yAPType, 
                                        MT_NETWORK,
                                        GTP_NORMALDATA_FRAME);

    //统计GTP'所有话单数据帧数(GTP_ALL_FRAME)
    m_pAllFrmPerf = CPerfItem::GetItem(yAPType, 
                                       MT_NETWORK,
                                       GTP_ALL_FRAME);

    //统计与所有GSN的连接数(GTP_ALLGSN_CONNECT)
    m_pGSNConnectPerf = CPerfItem::GetItem(yAPType, 
                                           MT_NETWORK,
                                           GTP_ALLGSN_CONNECT);
    //统计GTP'的队列长度(PERF_GTP_QUEUE_SIZE)
    m_pGtpQueueSize = CPerfItem::GetItem(yAPType, 
                                         MT_NETWORK,
                                         PERF_GTP_QUEUE_SIZE,
                                         PERF_STATS_TOTAL_VALUE);


    //统计GTP'帧的吞吐量(PERF_GTP_THROUGHPUT)
    m_pGtpThroughPut = CPerfItem::GetItem(yAPType, 
                                          MT_NETWORK,
                                          PERF_GTP_THROUGHPUT);

     //统计Ga接口平均响应时间
	m_pGtpResTime = CPerfItem::GetItem(yAPType,
										MT_NETWORK,
										PERF_GTP_RESTIME);
    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CGtpNetApp::Active
// 作者:    Wangfeng
// 时期:    01-10-29
// 描述:    激活GTP网络应用层
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-29      Wangfeng
******************************************************/
void CGtpNetApp::Active(void)
{
    if(!m_bActive)
    {
        //依次调用每个CGtpStack对象的SendNodeAliveRequest函数
        //及SendEchoRequest函数
        MAP_ID_TO_PS::iterator i;
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            UINT4 uLocalAddr = ACE_OS::inet_addr(m_szLocalAddr);
            ((CGtpStack*)(*i).second)->SendNodeAliveRequest(uLocalAddr);
            
			//D19464 Z30971删除激活时发送EchoRequest消息
			// ((CGtpStack*)(*i).second)->SendEchoRequest();
        }

        CNetApp::Active();

        m_pGSNConnectPerf->Set(m_StackMan.size());
    }
}


/******************************************************
// 函数名: 	CGtpNetApp::DeActive
// 作者:    Wangfeng
// 时期:    02-2-4
// 描述:    网络层去激活
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//02-2-4        Wangfeng
******************************************************/
void CGtpNetApp::DeActive(void)
{
    if(m_bActive)
    {
        //发送重定向消息
        SendRedirection();

        CNetApp::DeActive();
    }
}

/******************************************************
// 函数名:  CGtpNetApp::SendRedirection
// 作者:    Wangfeng
// 时期:    01-11-1
// 描述:    向所有GSN发重定向信息
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-11-1       Wangfeng
******************************************************/
void CGtpNetApp::SendRedirection(void)
{
    BYTE  yCause = CAUSE_SYSTEM_FAILURE;
    if(CNetWork::NWSTATUS_AUXUPGRADE == m_pNetWork->GetCurStatus())
    {
        yCause = CAUSE_OWN_GODOWN;
    }
    
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        ((CGtpStack*)(*i).second)->SendRedirectionRequest(m_szRedirectionAddr, yCause);
    }
}

int CGtpNetApp::SendFrame2Ex(MSG_INTRA* pMsg, CProtocolStack* pStack)
{
    m_pGtpThroughPut->Add(pMsg->Len);

    return CNetApp::SendFrame2Ex(pMsg, pStack);
}

/******************************************************
// 函数名:  CGtpNetApp::LoadStatus
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    从主状态文件中读取所有协议栈的状态信息
// 输入:
//       参数1 :void
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CGtpNetApp::LoadStatus(void)
{
    //调用CNetStatusInfo::EnumStackId函数，获得所有协议栈的编号

    //为每个编号创建一个CGtpStack对象
    //将映射关系<编号, 对应的CGtpStack对象的指针>加入到m_StackMan中；
    //依次调用每个CGtpStack对象LoadStatus函数
    int nNum = 0;
    UINT4 uStackId;
    int i, j, k;
    while((uStackId = CNetStatusInfo::EnumStackId(nNum++, STACK_GTP)) 
          != INVALID_STACK_ID)
    {
        if(m_uCurrentStackID < uStackId)
        {
            m_uCurrentStackID = uStackId;
        }

        if(m_StackMan.find(uStackId) == m_StackMan.end())
        {
            //创建一个GTP协议栈对象
            CGtpStack* pGtpStack = new CGtpStack(this, uStackId);
            if(!pGtpStack) return ERR_FAIL;

            //装入该协议栈的状态文件
            if(pGtpStack->LoadStatus() != ERR_SUCCESS)
            {
                delete pGtpStack;
                TRACE(MTS_NETWORK, S_NET_LOAD_GTP_STACK, uStackId);
                return ERR_FAIL;
            }
            
            VECTOR<UINT4> out_ip_table;
            pGtpStack->GetGsnIPTable(out_ip_table);

            /*
                如果是单IP地址的GSN,且IP地址存在于m_DelGsnIPTable中,则删除该GSN的协议栈
            */
            if(out_ip_table.size() == 1)
            {
                BOOL bFound = FALSE;
                VECTOR<UINT4>::iterator iter = m_DelGsnIPTable.begin();
                for(; iter != m_DelGsnIPTable.end(); iter++)
                {
                    if(*iter == out_ip_table[0])
                    {
                        m_DelGsnIPTable.erase(iter);
                        bFound = TRUE;
                        break;
                    }
                }

                if(bFound)
                {
                    out_ip_table.clear();
                }
            }
            /*
                如果是多IP地址的GSN,且其中没有一个IP地址存在于
                配置的IP地址表中,则删除该GSN的协议栈
            */
            else if(out_ip_table.size() > 1)
            {
                BOOL bFound = FALSE;
                for(j = 0; j < out_ip_table.size(); j++)
                {
                    UINT4 uIPAddr = out_ip_table[j];
                    if(m_GsnMultiIPMap.find(uIPAddr) != m_GsnMultiIPMap.end())
                    {
                        bFound = TRUE;
                        break;
                    }
                }

                if(!bFound)
                {
                    out_ip_table.clear();
                }
            }

            //删除IP地址表为空的协议栈对象
            if(out_ip_table.size() == 0)
            {
                char szMsg[100];
                sprintf(szMsg, 
                        S_NET_REMOVE_GTP_STACK,
                        pGtpStack->GetStackId());
                MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
                TRACE(MTS_NETWORK, szMsg);
                CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);

                pGtpStack->m_bDeleteFileOnDestroy = TRUE;
                delete pGtpStack;
                pGtpStack = NULL;
            }

            if(NULL != pGtpStack)
            {
                m_StackMan[uStackId] = pGtpStack;
                for(j = 0; j < out_ip_table.size(); j++)
                {
                    UINT4 uIPAddr = out_ip_table[j];
                    if(m_IPToStackMap.find(uIPAddr) == m_IPToStackMap.end())
                    {
                        m_IPToStackMap[uIPAddr] = pGtpStack;
                    }
                    else
                    {
                        //说明多个GSN共用同一个IP，这是设计错误
                        ASSERT(0);
                    }
                }
            }
        }
        else
        //说明状态文件中存在两个ID相同的协议栈，这是编程错误
        {
            ASSERT(0);
        }
    }
    
    //读出序列号列表
    int nRet = LoadPacketSNStatus();
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }
    
    /*
     	此时m_uCurrentStackID能保证是在所有状态文件中记录的最大协议栈号
        顺序遍历m_GsnMultiIPTable中各个GSN的IP地址表（以下称T1），
        对于每一个GSN的IP地址表（以下称T2），在各个协议栈对象查找第一个
        IP地址存在于T1的协议栈对象，如果能找到，则用T1来更新该协议栈的
        IP地址表即T2，并将后续协议栈存在于T1的IP地址删除掉，如果某协议栈
        的IP地址被全部删除，则删除该协议栈
    */
    for(i = 0; i < m_GsnMultiIPTable.size(); i++)
    {
        VECTOR<UINT4>& ip_table = m_GsnMultiIPTable[i];

        UINT4 uIPAddr = 0;
        CGtpStack* pGtpStack = NULL;
        for (j = 0; j < ip_table.size(); j++)
        {
            uIPAddr = ip_table[j];
            if(m_IPToStackMap.find(uIPAddr) != m_IPToStackMap.end())
            {
                pGtpStack = m_IPToStackMap[uIPAddr];
                break;
            }
        }

        if(NULL != pGtpStack)
        //找到了第i个IP地址表所对应的协议栈
        {
            //更新IP地址存在于ip_table中的GSN的IP地址表
            for (j = 0; j < ip_table.size(); j++)
            {
                UINT4 uIPAddr_k = ip_table[j];
                if(m_IPToStackMap.find(uIPAddr_k) != m_IPToStackMap.end())
                {
                    CGtpStack* pGtpStack_k = m_IPToStackMap[uIPAddr_k];
                    pGtpStack_k->DelGsnIp(uIPAddr_k);
                    m_IPToStackMap.erase(uIPAddr_k);
                }
            }

            //清除原协议栈IP地址在m_IPToStackMap中的映射关系
            VECTOR<UINT4> out_ip_table;
            pGtpStack->GetGsnIPTable(out_ip_table);
            for (k = 0; k < out_ip_table.size(); k++)
            {
                UINT4 uIPAddr_k = out_ip_table[k];
                if(m_IPToStackMap.find(uIPAddr_k) != m_IPToStackMap.end())
                {
                    m_IPToStackMap.erase(uIPAddr_k);
                }
            }

            pGtpStack->UpdateGsnIPTable(ip_table);
            for(j = 0; j < ip_table.size(); j++)
            {
                UINT4 uIPAddr = ip_table[j];
                if(m_IPToStackMap.find(uIPAddr) == m_IPToStackMap.end())
                {
                    m_IPToStackMap[uIPAddr] = pGtpStack;
                }
                else
                {
                    //说明多个GSN共用同一个IP，这是设计错误
                    ASSERT(0);
                }
            }
        }
    }

    MAP_ID_TO_PS::iterator iter = m_StackMan.begin();
    for (; iter != m_StackMan.end(); iter++)
    {
        CGtpStack* pGtpStack = (CGtpStack*)iter->second;
        VECTOR<UINT4> out_ip_table;
        pGtpStack->GetGsnIPTable(out_ip_table);

        //当用户将该GSN的IP地址删除时，程序也将该协议栈删除
        if(out_ip_table.size() == 0)
        {
            char szMsg[100];
            sprintf(szMsg, 
                    S_NET_REMOVE_GTP_STACK,
                    pGtpStack->GetStackId());
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
            TRACE(MTS_NETWORK, szMsg);
            CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);

            pGtpStack->m_bDeleteFileOnDestroy = TRUE;
            delete pGtpStack;

            m_StackMan.erase(iter);
            iter = m_StackMan.begin();
        }
        else
        {
            if(m_IPToStackName.find(out_ip_table[0]) != m_IPToStackName.end())
            {
                pGtpStack->SetGsnName(m_IPToStackName[out_ip_table[0]].c_str());
            }
        }
    }

    return nRet;
}


/******************************************************
// 函数名:  CGtpNetApp::SaveStatus
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    向主状态文件中写入所有协议栈的状态信息
// 输入:
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CGtpNetApp::SaveStatus(void)
{
    int nRet = ERR_SUCCESS;
    
    //先保存序列号列表文件
    if(GetNetWork()->IsWaitForResponse())
    {
        nRet = SavePacketSNStatus();
        if (nRet != ERR_SUCCESS) return nRet;
    }

    //依次调用每个CGtpStack对象的SaveStatus函数；
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        nRet = (*i).second->SaveStatus();
        if (nRet != ERR_SUCCESS)
        {
            TRACE(MTS_NETWORK, S_NET_SAVE_GTP_STATUS);
            return nRet;
        }
    }

    //清空待删除IP地址表的配置
    if(m_DelGsnIPTable.size() == 0)
    {
        CINIFile IniFile(GetCfgFilePath());
        IniFile.Open();

        //读取本进程对应的接入点号
        char strSec[20];
        sprintf(strSec, 
                "%s%d", 
                CFG_SEC_AP, 
                CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);

        char szLineContent[1024];
        IniFile.GetString(strSec,
                          CFG_AP_KEY_DEL_GSN_IP,
                          CFG_AP_KEY_DEL_GSN_IP_DEFAULT,
                          szLineContent,
                          sizeof(szLineContent));
        StringLRTrim(szLineContent);
        if(szLineContent[0] != '\0')
        {
            IniFile.WriteString(strSec,
                                CFG_AP_KEY_DEL_GSN_IP,
                                CFG_AP_KEY_DEL_GSN_IP_DEFAULT);
        }
    }

    m_pGSNConnectPerf->Set(m_StackMan.size());
    
    return nRet;
}


/******************************************************
// 函数名:  CGtpNetApp::LoadPacketSNStatus
// 作者:    Wangfeng
// 时期:    01-10-26
// 描述:    搜索工作目录下的所有状态文件并读入其内容
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
int CGtpNetApp::LoadPacketSNStatus()
{
    LIST <UINT4> files;
    UINT4 uFileNumber;
    int nRet = ERR_SUCCESS;
    char szPath[MAX_PATH];

    //搜索并建立历史包号记录文件列表
    ACE_DIR *pdir = ACE_OS::opendir(CNetWork::m_szWorkDir);
    if(NULL == pdir) return ERR_FAIL;

    dirent *pdirent;
    while(pdirent = ACE_OS::readdir(pdir))
    {
        const char *szFileName = pdirent->d_name;
        if(strstr(szFileName, ".sn"))
        //如果文件名中存在字符串".sn"
        {
            //跳过目录文件
            sprintf(szPath, 
                    "%s%s", 
                    CNetWork::m_szWorkDir, 
                    szFileName);
            ACE_stat subStat;
            ACE_OS::stat(szPath, &subStat);
            if(subStat.st_mode & S_IFDIR)
            {
                continue;
            }

            //根据文件名取序列号
            char *pFileName = new char[strlen(szFileName) + 1];
            strcpy(pFileName, szFileName);
            if (char *pDot = strchr(pFileName, '.')) 
            {
                *pDot = '\0';
            }
            uFileNumber = atoi(pFileName);
            delete [] pFileName;

            //若文件名不为数字或为零，则由atoi返回的值为0，不
            //为数字说明该文件不是状态文件，不应装入；若为0.sn，
            //由于正常包号从1开始，故该文件也不必装入
            if ((uFileNumber > 0) && 
                (uFileNumber <= m_pNetWork->GetPacketId()))
            {
                files.push_back(uFileNumber);
            }
            else
            {
                remove(szPath);
            }
        }
    }
    ACE_OS::closedir(pdir);

    //将包号从小到大排序
    if (files.size() > 0) files.sort();

    //装入记录
    CGtpStack *pTempStack = new CGtpStack(this, INVALID_STACK_ID);
    pTempStack->m_bDeleteFileOnDestroy = TRUE;
    LIST <UINT4>::iterator i = files.begin();
    while ((i != files.end()) && (nRet == ERR_SUCCESS))
    {

        //获取文件名
        uFileNumber = *i;
        i++;

        sprintf(szPath, 
                "%s%d.sn", 
                CNetWork::m_szWorkDir, 
                uFileNumber);

        //打开并读入文件
        FILE* file = fopen(szPath, "rb");
        if (!file)
        //打开文件失败
        {
            TRACE(MTS_NETWORK,
                   S_NET_OPEN_SEQ_FILE_ERR, 
                   szPath, errno, strerror(errno));
            MSGOUT(MTS_NETWORK,
                   MSGOUT_LEVEL_URGENT,
                   S_NET_OPEN_SEQ_FILE_ERR, 
                   szPath, errno, strerror(errno));
            continue;
        }

        UINT4 uStackId;
        SGtpSNFile snFile;
        snFile.pStatusFile = file;
        snFile.pFileRef = new CGtpSNFileRef(uFileNumber);

        //状态文件中可能存在已经被删除掉的协议栈的状态。读
        //入状态时必须跳过这些数据。这里创建一临时协议栈用
        //于读入这些不再需要的状态数据。
        pTempStack->Clear(TRUE);
        while (1 == fread(&uStackId, 
               sizeof(uStackId), 1, 
               snFile.pStatusFile))
        {
            if(m_uCurrentStackID < uStackId)
            {
                m_uCurrentStackID = uStackId;
            }

            if(m_StackMan.find(uStackId) != m_StackMan.end())
            {
                nRet = ((CGtpStack *)m_StackMan[uStackId])->
                                     LoadSNList(&snFile);
            }
            else
            {
                nRet = pTempStack->LoadSNList(&snFile);
            }
            if (nRet != ERR_SUCCESS)
            {
                TRACE(MTS_NETWORK, S_NET_READ_SEQ_FILE, uFileNumber);
                MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_URGENT,
                       S_NET_READ_SEQ_FILE, uFileNumber);
                break;
            }
        }

        fclose(snFile.pStatusFile); 

        //如果该文件中的包被所有协议栈都DISCARD，则文件在此处删除
        snFile.pFileRef->Release();
    }
    delete pTempStack;

    return ERR_SUCCESS;
}


/******************************************************
// 函数名:  CGtpNetApp::SavePacketSNStatus
// 作者:    Wangfeng
// 时期:    01-10-26
// 描述:    将当前包中各协议栈的序列号列表保存在以当前
//          帧号为名的文件中。该函数在每次数据发送之后和
//          收到应答之后被调用。由于仅需要保存已发送(提交)
//          的序列号，而已发送序列号列表在每次发送时形成，
//          应答时清空，因此只在发送之后才真正保存
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
int CGtpNetApp::SavePacketSNStatus()
{
    FILE* file;
    char szPath[MAX_PATH];
    int nRet = ERR_SUCCESS;

    //打开状态文件，不存在则创建
    sprintf(szPath, "%s%u.sn", 
            CNetWork::m_szWorkDir, 
            m_pNetWork->GetPacketId());
    file = fopen(szPath, "wb");
    if (!file)
    //打开文件失败
    {
        TRACE(MTS_NETWORK, S_NET_CREATE_SEQ_FILE, 
              szPath, errno, strerror(errno));
        return ERR_FAIL;
    }

    SGtpSNFile snFile;
    snFile.pStatusFile = file;
    snFile.pFileRef = new CGtpSNFileRef(m_pNetWork->GetPacketId());

    //依次保存所有所有协议栈的ID和调用其SaveSNList函数
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        //写一个协议栈的序列号列表信息
        CGtpStack *pStack = (CGtpStack *)((*i).second);
        UINT4 uStackId = (*i).first;
        if((fwrite(&uStackId, sizeof(uStackId), 1, file) != 1) || 
           (pStack->SaveSNList(&snFile) != ERR_SUCCESS))
        {
            nRet = ERR_FAIL;
            break;
        }
    }
    fflush(file);
    fclose(file);
    snFile.pFileRef->Release();

    //如果写序列号列表文件失败，则删除该文件
    if(nRet != ERR_SUCCESS)
    {
        remove(szPath);
    }

    return nRet;
}

void CGtpNetApp::Timer1Sec(void)
{
    CNetApp::Timer1Sec();

    m_pGtpQueueSize->Set(GetReadyDataSize());
}

/******************************************************
// 函数名:  CGtpNetApp::OnReceive
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    处理来自网络层的协议帧
// 输入:
//       参数1 :SNetFrame* pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CGtpNetApp::OnReceive(SNetFrame* pFrame)
{
    m_pGtpThroughPut->Add(pFrame->Len);

    if (m_bActive)
    {
        //避免创建一个0.0.0.0的协议栈
        UINT4 uClientIp = pFrame->SNetAddrPair.ClientIp;
        if(uClientIp == 0)
        {
            MSGOUT(MTS_NETWORK, 
                   MSGOUT_LEVEL_NORMAL,
                   S_RECV_GSN_IP_0);
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame);
            return;
        }
        
        //在m_IPToStackMap中找到键值为pFrame->SNetAddrPair.ClientIp
        //的元素，调用该元素所指向的CGtpStack对象的OnReceive函数
        if(m_IPToStackMap.find(uClientIp) == m_IPToStackMap.end())
        {
            CGtpStack* pGtpStack = new CGtpStack(this, ++m_uCurrentStackID);
            if(NULL == pGtpStack)
            {
                m_uCurrentStackID--;
                m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_FAIL);
                return;
            }

            VECTOR<UINT4> ip_table;
            if(m_GsnMultiIPMap.find(uClientIp) != m_GsnMultiIPMap.end())
            {
                int nGsnIndex = m_GsnMultiIPMap[uClientIp];
                if(nGsnIndex < m_GsnMultiIPTable.size())
                {
                    ip_table = m_GsnMultiIPTable[nGsnIndex];
                }
                else
                {
                    //设计错误
                    m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_FAIL);
                    ASSERT(0);
                    return;
                }
            }
            else
            {
                ip_table.push_back(uClientIp);
            }

            for(int i = 0; i < ip_table.size(); i++)
            {
                m_IPToStackMap[ip_table[i]] = pGtpStack;
            }
            if(m_IPToStackMap.find(uClientIp) == m_IPToStackMap.end())
            {
                //设计错误
                m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_FAIL);
                delete pGtpStack;
                m_uCurrentStackID--;
                ASSERT(0);
                return;
            }

            //设置GSN名
            if(m_IPToStackName.find(uClientIp) != m_IPToStackName.end())
            {
                pGtpStack->SetGsnName(m_IPToStackName[uClientIp].c_str());
            }

            pGtpStack->UpdateGsnIPTable(ip_table);
            m_StackMan[m_uCurrentStackID] = pGtpStack;

            char szMsg[100];
            struct in_addr addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
            addr.s_addr = uClientIp;
#else //_PLATFORM_HPUX
            addr.S_un.S_addr = uClientIp;
#endif//_PLATFORM_HPUX
            sprintf(szMsg, S_NET_CREATE_GTP_STACK,
                   ACE_OS::inet_ntoa(addr),
                   pFrame->SNetAddrPair.ClientPort);
            MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
            TRACE (MTS_NETWORK, szMsg);
            CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);

            m_pGSNConnectPerf->Set(m_StackMan.size());
        }

        if((m_Stack_To_RecvInv.size() < m_pNetWork->GetLicenseCount())
            || (m_Stack_To_RecvInv.find((UINT4)m_IPToStackMap[uClientIp]) 
                != m_Stack_To_RecvInv.end()))
        {
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame);
            m_IPToStackMap[uClientIp]->OnReceive(pFrame);
        }
        else
        {
            //模块数超过授权数
            m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_LICENSE_EXPIRED);
        }

		CNetApp::OnReceive(pFrame);
        return;
	}

    m_pNetWork->TrackProtocol((MSG_INTRA*)pFrame, ERR_APP_NOT_ACTIVED);
}


/******************************************************
// 函数名:  CGtpNetApp::GetMaxNewFrameCount
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    返回新收到帧队列最大长度
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
int CGtpNetApp::GetMaxNewFrameCount()
{
    return m_nMaxNewFrameCount;
}


/******************************************************
// 函数名:  CGtpNetApp::GetEchoReqInterval
// 作者:    Wangfeng
// 时期:    01-10-25
// 描述:    返回发送Echo Request帧的时间间隔，单位为分钟
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
int CGtpNetApp::GetEchoReqInv()
{
    return m_nEchoReqInv;
}


/******************************************************
// 函数名:  CGtpNetApp::GetRequestTimeOut
// 作者:    Wangfeng
// 时期:    01-10-25
// 描述:    返回REQUEST消息发送超时时间
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
int CGtpNetApp::GetRequestTimeOut()
{
    return m_nRequestTimeOut;
}


/******************************************************
// 函数名:  CGtpNetApp::GetTimeOutRepeat
// 作者:    Wangfeng
// 时期:    01-10-25
// 描述:    返回超时重发次数
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
int CGtpNetApp::GetTimeOutRepeat()
{
    return m_nTimeOutRepeat;
}


/******************************************************
// 函数名:  CGtpNetApp::GetNodeAliveReqInv
// 作者:    Wangfeng
// 时期:    01-10-25
// 描述:    发送Node Alive Request帧的时间间隔，单位为
//          分钟(这是主机要求的)
// 输入:
// 返回值: int 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
int CGtpNetApp::GetNodeAliveReqInv()
{
    return m_nNodeAliveReqInv;
}


/******************************************************
// 函数名:  CGtpNetApp::GetLocalAddress
// 作者:    Wangfeng
// 时期:    01-10-25
// 描述:    获得本机地址
// 输入:
// 返回值: const char* 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-25      Wangfeng
******************************************************/
const char* CGtpNetApp::GetLocalAddress()
{
    return m_szLocalAddr;
}


/******************************************************
// 函数名:  CGtpTrack::CGtpTrack
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    构造函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpTrack::CGtpTrack()
{
    m_pMmlReport = new CMMLReport("CG");
}


/******************************************************
// 函数名:  CGtpTrack::~CGtpTrack
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CGtpTrack::~CGtpTrack()
{
    if (m_pMmlReport)
    {
        delete m_pMmlReport;
        m_pMmlReport = 0;
    }
}


/******************************************************
// 函数名:  CGtpTrack::Analysis
// 作者:    Wangfeng
// 时期:    01-10-30
// 描述:    实施对GTP'协议帧的分析,由函数内部分配内存，
//          调用者不需要释放
// 输入:
//       参数1 :void* const pFrame--待分析的协议帧
//                        nProcRst--处理结果
// 返回值: const char* -- 返回分析所得的字符串
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-30      Wangfeng
******************************************************/
const char* CGtpTrack::Analysis(void* const pFrame, int nProcRst)
{

    char szBuffer[1024];

    m_pMmlReport->MakeReportHead();
    m_pMmlReport->AddString(S_PROTOCOL_TRACK_HEAD);
    m_pMmlReport->AddString(STR_NEWLINE);

    //将pFrame强制转换成SNetFrame*类型
    SNetFrame* pMsg = (SNetFrame*) pFrame;
    struct in_addr SvrAddr;
    struct in_addr CltAddr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
    SvrAddr.s_addr = pMsg->SNetAddrPair.ServerIp;
    CltAddr.s_addr = pMsg->SNetAddrPair.ClientIp;
#else //_PLATFORM_HPUX
    SvrAddr.S_un.S_addr = pMsg->SNetAddrPair.ServerIp;
    CltAddr.S_un.S_addr = pMsg->SNetAddrPair.ClientIp;
#endif//_PLATFORM_HPUX
    if (MT_NETWORK == pMsg->ReceiverMid)
    //表示是由主机发来的消息
    {
        //发送方
        sprintf(szBuffer, "GSN (%s %s:%d)", CMsgProcessor::g_szPtDes, 
                ACE_OS::inet_ntoa(CltAddr), 
                pMsg->SNetAddrPair.ClientPort);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_SENDER, szBuffer);

        //接收方
        sprintf(szBuffer, "CG (%s:%d)",
                ACE_OS::inet_ntoa(SvrAddr), 
                pMsg->SNetAddrPair.ServerPort);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_RECEIVER, szBuffer);
    }
    else
    //表示是由CG向给主机的消息
    {
        //发送方
        sprintf(szBuffer, "CG (%s %s:%d)", CMsgProcessor::g_szPtDes, 
                ACE_OS::inet_ntoa(SvrAddr), 
                pMsg->SNetAddrPair.ServerPort);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_SENDER, szBuffer);

        //接收方
        sprintf(szBuffer, "GSN (%s:%d)",
                ACE_OS::inet_ntoa(CltAddr), 
                pMsg->SNetAddrPair.ClientPort);
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_RECEIVER, szBuffer);
    }

    //构造序列号段
    SGtpFrame* pGTPFrame = (SGtpFrame *)pMsg->Data;
    sprintf(szBuffer, "%d", pGTPFrame->uSeqNum());
    m_pMmlReport->MakeReportItem2(S_TRACK_SEQ_NUM, szBuffer);

    //如果消息的数据区长度不足一个GTP'的帧头长度
    if(pMsg->Len < 1)
    {
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                      S_UNRECOGNIZED);
        m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                      S_FRAME_NOT_CORRECT);
    }
    //如果版本不支持
    else if (!pGTPFrame->CheckGTPFrame())
    {
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                      S_UNRECOGNIZED);
        m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                      S_VERSION_NOT_SUPPORTED);
    }
    //如果消息的数据区长度不足填写消息类型
    else if(pMsg->Len < 2)
    {
        m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                      S_UNRECOGNIZED);
        m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                      S_FRAME_NOT_CORRECT);
    }
    else
    {
        m_strTrackMsg = "";
        switch (pGTPFrame->yType())
        {
        //回响请求
        case ECHO_REQUEST:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_ECHO_REQUEST);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_ECHO_REQUEST);
                if(nProcRst != ERR_SUCCESS)
                {
                    m_strTrackMsg += GetProcRstExplain(nProcRst);
                    m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                                  m_strTrackMsg.c_str());
                }
            }
            break;

        //回响应答
        case ECHO_RESPONSE:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_ECHO_RESPONSE);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_ECHO_RESPONSE);
                OnGTPEchoResponse(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //数据传输请求
        case DATA_TRANSFER_REQUEST:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_DATA_TRANSFER_REQUEST);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_DATA_TRANSFER_REQUEST);
                OnGTPDataTransferRequest(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //数据传输应答
        case DATA_TRANSFER_RESPONSE:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_DATA_TRANSFER_RESPONSE);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_DATA_TRANSFER_RESPONSE);
                OnGTPDataTransferResponse(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //节点活动应答
        case NODE_ALIVE_RESPONSE:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_NODE_ALIVE_RESPONSE);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_NODE_ALIVE_RESPONSE);
                if(nProcRst != ERR_SUCCESS)
                {
                    m_strTrackMsg += GetProcRstExplain(nProcRst);
                    m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                                  m_strTrackMsg.c_str());
                }
            }
            break;

        //节点活动请求
        case NODE_ALIVE_REQUEST:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_NODE_ALIVE_REQUEST);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_NODE_ALIVE_REQUEST);
                OnGTPNodeAliveRequest(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //重定向申请
        case REDIRECTION_REQUEST:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_REDIRECTION_REQUEST);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_REDIRECTION_REQUEST);
                OnGTPRedirectionRequest(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //重定向应答
        case REDIRECTION_RESPONSE:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_REDIRECTION_RESPONSE);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_REDIRECTION_RESPONSE);
                OnGTPRedirectionResponse(pGTPFrame);
                m_strTrackMsg += GetProcRstExplain(nProcRst);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              m_strTrackMsg.c_str());
            }
            break;

        //版本不支持
        case VERSION_NOT_SUPPORTED:
            if((pMsg->Len < pGTPFrame->GetHeadLen()) || (pMsg->Len != pGTPFrame->GetSize()))
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_VERSION_NOT_SUPPORTED);
                m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                              S_FRAME_NOT_CORRECT);
            }
            else
            {
                m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, 
                                              S_VERSION_NOT_SUPPORTED);
                if(nProcRst != ERR_SUCCESS)
                {
                    m_strTrackMsg += GetProcRstExplain(nProcRst);
                    m_pMmlReport->MakeReportItem2(S_MML_TRACK_CONTENT, 
                                                  m_strTrackMsg.c_str());
                }
            }
            break;

        default:
            sprintf(szBuffer, "%d(%s)",
                    pGTPFrame->yType(),
                    S_UNRECOGNIZED);
            m_pMmlReport->MakeReportItem2(S_TRACK_MSG_TYPE, szBuffer);
            break;
        }
    }

    m_pMmlReport->MakeReportTail();
    return m_pMmlReport->GetReport();
}


/******************************************************
// 函数名:  CGtpTrack::OnGTPCancelData
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    解释取消的数据帧
// 输入:
//       参数1 :SGtpFrame *pFrame--包含可能重复话单的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPCancelData(SGtpFrame *pFrame)
{
    TIESNCanceled *pSNCanceled;
    int nChkRst = CHECK_TLV(pFrame, "TIESNCanceled",
                            TIESN_CANCELED_HEAD_LENGTH,
                            pSNCanceled,
                            TIETRANSFER_CMD_LENGTH);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_OPT_IE_INCRT;
        return;
    }

    char szBuf[20];
    sprintf(szBuf, " = %d;", pSNCanceled->uLen() / 2);
    m_strTrackMsg += S_RECORD_COUNT;
    m_strTrackMsg += szBuf;
}


/******************************************************
// 函数名:  CGtpTrack::OnGTPDataTransferRequest
// 作者:    Wangfeng
// 时期:    01-10-22
// 描述:    解释数据传输请求帧
// 输入:
//       参数1 :SGtpFrame *pFrame 所收到的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-22      Wangfeng
******************************************************/
void CGtpTrack::OnGTPDataTransferRequest(SGtpFrame *pFrame)
{
    //如果TIETransferCommand必选项未填
    if(pFrame->uLen() == 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_MISSING;
        return;
    }
    
    //2002-11-23 李杜芳 对应问题单 D19425
    if((pFrame->yTag() == GTP_V0_TAG1) || 
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        OnGTPSendData(pFrame);
        return;
    }
    //修改完毕
    
    TIETransferCommand *pCommand;

    int nChkRst = CHECK_TV(pFrame, "TIETransferCommand", 
                           TIETRANSFER_CMD_LENGTH,
                           pCommand, 0);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }

    switch (pCommand->yCmd())
    {
    //收到数据帧
    case TC_SENDDATA:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_IE_SEND_DATA;
        m_strTrackMsg += "; ";
        OnGTPSendData(pFrame);
        break;

    //收到可能重复的数据帧
    case TC_SENDDATAMAYBEDUP:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_IE_SEND_DATA_MAYBE_DUP;
        m_strTrackMsg += "; ";
        OnGTPSendData(pFrame);
        break;

    //申明上次送来的可能重复的数据帧有效
    case TC_CANCELDATA:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_IE_CANCEL_DATA;
        m_strTrackMsg += "; ";
        OnGTPCancelData(pFrame);
        break;

    //申明上次送来的可能重复的数据帧有效
    case TC_RELEASEDATA:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_IE_RELEASE_DATA;
        m_strTrackMsg += "; ";
        OnGTPReleaseData(pFrame);
        break;

    default:
        m_strTrackMsg += S_TRACK_ITEM_TYPE;
        m_strTrackMsg += " = ";
        m_strTrackMsg += S_UNRECOGNIZED;
        m_strTrackMsg += ";";
        break;
    }
}


/******************************************************
// 函数名:  CGtpTrack::OnGTPDataTransferResponse
// 作者:    Wangfeng
// 时期:    01-10-30
// 描述:    分析CG向GSN发送的数据传输应答
// 输入:
//       参数1 :SGtpFrame *pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-30      Wangfeng
******************************************************/
void CGtpTrack::OnGTPDataTransferResponse(SGtpFrame *pFrame)
{//收到的pFrame为网络字节序
    
    TIECause *pCause;
    TIESNResponse *pResponse;
    char szBuf[20];

    //分析cause结构
    pCause = (TIECause *)(pFrame->Data());
    m_strTrackMsg += S_REPONSE_CAUSE;
    m_strTrackMsg += " = ";
    m_strTrackMsg += GetResponseCauseDesp(pCause->yCause());
    m_strTrackMsg += "; ";

    //Transfer Reponse结构头
    pResponse = (TIESNResponse *) (pFrame->Data() + TIECAUSE_LENGTH);
    sprintf(szBuf, " = %d;", pResponse->uLen() / 2);
    m_strTrackMsg += S_RECORD_COUNT;
    m_strTrackMsg += szBuf;

    sprintf(szBuf, " = %d;", pResponse->uSeqNums(0));
    m_strTrackMsg += S_FIRST_FRAME_SEQ;
    m_strTrackMsg += szBuf;
}


/******************************************************
// 函数名:  CGtpTrack::GetResponseCauseDesp
// 作者:    Wangfeng
// 时期:    01-10-30
// 描述:    获得应答原因值的描述
// 输入:
//       参数1 :int nCause
// 返回值: const char* 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-30      Wangfeng
******************************************************/
const char* CGtpTrack::GetResponseCauseDesp(int nCause)
{
    switch(nCause)
    {
    case CAUSE_REQUEST_ACCEPTED:
        return S_CAUSE_REQUEST_ACCEPTED;

    case CAUSE_REQUEST_DUPFULFILLED:
        return S_CAUSE_REQUEST_DUPFULFILLED;

    case CAUSE_SYSTEM_FAILURE:
        return S_CAUSE_SYSTEM_FAILURE;

    case CAUSE_TRANS_BUF_FULL:
        return S_CAUSE_TRANS_BUF_FULL;

    case CAUSE_RECEIVE_BUF_FULL:
        return S_CAUSE_RECEIVE_BUF_FULL;

    case CAUSE_ANOTHER_GODOWN:
        return S_CAUSE_ANOTHER_GODOWN;

    case CAUSE_OWN_GODOWN:
        return S_CAUSE_OWN_GODOWN;

    case CAUSE_CDR_DECODE_ERR:
    //177 -- CDR decoding error
        return S_CAUSE_CDR_DECODE_ERR;

    case CAUSE_INVALID_MSG_FMT:
    //193 -- Invalid message format
        return S_CAUSE_INVALID_MSG_FMT;

    case CAUSE_VER_NOT_SUP:
    //198 -- Version not supported
        return S_CAUSE_VER_NOT_SUP;

    case CAUSE_NO_RES_AVAILABLE:
    //199 -- No resources available
        return S_CAUSE_NO_RES_AVAILABLE;

    case CAUSE_SERVICE_NOT_SUP:
    //200 -- Service not supported
        return S_CAUSE_SERVICE_NOT_SUP;

    case CAUSE_MAND_IE_INCRT:
    //201 -- Mandatory IE incorrect
        return S_CAUSE_MAND_IE_INCRT;

    case CAUSE_MAND_IE_MISSING:
    //202 -- Mandatory IE missing
        return S_CAUSE_MAND_IE_MISSING;

    case CAUSE_OPT_IE_INCRT:
    //203 -- Optional IE incorrect
        return S_CAUSE_OPT_IE_INCRT;

    case CAUSE_SYSTEM_FAILURE1:
    //204 -- System failure
        return S_CAUSE_SYSTEM_FAILURE;

    case CAUSE_REQUEST_FULFILLED:
    //253 -- Request already fulfilled
        return S_CAUSE_REQUEST_FULFILLED;

    case CAUSE_REQ_NOT_FULFILL:
    //255 -- Request not fulfilled
        return S_CAUSE_REQ_NOT_FULFILL;
    
    //by ldf 2002.4.17对应问题单D12742
    default:
        {
             break;
        }
    //修改完毕

    }

    return S_CAUSE_UNRECOGNIZED;
}


/******************************************************
// 函数名:  CGtpTrack::OnGTPEchoResponse
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    记录重启次数
// 输入:
//       参数1 :SGtpFrame *pFrame -- 所收到的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPEchoResponse(SGtpFrame *pFrame)
{
    TIERecovery *pRecovery;

    int nChkRst = CHECK_TV(pFrame, "TIERecovery", 
                           TIERECOVERY_LENGTH,
                           pRecovery, 0);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }

    char szBuf[20];
    sprintf(szBuf, " = %d;", pRecovery->yRestartCount());
    m_strTrackMsg += S_TRACK_RESTART_COUNT;
    m_strTrackMsg += szBuf;
}


/******************************************************
// 函数名:  CGtpTrack::OnGTPNodeAliveRequest
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    分析Node Alive Request帧
// 输入:
//       参数1 :SGtpFrame *pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPNodeAliveRequest(SGtpFrame *pFrame)
{
    //如果IP地址形式为ipv6，则返回Versoin Not Supported消息
    TIENodeAddress* pIpAddress = (TIENodeAddress* )(pFrame->Data());

    char szBuf[100];
    if (pFrame->uLen() < TIENODE_ADDRESS_LENGTH)
    {
        m_strTrackMsg += S_FRAME_NOT_CORRECT;
        return;
    }
    else if (pIpAddress->uLen() != 4)
    {
        sprintf(szBuf, " = %s;", S_IP_ADDRESS_V6);
    }
    else
    {
        struct in_addr addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        addr.s_addr = pIpAddress->uAddress();
#else //_PLATFORM_HPUX
        addr.S_un.S_addr = pIpAddress->uAddress();
#endif//_PLATFORM_HPUX
        sprintf(szBuf, " = %s;", ACE_OS::inet_ntoa(addr));
    }
    m_strTrackMsg += S_NODE_ALIVE_ADDRESS;
    m_strTrackMsg += szBuf;
}


/******************************************************
// 函数名:  CGtpTrack::OnGTPReleaseData
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    分析释放的数据帧
// 输入:
//       参数1 :SGtpFrame *pFrame -- 包含可能重复话单的帧
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPReleaseData(SGtpFrame *pFrame)
{
    TIESNReleased *pSNReleased;

    int nChkRst = CHECK_TLV(pFrame, "TIESNReleased", 
                            TIESN_RELEASED_HEAD_LENGTH,
                            pSNReleased,
                            TIETRANSFER_CMD_LENGTH);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_OPT_IE_INCRT;
        return;
    }

    char szBuf[20];
    sprintf(szBuf, " = %d;", pSNReleased->uLen() / 2);
    m_strTrackMsg += S_RECORD_COUNT;
    m_strTrackMsg += szBuf;
}


/******************************************************
// 函数名:  CGtpTrack::OnGTPSendData
// 作者:    Wangfeng
// 时期:    01-10-23
// 描述:    分析话单数据接收帧
// 输入:
//       参数1 :SGtpFrame *pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-23      Wangfeng
******************************************************/
void CGtpTrack::OnGTPSendData(SGtpFrame *pFrame)
{
    TIEBillPackage *pPackage;

    //2002-11-23 李杜芳 对应问题单 D19425
    if((pFrame->yTag() == GTP_V0_TAG1) || 
       (pFrame->yTag() == GTP_V06_TAG1))
    {
        int nChkRst = CHECK_TLV(pFrame, "TIEBillPackage",
                                TIEBILL_BACKAGE_HEAD_LENGTH,
                                pPackage, 0);
        if(nChkRst != 0)
        {
            m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
            return;
        }
    }
    else
    {
        int nChkRst = CHECK_TLV(pFrame, "TIEBillPackage",
                                TIEBILL_BACKAGE_HEAD_LENGTH,
                                pPackage, 
                                TIETRANSFER_CMD_LENGTH);
        if(nChkRst != 0)
        {
            m_strTrackMsg += S_CAUSE_OPT_IE_INCRT;
            return;
        }
    }
    //修改完毕

    char szBuf[20];
    sprintf(szBuf, " = %d; ", pPackage->yCount());
    m_strTrackMsg += S_RECORD_COUNT;
    m_strTrackMsg += szBuf;

    sprintf(szBuf, " = %d;", DATASIZE_OF_IEBILLPACKAGE(pPackage));
    m_strTrackMsg += S_RECORD_SIZE;
    m_strTrackMsg += szBuf;
}


/******************************************************
// 函数名:  CGtpTrack::OnGTPRedirectionRequest
// 作者:    Wangfeng
// 时期:    01-10-31
// 描述:    分析重定向申请
// 输入:
//       参数1 :SGtpFrame *pFrame
// 返回值: void 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-31      Wangfeng
******************************************************/
void CGtpTrack::OnGTPRedirectionRequest(SGtpFrame *pFrame)
{
    TIECause* pCause;
    int nChkRst = CHECK_TV(pFrame, "TIECause",
                           TIECAUSE_LENGTH,
                           pCause, 0);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }
    //填写TIECause结构
    m_strTrackMsg += S_REDIRECTION_CAUSE;
    m_strTrackMsg += " = ";
    m_strTrackMsg += GetResponseCauseDesp(pCause->yCause());
    m_strTrackMsg += "; ";

    //看看是否是没有TIERecommendNodeAddress域
    if(pFrame->uLen() == TIECAUSE_LENGTH)
    {
        return;
    }

    TIERecommendNodeAddress* pNodeAddress;
    nChkRst = CHECK_TLV(pFrame, "TIERecommendNodeAddress",
                        IETLV_HEAD_LENGTH,
                        pNodeAddress, TIECAUSE_LENGTH);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }

    char szBuf[100];
    if (pNodeAddress->uLen() != 4)
    {
        sprintf(szBuf, " = %s;", S_IP_ADDRESS_V6);
    }
    else
    {
        struct in_addr addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        addr.s_addr = pNodeAddress->uAddress();
#else //_PLATFORM_HPUX
        addr.S_un.S_addr = pNodeAddress->uAddress();
#endif//_PLATFORM_HPUX
        sprintf(szBuf, " = %s;", ACE_OS::inet_ntoa(addr));
    }
    m_strTrackMsg += S_REDIRECTION_ADDRESS;
    m_strTrackMsg += szBuf;
}

void CGtpTrack::OnGTPRedirectionResponse(SGtpFrame *pFrame)
{
    TIECause* pCause;
    int nChkRst = CHECK_TV(pFrame, "TIECause",
                           TIECAUSE_LENGTH,
                           pCause, 0);
    if(nChkRst != 0)
    {
        m_strTrackMsg += S_CAUSE_MAND_IE_INCRT;
        return;
    }

    m_strTrackMsg    = S_REDIRECTION_CAUSE;
    m_strTrackMsg   += " = ";
    m_strTrackMsg   += GetResponseCauseDesp(pCause->yCause());
}
