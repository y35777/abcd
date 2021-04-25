#if !defined(_FORMAT_DEFINED_H_)
#define _FORMAT_DEFINED_H_
/**************** The Head Comment of This File *********************/
// Description:  The #Define Head File of the Format Module
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


/************ Begin of The Resource Defining ************************/

//�������Ʊ�ʶ����
#define  VAR_DLL_LOAD  "DllLoad"    //����̬���ӿ⡱��ʶ
#define  VAR_DEFAULT   "Default"    //��ȱʡֵ����ʶ

//��ʽת���������Ʊ�ʶ����
#define  FUN_BLOCK_COPY                     "BlockCopy"      //�鿽������
#define  FUN_BLOCK_COPY_EX  "BlockCopyEx"    //�鿽��������֧��Intel/�����ֽ���ͬ���������Ļ��࿽�� ZhengYuqun 2004-04-28
#define  FUN_GET_BIN_YEAR                   "GetBinYear"     //��������λ��ת���ɶ�������λ�꺯��
#define  FUN_GET_TEXT_YEAR                  "GetTextYear"    //��������λ��ת�����ı���ʽ����λ�꺯��
#define  FUN_GET_TEXT_TIME                  "GetTextTime"    //��ʱ��ת���ɡ�YYYY-MM-DD HH:NN:SS����ʽ�ַ�������
#define  FUN_INT_TO_TEXT                    "Int2Text"       //����ת�����ı�����
#define  FUN_BCD_TO_STR                     "BCD2String"     //����BCD��ת�����ı�����
#define  FUN_BCD_TO_STR_2                   "BCD2String2"    //��n.5���ֽڵ�����BCD��ת�����ı�����
#define  FUN_RBCD_TO_STR                    "RBCD2String"    //����BCD��ת�����ı�����
#define  FUN_GET_ENUM_STR                   "GetEnumString"  //����ö��ֵ��ȡö���ַ�������
#define  FUN_SBCD_TO_STR                    "SBCD2String"    //����SBCD��ת�����ı�����
#define  FUN_ZXRBCD_TO_STR                  "ZXRBCD2String"  //���˻����еķ���BCD��ת�����ı�����

//added by zkq at 2004-09-28
#define  FUN_BCD_TO_BCD      "BCD2RBCD"      //��C&C08�����еļƷѺ����ɷ���BCD��Ϊ����BCD
//end

//Added by maimaoshi at 2002-05-18
#define  FUN_GET_TEXT_TIME_IN_BCD_T         "GetTextTimeInBCD_T"   //��6.5���ֽڵ�BCD������ʱ��ת���ɡ�YYYY-MM-DD HH:NN:SS.T����ʽ�ַ�������
#define  FUN_GET_TEXT_TIME_IN_BCD           "GetTextTimeInBCD"     //��BCD������ʱ��ת���ɡ�YYYY-MM-DD HH:NN:SS����ʽ�ַ�������
#define  FUN_GET_TEXT_TIME_IN_BCD_4Y        "GetTextTimeInBCD_4Y"  //��4λ���BCD������ʱ��ת���ɡ�YYYY-MM-DD HH:NN:SS����ʽ�ַ�������
#define  FUN_GET_TEXT_TIME_EX               "GetTextTimeEx"        //���ַ�������ʱ��ת���ɡ�YYYY-MM-DD HH:NN:SS����ʽ�ַ�������
#define  FUN_INT_TO_IP_TEXT                 "Int2IpText"           //��DWORD��IP��ַת��Ϊ�ַ���
#define  FUN_BCD_TO_SECOND                  "BCDToSecond"          //��DWORD��IP��ַת��Ϊ�ַ���
#define  FUN_GET_TEXT_TIME_NO_SP            "GetTextTimeNoSp"      //��ʱ��ת���ɡ�YYYYMMDDHHNNSS����ʽ�ַ�������
#define  FUN_GET_TEXT_FILLFF                "FillFF"               //�ѻ�����ȫ������FF
//End by maimaoshi at 2002-05-18
//Added by XDB on 2002.11.07
#define  FUN_BCD_NO_EXPLAIN                 "BCDNoExplain"         //��BCD��ת�����ַ�������������BCD���ַ��������ж��Ƿ����
#define  FUN_HBCD_TO_TEXT                   "HBCDToText"           //��BCD��ת�����ַ������������������С�ж��Ƿ����

//Added by ldf at 2003-01-08
#define  FUN_INT_TO_IP_TEXT_DIRECT          "Int2IpTextDirect"     //��DWORD��IP��ַת��Ϊ�ַ���
#define  FUN_ARRAY_TO_TEXT                  "Array2Text"           //������ת��Ϊ�ַ���
//End by ldf at 2003-01-08
/* Begin: Adder: Gan Zhiliang �������ַ������һ���ֶε�ת������  Date: 2003-3-29*/
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
//SoftX3000�ж���ҵ�����Ժͻ������͵������޸�
#define FUN_CHANGE_GSVN_AND_CALLTYPE        "ChangeGSVNandCallType"
#define FUN_CHANGE_GSVN_AND_CALLTYPE_TEXT   "ChangeGSVNandCallType2Text"
//End by zouyongsheng

//Added by z30971 at 2003-03-05
#define FUN_ARRAY_TO_HEX_STRING             "Array2HexString"
#define FUN_FILL_INVALID_VALUE              "FillInvaidValue"
#define FUN_GET_TEXT_TIME_EX                "GetTextTimeEx"
#define FUN_TRKGRP_TO_CALLNO                "TrkGrp2CallNo"
//ͨ��ʱ��ת����HHmmSS  ����ƽ 2004-02-14
#define FUN_DURATION_TO_HHMMSS              "Duration2HHmmSS"

//Int2Text(...)������ǿ�棬����Ϊȫ0xFF�ǣ�ת�ɿ��ַ���
#define FUN_INT_TO_TEXT_EX                  "Int2TextEx"

//���������������Ʊ�ʶ����
#define FUN_PREFIX                          "Prefix"      //��ȡ�绰����ǰnλ����
#define FUN_GET_AREA_ID_INSTRING            "GetAreaID"
#define  FUN_ISINNUMSEG                     "IsInNumSeg"  //�жϵ绰����ǰnλ�Ƿ�λ��ָ������ĺ���
#define  FUN_ISINENUM                       "IsInEnum"  //�жϵ绰����ǰnλ�Ƿ�λ��ָ������ĺ���

//Added by ZhengYuqun SWPD03631 2004-02-18
//MSoftX3000�Ļ�������ת��Ϊ2G�Ļ�������ʱ��Ҫ���ݻ������ͺ��м仰��ԭ��ֵ�����Ժ���������
#define FUN_CDRTYPE_2G      "CDRType2G"

//Added by ZhengYuqun SWPD05305 2004-05-19
//MSoftX3000��Diagnosticsת��Ϊ2G��Cause for teminationʱ��Ҫ�Զ���Ϣ�������⴦��
#define FUN_TERM_CAUSE_2G   "TermCause2G"

//Added by ZhengYuqun SWPD05305 2004-05-19
//MSoftX3000��Last MCCMNCת��Ϊ2G��Peer MNCʱ��Ҫ��2PLMN�л��������⴦��
#define FUN_PEER_MNC_2G     "PeerMnc2G"

//������Ϣ�е������к���ת��Ϊ�ı�  Added by ZhengYuqun
#define FUN_SMS_NUM_TO_TEXT    "SMSNum2Text"

//���䳤�ֶ������������ֶε��ֶγ��Ⱥ��ֶ�����ת��Ϊ�ı� Added by ZhengYuqun
#define FUN_VAR_LEN_TO_TEXT    "VarLen2Text"
#define FUN_VAR_VALUE_TO_TEXT  "VarValue2Text"
//��ñ䳤�ֶ������������ֶε��ֶγ��Ⱥ��ֶ����ݣ������� Added by ZhengYuqun SWPD06279 2004-06-26
#define FUN_GET_VAR_VALUE      "GetVarValue"
#define FUN_GET_VAR_LEN        "GetVarLen"

//����ƽ 2004-04-19 ���
#define FUN_PAD_NUMBER		"PadNumber"

#define  FMT_INI_APSection                  "AccessPoint"  //�����ļ��н�����������
#define  FMT_INI_APTYPE                     "APType"       //�����ļ��н��������������
#define  FMT_FIELD_MAX_LEN           500   //һ����������󳤶�  200��Ϊ500 ZhengYuqun SWPD02451 2003-11-18
#define  FMT_MAX_CALL_NUM_LEN        20    //�绰������󳤶�
//���Ⱥ 2004-05-27 ���
#define  FUN_EBCD_TO_STR         "EBCD2String"  //���ַ�E������
#define  FUN_BCD_TIME_TO_SEC     "BCDTime2Sec"   //ʱ�����BCD��ת����������ʾ���ַ���

//SWPD06703 Zheng Yuqun 2004-06-19
#define  FUN_BCD_TO_STR_BY_LEN   "BCD2StringByLen"   //���ݺ���ĳ�����ת��BCD��

#define  FUN_GET_ARG_SPEC_FIELD     "GetArgSpecField"

//����������ļ������򳤶ȶ���
#define  LEN_AP_NAME                 16    //��������Ƴ���
#define  LEN_AP_ORIG_FMT_NAME        16    //ԭʼ������ʽ���Ƴ���
#define  LEN_AP_ORIGBRS_FMT_NAME     16    //ԭʼ���������ʽ���Ƴ���

//ͨ�������ļ������򳤶ȶ���
#define  LEN_CHL_NAME                16    //ͨ�����Ƴ���
#define  LEN_CHL_FINAL_FMT_NAME      16    //���ջ�����ʽ���Ƴ���
#define  LEN_CHL_DIS_CONDITION       64    //�ּ���������
#define  LEN_CHL_FINALBRS_FMT_NAME   16    //���ջ��������ʽ���Ƴ���

//��ʽ�����ļ������򳤶ȶ���
#define  LEN_FMT_DESC                48    //��ʽ��������
#define  LEN_FMT_SRC_FMT_NAME        16    //ԭ��ʽ���Ƴ���
#define  LEN_FMT_DIS_CONDITION       64    //��ʽʶ����������


/****************** The 3G And GPRS Bill Format Defining *********************/

//����WCDMA���ֻ�����ʽ��
#define W_TMP_CDR_NAME          "3GWCDMA_T1"       //���л������͸�ʽ
#define W_ALL_CDR_NAME          W_TMP_CDR_NAME

//����3G���ֻ�����ʽ��
#define S_CDR_3GFORMATNAME      ("3G-SGSNPDP")       //ԭʼ�������͵�S-CDR
#define G_CDR_3GFORMATNAME      ("3G-GGSNPDP")       //ԭʼ�������͵�G-CDR
#define M_CDR_3GFORMATNAME      ("3G-SGSNMM")        //ԭʼ�������͵�M-CDR
#define S_SMO_CDR_3GFORMATNAME  ("3G-SGSNSMO")       //S_SMO_CDR
#define S_SMT_CDR_3GFORMATNAME  ("3G-SGSNSMT")       //S_SMT_CDR
#define L_S_CDR_3GFORMATNAME    ("3G-LARGESGSNPDP")  //���ջ������͵�S-CDR
#define L_G_CDR_3GFORMATNAME    ("3G-LARGEGGSNPDP")  //���ջ������͵�G-CDR
#define L_M_CDR_3GFORMATNAME    ("3G-LARGESGSNMM")   //���ջ������͵�M-CDR

//����GPRS���ֻ�����ʽ��
#define TMP_CDR_NAME    "GPRS_T1"
#define ALL_CDR_NAME    TMP_CDR_NAME

//����GPRS��ʽ���Ķ���
#define S_CDR_GPRSFORMATNAME        ("SGSNPDP")//ԭʼ�������͵�S-CDR
#define G_CDR_GPRSFORMATNAME        ("GGSNPDP")//ԭʼ�������͵�G-CDR
#define M_CDR_GPRSFORMATNAME        ("SGSNMM")//ԭʼ�������͵�M-CDR
#define S_SMO_CDR_GPRSFORMATNAME    ("SGSNSMO")//S_SMO_CDR
#define S_SMT_CDR_GPRSFORMATNAME    ("SGSNSMT")//S_SMT_CDR
#define L_S_CDR_GPRSFORMATNAME      ("LARGESGSNPDP")//���ջ������͵�S-CDR
#define L_G_CDR_GPRSFORMATNAME      ("LARGEGGSNPDP")//���ջ������͵�G-CDR
//#define L_M_CDR_GPRSFORMATNAME    ("LARGESGSNMM")//���ջ������͵�M-CDR

//����3G��GPRS���ֻ�����ʽ������
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


//����3G��GPRS���ֻ�����ʽ�ı�ʶ
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


/* Begin: ����3GR4���ֻ�����ʽ�ĸ�ʽ���� Adder: Gan Zhiliang   Date: 2003-6-27*/
#define S_CDR_R4FORMATNAME      ("r4-SGSNPDP")       //ԭʼ�������͵�S-CDR
#define G_CDR_R4FORMATNAME      ("r4-GGSNPDP")       //ԭʼ�������͵�G-CDR
#define M_CDR_R4FORMATNAME      ("r4-SGSNMM")        //ԭʼ�������͵�M-CDR
#define S_SMO_CDR_R4FORMATNAME  ("r4-SGSNSMO")       //S_SMO_CDR
#define S_SMT_CDR_R4FORMATNAME  ("r4-SGSNSMT")       //S_SMT_CDR
#define LCS_MT_CDR_R4FORMATNAME ("r4-LCSMT")         //LCS-MT-CDR
#define LCS_MO_CDR_R4FORMATNAME ("r4-LCSMO")         //LCS-MO-CDR
#define LCS_NI_CDR_R4FORMATNAME ("r4-LCSNI")         //LCS-NI-CDR
#define L_S_CDR_R4FORMATNAME    ("r4-LARGESGSNPDP")  //���ջ������͵�S-CDR
#define L_G_CDR_R4FORMATNAME    ("r4-LARGEGGSNPDP")  //���ջ������͵�G-CDR
#define L_M_CDR_R4FORMATNAME    ("r4-LARGESGSNMM")   //���ջ������͵�M-CDR

/* End  : Adder: Gan Zhiliang   Date: 2003-6-27*/

//����3G��GPRS����ͨ������
#define CHL_ALL  "CHL_1"

/****************** End of The 3G Bill Format Defining **************/


/************ End of The Resource Defining **************************/


#endif  //!defined(_FORMAT_DEFINED_H_)
