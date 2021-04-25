/*
    产品名:iGateway Bill V200
    模块名:后存盘
    文件名:hot_save_chanl.h
    描述信息:
    
    包含CHotSaveChannel类的定义. 
    因为实时计费的需求，要求送往计费中心的第二份话单文件对话单包的存储是实时的，
    所以增加此类，完成对第一份临时文件和第二份话单文件同时写操作的特性处理。

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2004 华为技术有限公司

    修改记录:
    陈凉威,2004-05-14,创建.
*/



#ifndef __HOTSAVECHANNEL_H__
#define __HOTSAVECHANNEL_H__

#include "save_chanl.h"

class CHotSaveChannel : public CSaveChannel  
{
public:
    //构造、析构函数
    CHotSaveChannel(CPatchEncap* pPatchEncap);
    virtual ~CHotSaveChannel();

    //初始化函数
    BOOL Init(UINT4 uAPID, UINT4 uChannelID, STRING& szChannelName, int nBillLen = 0);

protected:

    //关闭话单文件的后续处理
    BOOL DoWhenFileClose(); 
    
    //在保存话单包事务中的写话单文件操作
    int WriteBillFile(BYTE* pPacketInfo, BOOL bFlush); 

    //创建新的话单文件以及相关的状态文件刷新操作
    BOOL CreateNewFile();

    //根据状态文件里的信息对相关的成员变量进行初始化
    BOOL InitByStatusFile();

    //删除最后一个临时文件，在进行状态文件校验时调用
    BOOL DeleteTempFile();

protected:
    FILE*     m_pSeccondFile;        //当前正在处理的第二份话单文件对象指针
    STRING    m_szSecondFileName;    //当前正在处理的第二份话单文件名

};

#endif //__HOTSAVECHANNEL_H__
