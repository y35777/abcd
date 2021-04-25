/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_2GCDR_Org */
/* Created: Mon Mar 24 10:39:40 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Solaris 2.3 or later */
/* Target machine type: SPARC */
/* C compiler options required: None */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_2GCDR_Org.c -headerfile CDRF_2GCDR_Org.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix Org_ -dualheader -ansi -verbose -ber -root
 * D:\application\ossasn1\solaris\asn1dflt.solaris-sparc
 * D:\project\530\asn1app\maimaoshi\CDRF_2GCDR_Org.asn
 */

#include   <stddef.h>
#include   <string.h>
#include   <stdio.h>
#include   <limits.h>
#include   "asn1code.h"
#include   "asn1util.h"
#include   "ossber.h"
#include   "../asn1_r98/CDRF_2GCDR_Org.h"

#if ((OSS_TOED_API_LEVEL + 0) < 3)
    #define _oss_dec_getmem_internal	_oss_dec_getmem
#endif /* OSS_TOED_API_LEVEL */

static unsigned char _v0[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00 };
Org__ObjectID Org_gsm1205InformationModel = {
    6,
    _v0
};

static unsigned char _v1[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00, 0x02 };
Org__ObjectID Org_gsm1205ASN1Module = {
    7,
    _v1
};

static unsigned char _v2[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00, 0x03 };
Org__ObjectID Org_gsm1205ManagedObjectClass = {
    7,
    _v2
};

static unsigned char _v3[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00, 0x04 };
Org__ObjectID Org_gsm1205Package = {
    7,
    _v3
};

static unsigned char _v4[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00, 0x06 };
Org__ObjectID Org_gsm1205NameBinding = {
    7,
    _v4
};

static unsigned char _v5[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00, 0x07 };
Org__ObjectID Org_gsm1205Attribute = {
    7,
    _v5
};

static unsigned char _v6[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00, 0x09 };
Org__ObjectID Org_gsm1205Action = {
    7,
    _v6
};

static unsigned char _v7[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00, 0x0A };
Org__ObjectID Org_gsm1205Notification = {
    7,
    _v7
};

int Org_maxAddressLength = 20;

int Org_maxISDN_AddressLength = 10;

static ossBoolean _v8 = FALSE;


extern void _oss_run_TOED_on_SPARC_SOLARIS_23(void);
static void _oss_post_init(struct ossGlobal *world);

static void _ossinit_CDRF_2GCDR_Org(struct ossGlobal *world) {
    _oss_run_TOED_on_SPARC_SOLARIS_23();
    ossLinkBer(world);
    _oss_post_init(world);
}

static long _edOrg_IPBinaryAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_IPBinaryAddress *_in_data);
static void _d_Org_IPBinaryAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_IPBinaryAddress *_out_data);
static int _f_Org_IPBinaryAddress(struct ossGlobal *_g, Org_IPBinaryAddress *_data_ptr);
#ifdef OSSPRINT
static int _p_Org_IPBinaryAddress(struct ossGlobal *_g, Org_IPBinaryAddress *_data_ptr);
#endif
static long _edOrg_IPTextRepresentedAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_IPTextRepresentedAddress *_in_data);
static void _d_Org_IPTextRepresentedAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_IPTextRepresentedAddress *_out_data);
static int _f_Org_IPTextRepresentedAddress(struct ossGlobal *_g, Org_IPTextRepresentedAddress *_data_ptr);
#ifdef OSSPRINT
static int _p_Org_IPTextRepresentedAddress(struct ossGlobal *_g, Org_IPTextRepresentedAddress *_data_ptr);
#endif
static long _edOrg_IPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_IPAddress *_in_data);
static void _d_Org_IPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_IPAddress *_out_data);
static int _f_Org_IPAddress(struct ossGlobal *_g, Org_IPAddress *_data_ptr);
#ifdef OSSPRINT
static int _p_Org_IPAddress(struct ossGlobal *_g, Org_IPAddress *_data_ptr);
#endif
static long _edOrg_PDPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_PDPAddress *_in_data);
static void _d_Org_PDPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_PDPAddress *_out_data);
static int _f_Org_PDPAddress(struct ossGlobal *_g, Org_PDPAddress *_data_ptr);
#ifdef OSSPRINT
static int _p_Org_PDPAddress(struct ossGlobal *_g, Org_PDPAddress *_data_ptr);
#endif
static long _edOrg_QoSInformation(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_QoSInformation *_in_data);
static void _d_Org_QoSInformation(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_QoSInformation *_out_data);
static int _f_Org_QoSInformation(struct ossGlobal *_g, Org_QoSInformation *_data_ptr);
#ifdef OSSPRINT
static int _p_Org_QoSInformation(struct ossGlobal *_g, Org_QoSInformation *_data_ptr);
#endif
static long _edOrg_ChangeOfCharCondition(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_ChangeOfCharCondition *_in_data);
static void _d_Org_ChangeOfCharCondition(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_ChangeOfCharCondition *_out_data);
static int _f_Org_ChangeOfCharCondition(struct ossGlobal *_g, Org_ChangeOfCharCondition *_data_ptr);
#ifdef OSSPRINT
static int _p_Org_ChangeOfCharCondition(struct ossGlobal *_g, Org_ChangeOfCharCondition *_data_ptr);
#endif
static long _edOrg_ManagementExtension(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_ManagementExtension *_in_data);
static void _d_Org_ManagementExtension(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_ManagementExtension *_out_data);
static int _f_Org_ManagementExtension(struct ossGlobal *_g, Org_ManagementExtension *_data_ptr);
#ifdef OSSPRINT
static int _p_Org_ManagementExtension(struct ossGlobal *_g, Org_ManagementExtension *_data_ptr);
#endif
static long _edOrg_Diagnostics(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_Diagnostics *_in_data);
static void _d_Org_Diagnostics(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_Diagnostics *_out_data);
static int _f_Org_Diagnostics(struct ossGlobal *_g, Org_Diagnostics *_data_ptr);
#ifdef OSSPRINT
static int _p_Org_Diagnostics(struct ossGlobal *_g, Org_Diagnostics *_data_ptr);
#endif
static long _edOrg_ManagementExtensions(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_ManagementExtensions *_in_data);
static void _d_Org_ManagementExtensions(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_ManagementExtensions *_out_data);
static int _f_Org_ManagementExtensions(struct ossGlobal *_g, Org_ManagementExtensions *_data_ptr);
#ifdef OSSPRINT
static int _p_Org_ManagementExtensions(struct ossGlobal *_g, Org_ManagementExtensions *_data_ptr);
#endif
static int _eOrg_CallEventRecord(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags);
static int _dOrg_CallEventRecord(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen);
static int _fOrg_CallEventRecord(struct ossGlobal *_g, void *_data);
#ifdef OSSPRINT
static int _pOrg_CallEventRecord(struct ossGlobal *_g, void *_data);
#endif
static int _emCDR2G(struct ossGlobal *_g, int _pdunum, void *_inbuf, char **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH]);
static int _dmCDR2G(struct ossGlobal *_g, int *_pdunum, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH]);
static int _fmCDR2G(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl);
#ifdef OSSPRINT
static int _pmCDR2G(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl);
#endif



#if OSSDEBUG > 1
static const _Context_Id _context_id[] = {
    {"Org_iPBinV4Address", "OCTET STRING", _octetstring_type},
    {"Org_iPBinV6Address", "OCTET STRING", _octetstring_type},
    {"Org_iPTextV4Address", "Character String", _charstring_type},
    {"Org_iPTextV6Address", "Character String", _charstring_type},
    {"Org_iPBinaryAddress", "IPBinaryAddress", _choice_type},
    {"Org_iPTextRepresentedAddress", "IPTextRepresentedAddress", _choice_type},
    {"Org_iPAddress", "IPAddress", _choice_type},
    {"Org_eTSIAddress", "AddressString", _octetstring_type},
    {"meanThroughput", "QoSMeanThroughput", _enumerated_type},
    {"peakThroughput", "QoSPeakThroughput", _enumerated_type},
    {"precedence", "QoSPrecedence", _enumerated_type},
    {"delay", "QoSDelay", _enumerated_type},
    {"reliability", "QoSReliability", _enumerated_type},
    {"changeTime", "TimeStamp", _octetstring_type},
    {"changeCondition", "ChangeCondition", _enumerated_type},
    {"dataVolumeGPRSDownlink", "DataVolumeGPRS", _integer_type},
    {"dataVolumeGPRSUplink", "DataVolumeGPRS", _integer_type},
    {"Org_qosNegotiated", "QoSInformation", _sequence_type},
    {"Org_qosRequested", "QoSInformation", _sequence_type},
    {"information", "ANY", _any_type},
    {"Org_significance", "BOOLEAN", _boolean_type},
    {"identifier", "OBJECT IDENTIFIER", _object_identifier_type},
    {"Org_gsm0408Cause", "INTEGER", _integer_type},
    {"Org_gsm0902MapErrorValue", "INTEGER", _integer_type},
    {"Org_ccittQ767Cause", "INTEGER", _integer_type},
    {"Org_networkSpecificCause", "ManagementExtension", _sequence_type},
    {"Org_manufacturerSpecificCause", "ManagementExtension", _sequence_type},
    {NULL, "ManagementExtension", _sequence_type},
    {"CallEventRecord", "CHOICE", _choice_type},
    {"Org_sgsnPDPRecord", "SGSNPDPRecord", _set_type},
    {"Org_SGSNPDPRecord_servedMSISDN", "ISDN-AddressString", _octetstring_type},
    {"accessPointNameOI", "AccessPointNameOI", _charstring_type},
    {"Org_SGSNPDPRecord_apnSelectionMode", "APNSelectionMode", _enumerated_type},
    {"Org_SGSNPDPRecord_localSequenceNumber", "LocalSequenceNumber", _integer_type},
    {"Org_SGSNPDPRecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Org_SGSNPDPRecord_nodeID", "NodeID", _charstring_type},
    {"Org_SGSNPDPRecord_recordSequenceNumber", "INTEGER", _integer_type},
    {"Org_SGSNPDPRecord_diagnostics", "Diagnostics", _choice_type},
    {"causeForRecClosing", "CauseForRecClosing", _integer_type},
    {"Org_SGSNPDPRecord_sgsnChange", "SGSNChange", _boolean_type},
    {"duration", "CallDuration", _integer_type},
    {"recordOpeningTime", "TimeStamp", _octetstring_type},
    {"listOfTrafficVolumes", "SEQUENCE OF", _sequence_of_type},
    {NULL, "ChangeOfCharCondition", _sequence_type},
    {"servedPDPAddress", "PDPAddress", _choice_type},
    {"pdpType", "PDPType", _octetstring_type},
    {"accessPointNameNI", "AccessPointNameNI", _charstring_type},
    {"ggsnAddressUsed", "IPAddress", _choice_type},
    {"chargingID", "ChargingID", _integer_type},
    {"Org_SGSNPDPRecord_cellIdentity", "CellId", _octetstring_type},
    {"Org_SGSNPDPRecord_locationAreaCode", "LocationAreaCode", _octetstring_type},
    {"Org_SGSNPDPRecord_routingArea", "RoutingAreaCode", _octetstring_type},
    {"Org_SGSNPDPRecord_msNetworkCapability", "MSNetworkCapability", _octetstring_type},
    {"sgsnAddress", "IPAddress", _choice_type},
    {"Org_SGSNPDPRecord_servedIMEI", "IMEI", _octetstring_type},
    {"servedIMSI", "IMSI", _octetstring_type},
    {"Org_SGSNPDPRecord_anonymousAccessIndicator", "BOOLEAN", _boolean_type},
    {"Org_SGSNPDPRecord_networkInitiation", "NetworkInitiatedPDPContext", _boolean_type},
    {"recordType", "CallEventRecordType", _integer_type},
    {"Org_ggsnPDPRecord", "GGSNPDPRecord", _set_type},
    {"Org_GGSNPDPRecord_servedMSISDN", "ISDN-AddressString", _octetstring_type},
    {"Org_GGSNPDPRecord_apnSelectionMode", "APNSelectionMode", _enumerated_type},
    {"Org_GGSNPDPRecord_localSequenceNumber", "LocalSequenceNumber", _integer_type},
    {"Org_GGSNPDPRecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Org_GGSNPDPRecord_nodeID", "NodeID", _charstring_type},
    {"Org_GGSNPDPRecord_recordSequenceNumber", "INTEGER", _integer_type},
    {"Org_GGSNPDPRecord_diagnostics", "Diagnostics", _choice_type},
    {"Org_dynamicAddressFlag", "DynamicAddressFlag", _boolean_type},
    {"Org_remotePDPAddress", "SEQUENCE OF", _sequence_of_type},
    {NULL, "PDPAddress", _choice_type},
    {"sgsnAddress", "SEQUENCE OF", _sequence_of_type},
    {NULL, "IPAddress", _choice_type},
    {"ggsnAddress", "IPAddress", _choice_type},
    {"Org_GGSNPDPRecord_anonymousAccessIndicator", "BOOLEAN", _boolean_type},
    {"Org_GGSNPDPRecord_networkInitiation", "NetworkInitiatedPDPContext", _boolean_type},
    {"Org_sgsnMMRecord", "SGSNMMRecord", _set_type},
    {"Org_SGSNMMRecord_localSequenceNumber", "LocalSequenceNumber", _integer_type},
    {"Org_SGSNMMRecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Org_SGSNMMRecord_nodeID", "NodeID", _charstring_type},
    {"Org_SGSNMMRecord_recordSequenceNumber", "INTEGER", _integer_type},
    {"Org_SGSNMMRecord_diagnostics", "Diagnostics", _choice_type},
    {"Org_SGSNMMRecord_sgsnChange", "SGSNChange", _boolean_type},
    {"Org_duration", "CallDuration", _integer_type},
    {"Org_changeLocation", "SEQUENCE OF", _sequence_of_type},
    {NULL, "ChangeLocation", _sequence_type},
    {"Org_cellId", "CellId", _octetstring_type},
    {"routingAreaCode", "RoutingAreaCode", _octetstring_type},
    {"locationAreaCode", "LocationAreaCode", _octetstring_type},
    {"Org_SGSNMMRecord_cellIdentity", "CellId", _octetstring_type},
    {"Org_SGSNMMRecord_locationAreaCode", "LocationAreaCode", _octetstring_type},
    {"Org_SGSNMMRecord_routingArea", "RoutingAreaCode", _octetstring_type},
    {"Org_SGSNMMRecord_msNetworkCapability", "MSNetworkCapability", _octetstring_type},
    {"Org_SGSNMMRecord_servedIMEI", "IMEI", _octetstring_type},
    {"Org_sgsnSMORecord", "SGSNSMORecord", _set_type},
    {"Org_SGSNSMORecord_localSequenceNumber", "LocalSequenceNumber", _integer_type},
    {"Org_SGSNSMORecord_nodeID", "NodeID", _charstring_type},
    {"Org_SGSNSMORecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Org_SGSNSMORecord_smsResult", "Diagnostics", _choice_type},
    {"originationTime", "TimeStamp", _octetstring_type},
    {"messageReference", "MessageReference", _octetstring_type},
    {"Org_SGSNSMORecord_cellIdentity", "CellId", _octetstring_type},
    {"Org_SGSNSMORecord_routingArea", "RoutingAreaCode", _octetstring_type},
    {"Org_SGSNSMORecord_locationArea", "LocationAreaCode", _octetstring_type},
    {"recordingEntity", "AddressString", _octetstring_type},
    {"serviceCentre", "AddressString", _octetstring_type},
    {"msNetworkCapability", "MSNetworkCapability", _octetstring_type},
    {"Org_SGSNSMORecord_servedMSISDN", "ISDN-AddressString", _octetstring_type},
    {"Org_SGSNSMORecord_servedIMEI", "IMEI", _octetstring_type},
    {"Org_sgsnSMTRecord", "SGSNSMTRecord", _set_type},
    {"Org_SGSNSMTRecord_localSequenceNumber", "LocalSequenceNumber", _integer_type},
    {"Org_SGSNSMTRecord_nodeID", "NodeID", _charstring_type},
    {"Org_SGSNSMTRecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Org_SGSNSMTRecord_smsResult", "Diagnostics", _choice_type},
    {"Org_SGSNSMTRecord_cellIdentity", "CellId", _octetstring_type},
    {"Org_SGSNSMTRecord_routingArea", "RoutingAreaCode", _octetstring_type},
    {"Org_SGSNSMTRecord_locationArea", "LocationAreaCode", _octetstring_type},
    {"Org_SGSNSMTRecord_servedMSISDN", "ISDN-AddressString", _octetstring_type},
    {"Org_SGSNSMTRecord_servedIMEI", "IMEI", _octetstring_type}
};
#endif


static long _edOrg_IPBinaryAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_IPBinaryAddress *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	switch (_in_data->choice) {
	case 1:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[0];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.iPBinV4Address, 2, 4);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 2:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[1];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.iPBinV6Address, 2, 16);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	default:
	    _oss_enc_error(_g, TRUE, _bad_choice, (long)_in_data->choice);	/* Bad choice selector. */
	}

    }

    _constructed = TRUE;
    *_max_len = _out_len;
    return(_data_len);
}

static long _edOrg_IPTextRepresentedAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_IPTextRepresentedAddress *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	switch (_in_data->choice) {
	case 1:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[2];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.iPTextV4Address, 15);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 2:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[3];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.iPTextV6Address, 45);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	default:
	    _oss_enc_error(_g, TRUE, _bad_choice, (long)_in_data->choice);	/* Bad choice selector. */
	}

    }

    _constructed = TRUE;
    *_max_len = _out_len;
    return(_data_len);
}

static long _edOrg_IPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_IPAddress *_in_data)
{
#if OSSDEBUG > 1
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
#endif
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	switch (_in_data->choice) {
	case 1:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[4];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edOrg_IPBinaryAddress(_g, _out_pos, &_out_len, &_in_data->u.iPBinaryAddress);
	    _constructed = TRUE;
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 2:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[5];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edOrg_IPTextRepresentedAddress(_g, _out_pos, &_out_len, &_in_data->u.iPTextRepresentedAddress);
	    _constructed = TRUE;
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	default:
	    _oss_enc_error(_g, TRUE, _bad_choice, (long)_in_data->choice);	/* Bad choice selector. */
	}

    }

    _constructed = TRUE;
    *_max_len = _out_len;
    return(_data_len);
}

static long _edOrg_PDPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_PDPAddress *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	switch (_in_data->choice) {
	case 1:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[6];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edOrg_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.iPAddress);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 2:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[7];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.eTSIAddress, 2, 20);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	default:
	    _oss_enc_error(_g, TRUE, _bad_choice, (long)_in_data->choice);	/* Bad choice selector. */
	}

    }

    _constructed = TRUE;
    *_max_len = _out_len;
    return(_data_len);
}

static long _edOrg_QoSInformation(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_QoSInformation *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[8];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->meanThroughput);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[9];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->peakThroughput);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[10];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->precedence);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[11];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->delay);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[12];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->reliability);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	_data_len = _total_len;
    }

    _constructed = TRUE;
    *_max_len = _out_len;
    return(_data_len);
}

static long _edOrg_ChangeOfCharCondition(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_ChangeOfCharCondition *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[13];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->changeTime, 2, 9);
	if (_data_len < 0) {
	    _constructed = TRUE;
	    _data_len = -_data_len;
	}
	else
	    _constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[14];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->changeCondition);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[15];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->dataVolumeGPRSDownlink);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[16];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->dataVolumeGPRSUplink);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[17];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & Org_qosNegotiated_present) {
	    _data_len = _edOrg_QoSInformation(_g, _out_pos, &_out_len, &_in_data->qosNegotiated);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
	    _total_len += _data_len;
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[18];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & Org_qosRequested_present) {
	    _data_len = _edOrg_QoSInformation(_g, _out_pos, &_out_len, &_in_data->qosRequested);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
	    _total_len += _data_len;
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	_data_len = _total_len;
    }

    _constructed = TRUE;
    *_max_len = _out_len;
    return(_data_len);
}

static long _edOrg_ManagementExtension(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_ManagementExtension *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[19];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_uany(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->information);
	_constructed = TRUE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[20];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & Org_significance_present) {
	    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->significance);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
	    _total_len += _data_len;
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[21];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_eobjid(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->identifier, -1);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x6, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	_data_len = _total_len;
    }

    _constructed = TRUE;
    *_max_len = _out_len;
    return(_data_len);
}

static long _edOrg_Diagnostics(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_Diagnostics *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	switch (_in_data->choice) {
	case 1:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[22];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.gsm0408Cause);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 2:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[23];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.gsm0902MapErrorValue);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 3:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[24];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ccittQ767Cause);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 4:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[25];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edOrg_ManagementExtension(_g, _out_pos, &_out_len, &_in_data->u.networkSpecificCause);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 5:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[26];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edOrg_ManagementExtension(_g, _out_pos, &_out_len, &_in_data->u.manufacturerSpecificCause);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	default:
	    _oss_enc_error(_g, TRUE, _bad_choice, (long)_in_data->choice);	/* Bad choice selector. */
	}

    }

    _constructed = TRUE;
    *_max_len = _out_len;
    return(_data_len);
}

static long _edOrg_ManagementExtensions(struct ossGlobal *_g, char **_out_pos, long *_max_len, Org_ManagementExtensions *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	struct Org_ManagementExtensions_	*_temp;
	unsigned long count;
	unsigned long _lindex;
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
	_element._id = (_Context_Id *)&_context_id[27];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	for (_temp = *_in_data, count = 0; _temp; _temp = _temp->next) {
#if OSSDEBUG > 1
	    _element._occurrence = count + 1;
#endif

	    _oss_enc_push(_g, _temp);
	    count++;
	}
	if (count > 1) _oss_enc_error(_g, TRUE, _too_long, count);
	for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
	    _element._occurrence = count - _lindex;
#endif

	    _temp = (struct Org_ManagementExtensions_ *) _oss_enc_pop(_g);
	    _data_len = _edOrg_ManagementExtension(_g, _out_pos, &_out_len, &_temp->value);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x10, _constructed);
	    _total_len += _data_len;
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	_data_len = _total_len;
    }

    _constructed = TRUE;
    *_max_len = _out_len;
    return(_data_len);
}

static int _eOrg_CallEventRecord(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    Org_CallEventRecord	*_in_data;
    char *_out_buf;
    char **_out_pos = &_out_buf;
    long _out_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

#if OSSDEBUG > 1
    _Context_Element _element;

    _element._parent = NULL;
    _element._id = NULL;
    _element._occurrence = 0;
#endif

    _in_data = (Org_CallEventRecord *) _inbuf;
    if (!_oss_c->_buffer_provided) *_outbuf = NULL;
    _out_buf = *_outbuf;
    _out_len = _oss_c->_oss_outbuflen = (_oss_c->_buffer_provided) ? *_outlen : 0;
    _out_buf += _out_len;
#if OSSDEBUG > 1
    _oss_c->_oss_context_anchor._pdu_number = 1;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[28];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	switch (_in_data->choice) {
	case 1:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[29];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    {
		long _total_len = 0;

#if OSSDEBUG > 1
		_Context_Element _element;

		_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[30];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_servedMSISDN_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.servedMSISDN, 2, 10);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801D, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[31];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.accessPointNameOI, 37);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801A, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[32];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_apnSelectionMode_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.apnSelectionMode);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8019, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[33];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_localSequenceNumber_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.localSequenceNumber);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8018, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[34];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_recordExtensions_present) {
		    _data_len = _edOrg_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[35];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_nodeID_present) {
		    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.nodeID, 20);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8016, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[36];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present) {
		    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.recordSequenceNumber);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[37];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_diagnostics_present) {
		    _data_len = _edOrg_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.diagnostics);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[38];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.causeForRecClosing);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[39];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_sgsnChange_present) {
		    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.sgsnChange);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8012, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[40];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.duration);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[41];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.recordOpeningTime, 2, 9);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[42];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		{
		    struct Org__seqof1_	*_temp;
		    unsigned long count;
		    unsigned long _lindex;
		    long _total_len = 0;

#if OSSDEBUG > 1
		    _Context_Element _element;

		    _element._occurrence = 0;
		    _element._id = (_Context_Id *)&_context_id[43];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    for (_temp = _in_data->u.sgsnPDPRecord.listOfTrafficVolumes, count = 0; _temp; _temp = _temp->next) {
#if OSSDEBUG > 1
			_element._occurrence = count + 1;
#endif

			_oss_enc_push(_g, _temp);
			count++;
		    }
		    for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
			_element._occurrence = count - _lindex;
#endif

			_temp = (struct Org__seqof1_ *) _oss_enc_pop(_g);
			_data_len = _edOrg_ChangeOfCharCondition(_g, _out_pos, &_out_len, &_temp->value);
			_constructed = TRUE;
			_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x10, _constructed);
			_total_len += _data_len;
		    }
#if OSSDEBUG > 1
		    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    _data_len = _total_len;
		}

		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[44];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _edOrg_PDPAddress(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.servedPDPAddress);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[45];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.pdpType, 2, 2);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[46];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.accessPointNameNI, 63);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[47];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _edOrg_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.ggsnAddressUsed);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[48];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.chargingID);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800A, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[49];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_cellIdentity_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.cellIdentity, 2, 2);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[50];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_locationAreaCode_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.locationAreaCode, 2, 2);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8008, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[51];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_routingArea_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.routingArea, 2, 1);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8007, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[52];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_msNetworkCapability_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.msNetworkCapability, 2, 1);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[53];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _edOrg_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.sgsnAddress);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[54];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_servedIMEI_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.servedIMEI, 2, 8);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[55];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.servedIMSI, 2, 8);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[56];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_anonymousAccessIndicator_present) {
		    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.anonymousAccessIndicator);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[57];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_networkInitiation_present) {
		    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.networkInitiation);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[58];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.recordType);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		_data_len = _total_len;
	    }

	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 2:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[59];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    {
		long _total_len = 0;

#if OSSDEBUG > 1
		_Context_Element _element;

		_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[60];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_servedMSISDN_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.ggsnPDPRecord.servedMSISDN, 2, 10);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801D, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[61];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_apnSelectionMode_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.apnSelectionMode);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[62];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_localSequenceNumber_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.localSequenceNumber);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[63];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_recordExtensions_present) {
		    _data_len = _edOrg_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[64];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_nodeID_present) {
		    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.nodeID, 20);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8012, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[65];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present) {
		    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.recordSequenceNumber);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[66];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_diagnostics_present) {
		    _data_len = _edOrg_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.diagnostics);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[38];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.causeForRecClosing);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[40];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.duration);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[41];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.ggsnPDPRecord.recordOpeningTime, 2, 9);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[42];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		{
		    struct Org__seqof1_	*_temp;
		    unsigned long count;
		    unsigned long _lindex;
		    long _total_len = 0;

#if OSSDEBUG > 1
		    _Context_Element _element;

		    _element._occurrence = 0;
		    _element._id = (_Context_Id *)&_context_id[43];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    for (_temp = _in_data->u.ggsnPDPRecord.listOfTrafficVolumes, count = 0; _temp; _temp = _temp->next) {
#if OSSDEBUG > 1
			_element._occurrence = count + 1;
#endif

			_oss_enc_push(_g, _temp);
			count++;
		    }
		    for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
			_element._occurrence = count - _lindex;
#endif

			_temp = (struct Org__seqof1_ *) _oss_enc_pop(_g);
			_data_len = _edOrg_ChangeOfCharCondition(_g, _out_pos, &_out_len, &_temp->value);
			_constructed = TRUE;
			_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x10, _constructed);
			_total_len += _data_len;
		    }
#if OSSDEBUG > 1
		    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    _data_len = _total_len;
		}

		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[67];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_dynamicAddressFlag_present) {
		    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.dynamicAddressFlag);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[68];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_remotePDPAddress_present) {
		    {
			struct Org__seqof3	*_temp;
			unsigned long count;
			unsigned long _lindex;
			long _total_len = 0;

#if OSSDEBUG > 1
			_Context_Element _element;

			_element._occurrence = 0;
			_element._id = (_Context_Id *)&_context_id[69];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			for (_temp = _in_data->u.ggsnPDPRecord.remotePDPAddress, count = 0; _temp; _temp = _temp->next) {
#if OSSDEBUG > 1
			    _element._occurrence = count + 1;
#endif

			    _oss_enc_push(_g, _temp);
			    count++;
			}
			for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
			    _element._occurrence = count - _lindex;
#endif

			    _temp = (struct Org__seqof3 *) _oss_enc_pop(_g);
			    _data_len = _edOrg_PDPAddress(_g, _out_pos, &_out_len, &_temp->value);
			    _constructed = TRUE;
			    _total_len += _data_len;
			}
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			_data_len = _total_len;
		    }

		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800A, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[44];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _edOrg_PDPAddress(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.servedPDPAddress);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[45];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.ggsnPDPRecord.pdpType, 2, 2);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8008, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[46];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.accessPointNameNI, 63);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8007, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[70];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		{
		    struct Org__seqof2	*_temp;
		    unsigned long count;
		    unsigned long _lindex;
		    long _total_len = 0;

#if OSSDEBUG > 1
		    _Context_Element _element;

		    _element._occurrence = 0;
		    _element._id = (_Context_Id *)&_context_id[71];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    for (_temp = _in_data->u.ggsnPDPRecord.sgsnAddress, count = 0; _temp; _temp = _temp->next) {
#if OSSDEBUG > 1
			_element._occurrence = count + 1;
#endif

			_oss_enc_push(_g, _temp);
			count++;
		    }
		    for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
			_element._occurrence = count - _lindex;
#endif

			_temp = (struct Org__seqof2 *) _oss_enc_pop(_g);
			_data_len = _edOrg_IPAddress(_g, _out_pos, &_out_len, &_temp->value);
			_constructed = TRUE;
			_total_len += _data_len;
		    }
#if OSSDEBUG > 1
		    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    _data_len = _total_len;
		}

		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[48];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.chargingID);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[72];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _edOrg_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.ggsnAddress);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[55];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.ggsnPDPRecord.servedIMSI, 2, 8);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[73];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_anonymousAccessIndicator_present) {
		    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.anonymousAccessIndicator);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[74];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_networkInitiation_present) {
		    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.networkInitiation);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[58];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.recordType);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		_data_len = _total_len;
	    }

	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 3:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[75];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    {
		long _total_len = 0;

#if OSSDEBUG > 1
		_Context_Element _element;

		_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[76];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_localSequenceNumber_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.localSequenceNumber);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[77];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_recordExtensions_present) {
		    _data_len = _edOrg_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.sgsnMMRecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[78];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_nodeID_present) {
		    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.nodeID, 20);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[79];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_recordSequenceNumber_present) {
		    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.recordSequenceNumber);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[80];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_diagnostics_present) {
		    _data_len = _edOrg_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.sgsnMMRecord.diagnostics);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[38];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.causeForRecClosing);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[81];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_sgsnChange_present) {
		    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.sgsnChange);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[82];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_duration_present) {
		    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.duration);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800A, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[41];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.recordOpeningTime, 2, 9);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[83];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_changeLocation_present) {
		    {
			struct Org__seqof4	*_temp;
			unsigned long count;
			unsigned long _lindex;
			long _total_len = 0;

#if OSSDEBUG > 1
			_Context_Element _element;

			_element._occurrence = 0;
			_element._id = (_Context_Id *)&_context_id[84];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			for (_temp = _in_data->u.sgsnMMRecord.changeLocation, count = 0; _temp; _temp = _temp->next) {
#if OSSDEBUG > 1
			    _element._occurrence = count + 1;
#endif

			    _oss_enc_push(_g, _temp);
			    count++;
			}
			for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
			    _element._occurrence = count - _lindex;
#endif

			    _temp = (struct Org__seqof4 *) _oss_enc_pop(_g);
			    {
				long _total_len = 0;

#if OSSDEBUG > 1
				_Context_Element _element;

				_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[13];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.changeTime, 2, 9);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
				_total_len += _data_len;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[85];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_temp->value.bit_mask & Org_cellId_present) {
				    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.cellId, 2, 2);
				    if (_data_len < 0) {
					_constructed = TRUE;
					_data_len = -_data_len;
				    }
				    else
					_constructed = FALSE;
				    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
				    _total_len += _data_len;
				}

#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[86];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.routingAreaCode, 2, 1);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
				_total_len += _data_len;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[87];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.locationAreaCode, 2, 2);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
				_total_len += _data_len;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

				_data_len = _total_len;
			    }

			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x10, _constructed);
			    _total_len += _data_len;
			}
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			_data_len = _total_len;
		    }

		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8008, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[88];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_cellIdentity_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.cellIdentity, 2, 2);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8007, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[89];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_locationAreaCode_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.locationAreaCode, 2, 2);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[90];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_routingArea_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.routingArea, 2, 1);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[91];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_msNetworkCapability_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.msNetworkCapability, 2, 1);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[53];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _edOrg_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.sgsnMMRecord.sgsnAddress);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[92];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_servedIMEI_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.servedIMEI, 2, 8);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[55];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.servedIMSI, 2, 8);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[58];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.recordType);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		_data_len = _total_len;
	    }

	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 4:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[93];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    {
		long _total_len = 0;

#if OSSDEBUG > 1
		_Context_Element _element;

		_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[94];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_localSequenceNumber_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMORecord.localSequenceNumber);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[95];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_nodeID_present) {
		    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMORecord.nodeID, 20);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[96];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_recordExtensions_present) {
		    _data_len = _edOrg_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.sgsnSMORecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[97];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_smsResult_present) {
		    _data_len = _edOrg_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.sgsnSMORecord.smsResult);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[98];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.originationTime, 2, 9);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[99];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.messageReference, 2, 9);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800A, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[100];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_cellIdentity_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.cellIdentity, 2, 2);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[101];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_routingArea_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.routingArea, 2, 1);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8008, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[102];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_locationArea_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.locationArea, 2, 2);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8007, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[103];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.recordingEntity, 2, 20);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[104];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.serviceCentre, 2, 20);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[105];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.msNetworkCapability, 2, 1);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[106];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_servedMSISDN_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.servedMSISDN, 2, 10);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[107];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_servedIMEI_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.servedIMEI, 2, 8);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[55];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.servedIMSI, 2, 8);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[58];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMORecord.recordType);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		_data_len = _total_len;
	    }

	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 5:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[108];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    {
		long _total_len = 0;

#if OSSDEBUG > 1
		_Context_Element _element;

		_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[109];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_localSequenceNumber_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMTRecord.localSequenceNumber);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[110];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_nodeID_present) {
		    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMTRecord.nodeID, 20);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[111];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_recordExtensions_present) {
		    _data_len = _edOrg_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.sgsnSMTRecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[112];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_smsResult_present) {
		    _data_len = _edOrg_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.sgsnSMTRecord.smsResult);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[98];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.originationTime, 2, 9);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800A, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[113];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_cellIdentity_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.cellIdentity, 2, 2);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[114];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_routingArea_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.routingArea, 2, 1);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8008, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[115];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_locationArea_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.locationArea, 2, 2);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8007, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[103];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.recordingEntity, 2, 20);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[104];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.serviceCentre, 2, 20);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[105];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.msNetworkCapability, 2, 1);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[116];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_servedMSISDN_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.servedMSISDN, 2, 10);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[117];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_servedIMEI_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.servedIMEI, 2, 8);
		    if (_data_len < 0) {
			_constructed = TRUE;
			_data_len = -_data_len;
		    }
		    else
			_constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[55];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.servedIMSI, 2, 8);
		if (_data_len < 0) {
		    _constructed = TRUE;
		    _data_len = -_data_len;
		}
		else
		    _constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[58];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMTRecord.recordType);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		_data_len = _total_len;
	    }

	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	default:
	    _oss_enc_error(_g, TRUE, _bad_choice, (long)_in_data->choice);	/* Bad choice selector. */
	}

    }

    _constructed = TRUE;
    if (!_oss_c->_buffer_provided)
	_oss_set_outmem_d(_g, _out_len, _outlen, _outbuf);
    else {
	if (_flags & FRONT_ALIGN)
	    memmove(*_outbuf, *_out_pos, (size_t) _data_len);
	else
	    *_outbuf = *_out_pos;
	*_outlen = _data_len;
    }

    return(PDU_ENCODED);
}

static int _emCDR2G(struct ossGlobal *_g, int _pdunum, void *_inbuf, char **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH])
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    int _rc = 0;

    _oss_c->_oss_context_anchor._state = _encoding_data;
    _oss_c->_oss_context_anchor._pdu_number = 0;
    if (_errmsg != NULL) _errmsg[0] = 0;
#if OSSDEBUG > 0
    _oss_c->_oss_err_msg = _errmsg;
#else
    _oss_c->_oss_err_msg = NULL;
#endif
    _oss_c->_buffer_provided = (_flags & BUFFER_PROVIDED) ? TRUE : FALSE;
    if (_oss_c->_buffer_provided && (*_outbuf == NULL))
	_oss_c->_buffer_provided = FALSE;
    _oss_c->_restrain_mode = (_oss_c->_buffer_provided && (_flags & RESTRAIN)) ? TRUE : FALSE;
    _oss_c->_restrain_size = (_oss_c->_restrain_mode) ? *_outlen : 0;

    switch (_pdunum)
    {
	case 1:
	    _rc = _eOrg_CallEventRecord(_g, _inbuf, _outbuf, _outlen, _flags);
	    break;

	default:
	    _oss_enc_error(_g, TRUE, _pdu_range, (long) _pdunum);
    }

    _oss_c->_oss_err_msg = NULL;
    return _rc;
}

static void _d_Org_IPBinaryAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_IPBinaryAddress *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	if (!_oss_c->_tag_decoded) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	else
	    _constructed = _tag_constructed;
	switch (_data_tag) {
	default:
	    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    switch (_data_tag & ~0x8000U) {
	    case 0x0: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[0];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 1;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.iPBinV4Address, 2, 4, _constructed);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x1: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[1];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 2;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.iPBinV6Address, 2, 16, _constructed);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    default:
		_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    }
	    break;
	}
    }

}

static void _d_Org_IPTextRepresentedAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_IPTextRepresentedAddress *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	if (!_oss_c->_tag_decoded) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	else
	    _constructed = _tag_constructed;
	switch (_data_tag) {
	default:
	    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    switch (_data_tag & ~0x8000U) {
	    case 0x2: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[2];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 1;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.iPTextV4Address, 15, _constructed);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x3: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[3];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 2;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.iPTextV6Address, 45, _constructed);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    default:
		_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    }
	    break;
	}
    }

}

static void _d_Org_IPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_IPAddress *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	if (!_oss_c->_tag_decoded) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	else
	    _constructed = _tag_constructed;
	if (_data_tag == 0x8000 || _data_tag == 0x8001) {
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[4];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _out_data->choice = 1;
	    _d_Org_IPBinaryAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPBinaryAddress);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	} else
	if (_data_tag == 0x8002 || _data_tag == 0x8003) {
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[5];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _out_data->choice = 2;
	    _d_Org_IPTextRepresentedAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPTextRepresentedAddress);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	} else
	switch (_data_tag) {
	default:
	    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    switch (_data_tag & ~0x8000U) {
	    default:
		_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    }
	    break;
	}
    }

}

static void _d_Org_PDPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_PDPAddress *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	short _indef_tags = 0;
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	if (!_oss_c->_tag_decoded) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	else
	    _constructed = _tag_constructed;
	switch (_data_tag) {
	default:
	    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    switch (_data_tag & ~0x8000U) {
	    case 0x0: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[6];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 1;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		if (_data_length < 0) ++_indef_tags;
		_d_Org_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPAddress);
		while (_indef_tags > 0) {
		    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
		    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
		    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
		    _indef_tags--;
		}
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x1: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[7];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 2;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.eTSIAddress, 2, 20, _constructed);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    default:
		_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    }
	    break;
	}
    }

}

static void _d_Org_QoSInformation(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_QoSInformation *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[12];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_oss_c->_tag_decoded)
	    _oss_c->_tag_decoded = FALSE;
	else {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0x8000) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	}

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->reliability = (enum Org_QoSReliability) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[11];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->delay = (enum Org_QoSDelay) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[10];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8002) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->precedence = (enum Org_QoSPrecedence) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[9];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8003) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->peakThroughput = (enum Org_QoSPeakThroughput) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[8];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8004) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->meanThroughput = (enum Org_QoSMeanThroughput) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	if (_total_len < 0) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _oss_c->_tag_decoded = FALSE;
	} else {
	    if (*_in_pos != _end_pos) _oss_dec_error(_g, TRUE, _inconsis_len, 0L);
	}
    }

}

static void _d_Org_ChangeOfCharCondition(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_ChangeOfCharCondition *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	if (!_oss_c->_tag_decoded && (_total_len < 0 || *_in_pos < _end_pos)) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8001 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~Org_qosRequested_present;
	else {
	    _out_data->bit_mask |= Org_qosRequested_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[18];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    if (_oss_c->_tag_decoded)
		_oss_c->_tag_decoded = FALSE;
	    else {
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
		if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	    }

	    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	    _d_Org_QoSInformation(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->qosRequested);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (!_oss_c->_tag_decoded && (_total_len < 0 || *_in_pos < _end_pos)) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~Org_qosNegotiated_present;
	else {
	    _out_data->bit_mask |= Org_qosNegotiated_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[17];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    if (_oss_c->_tag_decoded)
		_oss_c->_tag_decoded = FALSE;
	    else {
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
		if (_data_tag != 0x8002) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	    }

	    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	    _d_Org_QoSInformation(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->qosNegotiated);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[16];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_oss_c->_tag_decoded)
	    _oss_c->_tag_decoded = FALSE;
	else {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0x8003) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	}

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->dataVolumeGPRSUplink = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[15];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8004) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->dataVolumeGPRSDownlink = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[14];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8005) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->changeCondition = (enum Org_ChangeCondition) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[13];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8006) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->changeTime, 2, 9, _constructed);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	if (_total_len < 0) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _oss_c->_tag_decoded = FALSE;
	} else {
	    if (*_in_pos != _end_pos) _oss_dec_error(_g, TRUE, _inconsis_len, 0L);
	}
    }

}

static void _d_Org_ManagementExtension(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_ManagementExtension *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	short _indef_tags = 0;
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[21];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_oss_c->_tag_decoded)
	    _oss_c->_tag_decoded = FALSE;
	else {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0x6) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	}

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	if (_constructed)
	    _oss_dec_error(_g, TRUE, _objid_is_constructed, 0L);
	_oss_dec_eobjid(_g, _in_pos, _buf_len, _data_length, _oss_c->_buffer_provided, &_out_data->identifier, -1);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	if (_total_len < 0 || *_in_pos < _end_pos) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8001 || (_total_len >= 0 && *_in_pos >= _end_pos)) {
	    _out_data->bit_mask &= ~Org_significance_present;
	    memcpy(&_out_data->significance, &_v8, sizeof(_v8));
	}
	else {
	    _out_data->bit_mask |= Org_significance_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[20];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    if (_oss_c->_tag_decoded)
		_oss_c->_tag_decoded = FALSE;
	    else {
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
		if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	    }

	    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	    _out_data->significance = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[19];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_data_length == 0) _data_length = 1;
	if (_oss_c->_tag_decoded)
	    _oss_c->_tag_decoded = FALSE;
	else {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0x8002) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	}

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	if (_data_length < 0) ++_indef_tags;
	_oss_dec_uany(_g, _in_pos, _buf_len, _data_length, _oss_c->_buffer_provided, &_out_data->information);
	while (_indef_tags > 0) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _indef_tags--;
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	if (_total_len < 0) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _oss_c->_tag_decoded = FALSE;
	} else {
	    if (*_in_pos != _end_pos) _oss_dec_error(_g, TRUE, _inconsis_len, 0L);
	}
    }

}

static void _d_Org_Diagnostics(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_Diagnostics *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	if (!_oss_c->_tag_decoded) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	else
	    _constructed = _tag_constructed;
	switch (_data_tag) {
	default:
	    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    switch (_data_tag & ~0x8000U) {
	    case 0x0: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[22];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 1;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_out_data->u.gsm0408Cause = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x1: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[23];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 2;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_out_data->u.gsm0902MapErrorValue = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x2: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[24];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 3;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_out_data->u.ccittQ767Cause = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x3: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[25];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 4;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_d_Org_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.networkSpecificCause);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x4: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[26];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 5;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_d_Org_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.manufacturerSpecificCause);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    default:
		_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    }
	    break;
	}
    }

}

static void _d_Org_ManagementExtensions(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Org_ManagementExtensions *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	struct Org_ManagementExtensions_	*_temp;
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

	unsigned long count = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	_temp = *_out_data;
#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[27];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	while (TRUE) {
	    if (_total_len < 0) {
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
		_oss_c->_tag_decoded = TRUE;
		if (_data_tag == 0) {
		    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
		    _oss_c->_tag_decoded = FALSE;
		    break;
		}
	    } else {
		if (*_in_pos >= _end_pos) break;
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
		_oss_c->_tag_decoded = TRUE;
	    }
	    count++;
	    if (count > 1) _oss_dec_error(_g, TRUE, _too_long, count);
	    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	    if (_temp) {
		_temp->next = (struct Org_ManagementExtensions_ *) _oss_dec_getmem(_g, sizeof(struct Org_ManagementExtensions_)*1, _oss_c->_buffer_provided);
		_temp = _temp->next;
	    } else {
		_temp = (struct Org_ManagementExtensions_ *) _oss_dec_getmem(_g, sizeof(struct Org_ManagementExtensions_)*1, _oss_c->_buffer_provided);
		*_out_data = _temp;
	    }
	    _temp->next = NULL;
#if OSSDEBUG > 1
	    _element._occurrence = count;
#endif

	    _oss_c->_tag_decoded = FALSE;
	    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	    _d_Org_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value);
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

    }
}

static int _dOrg_CallEventRecord(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    Org_CallEventRecord	*_out_data;
    char **_in_pos;
    long *_buf_len;
    long _data_length;
    unsigned short _data_tag = 0;
    ossBoolean _constructed = FALSE;
#if OSSDEBUG > 1
    _Context_Element _element;

    _element._parent = NULL;
    _element._id = NULL;
    _element._occurrence = 0;
#endif

    _in_pos = _inbuf;
    _buf_len = _inlen;
    if (!_oss_c->_buffer_provided) *_outbuf = NULL;
    _oss_c->_oss_outbufpos = (char *) *_outbuf;
    _oss_c->_oss_outbuflen = (_oss_c->_buffer_provided) ? *_outlen : 0;
#if OSSDEBUG > 1
    _oss_c->_oss_context_anchor._pdu_number = 1;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[28];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    _out_data = (struct Org_CallEventRecord *) _oss_dec_getmem(_g, sizeof(struct Org_CallEventRecord)*1, _oss_c->_buffer_provided);
    _oss_c->_tag_decoded = FALSE;
    _data_length = *_buf_len;
    {
#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	_oss_c->_tag_decoded = TRUE;
	switch (_data_tag) {
	default:
	    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    switch (_data_tag & ~0x8000U) {
	    case 0x0: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[29];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 1;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		{
		    short _indef_tags = 0;
		    long _total_len = _data_length;
		    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
		    unsigned long _present_flags = 0;
#if OSSDEBUG > 1
		    _Context_Element _element;
#endif

#if OSSDEBUG > 1
		    _element._occurrence = 0;
#endif

		    while (TRUE) {
			if (_total_len < 0) {
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			    if (_data_tag == 0) {
				if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				_oss_c->_tag_decoded = FALSE;
				break;
			    }
			} else {
			    if (*_in_pos >= _end_pos) break;
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			}
			switch (_data_tag) {
			default:
			    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    switch (_data_tag & ~0x8000U) {
			    case 0x0: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[58];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x1;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[57];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_networkInitiation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_networkInitiation_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.networkInitiation = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x2;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x2: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[56];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_anonymousAccessIndicator_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_anonymousAccessIndicator_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.anonymousAccessIndicator = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x4;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x3: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[55];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.servedIMSI, 2, 8, _constructed);
				_present_flags |= 0x8;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x4: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[54];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_servedIMEI_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.servedIMEI, 2, 8, _constructed);
				_present_flags |= 0x10;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x5: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[53];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.sgsnAddress);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x20;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x6: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[52];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_msNetworkCapability_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.msNetworkCapability, 2, 1, _constructed);
				_present_flags |= 0x40;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x7: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[51];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_routingArea_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.routingArea, 2, 1, _constructed);
				_present_flags |= 0x80;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x8: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[50];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_locationAreaCode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_locationAreaCode_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.locationAreaCode, 2, 2, _constructed);
				_present_flags |= 0x100;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x9: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[49];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_cellIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_cellIdentity_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.cellIdentity, 2, 2, _constructed);
				_present_flags |= 0x200;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xA: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[48];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.chargingID = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x400;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xB: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[47];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.ggsnAddressUsed);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x800;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xC: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[46];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnPDPRecord.accessPointNameNI, 63, _constructed);
				_present_flags |= 0x1000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xD: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[45];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.pdpType, 2, 2, _constructed);
				_present_flags |= 0x2000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xE: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[44];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.servedPDPAddress);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x4000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xF: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[42];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				{
				    struct Org__seqof1_	*_temp;
				    long _total_len = _data_length;
				    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

				    unsigned long count = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

				    _temp = _out_data->u.sgsnPDPRecord.listOfTrafficVolumes;
#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[43];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    while (TRUE) {
					if (_total_len < 0) {
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					    if (_data_tag == 0) {
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_oss_c->_tag_decoded = FALSE;
						break;
					    }
					} else {
					    if (*_in_pos >= _end_pos) break;
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					}
					count++;
					if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
					if (_temp) {
					    _temp->next = (struct Org__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct Org__seqof1_)*1, _oss_c->_buffer_provided);
					    _temp = _temp->next;
					} else {
					    _temp = (struct Org__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct Org__seqof1_)*1, _oss_c->_buffer_provided);
					    _out_data->u.sgsnPDPRecord.listOfTrafficVolumes = _temp;
					}
					_temp->next = NULL;
#if OSSDEBUG > 1
					_element._occurrence = count;
#endif

					_oss_c->_tag_decoded = FALSE;
					_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					_d_Org_ChangeOfCharCondition(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value);
				    }
#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

				}
				_present_flags |= 0x8000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x10: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[41];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.recordOpeningTime, 2, 9, _constructed);
				_present_flags |= 0x10000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x11: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[40];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x20000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x12: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[39];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_sgsnChange_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_sgsnChange_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.sgsnChange = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x40000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x13: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[38];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x80000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x14: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[37];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_diagnostics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.diagnostics);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x100000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x15: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[36];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_recordSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.recordSequenceNumber = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x200000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x16: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[35];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_nodeID_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnPDPRecord.nodeID, 20, _constructed);
				_present_flags |= 0x400000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x17: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[34];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d_Org_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.recordExtensions);
				_present_flags |= 0x800000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x18: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[33];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_localSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x1000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x19: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[32];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_apnSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_apnSelectionMode_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.apnSelectionMode = (enum Org_APNSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x2000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1A: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[31];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnPDPRecord.accessPointNameOI, 37, _constructed);
				_present_flags |= 0x4000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1D: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[30];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= Org_SGSNPDPRecord_servedMSISDN_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.servedMSISDN, 2, 10, _constructed);
				_present_flags |= 0x8000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    default:
				_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    }
			    break;
			}

		    }

		    if (!(_present_flags & 0x2)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_networkInitiation_present;
		    }
		    if (!(_present_flags & 0x4)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_anonymousAccessIndicator_present;
		    }
		    if (!(_present_flags & 0x10)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_servedIMEI_present;
		    }
		    if (!(_present_flags & 0x40)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_msNetworkCapability_present;
		    }
		    if (!(_present_flags & 0x80)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_routingArea_present;
		    }
		    if (!(_present_flags & 0x100)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_locationAreaCode_present;
		    }
		    if (!(_present_flags & 0x200)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_cellIdentity_present;
		    }
		    if (!(_present_flags & 0x40000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_sgsnChange_present;
		    }
		    if (!(_present_flags & 0x100000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_diagnostics_present;
		    }
		    if (!(_present_flags & 0x200000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_recordSequenceNumber_present;
		    }
		    if (!(_present_flags & 0x400000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x800000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x1000000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_localSequenceNumber_present;
		    }
		    if (!(_present_flags & 0x2000000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_apnSelectionMode_present;
		    }
		    if (!(_present_flags & 0x8000000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~Org_SGSNPDPRecord_servedMSISDN_present;
		    }
		    if ((_present_flags & 0x40BFC29) != 0x40BFC29) _oss_dec_error(_g, TRUE, _field_omit, 0L);
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x1: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[59];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 2;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		{
		    short _indef_tags = 0;
		    long _total_len = _data_length;
		    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
		    unsigned long _present_flags = 0;
#if OSSDEBUG > 1
		    _Context_Element _element;
#endif

#if OSSDEBUG > 1
		    _element._occurrence = 0;
#endif

		    while (TRUE) {
			if (_total_len < 0) {
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			    if (_data_tag == 0) {
				if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				_oss_c->_tag_decoded = FALSE;
				break;
			    }
			} else {
			    if (*_in_pos >= _end_pos) break;
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			}
			switch (_data_tag) {
			default:
			    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    switch (_data_tag & ~0x8000U) {
			    case 0x0: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[58];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x1;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[74];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_networkInitiation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_GGSNPDPRecord_networkInitiation_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.networkInitiation = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x2;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x2: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[73];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_anonymousAccessIndicator_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_GGSNPDPRecord_anonymousAccessIndicator_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.anonymousAccessIndicator = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x4;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x3: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[55];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.ggsnPDPRecord.servedIMSI, 2, 8, _constructed);
				_present_flags |= 0x8;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x4: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[72];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.ggsnAddress);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x10;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x5: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[48];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.chargingID = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x20;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x6: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[70];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				{
				    struct Org__seqof2	*_temp;
				    long _total_len = _data_length;
				    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

				    unsigned long count = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

				    _temp = _out_data->u.ggsnPDPRecord.sgsnAddress;
#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[71];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    while (TRUE) {
					if (_total_len < 0) {
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					    if (_data_tag == 0) {
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_oss_c->_tag_decoded = FALSE;
						break;
					    }
					} else {
					    if (*_in_pos >= _end_pos) break;
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					}
					count++;
					if (_temp) {
					    _temp->next = (struct Org__seqof2 *) _oss_dec_getmem(_g, sizeof(struct Org__seqof2)*1, _oss_c->_buffer_provided);
					    _temp = _temp->next;
					} else {
					    _temp = (struct Org__seqof2 *) _oss_dec_getmem(_g, sizeof(struct Org__seqof2)*1, _oss_c->_buffer_provided);
					    _out_data->u.ggsnPDPRecord.sgsnAddress = _temp;
					}
					_temp->next = NULL;
#if OSSDEBUG > 1
					_element._occurrence = count;
#endif

					_d_Org_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value);
				    }
#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

				}
				_present_flags |= 0x40;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x7: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[46];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.ggsnPDPRecord.accessPointNameNI, 63, _constructed);
				_present_flags |= 0x80;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x8: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[45];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.ggsnPDPRecord.pdpType, 2, 2, _constructed);
				_present_flags |= 0x100;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x9: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[44];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.servedPDPAddress);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x200;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xA: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[68];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_remotePDPAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_remotePDPAddress_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				{
				    struct Org__seqof3	*_temp;
				    long _total_len = _data_length;
				    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

				    unsigned long count = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

				    _temp = _out_data->u.ggsnPDPRecord.remotePDPAddress;
#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[69];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    while (TRUE) {
					if (_total_len < 0) {
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					    if (_data_tag == 0) {
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_oss_c->_tag_decoded = FALSE;
						break;
					    }
					} else {
					    if (*_in_pos >= _end_pos) break;
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					}
					count++;
					if (_temp) {
					    _temp->next = (struct Org__seqof3 *) _oss_dec_getmem(_g, sizeof(struct Org__seqof3)*1, _oss_c->_buffer_provided);
					    _temp = _temp->next;
					} else {
					    _temp = (struct Org__seqof3 *) _oss_dec_getmem(_g, sizeof(struct Org__seqof3)*1, _oss_c->_buffer_provided);
					    _out_data->u.ggsnPDPRecord.remotePDPAddress = _temp;
					}
					_temp->next = NULL;
#if OSSDEBUG > 1
					_element._occurrence = count;
#endif

					_d_Org_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value);
				    }
#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

				}
				_present_flags |= 0x400;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xB: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[67];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_dynamicAddressFlag_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_dynamicAddressFlag_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.dynamicAddressFlag = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x800;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xC: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[42];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				{
				    struct Org__seqof1_	*_temp;
				    long _total_len = _data_length;
				    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

				    unsigned long count = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

				    _temp = _out_data->u.ggsnPDPRecord.listOfTrafficVolumes;
#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[43];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    while (TRUE) {
					if (_total_len < 0) {
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					    if (_data_tag == 0) {
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_oss_c->_tag_decoded = FALSE;
						break;
					    }
					} else {
					    if (*_in_pos >= _end_pos) break;
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					}
					count++;
					if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
					if (_temp) {
					    _temp->next = (struct Org__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct Org__seqof1_)*1, _oss_c->_buffer_provided);
					    _temp = _temp->next;
					} else {
					    _temp = (struct Org__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct Org__seqof1_)*1, _oss_c->_buffer_provided);
					    _out_data->u.ggsnPDPRecord.listOfTrafficVolumes = _temp;
					}
					_temp->next = NULL;
#if OSSDEBUG > 1
					_element._occurrence = count;
#endif

					_oss_c->_tag_decoded = FALSE;
					_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					_d_Org_ChangeOfCharCondition(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value);
				    }
#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

				}
				_present_flags |= 0x1000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xD: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[41];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.ggsnPDPRecord.recordOpeningTime, 2, 9, _constructed);
				_present_flags |= 0x2000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xE: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[40];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x4000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xF: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[38];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x8000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x10: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[66];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_GGSNPDPRecord_diagnostics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.diagnostics);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x10000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x11: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[65];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_GGSNPDPRecord_recordSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.recordSequenceNumber = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x20000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x12: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[64];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_GGSNPDPRecord_nodeID_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.ggsnPDPRecord.nodeID, 20, _constructed);
				_present_flags |= 0x40000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x13: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[63];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_GGSNPDPRecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d_Org_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.recordExtensions);
				_present_flags |= 0x80000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x14: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[62];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_GGSNPDPRecord_localSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x100000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x15: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[61];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_apnSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_GGSNPDPRecord_apnSelectionMode_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.apnSelectionMode = (enum Org_APNSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x200000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;
#ifdef _PRODUCT_MOTO
/*应Motorola要求，临时把ServerdMSISDN tag值增加0x16 (CISCO GGSN问题)*/
/*ldf，2004-08-13 合入CGW 1.8.0.0.60,对应问题单SWPD07573,同B01对应问题单D20663,仅针对Motorola版本*/
                            case 0x16:
#endif
			    case 0x1D: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[60];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= Org_GGSNPDPRecord_servedMSISDN_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.ggsnPDPRecord.servedMSISDN, 2, 10, _constructed);
				_present_flags |= 0x400000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    default:
				_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    }
			    break;
			}

		    }

		    if (!(_present_flags & 0x2)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_GGSNPDPRecord_networkInitiation_present;
		    }
		    if (!(_present_flags & 0x4)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_GGSNPDPRecord_anonymousAccessIndicator_present;
		    }
		    if (!(_present_flags & 0x400)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_remotePDPAddress_present;
		    }
		    if (!(_present_flags & 0x800)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_dynamicAddressFlag_present;
		    }
		    if (!(_present_flags & 0x10000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_GGSNPDPRecord_diagnostics_present;
		    }
		    if (!(_present_flags & 0x20000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_GGSNPDPRecord_recordSequenceNumber_present;
		    }
		    if (!(_present_flags & 0x40000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_GGSNPDPRecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x80000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_GGSNPDPRecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x100000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_GGSNPDPRecord_localSequenceNumber_present;
		    }
		    if (!(_present_flags & 0x200000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_GGSNPDPRecord_apnSelectionMode_present;
		    }
		    if (!(_present_flags & 0x400000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~Org_GGSNPDPRecord_servedMSISDN_present;
		    }
		    if ((_present_flags & 0xF3F9) != 0xF3F9) _oss_dec_error(_g, TRUE, _field_omit, 0L);
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x2: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[75];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 3;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		{
		    short _indef_tags = 0;
		    long _total_len = _data_length;
		    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
		    unsigned long _present_flags = 0;
#if OSSDEBUG > 1
		    _Context_Element _element;
#endif

#if OSSDEBUG > 1
		    _element._occurrence = 0;
#endif

		    while (TRUE) {
			if (_total_len < 0) {
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			    if (_data_tag == 0) {
				if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				_oss_c->_tag_decoded = FALSE;
				break;
			    }
			} else {
			    if (*_in_pos >= _end_pos) break;
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			}
			switch (_data_tag) {
			default:
			    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    switch (_data_tag & ~0x8000U) {
			    case 0x0: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[58];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnMMRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x1;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[55];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.servedIMSI, 2, 8, _constructed);
				_present_flags |= 0x2;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x2: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[92];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_servedIMEI_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.servedIMEI, 2, 8, _constructed);
				_present_flags |= 0x4;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x3: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[53];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnMMRecord.sgsnAddress);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x8;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x4: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[91];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_msNetworkCapability_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.msNetworkCapability, 2, 1, _constructed);
				_present_flags |= 0x10;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x5: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[90];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_routingArea_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.routingArea, 2, 1, _constructed);
				_present_flags |= 0x20;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x6: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[89];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_locationAreaCode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_locationAreaCode_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.locationAreaCode, 2, 2, _constructed);
				_present_flags |= 0x40;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x7: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[88];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_cellIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_cellIdentity_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.cellIdentity, 2, 2, _constructed);
				_present_flags |= 0x80;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x8: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[83];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_changeLocation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_changeLocation_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				{
				    struct Org__seqof4	*_temp;
				    long _total_len = _data_length;
				    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

				    unsigned long count = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

				    _temp = _out_data->u.sgsnMMRecord.changeLocation;
#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[84];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    while (TRUE) {
					if (_total_len < 0) {
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					    if (_data_tag == 0) {
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_oss_c->_tag_decoded = FALSE;
						break;
					    }
					} else {
					    if (*_in_pos >= _end_pos) break;
					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    _oss_c->_tag_decoded = TRUE;
					}
					count++;
					if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
					if (_temp) {
					    _temp->next = (struct Org__seqof4 *) _oss_dec_getmem(_g, sizeof(struct Org__seqof4)*1, _oss_c->_buffer_provided);
					    _temp = _temp->next;
					} else {
					    _temp = (struct Org__seqof4 *) _oss_dec_getmem(_g, sizeof(struct Org__seqof4)*1, _oss_c->_buffer_provided);
					    _out_data->u.sgsnMMRecord.changeLocation = _temp;
					}
					_temp->next = NULL;
#if OSSDEBUG > 1
					_element._occurrence = count;
#endif

					_oss_c->_tag_decoded = FALSE;
					_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					{
					    long _total_len = _data_length;
					    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

#if OSSDEBUG > 1
					    _Context_Element _element;

					    _element._occurrence = 0;
#endif

#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[87];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    if (_data_tag != 0x8000) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.locationAreaCode, 2, 2, _constructed);
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[86];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
					    if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.routingAreaCode, 2, 1, _constructed);
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    if (_total_len < 0 || *_in_pos < _end_pos) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						_oss_c->_tag_decoded = TRUE;
					    }
					    if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
						_temp->value.bit_mask &= ~Org_cellId_present;
					    else {
						_temp->value.bit_mask |= Org_cellId_present;
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[85];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_oss_c->_tag_decoded)
						    _oss_c->_tag_decoded = FALSE;
						else {
						    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						    if (_data_tag != 0x8002) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
						}

						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.cellId, 2, 2, _constructed);
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }

#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[13];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_oss_c->_tag_decoded)
						_oss_c->_tag_decoded = FALSE;
					    else {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0x8003) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
					    }

					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.changeTime, 2, 9, _constructed);
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    if (_total_len < 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_oss_c->_tag_decoded = FALSE;
					    } else {
						if (*_in_pos != _end_pos) _oss_dec_error(_g, TRUE, _inconsis_len, 0L);
					    }
					}

				    }
#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

				}
				_present_flags |= 0x100;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x9: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[41];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.recordOpeningTime, 2, 9, _constructed);
				_present_flags |= 0x200;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xA: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[82];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_duration_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_duration_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnMMRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x400;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xB: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[81];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_sgsnChange_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_sgsnChange_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnMMRecord.sgsnChange = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x800;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xC: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[38];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnMMRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x1000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xD: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[80];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_diagnostics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnMMRecord.diagnostics);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x2000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xE: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[79];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_recordSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_recordSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnMMRecord.recordSequenceNumber = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x4000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xF: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[78];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_nodeID_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnMMRecord.nodeID, 20, _constructed);
				_present_flags |= 0x8000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x10: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[77];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d_Org_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnMMRecord.recordExtensions);
				_present_flags |= 0x10000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x11: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[76];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= Org_SGSNMMRecord_localSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnMMRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x20000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    default:
				_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    }
			    break;
			}

		    }

		    if (!(_present_flags & 0x4)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_servedIMEI_present;
		    }
		    if (!(_present_flags & 0x10)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_msNetworkCapability_present;
		    }
		    if (!(_present_flags & 0x20)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_routingArea_present;
		    }
		    if (!(_present_flags & 0x40)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_locationAreaCode_present;
		    }
		    if (!(_present_flags & 0x80)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_cellIdentity_present;
		    }
		    if (!(_present_flags & 0x100)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_changeLocation_present;
		    }
		    if (!(_present_flags & 0x400)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_duration_present;
		    }
		    if (!(_present_flags & 0x800)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_sgsnChange_present;
		    }
		    if (!(_present_flags & 0x2000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_diagnostics_present;
		    }
		    if (!(_present_flags & 0x4000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_recordSequenceNumber_present;
		    }
		    if (!(_present_flags & 0x8000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x10000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x20000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~Org_SGSNMMRecord_localSequenceNumber_present;
		    }
		    if ((_present_flags & 0x120B) != 0x120B) _oss_dec_error(_g, TRUE, _field_omit, 0L);
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x3: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[93];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 4;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		{
		    short _indef_tags = 0;
		    long _total_len = _data_length;
		    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
		    unsigned short _present_flags = 0;
#if OSSDEBUG > 1
		    _Context_Element _element;
#endif

#if OSSDEBUG > 1
		    _element._occurrence = 0;
#endif

		    while (TRUE) {
			if (_total_len < 0) {
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			    if (_data_tag == 0) {
				if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				_oss_c->_tag_decoded = FALSE;
				break;
			    }
			} else {
			    if (*_in_pos >= _end_pos) break;
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			}
			switch (_data_tag) {
			default:
			    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    switch (_data_tag & ~0x8000U) {
			    case 0x0: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[58];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnSMORecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x1;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[55];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.servedIMSI, 2, 8, _constructed);
				_present_flags |= 0x2;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x2: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[107];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= Org_SGSNSMORecord_servedIMEI_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.servedIMEI, 2, 8, _constructed);
				_present_flags |= 0x4;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x3: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[106];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= Org_SGSNSMORecord_servedMSISDN_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.servedMSISDN, 2, 10, _constructed);
				_present_flags |= 0x8;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x4: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[105];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.msNetworkCapability, 2, 1, _constructed);
				_present_flags |= 0x10;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x5: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[104];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.serviceCentre, 2, 20, _constructed);
				_present_flags |= 0x20;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x6: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[103];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.recordingEntity, 2, 20, _constructed);
				_present_flags |= 0x40;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x7: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[102];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_locationArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= Org_SGSNSMORecord_locationArea_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.locationArea, 2, 2, _constructed);
				_present_flags |= 0x80;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x8: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[101];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= Org_SGSNSMORecord_routingArea_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.routingArea, 2, 1, _constructed);
				_present_flags |= 0x100;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x9: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[100];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_cellIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= Org_SGSNSMORecord_cellIdentity_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.cellIdentity, 2, 2, _constructed);
				_present_flags |= 0x200;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xA: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[99];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.messageReference, 2, 9, _constructed);
				_present_flags |= 0x400;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xB: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[98];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.originationTime, 2, 9, _constructed);
				_present_flags |= 0x800;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xC: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[97];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_smsResult_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= Org_SGSNSMORecord_smsResult_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnSMORecord.smsResult);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x1000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xD: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[96];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= Org_SGSNSMORecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d_Org_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnSMORecord.recordExtensions);
				_present_flags |= 0x2000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xE: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[95];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= Org_SGSNSMORecord_nodeID_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnSMORecord.nodeID, 20, _constructed);
				_present_flags |= 0x4000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xF: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[94];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= Org_SGSNSMORecord_localSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnSMORecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x8000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    default:
				_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    }
			    break;
			}

		    }

		    if (!(_present_flags & 0x4)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~Org_SGSNSMORecord_servedIMEI_present;
		    }
		    if (!(_present_flags & 0x8)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~Org_SGSNSMORecord_servedMSISDN_present;
		    }
		    if (!(_present_flags & 0x80)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~Org_SGSNSMORecord_locationArea_present;
		    }
		    if (!(_present_flags & 0x100)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~Org_SGSNSMORecord_routingArea_present;
		    }
		    if (!(_present_flags & 0x200)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~Org_SGSNSMORecord_cellIdentity_present;
		    }
		    if (!(_present_flags & 0x1000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~Org_SGSNSMORecord_smsResult_present;
		    }
		    if (!(_present_flags & 0x2000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~Org_SGSNSMORecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x4000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~Org_SGSNSMORecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x8000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~Org_SGSNSMORecord_localSequenceNumber_present;
		    }
		    if ((_present_flags & 0xC73) != 0xC73) _oss_dec_error(_g, TRUE, _field_omit, 0L);
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x4: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[108];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 5;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		{
		    short _indef_tags = 0;
		    long _total_len = _data_length;
		    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
		    unsigned short _present_flags = 0;
#if OSSDEBUG > 1
		    _Context_Element _element;
#endif

#if OSSDEBUG > 1
		    _element._occurrence = 0;
#endif

		    while (TRUE) {
			if (_total_len < 0) {
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			    if (_data_tag == 0) {
				if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				_oss_c->_tag_decoded = FALSE;
				break;
			    }
			} else {
			    if (*_in_pos >= _end_pos) break;
			    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
			    _oss_c->_tag_decoded = TRUE;
			}
			switch (_data_tag) {
			default:
			    if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    switch (_data_tag & ~0x8000U) {
			    case 0x0: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[58];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnSMTRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x1;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[55];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.servedIMSI, 2, 8, _constructed);
				_present_flags |= 0x2;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x2: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[117];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= Org_SGSNSMTRecord_servedIMEI_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.servedIMEI, 2, 8, _constructed);
				_present_flags |= 0x4;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x3: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[116];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= Org_SGSNSMTRecord_servedMSISDN_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.servedMSISDN, 2, 10, _constructed);
				_present_flags |= 0x8;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x4: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[105];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.msNetworkCapability, 2, 1, _constructed);
				_present_flags |= 0x10;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x5: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[104];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.serviceCentre, 2, 20, _constructed);
				_present_flags |= 0x20;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x6: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[103];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.recordingEntity, 2, 20, _constructed);
				_present_flags |= 0x40;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x7: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[115];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_locationArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= Org_SGSNSMTRecord_locationArea_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.locationArea, 2, 2, _constructed);
				_present_flags |= 0x80;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x8: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[114];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= Org_SGSNSMTRecord_routingArea_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.routingArea, 2, 1, _constructed);
				_present_flags |= 0x100;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x9: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[113];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_cellIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= Org_SGSNSMTRecord_cellIdentity_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.cellIdentity, 2, 2, _constructed);
				_present_flags |= 0x200;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xA: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[98];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.originationTime, 2, 9, _constructed);
				_present_flags |= 0x400;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xB: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[112];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_smsResult_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= Org_SGSNSMTRecord_smsResult_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d_Org_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnSMTRecord.smsResult);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x800;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xC: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[111];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= Org_SGSNSMTRecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d_Org_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnSMTRecord.recordExtensions);
				_present_flags |= 0x1000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xD: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[110];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= Org_SGSNSMTRecord_nodeID_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnSMTRecord.nodeID, 20, _constructed);
				_present_flags |= 0x2000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xE: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[109];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= Org_SGSNSMTRecord_localSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnSMTRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x4000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    default:
				_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			    }
			    break;
			}

		    }

		    if (!(_present_flags & 0x4)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~Org_SGSNSMTRecord_servedIMEI_present;
		    }
		    if (!(_present_flags & 0x8)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~Org_SGSNSMTRecord_servedMSISDN_present;
		    }
		    if (!(_present_flags & 0x80)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~Org_SGSNSMTRecord_locationArea_present;
		    }
		    if (!(_present_flags & 0x100)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~Org_SGSNSMTRecord_routingArea_present;
		    }
		    if (!(_present_flags & 0x200)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~Org_SGSNSMTRecord_cellIdentity_present;
		    }
		    if (!(_present_flags & 0x800)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~Org_SGSNSMTRecord_smsResult_present;
		    }
		    if (!(_present_flags & 0x1000)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~Org_SGSNSMTRecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x2000)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~Org_SGSNSMTRecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x4000)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~Org_SGSNSMTRecord_localSequenceNumber_present;
		    }
		    if ((_present_flags & 0x473) != 0x473) _oss_dec_error(_g, TRUE, _field_omit, 0L);
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    default:
		_oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
	    }
	    break;
	}
    }

    *_outbuf = _out_data;
    if (_oss_c->_buffer_provided) *_outlen -= _oss_c->_oss_outbuflen;
    else if (_outlen != NULL) *_outlen = _oss_c->_oss_out_alloc;
    if (!_oss_c->_buffer_provided) _oss_releaseMem(_g, &_oss_c->_oss_mem);
    return(PDU_DECODED);
}

static int _dmCDR2G(struct ossGlobal *_g, int *_pdunum, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH])
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    int _rc = 0;
    _oss_c->_oss_context_anchor._state = _decoding_data;
    _oss_c->_oss_context_anchor._pdu_number = 0;
    if (_errmsg != NULL) _errmsg[0] = 0;
#if OSSDEBUG > 0
    _oss_c->_oss_err_msg = _errmsg;
#else
    _oss_c->_oss_err_msg = NULL;
#endif
    _oss_c->_buffer_provided = (_flags & BUFFER_PROVIDED) ? TRUE : FALSE;
    if (_oss_c->_buffer_provided && (*_outbuf == NULL))
	_oss_c->_buffer_provided = FALSE;
    _oss_c->_restrain_mode = (_oss_c->_buffer_provided && (_flags & RESTRAIN)) ? TRUE : FALSE;
    _oss_c->_restrain_size = (_oss_c->_restrain_mode) ? *_outlen : 0;
    *_pdunum = 1;
    _rc = _dOrg_CallEventRecord(_g, _inbuf, _inlen, _outbuf, _outlen);
    _oss_c->_oss_err_msg = NULL;
    return _rc;
}

static int _f_Org_IPBinaryAddress(struct ossGlobal *_g, Org_IPBinaryAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	break;

    case 2:
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_Org_IPTextRepresentedAddress(struct ossGlobal *_g, Org_IPTextRepresentedAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	break;

    case 2:
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_Org_IPAddress(struct ossGlobal *_g, Org_IPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	_f_Org_IPBinaryAddress(_g, &_data_ptr->u.iPBinaryAddress);
	break;

    case 2:
	_f_Org_IPTextRepresentedAddress(_g, &_data_ptr->u.iPTextRepresentedAddress);
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_Org_PDPAddress(struct ossGlobal *_g, Org_PDPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	_f_Org_IPAddress(_g, &_data_ptr->u.iPAddress);
	break;

    case 2:
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_Org_QoSInformation(struct ossGlobal *_g, Org_QoSInformation *_data_ptr)
{
    return(PDU_FREED);
}

static int _f_Org_ChangeOfCharCondition(struct ossGlobal *_g, Org_ChangeOfCharCondition *_data_ptr)
{
    if (_data_ptr->bit_mask & Org_qosRequested_present) {
	_f_Org_QoSInformation(_g, &_data_ptr->qosRequested);
    }

    if (_data_ptr->bit_mask & Org_qosNegotiated_present) {
	_f_Org_QoSInformation(_g, &_data_ptr->qosNegotiated);
    }

    return(PDU_FREED);
}

static int _f_Org_ManagementExtension(struct ossGlobal *_g, Org_ManagementExtension *_data_ptr)
{
    if (_data_ptr->identifier.value)
	(*_g->freep)(_data_ptr->identifier.value);
    if (_data_ptr->bit_mask & Org_significance_present) {
    }

    if (_data_ptr->information.value)
	(*_g->freep)(_data_ptr->information.value);
    return(PDU_FREED);
}

static int _f_Org_Diagnostics(struct ossGlobal *_g, Org_Diagnostics *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	break;

    case 2:
	break;

    case 3:
	break;

    case 4:
	_f_Org_ManagementExtension(_g, &_data_ptr->u.networkSpecificCause);
	break;

    case 5:
	_f_Org_ManagementExtension(_g, &_data_ptr->u.manufacturerSpecificCause);
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_Org_ManagementExtensions(struct ossGlobal *_g, Org_ManagementExtensions *_data_ptr)
{
    if (*_data_ptr) {
	struct Org_ManagementExtensions_	*_tempa = *_data_ptr;
	struct Org_ManagementExtensions_	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    _f_Org_ManagementExtension(_g, &_tempa->value);
	    (*_g->freep)(_tempa);
	    _tempa = _tempb;
	}
    }

    return(PDU_FREED);
}

static int _fOrg_CallEventRecord(struct ossGlobal *_g, void *_data)
{
    Org_CallEventRecord	*_data_ptr;
    _data_ptr = (Org_CallEventRecord *) _data;
    switch (_data_ptr->choice) {
    case 1:
	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_networkInitiation_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_anonymousAccessIndicator_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_servedIMEI_present) {
	}

	_f_Org_IPAddress(_g, &_data_ptr->u.sgsnPDPRecord.sgsnAddress);
	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_msNetworkCapability_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_routingArea_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_locationAreaCode_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_cellIdentity_present) {
	}

	_f_Org_IPAddress(_g, &_data_ptr->u.sgsnPDPRecord.ggsnAddressUsed);
	_f_Org_PDPAddress(_g, &_data_ptr->u.sgsnPDPRecord.servedPDPAddress);
	if (_data_ptr->u.sgsnPDPRecord.listOfTrafficVolumes) {
	    struct Org__seqof1_	*_tempa = _data_ptr->u.sgsnPDPRecord.listOfTrafficVolumes;
	    struct Org__seqof1_	*_tempb;
	    while (_tempa) {
		_tempb = _tempa->next;
		_f_Org_ChangeOfCharCondition(_g, &_tempa->value);
		(*_g->freep)(_tempa);
		_tempa = _tempb;
	    }
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_sgsnChange_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_diagnostics_present) {
	    _f_Org_Diagnostics(_g, &_data_ptr->u.sgsnPDPRecord.diagnostics);
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_nodeID_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_recordExtensions_present) {
	    _f_Org_ManagementExtensions(_g, &_data_ptr->u.sgsnPDPRecord.recordExtensions);
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_localSequenceNumber_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_apnSelectionMode_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_servedMSISDN_present) {
	}

	break;

    case 2:
	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_networkInitiation_present) {
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_anonymousAccessIndicator_present) {
	}

	_f_Org_IPAddress(_g, &_data_ptr->u.ggsnPDPRecord.ggsnAddress);
	if (_data_ptr->u.ggsnPDPRecord.sgsnAddress) {
	    struct Org__seqof2	*_tempa = _data_ptr->u.ggsnPDPRecord.sgsnAddress;
	    struct Org__seqof2	*_tempb;
	    while (_tempa) {
		_tempb = _tempa->next;
		_f_Org_IPAddress(_g, &_tempa->value);
		(*_g->freep)(_tempa);
		_tempa = _tempb;
	    }
	}

	_f_Org_PDPAddress(_g, &_data_ptr->u.ggsnPDPRecord.servedPDPAddress);
	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_remotePDPAddress_present) {
	    if (_data_ptr->u.ggsnPDPRecord.remotePDPAddress) {
		struct Org__seqof3	*_tempa = _data_ptr->u.ggsnPDPRecord.remotePDPAddress;
		struct Org__seqof3	*_tempb;
		while (_tempa) {
		    _tempb = _tempa->next;
		    _f_Org_PDPAddress(_g, &_tempa->value);
		    (*_g->freep)(_tempa);
		    _tempa = _tempb;
		}
	    }

	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_dynamicAddressFlag_present) {
	}

	if (_data_ptr->u.ggsnPDPRecord.listOfTrafficVolumes) {
	    struct Org__seqof1_	*_tempa = _data_ptr->u.ggsnPDPRecord.listOfTrafficVolumes;
	    struct Org__seqof1_	*_tempb;
	    while (_tempa) {
		_tempb = _tempa->next;
		_f_Org_ChangeOfCharCondition(_g, &_tempa->value);
		(*_g->freep)(_tempa);
		_tempa = _tempb;
	    }
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_diagnostics_present) {
	    _f_Org_Diagnostics(_g, &_data_ptr->u.ggsnPDPRecord.diagnostics);
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present) {
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_nodeID_present) {
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_recordExtensions_present) {
	    _f_Org_ManagementExtensions(_g, &_data_ptr->u.ggsnPDPRecord.recordExtensions);
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_localSequenceNumber_present) {
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_apnSelectionMode_present) {
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_servedMSISDN_present) {
	}

	break;

    case 3:
	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_servedIMEI_present) {
	}

	_f_Org_IPAddress(_g, &_data_ptr->u.sgsnMMRecord.sgsnAddress);
	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_msNetworkCapability_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_routingArea_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_locationAreaCode_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_cellIdentity_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_changeLocation_present) {
	    if (_data_ptr->u.sgsnMMRecord.changeLocation) {
		struct Org__seqof4	*_tempa = _data_ptr->u.sgsnMMRecord.changeLocation;
		struct Org__seqof4	*_tempb;
		while (_tempa) {
		    _tempb = _tempa->next;
		    if (_tempa->value.bit_mask & Org_cellId_present) {
		    }

		    (*_g->freep)(_tempa);
		    _tempa = _tempb;
		}
	    }

	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_duration_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_sgsnChange_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_diagnostics_present) {
	    _f_Org_Diagnostics(_g, &_data_ptr->u.sgsnMMRecord.diagnostics);
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_recordSequenceNumber_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_nodeID_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_recordExtensions_present) {
	    _f_Org_ManagementExtensions(_g, &_data_ptr->u.sgsnMMRecord.recordExtensions);
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_localSequenceNumber_present) {
	}

	break;

    case 4:
	if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_servedIMEI_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_servedMSISDN_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_locationArea_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_routingArea_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_cellIdentity_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_smsResult_present) {
	    _f_Org_Diagnostics(_g, &_data_ptr->u.sgsnSMORecord.smsResult);
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_recordExtensions_present) {
	    _f_Org_ManagementExtensions(_g, &_data_ptr->u.sgsnSMORecord.recordExtensions);
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_nodeID_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_localSequenceNumber_present) {
	}

	break;

    case 5:
	if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_servedIMEI_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_servedMSISDN_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_locationArea_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_routingArea_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_cellIdentity_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_smsResult_present) {
	    _f_Org_Diagnostics(_g, &_data_ptr->u.sgsnSMTRecord.smsResult);
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_recordExtensions_present) {
	    _f_Org_ManagementExtensions(_g, &_data_ptr->u.sgsnSMTRecord.recordExtensions);
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_nodeID_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_localSequenceNumber_present) {
	}

	break;

    default:
	/* Ignore for now */	;
    }

    (*_g->freep)(_data_ptr);
    return(PDU_FREED);
}

static int _fmCDR2G(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl)
{
    int _rc = 0;

    switch (_pdunum)
    {
	case 1:
	    _rc = _fOrg_CallEventRecord(_g, _data);
	    break;

	default:  return(PDU_RANGE);
    }
    return _rc;
}

#ifdef OSSPRINT
static int _p_Org_IPBinaryAddress(struct ossGlobal *_g, Org_IPBinaryAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	ossPrint(_g, "iPBinV4Address : ");
	_oss_prt_oct(_g, _data_ptr->u.iPBinV4Address.value, _data_ptr->u.iPBinV4Address.length);
	break;

    case 2:
	ossPrint(_g, "iPBinV6Address : ");
	_oss_prt_oct(_g, _data_ptr->u.iPBinV6Address.value, _data_ptr->u.iPBinV6Address.length);
	break;

    default:
	ossPrint(_g, "<unknown choice>");
    }

    return(PDU_PRINTED);
}

static int _p_Org_IPTextRepresentedAddress(struct ossGlobal *_g, Org_IPTextRepresentedAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	ossPrint(_g, "iPTextV4Address : ");
	_oss_prt_nchar(_g, (char *)_data_ptr->u.iPTextV4Address);
	break;

    case 2:
	ossPrint(_g, "iPTextV6Address : ");
	_oss_prt_nchar(_g, (char *)_data_ptr->u.iPTextV6Address);
	break;

    default:
	ossPrint(_g, "<unknown choice>");
    }

    return(PDU_PRINTED);
}

static int _p_Org_IPAddress(struct ossGlobal *_g, Org_IPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	ossPrint(_g, "iPBinaryAddress : ");
	_p_Org_IPBinaryAddress(_g, &_data_ptr->u.iPBinaryAddress);
	break;

    case 2:
	ossPrint(_g, "iPTextRepresentedAddress : ");
	_p_Org_IPTextRepresentedAddress(_g, &_data_ptr->u.iPTextRepresentedAddress);
	break;

    default:
	ossPrint(_g, "<unknown choice>");
    }

    return(PDU_PRINTED);
}

static int _p_Org_PDPAddress(struct ossGlobal *_g, Org_PDPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	ossPrint(_g, "iPAddress : ");
	_p_Org_IPAddress(_g, &_data_ptr->u.iPAddress);
	break;

    case 2:
	ossPrint(_g, "eTSIAddress : ");
	_oss_prt_oct(_g, _data_ptr->u.eTSIAddress.value, _data_ptr->u.eTSIAddress.length);
	break;

    default:
	ossPrint(_g, "<unknown choice>");
    }

    return(PDU_PRINTED);
}

static int _p_Org_QoSInformation(struct ossGlobal *_g, Org_QoSInformation *_data_ptr)
{
    {
	short _comma = 0;

	_oss_indent(_g, 1);
	ossPrint(_g, "{");
	_oss_indent(_g, 0);
	ossPrint(_g, "reliability ");
	{
	    ULONG_LONG value = _data_ptr->reliability;
	    ossPrint(_g, ULLONG_FMT, value);
	}
	_comma = 1;
	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "delay ");
	{
	    ULONG_LONG value = _data_ptr->delay;
	    ossPrint(_g, ULLONG_FMT, value);
	}
	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "precedence ");
	{
	    ULONG_LONG value = _data_ptr->precedence;
	    ossPrint(_g, ULLONG_FMT, value);
	}
	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "peakThroughput ");
	{
	    ULONG_LONG value = _data_ptr->peakThroughput;
	    ossPrint(_g, ULLONG_FMT, value);
	}
	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "meanThroughput ");
	{
	    ULONG_LONG value = _data_ptr->meanThroughput;
	    ossPrint(_g, ULLONG_FMT, value);
	}
	_oss_indent(_g, -1);
	ossPrint(_g, "}");
    }

    return(PDU_PRINTED);
}

static int _p_Org_ChangeOfCharCondition(struct ossGlobal *_g, Org_ChangeOfCharCondition *_data_ptr)
{
    {
	short _comma = 0;

	_oss_indent(_g, 1);
	ossPrint(_g, "{");
	if (_data_ptr->bit_mask & Org_qosRequested_present) {
	    _oss_indent(_g, 0);
	    ossPrint(_g, "qosRequested ");
	    _p_Org_QoSInformation(_g, &_data_ptr->qosRequested);
	    _comma = 1;
	}

	if (_data_ptr->bit_mask & Org_qosNegotiated_present) {
	    if (_comma) ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "qosNegotiated ");
	    _p_Org_QoSInformation(_g, &_data_ptr->qosNegotiated);
	    _comma = 1;
	}

	if (_comma) ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "dataVolumeGPRSUplink ");
	{
	    LONG_LONG value = _data_ptr->dataVolumeGPRSUplink;

	    ossPrint(_g, LLONG_FMT, value);
	}
	_comma = 1;
	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "dataVolumeGPRSDownlink ");
	{
	    LONG_LONG value = _data_ptr->dataVolumeGPRSDownlink;

	    ossPrint(_g, LLONG_FMT, value);
	}
	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "changeCondition ");
	{
	    ULONG_LONG value = _data_ptr->changeCondition;
	    ossPrint(_g, ULLONG_FMT, value);
	}
	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "changeTime ");
	_oss_prt_oct(_g, _data_ptr->changeTime.value, _data_ptr->changeTime.length);
	_oss_indent(_g, -1);
	ossPrint(_g, "}");
    }

    return(PDU_PRINTED);
}

static int _p_Org_ManagementExtension(struct ossGlobal *_g, Org_ManagementExtension *_data_ptr)
{
    {
	short _comma = 0;

	_oss_indent(_g, 1);
	ossPrint(_g, "{");
	_oss_indent(_g, 0);
	ossPrint(_g, "identifier ");
	{
	    OssBuf _val;
	    OssEncodedOID _e;
	    _val.length = 0;
	    _val.value = NULL;
	    _e.length = _data_ptr->identifier.length;
	    _e.value = _data_ptr->identifier.value;
	    ossEncodedOidToAsnVal(_g, &_e, &_val);
	    ossPrint(_g, "%.*s", (int)_val.length, _val.value);
	    (*_g->freep)(_val.value);
	}
	_comma = 1;
	if (_data_ptr->bit_mask & Org_significance_present) {
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "significance ");
	    ossPrint(_g, _data_ptr->significance ? "TRUE" : "FALSE");
	}

	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "information ");
	_oss_prt_oct(_g, _data_ptr->information.value, _data_ptr->information.length);
	_oss_indent(_g, -1);
	ossPrint(_g, "}");
    }

    return(PDU_PRINTED);
}

static int _p_Org_Diagnostics(struct ossGlobal *_g, Org_Diagnostics *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	ossPrint(_g, "gsm0408Cause : ");
	{
	    LONG_LONG value = _data_ptr->u.gsm0408Cause;

	    ossPrint(_g, LLONG_FMT, value);
	}
	break;

    case 2:
	ossPrint(_g, "gsm0902MapErrorValue : ");
	{
	    LONG_LONG value = _data_ptr->u.gsm0902MapErrorValue;

	    ossPrint(_g, LLONG_FMT, value);
	}
	break;

    case 3:
	ossPrint(_g, "ccittQ767Cause : ");
	{
	    LONG_LONG value = _data_ptr->u.ccittQ767Cause;

	    ossPrint(_g, LLONG_FMT, value);
	}
	break;

    case 4:
	ossPrint(_g, "networkSpecificCause : ");
	_p_Org_ManagementExtension(_g, &_data_ptr->u.networkSpecificCause);
	break;

    case 5:
	ossPrint(_g, "manufacturerSpecificCause : ");
	_p_Org_ManagementExtension(_g, &_data_ptr->u.manufacturerSpecificCause);
	break;

    default:
	ossPrint(_g, "<unknown choice>");
    }

    return(PDU_PRINTED);
}

static int _p_Org_ManagementExtensions(struct ossGlobal *_g, Org_ManagementExtensions *_data_ptr)
{
    _oss_indent(_g, 1);
    ossPrint(_g, "{");
    if (*_data_ptr) {
	struct Org_ManagementExtensions_	*_tempa = *_data_ptr;
	struct Org_ManagementExtensions_	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    _oss_indent(_g, 0);
	    _p_Org_ManagementExtension(_g, &_tempa->value);
	    _tempa = _tempb;
	    if (_tempa)
		ossPrint(_g, ",");
	}
    }

    _oss_indent(_g, -1);
    ossPrint(_g, "}");
    return(PDU_PRINTED);
}

static int _pOrg_CallEventRecord(struct ossGlobal *_g, void *_data)
{
    Org_CallEventRecord	*_data_ptr;
    _data_ptr = (Org_CallEventRecord *) _data;
    switch (_data_ptr->choice) {
    case 1:
	ossPrint(_g, "sgsnPDPRecord : ");
	{
	    short _comma = 0;

	    _oss_indent(_g, 1);
	    ossPrint(_g, "{");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordType ");
	    {
		LONG_LONG value = _data_ptr->u.sgsnPDPRecord.recordType;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    _comma = 1;
	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_networkInitiation_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "networkInitiation ");
		ossPrint(_g, _data_ptr->u.sgsnPDPRecord.networkInitiation ? "TRUE" : "FALSE");
	    }

	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_anonymousAccessIndicator_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "anonymousAccessIndicator ");
		ossPrint(_g, _data_ptr->u.sgsnPDPRecord.anonymousAccessIndicator ? "TRUE" : "FALSE");
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "servedIMSI ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnPDPRecord.servedIMSI.value, _data_ptr->u.sgsnPDPRecord.servedIMSI.length);
	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_servedIMEI_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "servedIMEI ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnPDPRecord.servedIMEI.value, _data_ptr->u.sgsnPDPRecord.servedIMEI.length);
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "sgsnAddress ");
	    _p_Org_IPAddress(_g, &_data_ptr->u.sgsnPDPRecord.sgsnAddress);
	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_msNetworkCapability_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "msNetworkCapability ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnPDPRecord.msNetworkCapability.value, _data_ptr->u.sgsnPDPRecord.msNetworkCapability.length);
	    }

	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_routingArea_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "routingArea ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnPDPRecord.routingArea.value, _data_ptr->u.sgsnPDPRecord.routingArea.length);
	    }

	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_locationAreaCode_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "locationAreaCode ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnPDPRecord.locationAreaCode.value, _data_ptr->u.sgsnPDPRecord.locationAreaCode.length);
	    }

	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_cellIdentity_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "cellIdentity ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnPDPRecord.cellIdentity.value, _data_ptr->u.sgsnPDPRecord.cellIdentity.length);
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "chargingID ");
	    {
		ULONG_LONG value = _data_ptr->u.sgsnPDPRecord.chargingID;
		ossPrint(_g, ULLONG_FMT, value);
	    }
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "ggsnAddressUsed ");
	    _p_Org_IPAddress(_g, &_data_ptr->u.sgsnPDPRecord.ggsnAddressUsed);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "accessPointNameNI ");
	    _oss_prt_nchar(_g, (char *)_data_ptr->u.sgsnPDPRecord.accessPointNameNI);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "pdpType ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnPDPRecord.pdpType.value, _data_ptr->u.sgsnPDPRecord.pdpType.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "servedPDPAddress ");
	    _p_Org_PDPAddress(_g, &_data_ptr->u.sgsnPDPRecord.servedPDPAddress);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "listOfTrafficVolumes ");
	    _oss_indent(_g, 1);
	    ossPrint(_g, "{");
	    if (_data_ptr->u.sgsnPDPRecord.listOfTrafficVolumes) {
		struct Org__seqof1_	*_tempa = _data_ptr->u.sgsnPDPRecord.listOfTrafficVolumes;
		struct Org__seqof1_	*_tempb;
		while (_tempa) {
		    _tempb = _tempa->next;
		    _oss_indent(_g, 0);
		    _p_Org_ChangeOfCharCondition(_g, &_tempa->value);
		    _tempa = _tempb;
		    if (_tempa)
			ossPrint(_g, ",");
		}
	    }

	    _oss_indent(_g, -1);
	    ossPrint(_g, "}");
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordOpeningTime ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnPDPRecord.recordOpeningTime.value, _data_ptr->u.sgsnPDPRecord.recordOpeningTime.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "duration ");
	    {
		LONG_LONG value = _data_ptr->u.sgsnPDPRecord.duration;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_sgsnChange_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "sgsnChange ");
		ossPrint(_g, _data_ptr->u.sgsnPDPRecord.sgsnChange ? "TRUE" : "FALSE");
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "causeForRecClosing ");
	    {
		LONG_LONG value = _data_ptr->u.sgsnPDPRecord.causeForRecClosing;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_diagnostics_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "diagnostics ");
		_p_Org_Diagnostics(_g, &_data_ptr->u.sgsnPDPRecord.diagnostics);
	    }

	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_recordSequenceNumber_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "recordSequenceNumber ");
		{
		    LONG_LONG value = _data_ptr->u.sgsnPDPRecord.recordSequenceNumber;

		    ossPrint(_g, LLONG_FMT, value);
		}
	    }

	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_nodeID_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "nodeID ");
		_oss_prt_nchar(_g, (char *)_data_ptr->u.sgsnPDPRecord.nodeID);
	    }

	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_recordExtensions_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "recordExtensions ");
		_p_Org_ManagementExtensions(_g, &_data_ptr->u.sgsnPDPRecord.recordExtensions);
	    }

	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_localSequenceNumber_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "localSequenceNumber ");
		{
		    ULONG_LONG value = _data_ptr->u.sgsnPDPRecord.localSequenceNumber;
		    ossPrint(_g, ULLONG_FMT, value);
		}
	    }

	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_apnSelectionMode_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "apnSelectionMode ");
		{
		    ULONG_LONG value = _data_ptr->u.sgsnPDPRecord.apnSelectionMode;
		    ossPrint(_g, ULLONG_FMT, value);
		}
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "accessPointNameOI ");
	    _oss_prt_nchar(_g, (char *)_data_ptr->u.sgsnPDPRecord.accessPointNameOI);
	    if (_data_ptr->u.sgsnPDPRecord.bit_mask & Org_SGSNPDPRecord_servedMSISDN_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "servedMSISDN ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnPDPRecord.servedMSISDN.value, _data_ptr->u.sgsnPDPRecord.servedMSISDN.length);
	    }

	    _oss_indent(_g, -1);
	    ossPrint(_g, "}");
	}

	break;

    case 2:
	ossPrint(_g, "ggsnPDPRecord : ");
	{
	    short _comma = 0;

	    _oss_indent(_g, 1);
	    ossPrint(_g, "{");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordType ");
	    {
		LONG_LONG value = _data_ptr->u.ggsnPDPRecord.recordType;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    _comma = 1;
	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_networkInitiation_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "networkInitiation ");
		ossPrint(_g, _data_ptr->u.ggsnPDPRecord.networkInitiation ? "TRUE" : "FALSE");
	    }

	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_anonymousAccessIndicator_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "anonymousAccessIndicator ");
		ossPrint(_g, _data_ptr->u.ggsnPDPRecord.anonymousAccessIndicator ? "TRUE" : "FALSE");
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "servedIMSI ");
	    _oss_prt_oct(_g, _data_ptr->u.ggsnPDPRecord.servedIMSI.value, _data_ptr->u.ggsnPDPRecord.servedIMSI.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "ggsnAddress ");
	    _p_Org_IPAddress(_g, &_data_ptr->u.ggsnPDPRecord.ggsnAddress);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "chargingID ");
	    {
		ULONG_LONG value = _data_ptr->u.ggsnPDPRecord.chargingID;
		ossPrint(_g, ULLONG_FMT, value);
	    }
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "sgsnAddress ");
	    _oss_indent(_g, 1);
	    ossPrint(_g, "{");
	    if (_data_ptr->u.ggsnPDPRecord.sgsnAddress) {
		struct Org__seqof2	*_tempa = _data_ptr->u.ggsnPDPRecord.sgsnAddress;
		struct Org__seqof2	*_tempb;
		while (_tempa) {
		    _tempb = _tempa->next;
		    _oss_indent(_g, 0);
		    _p_Org_IPAddress(_g, &_tempa->value);
		    _tempa = _tempb;
		    if (_tempa)
			ossPrint(_g, ",");
		}
	    }

	    _oss_indent(_g, -1);
	    ossPrint(_g, "}");
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "accessPointNameNI ");
	    _oss_prt_nchar(_g, (char *)_data_ptr->u.ggsnPDPRecord.accessPointNameNI);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "pdpType ");
	    _oss_prt_oct(_g, _data_ptr->u.ggsnPDPRecord.pdpType.value, _data_ptr->u.ggsnPDPRecord.pdpType.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "servedPDPAddress ");
	    _p_Org_PDPAddress(_g, &_data_ptr->u.ggsnPDPRecord.servedPDPAddress);
	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_remotePDPAddress_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "remotePDPAddress ");
		_oss_indent(_g, 1);
		ossPrint(_g, "{");
		if (_data_ptr->u.ggsnPDPRecord.remotePDPAddress) {
		    struct Org__seqof3	*_tempa = _data_ptr->u.ggsnPDPRecord.remotePDPAddress;
		    struct Org__seqof3	*_tempb;
		    while (_tempa) {
			_tempb = _tempa->next;
			_oss_indent(_g, 0);
			_p_Org_PDPAddress(_g, &_tempa->value);
			_tempa = _tempb;
			if (_tempa)
			    ossPrint(_g, ",");
		    }
		}

		_oss_indent(_g, -1);
		ossPrint(_g, "}");
	    }

	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_dynamicAddressFlag_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "dynamicAddressFlag ");
		ossPrint(_g, _data_ptr->u.ggsnPDPRecord.dynamicAddressFlag ? "TRUE" : "FALSE");
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "listOfTrafficVolumes ");
	    _oss_indent(_g, 1);
	    ossPrint(_g, "{");
	    if (_data_ptr->u.ggsnPDPRecord.listOfTrafficVolumes) {
		struct Org__seqof1_	*_tempa = _data_ptr->u.ggsnPDPRecord.listOfTrafficVolumes;
		struct Org__seqof1_	*_tempb;
		while (_tempa) {
		    _tempb = _tempa->next;
		    _oss_indent(_g, 0);
		    _p_Org_ChangeOfCharCondition(_g, &_tempa->value);
		    _tempa = _tempb;
		    if (_tempa)
			ossPrint(_g, ",");
		}
	    }

	    _oss_indent(_g, -1);
	    ossPrint(_g, "}");
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordOpeningTime ");
	    _oss_prt_oct(_g, _data_ptr->u.ggsnPDPRecord.recordOpeningTime.value, _data_ptr->u.ggsnPDPRecord.recordOpeningTime.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "duration ");
	    {
		LONG_LONG value = _data_ptr->u.ggsnPDPRecord.duration;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "causeForRecClosing ");
	    {
		LONG_LONG value = _data_ptr->u.ggsnPDPRecord.causeForRecClosing;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_diagnostics_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "diagnostics ");
		_p_Org_Diagnostics(_g, &_data_ptr->u.ggsnPDPRecord.diagnostics);
	    }

	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_recordSequenceNumber_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "recordSequenceNumber ");
		{
		    LONG_LONG value = _data_ptr->u.ggsnPDPRecord.recordSequenceNumber;

		    ossPrint(_g, LLONG_FMT, value);
		}
	    }

	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_nodeID_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "nodeID ");
		_oss_prt_nchar(_g, (char *)_data_ptr->u.ggsnPDPRecord.nodeID);
	    }

	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_recordExtensions_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "recordExtensions ");
		_p_Org_ManagementExtensions(_g, &_data_ptr->u.ggsnPDPRecord.recordExtensions);
	    }

	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_localSequenceNumber_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "localSequenceNumber ");
		{
		    ULONG_LONG value = _data_ptr->u.ggsnPDPRecord.localSequenceNumber;
		    ossPrint(_g, ULLONG_FMT, value);
		}
	    }

	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_apnSelectionMode_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "apnSelectionMode ");
		{
		    ULONG_LONG value = _data_ptr->u.ggsnPDPRecord.apnSelectionMode;
		    ossPrint(_g, ULLONG_FMT, value);
		}
	    }

	    if (_data_ptr->u.ggsnPDPRecord.bit_mask & Org_GGSNPDPRecord_servedMSISDN_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "servedMSISDN ");
		_oss_prt_oct(_g, _data_ptr->u.ggsnPDPRecord.servedMSISDN.value, _data_ptr->u.ggsnPDPRecord.servedMSISDN.length);
	    }

	    _oss_indent(_g, -1);
	    ossPrint(_g, "}");
	}

	break;

    case 3:
	ossPrint(_g, "sgsnMMRecord : ");
	{
	    short _comma = 0;

	    _oss_indent(_g, 1);
	    ossPrint(_g, "{");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordType ");
	    {
		LONG_LONG value = _data_ptr->u.sgsnMMRecord.recordType;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    _comma = 1;
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "servedIMSI ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnMMRecord.servedIMSI.value, _data_ptr->u.sgsnMMRecord.servedIMSI.length);
	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_servedIMEI_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "servedIMEI ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnMMRecord.servedIMEI.value, _data_ptr->u.sgsnMMRecord.servedIMEI.length);
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "sgsnAddress ");
	    _p_Org_IPAddress(_g, &_data_ptr->u.sgsnMMRecord.sgsnAddress);
	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_msNetworkCapability_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "msNetworkCapability ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnMMRecord.msNetworkCapability.value, _data_ptr->u.sgsnMMRecord.msNetworkCapability.length);
	    }

	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_routingArea_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "routingArea ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnMMRecord.routingArea.value, _data_ptr->u.sgsnMMRecord.routingArea.length);
	    }

	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_locationAreaCode_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "locationAreaCode ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnMMRecord.locationAreaCode.value, _data_ptr->u.sgsnMMRecord.locationAreaCode.length);
	    }

	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_cellIdentity_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "cellIdentity ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnMMRecord.cellIdentity.value, _data_ptr->u.sgsnMMRecord.cellIdentity.length);
	    }

	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_changeLocation_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "changeLocation ");
		_oss_indent(_g, 1);
		ossPrint(_g, "{");
		if (_data_ptr->u.sgsnMMRecord.changeLocation) {
		    struct Org__seqof4	*_tempa = _data_ptr->u.sgsnMMRecord.changeLocation;
		    struct Org__seqof4	*_tempb;
		    while (_tempa) {
			_tempb = _tempa->next;
			_oss_indent(_g, 0);
			{
			    short _comma = 0;

			    _oss_indent(_g, 1);
			    ossPrint(_g, "{");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "locationAreaCode ");
			    _oss_prt_oct(_g, _tempa->value.locationAreaCode.value, _tempa->value.locationAreaCode.length);
			    _comma = 1;
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "routingAreaCode ");
			    _oss_prt_oct(_g, _tempa->value.routingAreaCode.value, _tempa->value.routingAreaCode.length);
			    if (_tempa->value.bit_mask & Org_cellId_present) {
				ossPrint(_g, ",");
				_oss_indent(_g, 0);
				ossPrint(_g, "cellId ");
				_oss_prt_oct(_g, _tempa->value.cellId.value, _tempa->value.cellId.length);
			    }

			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "changeTime ");
			    _oss_prt_oct(_g, _tempa->value.changeTime.value, _tempa->value.changeTime.length);
			    _oss_indent(_g, -1);
			    ossPrint(_g, "}");
			}

			_tempa = _tempb;
			if (_tempa)
			    ossPrint(_g, ",");
		    }
		}

		_oss_indent(_g, -1);
		ossPrint(_g, "}");
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordOpeningTime ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnMMRecord.recordOpeningTime.value, _data_ptr->u.sgsnMMRecord.recordOpeningTime.length);
	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_duration_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "duration ");
		{
		    LONG_LONG value = _data_ptr->u.sgsnMMRecord.duration;

		    ossPrint(_g, LLONG_FMT, value);
		}
	    }

	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_sgsnChange_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "sgsnChange ");
		ossPrint(_g, _data_ptr->u.sgsnMMRecord.sgsnChange ? "TRUE" : "FALSE");
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "causeForRecClosing ");
	    {
		LONG_LONG value = _data_ptr->u.sgsnMMRecord.causeForRecClosing;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_diagnostics_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "diagnostics ");
		_p_Org_Diagnostics(_g, &_data_ptr->u.sgsnMMRecord.diagnostics);
	    }

	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_recordSequenceNumber_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "recordSequenceNumber ");
		{
		    LONG_LONG value = _data_ptr->u.sgsnMMRecord.recordSequenceNumber;

		    ossPrint(_g, LLONG_FMT, value);
		}
	    }

	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_nodeID_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "nodeID ");
		_oss_prt_nchar(_g, (char *)_data_ptr->u.sgsnMMRecord.nodeID);
	    }

	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_recordExtensions_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "recordExtensions ");
		_p_Org_ManagementExtensions(_g, &_data_ptr->u.sgsnMMRecord.recordExtensions);
	    }

	    if (_data_ptr->u.sgsnMMRecord.bit_mask & Org_SGSNMMRecord_localSequenceNumber_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "localSequenceNumber ");
		{
		    ULONG_LONG value = _data_ptr->u.sgsnMMRecord.localSequenceNumber;
		    ossPrint(_g, ULLONG_FMT, value);
		}
	    }

	    _oss_indent(_g, -1);
	    ossPrint(_g, "}");
	}

	break;

    case 4:
	ossPrint(_g, "sgsnSMORecord : ");
	{
	    short _comma = 0;

	    _oss_indent(_g, 1);
	    ossPrint(_g, "{");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordType ");
	    {
		LONG_LONG value = _data_ptr->u.sgsnSMORecord.recordType;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    _comma = 1;
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "servedIMSI ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.servedIMSI.value, _data_ptr->u.sgsnSMORecord.servedIMSI.length);
	    if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_servedIMEI_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "servedIMEI ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.servedIMEI.value, _data_ptr->u.sgsnSMORecord.servedIMEI.length);
	    }

	    if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_servedMSISDN_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "servedMSISDN ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.servedMSISDN.value, _data_ptr->u.sgsnSMORecord.servedMSISDN.length);
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "msNetworkCapability ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.msNetworkCapability.value, _data_ptr->u.sgsnSMORecord.msNetworkCapability.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "serviceCentre ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.serviceCentre.value, _data_ptr->u.sgsnSMORecord.serviceCentre.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordingEntity ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.recordingEntity.value, _data_ptr->u.sgsnSMORecord.recordingEntity.length);
	    if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_locationArea_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "locationArea ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.locationArea.value, _data_ptr->u.sgsnSMORecord.locationArea.length);
	    }

	    if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_routingArea_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "routingArea ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.routingArea.value, _data_ptr->u.sgsnSMORecord.routingArea.length);
	    }

	    if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_cellIdentity_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "cellIdentity ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.cellIdentity.value, _data_ptr->u.sgsnSMORecord.cellIdentity.length);
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "messageReference ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.messageReference.value, _data_ptr->u.sgsnSMORecord.messageReference.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "originationTime ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMORecord.originationTime.value, _data_ptr->u.sgsnSMORecord.originationTime.length);
	    if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_smsResult_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "smsResult ");
		_p_Org_Diagnostics(_g, &_data_ptr->u.sgsnSMORecord.smsResult);
	    }

	    if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_recordExtensions_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "recordExtensions ");
		_p_Org_ManagementExtensions(_g, &_data_ptr->u.sgsnSMORecord.recordExtensions);
	    }

	    if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_nodeID_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "nodeID ");
		_oss_prt_nchar(_g, (char *)_data_ptr->u.sgsnSMORecord.nodeID);
	    }

	    if (_data_ptr->u.sgsnSMORecord.bit_mask & Org_SGSNSMORecord_localSequenceNumber_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "localSequenceNumber ");
		{
		    ULONG_LONG value = _data_ptr->u.sgsnSMORecord.localSequenceNumber;
		    ossPrint(_g, ULLONG_FMT, value);
		}
	    }

	    _oss_indent(_g, -1);
	    ossPrint(_g, "}");
	}

	break;

    case 5:
	ossPrint(_g, "sgsnSMTRecord : ");
	{
	    short _comma = 0;

	    _oss_indent(_g, 1);
	    ossPrint(_g, "{");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordType ");
	    {
		LONG_LONG value = _data_ptr->u.sgsnSMTRecord.recordType;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    _comma = 1;
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "servedIMSI ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.servedIMSI.value, _data_ptr->u.sgsnSMTRecord.servedIMSI.length);
	    if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_servedIMEI_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "servedIMEI ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.servedIMEI.value, _data_ptr->u.sgsnSMTRecord.servedIMEI.length);
	    }

	    if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_servedMSISDN_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "servedMSISDN ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.servedMSISDN.value, _data_ptr->u.sgsnSMTRecord.servedMSISDN.length);
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "msNetworkCapability ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.msNetworkCapability.value, _data_ptr->u.sgsnSMTRecord.msNetworkCapability.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "serviceCentre ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.serviceCentre.value, _data_ptr->u.sgsnSMTRecord.serviceCentre.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "recordingEntity ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.recordingEntity.value, _data_ptr->u.sgsnSMTRecord.recordingEntity.length);
	    if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_locationArea_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "locationArea ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.locationArea.value, _data_ptr->u.sgsnSMTRecord.locationArea.length);
	    }

	    if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_routingArea_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "routingArea ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.routingArea.value, _data_ptr->u.sgsnSMTRecord.routingArea.length);
	    }

	    if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_cellIdentity_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "cellIdentity ");
		_oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.cellIdentity.value, _data_ptr->u.sgsnSMTRecord.cellIdentity.length);
	    }

	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "originationTime ");
	    _oss_prt_oct(_g, _data_ptr->u.sgsnSMTRecord.originationTime.value, _data_ptr->u.sgsnSMTRecord.originationTime.length);
	    if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_smsResult_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "smsResult ");
		_p_Org_Diagnostics(_g, &_data_ptr->u.sgsnSMTRecord.smsResult);
	    }

	    if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_recordExtensions_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "recordExtensions ");
		_p_Org_ManagementExtensions(_g, &_data_ptr->u.sgsnSMTRecord.recordExtensions);
	    }

	    if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_nodeID_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "nodeID ");
		_oss_prt_nchar(_g, (char *)_data_ptr->u.sgsnSMTRecord.nodeID);
	    }

	    if (_data_ptr->u.sgsnSMTRecord.bit_mask & Org_SGSNSMTRecord_localSequenceNumber_present) {
		ossPrint(_g, ",");
		_oss_indent(_g, 0);
		ossPrint(_g, "localSequenceNumber ");
		{
		    ULONG_LONG value = _data_ptr->u.sgsnSMTRecord.localSequenceNumber;
		    ossPrint(_g, ULLONG_FMT, value);
		}
	    }

	    _oss_indent(_g, -1);
	    ossPrint(_g, "}");
	}

	break;

    default:
	ossPrint(_g, "<unknown choice>");
    }

    return(PDU_PRINTED);
}

static int _pmCDR2G(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl)
{
    int _rc = 0;

    if (_pdunum < 0  && !(_g->decodingFlags & RESERVED_FLAG1))
	return(PDU_RANGE);
    switch (_pdunum < 0 ? -_pdunum : _pdunum)
    {
	case 1:
	    ossPrint(_g, "%sCallEventRecord %s ", _pdunum < 0 ? "" : "value ", _pdunum < 0 ? ":" : "::=");
	    _rc = _pOrg_CallEventRecord(_g, _data);
	    break;

	default:  return(PDU_RANGE);
    }
    if (_pdunum > 0)
	ossPrint(_g, "\n");
    return _rc;
}

#endif

/* Entry points */
static const _entry_point_struct _entry_points = {_ossinit_CDRF_2GCDR_Org, _emCDR2G, _dmCDR2G, _fmCDR2G, 16,
 NULL, NULL, 0, 0, NULL
#ifdef OSSPRINT
, _pmCDR2G
#endif
};
static void _oss_post_init(struct ossGlobal *world) {
    static const unsigned char _oss_typeinfo[] = {
        0x00, 0x75, 0x33, 0x30, 0xf4, 0x02, 0x8a, 0xad, 0x16, 0xf4,
        0xab, 0xb5, 0x16, 0xe9, 0xab, 0x04, 0x5c, 0x67, 0x59, 0x93,
        0x69, 0x42, 0x5e, 0x01, 0x20, 0xe6, 0x04, 0x0a, 0x92, 0x77,
        0x92, 0xc1, 0x98, 0xfd, 0x1a, 0x83, 0xad, 0x90, 0xd6, 0xaa,
        0x38, 0x90, 0x4f, 0x00, 0xb0, 0x51, 0xfd, 0xbf, 0x59, 0x28,
        0xd2, 0x9c, 0x1e, 0x5a, 0x71, 0x82, 0x38, 0xb9, 0x0b, 0xb6,
        0x1e, 0x78, 0x6e, 0x0a, 0x1d, 0xeb, 0xc7, 0x0b, 0x58, 0xbd,
        0xd7, 0xe6, 0x83, 0x1e, 0x7a, 0xc3, 0x2c, 0x5e, 0xab, 0xfd,
        0x98, 0xa7, 0x34, 0xe7, 0x8f, 0x34, 0x50, 0x58, 0xa7, 0x35,
        0xcc, 0x63, 0xc7, 0x74, 0x1e, 0x26, 0x57, 0xf0, 0x56, 0xc0,
        0xc3, 0x8b, 0xdf, 0xa4, 0x62, 0xc1, 0x85, 0x3f, 0x65, 0x48,
        0x5a, 0x5e, 0x67, 0x3d, 0xe6, 0x57, 0x3a, 0xfc, 0x43
    };
    ossInitRootContext(world, (unsigned char *)_oss_typeinfo);
}

#ifdef _OSSGETHEADER
void *DLL_ENTRY_FDEF ossGetHeader()
{
    return (void *)&_entry_points;
}
#endif /* _OSSGETHEADER */

void *CDRF_2GCDR_Org = (void *)&_entry_points;
