#if !defined(_FORMAT_DEFINED_H_)
#define _FORMAT_DEFINED_H_
/**************** The Head Comment of This File *********************/
// Description:  The #Define Head File of the Format Module
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


/************ Begin of The Resource Defining ************************/

//变量名称标识定义
#define  VAR_DLL_LOAD  "DllLoad"    //“动态链接库”标识
#define  VAR_DEFAULT   "Default"    //“缺省值”标识

//格式转换函数名称标识定义
#define  FUN_BLOCK_COPY                     "BlockCopy"      //块拷贝函数
#define  FUN_BLOCK_COPY_EX  "BlockCopyEx"    //块拷贝函数，支持Intel/网络字节序不同长度整数的互相拷贝 ZhengYuqun 2004-04-28
#define  FUN_GET_BIN_YEAR                   "GetBinYear"     //二进制两位年转换成二进制四位年函数
#define  FUN_GET_TEXT_YEAR                  "GetTextYear"    //二进制两位年转换成文本格式的四位年函数
#define  FUN_GET_TEXT_TIME                  "GetTextTime"    //把时间转换成“YYYY-MM-DD HH:NN:SS”格式字符串函数
#define  FUN_INT_TO_TEXT                    "Int2Text"       //整数转换成文本函数
#define  FUN_BCD_TO_STR                     "BCD2String"     //正向BCD码转换成文本函数
#define  FUN_BCD_TO_STR_2                   "BCD2String2"    //将n.5个字节的正向BCD码转换成文本函数
#define  FUN_RBCD_TO_STR                    "RBCD2String"    //反向BCD码转换成文本函数
#define  FUN_GET_ENUM_STR                   "GetEnumString"  //根据枚举值获取枚举字符串函数
#define  FUN_SBCD_TO_STR                    "SBCD2String"    //正向SBCD码转换成文本函数
#define  FUN_ZXRBCD_TO_STR                  "ZXRBCD2String"  //中兴话单中的反向BCD码转换成文本函数

//added by zkq at 2004-09-28
#define  FUN_BCD_TO_BCD      "BCD2RBCD"      //把C&C08话单中的计费号码由反向BCD改为正向BCD
//end

//Added by maimaoshi at 2002-05-18
#define  FUN_GET_TEXT_TIME_IN_BCD_T         "GetTextTimeInBCD_T"   //把6.5个字节的BCD的日期时间转换成“YYYY-MM-DD HH:NN:SS.T”格式字符串函数
#define  FUN_GET_TEXT_TIME_IN_BCD           "GetTextTimeInBCD"     //把BCD的日期时间转换成“YYYY-MM-DD HH:NN:SS”格式字符串函数
#define  FUN_GET_TEXT_TIME_IN_BCD_4Y        "GetTextTimeInBCD_4Y"  //把4位年的BCD的日期时间转换成“YYYY-MM-DD HH:NN:SS”格式字符串函数
#define  FUN_GET_TEXT_TIME_EX               "GetTextTimeEx"        //把字符的日期时间转换成“YYYY-MM-DD HH:NN:SS”格式字符串函数
#define  FUN_INT_TO_IP_TEXT                 "Int2IpText"           //把DWORD的IP地址转换为字符串
#define  FUN_BCD_TO_SECOND                  "BCDToSecond"          //把DWORD的IP地址转换为字符串
#define  FUN_GET_TEXT_TIME_NO_SP            "GetTextTimeNoSp"      //把时间转换成“YYYYMMDDHHNNSS”格式字符串函数
#define  FUN_GET_TEXT_FILLFF                "FillFF"               //把话单域全部填上FF
//End by maimaoshi at 2002-05-18
//Added by XDB on 2002.11.07
#define  FUN_BCD_NO_EXPLAIN                 "BCDNoExplain"         //将BCD码转换成字符串函数，根据BCD码字符串长度判断是否结束
#define  FUN_HBCD_TO_TEXT                   "HBCDToText"           //将BCD码转换成字符串函数，根据数组大小判断是否结束

//Added by ldf at 2003-01-08
#define  FUN_INT_TO_IP_TEXT_DIRECT          "Int2IpTextDirect"     //把DWORD的IP地址转换为字符串
#define  FUN_ARRAY_TO_TEXT                  "Array2Text"           //把数组转换为字符串
//End by ldf at 2003-01-08
/* Begin: Adder: Gan Zhiliang 增加用字符串填充一个字段的转换函数  Date: 2003-3-29*/
#define FUN_FILL_WITH_STR                   "FillWithStr"
/* End  : Adder: Gan Zhiliang   Date: 2003-3-29*/

#define FUN_FILL_BY_INI_FILE                "FillByIniFile"

//Added by maimaoshi at 2003-03-10
#define FUN_TO_LRLN                         "ToLRLN"
#define FUN_TO_LR                           "ToLR"
#define FUN_TO_LN                           "ToLN"
#define FUN_STRING_COPY                     "StringCopy"
#define FUN_BYTE_TEXT2                      "ByteText2"
#define FUN_GET_TEXT_TIME_INSTRING          "GetTextTimeInString"
//End by maimaoshi at 2003-03-10
//by ldf 2003-04-01
#define FUN_CHECK_SUM                       "CheckSum"
//end
#define FUN_LINK_TEXT                       "LinkText"
#define FUN_GET_RECORDTYPE                  "GetRecordType"
#define FUN_GET_SUBCATEGORY                 "GetSubCategory"
#define FUN_ISUP_CHARGE_NUMBER              "ISUPChargeNumber"
#define FUN_ISUP_CHARGE_NUMBER_NOA          "ISUPChargeNumberNOA"
#define FUN_CONVERT10MS                     "Convert10ms"
#define FUN_VIDEOTIME                       "VideoTime"
#define FUN_VIDEODURATION                   "VideoDuration"
#define FUN_ADD                             "Add"
//Added by ZhengYuqun 2003-08-27 SWPD01198
#define FUN_PAD_AREACODE                    "PadAreacode"

#define FUN_REPLACE_PREFIX                  "ReplacePrefix"

//added by lucy 2004-06-25
#define FUN_REPLACE_NUM_BY_STR              "ReplaceNumByStr"

//Added by zouyongsheng at 2002-09-05
//SoftX3000中对于业务属性和话单类型的特殊修改
#define FUN_CHANGE_GSVN_AND_CALLTYPE        "ChangeGSVNandCallType"
#define FUN_CHANGE_GSVN_AND_CALLTYPE_TEXT   "ChangeGSVNandCallType2Text"
//End by zouyongsheng

//Added by z30971 at 2003-03-05
#define FUN_ARRAY_TO_HEX_STRING             "Array2HexString"
#define FUN_FILL_INVALID_VALUE              "FillInvaidValue"
#define FUN_GET_TEXT_TIME_EX                "GetTextTimeEx"
#define FUN_TRKGRP_TO_CALLNO                "TrkGrp2CallNo"
//通话时长转换成HHmmSS  邱意平 2004-02-14
#define FUN_DURATION_TO_HHMMSS              "Duration2HHmmSS"

//Int2Text(...)函数增强版，数字为全0xFF是，转成空字符串
#define FUN_INT_TO_TEXT_EX                  "Int2TextEx"

//查找条件函数名称标识定义
#define FUN_PREFIX                          "Prefix"      //获取电话号码前n位函数
#define FUN_GET_AREA_ID_INSTRING            "GetAreaID"
#define  FUN_ISINNUMSEG                     "IsInNumSeg"  //判断电话号码前n位是否位于指定区间的函数
#define  FUN_ISINENUM                       "IsInEnum"  //判断电话号码前n位是否位于指定区间的函数

//Added by ZhengYuqun SWPD03631 2004-02-18
//MSoftX3000的话单类型转换为2G的话单类型时需要根据话单类型和中间话单原因值生成试呼话单类型
#define FUN_CDRTYPE_2G      "CDRType2G"

//Added by ZhengYuqun SWPD05305 2004-05-19
//MSoftX3000的Diagnostics转换为2G的Cause for temination时需要对短消息话单特殊处理
#define FUN_TERM_CAUSE_2G   "TermCause2G"

//Added by ZhengYuqun SWPD05305 2004-05-19
//MSoftX3000的Last MCCMNC转换为2G的Peer MNC时需要对2PLMN切换话单特殊处理
#define FUN_PEER_MNC_2G     "PeerMnc2G"

//将短消息中的主被叫号码转换为文本  Added by ZhengYuqun
#define FUN_SMS_NUM_TO_TEXT    "SMSNum2Text"

//将变长字段中所包含的字段的字段长度和字段内容转换为文本 Added by ZhengYuqun
#define FUN_VAR_LEN_TO_TEXT    "VarLen2Text"
#define FUN_VAR_VALUE_TO_TEXT  "VarValue2Text"
//获得变长字段中所包含的字段的字段长度和字段内容，二进制 Added by ZhengYuqun SWPD06279 2004-06-26
#define FUN_GET_VAR_VALUE      "GetVarValue"
#define FUN_GET_VAR_LEN        "GetVarLen"

//邱意平 2004-04-19 添加
#define FUN_PAD_NUMBER		"PadNumber"

#define  FMT_INI_APSection                  "AccessPoint"  //配置文件中接入点的区域名
#define  FMT_INI_APTYPE                     "APType"       //配置文件中接入点类型项名称
#define  FMT_FIELD_MAX_LEN           500   //一个话单域最大长度  200改为500 ZhengYuqun SWPD02451 2003-11-18
#define  FMT_MAX_CALL_NUM_LEN        20    //电话号码最大长度
//左克群 2004-05-27 添加
#define  FUN_EBCD_TO_STR         "EBCD2String"  //以字符E结束符
#define  FUN_BCD_TIME_TO_SEC     "BCDTime2Sec"   //时分秒的BCD码转换成以秒显示的字符串

//SWPD06703 Zheng Yuqun 2004-06-19
#define  FUN_BCD_TO_STR_BY_LEN   "BCD2StringByLen"   //根据号码的长度来转换BCD码

#define  FUN_GET_ARG_SPEC_FIELD     "GetArgSpecField"

//接入点配置文件各个域长度定义
#define  LEN_AP_NAME                 16    //接入点名称长度
#define  LEN_AP_ORIG_FMT_NAME        16    //原始话单格式名称长度
#define  LEN_AP_ORIGBRS_FMT_NAME     16    //原始话单浏览格式名称长度

//通道配置文件各个域长度定义
#define  LEN_CHL_NAME                16    //通道名称长度
#define  LEN_CHL_FINAL_FMT_NAME      16    //最终话单格式名称长度
#define  LEN_CHL_DIS_CONDITION       64    //分拣条件长度
#define  LEN_CHL_FINALBRS_FMT_NAME   16    //最终话单浏览格式名称长度

//格式配置文件各个域长度定义
#define  LEN_FMT_DESC                48    //格式描述长度
#define  LEN_FMT_SRC_FMT_NAME        16    //原格式名称长度
#define  LEN_FMT_DIS_CONDITION       64    //格式识别条件长度


/****************** The 3G And GPRS Bill Format Defining *********************/

//定义WCDMA各种话单格式名
#define W_TMP_CDR_NAME          "3GWCDMA_T1"       //所有话单类型格式
#define W_ALL_CDR_NAME          W_TMP_CDR_NAME

//定义3G各种话单格式名
#define S_CDR_3GFORMATNAME      ("3G-SGSNPDP")       //原始话单类型的S-CDR
#define G_CDR_3GFORMATNAME      ("3G-GGSNPDP")       //原始话单类型的G-CDR
#define M_CDR_3GFORMATNAME      ("3G-SGSNMM")        //原始话单类型的M-CDR
#define S_SMO_CDR_3GFORMATNAME  ("3G-SGSNSMO")       //S_SMO_CDR
#define S_SMT_CDR_3GFORMATNAME  ("3G-SGSNSMT")       //S_SMT_CDR
#define L_S_CDR_3GFORMATNAME    ("3G-LARGESGSNPDP")  //最终话单类型的S-CDR
#define L_G_CDR_3GFORMATNAME    ("3G-LARGEGGSNPDP")  //最终话单类型的G-CDR
#define L_M_CDR_3GFORMATNAME    ("3G-LARGESGSNMM")   //最终话单类型的M-CDR

//定义GPRS各种话单格式名
#define TMP_CDR_NAME    "GPRS_T1"
#define ALL_CDR_NAME    TMP_CDR_NAME

//关于GPRS格式名的定义
#define S_CDR_GPRSFORMATNAME        ("SGSNPDP")//原始话单类型的S-CDR
#define G_CDR_GPRSFORMATNAME        ("GGSNPDP")//原始话单类型的G-CDR
#define M_CDR_GPRSFORMATNAME        ("SGSNMM")//原始话单类型的M-CDR
#define S_SMO_CDR_GPRSFORMATNAME    ("SGSNSMO")//S_SMO_CDR
#define S_SMT_CDR_GPRSFORMATNAME    ("SGSNSMT")//S_SMT_CDR
#define L_S_CDR_GPRSFORMATNAME      ("LARGESGSNPDP")//最终话单类型的S-CDR
#define L_G_CDR_GPRSFORMATNAME      ("LARGEGGSNPDP")//最终话单类型的G-CDR
//#define L_M_CDR_GPRSFORMATNAME    ("LARGESGSNMM")//最终话单类型的M-CDR

//定义3G和GPRS各种话单格式的描述
#define S_CDR_DESC          "S-CDR"        //S-CDR
#define G_CDR_DESC          "G-CDR"        //G-CDR
#define M_CDR_DESC          "M-CDR"        //M-CDR
#define S_SMO_CDR_DESC      "S-SMO-CDR"    //S-SMO-CDR
#define S_SMT_CDR_DESC      "S-SMT-CDR"    //S-SMT-CDR
#define S_LCS_MT_CDR_DESC   "LCS-MT-CDR"
#define S_LCO_MT_CDR_DESC   "LCS-MO-CDR"
#define S_LCN_MT_CDR_DESC   "LCS-NI-CDR"
#define L_S_CDR_DESC        "L-S-CDR"      //L-S-CDR
#define L_G_CDR_DESC        "L-G-CDR"      //L-G-CDR
#define L_M_CDR_DESC        "L-M-CDR"      //L-M-CDR


//定义3G和GPRS各种话单格式的标识
#define S_CDR_FORMATTYPE        1    //S-CDR
#define G_CDR_FORMATTYPE        2    //G-CDR
#define M_CDR_FORMATTYPE        3    //M-CDR
#define S_SMO_CDR_FORMATTYPE    4    //S-SMO-CDR
#define S_SMT_CDR_FORMATTYPE    5    //S-SMT-CDR
#define LCS_MT_CDR              6
#define LCS_MO_CDR              7
#define LCS_NI_CDR              8
#define Q_LCS_MT_CDR            21
#define Q_LCS_MO_CDR            22
#define Q_LCS_NI_CDR            23
#define L_S_CDR_FORMATTYPE      27   //L_S_CDR
#define L_G_CDR_FORMATTYPE      28   //L_G_CDR
#define L_M_CDR_FORMATTYPE      29   //L_M_CDR


/* Begin: 定义3GR4各种话单格式的格式名称 Adder: Gan Zhiliang   Date: 2003-6-27*/
#define S_CDR_R4FORMATNAME      ("r4-SGSNPDP")       //原始话单类型的S-CDR
#define G_CDR_R4FORMATNAME      ("r4-GGSNPDP")       //原始话单类型的G-CDR
#define M_CDR_R4FORMATNAME      ("r4-SGSNMM")        //原始话单类型的M-CDR
#define S_SMO_CDR_R4FORMATNAME  ("r4-SGSNSMO")       //S_SMO_CDR
#define S_SMT_CDR_R4FORMATNAME  ("r4-SGSNSMT")       //S_SMT_CDR
#define LCS_MT_CDR_R4FORMATNAME ("r4-LCSMT")         //LCS-MT-CDR
#define LCS_MO_CDR_R4FORMATNAME ("r4-LCSMO")         //LCS-MO-CDR
#define LCS_NI_CDR_R4FORMATNAME ("r4-LCSNI")         //LCS-NI-CDR
#define L_S_CDR_R4FORMATNAME    ("r4-LARGESGSNPDP")  //最终话单类型的S-CDR
#define L_G_CDR_R4FORMATNAME    ("r4-LARGEGGSNPDP")  //最终话单类型的G-CDR
#define L_M_CDR_R4FORMATNAME    ("r4-LARGESGSNMM")   //最终话单类型的M-CDR

/* End  : Adder: Gan Zhiliang   Date: 2003-6-27*/

//定义3G和GPRS话单通道名称
#define CHL_ALL  "CHL_1"

/****************** End of The 3G Bill Format Defining **************/


/************ End of The Resource Defining **************************/


#endif  //!defined(_FORMAT_DEFINED_H_)
