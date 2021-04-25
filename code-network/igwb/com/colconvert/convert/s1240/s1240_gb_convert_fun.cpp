//转换S1240详细话单到文本格式

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../comm_convert_fun.h"
#include "../include/collect_convert_define.h"
#include "../../include/collect_convert_include.h"
#include "../../../include/toolbox.h"
#include "../../../include/format_out.h"



//S1240国标IDD详单的话单格式
struct S1240GBIDDDetailSrcFmt
{
    unsigned char RecordType : 4;   //BCD，表示该话单记录的呼叫的类型
                                    //1： PSTN IDD/DDD呼叫记录
                                    //2： DBO呼叫记录
                                    //3： IN呼叫记录
                                    //4： ISDN/CTX记录
                                    //5： TAX产生记录
                                    //6-7：保留
    unsigned char PartialFlg : 4;   //BCD，表示该话单是单独的话单还是一个长话单中的一部分
                                    //0： 单个记录
                                    //1： 记录的第一部分
                                    //2： 记录的中间部分
                                    //3： 记录的最后部分
    unsigned char SeqNum[4];        //顺序号
    unsigned char CallerLocAttr[1]; //表示主叫用户的地址性质
                                    //0： 用户号码
                                    //1： 空闲
                                    //2： 国内有效号码
                                    //3： 国际号码
    unsigned char CallerNum[10];    //主叫号码，BCD向左对齐，不足位用16进制的E填充
    unsigned char CalledLocAttr[1]; //表示被叫用户的地址性质
    unsigned char CalledNum[10];    //被叫号码，BCD向左对齐，不足位用16进制的E填充
    unsigned char StartYear[1];     //起始年，BCD码
    unsigned char StartMonth[1];    //起始月，BCD码
    unsigned char StartDay[1];      //起始日，BCD码
    unsigned char StartHour[1];     //起始时，BCD码
    unsigned char StartMinute[1];   //起始分，BCD码
    unsigned char StartSecond[1];   //起始秒，BCD码
    unsigned char StartTSecond : 4; //起始十分之一秒，BCD码
    unsigned char CallType : 4;     //表示呼叫类型
                                    //0~1：备用
                                    //2：国际转话
                                    //3：本地呼叫
                                    //4：设备呼叫（facility call）
                                    //5：国内发话（发端长途局）
                                    //6：国际发话（国际和发端长途局）
                                    //7：国内来话（终端长途局）
                                    //8：国际来话（国际入口局）
                                    //9：转话（国际/长途局）（不用于国内转话）
    unsigned char EndYear[1];       //终止年，BCD码
    unsigned char EndMonth[1];      //终止月，BCD码
    unsigned char EndDay[1];        //终止日，BCD码
    unsigned char EndHour[1];       //终止时，BCD码
    unsigned char EndMinute[1];     //终止分，BCD码
    unsigned char EndSecond[1];     //终止秒，BCD码
    unsigned char EndTSecond : 4;   //终止十分之一秒，BCD码
    unsigned char TermCause : 4;    //BCD码，表示呼叫结束的原因
                                    //0：主叫用户挂机
                                    //1：被叫用户挂机
                                    //2：异常结束
                                    //3：不可用
    unsigned char Duration[4];      //通话时长，BCD码
    unsigned char CallerType1[1];
    unsigned char CallerType2 : 4;  //BCD码，主叫用户类别
                                    //00：主叫用户类别不知道
                                    //01：话务员法语
                                    //02：话务员英语
                                    //03：话务员德语
                                    //04：话务员俄语
                                    //05：话务员西班牙语
                                    //06：话务员，双方协商采用的语言（汉语）
                                    //07：话务员，双方协商采用的语言
                                    //08：话务员，双方协商采用的语言（日语）
                                    //09：国内话务员
                                    //0A：普通用户（长-长，长-市间用）
                                    //0B：优先用户（长-长，长-市，市-市间用）
                                    //0C：数据呼叫（话带数据）
                                    //0D：测试呼叫
                                    //0E：备用
                                    //0F：付费电话
                                    //10~BF：备用
                                    //E0~EF：国内备用
                                    //F0：普通、免费（市-长间用）
                                    //F1：普通定期（市-长间用）
                                    //F2：普通用户表立即（市-长间用）
                                    //F3：普通打印机立即（市-长间用）
                                    //F4：优先免费（市-长间用）
                                    //F5：优先定期（市-长间用）
    unsigned char ValidFlg : 1;     //记录有效性指示，二进制，0：该记录有效 1：该记录无效
    unsigned char TimerTickFlg : 1; //时钟变化标识，0：呼叫过程中时钟发生了变化 1：呼叫过程中时钟没有发生变化
    unsigned char FreeFlg : 1;      //计费/免费标识，二进制，0：计费 1：免费
    unsigned char TryCallFlg : 1;   //试呼标记 （备用），二进制，0：试呼免费 1：试呼计费

    unsigned char IncomingCTX[2];   //入中继群标识（0~9999），BCD码
    unsigned char OutgoingCTX[2];   //出中继群标识（0~9999），BCD码
    unsigned char CallToSS[7];      //呼叫中调用SS，二进制
    unsigned char CallingType[1];   //计费方身份标识
                                    //1：对主叫用户计费
                                    //2：对被叫用户计费
                                    //3：对目标地址号码计费
                                    //127：对规定的计费号码计费
    unsigned char RelatedNumExp[1]; //连接号码地址性质表示语，BCD向右对齐
    unsigned char RelatedNum[10];   //连接号码，BCD向左对齐
    unsigned char Fee[4];           //费用，BCD码
    unsigned char Reserved[4];      //保留
};

//S1240国标ISDN详单的话单格式
struct S1240GBISDNDetailSrcFmt
{
    unsigned char RecordType : 4;   //BCD，表示该话单记录的呼叫的类型
                                    //1： PSTN IDD/DDD呼叫记录
                                    //2： DBO呼叫记录
                                    //3： IN呼叫记录
                                    //4： ISDN/CTX记录
                                    //5： TAX产生记录
                                    //6-7：保留
    unsigned char PartialFlg : 4;   //BCD，表示该话单是单独的话单还是一个长话单中的一部分
                                    //0： 单个记录
                                    //1： 记录的第一部分
                                    //2： 记录的中间部分
                                    //3： 记录的最后部分
    unsigned char SeqNum[4];        //顺序号
    unsigned char CallerLocAttr[1]; //表示主叫用户的地址性质,BCD向右对齐
                                    //0： 用户号码
                                    //1： 空闲
                                    //2： 国内有效号码
                                    //3： 国际号码
    unsigned char CallerNum[10];    //主叫号码，BCD向左对齐，不足位用16进制的E填充
    unsigned char CalledLocAttr[1]; //表示被叫用户的地址性质
    unsigned char CalledNum[10];    //被叫号码，BCD向左对齐，不足位用16进制的E填充
    unsigned char StartYear[1];     //起始年，BCD码
    unsigned char StartMonth[1];    //起始月，BCD码
    unsigned char StartDay[1];      //起始日，BCD码
    unsigned char StartHour[1];     //起始时，BCD码
    unsigned char StartMinute[1];   //起始分，BCD码
    unsigned char StartSecond[1];   //起始秒，BCD码
    unsigned char StartTSecond : 4; //起始十分之一秒，BCD码
    unsigned char CallType : 4;     //表示呼叫类型
                                    //0~1：备用
                                    //2：国际转话
                                    //3：本地呼叫
                                    //4：设备呼叫（facility call）
                                    //5：国内发话（发端长途局）
                                    //6：国际发话（国际和发端长途局）
                                    //7：国内来话（终端长途局）
                                    //8：国际来话（国际入口局）
                                    //9：转话（国际/长途局）（不用于国内转话）
    unsigned char EndYear[1];       //终止年，BCD码
    unsigned char EndMonth[1];      //终止月，BCD码
    unsigned char EndDay[1];        //终止日，BCD码
    unsigned char EndHour[1];       //终止时，BCD码
    unsigned char EndMinute[1];     //终止分，BCD码
    unsigned char EndSecond[1];     //终止秒，BCD码
    unsigned char EndTSecond : 4;   //终止十分之一秒，BCD码
    unsigned char TermCause : 4;    //BCD码，表示呼叫结束的原因
                                    //0：主叫用户挂机
                                    //1：被叫用户挂机
                                    //2：异常结束
                                    //3：不可用
    unsigned char Duration[4];      //通话时长，BCD码
    unsigned char CallerType1[1];
    unsigned char CallerType2 : 4;  //BCD码，主叫用户类别
                                    //00：主叫用户类别不知道
                                    //01：话务员法语
                                    //02：话务员英语
                                    //03：话务员德语
                                    //04：话务员俄语
                                    //05：话务员西班牙语
                                    //06：话务员，双方协商采用的语言（汉语）
                                    //07：话务员，双方协商采用的语言
                                    //08：话务员，双方协商采用的语言（日语）
                                    //09：国内话务员
                                    //0A：普通用户（长-长，长-市间用）
                                    //0B：优先用户（长-长，长-市，市-市间用）
                                    //0C：数据呼叫（话带数据）
                                    //0D：测试呼叫
                                    //0E：备用
                                    //0F：付费电话
                                    //10~BF：备用
                                    //E0~EF：国内备用
                                    //F0：普通、免费（市-长间用）
                                    //F1：普通定期（市-长间用）
                                    //F2：普通用户表立即（市-长间用）
                                    //F3：普通打印机立即（市-长间用）
                                    //F4：优先免费（市-长间用）
                                    //F5：优先定期（市-长间用）
    unsigned char ValidFlg : 1;     //记录有效性指示，二进制，0：该记录有效 1：该记录无效
    unsigned char TimerTickFlg : 1; //时钟变化标识，0：呼叫过程中时钟发生了变化 1：呼叫过程中时钟没有发生变化
    unsigned char FreeFlg : 1;      //计费/免费标识，二进制，0：计费 1：免费
    unsigned char TryCallFlg : 1;   //试呼标记 （备用），二进制，0：试呼免费 1：试呼计费
    unsigned char IncomingCTX[2];   //入中继群标识（0~9999），BCD码
    unsigned char OutgoingCTX[2];   //出中继群标识（0~9999），BCD码
    unsigned char CallToSS[7];      //呼叫中调用SS，二进制
    unsigned char CallingType[1];   //计费方身份标识
                                    //1：对主叫用户计费
                                    //2：对被叫用户计费
                                    //3：对目标地址号码计费
                                    //127：对规定的计费号码计费
    unsigned char RelatedNumExp[1]; //连接号码地址性质表示语，BCD向右对齐
    unsigned char RelatedNum[10];   //连接号码，BCD向左对齐
    unsigned char Fee[4];           //费用，BCD码
    unsigned char BearingService[1];//BCD，承载业务0~99
                                    //1：电路模式，64kbit/s不受限，8kHz结构化承载业务
                                    //2：电路模式，64kbit/s,8kHz结构化承载语音
                                    //3：电路模式，64kbit/s,8kHz结构化承载3.1kHz语音
                                    //4：分组模式，ISDN虚呼叫，永久虚电路承载业务由B通道提供的用户接入
                                    //5：用户信令承载业务
                                    //7：电路模式，2*64kbit/s不受限，8kHz结构化承载业务类别
                                    //8：电路模式，384kbit/s不受限，8kHz结构化承载业务类别
                                    //9：电路模式，1536kbit/s不受限，8kHz结构化承载业务类别
                                    //10：电路模式，1920kbit/s不受限， 8 kHz结构化承载业务类别
    unsigned char TerminalServ : 4; //终端业务，BCD:0~99
                                    //0：备用
                                    //1：3.1kHz电信业务
                                    //2：7kHz电信业务
                                    //3：4类传真
                                    //4：智能用户电报
                                    //5：可视图文
                                    //6：混合型电信业务
                                    //7：7kHz图象
    unsigned char TryCallPulse : 4; //备用（试呼脉冲）
    unsigned char UUS1Count[1];     //UUS1计数，二进制
    unsigned char UUS2Count[1];     //UUS2计数，二进制
    unsigned char CallerSpecNum[5]; //主叫方专用号码,BCD向左对齐
    unsigned char CalledSpecNum[5]; //被叫方专用号码,BCD向左对齐
    unsigned char CTXLabel[2];      //CTX标识，二进制
    unsigned char AccountProp[1];   //计费号码地址性质表示语,BCD向右对齐
                                    //0：用户号码
                                    //1：空闲（国内备用）
                                    //2：国内号码
                                    //3：国际号码
                                    //4：记帐卡A类卡
                                    //5：记帐卡B类卡
                                    //6：记帐卡C类卡
                                    //7：记帐卡D类卡
                                    //8：VISA卡
                                    //9：VPN群号
                                    //10：VPN分机号
                                    //12：工商银行帐号
                                    //13：建设银行帐号
                                    //14：中国银行帐号
                                    //15：交通银行帐号
                                    //16：农业银行帐号
                                    //其他：备用
    unsigned char AccountNum[11];   //计费号码,BCD向左对齐
    unsigned char Reserved[4];      //保留
};

//S1240国标IN详单的话单格式
struct S1240GBINDetailSrcFmt
{
    unsigned char RecordType : 4;   //BCD，表示该话单记录的呼叫的类型
                                    //1： PSTN IDD/DDD呼叫记录
                                    //2： DBO呼叫记录
                                    //3： IN呼叫记录
                                    //4： ISDN/CTX记录
                                    //5： TAX产生记录
                                    //6-7：保留
    unsigned char PartialFlg : 4;   //BCD，表示该话单是单独的话单还是一个长话单中的一部分
                                    //0： 单个记录
                                    //1： 记录的第一部分
                                    //2： 记录的中间部分
                                    //3： 记录的最后部分
    unsigned char SeqNum[4];        //顺序号
    unsigned char CallerLocAttr[1]; //表示主叫用户的地址性质
                                    //0： 用户号码
                                    //1： 空闲
                                    //2： 国内有效号码
                                    //3： 国际号码
    unsigned char CallerNum[10];    //主叫号码，BCD向左对齐，不足位用16进制的E填充
    unsigned char CalledLocAttr[1]; //表示被叫用户的地址性质
    unsigned char CalledNum[10];    //被叫号码，BCD向左对齐，不足位用16进制的E填充
    unsigned char StartYear[1];     //起始年，BCD码
    unsigned char StartMonth[1];    //起始月，BCD码
    unsigned char StartDay[1];      //起始日，BCD码
    unsigned char StartHour[1];     //起始时，BCD码
    unsigned char StartMinute[1];   //起始分，BCD码
    unsigned char StartSecond[1];   //起始秒，BCD码
    unsigned char StartTSecond : 4; //起始十分之一秒，BCD码
    unsigned char CallType : 4;     //表示呼叫类型
                                    //0~1：备用
                                    //2：国际转话
                                    //3：本地呼叫
                                    //4：设备呼叫（facility call）
                                    //5：国内发话（发端长途局）
                                    //6：国际发话（国际和发端长途局）
                                    //7：国内来话（终端长途局）
                                    //8：国际来话（国际入口局）
                                    //9：转话（国际/长途局）（不用于国内转话）
    unsigned char EndYear[1];       //终止年，BCD码
    unsigned char EndMonth[1];      //终止月，BCD码
    unsigned char EndDay[1];        //终止日，BCD码
    unsigned char EndHour[1];       //终止时，BCD码
    unsigned char EndMinute[1];     //终止分，BCD码
    unsigned char EndSecond[1];     //终止秒，BCD码
    unsigned char EndTSecond : 4;   //终止十分之一秒，BCD码
    unsigned char TermCause : 4;    //BCD码，表示呼叫结束的原因
                                    //0：主叫用户挂机
                                    //1：被叫用户挂机
                                    //2：异常结束
                                    //3：不可用
    unsigned char Duration[4];      //通话时长，BCD码
    unsigned char CallerType1[1];
    unsigned char CallerType2 : 4;  //BCD码，主叫用户类别
                                    //00：主叫用户类别不知道
                                    //01：话务员法语
                                    //02：话务员英语
                                    //03：话务员德语
                                    //04：话务员俄语
                                    //05：话务员西班牙语
                                    //06：话务员，双方协商采用的语言（汉语）
                                    //07：话务员，双方协商采用的语言
                                    //08：话务员，双方协商采用的语言（日语）
                                    //09：国内话务员
                                    //0A：普通用户（长-长，长-市间用）
                                    //0B：优先用户（长-长，长-市，市-市间用）
                                    //0C：数据呼叫（话带数据）
                                    //0D：测试呼叫
                                    //0E：备用
                                    //0F：付费电话
                                    //10~BF：备用
                                    //E0~EF：国内备用
                                    //F0：普通、免费（市-长间用）
                                    //F1：普通定期（市-长间用）
                                    //F2：普通用户表立即（市-长间用）
                                    //F3：普通打印机立即（市-长间用）
                                    //F4：优先免费（市-长间用）
                                    //F5：优先定期（市-长间用）
    unsigned char ValidFlg : 1;     //记录有效性指示，二进制，0：该记录有效 1：该记录无效
    unsigned char TimerTickFlg : 1; //时钟变化标识，0：呼叫过程中时钟发生了变化 1：呼叫过程中时钟没有发生变化
    unsigned char FreeFlg : 1;      //计费/免费标识，二进制，0：计费 1：免费
    unsigned char TryCallFlg : 1;   //试呼标记 （备用），二进制，0：试呼免费 1：试呼计费
    unsigned char IncomingCTX[2];   //入中继群标识（0~9999），BCD码
    unsigned char OutgoingCTX[2];   //出中继群标识（0~9999），BCD码
    unsigned char CallToSS[7];      //呼叫中调用SS，二进制
    unsigned char CallingType[1];   //计费方身份标识
                                    //1：对主叫用户计费
                                    //2：对被叫用户计费
                                    //3：对目标地址号码计费
                                    //127：对规定的计费号码计费
    unsigned char Fee[4];           //费用，BCD码
    unsigned char BearingService[1];//BCD，承载业务0~99
                                    //1：电路模式，64kbit/s不受限，8kHz结构化承载业务
                                    //2：电路模式，64kbit/s,8kHz结构化承载语音
                                    //3：电路模式，64kbit/s,8kHz结构化承载3.1kHz语音
                                    //4：分组模式，ISDN虚呼叫，永久虚电路承载业务由B通道提供的用户接入
                                    //5：用户信令承载业务
                                    //7：电路模式，2*64kbit/s不受限，8kHz结构化承载业务类别
                                    //8：电路模式，384kbit/s不受限，8kHz结构化承载业务类别
                                    //9：电路模式，1536kbit/s不受限，8kHz结构化承载业务类别
                                    //10：电路模式，1920kbit/s不受限， 8 kHz结构化承载业务类别
    unsigned char TerminalServ : 4; //终端业务，BCD:0~99
                                    //0：备用
                                    //1：3.1kHz电信业务
                                    //2：7kHz电信业务
                                    //3：4类传真
                                    //4：智能用户电报
                                    //5：可视图文
                                    //6：混合型电信业务
                                    //7：7kHz图象
    unsigned char TryCallPulse : 4; //备用（试呼脉冲）
    unsigned char UUS1Count[1];     //UUS1计数，二进制
    unsigned char UUS2Count[1];     //UUS2计数，二进制
    unsigned char CallerSpecNum[5]; //主叫方专用号码,BCD向左对齐
    unsigned char CalledSpecNum[5]; //被叫方专用号码,BCD向左对齐
    unsigned char CTXLabel[2];      //CTX标识，二进制
    unsigned char AccountProp[1];   //计费号码地址性质表示语,BCD向右对齐
                                    //0：用户号码
                                    //1：空闲（国内备用）
                                    //2：国内号码
                                    //3：国际号码
                                    //4：记帐卡A类卡
                                    //5：记帐卡B类卡
                                    //6：记帐卡C类卡
                                    //7：记帐卡D类卡
                                    //8：VISA卡
                                    //9：VPN群号
                                    //10：VPN分机号
                                    //12：工商银行帐号
                                    //13：建设银行帐号
                                    //14：中国银行帐号
                                    //15：交通银行帐号
                                    //16：农业银行帐号
                                    //其他：备用
    unsigned char AccountNum[11];   //计费号码,BCD向左对齐
    unsigned char TransNumExp[1];   //翻译号码地址性质表示语,BCD向右对齐
                                    //0：用户号码 1：空闲 2：国内有效号码 3：国际号码
    unsigned char TransNum[10];     //翻译号码,BCD向左对齐
    unsigned char PosiNumExp[1];    //位置号码地址性质表示语,BCD向右对齐
                                    //0：用户号码 1：空闲 2：国内有效号码 3：国际号码
    unsigned char PosiNum[10];      //位置号码,BCD向左对齐
    unsigned char ChargeRateType[2];//计费率种类,BCD
    unsigned char ChargeRate1[1];   //计费调整系数，BCD 建议 1个字节表示计费调整率0~255 半个字节计费调整类型1=费率，2=费用
    unsigned char ChargeRate2 : 4;
    unsigned char Reserved1 : 4;    //保留
    unsigned char INAddition[3];    //IN附加费，BCD 建议2个字节表示附加费值 1个字节表示附加费类型，1=附加钱数 2表示附加费率
    unsigned char AddedPara[20];    //透明参数,表示IN业务中调用的业务特性
    unsigned char Reserved[20];     //保留
};

//计次表记录的源格式
struct S1240GBMeterSrcFmt
{
    unsigned char RecordType : 4;   //BCD，0：表示计次话单 1-9：保留
    unsigned char PulseNum : 4;     //BCD，表示本话单使用的跳表个数
    unsigned char SeqNum[4];        //顺序号，二进制
    unsigned char SwitchLabel[4];   //交换机的OPC标识，二进制
    unsigned char StartYear[2];     //起始年，BCD码
    unsigned char StartMonth[1];    //起始月，BCD码
    unsigned char StartDay[1];      //起始日，BCD码
    unsigned char StartHour[1];     //起始时，BCD码
    unsigned char StartMinute[1];   //起始分，BCD码
    unsigned char StartSecond[1];   //起始秒，BCD码
    unsigned char CallerZone[4];    //主叫区号，BCD向左对齐，主叫号码所在本地网的区号，
                                    //当系统涉及两个以上的本地网应用时，
                                    //可通过区号来区分不同本地网的话单
    unsigned char CallerNum[6];     //主叫号码，BCD向左对齐，不足位用16进制的E填充

    unsigned char Meter1[4];
    unsigned char Meter2[4];
    unsigned char Meter3[4];
    unsigned char Meter4[4];
    unsigned char Meter5[4];
    unsigned char Reserved[14];
};

//块的头记录
struct S1240GBCDRBlockHead
{
    /*
        数据块系列号：仅用于一个文件内。即在每次创建一个新文件时，该系列号被复位为0。
        二进制编码。取值范围为0全32767
    */
    unsigned char   BlockCsn[2];

    /*
        起始日期／时间：仅对联机输出的计费文件有意义，且仅用于每个文件的第一个数据块中。
        对于其他数据块用十六进制"EEEEEEEEEEEE"填充。采用二进制编码，其格式为YYYYMMDDHHMM。
        YYYY表示年份，取值1980-H' FFFF，MM表示月，取值01-12，DD表示日，取值01－31，HH表示小时，
        取值00一23，MM表示分钟，取值00一59
    */
    unsigned char   StartYear[2];       //起始年，二进制
    unsigned char   StartMonth[1];      //起始月，二进制
    unsigned char   StartDay[1];        //起始日，二进制
    unsigned char   StartHour[1];       //起始时，二进制
    unsigned char   StartMinute[1];     //起始分，二进制

    /*
        文件类型：在这里仅为一种文件类型，即详细活单，用 ASCll码'D'表示
    */
    unsigned char   FileType[1];        //文件类型

    /*
        交换机OPC；指明交换机自己的 OPC。仅对联机输出的计费文件有意义。
        其仅用于每个文件的第一个数据块中，对于其他数据块用十六进制"EEEEEE"填充。
        H进制编码，取值范围为0-16777215
    */
    unsigned char   OPC[3];

    /*
        数据记录类型：仅对联机输出的计费文件有意义，用来指明文件中数据记录的类型，
        取值同表1中的记录类型，用二进制代码表示
    */
    unsigned char   RecordType[1];

    /*
        实际块长度；仅对联机输出的计费文件有意义，用来指明在该数据块上实际有效数据的字书数，
        包括20个字节的头记录，但不包括数据块尾部的填充部分。二进制编码，取值范围0－2048
    */
    unsigned char   BlockLen[2];

    /*
        计费子系统标识符：仅对联机输出的计费文件有意义，且在每个文件的第一个数据块中才使用这个域。
        在其他数据块中填充十六进制"EE"，二进制编码，取值范围为0-255
    */
    unsigned char   AccountLabel[1];

    /*
        文件系列号：仅对联机输出的计费文件有意义。且在每个文件的第一个数据块中才使用这个域。
        在其他数据块中填充十六进制"EEEEEEEE"，二进制编码
    */
    unsigned char   FileSN[4];
};

//记录头
struct S1240GBCDRRecordHead
{
    unsigned char RecordType : 4;   //BCD，表示该话单记录的呼叫的类型
                                    //0:  计次表
                                    //1： PSTN IDD/DDD呼叫记录
                                    //2： DBO呼叫记录
                                    //3： IN呼叫记录
                                    //4： ISDN/CTX记录
                                    //5： TAX产生记录
                                    //6-7：保留
    unsigned char PartialFlg : 4;

    unsigned char SeqNum[4];        //顺序号，二进制

};





int ProcessS1240GBFile(const char*   pszSrcFile,
                       VECTOR<STRING>& FinalFileNameList,
                       VECTOR<int>&    CvtResultList,
                       const int bFileLevelVerify,
                       const int bBillLevelVerify,
                       void* pIBillProcessor,
                       const int nOrigBillLen)
{
    IBillProcessor* pBillProcessor = (IBillProcessor*)pIBillProcessor;

    FILE* pSrcFile = fopen(pszSrcFile, "rb");
    if (NULL == pSrcFile)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开话单文件(%s)失败",
               pszSrcFile);

        WRITELOG_F("打开话单文件(%s)失败",
                   pszSrcFile);

        return ERR_FAIL;
    }

    VECTOR<FILE*> ChlFiles;
    VECTOR<STRING>::iterator item = FinalFileNameList.begin();

    for (; item != FinalFileNameList.end(); item++)
    {
        ChlFiles.push_back(NULL);
    }

    //进行文件级校验。
    long nSrcFileSize = FileSize(pSrcFile);

    if (bFileLevelVerify)
    {
        if (nSrcFileSize % 2048 != 0)
        {
            if (!FileCopy(pszSrcFile, FinalFileNameList[0].c_str()))
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "话单文件(%s)拷贝到错误文件(%s)失败",
                       pszSrcFile,
                       FinalFileNameList[0].c_str());

                WRITELOG_F("话单文件(%s)拷贝到错误文件(%s)失败",
                           pszSrcFile,
                           FinalFileNameList[0].c_str());
            }
            else
            {
                CvtResultList[0] = 1;
            }

            fclose(pSrcFile);
            return ERR_FAIL;
        }
    }

    /*
     * 开始进行话单转换。
     */
    long nBlockNum = (nSrcFileSize + 2048 - 1) / 2048;

    char buf[2048];
    S1240GBCDRBlockHead* CDRBlock = (S1240GBCDRBlockHead*)buf;

    int nResult = ERR_SUCCESS;
    int nBlockIndex = 0;
    short nBlockLen = 0;
    int nSeqNum = -1;
    int nRet = ERR_SUCCESS;
    while (nBlockNum-- && (nRet == ERR_SUCCESS))
    {
        //读一个块的数据
        int nReadLen = fread(CDRBlock, 1, 2048, pSrcFile);
        if(nReadLen <= 0)
        {
            nBlockNum = 0;
            continue;
        }
        else if(nReadLen <= 20)
        {
            continue;
        }

        memcpy(&nBlockLen, &CDRBlock->BlockLen[0], 2);
        nBlockLen = NTOHS(nBlockLen);
        nBlockLen = min(nBlockLen, nReadLen);
        if(nBlockLen <= 20)
        {
            continue;
        }

        char invalid_flag[10];
        memset(&invalid_flag, 0xEE, min(nReadLen - 20, sizeof(invalid_flag)));
        if (memcmp(CDRBlock, &invalid_flag, sizeof(invalid_flag)) == 0)
        {
            continue;
        }

        int ReadByte    = 20;
        int RecordIndex = 0;
        int RecordTypeInHead  = ((S1240GBCDRRecordHead*)(&buf[20]))->RecordType;
        int RecordLenInHead   = 0;
        switch(RecordTypeInHead)
        {
        case 0: //1： Meter呼叫记录
            RecordLenInHead = 60;
            break;

        case 1: //1： PSTN IDD/DDD呼叫记录
            RecordLenInHead = 78;
            break;
        case 3: //3： IN呼叫记录
            RecordLenInHead = 160;
            break;
        case 4: //4： ISDN/CTX记录
            RecordLenInHead = 106;
            break;
        default:
            continue;
        }
        while (ReadByte <= nBlockLen - 1)//最小的话单长度为60字节
        {
            int RecordType = buf[ReadByte] >> 4;
            int RecordLen  = 0;
            switch(RecordType)
            {
            case 0: //1： Meter呼叫记录
                RecordLen = 60;
                break;

            case 1: //1： PSTN IDD/DDD呼叫记录
                RecordLen = 78;
                break;

            case 3: //3： IN呼叫记录
                RecordLen = 160;
                break;

            case 4: //4： ISDN/CTX记录
                RecordLen = 106;
                break;

            default:
                RecordLen = RecordLenInHead;
                break;
            }

            if (memcmp(&buf[ReadByte], &invalid_flag, min(nReadLen - 20, sizeof(invalid_flag))) == 0)
            {
                ReadByte += RecordLen;
                continue;
            }
            S1240GBCDRRecordHead* pRecordHead = (S1240GBCDRRecordHead*)&buf[ReadByte];
/*
            if(RecordType != pRecordHead->RecordType)
            {
                if (bBillLevelVerify)
                {
                    if (ChlFiles[0] == NULL)
                    {
                        ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                        if (ChlFiles[0] == NULL)
                        {
                            COMSGOUT(MSGOUT_LEVEL_URGENT,
                                   "打开文件(%s)失败",
                                   FinalFileNameList[0].c_str());

                            WRITELOG_F("打开文件(%s)失败",
                                       FinalFileNameList[0].c_str());
                        }
                        else
                        {
                            fwrite(&buf[ReadByte], RecordLen, 1, ChlFiles[0]);
                            CvtResultList[0] = 1;
                        }
                    }
                    else
                    {
                        fwrite(&buf[ReadByte], RecordLen, 1, ChlFiles[0]);
                        CvtResultList[0] = 1;
                    }
                }

                ReadByte += RecordLen;
                continue;
            }
*/
            if(ReadByte <= nBlockLen - 5)
            {
                //检查话单序列号连续
                int nCurSeqNum = 0;
                memcpy(&nCurSeqNum, &pRecordHead->SeqNum[0], sizeof(pRecordHead->SeqNum));
                nCurSeqNum = NTOHL(nCurSeqNum);

                if(nSeqNum != -1)
                {
                    if(nCurSeqNum != nSeqNum + 1 && bBillLevelVerify)
                    {
                        COMSGOUT(MSGOUT_LEVEL_URGENT,
                               "话单文件(%s)中话单序列号不连续(%d-%d)",
                               pszSrcFile,
                               nSeqNum,
                               nCurSeqNum);
                        WRITELOG_F("话单文件(%s)中话单序列号不连续(%d-%d)",
                                   pszSrcFile,
                                   nSeqNum,
                                   nCurSeqNum);
                    }
                }
                nSeqNum = nCurSeqNum;
            }


            //用格式库进行转换
            UINT4 uChlID;
            CFormat* pDestFmt = NULL;
            void* pDestBill = NULL;
            UINT4 uLen;
            int nReadRecordLen = min(RecordLen, nBlockLen - ReadByte);

            //获取分拣通道号和转换格式对象，并进行话单的处理
            pBillProcessor->Distribute(&buf[ReadByte],
                                       nReadRecordLen,
                                       uChlID,
                                       pDestFmt);

            if ((pDestFmt != NULL) && (uChlID != 0))
            {
                nResult = pDestFmt->ConvertABill(&buf[ReadByte],
                                                 nReadRecordLen,
                                                 pDestBill, uLen);
                if (ERR_SUCCESS == nResult)
                {
                    if (ChlFiles[uChlID] == NULL)
                    {
                        FILE* pFile = fopen(FinalFileNameList[uChlID].c_str(), "wb");
                        if (pFile == NULL)
                        {
                            COMSGOUT(MSGOUT_LEVEL_URGENT,
                                   "打开最终话单文件(%s)失败",
                                   FinalFileNameList[uChlID].c_str());

                            WRITELOG_F("打开最终话单文件(%s)失败",
                                       FinalFileNameList[uChlID].c_str());
                            delete pDestBill;
                            nRet = ERR_FAIL;
                            break;
                        }

                        ChlFiles[uChlID] = pFile;
                    }

                    fwrite(pDestBill, uLen, 1, ChlFiles[uChlID]);
                    CvtResultList[uChlID] = 1;

                    delete pDestBill;
                }
            }
            else
            {
                nResult = ERR_FAIL;
            }

            //如果转换不成功，将错误话单写入错误文件
            if (ERR_SUCCESS != nResult)
            {
                if (ChlFiles[0] == NULL)
                {
                    ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                    if (ChlFiles[0] == NULL)
                    {
                        COMSGOUT(MSGOUT_LEVEL_URGENT,
                               "打开文件(%s)失败",
                               FinalFileNameList[0].c_str());

                        WRITELOG_F("打开文件(%s)失败",
                                   FinalFileNameList[0].c_str());
                        nRet = ERR_FAIL;
                        break;
                    }
                }

                fwrite(&buf[ReadByte], nReadRecordLen, 1, ChlFiles[0]);
                CvtResultList[0] = 1;
            }

            ReadByte += nReadRecordLen;
        }

    }

    fclose(pSrcFile);
    pSrcFile = NULL;

    for(int j = 0; j < ChlFiles.size(); j++)
    {
        if (ChlFiles[j] != NULL)
        {
            fclose(ChlFiles[j]);
            ChlFiles[j] = NULL;
        }
    }

    return nRet;
}

