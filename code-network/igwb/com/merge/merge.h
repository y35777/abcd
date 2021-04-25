
#ifndef _MERGE_H
#define _MERGE_H

#include <assert.h>
#include "list.h"
#include "bill.h"
#include "index.h"

class CMergeDelegation
{
protected:
    //Qw队列节点的结构。
    struct CNode
    {
        CBill*  m_pBill;      //话单对象的指针。
        PackID_t m_PackNum;   //话单对应的包号。
    };

protected:
    //指向合并对象的指针，其中包含了Qok队列。
    CMerger* m_pMerger;

    //Qw队列的头指针。
    CList_t<CNode> m_Qw;

    //存放超时话单的队列。
    CList_t<CBill*> m_Qt;

    //索引表。
    CHashIndex_t<CKey> m_Index;

    //Qw的长度。
    UINT4 m_uQwLen;
    
    //每次从Qt队列中取话单的数量。
    UINT4 m_uQtStep;

    //m值。
    UINT4 m_uMValue;

    //对象合并的话单的类型。
    UINT4 m_uBillType;

public:
    CMergeDelegation(CMerger* pMerger,UINT4 uBillType);
    virtual ~CMergeDelegation();

    //初始话单合并对象。
    virtual void Init(UINT4 uIndexSize);

    //返回合并的话单类型。
    virtual UINT4 GetBillType();

    //合并一条话单。
    virtual void MergeABill(CBill* pBill, PackID_t lPackNum);

    //把一条话单放入Qok中。
    virtual void MoveToQok(CBill* pBill);

    //把Qw中的话单全部放入Qok中。
    virtual void Flush();

    //产生黑话单。
    virtual void GenerateBlackBill(PackID_t lPackNum);

    //设置Qw队列的最大长度，作为产生黑话单的依据。
    virtual void SetMaxQwLength(UINT4 uLen);

    //设置M值，作为产生黑话单的依据。
    virtual void SetM(UINT4 uValue);

    //返回Qw队列中的等待话单数。
    virtual UINT4 GetQwLength();

    //返回Qt中存放的等待送走的话单数。
    virtual UINT4 GetQtLength();
};

class IGWB_Svc_Export CMerger
{
protected:
    //完成合并功能的对象列表。
    CList_t<CMergeDelegation*> m_Delegation;

    //Qok队列。
    CList_t<CBill*> m_Qok;

    //HASH表的大小。
    UINT4 m_uHashSize;

    //Qw队列的最大长度。
    UINT4 m_uMaxQwLen;

    //最大包号差。
    UINT4 m_uM;

public:
    CMerger();
    ~CMerger();

    //合并一条话单。
    virtual void MergeABill(CBill* pBill, PackID_t lPackNum);

    //把一条话单放入Qok中。
    virtual void MoveToQok(CBill* pBill);

    //把Qw中的话单全部放入Qok中。
    virtual void Flush();

    //产生黑话单。
    virtual void GenerateBlackBill(PackID_t lPackNum);

    /*
        注册话单类型。
        该函数必须在SetM、SetMaxQwLength、SetHashSize方法之后调用。
    */
    virtual void RegisterBillType(UINT4 uTypeID);

    //设置Qw队列的最大长度，作为产生黑话单的依据。
    virtual void SetMaxQwLength(UINT4 uLen);

    //设置最大包号差，作为产生黑话单的依据。
    virtual void SetM(UINT4 uValue); 

    //设置HASH索引表的大小。
    virtual void SetHashSize(UINT4 uSize);
    
    //从Qok中返回一条话单，若Qok为空，则返回NULL。
    virtual CBill* GetOKBill();

    //计算恢复的起始包号。
    virtual UINT4 GetResumePID(UINT4 uPid);

    //判断是否仍有话单在内存中等待。
    virtual BOOL IsBillRemain();

	//颜文远2002-03-06添加
    //返回Qw队列中的等待话单数。
    virtual UINT4 GetQwLength(UINT4 uBillTypeID);

    //返回Qt中存放的等待送走的话单数。
    virtual UINT4 GetQtLength(UINT4 uBillTypeID);

	//获取要合并的话单类型的个数
	int GetBillTypeCount();

	//获得指定的话单类型的ID,nIndex从0开始
	UINT4 GetBillTypeID(int nIndex);
	//添加结束，颜文远2002-03-06
};

#endif

