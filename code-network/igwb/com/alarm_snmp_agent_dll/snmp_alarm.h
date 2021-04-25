//
// AgentGen Standard v1.5  Copyright (C) Frank Fock, All rights reserved
// Not for use in a commercial environment
//


#ifndef _huawei_snmpalarm_mib_h
#define _huawei_snmpalarm_mib_h
#ifdef _PLATFORM_WIN32
#pragma warning(disable:4786)
#endif

#include "./resource.h"
#include "agent_define.h"

#include <mib.h>
#include <snmp_textual_conventions.h>
#include <notification_originator.h>
#include "log.h"





#define oidAlarmQueryTable               "1.3.6.1.4.1.2011.2.9.1"
#define oidAlarmQueryEntry               "1.3.6.1.4.1.2011.2.9.1.1"
#define oidQueryNmsName                  "1.3.6.1.4.1.2011.2.9.1.1.1"
#define colQueryNmsName                  "1"
#define oidQueryType                     "1.3.6.1.4.1.2011.2.9.1.1.2"
#define colQueryType                     "2"
#define oidQueryRestoreFlag              "1.3.6.1.4.1.2011.2.9.1.1.3"
#define colQueryRestoreFlag              "3"
#define oidQueryAckFlag                  "1.3.6.1.4.1.2011.2.9.1.1.4"
#define colQueryAckFlag                  "4"
#define oidQueryModuleNumber             "1.3.6.1.4.1.2011.2.9.1.1.5"
#define colQueryModuleNumber             "5"
#define oidQueryCsnScope                 "1.3.6.1.4.1.2011.2.9.1.1.6"
#define colQueryCsnScope                 "6"
#define oidQueryIDScope                  "1.3.6.1.4.1.2011.2.9.1.1.7"
#define colQueryIDScope                  "7"
#define oidQueryLevel                    "1.3.6.1.4.1.2011.2.9.1.1.8"
#define colQueryLevel                    "8"
#define oidQuerySubsystemID              "1.3.6.1.4.1.2011.2.9.1.1.9"
#define colQuerySubsystemID              "9"
#define oidQueryDateScope                "1.3.6.1.4.1.2011.2.9.1.1.10"
#define colQueryDateScope                "10"
#define oidQueryTimeScope                "1.3.6.1.4.1.2011.2.9.1.1.11"
#define colQueryTimeScope                "11"
#define oidReturnCsn                     "1.3.6.1.4.1.2011.2.9.1.1.12"
#define colReturnCsn                     "12"
#define oidQueryRowStatus                "1.3.6.1.4.1.2011.2.9.1.1.13"
#define colQueryRowStatus                "13"
#define oidAlarmCsn                      "1.3.6.1.4.1.2011.2.9.2.0"
#define oidAlarmID                       "1.3.6.1.4.1.2011.2.9.3.0"
#define oidAlarmType                     "1.3.6.1.4.1.2011.2.9.4.0"
#define oidAlarmLevel                    "1.3.6.1.4.1.2011.2.9.5.0"
#define oidAlarmModuleNumber             "1.3.6.1.4.1.2011.2.9.6.0"
#define oidFunctionID                    "1.3.6.1.4.1.2011.2.9.7.0"
#define oidAlarmSubsystemID              "1.3.6.1.4.1.2011.2.9.8.0"
#define oidAlarmDate                     "1.3.6.1.4.1.2011.2.9.9.0"
#define oidCpuPosition                   "1.3.6.1.4.1.2011.2.9.10.0"
#define oidProcesID                      "1.3.6.1.4.1.2011.2.9.11.0"
#define oidAlarmParameter                "1.3.6.1.4.1.2011.2.9.12.0"
#define oidAlarmRestoreType              "1.3.6.1.4.1.2011.2.9.15.0"
#define oidRestoreDate                   "1.3.6.1.4.1.2011.2.9.16.0"
#define oidAcknowledgementOperator       "1.3.6.1.4.1.2011.2.9.17.0"
#define oidAcknowledgementDate           "1.3.6.1.4.1.2011.2.9.18.0"
#define oidQueryAlarm                    "1.3.6.1.4.1.2011.2.9.30.0"
#define oidErrnoInformation              "1.3.6.1.4.1.2011.2.9.31.0"
#define oidErrorTrap                     "1.3.6.1.4.1.2011.2.9.32"
#define oidAlarmTrap                     "1.3.6.1.4.1.2011.2.9.33"
#define oidNmsTable                      "1.3.6.1.4.1.2011.2.9.37"
#define oidNmsEntry                      "1.3.6.1.4.1.2011.2.9.37.1"
#define oidNmsName                       "1.3.6.1.4.1.2011.2.9.37.1.1"
#define colNmsName                       "1"
#define oidNmsIP                         "1.3.6.1.4.1.2011.2.9.37.1.2"
#define colNmsIP                         "2"
#define oidNmsPort                       "1.3.6.1.4.1.2011.2.9.37.1.3"
#define colNmsPort                       "3"
#define oidNmsRowStatus                  "1.3.6.1.4.1.2011.2.9.37.1.4"
#define colNmsRowStatus                  "4"

#define oidHuawei                        "1.3.6.1.4.1.2011"
//--AgentGen BEGIN=_INCLUDE
//--AgentGen END


/**
 *  queryType
 *
 * "Description."
 */


class queryType: public MibLeaf {

public:
    queryType(const Oidx&);
    virtual ~queryType();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=queryType
//--AgentGen END
};


/**
 *  queryRestoreFlag
 *
 * "Description."
 */


class queryRestoreFlag: public MibLeaf {

public:
    queryRestoreFlag(const Oidx&);
    virtual ~queryRestoreFlag();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=queryRestoreFlag
//--AgentGen END
};


/**
 *  queryAckFlag
 *
 * "Description."
 */


class queryAckFlag: public MibLeaf {

public:
    queryAckFlag(const Oidx&);
    virtual ~queryAckFlag();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=queryAckFlag
//--AgentGen END
};


/**
 *  queryModuleNumber
 *
 * "Description."
 */


class queryModuleNumber: public MibLeaf {

public:
    queryModuleNumber(const Oidx&);
    virtual ~queryModuleNumber();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual long        get_state();
    virtual void        set_state(long);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    int m_nVal;
//--AgentGen BEGIN=queryModuleNumber
//--AgentGen END
};


/**
 *  queryCsnScope
 *
 * "Description."
 */


class queryCsnScope: public MibLeaf {

public:
    queryCsnScope(const Oidx&);
    virtual ~queryCsnScope();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=queryCsnScope
//--AgentGen END
};


/**
 *  queryIDScope
 *
 * "Description."
 */


class queryIDScope: public MibLeaf {

public:
    queryIDScope(const Oidx&);
    virtual ~queryIDScope();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=queryIDScope
//--AgentGen END
};


/**
 *  queryLevel
 *
 * "Description."
 */


class queryLevel: public MibLeaf {

public:
    queryLevel(const Oidx&);
    virtual ~queryLevel();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=queryLevel
//--AgentGen END
};


/**
 *  querySubsystemID
 *
 * "Description."
 */


class querySubsystemID: public MibLeaf {

public:
    querySubsystemID(const Oidx&);
    virtual ~querySubsystemID();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=querySubsystemID
//--AgentGen END
};


/**
 *  queryDateScope
 *
 * "Description."
 */


class queryDateScope: public MibLeaf {

public:
    queryDateScope(const Oidx&);
    virtual ~queryDateScope();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=queryDateScope
//--AgentGen END
};


/**
 *  queryTimeScope
 *
 * "Description."
 */


class queryTimeScope: public MibLeaf {

public:
    queryTimeScope(const Oidx&);
    virtual ~queryTimeScope();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=queryTimeScope
//--AgentGen END
};


/**
 *  returnCsn
 *
 * "Description."
 */


class returnCsn: public MibLeaf {

public:
    returnCsn(const Oidx&);
    virtual ~returnCsn();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual long        get_state();
    virtual void        set_state(long);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    int m_nVal;
//--AgentGen BEGIN=returnCsn
//--AgentGen END
};


/**
 *  queryRowStatus
 *
 * "Description."
 */


class queryRowStatus: public snmpRowStatus {

public:
    queryRowStatus(const Oidx&);
    virtual ~queryRowStatus();

    virtual MibEntryPtr clone();
    virtual long        get_state();
    virtual void        set_state(long);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
//--AgentGen BEGIN=queryRowStatus
//--AgentGen END
};


/**
 *  alarmCsn
 *
 * "Description."
 */


class alarmCsn: public MibLeaf {

public:
    alarmCsn();
    virtual ~alarmCsn();

    static alarmCsn* s_pInstance;

    virtual void        get_request(Request*, int);
    virtual long        get_state();
    virtual void        set_state(long);

//--AgentGen BEGIN=alarmCsn
//--AgentGen END
};


/**
 *  alarmID
 *
 * "Description."
 */


class alarmID: public MibLeaf {

public:
    alarmID();
    virtual ~alarmID();

    static alarmID* s_pInstance;

    virtual void        get_request(Request*, int);
    virtual long        get_state();
    virtual void        set_state(long);

//--AgentGen BEGIN=alarmID
//--AgentGen END
};


/**
 *  alarmType
 *
 * "Description."
 */


class alarmType: public MibLeaf {

public:
    alarmType();
    virtual ~alarmType();

    static alarmType* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=alarmType
//--AgentGen END
};


/**
 *  alarmLevel
 *
 * "Description."
 */


class alarmLevel: public MibLeaf {

public:
    alarmLevel();
    virtual ~alarmLevel();

    static alarmLevel* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=alarmLevel
//--AgentGen END
};


/**
 *  alarmModuleNumber
 *
 * "Description."
 */


class alarmModuleNumber: public MibLeaf {

public:
    alarmModuleNumber();
    virtual ~alarmModuleNumber();

    static alarmModuleNumber* s_pInstance;

    virtual void        get_request(Request*, int);
    virtual long        get_state();
    virtual void        set_state(long);

//--AgentGen BEGIN=alarmModuleNumber
//--AgentGen END
};


/**
 *  functionID
 *
 * "Description."
 */


class functionID: public MibLeaf {

public:
    functionID();
    virtual ~functionID();

    static functionID* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=functionID
//--AgentGen END
};


/**
 *  alarmSubsystemID
 *
 * "Description."
 */


class alarmSubsystemID: public MibLeaf {

public:
    alarmSubsystemID();
    virtual ~alarmSubsystemID();

    static alarmSubsystemID* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=alarmSubsystemID
//--AgentGen END
};


/**
 *  alarmDate
 *
 * "Description."
 */


class alarmDate: public MibLeaf {

public:
    alarmDate();
    virtual ~alarmDate();

    static alarmDate* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=alarmDate
//--AgentGen END
};


/**
 *  cpuPosition
 *
 * "Description."
 */


class cpuPosition: public MibLeaf {

public:
    cpuPosition();
    virtual ~cpuPosition();

    static cpuPosition* s_pInstance;

    virtual void        get_request(Request*, int);
    virtual long        get_state();
    virtual void        set_state(long);

//--AgentGen BEGIN=cpuPosition
//--AgentGen END
};


/**
 *  procesID
 *
 * "Description."
 */


class procesID: public MibLeaf {

public:
    procesID();
    virtual ~procesID();

    static procesID* s_pInstance;

    virtual void        get_request(Request*, int);
    virtual long        get_state();
    virtual void        set_state(long);

//--AgentGen BEGIN=procesID
//--AgentGen END
};


/**
 *  alarmParameter
 *
 * "Description."
 */


class alarmParameter: public MibLeaf {

public:
    alarmParameter();
    virtual ~alarmParameter();

    static alarmParameter* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=alarmParameter
//--AgentGen END
};


/**
 *  alarmRestoreType
 *
 * "Description."
 */


class alarmRestoreType: public MibLeaf {

public:
    alarmRestoreType();
    virtual ~alarmRestoreType();

    static alarmRestoreType* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=alarmRestoreType
//--AgentGen END
};


/**
 *  restoreDate
 *
 * "Description."
 */


class restoreDate: public MibLeaf {

public:
    restoreDate();
    virtual ~restoreDate();

    static restoreDate* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=restoreDate
//--AgentGen END
};


/**
 *  acknowledgementOperator
 *
 * "Description."
 */


class acknowledgementOperator: public MibLeaf {

public:
    acknowledgementOperator();
    virtual ~acknowledgementOperator();

    static acknowledgementOperator* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=acknowledgementOperator
//--AgentGen END
};


/**
 *  acknowledgementDate
 *
 * "Description."
 */


class acknowledgementDate: public MibLeaf {

public:
    acknowledgementDate();
    virtual ~acknowledgementDate();

    static acknowledgementDate* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=acknowledgementDate
//--AgentGen END
};


/**
 *  queryAlarm
 *
 * "Description."
 */


class queryAlarm: public MibLeaf {

public:
    queryAlarm();
    virtual ~queryAlarm();

    static queryAlarm* s_pInstance;

    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
    
protected:
    GenAddress m_curPeerIp;
    STRING m_strError;
    char m_szNmsName[VALLEN];
    MibTableRow *m_pQueryRow;
//--AgentGen BEGIN=queryAlarm
//--AgentGen END
};


/**
 *  errnoInformation
 *
 * "Description."
 */


class errnoInformation: public MibLeaf {

public:
    errnoInformation();
    virtual ~errnoInformation();

    static errnoInformation* s_pInstance;

    virtual void        get_request(Request*, int);

//--AgentGen BEGIN=errnoInformation
//--AgentGen END
};


/**
 *  errorTrap
 *
 * "Description."
 */


class errorTrap: public NotificationOriginator {

public:
    errorTrap();
    virtual ~errorTrap();

    virtual void            generate(Vbx*, int, const OctetStr&);

//--AgentGen BEGIN=errorTrap
//--AgentGen END
};


/**
 *  alarmTrap
 *
 * "Description."
 */


class alarmTrap: public NotificationOriginator {

public:
    alarmTrap();
    virtual ~alarmTrap();

    virtual void            generate(Vbx*, int, const OctetStr&);

//--AgentGen BEGIN=alarmTrap
//--AgentGen END
};


/**
 *  nmsIP
 *
 * "Description."
 */


class nmsIP: public MibLeaf {

public:
    nmsIP(const Oidx&);
    virtual ~nmsIP();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    unsigned char m_szCurVal[VALLEN];
    unsigned long m_nValLen;
//--AgentGen BEGIN=nmsIP
//--AgentGen END
};


/**
 *  nmsPort
 *
 * "Description."
 */


class nmsPort: public MibLeaf {

public:
    nmsPort(const Oidx&);
    virtual ~nmsPort();

    virtual MibEntryPtr clone();
    virtual void        get_request(Request*, int);
    virtual long        get_state();
    virtual void        set_state(long);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);
    virtual boolean     value_ok(const Vbx&);
protected:
    GenAddress m_curPeerIp;
    int m_nVal;
//--AgentGen BEGIN=nmsPort
//--AgentGen END
};


/**
 *  nmsRowStatus
 *
 * "Description."
 */


class nmsRowStatus: public snmpRowStatus {

public:
    nmsRowStatus(const Oidx&);
    virtual ~nmsRowStatus();

    virtual MibEntryPtr clone();
    virtual long        get_state();
    virtual void        set_state(long);
    virtual int         set(const Vbx&);
    virtual int         prepare_set_request(Request*, int&);

//--AgentGen BEGIN=nmsRowStatus
//--AgentGen END
};


/**
 *  alarmQueryEntry
 *
 * "Description."
 */


class alarmQueryEntry: public MibTable {

public:
    alarmQueryEntry();
    virtual ~alarmQueryEntry();

    static alarmQueryEntry* s_pInstance;

    virtual boolean ready_for_service(Vbx*, int);
    virtual void    row_added(MibTableRow*, const Oidx&, MibTable*);
    virtual void    row_delete(MibTableRow*, const Oidx&, MibTable*);
    virtual void    row_init(MibTableRow*, const Oidx&, MibTable*);
    virtual void    row_activated(MibTableRow*, const Oidx&, MibTable*);
    virtual void    row_deactivated(MibTableRow*, const Oidx&, MibTable*);
    virtual void    set_row(MibTableRow* r, char* p0, char* p1, char* p2, long p3, char* p4, char* p5, char* p6, char* p7, char* p8, char* p9, long p10, long p11);    
    virtual int     commit_set_request(Request*, int);
    int change_status()
    {
        return m_nChangeStatus;
    }
    const STRING &get_cur_index()
    {
        return m_curIndex;
    }

    //对所有查询条件进行校验或转换
    static BOOL condtion_valid(SQueryStatus&, SAlarmQueryCond*);

    //告警类型校验或转换
    static BOOL type_valid(const char*, SAlarmQueryCond*);

    //恢复标志校验或转换
    static BOOL res_flg_valid(const char*, SAlarmQueryCond*);

    //确认标志校验或转换
    static BOOL ack_flag_valid(const char*, SAlarmQueryCond*);

    //模块号校验或转换
    static BOOL mod_num_valid(int, SAlarmQueryCond*);

    //流水号校验或转换
    static BOOL csn_scope_valid(const char*, SAlarmQueryCond*);

    //告警编号校验或转换
    static BOOL id_scope_valid(const char*, SAlarmQueryCond*);

    //告警级别校验或转换
    static BOOL level_valid(const char*, SAlarmQueryCond*);

    //网管分类校验或转换
    static BOOL sub_sys_valid(const char*, SAlarmQueryCond*);

    //日期校验或转换
    static BOOL date_scope_valid(const char*, SAlarmQueryCond*);

    //时间校验或转换
    static BOOL time_scope_valid(const char*, SAlarmQueryCond*);

    //返回记录数校验或转换
    static BOOL return_csn_valid(int, SAlarmQueryCond*);
protected:
    STRING m_curIndex;
    int m_nChangeStatus;
    static STRING m_strError;
    static char m_szCatch[VALLEN];
//--AgentGen BEGIN=alarmQueryEntry
//--AgentGen END
};


/**
 *  nmsEntry
 *
 * "Description."
 */


class nmsEntry: public MibTable {

public:
    nmsEntry();
    virtual ~nmsEntry();

    static nmsEntry* s_pInstance;

    virtual boolean ready_for_service(Vbx*, int);
    virtual void    row_added(MibTableRow*, const Oidx&, MibTable*);
    virtual void    row_delete(MibTableRow*, const Oidx&, MibTable*);
    virtual void    row_init(MibTableRow*, const Oidx&, MibTable*);
    virtual void    row_activated(MibTableRow*, const Oidx&, MibTable*);
    virtual void    row_deactivated(MibTableRow*, const Oidx&, MibTable*);
    virtual void    set_row(MibTableRow* r, char* p0, long p1, long p2);
    int             get_nms_list(LIST<STRING> &);
    virtual int     commit_set_request(Request*, int);
    int change_status()
    {
        return m_nChangeStatus;
    }
    const STRING &get_cur_index()
    {
        return m_curIndex;
    }
protected:
    STRING m_curIndex;
    int m_nChangeStatus;
    STRING m_strError;
//--AgentGen BEGIN=nmsEntry
//--AgentGen END
};


class huawei_snmpalarm_mib: public MibGroup
{
  public:
    huawei_snmpalarm_mib();
    virtual ~huawei_snmpalarm_mib() { }

//--AgentGen BEGIN=huawei_snmpalarm_mib
//--AgentGen END

};

//--AgentGen BEGIN=_END
//--AgentGen END


/**
 * huawei_snmpalarm_mib.h generated by AgentGen 1.5 for AGENT++v3.4 
 * Tue Jul 01 15:47:22 CST 2003.
 */


#endif



