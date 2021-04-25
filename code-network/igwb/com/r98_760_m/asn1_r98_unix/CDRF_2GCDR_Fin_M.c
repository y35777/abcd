/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_2GCDR_Fin_M */
/* Created: Mon Mar 24 10:39:03 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Solaris 2.3 or later */
/* Target machine type: SPARC */
/* C compiler options required: None */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_2GCDR_Fin_M.c -headerfile CDRF_2GCDR_Fin_M.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix Fin_M_ -dualheader -ansi -verbose -ber -root
 * D:\application\ossasn1\solaris\asn1dflt.solaris-sparc
 * D:\project\530\asn1app\maimaoshi\CDRF_2GCDR_Fin_M.asn
 */

#include   <stddef.h>
#include   <string.h>
#include   <stdio.h>
#include   <limits.h>
#include   "asn1code.h"
#include   "asn1util.h"
#include   "ossber.h"
#include   "../asn1_r98/CDRF_2GCDR_Fin_M.h"

#if ((OSS_TOED_API_LEVEL + 0) < 3)
    #define _oss_dec_getmem_internal	_oss_dec_getmem
#endif /* OSS_TOED_API_LEVEL */

int Fin_M_maxAddressLength = 20;

int Fin_M_maxISDN_AddressLength = 10;

static ossBoolean _v0 = FALSE;


extern void _oss_run_TOED_on_SPARC_SOLARIS_23(void);
static void _oss_post_init(struct ossGlobal *world);

static void _ossinit_CDRF_2GCDR_Fin_M(struct ossGlobal *world) {
    _oss_run_TOED_on_SPARC_SOLARIS_23();
    ossLinkBer(world);
    _oss_post_init(world);
}

static long _edFin_M_IPBinaryAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_IPBinaryAddress *_in_data);
static void _d_Fin_M_IPBinaryAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_IPBinaryAddress *_out_data);
static int _f_Fin_M_IPBinaryAddress(struct ossGlobal *_g, Fin_M_IPBinaryAddress *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_IPBinaryAddress(struct ossGlobal *_g, Fin_M_IPBinaryAddress *_data_ptr);
#endif
static long _ed0Fin_M_IPTextRepresentedAddr(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_IPTextRepresentedAddress *_in_data);
static void _d_0Fin_M_IPTextRepresentedAddr(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_IPTextRepresentedAddress *_out_data);
static int _f_0Fin_M_IPTextRepresentedAddr(struct ossGlobal *_g, Fin_M_IPTextRepresentedAddress *_data_ptr);
#ifdef OSSPRINT
static int _p_0Fin_M_IPTextRepresentedAddr(struct ossGlobal *_g, Fin_M_IPTextRepresentedAddress *_data_ptr);
#endif
static long _edFin_M_IPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_IPAddress *_in_data);
static void _d_Fin_M_IPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_IPAddress *_out_data);
static int _f_Fin_M_IPAddress(struct ossGlobal *_g, Fin_M_IPAddress *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_IPAddress(struct ossGlobal *_g, Fin_M_IPAddress *_data_ptr);
#endif
static long _edFin_M_PDPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_PDPAddress *_in_data);
static void _d_Fin_M_PDPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_PDPAddress *_out_data);
static int _f_Fin_M_PDPAddress(struct ossGlobal *_g, Fin_M_PDPAddress *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_PDPAddress(struct ossGlobal *_g, Fin_M_PDPAddress *_data_ptr);
#endif
static long _edFin_M_QoSInformation(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_QoSInformation *_in_data);
static void _d_Fin_M_QoSInformation(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_QoSInformation *_out_data);
static int _f_Fin_M_QoSInformation(struct ossGlobal *_g, Fin_M_QoSInformation *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_QoSInformation(struct ossGlobal *_g, Fin_M_QoSInformation *_data_ptr);
#endif
static long _edFin_M_ChangeOfCharCondition(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_ChangeOfCharCondition *_in_data);
static void _d_Fin_M_ChangeOfCharCondition(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_ChangeOfCharCondition *_out_data);
static int _f_Fin_M_ChangeOfCharCondition(struct ossGlobal *_g, Fin_M_ChangeOfCharCondition *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_ChangeOfCharCondition(struct ossGlobal *_g, Fin_M_ChangeOfCharCondition *_data_ptr);
#endif
static long _edFin_M_RECORDSequenceNumber(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_RECORDSequenceNumber *_in_data);
static void _d_Fin_M_RECORDSequenceNumber(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_RECORDSequenceNumber *_out_data);
static int _f_Fin_M_RECORDSequenceNumber(struct ossGlobal *_g, Fin_M_RECORDSequenceNumber *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_RECORDSequenceNumber(struct ossGlobal *_g, Fin_M_RECORDSequenceNumber *_data_ptr);
#endif
static long _edFin_M_ManagementExtension(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_ManagementExtension *_in_data);
static void _d_Fin_M_ManagementExtension(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_ManagementExtension *_out_data);
static int _f_Fin_M_ManagementExtension(struct ossGlobal *_g, Fin_M_ManagementExtension *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_ManagementExtension(struct ossGlobal *_g, Fin_M_ManagementExtension *_data_ptr);
#endif
static long _edFin_M_Diagnostics(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_Diagnostics *_in_data);
static void _d_Fin_M_Diagnostics(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_Diagnostics *_out_data);
static int _f_Fin_M_Diagnostics(struct ossGlobal *_g, Fin_M_Diagnostics *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_Diagnostics(struct ossGlobal *_g, Fin_M_Diagnostics *_data_ptr);
#endif
static long _edFin_M_ManagementExtensions(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_ManagementExtensions *_in_data);
static void _d_Fin_M_ManagementExtensions(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_ManagementExtensions *_out_data);
static int _f_Fin_M_ManagementExtensions(struct ossGlobal *_g, Fin_M_ManagementExtensions *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_ManagementExtensions(struct ossGlobal *_g, Fin_M_ManagementExtensions *_data_ptr);
#endif
static long _edFin_M_RecordNumberList(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_RecordNumberList *_in_data);
static void _d_Fin_M_RecordNumberList(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_RecordNumberList *_out_data);
static int _f_Fin_M_RecordNumberList(struct ossGlobal *_g, Fin_M_RecordNumberList *_data_ptr);
#ifdef OSSPRINT
static int _p_Fin_M_RecordNumberList(struct ossGlobal *_g, Fin_M_RecordNumberList *_data_ptr);
#endif
static int _eFin_M_BSRecord(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags);
static int _dFin_M_BSRecord(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen);
static int _fFin_M_BSRecord(struct ossGlobal *_g, void *_data);
#ifdef OSSPRINT
static int _pFin_M_BSRecord(struct ossGlobal *_g, void *_data);
#endif
static int _emCDR2G(struct ossGlobal *_g, int _pdunum, void *_inbuf, char **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH]);
static int _dmCDR2G(struct ossGlobal *_g, int *_pdunum, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH]);
static int _fmCDR2G(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl);
#ifdef OSSPRINT
static int _pmCDR2G(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl);
#endif



#if OSSDEBUG > 1
static const _Context_Id _context_id[] = {
    {"Fin_M_iPBinV4Address", "OCTET STRING", _octetstring_type},
    {"Fin_M_iPBinV6Address", "OCTET STRING", _octetstring_type},
    {"Fin_M_iPTextV4Address", "Character String", _charstring_type},
    {"Fin_M_iPTextV6Address", "Character String", _charstring_type},
    {"Fin_M_iPBinaryAddress", "IPBinaryAddress", _choice_type},
    {"Fin_M_iPTextRepresentedAddress", "IPTextRepresentedAddress", _choice_type},
    {"Fin_M_iPAddress", "IPAddress", _choice_type},
    {"Fin_M_eTSIAddress", "AddressString", _octetstring_type},
    {"meanThroughput", "QoSMeanThroughput", _enumerated_type},
    {"peakThroughput", "QoSPeakThroughput", _enumerated_type},
    {"precedence", "QoSPrecedence", _enumerated_type},
    {"delay", "QoSDelay", _enumerated_type},
    {"reliability", "QoSReliability", _enumerated_type},
    {"changeTime", "TimeStamp", _octetstring_type},
    {"changeCondition", "ChangeCondition", _enumerated_type},
    {"dataVolumeGPRSDownlink", "DataVolumeGPRS", _integer_type},
    {"dataVolumeGPRSUplink", "DataVolumeGPRS", _integer_type},
    {"Fin_M_qosNegotiated", "QoSInformation", _sequence_type},
    {"Fin_M_qosRequested", "QoSInformation", _sequence_type},
    {"recordNumberList", "RecordNumberList", _sequence_of_type},
    {"gsnAddress", "IPAddress", _choice_type},
    {"information", "ANY", _any_type},
    {"Fin_M_significance", "BOOLEAN", _boolean_type},
    {"identifier", "OBJECT IDENTIFIER", _object_identifier_type},
    {"Fin_M_gsm0408Cause", "INTEGER", _integer_type},
    {"Fin_M_gsm0902MapErrorValue", "INTEGER", _integer_type},
    {"Fin_M_ccittQ767Cause", "INTEGER", _integer_type},
    {"Fin_M_networkSpecificCause", "ManagementExtension", _sequence_type},
    {"Fin_M_manufacturerSpecificCause", "ManagementExtension", _sequence_type},
    {NULL, "ManagementExtension", _sequence_type},
    {NULL, "RecordNumber", _integer_type},
    {"BSRecord", "SEQUENCE", _sequence_type},
    {"listOfCDR", "SEQUENCE OF", _sequence_of_type},
    {NULL, "CallEventRecord", _choice_type},
    {"Fin_M_sgsnPDPRecord", "SGSNPDPRecord", _set_type},
    {"consolidationResult", "ConsolidationResult", _enumerated_type},
    {"Fin_M_SGSNPDPRecord_servedMSISDN", "ISDN-AddressString", _octetstring_type},
    {"accessPointNameOI", "AccessPointNameOI", _charstring_type},
    {"Fin_M_SGSNPDPRecord_apnSelectionMode", "APNSelectionMode", _enumerated_type},
    {"Fin_M_SGSNPDPRecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Fin_M_SGSNPDPRecord_nodeID", "NodeID", _charstring_type},
    {"Fin_M_SGSNPDPRecord_recordSequenceNumber", "SEQUENCE OF", _sequence_of_type},
    {NULL, "RECORDSequenceNumber", _sequence_type},
    {"causeForRecClosing", "CauseForRecClosing", _integer_type},
    {"Fin_M_SGSNPDPRecord_sgsnChange", "SGSNChange", _boolean_type},
    {"duration", "CallDuration", _integer_type},
    {"recordOpeningTime", "TimeStamp", _octetstring_type},
    {"listOfTrafficVolumes", "SEQUENCE OF", _sequence_of_type},
    {NULL, "ChangeOfCharCondition", _sequence_type},
    {"servedPDPAddress", "PDPAddress", _choice_type},
    {"pdpType", "PDPType", _octetstring_type},
    {"accessPointNameNI", "AccessPointNameNI", _charstring_type},
    {"ggsnAddressUsed", "IPAddress", _choice_type},
    {"chargingID", "ChargingID", _integer_type},
    {"Fin_M_SGSNPDPRecord_cellIdentity", "CellId", _octetstring_type},
    {"Fin_M_SGSNPDPRecord_locationAreaCode", "LocationAreaCode", _octetstring_type},
    {"Fin_M_SGSNPDPRecord_routingArea", "RoutingAreaCode", _octetstring_type},
    {"Fin_M_SGSNPDPRecord_msNetworkCapability", "MSNetworkCapability", _octetstring_type},
    {"sgsnAddress", "SEQUENCE OF", _sequence_of_type},
    {NULL, "IPAddress", _choice_type},
    {"Fin_M_SGSNPDPRecord_servedIMEI", "IMEI", _octetstring_type},
    {"servedIMSI", "IMSI", _octetstring_type},
    {"Fin_M_SGSNPDPRecord_anonymousAccessIndicator", "BOOLEAN", _boolean_type},
    {"Fin_M_SGSNPDPRecord_networkInitiation", "NetworkInitiatedPDPContext", _boolean_type},
    {"recordType", "CallEventRecordType", _integer_type},
    {"Fin_M_ggsnPDPRecord", "GGSNPDPRecord", _set_type},
    {"Fin_M_GGSNPDPRecord_servedMSISDN", "ISDN-AddressString", _octetstring_type},
    {"Fin_M_GGSNPDPRecord_apnSelectionMode", "APNSelectionMode", _enumerated_type},
    {"Fin_M_GGSNPDPRecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Fin_M_GGSNPDPRecord_nodeID", "NodeID", _charstring_type},
    {"Fin_M_GGSNPDPRecord_recordSequenceNumber", "RECORDSequenceNumber", _sequence_type},
    {"Fin_M_dynamicAddressFlag", "DynamicAddressFlag", _boolean_type},
    {"Fin_M_remotePDPAddress", "SEQUENCE OF", _sequence_of_type},
    {NULL, "PDPAddress", _choice_type},
    {"ggsnAddress", "IPAddress", _choice_type},
    {"Fin_M_GGSNPDPRecord_anonymousAccessIndicator", "BOOLEAN", _boolean_type},
    {"Fin_M_GGSNPDPRecord_networkInitiation", "NetworkInitiatedPDPContext", _boolean_type},
    {"Fin_M_sgsnMMRecord", "SGSNMMRecord", _set_type},
    {"Fin_M_SGSNMMRecord_localSequenceNumber", "LocalSequenceNumber", _integer_type},
    {"Fin_M_SGSNMMRecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Fin_M_SGSNMMRecord_nodeID", "NodeID", _charstring_type},
    {"Fin_M_SGSNMMRecord_recordSequenceNumber", "INTEGER", _integer_type},
    {"Fin_M_diagnostics", "Diagnostics", _choice_type},
    {"Fin_M_SGSNMMRecord_sgsnChange", "SGSNChange", _boolean_type},
    {"Fin_M_duration", "CallDuration", _integer_type},
    {"Fin_M_changeLocation", "SEQUENCE OF", _sequence_of_type},
    {NULL, "ChangeLocation", _sequence_type},
    {"Fin_M_cellId", "CellId", _octetstring_type},
    {"routingAreaCode", "RoutingAreaCode", _octetstring_type},
    {"locationAreaCode", "LocationAreaCode", _octetstring_type},
    {"Fin_M_SGSNMMRecord_cellIdentity", "CellId", _octetstring_type},
    {"Fin_M_SGSNMMRecord_locationAreaCode", "LocationAreaCode", _octetstring_type},
    {"Fin_M_SGSNMMRecord_routingArea", "RoutingAreaCode", _octetstring_type},
    {"Fin_M_SGSNMMRecord_msNetworkCapability", "MSNetworkCapability", _octetstring_type},
    {"sgsnAddress", "IPAddress", _choice_type},
    {"Fin_M_SGSNMMRecord_servedIMEI", "IMEI", _octetstring_type},
    {"Fin_M_sgsnSMORecord", "SGSNSMORecord", _set_type},
    {"Fin_M_SGSNSMORecord_localSequenceNumber", "LocalSequenceNumber", _integer_type},
    {"Fin_M_SGSNSMORecord_nodeID", "NodeID", _charstring_type},
    {"Fin_M_SGSNSMORecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Fin_M_SGSNSMORecord_smsResult", "Diagnostics", _choice_type},
    {"originationTime", "TimeStamp", _octetstring_type},
    {"messageReference", "MessageReference", _octetstring_type},
    {"Fin_M_SGSNSMORecord_cellIdentity", "CellId", _octetstring_type},
    {"Fin_M_SGSNSMORecord_routingArea", "RoutingAreaCode", _octetstring_type},
    {"Fin_M_SGSNSMORecord_locationArea", "LocationAreaCode", _octetstring_type},
    {"recordingEntity", "AddressString", _octetstring_type},
    {"serviceCentre", "AddressString", _octetstring_type},
    {"msNetworkCapability", "MSNetworkCapability", _octetstring_type},
    {"Fin_M_SGSNSMORecord_servedMSISDN", "ISDN-AddressString", _octetstring_type},
    {"Fin_M_SGSNSMORecord_servedIMEI", "IMEI", _octetstring_type},
    {"Fin_M_sgsnSMTRecord", "SGSNSMTRecord", _set_type},
    {"Fin_M_SGSNSMTRecord_localSequenceNumber", "LocalSequenceNumber", _integer_type},
    {"Fin_M_SGSNSMTRecord_nodeID", "NodeID", _charstring_type},
    {"Fin_M_SGSNSMTRecord_recordExtensions", "ManagementExtensions", _set_of_type},
    {"Fin_M_SGSNSMTRecord_smsResult", "Diagnostics", _choice_type},
    {"Fin_M_SGSNSMTRecord_cellIdentity", "CellId", _octetstring_type},
    {"Fin_M_SGSNSMTRecord_routingArea", "RoutingAreaCode", _octetstring_type},
    {"Fin_M_SGSNSMTRecord_locationArea", "LocationAreaCode", _octetstring_type},
    {"Fin_M_SGSNSMTRecord_servedMSISDN", "ISDN-AddressString", _octetstring_type},
    {"Fin_M_SGSNSMTRecord_servedIMEI", "IMEI", _octetstring_type},
    {"bsXferRecord", "BSXferSummaryRecord", _set_type},
    {"cgID", "OCTET STRING", _octetstring_type},
    {"duplicateCdr", "OCTET STRING", _octetstring_type},
    {"missingCdr", "OCTET STRING", _octetstring_type},
    {"totalCdr", "OCTET STRING", _octetstring_type},
    {"cdrType", "OCTET STRING", _octetstring_type},
    {"lrsnEnd", "OCTET STRING", _octetstring_type},
    {"lrsnStart", "OCTET STRING", _octetstring_type},
    {"closeTime", "OCTET STRING", _octetstring_type},
    {"openTime", "OCTET STRING", _octetstring_type},
    {"bsXferFile", "OCTET STRING", _octetstring_type},
    {"cgnID", "OCTET STRING", _octetstring_type}
};
#endif


static long _edFin_M_IPBinaryAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_IPBinaryAddress *_in_data)
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

static long _ed0Fin_M_IPTextRepresentedAddr(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_IPTextRepresentedAddress *_in_data)
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

static long _edFin_M_IPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_IPAddress *_in_data)
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

	    _data_len = _edFin_M_IPBinaryAddress(_g, _out_pos, &_out_len, &_in_data->u.iPBinaryAddress);
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

	    _data_len = _ed0Fin_M_IPTextRepresentedAddr(_g, _out_pos, &_out_len, &_in_data->u.iPTextRepresentedAddress);
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

static long _edFin_M_PDPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_PDPAddress *_in_data)
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

	    _data_len = _edFin_M_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.iPAddress);
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

static long _edFin_M_QoSInformation(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_QoSInformation *_in_data)
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

static long _edFin_M_ChangeOfCharCondition(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_ChangeOfCharCondition *_in_data)
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

	if (_in_data->bit_mask & Fin_M_qosNegotiated_present) {
	    _data_len = _edFin_M_QoSInformation(_g, _out_pos, &_out_len, &_in_data->qosNegotiated);
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

	if (_in_data->bit_mask & Fin_M_qosRequested_present) {
	    _data_len = _edFin_M_QoSInformation(_g, _out_pos, &_out_len, &_in_data->qosRequested);
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

static long _edFin_M_RECORDSequenceNumber(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_RECORDSequenceNumber *_in_data)
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

	_data_len = _edFin_M_RecordNumberList(_g, _out_pos, &_out_len, &_in_data->recordNumberList);
	_constructed = TRUE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[20];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _edFin_M_IPAddress(_g, _out_pos, &_out_len, &_in_data->gsnAddress);
	_constructed = TRUE;
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

static long _edFin_M_ManagementExtension(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_ManagementExtension *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[21];
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
	_element._id = (_Context_Id *)&_context_id[22];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & Fin_M_significance_present) {
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
	_element._id = (_Context_Id *)&_context_id[23];
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

static long _edFin_M_Diagnostics(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_Diagnostics *_in_data)
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
	    _element._id = (_Context_Id *)&_context_id[24];
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
	    _element._id = (_Context_Id *)&_context_id[25];
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
	    _element._id = (_Context_Id *)&_context_id[26];
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
	    _element._id = (_Context_Id *)&_context_id[27];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edFin_M_ManagementExtension(_g, _out_pos, &_out_len, &_in_data->u.networkSpecificCause);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 5:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[28];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edFin_M_ManagementExtension(_g, _out_pos, &_out_len, &_in_data->u.manufacturerSpecificCause);
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

static long _edFin_M_ManagementExtensions(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_ManagementExtensions *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	struct Fin_M_ManagementExtensions_	*_temp;
	unsigned long count;
	unsigned long _lindex;
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
	_element._id = (_Context_Id *)&_context_id[29];
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

	    _temp = (struct Fin_M_ManagementExtensions_ *) _oss_enc_pop(_g);
	    _data_len = _edFin_M_ManagementExtension(_g, _out_pos, &_out_len, &_temp->value);
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

static long _edFin_M_RecordNumberList(struct ossGlobal *_g, char **_out_pos, long *_max_len, Fin_M_RecordNumberList *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	struct Fin_M_RecordNumberList_	*_temp;
	unsigned long count;
	unsigned long _lindex;
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
	_element._id = (_Context_Id *)&_context_id[30];
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
	for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
	    _element._occurrence = count - _lindex;
#endif

	    _temp = (struct Fin_M_RecordNumberList_ *) _oss_enc_pop(_g);
	    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x2, _constructed);
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

static int _eFin_M_BSRecord(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    Fin_M_BSRecord	*_in_data;
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

    _in_data = (Fin_M_BSRecord *) _inbuf;
    if (!_oss_c->_buffer_provided) *_outbuf = NULL;
    _out_buf = *_outbuf;
    _out_len = _oss_c->_oss_outbuflen = (_oss_c->_buffer_provided) ? *_outlen : 0;
    _out_buf += _out_len;
#if OSSDEBUG > 1
    _oss_c->_oss_context_anchor._pdu_number = 1;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[31];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[32];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	{
	    struct Fin_M__seqof6	*_temp;
	    unsigned long count;
	    unsigned long _lindex;
	    long _total_len = 0;

#if OSSDEBUG > 1
	    _Context_Element _element;

	    _element._occurrence = 0;
	    _element._id = (_Context_Id *)&_context_id[33];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    for (_temp = _in_data->listOfCDR, count = 0; _temp; _temp = _temp->next) {
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

		_temp = (struct Fin_M__seqof6 *) _oss_enc_pop(_g);
		{
#if OSSDEBUG > 1
		    _Context_Element _element;

		    _element._occurrence = 0;
#endif

		    switch (_temp->value.choice) {
		    case 1:
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[34];
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
			    _element._id = (_Context_Id *)&_context_id[35];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.consolidationResult);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801E, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[36];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.servedMSISDN, 2, 10);
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
			    _element._id = (_Context_Id *)&_context_id[37];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.accessPointNameOI, 37);
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
			    _element._id = (_Context_Id *)&_context_id[38];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_apnSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.apnSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8019, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[39];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_recordExtensions_present) {
				_data_len = _edFin_M_ManagementExtensions(_g, _out_pos, &_out_len, &_temp->value.u.sgsnPDPRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
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

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_nodeID_present) {
				_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.nodeID, 20);
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
			    _element._id = (_Context_Id *)&_context_id[41];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_recordSequenceNumber_present) {
				{
				    struct Fin_M__seqof3	*_temp1;
				    unsigned long count;
				    unsigned long _lindex;
				    long _total_len = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
				    _element._id = (_Context_Id *)&_context_id[42];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    for (_temp1 = _temp->value.u.sgsnPDPRecord.recordSequenceNumber, count = 0; _temp1; _temp1 = _temp1->next) {
#if OSSDEBUG > 1
					_element._occurrence = count + 1;
#endif

					_oss_enc_push(_g, _temp1);
					count++;
				    }
				    for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
					_element._occurrence = count - _lindex;
#endif

					_temp1 = (struct Fin_M__seqof3 *) _oss_enc_pop(_g);
					_data_len = _edFin_M_RECORDSequenceNumber(_g, _out_pos, &_out_len, &_temp1->value);
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
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[43];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.causeForRecClosing);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[44];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_sgsnChange_present) {
				_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.sgsnChange);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8012, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[45];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.duration);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[46];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.recordOpeningTime, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[47];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    {
				struct Fin_M__seqof2_	*_temp1;
				unsigned long count;
				unsigned long _lindex;
				long _total_len = 0;

#if OSSDEBUG > 1
				_Context_Element _element;

				_element._occurrence = 0;
				_element._id = (_Context_Id *)&_context_id[48];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				for (_temp1 = _temp->value.u.sgsnPDPRecord.listOfTrafficVolumes, count = 0; _temp1; _temp1 = _temp1->next) {
#if OSSDEBUG > 1
				    _element._occurrence = count + 1;
#endif

				    _oss_enc_push(_g, _temp1);
				    count++;
				}
				for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
				    _element._occurrence = count - _lindex;
#endif

				    _temp1 = (struct Fin_M__seqof2_ *) _oss_enc_pop(_g);
				    _data_len = _edFin_M_ChangeOfCharCondition(_g, _out_pos, &_out_len, &_temp1->value);
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
			    _element._id = (_Context_Id *)&_context_id[49];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _edFin_M_PDPAddress(_g, _out_pos, &_out_len, &_temp->value.u.sgsnPDPRecord.servedPDPAddress);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[50];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.pdpType, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[51];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.accessPointNameNI, 63);
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
			    _element._id = (_Context_Id *)&_context_id[52];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _edFin_M_IPAddress(_g, _out_pos, &_out_len, &_temp->value.u.sgsnPDPRecord.ggsnAddressUsed);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[53];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.chargingID);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800A, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[54];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_cellIdentity_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.cellIdentity, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[55];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_locationAreaCode_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.locationAreaCode, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[56];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_routingArea_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.routingArea, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[57];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_msNetworkCapability_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.msNetworkCapability, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[58];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    {
				struct Fin_M__seqof1_	*_temp1;
				unsigned long count;
				unsigned long _lindex;
				long _total_len = 0;

#if OSSDEBUG > 1
				_Context_Element _element;

				_element._occurrence = 0;
				_element._id = (_Context_Id *)&_context_id[59];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				for (_temp1 = _temp->value.u.sgsnPDPRecord.sgsnAddress, count = 0; _temp1; _temp1 = _temp1->next) {
#if OSSDEBUG > 1
				    _element._occurrence = count + 1;
#endif

				    _oss_enc_push(_g, _temp1);
				    count++;
				}
				for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
				    _element._occurrence = count - _lindex;
#endif

				    _temp1 = (struct Fin_M__seqof1_ *) _oss_enc_pop(_g);
				    _data_len = _edFin_M_IPAddress(_g, _out_pos, &_out_len, &_temp1->value);
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
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[60];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_servedIMEI_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.servedIMEI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.servedIMSI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[62];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_anonymousAccessIndicator_present) {
				_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.anonymousAccessIndicator);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
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

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_networkInitiation_present) {
				_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.networkInitiation);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
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

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.recordType);
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
			_element._id = (_Context_Id *)&_context_id[65];
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
			    _element._id = (_Context_Id *)&_context_id[35];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.consolidationResult);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801E, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[66];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.ggsnPDPRecord.servedMSISDN, 2, 10);
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
			    _element._id = (_Context_Id *)&_context_id[67];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_apnSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.apnSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
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

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_recordExtensions_present) {
				_data_len = _edFin_M_ManagementExtensions(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[69];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_nodeID_present) {
				_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.nodeID, 20);
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
			    _element._id = (_Context_Id *)&_context_id[70];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_recordSequenceNumber_present) {
				_data_len = _edFin_M_RECORDSequenceNumber(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.recordSequenceNumber);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[43];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.causeForRecClosing);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[45];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.duration);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[46];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.ggsnPDPRecord.recordOpeningTime, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[47];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    {
				struct Fin_M__seqof2_	*_temp1;
				unsigned long count;
				unsigned long _lindex;
				long _total_len = 0;

#if OSSDEBUG > 1
				_Context_Element _element;

				_element._occurrence = 0;
				_element._id = (_Context_Id *)&_context_id[48];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				for (_temp1 = _temp->value.u.ggsnPDPRecord.listOfTrafficVolumes, count = 0; _temp1; _temp1 = _temp1->next) {
#if OSSDEBUG > 1
				    _element._occurrence = count + 1;
#endif

				    _oss_enc_push(_g, _temp1);
				    count++;
				}
				for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
				    _element._occurrence = count - _lindex;
#endif

				    _temp1 = (struct Fin_M__seqof2_ *) _oss_enc_pop(_g);
				    _data_len = _edFin_M_ChangeOfCharCondition(_g, _out_pos, &_out_len, &_temp1->value);
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
			    _element._id = (_Context_Id *)&_context_id[71];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_dynamicAddressFlag_present) {
				_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.dynamicAddressFlag);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[72];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_remotePDPAddress_present) {
				{
				    struct Fin_M__seqof4	*_temp1;
				    unsigned long count;
				    unsigned long _lindex;
				    long _total_len = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
				    _element._id = (_Context_Id *)&_context_id[73];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    for (_temp1 = _temp->value.u.ggsnPDPRecord.remotePDPAddress, count = 0; _temp1; _temp1 = _temp1->next) {
#if OSSDEBUG > 1
					_element._occurrence = count + 1;
#endif

					_oss_enc_push(_g, _temp1);
					count++;
				    }
				    for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
					_element._occurrence = count - _lindex;
#endif

					_temp1 = (struct Fin_M__seqof4 *) _oss_enc_pop(_g);
					_data_len = _edFin_M_PDPAddress(_g, _out_pos, &_out_len, &_temp1->value);
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
			    _element._id = (_Context_Id *)&_context_id[49];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _edFin_M_PDPAddress(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.servedPDPAddress);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[50];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.ggsnPDPRecord.pdpType, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[51];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.accessPointNameNI, 63);
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
			    _element._id = (_Context_Id *)&_context_id[58];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    {
				struct Fin_M__seqof1_	*_temp1;
				unsigned long count;
				unsigned long _lindex;
				long _total_len = 0;

#if OSSDEBUG > 1
				_Context_Element _element;

				_element._occurrence = 0;
				_element._id = (_Context_Id *)&_context_id[59];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				for (_temp1 = _temp->value.u.ggsnPDPRecord.sgsnAddress, count = 0; _temp1; _temp1 = _temp1->next) {
#if OSSDEBUG > 1
				    _element._occurrence = count + 1;
#endif

				    _oss_enc_push(_g, _temp1);
				    count++;
				}
				for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
				    _element._occurrence = count - _lindex;
#endif

				    _temp1 = (struct Fin_M__seqof1_ *) _oss_enc_pop(_g);
				    _data_len = _edFin_M_IPAddress(_g, _out_pos, &_out_len, &_temp1->value);
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
			    _element._id = (_Context_Id *)&_context_id[53];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.chargingID);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[74];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _edFin_M_IPAddress(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.ggsnAddress);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.ggsnPDPRecord.servedIMSI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[75];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_anonymousAccessIndicator_present) {
				_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.anonymousAccessIndicator);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[76];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_networkInitiation_present) {
				_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.networkInitiation);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
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

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.recordType);
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
			_element._id = (_Context_Id *)&_context_id[77];
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
			    _element._id = (_Context_Id *)&_context_id[78];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_localSequenceNumber_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.localSequenceNumber);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
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

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_recordExtensions_present) {
				_data_len = _edFin_M_ManagementExtensions(_g, _out_pos, &_out_len, &_temp->value.u.sgsnMMRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
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

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_nodeID_present) {
				_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.nodeID, 20);
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
			    _element._id = (_Context_Id *)&_context_id[81];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_recordSequenceNumber_present) {
				_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.recordSequenceNumber);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
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

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_diagnostics_present) {
				_data_len = _edFin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnMMRecord.diagnostics);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[43];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.causeForRecClosing);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[83];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_sgsnChange_present) {
				_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.sgsnChange);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[84];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_duration_present) {
				_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.duration);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800A, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[46];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.recordOpeningTime, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[85];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_changeLocation_present) {
				{
				    struct Fin_M__seqof5	*_temp1;
				    unsigned long count;
				    unsigned long _lindex;
				    long _total_len = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
				    _element._id = (_Context_Id *)&_context_id[86];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    for (_temp1 = _temp->value.u.sgsnMMRecord.changeLocation, count = 0; _temp1; _temp1 = _temp1->next) {
#if OSSDEBUG > 1
					_element._occurrence = count + 1;
#endif

					_oss_enc_push(_g, _temp1);
					count++;
				    }
				    for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
					_element._occurrence = count - _lindex;
#endif

					_temp1 = (struct Fin_M__seqof5 *) _oss_enc_pop(_g);
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

					    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp1->value.changeTime, 2, 9);
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
					    _element._id = (_Context_Id *)&_context_id[87];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp1->value.bit_mask & Fin_M_cellId_present) {
						_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp1->value.cellId, 2, 2);
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
					    _element._id = (_Context_Id *)&_context_id[88];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp1->value.routingAreaCode, 2, 1);
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
					    _element._id = (_Context_Id *)&_context_id[89];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp1->value.locationAreaCode, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[90];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_cellIdentity_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.cellIdentity, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[91];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_locationAreaCode_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.locationAreaCode, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[92];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_routingArea_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.routingArea, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[93];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_msNetworkCapability_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.msNetworkCapability, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[94];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _edFin_M_IPAddress(_g, _out_pos, &_out_len, &_temp->value.u.sgsnMMRecord.sgsnAddress);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[95];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_servedIMEI_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.servedIMEI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.servedIMSI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[64];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.recordType);
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
			_element._id = (_Context_Id *)&_context_id[96];
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
			    _element._id = (_Context_Id *)&_context_id[97];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_localSequenceNumber_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMORecord.localSequenceNumber);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
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

			    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_nodeID_present) {
				_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMORecord.nodeID, 20);
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
			    _element._id = (_Context_Id *)&_context_id[99];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_recordExtensions_present) {
				_data_len = _edFin_M_ManagementExtensions(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMORecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[100];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_smsResult_present) {
				_data_len = _edFin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMORecord.smsResult);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.originationTime, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[102];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.messageReference, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[103];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_cellIdentity_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.cellIdentity, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[104];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_routingArea_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.routingArea, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[105];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_locationArea_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.locationArea, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[106];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.recordingEntity, 2, 20);
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
			    _element._id = (_Context_Id *)&_context_id[107];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.serviceCentre, 2, 20);
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
			    _element._id = (_Context_Id *)&_context_id[108];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.msNetworkCapability, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[109];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.servedMSISDN, 2, 10);
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
			    _element._id = (_Context_Id *)&_context_id[110];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_servedIMEI_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.servedIMEI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.servedIMSI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[64];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMORecord.recordType);
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
			_element._id = (_Context_Id *)&_context_id[111];
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
			    _element._id = (_Context_Id *)&_context_id[112];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_localSequenceNumber_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMTRecord.localSequenceNumber);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[113];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_nodeID_present) {
				_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMTRecord.nodeID, 20);
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
			    _element._id = (_Context_Id *)&_context_id[114];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_recordExtensions_present) {
				_data_len = _edFin_M_ManagementExtensions(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMTRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
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

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_smsResult_present) {
				_data_len = _edFin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMTRecord.smsResult);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.originationTime, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[116];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_cellIdentity_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.cellIdentity, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[117];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_routingArea_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.routingArea, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[118];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_locationArea_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.locationArea, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[106];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.recordingEntity, 2, 20);
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
			    _element._id = (_Context_Id *)&_context_id[107];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.serviceCentre, 2, 20);
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
			    _element._id = (_Context_Id *)&_context_id[108];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.msNetworkCapability, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[119];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.servedMSISDN, 2, 10);
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
			    _element._id = (_Context_Id *)&_context_id[120];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_servedIMEI_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.servedIMEI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.servedIMSI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[64];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMTRecord.recordType);
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
			_oss_enc_error(_g, TRUE, _bad_choice, (long)_temp->value.choice);	/* Bad choice selector. */
		    }

		}

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
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[121];
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
	    _element._id = (_Context_Id *)&_context_id[122];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.cgID, 2, 15);
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
	    _element._id = (_Context_Id *)&_context_id[123];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.duplicateCdr, 2, 10);
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
	    _element._id = (_Context_Id *)&_context_id[124];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.missingCdr, 2, 10);
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
	    _element._id = (_Context_Id *)&_context_id[125];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.totalCdr, 2, 10);
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
	    _element._id = (_Context_Id *)&_context_id[126];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.cdrType, 2, 3);
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
	    _element._id = (_Context_Id *)&_context_id[127];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.lrsnEnd, 2, 10);
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
	    _element._id = (_Context_Id *)&_context_id[128];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.lrsnStart, 2, 10);
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
	    _element._id = (_Context_Id *)&_context_id[129];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.closeTime, 2, 14);
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
	    _element._id = (_Context_Id *)&_context_id[130];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.openTime, 2, 14);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
	    _total_len += _data_len;
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[131];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.bsXferFile, 2, 34);
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
	    _element._id = (_Context_Id *)&_context_id[132];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->bsXferRecord.cgnID, 2, 15);
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
	    _rc = _eFin_M_BSRecord(_g, _inbuf, _outbuf, _outlen, _flags);
	    break;

	default:
	    _oss_enc_error(_g, TRUE, _pdu_range, (long) _pdunum);
    }

    _oss_c->_oss_err_msg = NULL;
    return _rc;
}

static void _d_Fin_M_IPBinaryAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_IPBinaryAddress *_out_data)
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

static void _d_0Fin_M_IPTextRepresentedAddr(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_IPTextRepresentedAddress *_out_data)
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

static void _d_Fin_M_IPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_IPAddress *_out_data)
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
	    _d_Fin_M_IPBinaryAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPBinaryAddress);
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
	    _d_0Fin_M_IPTextRepresentedAddr(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPTextRepresentedAddress);
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

static void _d_Fin_M_PDPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_PDPAddress *_out_data)
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
		_d_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPAddress);
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

static void _d_Fin_M_QoSInformation(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_QoSInformation *_out_data)
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
	_out_data->reliability = (enum Fin_M_QoSReliability) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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
	_out_data->delay = (enum Fin_M_QoSDelay) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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
	_out_data->precedence = (enum Fin_M_QoSPrecedence) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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
	_out_data->peakThroughput = (enum Fin_M_QoSPeakThroughput) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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
	_out_data->meanThroughput = (enum Fin_M_QoSMeanThroughput) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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

static void _d_Fin_M_ChangeOfCharCondition(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_ChangeOfCharCondition *_out_data)
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
	    _out_data->bit_mask &= ~Fin_M_qosRequested_present;
	else {
	    _out_data->bit_mask |= Fin_M_qosRequested_present;
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
	    _d_Fin_M_QoSInformation(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->qosRequested);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (!_oss_c->_tag_decoded && (_total_len < 0 || *_in_pos < _end_pos)) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~Fin_M_qosNegotiated_present;
	else {
	    _out_data->bit_mask |= Fin_M_qosNegotiated_present;
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
	    _d_Fin_M_QoSInformation(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->qosNegotiated);
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
	_out_data->changeCondition = (enum Fin_M_ChangeCondition) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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

static void _d_Fin_M_RECORDSequenceNumber(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_RECORDSequenceNumber *_out_data)
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
	_element._id = (_Context_Id *)&_context_id[20];
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
	if (_data_length < 0) ++_indef_tags;
	_d_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->gsnAddress);
	while (_indef_tags > 0) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _indef_tags--;
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[19];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_d_Fin_M_RecordNumberList(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->recordNumberList);
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

static void _d_Fin_M_ManagementExtension(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_ManagementExtension *_out_data)
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
	_element._id = (_Context_Id *)&_context_id[23];
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
	    _out_data->bit_mask &= ~Fin_M_significance_present;
	    memcpy(&_out_data->significance, &_v0, sizeof(_v0));
	}
	else {
	    _out_data->bit_mask |= Fin_M_significance_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[22];
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
	_element._id = (_Context_Id *)&_context_id[21];
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

static void _d_Fin_M_Diagnostics(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_Diagnostics *_out_data)
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
		_element._id = (_Context_Id *)&_context_id[24];
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
		_element._id = (_Context_Id *)&_context_id[25];
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
		_element._id = (_Context_Id *)&_context_id[26];
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
		_element._id = (_Context_Id *)&_context_id[27];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 4;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_d_Fin_M_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.networkSpecificCause);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x4: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[28];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 5;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_d_Fin_M_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.manufacturerSpecificCause);
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

static void _d_Fin_M_ManagementExtensions(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_ManagementExtensions *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	struct Fin_M_ManagementExtensions_	*_temp;
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

	unsigned long count = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	_temp = *_out_data;
#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[29];
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
		_temp->next = (struct Fin_M_ManagementExtensions_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M_ManagementExtensions_)*1, _oss_c->_buffer_provided);
		_temp = _temp->next;
	    } else {
		_temp = (struct Fin_M_ManagementExtensions_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M_ManagementExtensions_)*1, _oss_c->_buffer_provided);
		*_out_data = _temp;
	    }
	    _temp->next = NULL;
#if OSSDEBUG > 1
	    _element._occurrence = count;
#endif

	    _oss_c->_tag_decoded = FALSE;
	    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	    _d_Fin_M_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value);
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

    }
}

static void _d_Fin_M_RecordNumberList(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, Fin_M_RecordNumberList *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	struct Fin_M_RecordNumberList_	*_temp;
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

	unsigned long count = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	_temp = *_out_data;
#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[30];
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
	    if (_data_tag != 0x2) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	    if (_temp) {
		_temp->next = (struct Fin_M_RecordNumberList_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M_RecordNumberList_)*1, _oss_c->_buffer_provided);
		_temp = _temp->next;
	    } else {
		_temp = (struct Fin_M_RecordNumberList_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M_RecordNumberList_)*1, _oss_c->_buffer_provided);
		*_out_data = _temp;
	    }
	    _temp->next = NULL;
#if OSSDEBUG > 1
	    _element._occurrence = count;
#endif

	    _oss_c->_tag_decoded = FALSE;
	    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	    _temp->value = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

    }
}

static int _dFin_M_BSRecord(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    Fin_M_BSRecord	*_out_data;
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
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[31];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    _out_data = (struct Fin_M_BSRecord *) _oss_dec_getmem(_g, sizeof(struct Fin_M_BSRecord)*1, _oss_c->_buffer_provided);
    _oss_c->_tag_decoded = FALSE;
    _data_length = *_buf_len;
    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
    {
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[121];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8000) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	{
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
			_element._id = (_Context_Id *)&_context_id[132];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.cgnID, 2, 15, _constructed);
			_present_flags |= 0x1;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0x1: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[131];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.bsXferFile, 2, 34, _constructed);
			_present_flags |= 0x2;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0x2: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[130];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.openTime, 2, 14, _constructed);
			_present_flags |= 0x4;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0x3: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[129];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.closeTime, 2, 14, _constructed);
			_present_flags |= 0x8;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0x4: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[128];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.lrsnStart, 2, 10, _constructed);
			_present_flags |= 0x10;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0x5: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[127];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.lrsnEnd, 2, 10, _constructed);
			_present_flags |= 0x20;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0x6: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[126];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.cdrType, 2, 3, _constructed);
			_present_flags |= 0x40;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0x7: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[125];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.totalCdr, 2, 10, _constructed);
			_present_flags |= 0x80;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0x8: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[124];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.missingCdr, 2, 10, _constructed);
			_present_flags |= 0x100;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0x9: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[123];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.duplicateCdr, 2, 10, _constructed);
			_present_flags |= 0x200;
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

		    }   break;

		    case 0xA: {
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[122];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			_oss_c->_tag_decoded = FALSE;
			_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
			_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->bsXferRecord.cgID, 2, 15, _constructed);
			_present_flags |= 0x400;
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

	    if ((_present_flags & 0x7FF) != 0x7FF) _oss_dec_error(_g, TRUE, _field_omit, 0L);
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[32];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	{
	    struct Fin_M__seqof6	*_temp;
	    long _total_len = _data_length;
	    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

	    unsigned long count = 0;

#if OSSDEBUG > 1
	    _Context_Element _element;

	    _element._occurrence = 0;
#endif

	    _temp = _out_data->listOfCDR;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[33];
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
		    _temp->next = (struct Fin_M__seqof6 *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof6)*1, _oss_c->_buffer_provided);
		    _temp = _temp->next;
		} else {
		    _temp = (struct Fin_M__seqof6 *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof6)*1, _oss_c->_buffer_provided);
		    _out_data->listOfCDR = _temp;
		}
		_temp->next = NULL;
#if OSSDEBUG > 1
		_element._occurrence = count;
#endif

		{
#if OSSDEBUG > 1
		    _Context_Element _element;

		    _element._occurrence = 0;
#endif

		    switch (_data_tag) {
		    default:
			if (_data_tag < 0x8000U) _oss_dec_error(_g, TRUE, _unknown_field, (long)_data_tag);
			switch (_data_tag & ~0x8000U) {
			case 0x0: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[34];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _temp->value.choice = 1;
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
					    _element._id = (_Context_Id *)&_context_id[64];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[63];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_networkInitiation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_networkInitiation_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.networkInitiation = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x2: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[62];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_anonymousAccessIndicator_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_anonymousAccessIndicator_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.anonymousAccessIndicator = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.servedIMSI, 2, 8, _constructed);
					    _present_flags |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x4: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[60];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_servedIMEI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.servedIMEI, 2, 8, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[58];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct Fin_M__seqof1_	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnPDPRecord.sgsnAddress;
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[59];
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
						    if (_temp1) {
							_temp1->next = (struct Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
							_temp->value.u.sgsnPDPRecord.sgsnAddress = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _d_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
						}
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }
					    _present_flags |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[57];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_msNetworkCapability_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.msNetworkCapability, 2, 1, _constructed);
					    _present_flags |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x7: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[56];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_routingArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.routingArea, 2, 1, _constructed);
					    _present_flags |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[55];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_locationAreaCode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_locationAreaCode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.locationAreaCode, 2, 2, _constructed);
					    _present_flags |= 0x100;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x9: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[54];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_cellIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_cellIdentity_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.cellIdentity, 2, 2, _constructed);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[53];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.chargingID = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x400;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[52];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnPDPRecord.ggsnAddressUsed);
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
					    _element._id = (_Context_Id *)&_context_id[51];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnPDPRecord.accessPointNameNI, 63, _constructed);
					    _present_flags |= 0x1000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xD: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[50];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.pdpType, 2, 2, _constructed);
					    _present_flags |= 0x2000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[49];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_Fin_M_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnPDPRecord.servedPDPAddress);
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
					    _element._id = (_Context_Id *)&_context_id[47];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct Fin_M__seqof2_	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnPDPRecord.listOfTrafficVolumes;
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[48];
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
						    if (_temp1) {
							_temp1->next = (struct Fin_M__seqof2_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof2_)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct Fin_M__seqof2_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof2_)*1, _oss_c->_buffer_provided);
							_temp->value.u.sgsnPDPRecord.listOfTrafficVolumes = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _oss_c->_tag_decoded = FALSE;
						    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						    _d_Fin_M_ChangeOfCharCondition(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
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
					    _element._id = (_Context_Id *)&_context_id[46];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.recordOpeningTime, 2, 9, _constructed);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[45];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x20000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x12: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[44];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_sgsnChange_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_sgsnChange_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.sgsnChange = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x40000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x13: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[43];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x80000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x15: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[41];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_recordSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_recordSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct Fin_M__seqof3	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnPDPRecord.recordSequenceNumber;
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[42];
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
						    if (_temp1) {
							_temp1->next = (struct Fin_M__seqof3 *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof3)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct Fin_M__seqof3 *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof3)*1, _oss_c->_buffer_provided);
							_temp->value.u.sgsnPDPRecord.recordSequenceNumber = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _oss_c->_tag_decoded = FALSE;
						    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						    _d_Fin_M_RECORDSequenceNumber(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
						}
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }
					    _present_flags |= 0x100000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x16: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[40];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnPDPRecord.nodeID, 20, _constructed);
					    _present_flags |= 0x200000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x17: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[39];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_Fin_M_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnPDPRecord.recordExtensions);
					    _present_flags |= 0x400000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x19: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[38];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_apnSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_apnSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.apnSelectionMode = (enum Fin_M_APNSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x800000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1A: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[37];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnPDPRecord.accessPointNameOI, 37, _constructed);
					    _present_flags |= 0x1000000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1D: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[36];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= Fin_M_SGSNPDPRecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.servedMSISDN, 2, 10, _constructed);
					    _present_flags |= 0x2000000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1E: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[35];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.consolidationResult = (enum Fin_M_ConsolidationResult) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x4000000;
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
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_networkInitiation_present;
				}
				if (!(_present_flags & 0x4)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_anonymousAccessIndicator_present;
				}
				if (!(_present_flags & 0x10)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_servedIMEI_present;
				}
				if (!(_present_flags & 0x40)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_msNetworkCapability_present;
				}
				if (!(_present_flags & 0x80)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_routingArea_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_locationAreaCode_present;
				}
				if (!(_present_flags & 0x200)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_cellIdentity_present;
				}
				if (!(_present_flags & 0x40000)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_sgsnChange_present;
				}
				if (!(_present_flags & 0x100000)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_recordSequenceNumber_present;
				}
				if (!(_present_flags & 0x200000)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_nodeID_present;
				}
				if (!(_present_flags & 0x400000)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_recordExtensions_present;
				}
				if (!(_present_flags & 0x800000)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_apnSelectionMode_present;
				}
				if (!(_present_flags & 0x2000000)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~Fin_M_SGSNPDPRecord_servedMSISDN_present;
				}
				if ((_present_flags & 0x50BFC29) != 0x50BFC29) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x1: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[65];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _temp->value.choice = 2;
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
					    _element._id = (_Context_Id *)&_context_id[64];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[76];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_networkInitiation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= Fin_M_GGSNPDPRecord_networkInitiation_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.networkInitiation = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x2;
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

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_anonymousAccessIndicator_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= Fin_M_GGSNPDPRecord_anonymousAccessIndicator_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.anonymousAccessIndicator = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.servedIMSI, 2, 8, _constructed);
					    _present_flags |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x4: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[74];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.ggsnAddress);
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
					    _element._id = (_Context_Id *)&_context_id[53];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.chargingID = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[58];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct Fin_M__seqof1_	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.ggsnPDPRecord.sgsnAddress;
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[59];
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
						    if (_temp1) {
							_temp1->next = (struct Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
							_temp->value.u.ggsnPDPRecord.sgsnAddress = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _d_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
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
					    _element._id = (_Context_Id *)&_context_id[51];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.ggsnPDPRecord.accessPointNameNI, 63, _constructed);
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

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.pdpType, 2, 2, _constructed);
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

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_Fin_M_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.servedPDPAddress);
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
					    _element._id = (_Context_Id *)&_context_id[72];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_remotePDPAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= Fin_M_remotePDPAddress_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct Fin_M__seqof4	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.ggsnPDPRecord.remotePDPAddress;
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[73];
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
						    if (_temp1) {
							_temp1->next = (struct Fin_M__seqof4 *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof4)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct Fin_M__seqof4 *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof4)*1, _oss_c->_buffer_provided);
							_temp->value.u.ggsnPDPRecord.remotePDPAddress = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _d_Fin_M_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
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
					    _element._id = (_Context_Id *)&_context_id[71];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_dynamicAddressFlag_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= Fin_M_dynamicAddressFlag_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.dynamicAddressFlag = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x800;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xC: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[47];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct Fin_M__seqof2_	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.ggsnPDPRecord.listOfTrafficVolumes;
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[48];
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
						    if (_temp1) {
							_temp1->next = (struct Fin_M__seqof2_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof2_)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct Fin_M__seqof2_ *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof2_)*1, _oss_c->_buffer_provided);
							_temp->value.u.ggsnPDPRecord.listOfTrafficVolumes = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _oss_c->_tag_decoded = FALSE;
						    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						    _d_Fin_M_ChangeOfCharCondition(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
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
					    _element._id = (_Context_Id *)&_context_id[46];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.recordOpeningTime, 2, 9, _constructed);
					    _present_flags |= 0x2000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[45];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x4000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xF: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[43];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x8000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[70];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_recordSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= Fin_M_GGSNPDPRecord_recordSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_Fin_M_RECORDSequenceNumber(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.recordSequenceNumber);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x12: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[69];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= Fin_M_GGSNPDPRecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.ggsnPDPRecord.nodeID, 20, _constructed);
					    _present_flags |= 0x20000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x13: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[68];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= Fin_M_GGSNPDPRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_Fin_M_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.recordExtensions);
					    _present_flags |= 0x40000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x15: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[67];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_apnSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= Fin_M_GGSNPDPRecord_apnSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.apnSelectionMode = (enum Fin_M_APNSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x80000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1D: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[66];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= Fin_M_GGSNPDPRecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.servedMSISDN, 2, 10, _constructed);
					    _present_flags |= 0x100000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1E: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[35];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.consolidationResult = (enum Fin_M_ConsolidationResult) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x200000;
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
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~Fin_M_GGSNPDPRecord_networkInitiation_present;
				}
				if (!(_present_flags & 0x4)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~Fin_M_GGSNPDPRecord_anonymousAccessIndicator_present;
				}
				if (!(_present_flags & 0x400)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~Fin_M_remotePDPAddress_present;
				}
				if (!(_present_flags & 0x800)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~Fin_M_dynamicAddressFlag_present;
				}
				if (!(_present_flags & 0x10000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~Fin_M_GGSNPDPRecord_recordSequenceNumber_present;
				}
				if (!(_present_flags & 0x20000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~Fin_M_GGSNPDPRecord_nodeID_present;
				}
				if (!(_present_flags & 0x40000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~Fin_M_GGSNPDPRecord_recordExtensions_present;
				}
				if (!(_present_flags & 0x80000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~Fin_M_GGSNPDPRecord_apnSelectionMode_present;
				}
				if (!(_present_flags & 0x100000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~Fin_M_GGSNPDPRecord_servedMSISDN_present;
				}
				if ((_present_flags & 0x20F3F9) != 0x20F3F9) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x2: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[77];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _temp->value.choice = 3;
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
					    _element._id = (_Context_Id *)&_context_id[64];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnMMRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.servedIMSI, 2, 8, _constructed);
					    _present_flags |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x2: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[95];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_servedIMEI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.servedIMEI, 2, 8, _constructed);
					    _present_flags |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[94];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnMMRecord.sgsnAddress);
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
					    _element._id = (_Context_Id *)&_context_id[93];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_msNetworkCapability_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.msNetworkCapability, 2, 1, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[92];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_routingArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.routingArea, 2, 1, _constructed);
					    _present_flags |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[91];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_locationAreaCode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_locationAreaCode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.locationAreaCode, 2, 2, _constructed);
					    _present_flags |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x7: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[90];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_cellIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_cellIdentity_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.cellIdentity, 2, 2, _constructed);
					    _present_flags |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[85];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_changeLocation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_changeLocation_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct Fin_M__seqof5	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnMMRecord.changeLocation;
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[86];
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
						    if (_temp1) {
							_temp1->next = (struct Fin_M__seqof5 *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof5)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct Fin_M__seqof5 *) _oss_dec_getmem(_g, sizeof(struct Fin_M__seqof5)*1, _oss_c->_buffer_provided);
							_temp->value.u.sgsnMMRecord.changeLocation = _temp1;
						    }
						    _temp1->next = NULL;
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
							_element._id = (_Context_Id *)&_context_id[89];
							_element._parent = _oss_c->_oss_context_anchor._last;
							_oss_c->_oss_context_anchor._last = &_element;
#endif

							_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
							if (_data_tag != 0x8000) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

							_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp1->value.locationAreaCode, 2, 2, _constructed);
#if OSSDEBUG > 1
							_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
							_element._id = (_Context_Id *)&_context_id[88];
							_element._parent = _oss_c->_oss_context_anchor._last;
							_oss_c->_oss_context_anchor._last = &_element;
#endif

							_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
							if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

							_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp1->value.routingAreaCode, 2, 1, _constructed);
#if OSSDEBUG > 1
							_oss_c->_oss_context_anchor._last = _element._parent;
#endif

							if (_total_len < 0 || *_in_pos < _end_pos) {
							    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
							    _oss_c->_tag_decoded = TRUE;
							}
							if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
							    _temp1->value.bit_mask &= ~Fin_M_cellId_present;
							else {
							    _temp1->value.bit_mask |= Fin_M_cellId_present;
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[87];
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
							    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp1->value.cellId, 2, 2, _constructed);
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
							_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp1->value.changeTime, 2, 9, _constructed);
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
					    _element._id = (_Context_Id *)&_context_id[46];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.recordOpeningTime, 2, 9, _constructed);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[84];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_duration_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_duration_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnMMRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x400;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[83];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_sgsnChange_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_sgsnChange_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnMMRecord.sgsnChange = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x800;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xC: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[43];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnMMRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xD: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[82];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_diagnostics_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnMMRecord.diagnostics);
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
					    _element._id = (_Context_Id *)&_context_id[81];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_recordSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_recordSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnMMRecord.recordSequenceNumber = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x4000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xF: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[80];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnMMRecord.nodeID, 20, _constructed);
					    _present_flags |= 0x8000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x10: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[79];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_Fin_M_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnMMRecord.recordExtensions);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[78];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= Fin_M_SGSNMMRecord_localSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnMMRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_servedIMEI_present;
				}
				if (!(_present_flags & 0x10)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_msNetworkCapability_present;
				}
				if (!(_present_flags & 0x20)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_routingArea_present;
				}
				if (!(_present_flags & 0x40)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_locationAreaCode_present;
				}
				if (!(_present_flags & 0x80)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_cellIdentity_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_changeLocation_present;
				}
				if (!(_present_flags & 0x400)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_duration_present;
				}
				if (!(_present_flags & 0x800)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_sgsnChange_present;
				}
				if (!(_present_flags & 0x2000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_diagnostics_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_recordSequenceNumber_present;
				}
				if (!(_present_flags & 0x8000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_nodeID_present;
				}
				if (!(_present_flags & 0x10000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_recordExtensions_present;
				}
				if (!(_present_flags & 0x20000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~Fin_M_SGSNMMRecord_localSequenceNumber_present;
				}
				if ((_present_flags & 0x120B) != 0x120B) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x3: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[96];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _temp->value.choice = 4;
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
					    _element._id = (_Context_Id *)&_context_id[64];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMORecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.servedIMSI, 2, 8, _constructed);
					    _present_flags |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x2: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[110];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= Fin_M_SGSNSMORecord_servedIMEI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.servedIMEI, 2, 8, _constructed);
					    _present_flags |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[109];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= Fin_M_SGSNSMORecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.servedMSISDN, 2, 10, _constructed);
					    _present_flags |= 0x8;
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

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.msNetworkCapability, 2, 1, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[107];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.serviceCentre, 2, 20, _constructed);
					    _present_flags |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[106];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.recordingEntity, 2, 20, _constructed);
					    _present_flags |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x7: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[105];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_locationArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= Fin_M_SGSNSMORecord_locationArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.locationArea, 2, 2, _constructed);
					    _present_flags |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[104];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= Fin_M_SGSNSMORecord_routingArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.routingArea, 2, 1, _constructed);
					    _present_flags |= 0x100;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x9: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[103];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_cellIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= Fin_M_SGSNSMORecord_cellIdentity_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.cellIdentity, 2, 2, _constructed);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[102];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.messageReference, 2, 9, _constructed);
					    _present_flags |= 0x400;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[101];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.originationTime, 2, 9, _constructed);
					    _present_flags |= 0x800;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xC: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[100];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_smsResult_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= Fin_M_SGSNSMORecord_smsResult_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMORecord.smsResult);
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
					    _element._id = (_Context_Id *)&_context_id[99];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= Fin_M_SGSNSMORecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_Fin_M_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMORecord.recordExtensions);
					    _present_flags |= 0x2000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[98];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= Fin_M_SGSNSMORecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnSMORecord.nodeID, 20, _constructed);
					    _present_flags |= 0x4000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xF: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[97];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= Fin_M_SGSNSMORecord_localSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMORecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~Fin_M_SGSNSMORecord_servedIMEI_present;
				}
				if (!(_present_flags & 0x8)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~Fin_M_SGSNSMORecord_servedMSISDN_present;
				}
				if (!(_present_flags & 0x80)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~Fin_M_SGSNSMORecord_locationArea_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~Fin_M_SGSNSMORecord_routingArea_present;
				}
				if (!(_present_flags & 0x200)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~Fin_M_SGSNSMORecord_cellIdentity_present;
				}
				if (!(_present_flags & 0x1000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~Fin_M_SGSNSMORecord_smsResult_present;
				}
				if (!(_present_flags & 0x2000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~Fin_M_SGSNSMORecord_recordExtensions_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~Fin_M_SGSNSMORecord_nodeID_present;
				}
				if (!(_present_flags & 0x8000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~Fin_M_SGSNSMORecord_localSequenceNumber_present;
				}
				if ((_present_flags & 0xC73) != 0xC73) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x4: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[111];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _temp->value.choice = 5;
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
					    _element._id = (_Context_Id *)&_context_id[64];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMTRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.servedIMSI, 2, 8, _constructed);
					    _present_flags |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x2: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[120];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= Fin_M_SGSNSMTRecord_servedIMEI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.servedIMEI, 2, 8, _constructed);
					    _present_flags |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[119];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= Fin_M_SGSNSMTRecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.servedMSISDN, 2, 10, _constructed);
					    _present_flags |= 0x8;
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

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.msNetworkCapability, 2, 1, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[107];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.serviceCentre, 2, 20, _constructed);
					    _present_flags |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[106];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.recordingEntity, 2, 20, _constructed);
					    _present_flags |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x7: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[118];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_locationArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= Fin_M_SGSNSMTRecord_locationArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.locationArea, 2, 2, _constructed);
					    _present_flags |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[117];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= Fin_M_SGSNSMTRecord_routingArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.routingArea, 2, 1, _constructed);
					    _present_flags |= 0x100;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x9: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[116];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_cellIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= Fin_M_SGSNSMTRecord_cellIdentity_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.cellIdentity, 2, 2, _constructed);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[101];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.originationTime, 2, 9, _constructed);
					    _present_flags |= 0x400;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[115];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_smsResult_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= Fin_M_SGSNSMTRecord_smsResult_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMTRecord.smsResult);
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
					    _element._id = (_Context_Id *)&_context_id[114];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= Fin_M_SGSNSMTRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_Fin_M_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMTRecord.recordExtensions);
					    _present_flags |= 0x1000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xD: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[113];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= Fin_M_SGSNSMTRecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnSMTRecord.nodeID, 20, _constructed);
					    _present_flags |= 0x2000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[112];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= Fin_M_SGSNSMTRecord_localSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMTRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~Fin_M_SGSNSMTRecord_servedIMEI_present;
				}
				if (!(_present_flags & 0x8)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~Fin_M_SGSNSMTRecord_servedMSISDN_present;
				}
				if (!(_present_flags & 0x80)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~Fin_M_SGSNSMTRecord_locationArea_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~Fin_M_SGSNSMTRecord_routingArea_present;
				}
				if (!(_present_flags & 0x200)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~Fin_M_SGSNSMTRecord_cellIdentity_present;
				}
				if (!(_present_flags & 0x800)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~Fin_M_SGSNSMTRecord_smsResult_present;
				}
				if (!(_present_flags & 0x1000)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~Fin_M_SGSNSMTRecord_recordExtensions_present;
				}
				if (!(_present_flags & 0x2000)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~Fin_M_SGSNSMTRecord_nodeID_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~Fin_M_SGSNSMTRecord_localSequenceNumber_present;
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

	    }
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

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
    _rc = _dFin_M_BSRecord(_g, _inbuf, _inlen, _outbuf, _outlen);
    _oss_c->_oss_err_msg = NULL;
    return _rc;
}

static int _f_Fin_M_IPBinaryAddress(struct ossGlobal *_g, Fin_M_IPBinaryAddress *_data_ptr)
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

static int _f_0Fin_M_IPTextRepresentedAddr(struct ossGlobal *_g, Fin_M_IPTextRepresentedAddress *_data_ptr)
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

static int _f_Fin_M_IPAddress(struct ossGlobal *_g, Fin_M_IPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	_f_Fin_M_IPBinaryAddress(_g, &_data_ptr->u.iPBinaryAddress);
	break;

    case 2:
	_f_0Fin_M_IPTextRepresentedAddr(_g, &_data_ptr->u.iPTextRepresentedAddress);
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_Fin_M_PDPAddress(struct ossGlobal *_g, Fin_M_PDPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	_f_Fin_M_IPAddress(_g, &_data_ptr->u.iPAddress);
	break;

    case 2:
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_Fin_M_QoSInformation(struct ossGlobal *_g, Fin_M_QoSInformation *_data_ptr)
{
    return(PDU_FREED);
}

static int _f_Fin_M_ChangeOfCharCondition(struct ossGlobal *_g, Fin_M_ChangeOfCharCondition *_data_ptr)
{
    if (_data_ptr->bit_mask & Fin_M_qosRequested_present) {
	_f_Fin_M_QoSInformation(_g, &_data_ptr->qosRequested);
    }

    if (_data_ptr->bit_mask & Fin_M_qosNegotiated_present) {
	_f_Fin_M_QoSInformation(_g, &_data_ptr->qosNegotiated);
    }

    return(PDU_FREED);
}

static int _f_Fin_M_RECORDSequenceNumber(struct ossGlobal *_g, Fin_M_RECORDSequenceNumber *_data_ptr)
{
    _f_Fin_M_IPAddress(_g, &_data_ptr->gsnAddress);
    _f_Fin_M_RecordNumberList(_g, &_data_ptr->recordNumberList);
    return(PDU_FREED);
}

static int _f_Fin_M_ManagementExtension(struct ossGlobal *_g, Fin_M_ManagementExtension *_data_ptr)
{
    if (_data_ptr->identifier.value)
	(*_g->freep)(_data_ptr->identifier.value);
    if (_data_ptr->bit_mask & Fin_M_significance_present) {
    }

    if (_data_ptr->information.value)
	(*_g->freep)(_data_ptr->information.value);
    return(PDU_FREED);
}

static int _f_Fin_M_Diagnostics(struct ossGlobal *_g, Fin_M_Diagnostics *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	break;

    case 2:
	break;

    case 3:
	break;

    case 4:
	_f_Fin_M_ManagementExtension(_g, &_data_ptr->u.networkSpecificCause);
	break;

    case 5:
	_f_Fin_M_ManagementExtension(_g, &_data_ptr->u.manufacturerSpecificCause);
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_Fin_M_ManagementExtensions(struct ossGlobal *_g, Fin_M_ManagementExtensions *_data_ptr)
{
    if (*_data_ptr) {
	struct Fin_M_ManagementExtensions_	*_tempa = *_data_ptr;
	struct Fin_M_ManagementExtensions_	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    _f_Fin_M_ManagementExtension(_g, &_tempa->value);
	    (*_g->freep)(_tempa);
	    _tempa = _tempb;
	}
    }

    return(PDU_FREED);
}

static int _f_Fin_M_RecordNumberList(struct ossGlobal *_g, Fin_M_RecordNumberList *_data_ptr)
{
    if (*_data_ptr) {
	struct Fin_M_RecordNumberList_	*_tempa = *_data_ptr;
	struct Fin_M_RecordNumberList_	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    (*_g->freep)(_tempa);
	    _tempa = _tempb;
	}
    }

    return(PDU_FREED);
}

static int _fFin_M_BSRecord(struct ossGlobal *_g, void *_data)
{
    Fin_M_BSRecord	*_data_ptr;
    _data_ptr = (Fin_M_BSRecord *) _data;
    if (_data_ptr->listOfCDR) {
	struct Fin_M__seqof6	*_tempa = _data_ptr->listOfCDR;
	struct Fin_M__seqof6	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    switch (_tempa->value.choice) {
	    case 1:
		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_networkInitiation_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_anonymousAccessIndicator_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_servedIMEI_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.sgsnAddress) {
		    struct Fin_M__seqof1_	*_temp1a = _tempa->value.u.sgsnPDPRecord.sgsnAddress;
		    struct Fin_M__seqof1_	*_temp1b;
		    while (_temp1a) {
			_temp1b = _temp1a->next;
			_f_Fin_M_IPAddress(_g, &_temp1a->value);
			(*_g->freep)(_temp1a);
			_temp1a = _temp1b;
		    }
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_msNetworkCapability_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_locationAreaCode_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_cellIdentity_present) {
		}

		_f_Fin_M_IPAddress(_g, &_tempa->value.u.sgsnPDPRecord.ggsnAddressUsed);
		_f_Fin_M_PDPAddress(_g, &_tempa->value.u.sgsnPDPRecord.servedPDPAddress);
		if (_tempa->value.u.sgsnPDPRecord.listOfTrafficVolumes) {
		    struct Fin_M__seqof2_	*_temp1a = _tempa->value.u.sgsnPDPRecord.listOfTrafficVolumes;
		    struct Fin_M__seqof2_	*_temp1b;
		    while (_temp1a) {
			_temp1b = _temp1a->next;
			_f_Fin_M_ChangeOfCharCondition(_g, &_temp1a->value);
			(*_g->freep)(_temp1a);
			_temp1a = _temp1b;
		    }
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_sgsnChange_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_recordSequenceNumber_present) {
		    if (_tempa->value.u.sgsnPDPRecord.recordSequenceNumber) {
			struct Fin_M__seqof3	*_temp1a = _tempa->value.u.sgsnPDPRecord.recordSequenceNumber;
			struct Fin_M__seqof3	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    _f_Fin_M_RECORDSequenceNumber(_g, &_temp1a->value);
			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_recordExtensions_present) {
		    _f_Fin_M_ManagementExtensions(_g, &_tempa->value.u.sgsnPDPRecord.recordExtensions);
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_apnSelectionMode_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_servedMSISDN_present) {
		}

		break;

	    case 2:
		if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_networkInitiation_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_anonymousAccessIndicator_present) {
		}

		_f_Fin_M_IPAddress(_g, &_tempa->value.u.ggsnPDPRecord.ggsnAddress);
		if (_tempa->value.u.ggsnPDPRecord.sgsnAddress) {
		    struct Fin_M__seqof1_	*_temp1a = _tempa->value.u.ggsnPDPRecord.sgsnAddress;
		    struct Fin_M__seqof1_	*_temp1b;
		    while (_temp1a) {
			_temp1b = _temp1a->next;
			_f_Fin_M_IPAddress(_g, &_temp1a->value);
			(*_g->freep)(_temp1a);
			_temp1a = _temp1b;
		    }
		}

		_f_Fin_M_PDPAddress(_g, &_tempa->value.u.ggsnPDPRecord.servedPDPAddress);
		if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_remotePDPAddress_present) {
		    if (_tempa->value.u.ggsnPDPRecord.remotePDPAddress) {
			struct Fin_M__seqof4	*_temp1a = _tempa->value.u.ggsnPDPRecord.remotePDPAddress;
			struct Fin_M__seqof4	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    _f_Fin_M_PDPAddress(_g, &_temp1a->value);
			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_dynamicAddressFlag_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.listOfTrafficVolumes) {
		    struct Fin_M__seqof2_	*_temp1a = _tempa->value.u.ggsnPDPRecord.listOfTrafficVolumes;
		    struct Fin_M__seqof2_	*_temp1b;
		    while (_temp1a) {
			_temp1b = _temp1a->next;
			_f_Fin_M_ChangeOfCharCondition(_g, &_temp1a->value);
			(*_g->freep)(_temp1a);
			_temp1a = _temp1b;
		    }
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_recordSequenceNumber_present) {
		    _f_Fin_M_RECORDSequenceNumber(_g, &_tempa->value.u.ggsnPDPRecord.recordSequenceNumber);
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_nodeID_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_recordExtensions_present) {
		    _f_Fin_M_ManagementExtensions(_g, &_tempa->value.u.ggsnPDPRecord.recordExtensions);
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_apnSelectionMode_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_servedMSISDN_present) {
		}

		break;

	    case 3:
		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_servedIMEI_present) {
		}

		_f_Fin_M_IPAddress(_g, &_tempa->value.u.sgsnMMRecord.sgsnAddress);
		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_msNetworkCapability_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_locationAreaCode_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_cellIdentity_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_changeLocation_present) {
		    if (_tempa->value.u.sgsnMMRecord.changeLocation) {
			struct Fin_M__seqof5	*_temp1a = _tempa->value.u.sgsnMMRecord.changeLocation;
			struct Fin_M__seqof5	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    if (_temp1a->value.bit_mask & Fin_M_cellId_present) {
			    }

			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_duration_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_sgsnChange_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_diagnostics_present) {
		    _f_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnMMRecord.diagnostics);
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_recordSequenceNumber_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_recordExtensions_present) {
		    _f_Fin_M_ManagementExtensions(_g, &_tempa->value.u.sgsnMMRecord.recordExtensions);
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_localSequenceNumber_present) {
		}

		break;

	    case 4:
		if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_servedIMEI_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_locationArea_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_cellIdentity_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_smsResult_present) {
		    _f_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnSMORecord.smsResult);
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_recordExtensions_present) {
		    _f_Fin_M_ManagementExtensions(_g, &_tempa->value.u.sgsnSMORecord.recordExtensions);
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_localSequenceNumber_present) {
		}

		break;

	    case 5:
		if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_servedIMEI_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_locationArea_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_cellIdentity_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_smsResult_present) {
		    _f_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnSMTRecord.smsResult);
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_recordExtensions_present) {
		    _f_Fin_M_ManagementExtensions(_g, &_tempa->value.u.sgsnSMTRecord.recordExtensions);
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_localSequenceNumber_present) {
		}

		break;

	    default:
		/* Ignore for now */	;
	    }

	    (*_g->freep)(_tempa);
	    _tempa = _tempb;
	}
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
	    _rc = _fFin_M_BSRecord(_g, _data);
	    break;

	default:  return(PDU_RANGE);
    }
    return _rc;
}

#ifdef OSSPRINT
static int _p_Fin_M_IPBinaryAddress(struct ossGlobal *_g, Fin_M_IPBinaryAddress *_data_ptr)
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

static int _p_0Fin_M_IPTextRepresentedAddr(struct ossGlobal *_g, Fin_M_IPTextRepresentedAddress *_data_ptr)
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

static int _p_Fin_M_IPAddress(struct ossGlobal *_g, Fin_M_IPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	ossPrint(_g, "iPBinaryAddress : ");
	_p_Fin_M_IPBinaryAddress(_g, &_data_ptr->u.iPBinaryAddress);
	break;

    case 2:
	ossPrint(_g, "iPTextRepresentedAddress : ");
	_p_0Fin_M_IPTextRepresentedAddr(_g, &_data_ptr->u.iPTextRepresentedAddress);
	break;

    default:
	ossPrint(_g, "<unknown choice>");
    }

    return(PDU_PRINTED);
}

static int _p_Fin_M_PDPAddress(struct ossGlobal *_g, Fin_M_PDPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	ossPrint(_g, "iPAddress : ");
	_p_Fin_M_IPAddress(_g, &_data_ptr->u.iPAddress);
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

static int _p_Fin_M_QoSInformation(struct ossGlobal *_g, Fin_M_QoSInformation *_data_ptr)
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

static int _p_Fin_M_ChangeOfCharCondition(struct ossGlobal *_g, Fin_M_ChangeOfCharCondition *_data_ptr)
{
    {
	short _comma = 0;

	_oss_indent(_g, 1);
	ossPrint(_g, "{");
	if (_data_ptr->bit_mask & Fin_M_qosRequested_present) {
	    _oss_indent(_g, 0);
	    ossPrint(_g, "qosRequested ");
	    _p_Fin_M_QoSInformation(_g, &_data_ptr->qosRequested);
	    _comma = 1;
	}

	if (_data_ptr->bit_mask & Fin_M_qosNegotiated_present) {
	    if (_comma) ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "qosNegotiated ");
	    _p_Fin_M_QoSInformation(_g, &_data_ptr->qosNegotiated);
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

static int _p_Fin_M_RECORDSequenceNumber(struct ossGlobal *_g, Fin_M_RECORDSequenceNumber *_data_ptr)
{
    {
	short _comma = 0;

	_oss_indent(_g, 1);
	ossPrint(_g, "{");
	_oss_indent(_g, 0);
	ossPrint(_g, "gsnAddress ");
	_p_Fin_M_IPAddress(_g, &_data_ptr->gsnAddress);
	_comma = 1;
	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "recordNumberList ");
	_p_Fin_M_RecordNumberList(_g, &_data_ptr->recordNumberList);
	_oss_indent(_g, -1);
	ossPrint(_g, "}");
    }

    return(PDU_PRINTED);
}

static int _p_Fin_M_ManagementExtension(struct ossGlobal *_g, Fin_M_ManagementExtension *_data_ptr)
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
	if (_data_ptr->bit_mask & Fin_M_significance_present) {
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

static int _p_Fin_M_Diagnostics(struct ossGlobal *_g, Fin_M_Diagnostics *_data_ptr)
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
	_p_Fin_M_ManagementExtension(_g, &_data_ptr->u.networkSpecificCause);
	break;

    case 5:
	ossPrint(_g, "manufacturerSpecificCause : ");
	_p_Fin_M_ManagementExtension(_g, &_data_ptr->u.manufacturerSpecificCause);
	break;

    default:
	ossPrint(_g, "<unknown choice>");
    }

    return(PDU_PRINTED);
}

static int _p_Fin_M_ManagementExtensions(struct ossGlobal *_g, Fin_M_ManagementExtensions *_data_ptr)
{
    _oss_indent(_g, 1);
    ossPrint(_g, "{");
    if (*_data_ptr) {
	struct Fin_M_ManagementExtensions_	*_tempa = *_data_ptr;
	struct Fin_M_ManagementExtensions_	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    _oss_indent(_g, 0);
	    _p_Fin_M_ManagementExtension(_g, &_tempa->value);
	    _tempa = _tempb;
	    if (_tempa)
		ossPrint(_g, ",");
	}
    }

    _oss_indent(_g, -1);
    ossPrint(_g, "}");
    return(PDU_PRINTED);
}

static int _p_Fin_M_RecordNumberList(struct ossGlobal *_g, Fin_M_RecordNumberList *_data_ptr)
{
    _oss_indent(_g, 1);
    ossPrint(_g, "{");
    if (*_data_ptr) {
	struct Fin_M_RecordNumberList_	*_tempa = *_data_ptr;
	struct Fin_M_RecordNumberList_	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    _oss_indent(_g, 0);
	    {
		LONG_LONG value = _tempa->value;

		ossPrint(_g, LLONG_FMT, value);
	    }
	    _tempa = _tempb;
	    if (_tempa)
		ossPrint(_g, ",");
	}
    }

    _oss_indent(_g, -1);
    ossPrint(_g, "}");
    return(PDU_PRINTED);
}

static int _pFin_M_BSRecord(struct ossGlobal *_g, void *_data)
{
    Fin_M_BSRecord	*_data_ptr;
    _data_ptr = (Fin_M_BSRecord *) _data;
    {
	short _comma = 0;

	_oss_indent(_g, 1);
	ossPrint(_g, "{");
	_oss_indent(_g, 0);
	ossPrint(_g, "bsXferRecord ");
	{
	    short _comma = 0;

	    _oss_indent(_g, 1);
	    ossPrint(_g, "{");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "cgnID ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.cgnID.value, _data_ptr->bsXferRecord.cgnID.length);
	    _comma = 1;
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "bsXferFile ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.bsXferFile.value, _data_ptr->bsXferRecord.bsXferFile.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "openTime ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.openTime.value, _data_ptr->bsXferRecord.openTime.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "closeTime ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.closeTime.value, _data_ptr->bsXferRecord.closeTime.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "lrsnStart ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.lrsnStart.value, _data_ptr->bsXferRecord.lrsnStart.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "lrsnEnd ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.lrsnEnd.value, _data_ptr->bsXferRecord.lrsnEnd.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "cdrType ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.cdrType.value, _data_ptr->bsXferRecord.cdrType.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "totalCdr ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.totalCdr.value, _data_ptr->bsXferRecord.totalCdr.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "missingCdr ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.missingCdr.value, _data_ptr->bsXferRecord.missingCdr.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "duplicateCdr ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.duplicateCdr.value, _data_ptr->bsXferRecord.duplicateCdr.length);
	    ossPrint(_g, ",");
	    _oss_indent(_g, 0);
	    ossPrint(_g, "cgID ");
	    _oss_prt_oct(_g, _data_ptr->bsXferRecord.cgID.value, _data_ptr->bsXferRecord.cgID.length);
	    _oss_indent(_g, -1);
	    ossPrint(_g, "}");
	}

	_comma = 1;
	ossPrint(_g, ",");
	_oss_indent(_g, 0);
	ossPrint(_g, "listOfCDR ");
	_oss_indent(_g, 1);
	ossPrint(_g, "{");
	if (_data_ptr->listOfCDR) {
	    struct Fin_M__seqof6	*_tempa = _data_ptr->listOfCDR;
	    struct Fin_M__seqof6	*_tempb;
	    while (_tempa) {
		_tempb = _tempa->next;
		_oss_indent(_g, 0);
		switch (_tempa->value.choice) {
		case 1:
		    ossPrint(_g, "sgsnPDPRecord : ");
		    {
			short _comma = 0;

			_oss_indent(_g, 1);
			ossPrint(_g, "{");
			_oss_indent(_g, 0);
			ossPrint(_g, "recordType ");
			{
			    LONG_LONG value = _tempa->value.u.sgsnPDPRecord.recordType;

			    ossPrint(_g, LLONG_FMT, value);
			}
			_comma = 1;
			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_networkInitiation_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "networkInitiation ");
			    ossPrint(_g, _tempa->value.u.sgsnPDPRecord.networkInitiation ? "TRUE" : "FALSE");
			}

			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_anonymousAccessIndicator_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "anonymousAccessIndicator ");
			    ossPrint(_g, _tempa->value.u.sgsnPDPRecord.anonymousAccessIndicator ? "TRUE" : "FALSE");
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "servedIMSI ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnPDPRecord.servedIMSI.value, _tempa->value.u.sgsnPDPRecord.servedIMSI.length);
			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_servedIMEI_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "servedIMEI ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnPDPRecord.servedIMEI.value, _tempa->value.u.sgsnPDPRecord.servedIMEI.length);
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "sgsnAddress ");
			_oss_indent(_g, 1);
			ossPrint(_g, "{");
			if (_tempa->value.u.sgsnPDPRecord.sgsnAddress) {
			    struct Fin_M__seqof1_	*_temp1a = _tempa->value.u.sgsnPDPRecord.sgsnAddress;
			    struct Fin_M__seqof1_	*_temp1b;
			    while (_temp1a) {
				_temp1b = _temp1a->next;
				_oss_indent(_g, 0);
				_p_Fin_M_IPAddress(_g, &_temp1a->value);
				_temp1a = _temp1b;
				if (_temp1a)
				    ossPrint(_g, ",");
			    }
			}

			_oss_indent(_g, -1);
			ossPrint(_g, "}");
			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_msNetworkCapability_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "msNetworkCapability ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnPDPRecord.msNetworkCapability.value, _tempa->value.u.sgsnPDPRecord.msNetworkCapability.length);
			}

			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_routingArea_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "routingArea ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnPDPRecord.routingArea.value, _tempa->value.u.sgsnPDPRecord.routingArea.length);
			}

			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_locationAreaCode_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "locationAreaCode ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnPDPRecord.locationAreaCode.value, _tempa->value.u.sgsnPDPRecord.locationAreaCode.length);
			}

			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_cellIdentity_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "cellIdentity ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnPDPRecord.cellIdentity.value, _tempa->value.u.sgsnPDPRecord.cellIdentity.length);
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "chargingID ");
			{
			    ULONG_LONG value = _tempa->value.u.sgsnPDPRecord.chargingID;
			    ossPrint(_g, ULLONG_FMT, value);
			}
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "ggsnAddressUsed ");
			_p_Fin_M_IPAddress(_g, &_tempa->value.u.sgsnPDPRecord.ggsnAddressUsed);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "accessPointNameNI ");
			_oss_prt_nchar(_g, (char *)_tempa->value.u.sgsnPDPRecord.accessPointNameNI);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "pdpType ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnPDPRecord.pdpType.value, _tempa->value.u.sgsnPDPRecord.pdpType.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "servedPDPAddress ");
			_p_Fin_M_PDPAddress(_g, &_tempa->value.u.sgsnPDPRecord.servedPDPAddress);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "listOfTrafficVolumes ");
			_oss_indent(_g, 1);
			ossPrint(_g, "{");
			if (_tempa->value.u.sgsnPDPRecord.listOfTrafficVolumes) {
			    struct Fin_M__seqof2_	*_temp1a = _tempa->value.u.sgsnPDPRecord.listOfTrafficVolumes;
			    struct Fin_M__seqof2_	*_temp1b;
			    while (_temp1a) {
				_temp1b = _temp1a->next;
				_oss_indent(_g, 0);
				_p_Fin_M_ChangeOfCharCondition(_g, &_temp1a->value);
				_temp1a = _temp1b;
				if (_temp1a)
				    ossPrint(_g, ",");
			    }
			}

			_oss_indent(_g, -1);
			ossPrint(_g, "}");
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "recordOpeningTime ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnPDPRecord.recordOpeningTime.value, _tempa->value.u.sgsnPDPRecord.recordOpeningTime.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "duration ");
			{
			    LONG_LONG value = _tempa->value.u.sgsnPDPRecord.duration;

			    ossPrint(_g, LLONG_FMT, value);
			}
			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_sgsnChange_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "sgsnChange ");
			    ossPrint(_g, _tempa->value.u.sgsnPDPRecord.sgsnChange ? "TRUE" : "FALSE");
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "causeForRecClosing ");
			{
			    LONG_LONG value = _tempa->value.u.sgsnPDPRecord.causeForRecClosing;

			    ossPrint(_g, LLONG_FMT, value);
			}
			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_recordSequenceNumber_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "recordSequenceNumber ");
			    _oss_indent(_g, 1);
			    ossPrint(_g, "{");
			    if (_tempa->value.u.sgsnPDPRecord.recordSequenceNumber) {
				struct Fin_M__seqof3	*_temp1a = _tempa->value.u.sgsnPDPRecord.recordSequenceNumber;
				struct Fin_M__seqof3	*_temp1b;
				while (_temp1a) {
				    _temp1b = _temp1a->next;
				    _oss_indent(_g, 0);
				    _p_Fin_M_RECORDSequenceNumber(_g, &_temp1a->value);
				    _temp1a = _temp1b;
				    if (_temp1a)
					ossPrint(_g, ",");
				}
			    }

			    _oss_indent(_g, -1);
			    ossPrint(_g, "}");
			}

			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_nodeID_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "nodeID ");
			    _oss_prt_nchar(_g, (char *)_tempa->value.u.sgsnPDPRecord.nodeID);
			}

			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_recordExtensions_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "recordExtensions ");
			    _p_Fin_M_ManagementExtensions(_g, &_tempa->value.u.sgsnPDPRecord.recordExtensions);
			}

			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_apnSelectionMode_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "apnSelectionMode ");
			    {
				ULONG_LONG value = _tempa->value.u.sgsnPDPRecord.apnSelectionMode;
				ossPrint(_g, ULLONG_FMT, value);
			    }
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "accessPointNameOI ");
			_oss_prt_nchar(_g, (char *)_tempa->value.u.sgsnPDPRecord.accessPointNameOI);
			if (_tempa->value.u.sgsnPDPRecord.bit_mask & Fin_M_SGSNPDPRecord_servedMSISDN_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "servedMSISDN ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnPDPRecord.servedMSISDN.value, _tempa->value.u.sgsnPDPRecord.servedMSISDN.length);
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "consolidationResult ");
			{
			    ULONG_LONG value = _tempa->value.u.sgsnPDPRecord.consolidationResult;
			    ossPrint(_g, ULLONG_FMT, value);
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
			    LONG_LONG value = _tempa->value.u.ggsnPDPRecord.recordType;

			    ossPrint(_g, LLONG_FMT, value);
			}
			_comma = 1;
			if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_networkInitiation_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "networkInitiation ");
			    ossPrint(_g, _tempa->value.u.ggsnPDPRecord.networkInitiation ? "TRUE" : "FALSE");
			}

			if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_anonymousAccessIndicator_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "anonymousAccessIndicator ");
			    ossPrint(_g, _tempa->value.u.ggsnPDPRecord.anonymousAccessIndicator ? "TRUE" : "FALSE");
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "servedIMSI ");
			_oss_prt_oct(_g, _tempa->value.u.ggsnPDPRecord.servedIMSI.value, _tempa->value.u.ggsnPDPRecord.servedIMSI.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "ggsnAddress ");
			_p_Fin_M_IPAddress(_g, &_tempa->value.u.ggsnPDPRecord.ggsnAddress);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "chargingID ");
			{
			    ULONG_LONG value = _tempa->value.u.ggsnPDPRecord.chargingID;
			    ossPrint(_g, ULLONG_FMT, value);
			}
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "sgsnAddress ");
			_oss_indent(_g, 1);
			ossPrint(_g, "{");
			if (_tempa->value.u.ggsnPDPRecord.sgsnAddress) {
			    struct Fin_M__seqof1_	*_temp1a = _tempa->value.u.ggsnPDPRecord.sgsnAddress;
			    struct Fin_M__seqof1_	*_temp1b;
			    while (_temp1a) {
				_temp1b = _temp1a->next;
				_oss_indent(_g, 0);
				_p_Fin_M_IPAddress(_g, &_temp1a->value);
				_temp1a = _temp1b;
				if (_temp1a)
				    ossPrint(_g, ",");
			    }
			}

			_oss_indent(_g, -1);
			ossPrint(_g, "}");
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "accessPointNameNI ");
			_oss_prt_nchar(_g, (char *)_tempa->value.u.ggsnPDPRecord.accessPointNameNI);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "pdpType ");
			_oss_prt_oct(_g, _tempa->value.u.ggsnPDPRecord.pdpType.value, _tempa->value.u.ggsnPDPRecord.pdpType.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "servedPDPAddress ");
			_p_Fin_M_PDPAddress(_g, &_tempa->value.u.ggsnPDPRecord.servedPDPAddress);
			if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_remotePDPAddress_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "remotePDPAddress ");
			    _oss_indent(_g, 1);
			    ossPrint(_g, "{");
			    if (_tempa->value.u.ggsnPDPRecord.remotePDPAddress) {
				struct Fin_M__seqof4	*_temp1a = _tempa->value.u.ggsnPDPRecord.remotePDPAddress;
				struct Fin_M__seqof4	*_temp1b;
				while (_temp1a) {
				    _temp1b = _temp1a->next;
				    _oss_indent(_g, 0);
				    _p_Fin_M_PDPAddress(_g, &_temp1a->value);
				    _temp1a = _temp1b;
				    if (_temp1a)
					ossPrint(_g, ",");
				}
			    }

			    _oss_indent(_g, -1);
			    ossPrint(_g, "}");
			}

			if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_dynamicAddressFlag_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "dynamicAddressFlag ");
			    ossPrint(_g, _tempa->value.u.ggsnPDPRecord.dynamicAddressFlag ? "TRUE" : "FALSE");
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "listOfTrafficVolumes ");
			_oss_indent(_g, 1);
			ossPrint(_g, "{");
			if (_tempa->value.u.ggsnPDPRecord.listOfTrafficVolumes) {
			    struct Fin_M__seqof2_	*_temp1a = _tempa->value.u.ggsnPDPRecord.listOfTrafficVolumes;
			    struct Fin_M__seqof2_	*_temp1b;
			    while (_temp1a) {
				_temp1b = _temp1a->next;
				_oss_indent(_g, 0);
				_p_Fin_M_ChangeOfCharCondition(_g, &_temp1a->value);
				_temp1a = _temp1b;
				if (_temp1a)
				    ossPrint(_g, ",");
			    }
			}

			_oss_indent(_g, -1);
			ossPrint(_g, "}");
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "recordOpeningTime ");
			_oss_prt_oct(_g, _tempa->value.u.ggsnPDPRecord.recordOpeningTime.value, _tempa->value.u.ggsnPDPRecord.recordOpeningTime.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "duration ");
			{
			    LONG_LONG value = _tempa->value.u.ggsnPDPRecord.duration;

			    ossPrint(_g, LLONG_FMT, value);
			}
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "causeForRecClosing ");
			{
			    LONG_LONG value = _tempa->value.u.ggsnPDPRecord.causeForRecClosing;

			    ossPrint(_g, LLONG_FMT, value);
			}
			if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_recordSequenceNumber_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "recordSequenceNumber ");
			    _p_Fin_M_RECORDSequenceNumber(_g, &_tempa->value.u.ggsnPDPRecord.recordSequenceNumber);
			}

			if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_nodeID_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "nodeID ");
			    _oss_prt_nchar(_g, (char *)_tempa->value.u.ggsnPDPRecord.nodeID);
			}

			if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_recordExtensions_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "recordExtensions ");
			    _p_Fin_M_ManagementExtensions(_g, &_tempa->value.u.ggsnPDPRecord.recordExtensions);
			}

			if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_apnSelectionMode_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "apnSelectionMode ");
			    {
				ULONG_LONG value = _tempa->value.u.ggsnPDPRecord.apnSelectionMode;
				ossPrint(_g, ULLONG_FMT, value);
			    }
			}

			if (_tempa->value.u.ggsnPDPRecord.bit_mask & Fin_M_GGSNPDPRecord_servedMSISDN_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "servedMSISDN ");
			    _oss_prt_oct(_g, _tempa->value.u.ggsnPDPRecord.servedMSISDN.value, _tempa->value.u.ggsnPDPRecord.servedMSISDN.length);
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "consolidationResult ");
			{
			    ULONG_LONG value = _tempa->value.u.ggsnPDPRecord.consolidationResult;
			    ossPrint(_g, ULLONG_FMT, value);
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
			    LONG_LONG value = _tempa->value.u.sgsnMMRecord.recordType;

			    ossPrint(_g, LLONG_FMT, value);
			}
			_comma = 1;
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "servedIMSI ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnMMRecord.servedIMSI.value, _tempa->value.u.sgsnMMRecord.servedIMSI.length);
			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_servedIMEI_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "servedIMEI ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnMMRecord.servedIMEI.value, _tempa->value.u.sgsnMMRecord.servedIMEI.length);
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "sgsnAddress ");
			_p_Fin_M_IPAddress(_g, &_tempa->value.u.sgsnMMRecord.sgsnAddress);
			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_msNetworkCapability_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "msNetworkCapability ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnMMRecord.msNetworkCapability.value, _tempa->value.u.sgsnMMRecord.msNetworkCapability.length);
			}

			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_routingArea_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "routingArea ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnMMRecord.routingArea.value, _tempa->value.u.sgsnMMRecord.routingArea.length);
			}

			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_locationAreaCode_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "locationAreaCode ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnMMRecord.locationAreaCode.value, _tempa->value.u.sgsnMMRecord.locationAreaCode.length);
			}

			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_cellIdentity_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "cellIdentity ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnMMRecord.cellIdentity.value, _tempa->value.u.sgsnMMRecord.cellIdentity.length);
			}

			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_changeLocation_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "changeLocation ");
			    _oss_indent(_g, 1);
			    ossPrint(_g, "{");
			    if (_tempa->value.u.sgsnMMRecord.changeLocation) {
				struct Fin_M__seqof5	*_temp1a = _tempa->value.u.sgsnMMRecord.changeLocation;
				struct Fin_M__seqof5	*_temp1b;
				while (_temp1a) {
				    _temp1b = _temp1a->next;
				    _oss_indent(_g, 0);
				    {
					short _comma = 0;

					_oss_indent(_g, 1);
					ossPrint(_g, "{");
					_oss_indent(_g, 0);
					ossPrint(_g, "locationAreaCode ");
					_oss_prt_oct(_g, _temp1a->value.locationAreaCode.value, _temp1a->value.locationAreaCode.length);
					_comma = 1;
					ossPrint(_g, ",");
					_oss_indent(_g, 0);
					ossPrint(_g, "routingAreaCode ");
					_oss_prt_oct(_g, _temp1a->value.routingAreaCode.value, _temp1a->value.routingAreaCode.length);
					if (_temp1a->value.bit_mask & Fin_M_cellId_present) {
					    ossPrint(_g, ",");
					    _oss_indent(_g, 0);
					    ossPrint(_g, "cellId ");
					    _oss_prt_oct(_g, _temp1a->value.cellId.value, _temp1a->value.cellId.length);
					}

					ossPrint(_g, ",");
					_oss_indent(_g, 0);
					ossPrint(_g, "changeTime ");
					_oss_prt_oct(_g, _temp1a->value.changeTime.value, _temp1a->value.changeTime.length);
					_oss_indent(_g, -1);
					ossPrint(_g, "}");
				    }

				    _temp1a = _temp1b;
				    if (_temp1a)
					ossPrint(_g, ",");
				}
			    }

			    _oss_indent(_g, -1);
			    ossPrint(_g, "}");
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "recordOpeningTime ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnMMRecord.recordOpeningTime.value, _tempa->value.u.sgsnMMRecord.recordOpeningTime.length);
			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_duration_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "duration ");
			    {
				LONG_LONG value = _tempa->value.u.sgsnMMRecord.duration;

				ossPrint(_g, LLONG_FMT, value);
			    }
			}

			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_sgsnChange_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "sgsnChange ");
			    ossPrint(_g, _tempa->value.u.sgsnMMRecord.sgsnChange ? "TRUE" : "FALSE");
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "causeForRecClosing ");
			{
			    LONG_LONG value = _tempa->value.u.sgsnMMRecord.causeForRecClosing;

			    ossPrint(_g, LLONG_FMT, value);
			}
			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_diagnostics_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "diagnostics ");
			    _p_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnMMRecord.diagnostics);
			}

			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_recordSequenceNumber_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "recordSequenceNumber ");
			    {
				LONG_LONG value = _tempa->value.u.sgsnMMRecord.recordSequenceNumber;

				ossPrint(_g, LLONG_FMT, value);
			    }
			}

			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_nodeID_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "nodeID ");
			    _oss_prt_nchar(_g, (char *)_tempa->value.u.sgsnMMRecord.nodeID);
			}

			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_recordExtensions_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "recordExtensions ");
			    _p_Fin_M_ManagementExtensions(_g, &_tempa->value.u.sgsnMMRecord.recordExtensions);
			}

			if (_tempa->value.u.sgsnMMRecord.bit_mask & Fin_M_SGSNMMRecord_localSequenceNumber_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "localSequenceNumber ");
			    {
				ULONG_LONG value = _tempa->value.u.sgsnMMRecord.localSequenceNumber;
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
			    LONG_LONG value = _tempa->value.u.sgsnSMORecord.recordType;

			    ossPrint(_g, LLONG_FMT, value);
			}
			_comma = 1;
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "servedIMSI ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.servedIMSI.value, _tempa->value.u.sgsnSMORecord.servedIMSI.length);
			if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_servedIMEI_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "servedIMEI ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.servedIMEI.value, _tempa->value.u.sgsnSMORecord.servedIMEI.length);
			}

			if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_servedMSISDN_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "servedMSISDN ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.servedMSISDN.value, _tempa->value.u.sgsnSMORecord.servedMSISDN.length);
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "msNetworkCapability ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.msNetworkCapability.value, _tempa->value.u.sgsnSMORecord.msNetworkCapability.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "serviceCentre ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.serviceCentre.value, _tempa->value.u.sgsnSMORecord.serviceCentre.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "recordingEntity ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.recordingEntity.value, _tempa->value.u.sgsnSMORecord.recordingEntity.length);
			if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_locationArea_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "locationArea ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.locationArea.value, _tempa->value.u.sgsnSMORecord.locationArea.length);
			}

			if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_routingArea_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "routingArea ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.routingArea.value, _tempa->value.u.sgsnSMORecord.routingArea.length);
			}

			if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_cellIdentity_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "cellIdentity ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.cellIdentity.value, _tempa->value.u.sgsnSMORecord.cellIdentity.length);
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "messageReference ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.messageReference.value, _tempa->value.u.sgsnSMORecord.messageReference.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "originationTime ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMORecord.originationTime.value, _tempa->value.u.sgsnSMORecord.originationTime.length);
			if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_smsResult_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "smsResult ");
			    _p_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnSMORecord.smsResult);
			}

			if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_recordExtensions_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "recordExtensions ");
			    _p_Fin_M_ManagementExtensions(_g, &_tempa->value.u.sgsnSMORecord.recordExtensions);
			}

			if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_nodeID_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "nodeID ");
			    _oss_prt_nchar(_g, (char *)_tempa->value.u.sgsnSMORecord.nodeID);
			}

			if (_tempa->value.u.sgsnSMORecord.bit_mask & Fin_M_SGSNSMORecord_localSequenceNumber_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "localSequenceNumber ");
			    {
				ULONG_LONG value = _tempa->value.u.sgsnSMORecord.localSequenceNumber;
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
			    LONG_LONG value = _tempa->value.u.sgsnSMTRecord.recordType;

			    ossPrint(_g, LLONG_FMT, value);
			}
			_comma = 1;
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "servedIMSI ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.servedIMSI.value, _tempa->value.u.sgsnSMTRecord.servedIMSI.length);
			if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_servedIMEI_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "servedIMEI ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.servedIMEI.value, _tempa->value.u.sgsnSMTRecord.servedIMEI.length);
			}

			if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_servedMSISDN_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "servedMSISDN ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.servedMSISDN.value, _tempa->value.u.sgsnSMTRecord.servedMSISDN.length);
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "msNetworkCapability ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.msNetworkCapability.value, _tempa->value.u.sgsnSMTRecord.msNetworkCapability.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "serviceCentre ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.serviceCentre.value, _tempa->value.u.sgsnSMTRecord.serviceCentre.length);
			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "recordingEntity ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.recordingEntity.value, _tempa->value.u.sgsnSMTRecord.recordingEntity.length);
			if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_locationArea_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "locationArea ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.locationArea.value, _tempa->value.u.sgsnSMTRecord.locationArea.length);
			}

			if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_routingArea_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "routingArea ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.routingArea.value, _tempa->value.u.sgsnSMTRecord.routingArea.length);
			}

			if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_cellIdentity_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "cellIdentity ");
			    _oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.cellIdentity.value, _tempa->value.u.sgsnSMTRecord.cellIdentity.length);
			}

			ossPrint(_g, ",");
			_oss_indent(_g, 0);
			ossPrint(_g, "originationTime ");
			_oss_prt_oct(_g, _tempa->value.u.sgsnSMTRecord.originationTime.value, _tempa->value.u.sgsnSMTRecord.originationTime.length);
			if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_smsResult_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "smsResult ");
			    _p_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnSMTRecord.smsResult);
			}

			if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_recordExtensions_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "recordExtensions ");
			    _p_Fin_M_ManagementExtensions(_g, &_tempa->value.u.sgsnSMTRecord.recordExtensions);
			}

			if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_nodeID_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "nodeID ");
			    _oss_prt_nchar(_g, (char *)_tempa->value.u.sgsnSMTRecord.nodeID);
			}

			if (_tempa->value.u.sgsnSMTRecord.bit_mask & Fin_M_SGSNSMTRecord_localSequenceNumber_present) {
			    ossPrint(_g, ",");
			    _oss_indent(_g, 0);
			    ossPrint(_g, "localSequenceNumber ");
			    {
				ULONG_LONG value = _tempa->value.u.sgsnSMTRecord.localSequenceNumber;
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

		_tempa = _tempb;
		if (_tempa)
		    ossPrint(_g, ",");
	    }
	}

	_oss_indent(_g, -1);
	ossPrint(_g, "}");
	_oss_indent(_g, -1);
	ossPrint(_g, "}");
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
	    ossPrint(_g, "%sBSRecord %s ", _pdunum < 0 ? "" : "value ", _pdunum < 0 ? ":" : "::=");
	    _rc = _pFin_M_BSRecord(_g, _data);
	    break;

	default:  return(PDU_RANGE);
    }
    if (_pdunum > 0)
	ossPrint(_g, "\n");
    return _rc;
}

#endif

/* Entry points */
static const _entry_point_struct _entry_points = {_ossinit_CDRF_2GCDR_Fin_M, _emCDR2G, _dmCDR2G, _fmCDR2G, 16,
 NULL, NULL, 0, 0, NULL
#ifdef OSSPRINT
, _pmCDR2G
#endif
};
static void _oss_post_init(struct ossGlobal *world) {
    static const unsigned char _oss_typeinfo[] = {
        0x00, 0x79, 0x33, 0x5a, 0xfd, 0x82, 0x38, 0xc7, 0x1f, 0x74,
        0x19, 0xdf, 0x1f, 0x69, 0x19, 0xb6, 0x89, 0xcf, 0x11, 0x73,
        0xc2, 0x4e, 0xb8, 0x9b, 0x86, 0x39, 0xa4, 0xce, 0x04, 0x43,
        0x35, 0x0e, 0x9f, 0x44, 0x90, 0x17, 0x10, 0x1c, 0x21, 0x7e,
        0x88, 0x9e, 0xdd, 0x89, 0x7f, 0x2f, 0xf0, 0xac, 0xc8, 0x27,
        0xad, 0x92, 0x42, 0xd3, 0x03, 0xb4, 0xd5, 0x1d, 0xfc, 0xd9,
        0x55, 0xa8, 0x77, 0x85, 0xb8, 0x05, 0x74, 0x04, 0xb1, 0x9d,
        0xa0, 0x2d, 0x91, 0xaa, 0xb6, 0x9a, 0x2b, 0x6e, 0x13, 0xdb,
        0x9f, 0x78, 0xb1, 0xd8, 0xd9, 0x63, 0x86, 0x13, 0xd7, 0x22,
        0xa3, 0x2f, 0x83, 0x66, 0xbb, 0x22, 0x45, 0xbf, 0x8f, 0xf3,
        0x59, 0xca, 0x0e, 0x4f, 0xd0, 0x97, 0x9c, 0xbd, 0xd0, 0x14,
        0x2d, 0x77, 0x85, 0xe3, 0x56, 0xb8, 0xfd, 0x9b, 0x39, 0x8b,
        0x01, 0x82, 0xc3
    };
    ossInitRootContext(world, (unsigned char *)_oss_typeinfo);
}

#ifdef _OSSGETHEADER
void *DLL_ENTRY_FDEF ossGetHeader()
{
    return (void *)&_entry_points;
}
#endif /* _OSSGETHEADER */

void *CDRF_2GCDR_Fin_M = (void *)&_entry_points;
