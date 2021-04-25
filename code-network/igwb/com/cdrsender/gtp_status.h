/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : gtp_client.h
 *描述信息  ：本文件是类CGtpClient的定义
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#ifndef __GTP_STATUS_H__
#define __GTP_STATUS_H__

#define PEER_IPCOUNT_MAX  10

//CDRSENDER模块GTP'协议的状态结构
typedef struct _GtpInfo
{
    unsigned long  uAppType;             //协议类型,取值于APP_TYPE
    unsigned long  uGtpVersion;          //当前GTP'的版本号
    unsigned long  nPeerActivedIP;       //对端活动IP地址
    unsigned long  uPeerIp[PEER_IPCOUNT_MAX];  //给对端预留10个IP地址
    unsigned long  uPeerPort;            //BS的端口号
    unsigned long  uLocalIp;             //本地IP地址,只保留最近用的那个
    unsigned long  uLocalPort;           //本地端口号,只保留最近用的那个
    unsigned long  uPeerRestartCount;    //对端的重启次数
    unsigned long  uLocaRestartCount;    //本端重起的次数
    unsigned short uSeqNum;              //已经成功发送的数据帧的序列号            
    unsigned long uReserved[50];         //保留一部分空间
} SGtpInfo;

#define GTP_NODE_INFO_LENGTH sizeof(SGtpInfo)


#endif //__GTP_STATUS_H__