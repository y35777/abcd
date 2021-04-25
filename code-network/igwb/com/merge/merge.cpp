/*****************************************************
*       Merge.cpp
******************************************************
*Copyright 2001 by Huawei CO.,LTD.
* All rights reserved.
* Compiler: Visual C++ 6.0
* Author:   Zhou Yonghui
* Date:     01-5-24
* Description:  
* 
* Side Effects: NONE
* Functions:
* Notes:    
* 
* Update: 01-5-24 下午 05:39:16
* Date          Name          Description 
============== ================ ======================
## 01-5-24   Zhou Yonghui
*****************************************************/
#pragma warning(disable:4239)
#include "../include/base_type.h"
#include "../include/export_dll.h"
#include "merge.h"
#include "../toolbase/frame_tool.h"

/*
    定义默认情况下,哈希表的大小、Qw队列的最大长度、
    话单最大等待时间.
*/
#define DEFAULT_HASH_TABLE_LENGTH       100000
#define DEFAULT_MAX_QW_LENGTH           100000
#define DEFAULT_MAX_WAITTING_TIME       10800

CMerger::CMerger()
{
    m_uHashSize = DEFAULT_HASH_TABLE_LENGTH;
    m_uMaxQwLen = DEFAULT_MAX_QW_LENGTH;
    m_uM        = DEFAULT_MAX_WAITTING_TIME;
}

CMerger::~CMerger()
{
    CMergeDelegation* pDelegation = NULL;
    while(m_Delegation.GetSize() > 0)
    {
        pDelegation = m_Delegation.GetHead();
        m_Delegation.RemoveHead();
        delete pDelegation;
    }
}


/******************************************************
* 函数名:   CMerger::MergeABill
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    合并一张话单。
*       CMerger对象根据话单类型，把话单委托给恰当的合并对象。
* 输入: 
*   参数1 :CBill* pBill，话单对象的指针。
*   参数2 :PackID_t PackNum，话单的包号。
* 返回值: 无。 
* 说明:     若内存操作失败强抛出异常。
* 更新记录: 
*
* 日期        姓名          描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::MergeABill(CBill* pBill,PackID_t PackNum)
{
    assert(pBill != NULL);

    //非部分话单和热计费话单
    if(!pBill->IsPartial() || pBill->IsHotBilling()) 
    {
        MoveToQok(pBill);
        return;
    }

    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;
    UINT4 uType = pBill->GetBillType();

    while(pos != NULL)
    {
        //取pos位置的值，并把pos指向下一个元素。
        pDelegation = m_Delegation.GetNext(pos);
        if(uType == pDelegation->GetBillType())
        {
            pDelegation->MergeABill(pBill,PackNum);
            return;
        }
    }

    //话单类型没有注册。
    //assert(0);
}

/******************************************************
* 函数名:   CMerger::MoveToQok
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    把一条话单送Qok队列。
* 输入: 
*   参数1 :CBill* pBill,话单对象的指针。
* 返回值: void 
* 说明:     若内存操作失败强抛出异常。
* 更新记录: 
*
* 日期        姓名          描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::MoveToQok(CBill* pBill)
{
    m_Qok.AddTail(pBill);
}


/******************************************************
* 函数名:   RegisterBillType
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    向CMerger注册话单类型。
*      CMerger对象将对每一种话单生成一个合并对象。
* 输入: 
*   参数1 :int uTypeID，话单类型值。
* 返回值: 无。
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期        姓名          描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::RegisterBillType(UINT4 uTypeID)
{
    //生成一个合并对象。
    CMergeDelegation* pDelegation = new CMergeDelegation(this,uTypeID);
    if(!pDelegation)
    {
        throw "RegisterBillType";
    }

    pDelegation->SetM(m_uM);
    pDelegation->SetMaxQwLength(m_uMaxQwLen);
    pDelegation->Init(m_uHashSize);

    //把合并对象追加到队列的尾部。
    m_Delegation.AddTail(pDelegation);
}


/******************************************************
* 函数名:   CMerger::Flush
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    把所有Qw中的所有话单送Qok队列。
* 输入:     无。
* 返回值:   无。
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期        姓名          描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::Flush()
{
    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        pDelegation = m_Delegation.GetNext(pos);
        pDelegation->Flush();
    }
}


/******************************************************
* 函数名:   CMerger::GenerateBlackBill
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    生成黑话单。
*      把所有Qw队列中所有包号与lPackNum的差大于m的话单送Qok。
*      把所有长度超过预定值的Qw尾部部分话单送Qok。
* 输入: 
*   参数1 :PackID_t PackNum，当前话单包号。
* 返回值: 无。
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期        姓名          描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::GenerateBlackBill(PackID_t PackNum)
{
    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        //取pos位置的值，并把pos指向下一个元素。
        pDelegation = m_Delegation.GetNext(pos);
        pDelegation->GenerateBlackBill(PackNum);
    }
}


/******************************************************
* 函数名:  CMerger::SetMaxQwLength
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    设置Qw队列的最大长度。
* 输入: 
*   参数1 :long uLen，Qw队列的最大允许长度。
* 返回值: 无。
* 说明: 
* 更新记录: 
*
* 日期        姓名          描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::SetMaxQwLength(UINT4 uLen)
{
    m_uMaxQwLen = uLen;
}


/******************************************************
* 函数名:   CMerger::SetM
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    设置m值(最大话单包号差)。
* 输入: 
*   参数1 :long uValue
* 返回值: 无。 
* 说明: 
* 更新记录: 
*
* 日期        姓名          描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMerger::SetM(UINT4 uValue)
{
    m_uM = uValue;
}

void CMerger::SetHashSize(UINT4 uSize)
{
    m_uHashSize = uSize;
}


/******************************************************
* 函数名:   CMerger::GetOKBill
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    返回Qok尾部的一张话单。
* 输入: 
* 返回值: CBill* ，话单对象的指针。
* 说明:     若Qok有话单则返回话单指针，若没有就返回NULL。
*           若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期        姓名          描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
CBill* CMerger::GetOKBill()
{
    if(m_Qok.IsEmpty())
    {
        return NULL;
    }

    CBill* pBill = m_Qok.GetHead();
    m_Qok.RemoveHead();
    
    return pBill;
}


/******************************************************
* 函数名:  CMergeDelegation::GetResumePID
* 作者:    Zhou Yonghui
* 日期:    01-11-22
* 描述:    计算前存盘模块重发话单包的起点。
*
* 输入:    uPid,后存盘模块保存的最后一个话单包的包号。
* 返回值:  UINT4,前存盘模块重发话单包的起点。
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-11-22        Zhou Yonghui
******************************************************/
UINT4 CMerger::GetResumePID(UINT4 uPid)
{
    if(uPid <= 2 * m_uM)
        return 0;

    return (uPid - (uPid % m_uM) - 2 * m_uM);
}

/******************************************************
* 函数名:  CMergeDelegation::IsBillRemain
* 作者:    Zhou Yonghui
* 日期:    01-11-22
* 描述:    判断是否仍有话单在内存中等待。
*
* 返回值:  若有话单在内存中等待,返回TRUE,否则返回FALSE.
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-11-22        Zhou Yonghui
******************************************************/
BOOL CMerger::IsBillRemain()
{
    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        pDelegation = m_Delegation.GetNext(pos);

        if(pDelegation->GetQwLength() > 0)
            return TRUE;

        if(pDelegation->GetQtLength() > 0)
            return TRUE;
    }

    //已经没有话单在内存中等待。
    return FALSE;
}

//颜文远2002-03-06添加
//返回Qw队列中的等待话单数。
UINT4 CMerger::GetQwLength(UINT4 uBillTypeID)
{
	UINT4 uQwLength = 0;
	
    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        //取pos位置的值，并把pos指向下一个元素。
        pDelegation = m_Delegation.GetNext(pos);
        if(uBillTypeID == pDelegation->GetBillType())
        {
            uQwLength = pDelegation->GetQwLength();
            break;
        }
    }

	return uQwLength;
}

//返回Qt中存放的等待送走的话单数。
UINT4 CMerger::GetQtLength(UINT4 uBillTypeID)
{
	UINT4 uQtLength = 0;

    POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;

    while(pos != NULL)
    {
        //取pos位置的值，并把pos指向下一个元素。
        pDelegation = m_Delegation.GetNext(pos);
        if(uBillTypeID == pDelegation->GetBillType())
        {
            uQtLength = pDelegation->GetQtLength();
            break;
        }
    }

	return uQtLength;
}

//获取要合并的话单类型的个数
int CMerger::GetBillTypeCount()
{
	return m_Delegation.GetSize();
}

//获得指定的话单类型的ID,nIndex从0开始
UINT4 CMerger::GetBillTypeID(int nIndex)
{
	UINT4 uBillTypeID = 0xffffffff;  //初始化为一个无效的ID

	//如果nIndex不合法，则返回一个无效ID
	if (((UINT4)(nIndex + 1) > m_Delegation.GetSize()) || (nIndex < 0))
	{
		return uBillTypeID;
	}
    
	POSITION pos = m_Delegation.GetHeadPosition();
    CMergeDelegation* pDelegation = NULL;
	int nCounter = 0;

    while(pos != NULL)
    {
        //取pos位置的值，并把pos指向下一个元素。
        pDelegation = m_Delegation.GetNext(pos);
        if(nCounter == nIndex)
        {
            uBillTypeID = pDelegation->GetBillType();
            break;
        }

		nCounter++;
    }

	return uBillTypeID;
}
//添加结束，颜文远2002-03-06


/********** CMergeDelegetion的实现 ***************/

CMergeDelegation::CMergeDelegation(CMerger* pMerger,UINT4 uBillType)
{
    assert(pMerger != NULL);

    m_uBillType = uBillType;
    m_uQwLen    = DEFAULT_MAX_QW_LENGTH;
    m_uMValue   = DEFAULT_MAX_WAITTING_TIME;
    m_uQtStep   = 0;
    m_pMerger   = pMerger;
}

CMergeDelegation::~CMergeDelegation()
{
}


/******************************************************
* 函数名:   CMergeDelegation::Init
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    初始化HASH索引表。
* 输入: 
*       参数1 :long uIndexSize，新的HASH表的大小。
* 返回值: void 
* 说明:    若初始化成功，HASH所有中所有数据都将丢失。
*          若内存操作失败，将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::Init(UINT4 uIndexSize)
{
    m_Index.InitHashTable(uIndexSize);
}


/******************************************************
* 函数名:   CMergeDelegation::MergeABill
* 作者:    Zhou Yonghui
* 日期:    01-5-24
* 描述:    由CMerger对象的MergeABill方法调用，完成一张话单的合并。
* 输入: 
*   参数1 :CBill* pBill，话单对象的指针。
*   参数2 :PackID_t PackNum，话单的包号。
* 返回值: 无。 
* 说明:     若内存操作失败将抛出异常。
* 更新记录: 
*
* 日期        姓名          描述 
* ========== ================== ======================
*01-5-24       Zhou Yonghui
******************************************************/
void CMergeDelegation::MergeABill(CBill* pBill, PackID_t PackNum)
{
    assert(pBill != NULL);
    assert(pBill->IsPartial());

    CBill* BillCur = pBill;
    CKey Key = BillCur->CreateKey();

    //pos是BillCur在Qw中对应话单的“位置指针”。
    POSITION pos = (POSITION)m_Index.Lookup(Key);

    if(pos == NULL)                         //没有找到匹配的话单。
    {
        if(BillCur->IsLastPartialRec())     //末张部分话单，送Qok。
        {
            MoveToQok(BillCur);
        }
        else if(BillCur->IsFirstPartialRec())//第一张部分话单。
        {
            CNode node;
            node.m_pBill = BillCur;
            node.m_PackNum = PackNum;
            
            //把BillCur加入Qw头部，并返回在Qw中的“位置指针”。
            pos = m_Qw.AddHead(node);
            
            //在HASH索引表中把话单的索引键与“位置指针”关联起来。
            m_Index.SetAt(Key,(void*)pos);
        }
        else                                //中间部分话单。
        {
            CNode node;
            node.m_pBill = BillCur;
            node.m_PackNum = PackNum - (PackNum % m_uMValue);

            //把BillCur加入Qw尾部，并返回在Qw中的“位置指针”。
            pos = m_Qw.AddTail(node);
            m_Index.SetAt(Key,(void*)pos);
        }
        return;
    }

    CBill* BillW = m_Qw.GetAt(pos).m_pBill; //找到匹配的话单（pos不改变）。
    if(BillW->CanSafeMerge(BillCur))        //允许合并。
    {
        //判断是否重单
        if(BillW->IsDuplicated(BillCur))
        {            
            SInnerAlarm sAlarm;
            sAlarm.yAlarmType    = AT_EVENT;
            sAlarm.uAlarmID      = ALARM_ID_ABNORMAL_BILL;
            sAlarm.yAlarmPara[0] = ALARM_PARA_DUPLICATED_BILL;
            CFrameTools::SendAlarm(&sAlarm);
            
            delete BillCur;
            return;            
        }
        
        if(!BillW->Merge(BillCur))          //把BillCur合入BillW中。
        {
            throw "Merge bill fail.";       //若合并失败抛出异常。
        }
        delete BillCur;                     //BillCur已经合入BillW中，
                                            //需要回收空间。

        if(BillW->IsPDPCompleted())
        {
            if(BillW->NeedWaiting())
            {
                return;                     //继续等待后续话单。
            }

            m_Index.RemoveAt(Key,pos);      //删除BillW在HASH表中的索引。
            m_Qw.RemoveAt(pos);             //删除BillW在Qw中对应的节点。
            MoveToQok(BillW);               //把合并后的话单(BillW)送Qok。
        }
    }
    else                                    //不允许合并。
    {
        if(BillCur->IsLastPartialRec())     //末张部分话单，BillW、BillCur分别送Qok。
        {
            m_Index.RemoveAt(Key,pos);      //删除BillW在HASH表中的索引。
            m_Qw.RemoveAt(pos);             //删除BillW在Qw中对应的节点。
            MoveToQok(BillW);               //把BillW送Qok。
            MoveToQok(BillCur);             //把BillCur送Qok。
        }
        else                                //中间部分话单，送Qw等待后续话单。
        {
            MoveToQok(BillW);               //BillW送Qok。
            CNode& node = m_Qw.GetAt(pos);  //BillCur替换BillW在Qw中的位置。
            node.m_pBill = BillCur;
        }
    }
}


/******************************************************
* 函数名:   CMergeDelegation::MoveToQok
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    把一张话单送Qok队列。
* 输入: 
*       参数1 :CBill* pBill
* 返回值: void 
* 说明:    所有类型的话单使用同一个由CMerger维护的Qok队列。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::MoveToQok(CBill* pBill)
{
    assert(pBill != NULL);
    m_pMerger->MoveToQok(pBill);
}


/******************************************************
* 函数名:   CMergeDelegation::Flush
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    把Qw、Qt中的所有话单都作为黑话单送Qok队列。
* 输入: 
* 返回值: void 
* 说明:    若内存操作失败，将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::Flush()
{
    POSITION pos = m_Qw.GetHeadPosition();

    while(pos != NULL)
    {
        //返回pos位置的值，且pos指向下一个位置。
        CNode& node = m_Qw.GetNext(pos);
        MoveToQok(node.m_pBill);
    }

    //清除Qok队列。
    m_Index.RemoveAll();
    //清除Qw队列。
    m_Qw.RemoveAll();

    //Qt中存放的是CBill*指针。
    pos = m_Qt.GetHeadPosition();
    while(pos != NULL)
    {
        MoveToQok(m_Qt.GetNext(pos));
    }

    m_Qt.RemoveAll();
}


/******************************************************
* 函数名:   CMergeDelegation::GenerateBlackBill
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    生成黑话单。
* 输入: 
*       参数1 :PackID_t PackNum，当前话单包号。
* 返回值: 无。
* 说明:    若内存操作失败，将抛出异常。
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::GenerateBlackBill(PackID_t PackNum)
{
    POSITION pos = m_Qw.GetTailPosition();
    POSITION prev = NULL;

#ifdef _ONLY_FOR_TEST_

    if((PackNum % 10) == 0)
    {
        printf("Length of Qw: %d,Length of Qt:%d.\n",
            m_Qw.GetSize(),m_Qt.GetSize());
    }

#endif

    /* 判断Qw队列的长度。 */
    while(m_uQwLen < m_Qw.GetSize())
    {
        prev = pos;
        /* 返回pos位置的值，且pos指向前一个位置。 */
        CNode& node = m_Qw.GetPrev(pos);

        CKey Key = node.m_pBill->CreateKey();
        MoveToQok(node.m_pBill);
        m_Index.RemoveAt(Key,prev);     //删除HASH索引表中的节点。
        m_Qw.RemoveAt(prev);            //删除Qw中对应节点。
    }

    /* 判断是否到达超时检查点。 */
    if((PackNum % m_uMValue) == 0)
    {
        /* 
         * 把超时话单话单送Qt,这时Qt一定为空。
         */
        assert(m_Qt.IsEmpty());

        pos = m_Qw.GetTailPosition();
        
        while(pos != NULL)
        {
            prev = pos;
            /* 返回pos位置的值，且pos指向前一个位置。 */
            CNode& node = m_Qw.GetPrev(pos);

            if(node.m_PackNum <= (PackNum - m_uMValue))
            {
                CKey Key = node.m_pBill->CreateKey();
                m_Qt.AddHead(node.m_pBill);

                m_Index.RemoveAt(Key,prev); //删除HASH索引表中的节点。
                m_Qw.RemoveAt(prev);        //删除Qw中对应节点。
            }
            else
            {
                break;
            }
        }
        
        UINT4 uLen = m_Qt.GetSize();
        
        /* 
         * 每次从Qt中取话单的数量，这个值必须保证在到达下一个检查点之前，
         * Qt中的话单已经被全部送Qok。
         */
        m_uQtStep = (UINT4)(uLen / (m_uMValue - 1)) + 1;
    }
    else
    {
        /* 把Qt中的部分话单送Qok中。 */
        if(m_Qt.GetSize() >= m_uQtStep)
        {
            for(UINT4 i = 0;i < m_uQtStep;i++)
            {
                MoveToQok(m_Qt.GetTail());
                m_Qt.RemoveTail();
            }
        }
        else
        {
            pos = m_Qt.GetHeadPosition();
            while(pos != NULL)
            {
                MoveToQok(m_Qt.GetNext(pos));
            }

            m_Qt.RemoveAll();
        }
    }
}


/******************************************************
* 函数名:   CMergeDelegation::SetMaxQwLength
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    设置Qw队列的最大长度，作为生成黑话单的依据。
* 输入: 
*       参数1 :long uLen，Qw队列的最大长度。
* 返回值: void 
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::SetMaxQwLength(UINT4 uLen)
{
    m_uQwLen = uLen;
}


/******************************************************
* 函数名:   CMergeDelegation::SetM
* 作者:    Zhou Yonghui
* 日期:    01-6-7
* 描述:    设置最大包号差，作为生成黑话单的依据。
* 输入: 
*       参数1 :long uValue，最大包号差。
* 返回值: 无。 
* 说明: 
* 更新记录: 
*
* 日期            姓名                  描述 
* ========== ================== ======================
*01-6-7        Zhou Yonghui
******************************************************/
void CMergeDelegation::SetM(UINT4 uValue)
{
    m_uMValue = uValue;
}

UINT4 CMergeDelegation::GetBillType()
{
    return m_uBillType;
}

UINT4 CMergeDelegation::GetQtLength()
{
    return m_Qt.GetSize();
}

UINT4 CMergeDelegation::GetQwLength()
{
    return m_Qw.GetSize();
}
