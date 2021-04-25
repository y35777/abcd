#ifndef __XML_TAG_DEFS_H__
#define __XML_TAG_DEFS_H__


//¸æ¾¯Ä£¿é¶¨Òå
/***********************************************************************************
                          ¸æ¾¯ÅäÖÃÎÄ¼þAlarmCofig.xml¸ñÊ½
 -----------------------------------------------------------------------------------
<?xml version="1.0" encoding="ISO8859-1" standalone="no" ?> 
<AlarmConfig>
    <SAlarmExplain ID="1" Type="0" Name="AlarmName" Level="1" FuncID="0" SubSysID="1" SendToOMC="1" SendToBox="1" ParaLen="1024" Detail="Detail information of alarm" FixAdvice="Advice inforation of restore this alarm" ParaExp="The expain information of the parameter" ParaNum="1" /> 
    <SAlarmExplain ID="2" Type="0" Name="AlarmName" Level="1" FuncID="0" SubSysID="1" SendToOMC="1" SendToBox="1" ParaLen="1024" Detail="Detail information of alarm" FixAdvice="Advice inforation of restore this alarm" ParaExp="The expain information of the parameter" ParaNum="1" /> 
</AlarmConfig>
 -----------------------------------------------------------------------------------

***********************************************************************************/
#define XMLTAG_ALARMCFG_ROOT              "AlarmConfig"
#define XMLTAG_ALARMCFG_SALARMEXPLAIN     "SAlarmExplain"
#define XMLTAG_ALARMCFG_ID                "ID" 
#define XMLTAG_ALARMCFG_TYPE              "Type" 
#define XMLTAG_ALARMCFG_NAME              "Name"
#define XMLTAG_ALARMCFG_LEVEL             "Level"
#define XMLTAG_ALARMCFG_FUNCID            "FuncID" 
#define XMLTAG_ALARMCFG_SUBSYSID          "SubSysID"
#define XMLTAG_ALARMCFG_SENDTOOMC         "SendToOMC"
#define XMLTAG_ALARMCFG_SENDTOBOX         "SendToBox"
#define XMLTAG_ALARMCFG_PARALEN           "ParaLen"
#define XMLTAG_ALARMCFG_DETAIL            "Detail"
#define XMLTAG_ALARMCFG_FIXADVICE         "FixAdvice"
#define XMLTAG_ALARMCFG_PARAEXP           "ParaExp"
#define XMLTAG_ALARMCFG_PARANUM           "ParaNum"  



#endif //__XML_TAG_DEFS_H__
