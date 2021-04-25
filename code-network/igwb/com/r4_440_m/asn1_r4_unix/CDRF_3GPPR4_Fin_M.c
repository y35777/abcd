/********************************************************/
/* Copyright (C) 2004 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_R4_Fin_M */
/* Created: Fri Mar 05 11:02:50 2004 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Solaris 2.3 or later */
/* Target machine type: SPARC */
/* C compiler options required: None */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_3GPPR4_Fin_M.c -headerfile CDRF_3GPPR4_Fin_M.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix R4_Fin_M_ -nodebug -dualheader -ansi -verbose
 * -ber -root D:\application\ossasn1\solaris\asn1dflt.solaris-sparc
 * D:\yutemp\lidufang\0305\R4\r4_fin_M\CDRF_R4_Fin_M.asn
 */

#include   <stddef.h>
#include   <string.h>
#include   <stdio.h>
#include   <limits.h>
#include   "asn1code.h"
#include   "asn1util.h"
#include   "ossber.h"
#include   "../asn1_r4/CDRF_3GPPR4_Fin_M.h"

#if ((OSS_TOED_API_LEVEL + 0) < 3)
    #define _oss_dec_getmem_internal	_oss_dec_getmem
#endif /* OSS_TOED_API_LEVEL */

int R4_Fin_M_maxAddressLength = 20;

int R4_Fin_M_maxISDN_AddressLength = 9;

int R4_Fin_M_maxExt_GeographicalInformation = 20;

static ossBoolean _v0 = FALSE;


extern void _oss_run_TOED_on_SPARC_SOLARIS_23(void);
static void _oss_post_init(struct ossGlobal *world);

static void _ossinit_CDRF_R4_Fin_M(struct ossGlobal *world) {
    _oss_run_TOED_on_SPARC_SOLARIS_23();
    ossLinkBer(world);
    _oss_post_init(world);
}

static long _edR4_Fin_M_IPBinaryAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_IPBinaryAddress *_in_data);
static void _d_R4_Fin_M_IPBinaryAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_IPBinaryAddress *_out_data);
static int _f_R4_Fin_M_IPBinaryAddress(struct ossGlobal *_g, R4_Fin_M_IPBinaryAddress *_data_ptr);
static long _ed0R4_Fin_M_IPTextRepresentedA(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_IPTextRepresentedAddress *_in_data);
static void _d_0R4_Fin_M_IPTextRepresentedA(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_IPTextRepresentedAddress *_out_data);
static int _f_0R4_Fin_M_IPTextRepresentedA(struct ossGlobal *_g, R4_Fin_M_IPTextRepresentedAddress *_data_ptr);
static long _edR4_Fin_M_IPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_IPAddress *_in_data);
static void _d_R4_Fin_M_IPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_IPAddress *_out_data);
static int _f_R4_Fin_M_IPAddress(struct ossGlobal *_g, R4_Fin_M_IPAddress *_data_ptr);
static long _edR4_Fin_M_PDPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_PDPAddress *_in_data);
static void _d_R4_Fin_M_PDPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_PDPAddress *_out_data);
static int _f_R4_Fin_M_PDPAddress(struct ossGlobal *_g, R4_Fin_M_PDPAddress *_data_ptr);
static long _ed1R4_Fin_M_ChangeOfCharCondit(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_ChangeOfCharCondition *_in_data);
static void _d_1R4_Fin_M_ChangeOfCharCondit(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_ChangeOfCharCondition *_out_data);
static int _f_1R4_Fin_M_ChangeOfCharCondit(struct ossGlobal *_g, R4_Fin_M_ChangeOfCharCondition *_data_ptr);
static long _edR4_Fin_M_ManagementExtension(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_ManagementExtension *_in_data);
static void _d_R4_Fin_M_ManagementExtension(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_ManagementExtension *_out_data);
static int _f_R4_Fin_M_ManagementExtension(struct ossGlobal *_g, R4_Fin_M_ManagementExtension *_data_ptr);
static long _edR4_Fin_M_Diagnostics(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_Diagnostics *_in_data);
static void _d_R4_Fin_M_Diagnostics(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_Diagnostics *_out_data);
static int _f_R4_Fin_M_Diagnostics(struct ossGlobal *_g, R4_Fin_M_Diagnostics *_data_ptr);
static long _edR4_Fin_M__seqof1(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M__seqof1 *_in_data);
static void _d_R4_Fin_M__seqof1(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M__seqof1 *_out_data);
static int _f_R4_Fin_M__seqof1(struct ossGlobal *_g, R4_Fin_M__seqof1 *_data_ptr);
static long _edR4_Fin_M_SequenceList(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_SequenceList *_in_data);
static void _d_R4_Fin_M_SequenceList(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_SequenceList *_out_data);
static int _f_R4_Fin_M_SequenceList(struct ossGlobal *_g, R4_Fin_M_SequenceList *_data_ptr);
static long _edR4_Fin_M_7(struct ossGlobal *_g, char **_out_pos, long *_max_len, struct R4_Fin_M__seqof1_ **_in_data);
static void _d_R4_Fin_M_7(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, struct R4_Fin_M__seqof1_ **_out_data);
static int _f_R4_Fin_M_7(struct ossGlobal *_g, struct R4_Fin_M__seqof1_ **_data_ptr);
static long _ed2R4_Fin_M_LocalSequenceNumbe(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_LocalSequenceNumberList *_in_data);
static void _d_2R4_Fin_M_LocalSequenceNumbe(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_LocalSequenceNumberList *_out_data);
static int _f_2R4_Fin_M_LocalSequenceNumbe(struct ossGlobal *_g, R4_Fin_M_LocalSequenceNumberList *_data_ptr);
static long _edR4_Fin_M_CAMELInformationSMS(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_CAMELInformationSMS *_in_data);
static void _d_R4_Fin_M_CAMELInformationSMS(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_CAMELInformationSMS *_out_data);
static int _f_R4_Fin_M_CAMELInformationSMS(struct ossGlobal *_g, R4_Fin_M_CAMELInformationSMS *_data_ptr);
static long _edR4_Fin_M_LCSClientExternalID(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_LCSClientExternalID *_in_data);
static void _d_R4_Fin_M_LCSClientExternalID(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_LCSClientExternalID *_out_data);
static int _f_R4_Fin_M_LCSClientExternalID(struct ossGlobal *_g, R4_Fin_M_LCSClientExternalID *_data_ptr);
static long _edR4_Fin_M_LCSClientIdentity(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_LCSClientIdentity *_in_data);
static void _d_R4_Fin_M_LCSClientIdentity(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_LCSClientIdentity *_out_data);
static int _f_R4_Fin_M_LCSClientIdentity(struct ossGlobal *_g, R4_Fin_M_LCSClientIdentity *_data_ptr);
static long _edR4_Fin_M_LocationAreaAndCell(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_LocationAreaAndCell *_in_data);
static void _d_R4_Fin_M_LocationAreaAndCell(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_LocationAreaAndCell *_out_data);
static int _f_R4_Fin_M_LocationAreaAndCell(struct ossGlobal *_g, R4_Fin_M_LocationAreaAndCell *_data_ptr);
static long _ed3R4_Fin_M_ManagementExtensio(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_ManagementExtensions *_in_data);
static void _d_3R4_Fin_M_ManagementExtensio(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_ManagementExtensions *_out_data);
static int _f_3R4_Fin_M_ManagementExtensio(struct ossGlobal *_g, R4_Fin_M_ManagementExtensions *_data_ptr);
static int _eR4_Fin_M_BSRecord(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags);
static int _dR4_Fin_M_BSRecord(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen);
static int _fR4_Fin_M_BSRecord(struct ossGlobal *_g, void *_data);
static int _eR4_Fin_M_Horizontal_Accuracy(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags);
static int _dR4_Fin_M_Horizontal_Accuracy(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen);
static int _fR4_Fin_M_Horizontal_Accuracy(struct ossGlobal *_g, void *_data);
static int _eR4_Fin_M_ResponseTime(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags);
static int _dR4_Fin_M_ResponseTime(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen);
static int _fR4_Fin_M_ResponseTime(struct ossGlobal *_g, void *_data);
static int _eR4_Fin_M_Vertical_Accuracy(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags);
static int _dR4_Fin_M_Vertical_Accuracy(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen);
static int _fR4_Fin_M_Vertical_Accuracy(struct ossGlobal *_g, void *_data);
static int _emCDRF_R4(struct ossGlobal *_g, int _pdunum, void *_inbuf, char **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH]);
static int _dmCDRF_R4(struct ossGlobal *_g, int *_pdunum, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH]);
static int _fmCDRF_R4(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl);



#if OSSDEBUG > 1
static const _Context_Id _context_id[] = {
    {"field #1", "OCTET STRING", _octetstring_type},
    {"field #2", "OCTET STRING", _octetstring_type},
    {"field #1", "Character String", _charstring_type},
    {"field #2", "Character String", _charstring_type},
    {"field #1", "CHOICE", _choice_type},
    {"field #2", "CHOICE", _choice_type},
    {"field #6", "OCTET STRING", _octetstring_type},
    {"field #5", "ENUMERATED", _enumerated_type},
    {"field #4", "INTEGER", _integer_type},
    {"field #3", "INTEGER", _integer_type},
    {"field #3", "OCTET STRING", _octetstring_type},
    {"field #2", "BOOLEAN", _boolean_type},
    {"field #1", "OBJECT IDENTIFIER", _object_identifier_type},
    {"field #1", "INTEGER", _integer_type},
    {"field #2", "INTEGER", _integer_type},
    {"field #4", "SEQUENCE", _sequence_type},
    {"field #5", "SEQUENCE", _sequence_type},
    {"field #6", "ENUMERATED", _enumerated_type},
    {"field #7", "ENUMERATED", _enumerated_type},
    {NULL, "INTEGER", _integer_type},
    {"field #2", "SEQUENCE OF", _sequence_of_type},
    {"field #8", "OCTET STRING", _octetstring_type},
    {"field #7", "OCTET STRING", _octetstring_type},
    {"field #5", "OCTET STRING", _octetstring_type},
    {"field #4", "OCTET STRING", _octetstring_type},
    {"field #3", "ENUMERATED", _enumerated_type},
    {"field #1", "SEQUENCE", _sequence_type},
    {NULL, "SEQUENCE", _sequence_type},
    {"PDU #1", "SEQUENCE", _sequence_type},
    {NULL, "CHOICE", _choice_type},
    {"field #1", "SET", _set_type},
    {"field #34", "ENUMERATED", _enumerated_type},
    {"field #33", "BOOLEAN", _boolean_type},
    {"field #32", "ENUMERATED", _enumerated_type},
    {"field #31", "SEQUENCE OF", _sequence_of_type},
    {"field #30", "SET", _set_type},
    {"field #9", "BOOLEAN", _boolean_type},
    {"field #7", "BIT STRING", _bitstring_type},
    {"field #6", "INTEGER", _integer_type},
    {"field #5", "Character String", _charstring_type},
    {"field #4", "Character String", _charstring_type},
    {"field #29", "ENUMERATED", _enumerated_type},
    {"field #28", "OCTET STRING", _octetstring_type},
    {"field #27", "OCTET STRING", _octetstring_type},
    {"field #26", "Character String", _charstring_type},
    {"field #25", "ENUMERATED", _enumerated_type},
    {"field #24", "SEQUENCE OF", _sequence_of_type},
    {"field #23", "SET OF", _set_of_type},
    {"field #22", "Character String", _charstring_type},
    {"field #21", "SEQUENCE OF", _sequence_of_type},
    {"field #20", "CHOICE", _choice_type},
    {"field #19", "INTEGER", _integer_type},
    {"field #18", "BOOLEAN", _boolean_type},
    {"field #17", "INTEGER", _integer_type},
    {"field #16", "OCTET STRING", _octetstring_type},
    {"field #15", "SEQUENCE OF", _sequence_of_type},
    {"field #14", "CHOICE", _choice_type},
    {"field #13", "OCTET STRING", _octetstring_type},
    {"field #12", "Character String", _charstring_type},
    {"field #11", "CHOICE", _choice_type},
    {"field #10", "INTEGER", _integer_type},
    {"field #9", "OCTET STRING", _octetstring_type},
    {"field #5", "SEQUENCE OF", _sequence_of_type},
    {"field #2", "SET", _set_type},
    {"field #24", "OCTET STRING", _octetstring_type},
    {"field #23", "ENUMERATED", _enumerated_type},
    {"field #22", "OCTET STRING", _octetstring_type},
    {"field #21", "OCTET STRING", _octetstring_type},
    {"field #20", "ENUMERATED", _enumerated_type},
    {"field #19", "SEQUENCE", _sequence_type},
    {"field #18", "SET OF", _set_of_type},
    {"field #17", "Character String", _charstring_type},
    {"field #16", "SEQUENCE", _sequence_type},
    {"field #15", "CHOICE", _choice_type},
    {"field #14", "INTEGER", _integer_type},
    {"field #13", "INTEGER", _integer_type},
    {"field #12", "OCTET STRING", _octetstring_type},
    {"field #11", "SEQUENCE OF", _sequence_of_type},
    {"field #10", "BOOLEAN", _boolean_type},
    {"field #9", "CHOICE", _choice_type},
    {"field #7", "Character String", _charstring_type},
    {"field #6", "SEQUENCE OF", _sequence_of_type},
    {"field #5", "INTEGER", _integer_type},
    {"field #4", "CHOICE", _choice_type},
    {"field #3", "SET", _set_type},
    {"field #22", "ENUMERATED", _enumerated_type},
    {"field #21", "SET", _set_type},
    {"field #7", "BOOLEAN", _boolean_type},
    {"field #5", "BIT STRING", _bitstring_type},
    {"field #20", "OCTET STRING", _octetstring_type},
    {"field #19", "OCTET STRING", _octetstring_type},
    {"field #18", "INTEGER", _integer_type},
    {"field #17", "SET OF", _set_of_type},
    {"field #16", "Character String", _charstring_type},
    {"field #15", "INTEGER", _integer_type},
    {"field #12", "BOOLEAN", _boolean_type},
    {"field #11", "INTEGER", _integer_type},
    {"field #10", "OCTET STRING", _octetstring_type},
    {"field #9", "SEQUENCE OF", _sequence_of_type},
    {"field #4", "SET", _set_type},
    {"field #21", "ENUMERATED", _enumerated_type},
    {"field #20", "SET", _set_type},
    {"field #18", "ENUMERATED", _enumerated_type},
    {"field #17", "OCTET STRING", _octetstring_type},
    {"field #16", "INTEGER", _integer_type},
    {"field #15", "Character String", _charstring_type},
    {"field #14", "SET OF", _set_of_type},
    {"field #13", "CHOICE", _choice_type},
    {"field #11", "OCTET STRING", _octetstring_type},
    {"field #5", "SET", _set_type},
    {"field #19", "SET", _set_type},
    {"field #17", "ENUMERATED", _enumerated_type},
    {"field #14", "Character String", _charstring_type},
    {"field #13", "SET OF", _set_of_type},
    {"field #12", "CHOICE", _choice_type},
    {"field #6", "SET", _set_type},
    {"field #27", "INTEGER", _integer_type},
    {"field #26", "SET OF", _set_of_type},
    {"field #24", "ENUMERATED", _enumerated_type},
    {"field #23", "OCTET STRING", _octetstring_type},
    {"field #22", "INTEGER", _integer_type},
    {"field #21", "Character String", _charstring_type},
    {"field #18", "OCTET STRING", _octetstring_type},
    {"field #15", "NULL", _null_type},
    {"field #14", "ENUMERATED", _enumerated_type},
    {"field #8", "SEQUENCE", _sequence_type},
    {"field #2", "BIT STRING", _bitstring_type},
    {"field #1", "ENUMERATED", _enumerated_type},
    {"field #7", "CHOICE", _choice_type},
    {"field #7", "SET", _set_type},
    {"field #25", "INTEGER", _integer_type},
    {"field #24", "SET OF", _set_of_type},
    {"field #20", "INTEGER", _integer_type},
    {"field #19", "Character String", _charstring_type},
    {"field #18", "CHOICE", _choice_type},
    {"field #15", "OCTET STRING", _octetstring_type},
    {"field #14", "SEQUENCE", _sequence_type},
    {"field #8", "ENUMERATED", _enumerated_type},
    {"field #8", "SET", _set_type},
    {"PDU #2", "OCTET STRING", _octetstring_type},
    {"PDU #3", "SEQUENCE", _sequence_type},
    {"PDU #4", "OCTET STRING", _octetstring_type}
};
#endif


static long _edR4_Fin_M_IPBinaryAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_IPBinaryAddress *_in_data)
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

static long _ed0R4_Fin_M_IPTextRepresentedA(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_IPTextRepresentedAddress *_in_data)
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

static long _edR4_Fin_M_IPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_IPAddress *_in_data)
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

	    _data_len = _edR4_Fin_M_IPBinaryAddress(_g, _out_pos, &_out_len, &_in_data->u.iPBinaryAddress);
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

	    _data_len = _ed0R4_Fin_M_IPTextRepresentedA(_g, _out_pos, &_out_len, &_in_data->u.iPTextRepresentedAddress);
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

static long _edR4_Fin_M_PDPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_PDPAddress *_in_data)
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
	    _element._id = (_Context_Id *)&_context_id[4];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.iPAddress);
	    _constructed = TRUE;
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

static long _ed1R4_Fin_M_ChangeOfCharCondit(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_ChangeOfCharCondition *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[6];
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
	_element._id = (_Context_Id *)&_context_id[7];
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
	_element._id = (_Context_Id *)&_context_id[8];
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
	_element._id = (_Context_Id *)&_context_id[9];
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
	_element._id = (_Context_Id *)&_context_id[1];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_qosNegotiated_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->qosNegotiated, 2, 12);
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
	_element._id = (_Context_Id *)&_context_id[0];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_qosRequested_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->qosRequested, 2, 12);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
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

static long _edR4_Fin_M_ManagementExtension(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_ManagementExtension *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[10];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_uoct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->information, 4, -1);
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
	_element._id = (_Context_Id *)&_context_id[11];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_significance_present) {
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
	_element._id = (_Context_Id *)&_context_id[12];
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

static long _edR4_Fin_M_Diagnostics(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_Diagnostics *_in_data)
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
	    _element._id = (_Context_Id *)&_context_id[13];
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
	    _element._id = (_Context_Id *)&_context_id[14];
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
	    _element._id = (_Context_Id *)&_context_id[9];
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
	    _element._id = (_Context_Id *)&_context_id[15];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edR4_Fin_M_ManagementExtension(_g, _out_pos, &_out_len, &_in_data->u.networkSpecificCause);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 5:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[16];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _edR4_Fin_M_ManagementExtension(_g, _out_pos, &_out_len, &_in_data->u.manufacturerSpecificCause);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 6:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[17];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.positionMethodFailureCause);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 7:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[18];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.unauthorizedLCSClientCause);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
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

static long _edR4_Fin_M__seqof1(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M__seqof1 *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	struct R4_Fin_M__seqof1_	*_temp;
	unsigned long count;
	unsigned long _lindex;
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
	_element._id = (_Context_Id *)&_context_id[19];
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

	    _temp = (struct R4_Fin_M__seqof1_ *) _oss_enc_pop(_g);
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

static long _edR4_Fin_M_SequenceList(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_SequenceList *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[20];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_recSequenceNumber_present) {
	    _data_len = _edR4_Fin_M__seqof1(_g, _out_pos, &_out_len, &_in_data->recSequenceNumber);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
	    _total_len += _data_len;
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[4];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_in_data->sgsnAddress);
	_constructed = TRUE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
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

static long _edR4_Fin_M_7(struct ossGlobal *_g, char **_out_pos, long *_max_len, struct R4_Fin_M__seqof1_ **_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	struct R4_Fin_M__seqof1_	*_temp;
	unsigned long count;
	unsigned long _lindex;
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
	_element._id = (_Context_Id *)&_context_id[19];
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

	    _temp = (struct R4_Fin_M__seqof1_ *) _oss_enc_pop(_g);
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

static long _ed2R4_Fin_M_LocalSequenceNumbe(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_LocalSequenceNumberList *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[20];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_localSeqNumberList_present) {
	    _data_len = _edR4_Fin_M_7(_g, _out_pos, &_out_len, &_in_data->localSeqNumberList);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
	    _total_len += _data_len;
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[4];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_in_data->gsnAddress);
	_constructed = TRUE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
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

static long _edR4_Fin_M_CAMELInformationSMS(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_CAMELInformationSMS *_in_data)
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

	if (_in_data->bit_mask & R4_Fin_M_smsReferenceNumber_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->smsReferenceNumber, 2, 8);
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
	_element._id = (_Context_Id *)&_context_id[22];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_CAMELInformationSMS_freeFormatData_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->freeFormatData, 2, 160);
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
	_element._id = (_Context_Id *)&_context_id[6];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_cAMELSMSCAddress_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->cAMELSMSCAddress, 2, 20);
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
	_element._id = (_Context_Id *)&_context_id[23];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_cAMELDestinationSubscriberNumber_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->cAMELDestinationSubscriberNumber, 2, 12);
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
	_element._id = (_Context_Id *)&_context_id[24];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_cAMELCallingPartyNumber_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->cAMELCallingPartyNumber, 2, 10);
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
	_element._id = (_Context_Id *)&_context_id[25];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_defaultSMSHandling_present) {
	    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->defaultSMSHandling);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
	    _total_len += _data_len;
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[14];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_CAMELInformationSMS_serviceKey_present) {
	    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->serviceKey);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
	    _total_len += _data_len;
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[0];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_CAMELInformationSMS_sCFAddress_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->sCFAddress, 2, 20);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
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

static long _edR4_Fin_M_LCSClientExternalID(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_LCSClientExternalID *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[0];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_externalAddress_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->externalAddress, 2, 20);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
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

static long _edR4_Fin_M_LCSClientIdentity(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_LCSClientIdentity *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[25];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_lcsClientInternalID_present) {
	    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->lcsClientInternalID);
	    _constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
	    _total_len += _data_len;
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[1];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_lcsClientDialedByMS_present) {
	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->lcsClientDialedByMS, 2, 20);
	    if (_data_len < 0) {
		_constructed = TRUE;
		_data_len = -_data_len;
	    }
	    else
		_constructed = FALSE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
	    _total_len += _data_len;
	}

#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[26];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & R4_Fin_M_lcsClientExternalID_present) {
	    _data_len = _edR4_Fin_M_LCSClientExternalID(_g, _out_pos, &_out_len, &_in_data->lcsClientExternalID);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8000, _constructed);
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

static long _edR4_Fin_M_LocationAreaAndCell(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_LocationAreaAndCell *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[1];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->cellId, 2, 2);
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
	_element._id = (_Context_Id *)&_context_id[0];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->locationAreaCode, 2, 2);
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
    *_max_len = _out_len;
    return(_data_len);
}

static long _ed3R4_Fin_M_ManagementExtensio(struct ossGlobal *_g, char **_out_pos, long *_max_len, R4_Fin_M_ManagementExtensions *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	struct R4_Fin_M_ManagementExtensions_	*_temp;
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
	for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
	    _element._occurrence = count - _lindex;
#endif

	    _temp = (struct R4_Fin_M_ManagementExtensions_ *) _oss_enc_pop(_g);
	    _data_len = _edR4_Fin_M_ManagementExtension(_g, _out_pos, &_out_len, &_temp->value);
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

static int _eR4_Fin_M_BSRecord(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    R4_Fin_M_BSRecord	*_in_data;
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

    _in_data = (R4_Fin_M_BSRecord *) _inbuf;
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
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[20];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	{
	    struct R4_Fin_M__seqof8	*_temp;
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

		_temp = (struct R4_Fin_M__seqof8 *) _oss_enc_pop(_g);
		{
#if OSSDEBUG > 1
		    _Context_Element _element;

		    _element._occurrence = 0;
#endif

		    switch (_temp->value.choice) {
		    case 1:
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[30];
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
			    _element._id = (_Context_Id *)&_context_id[31];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.consolidationResult);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8022, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[32];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_dynamicAddressFlag_present) {
				_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.dynamicAddressFlag);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8021, _constructed);
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

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_chChSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.chChSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8020, _constructed);
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

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_rNCUnsentDownlinkVolumeList_present) {
				{
				    struct R4_Fin_M__seqof6	*_temp1;
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

				    for (_temp1 = _temp->value.u.sgsnPDPRecord.rNCUnsentDownlinkVolumeList, count = 0; _temp1; _temp1 = _temp1->next) {
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

					_temp1 = (struct R4_Fin_M__seqof6 *) _oss_enc_pop(_g);
					{
					    long _total_len = 0;

#if OSSDEBUG > 1
					    _Context_Element _element;

					    _element._occurrence = 0;
#endif

#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[20];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp1->value.bit_mask & R4_Fin_M_rNCUnsentDnVolumeList_present) {
						{
						    struct R4_Fin_M__seqof1_	*_temp2;
						    unsigned long count;
						    unsigned long _lindex;
						    long _total_len = 0;

#if OSSDEBUG > 1
						    _Context_Element _element;

						    _element._occurrence = 0;
						    _element._id = (_Context_Id *)&_context_id[19];
						    _element._parent = _oss_c->_oss_context_anchor._last;
						    _oss_c->_oss_context_anchor._last = &_element;
#endif

						    for (_temp2 = _temp1->value.rNCUnsentDnVolumeList, count = 0; _temp2; _temp2 = _temp2->next) {
#if OSSDEBUG > 1
							_element._occurrence = count + 1;
#endif

							_oss_enc_push(_g, _temp2);
							count++;
						    }
						    for (_lindex = 0; _lindex < count; _lindex++) {
#if OSSDEBUG > 1
							_element._occurrence = count - _lindex;
#endif

							_temp2 = (struct R4_Fin_M__seqof1_ *) _oss_enc_pop(_g);
							_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp2->value);
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
						_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
						_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
						_total_len += _data_len;
					    }

#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[4];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_temp1->value.sgsnAddress);
					    _constructed = TRUE;
					    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
					    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
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
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801F, _constructed);
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

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_cAMELInformationPDP_present) {
				{
				    long _total_len = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[36];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_fFDAppendIndicator_present) {
					_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.fFDAppendIndicator);
					_constructed = FALSE;
					_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
					_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
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

				    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_freeFormatData_present) {
					_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.freeFormatData, 2, 160);
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
				    _element._id = (_Context_Id *)&_context_id[37];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_levelOfCAMELService_present) {
					_data_len = _oss_encd_pbit(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.levelOfCAMELService,3);
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
				    _element._id = (_Context_Id *)&_context_id[38];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_numberOfDPEncountered_present) {
					_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.numberOfDPEncountered);
					_constructed = FALSE;
					_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
					_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
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

				    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_cAMELAccessPointNameOI_present) {
					_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.cAMELAccessPointNameOI, 37);
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
				    _element._id = (_Context_Id *)&_context_id[40];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_cAMELAccessPointNameNI_present) {
					_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.cAMELAccessPointNameNI, 63);
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
				    _element._id = (_Context_Id *)&_context_id[25];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_defaultTransactionHandling_present) {
					_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.defaultTransactionHandling);
					_constructed = FALSE;
					_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
					_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
					_total_len += _data_len;
				    }

#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[14];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_serviceKey_present) {
					_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.serviceKey);
					_constructed = FALSE;
					_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
					_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
					_total_len += _data_len;
				    }

#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[0];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_sCFAddress_present) {
					_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.sCFAddress, 2, 20);
					if (_data_len < 0) {
					    _constructed = TRUE;
					    _data_len = -_data_len;
					}
					else
					    _constructed = FALSE;
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
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801E, _constructed);
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

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_systemType_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.systemType);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801D, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[42];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.chargingCharacteristics, 2, 2);
			    if (_data_len < 0) {
				_constructed = TRUE;
				_data_len = -_data_len;
			    }
			    else
				_constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801C, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[43];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.servedMSISDN, 2, 9);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801B, _constructed);
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

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_accessPointNameOI_present) {
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[45];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_apnSelectionMode_present) {
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
			    _element._id = (_Context_Id *)&_context_id[46];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_localSequenceNumberList_present) {
				{
				    struct R4_Fin_M__seqof5	*_temp1;
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

				    for (_temp1 = _temp->value.u.sgsnPDPRecord.localSequenceNumberList, count = 0; _temp1; _temp1 = _temp1->next) {
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

					_temp1 = (struct R4_Fin_M__seqof5 *) _oss_enc_pop(_g);
					_data_len = _ed2R4_Fin_M_LocalSequenceNumbe(_g, _out_pos, &_out_len, &_temp1->value);
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
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8018, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[47];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_recordExtensions_present) {
				_data_len = _ed3R4_Fin_M_ManagementExtensio(_g, _out_pos, &_out_len, &_temp->value.u.sgsnPDPRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[48];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_nodeID_present) {
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
			    _element._id = (_Context_Id *)&_context_id[49];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_recSequenceNumList_present) {
				{
				    struct R4_Fin_M__seqof4	*_temp1;
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

				    for (_temp1 = _temp->value.u.sgsnPDPRecord.recSequenceNumList, count = 0; _temp1; _temp1 = _temp1->next) {
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

					_temp1 = (struct R4_Fin_M__seqof4 *) _oss_enc_pop(_g);
					_data_len = _edR4_Fin_M_SequenceList(_g, _out_pos, &_out_len, &_temp1->value);
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
			    _element._id = (_Context_Id *)&_context_id[50];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_diagnostics_present) {
				_data_len = _edR4_Fin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnPDPRecord.diagnostics);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
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

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnPDPRecord.causeForRecClosing);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[52];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_sgsnChange_present) {
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
			    _element._id = (_Context_Id *)&_context_id[53];
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
			    _element._id = (_Context_Id *)&_context_id[54];
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
			    _element._id = (_Context_Id *)&_context_id[55];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_listOfTrafficVolumes_present) {
				{
				    struct R4_Fin_M__seqof3_	*_temp1;
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

					_temp1 = (struct R4_Fin_M__seqof3_ *) _oss_enc_pop(_g);
					_data_len = _ed1R4_Fin_M_ChangeOfCharCondit(_g, _out_pos, &_out_len, &_temp1->value);
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[56];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_servedPDPAddress_present) {
				_data_len = _edR4_Fin_M_PDPAddress(_g, _out_pos, &_out_len, &_temp->value.u.sgsnPDPRecord.servedPDPAddress);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
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

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_pdpType_present) {
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[58];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_accessPointNameNI_present) {
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[59];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_temp->value.u.sgsnPDPRecord.ggsnAddressUsed);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[60];
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_cellIdentifier_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.cellIdentifier, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[21];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_locationAreaCode_present) {
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
			    _element._id = (_Context_Id *)&_context_id[22];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_routingArea_present) {
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
			    _element._id = (_Context_Id *)&_context_id[6];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_msNetworkCapability_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnPDPRecord.msNetworkCapability, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[62];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_sgsnAddress_present) {
				{
				    struct R4_Fin_M__seqof2_	*_temp1;
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

					_temp1 = (struct R4_Fin_M__seqof2_ *) _oss_enc_pop(_g);
					_data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_temp1->value);
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[24];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_servedIMEI_present) {
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
			    _element._id = (_Context_Id *)&_context_id[10];
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
			    _element._id = (_Context_Id *)&_context_id[11];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_networkInitiation_present) {
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
			    _element._id = (_Context_Id *)&_context_id[13];
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
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			break;

		    case 2:
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[63];
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
			    _element._id = (_Context_Id *)&_context_id[45];
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
			    _element._id = (_Context_Id *)&_context_id[64];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_sgsnPLMNIdentifier_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.ggsnPDPRecord.sgsnPLMNIdentifier, 2, 3);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801B, _constructed);
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

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_chChSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.chChSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8018, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.ggsnPDPRecord.chargingCharacteristics, 2, 2);
			    if (_data_len < 0) {
				_constructed = TRUE;
				_data_len = -_data_len;
			    }
			    else
				_constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[67];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.ggsnPDPRecord.servedMSISDN, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[68];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_apnSelectionMode_present) {
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
			    _element._id = (_Context_Id *)&_context_id[69];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_localSequenceNumberList_present) {
				_data_len = _ed2R4_Fin_M_LocalSequenceNumbe(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.localSequenceNumberList);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
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

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_recordExtensions_present) {
				_data_len = _ed3R4_Fin_M_ManagementExtensio(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[71];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_nodeID_present) {
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
			    _element._id = (_Context_Id *)&_context_id[72];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_recSequenceNumList_present) {
				_data_len = _edR4_Fin_M_SequenceList(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.recSequenceNumList);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[73];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_diagnostics_present) {
				_data_len = _edR4_Fin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.diagnostics);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
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

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.ggsnPDPRecord.causeForRecClosing);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[75];
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
			    _element._id = (_Context_Id *)&_context_id[76];
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
			    _element._id = (_Context_Id *)&_context_id[77];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_listOfTrafficVolumes_present) {
				{
				    struct R4_Fin_M__seqof3_	*_temp1;
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

					_temp1 = (struct R4_Fin_M__seqof3_ *) _oss_enc_pop(_g);
					_data_len = _ed1R4_Fin_M_ChangeOfCharCondit(_g, _out_pos, &_out_len, &_temp1->value);
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[78];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_dynamicAddressFlag_present) {
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
			    _element._id = (_Context_Id *)&_context_id[79];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_servedPDPAddress_present) {
				_data_len = _edR4_Fin_M_PDPAddress(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.servedPDPAddress);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
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

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_pdpType_present) {
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[80];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_accessPointNameNI_present) {
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[81];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    {
				struct R4_Fin_M__seqof2_	*_temp1;
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

				    _temp1 = (struct R4_Fin_M__seqof2_ *) _oss_enc_pop(_g);
				    _data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_temp1->value);
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
			    _element._id = (_Context_Id *)&_context_id[82];
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
			    _element._id = (_Context_Id *)&_context_id[83];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_temp->value.u.ggsnPDPRecord.ggsnAddress);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[10];
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
			    _element._id = (_Context_Id *)&_context_id[11];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_networkInitiation_present) {
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
			    _element._id = (_Context_Id *)&_context_id[13];
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
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			break;

		    case 3:
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[84];
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
			    _element._id = (_Context_Id *)&_context_id[65];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_chChSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.chChSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8016, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[85];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_systemType_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.systemType);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
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

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_cAMELInformationMM_present) {
				{
				    long _total_len = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[87];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_fFDAppendIndicator_present) {
					_data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.cAMELInformationMM.fFDAppendIndicator);
					_constructed = FALSE;
					_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
					_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8007, _constructed);
					_total_len += _data_len;
				    }

#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[6];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_freeFormatData_present) {
					_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.cAMELInformationMM.freeFormatData, 2, 160);
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
				    _element._id = (_Context_Id *)&_context_id[88];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_levelOfCAMELService_present) {
					_data_len = _oss_encd_pbit(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.cAMELInformationMM.levelOfCAMELService,3);
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
				    _element._id = (_Context_Id *)&_context_id[8];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_numberOfDPEncountered_present) {
					_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.cAMELInformationMM.numberOfDPEncountered);
					_constructed = FALSE;
					_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
					_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
					_total_len += _data_len;
				    }

#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[25];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_defaultTransactionHandling_present) {
					_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.cAMELInformationMM.defaultTransactionHandling);
					_constructed = FALSE;
					_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
					_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
					_total_len += _data_len;
				    }

#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[14];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_serviceKey_present) {
					_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnMMRecord.cAMELInformationMM.serviceKey);
					_constructed = FALSE;
					_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
					_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
					_total_len += _data_len;
				    }

#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[0];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_sCFAddress_present) {
					_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.cAMELInformationMM.sCFAddress, 2, 20);
					if (_data_len < 0) {
					    _constructed = TRUE;
					    _data_len = -_data_len;
					}
					else
					    _constructed = FALSE;
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
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.chargingCharacteristics, 2, 2);
			    if (_data_len < 0) {
				_constructed = TRUE;
				_data_len = -_data_len;
			    }
			    else
				_constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[90];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.servedMSISDN, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[91];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_localSequenceNumber_present) {
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
			    _element._id = (_Context_Id *)&_context_id[92];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_recordExtensions_present) {
				_data_len = _ed3R4_Fin_M_ManagementExtensio(_g, _out_pos, &_out_len, &_temp->value.u.sgsnMMRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
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

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_nodeID_present) {
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
			    _element._id = (_Context_Id *)&_context_id[94];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_recordSequenceNumber_present) {
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
			    _element._id = (_Context_Id *)&_context_id[56];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_diagnostics_present) {
				_data_len = _edR4_Fin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnMMRecord.diagnostics);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[75];
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
			    _element._id = (_Context_Id *)&_context_id[95];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_sgsnChange_present) {
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
			    _element._id = (_Context_Id *)&_context_id[96];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_duration_present) {
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
			    _element._id = (_Context_Id *)&_context_id[97];
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
			    _element._id = (_Context_Id *)&_context_id[98];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_changeLocation_present) {
				{
				    struct R4_Fin_M__seqof7	*_temp1;
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

					_temp1 = (struct R4_Fin_M__seqof7 *) _oss_enc_pop(_g);
					{
					    long _total_len = 0;

#if OSSDEBUG > 1
					    _Context_Element _element;

					    _element._occurrence = 0;
#endif

#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[24];
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
					    _element._id = (_Context_Id *)&_context_id[10];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp1->value.bit_mask & R4_Fin_M_cellId_present) {
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
					    _element._id = (_Context_Id *)&_context_id[1];
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
					    _element._id = (_Context_Id *)&_context_id[0];
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
			    _element._id = (_Context_Id *)&_context_id[21];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_cellIdentifier_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.cellIdentifier, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[22];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_locationAreaCode_present) {
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
			    _element._id = (_Context_Id *)&_context_id[6];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_routingArea_present) {
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
			    _element._id = (_Context_Id *)&_context_id[23];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_msNetworkCapability_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnMMRecord.msNetworkCapability, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[83];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_sgsnAddress_present) {
				_data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_temp->value.u.sgsnMMRecord.sgsnAddress);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[10];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_servedIMEI_present) {
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
			    _element._id = (_Context_Id *)&_context_id[1];
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
			    _element._id = (_Context_Id *)&_context_id[13];
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
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8016, _constructed);
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			break;

		    case 4:
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[99];
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
			    _element._id = (_Context_Id *)&_context_id[100];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_chChSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMORecord.chChSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
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

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_cAMELInformationSMS_present) {
				_data_len = _edR4_Fin_M_CAMELInformationSMS(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMORecord.cAMELInformationSMS);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
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

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_destinationNumber_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.destinationNumber, 2, 12);
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
			    _element._id = (_Context_Id *)&_context_id[102];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_systemType_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMORecord.systemType);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.chargingCharacteristics, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[104];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_localSequenceNumber_present) {
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
			    _element._id = (_Context_Id *)&_context_id[105];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_nodeID_present) {
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
			    _element._id = (_Context_Id *)&_context_id[106];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_recordExtensions_present) {
				_data_len = _ed3R4_Fin_M_ManagementExtensio(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMORecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
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

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_smsResult_present) {
				_data_len = _edR4_Fin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMORecord.smsResult);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.eventTimeStamp, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[108];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_uoct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.messageReference, 4, -1);
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
			    _element._id = (_Context_Id *)&_context_id[97];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_cellIdentifier_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.cellIdentifier, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_routingArea_present) {
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
			    _element._id = (_Context_Id *)&_context_id[21];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_locationArea_present) {
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
			    _element._id = (_Context_Id *)&_context_id[22];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_recordingEntity_present) {
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[6];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_serviceCentre_present) {
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[23];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_msNetworkCapability_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.msNetworkCapability, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[24];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.servedMSISDN, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[10];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_servedIMEI_present) {
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
			    _element._id = (_Context_Id *)&_context_id[1];
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
			    _element._id = (_Context_Id *)&_context_id[13];
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
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			break;

		    case 5:
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[109];
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
			    _element._id = (_Context_Id *)&_context_id[110];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_cAMELInformationSMS_present) {
				_data_len = _edR4_Fin_M_CAMELInformationSMS(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMTRecord.cAMELInformationSMS);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8012, _constructed);
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

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_chChSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMTRecord.chChSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
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

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_systemType_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnSMTRecord.systemType);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[54];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.chargingCharacteristics, 2, 2);
			    if (_data_len < 0) {
				_constructed = TRUE;
				_data_len = -_data_len;
			    }
			    else
				_constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[94];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_localSequenceNumber_present) {
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
			    _element._id = (_Context_Id *)&_context_id[112];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_nodeID_present) {
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
			    _element._id = (_Context_Id *)&_context_id[113];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_recordExtensions_present) {
				_data_len = _ed3R4_Fin_M_ManagementExtensio(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMTRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
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

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_smsResult_present) {
				_data_len = _edR4_Fin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnSMTRecord.smsResult);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[108];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.eventTimeStamp, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[97];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_cellIdentifier_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.cellIdentifier, 2, 2);
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_routingArea_present) {
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
			    _element._id = (_Context_Id *)&_context_id[21];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_locationArea_present) {
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
			    _element._id = (_Context_Id *)&_context_id[22];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_recordingEntity_present) {
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[6];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_serviceCentre_present) {
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
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[23];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_msNetworkCapability_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.msNetworkCapability, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[24];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMTRecord.servedMSISDN, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[10];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_servedIMEI_present) {
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
			    _element._id = (_Context_Id *)&_context_id[1];
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
			    _element._id = (_Context_Id *)&_context_id[13];
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
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8018, _constructed);
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			break;

		    case 6:
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[115];
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
			    _element._id = (_Context_Id *)&_context_id[116];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.causeForRecClosing);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801B, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[117];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_recordExtensions_present) {
				_data_len = _ed3R4_Fin_M_ManagementExtensio(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCTRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801A, _constructed);
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

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_systemType_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.systemType);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8019, _constructed);
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

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_chChSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.chChSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8018, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[119];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.chargingCharacteristics, 2, 2);
			    if (_data_len < 0) {
				_constructed = TRUE;
				_data_len = -_data_len;
			    }
			    else
				_constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[120];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_localSequenceNumber_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.localSequenceNumber);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8016, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[121];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_nodeID_present) {
				_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.nodeID, 20);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
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

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_diagnostics_present) {
				_data_len = _edR4_Fin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCTRecord.diagnostics);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
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

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_lcsCause_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.lcsCause, 2, 1);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[122];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_locationEstimate_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.locationEstimate, 2, 20);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
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

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_routingArea_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.routingArea, 2, 1);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
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

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_location_present) {
				_data_len = _edR4_Fin_M_LocationAreaAndCell(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCTRecord.location);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[123];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_privacyOverride_present) {
				_data_len = 0;
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[124];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_notificationToMSUser_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.notificationToMSUser);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[75];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_measurementDuration_present) {
				_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.measurementDuration);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.eventTimeStamp, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[108];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.mlcNumber, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[97];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_lcsPriority_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.lcsPriority, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_lcsQos_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.lcsQos, 2, 4);
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
			    _element._id = (_Context_Id *)&_context_id[125];
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
				_element._id = (_Context_Id *)&_context_id[126];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_temp->value.u.sgsnLCTRecord.locationType.bit_mask & R4_Fin_M_deferredLocationEventType_present) {
				    _data_len = _oss_encd_pbit(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.locationType.deferredLocationEventType,16);
				    if (_data_len < 0) {
					_constructed = TRUE;
					_data_len = -_data_len;
				    }
				    else
					_constructed = FALSE;
				    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8001, _constructed);
				    _total_len += _data_len;
				}

#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[127];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.locationType.locationEstimateType);
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
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8007, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[128];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_sgsnAddress_present) {
				_data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCTRecord.sgsnAddress);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[6];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.servedMSISDN, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[23];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.servedIMSI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[15];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _edR4_Fin_M_LCSClientIdentity(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCTRecord.lcsClientIdentity);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[25];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.lcsClientType);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[1];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCTRecord.recordingEntity, 2, 20);
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
			    _element._id = (_Context_Id *)&_context_id[13];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCTRecord.recordType);
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
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8019, _constructed);
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			break;

		    case 7:
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[129];
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
			    _element._id = (_Context_Id *)&_context_id[130];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCORecord.causeForRecClosing);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8019, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[131];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_recordExtensions_present) {
				_data_len = _ed3R4_Fin_M_ManagementExtensio(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCORecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8018, _constructed);
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

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_systemType_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCORecord.systemType);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[85];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_chChSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCORecord.chChSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8016, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.chargingCharacteristics, 2, 2);
			    if (_data_len < 0) {
				_constructed = TRUE;
				_data_len = -_data_len;
			    }
			    else
				_constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[132];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_localSequenceNumber_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCORecord.localSequenceNumber);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[133];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_nodeID_present) {
				_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCORecord.nodeID, 20);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[134];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_diagnostics_present) {
				_data_len = _edR4_Fin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCORecord.diagnostics);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8012, _constructed);
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

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsCause_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.lcsCause, 2, 1);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[54];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_locationEstimate_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.locationEstimate, 2, 20);
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
			    _element._id = (_Context_Id *)&_context_id[135];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_routingArea_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.routingArea, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[136];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_location_present) {
				_data_len = _edR4_Fin_M_LocationAreaAndCell(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCORecord.location);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[75];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_measurementDuration_present) {
				_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCORecord.measurementDuration);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.eventTimeStamp, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[108];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_mlcNumber_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.mlcNumber, 2, 9);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800A, _constructed);
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

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsPriority_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.lcsPriority, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsQos_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.lcsQos, 2, 4);
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
			    _element._id = (_Context_Id *)&_context_id[137];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCORecord.locationMethod);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8007, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[128];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_sgsnAddress_present) {
				_data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCORecord.sgsnAddress);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[6];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.servedMSISDN, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[23];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.servedIMSI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[15];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsClientIdentity_present) {
				_data_len = _edR4_Fin_M_LCSClientIdentity(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCORecord.lcsClientIdentity);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[25];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsClientType_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCORecord.lcsClientType);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[1];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCORecord.recordingEntity, 2, 20);
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
			    _element._id = (_Context_Id *)&_context_id[13];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCORecord.recordType);
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
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801A, _constructed);
#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			break;

		    case 8:
#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[138];
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
			    _element._id = (_Context_Id *)&_context_id[130];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCNRecord.causeForRecClosing);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8019, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[131];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_recordExtensions_present) {
				_data_len = _ed3R4_Fin_M_ManagementExtensio(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCNRecord.recordExtensions);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8018, _constructed);
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

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_systemType_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCNRecord.systemType);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[85];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_chChSelectionMode_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCNRecord.chChSelectionMode);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8016, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.chargingCharacteristics, 2, 2);
			    if (_data_len < 0) {
				_constructed = TRUE;
				_data_len = -_data_len;
			    }
			    else
				_constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
			    _total_len += _data_len;
#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[132];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_localSequenceNumber_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCNRecord.localSequenceNumber);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[133];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_nodeID_present) {
				_data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCNRecord.nodeID, 20);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[134];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_diagnostics_present) {
				_data_len = _edR4_Fin_M_Diagnostics(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCNRecord.diagnostics);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8012, _constructed);
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

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsCause_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.lcsCause, 2, 1);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[54];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_locationEstimate_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.locationEstimate, 2, 20);
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
			    _element._id = (_Context_Id *)&_context_id[135];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_routingArea_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.routingArea, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[136];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_location_present) {
				_data_len = _edR4_Fin_M_LocationAreaAndCell(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCNRecord.location);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[75];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_measurementDuration_present) {
				_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCNRecord.measurementDuration);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
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

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.eventTimeStamp, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[108];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_mlcNumber_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.mlcNumber, 2, 9);
				if (_data_len < 0) {
				    _constructed = TRUE;
				    _data_len = -_data_len;
				}
				else
				    _constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800A, _constructed);
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

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsPriority_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.lcsPriority, 2, 1);
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
			    _element._id = (_Context_Id *)&_context_id[61];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsQos_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.lcsQos, 2, 4);
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
			    _element._id = (_Context_Id *)&_context_id[21];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_servedIMEI_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.servedIMEI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[128];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_sgsnAddress_present) {
				_data_len = _edR4_Fin_M_IPAddress(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCNRecord.sgsnAddress);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[6];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_servedMSISDN_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.servedMSISDN, 2, 9);
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
			    _element._id = (_Context_Id *)&_context_id[23];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_servedIMSI_present) {
				_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.servedIMSI, 2, 8);
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
			    _element._id = (_Context_Id *)&_context_id[15];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsClientIdentity_present) {
				_data_len = _edR4_Fin_M_LCSClientIdentity(_g, _out_pos, &_out_len, &_temp->value.u.sgsnLCNRecord.lcsClientIdentity);
				_constructed = TRUE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[25];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsClientType_present) {
				_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCNRecord.lcsClientType);
				_constructed = FALSE;
				_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
				_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
				_total_len += _data_len;
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[1];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_temp->value.u.sgsnLCNRecord.recordingEntity, 2, 20);
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
			    _element._id = (_Context_Id *)&_context_id[13];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _temp->value.u.sgsnLCNRecord.recordType);
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
			_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801B, _constructed);
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
	_element._id = (_Context_Id *)&_context_id[30];
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
	    _element._id = (_Context_Id *)&_context_id[108];
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
	    _element._id = (_Context_Id *)&_context_id[97];
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
	    _element._id = (_Context_Id *)&_context_id[61];
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
	    _element._id = (_Context_Id *)&_context_id[21];
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
	    _element._id = (_Context_Id *)&_context_id[22];
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
	    _element._id = (_Context_Id *)&_context_id[6];
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
	    _element._id = (_Context_Id *)&_context_id[23];
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
	    _element._id = (_Context_Id *)&_context_id[24];
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
	    _element._id = (_Context_Id *)&_context_id[10];
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
	    _element._id = (_Context_Id *)&_context_id[1];
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
	    _element._id = (_Context_Id *)&_context_id[0];
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

static int _eR4_Fin_M_Horizontal_Accuracy(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    R4_Fin_M_Horizontal_Accuracy	*_in_data;
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

    _in_data = (R4_Fin_M_Horizontal_Accuracy *) _inbuf;
    if (!_oss_c->_buffer_provided) *_outbuf = NULL;
    _out_buf = *_outbuf;
    _out_len = _oss_c->_oss_outbuflen = (_oss_c->_buffer_provided) ? *_outlen : 0;
    _out_buf += _out_len;
#if OSSDEBUG > 1
    _oss_c->_oss_context_anchor._pdu_number = 2;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[139];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data, 2, 1);
    if (_data_len < 0) {
	_constructed = TRUE;
	_data_len = -_data_len;
    }
    else
	_constructed = FALSE;
    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x4, _constructed);
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

static int _eR4_Fin_M_ResponseTime(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    R4_Fin_M_ResponseTime	*_in_data;
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

    _in_data = (R4_Fin_M_ResponseTime *) _inbuf;
    if (!_oss_c->_buffer_provided) *_outbuf = NULL;
    _out_buf = *_outbuf;
    _out_len = _oss_c->_oss_outbuflen = (_oss_c->_buffer_provided) ? *_outlen : 0;
    _out_buf += _out_len;
#if OSSDEBUG > 1
    _oss_c->_oss_context_anchor._pdu_number = 3;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[140];
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
	_element._id = (_Context_Id *)&_context_id[127];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->responseTimeCategory);
	_constructed = FALSE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0xA, _constructed);
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

static int _eR4_Fin_M_Vertical_Accuracy(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    R4_Fin_M_Vertical_Accuracy	*_in_data;
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

    _in_data = (R4_Fin_M_Vertical_Accuracy *) _inbuf;
    if (!_oss_c->_buffer_provided) *_outbuf = NULL;
    _out_buf = *_outbuf;
    _out_len = _oss_c->_oss_outbuflen = (_oss_c->_buffer_provided) ? *_outlen : 0;
    _out_buf += _out_len;
#if OSSDEBUG > 1
    _oss_c->_oss_context_anchor._pdu_number = 4;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[141];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data, 2, 1);
    if (_data_len < 0) {
	_constructed = TRUE;
	_data_len = -_data_len;
    }
    else
	_constructed = FALSE;
    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x4, _constructed);
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

static int _emCDRF_R4(struct ossGlobal *_g, int _pdunum, void *_inbuf, char **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH])
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
	    _rc = _eR4_Fin_M_BSRecord(_g, _inbuf, _outbuf, _outlen, _flags);
	    break;

	case 2:
	    _rc = _eR4_Fin_M_Horizontal_Accuracy(_g, _inbuf, _outbuf, _outlen, _flags);
	    break;

	case 3:
	    _rc = _eR4_Fin_M_ResponseTime(_g, _inbuf, _outbuf, _outlen, _flags);
	    break;

	case 4:
	    _rc = _eR4_Fin_M_Vertical_Accuracy(_g, _inbuf, _outbuf, _outlen, _flags);
	    break;

	default:
	    _oss_enc_error(_g, TRUE, _pdu_range, (long) _pdunum);
    }

    _oss_c->_oss_err_msg = NULL;
    return _rc;
}

static void _d_R4_Fin_M_IPBinaryAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_IPBinaryAddress *_out_data)
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

static void _d_0R4_Fin_M_IPTextRepresentedA(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_IPTextRepresentedAddress *_out_data)
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

static void _d_R4_Fin_M_IPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_IPAddress *_out_data)
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
	    _d_R4_Fin_M_IPBinaryAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPBinaryAddress);
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
	    _d_0R4_Fin_M_IPTextRepresentedA(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPTextRepresentedAddress);
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

static void _d_R4_Fin_M_PDPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_PDPAddress *_out_data)
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
		_element._id = (_Context_Id *)&_context_id[4];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 1;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		if (_data_length < 0) ++_indef_tags;
		_d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPAddress);
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
		_element._id = (_Context_Id *)&_context_id[1];
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

static void _d_1R4_Fin_M_ChangeOfCharCondit(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_ChangeOfCharCondition *_out_data)
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
	    _out_data->bit_mask &= ~R4_Fin_M_qosRequested_present;
	else {
	    _out_data->bit_mask |= R4_Fin_M_qosRequested_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[0];
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
	    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->qosRequested, 2, 12, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (!_oss_c->_tag_decoded && (_total_len < 0 || *_in_pos < _end_pos)) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~R4_Fin_M_qosNegotiated_present;
	else {
	    _out_data->bit_mask |= R4_Fin_M_qosNegotiated_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[1];
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
	    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->qosNegotiated, 2, 12, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[9];
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
	_element._id = (_Context_Id *)&_context_id[8];
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
	_element._id = (_Context_Id *)&_context_id[7];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8005) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->changeCondition = (enum R4_Fin_M_ChangeCondition) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[6];
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

static void _d_R4_Fin_M_ManagementExtension(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_ManagementExtension *_out_data)
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
	    _out_data->bit_mask &= ~R4_Fin_M_significance_present;
	    memcpy(&_out_data->significance, &_v0, sizeof(_v0));
	}
	else {
	    _out_data->bit_mask |= R4_Fin_M_significance_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[11];
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
	_element._id = (_Context_Id *)&_context_id[10];
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
	_oss_dec_uoct(_g, _in_pos, _buf_len, _data_length, _oss_c->_buffer_provided, &_out_data->information, 4, -1, _constructed);
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

static void _d_R4_Fin_M_Diagnostics(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_Diagnostics *_out_data)
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
		_element._id = (_Context_Id *)&_context_id[13];
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
		_element._id = (_Context_Id *)&_context_id[14];
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
		_element._id = (_Context_Id *)&_context_id[9];
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
		_element._id = (_Context_Id *)&_context_id[15];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 4;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_d_R4_Fin_M_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.networkSpecificCause);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x4: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[16];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 5;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_d_R4_Fin_M_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.manufacturerSpecificCause);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x5: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[17];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 6;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_out_data->u.positionMethodFailureCause = (enum R4_Fin_M_PositionMethodFailure_Diagnostic) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x6: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[18];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 7;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_out_data->u.unauthorizedLCSClientCause = (enum R4_Fin_M_UnauthorizedLCSClient_Diagnostic) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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

static void _d_R4_Fin_M__seqof1(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M__seqof1 *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	struct R4_Fin_M__seqof1_	*_temp;
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

	unsigned long count = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	_temp = *_out_data;
#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[19];
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
		_temp->next = (struct R4_Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
		_temp = _temp->next;
	    } else {
		_temp = (struct R4_Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
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

static void _d_R4_Fin_M_SequenceList(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_SequenceList *_out_data)
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
	_element._id = (_Context_Id *)&_context_id[4];
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
	if (_data_length < 0) ++_indef_tags;
	_d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->sgsnAddress);
	while (_indef_tags > 0) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _indef_tags--;
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	if (_total_len < 0 || *_in_pos < _end_pos) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~R4_Fin_M_recSequenceNumber_present;
	else {
	    _out_data->bit_mask |= R4_Fin_M_recSequenceNumber_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[20];
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
	    _d_R4_Fin_M__seqof1(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->recSequenceNumber);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (_total_len < 0) {
	    if (!_oss_c->_tag_decoded)
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _oss_c->_tag_decoded = FALSE;
	} else {
	    if (*_in_pos != _end_pos) _oss_dec_error(_g, TRUE, _inconsis_len, 0L);
	}
    }

}

static void _d_R4_Fin_M_7(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, struct R4_Fin_M__seqof1_ **_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	struct R4_Fin_M__seqof1_	*_temp;
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

	unsigned long count = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	_temp = *_out_data;
#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[19];
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
		_temp->next = (struct R4_Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
		_temp = _temp->next;
	    } else {
		_temp = (struct R4_Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
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

static void _d_2R4_Fin_M_LocalSequenceNumbe(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_LocalSequenceNumberList *_out_data)
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
	_element._id = (_Context_Id *)&_context_id[4];
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
	if (_data_length < 0) ++_indef_tags;
	_d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->gsnAddress);
	while (_indef_tags > 0) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _indef_tags--;
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	if (_total_len < 0 || *_in_pos < _end_pos) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~R4_Fin_M_localSeqNumberList_present;
	else {
	    _out_data->bit_mask |= R4_Fin_M_localSeqNumberList_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[20];
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
	    _d_R4_Fin_M_7(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->localSeqNumberList);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (_total_len < 0) {
	    if (!_oss_c->_tag_decoded)
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _oss_c->_tag_decoded = FALSE;
	} else {
	    if (*_in_pos != _end_pos) _oss_dec_error(_g, TRUE, _inconsis_len, 0L);
	}
    }

}

static void _d_R4_Fin_M_CAMELInformationSMS(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_CAMELInformationSMS *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

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
		case 0x1: {
#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[0];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_out_data->bit_mask & R4_Fin_M_CAMELInformationSMS_sCFAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
		    _out_data->bit_mask |= R4_Fin_M_CAMELInformationSMS_sCFAddress_present;
		    _oss_c->_tag_decoded = FALSE;
		    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->sCFAddress, 2, 20, _constructed);
		    _present_flags |= 0x1;
#if OSSDEBUG > 1
		    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

		}   break;

		case 0x2: {
#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[14];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_out_data->bit_mask & R4_Fin_M_CAMELInformationSMS_serviceKey_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
		    _out_data->bit_mask |= R4_Fin_M_CAMELInformationSMS_serviceKey_present;
		    _oss_c->_tag_decoded = FALSE;
		    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		    _out_data->serviceKey = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
		    _present_flags |= 0x2;
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

		    if (_out_data->bit_mask & R4_Fin_M_defaultSMSHandling_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
		    _out_data->bit_mask |= R4_Fin_M_defaultSMSHandling_present;
		    _oss_c->_tag_decoded = FALSE;
		    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		    _out_data->defaultSMSHandling = (enum R4_Fin_M_DefaultSMS_Handling) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
		    _present_flags |= 0x4;
#if OSSDEBUG > 1
		    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

		}   break;

		case 0x4: {
#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[24];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_out_data->bit_mask & R4_Fin_M_cAMELCallingPartyNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
		    _out_data->bit_mask |= R4_Fin_M_cAMELCallingPartyNumber_present;
		    _oss_c->_tag_decoded = FALSE;
		    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->cAMELCallingPartyNumber, 2, 10, _constructed);
		    _present_flags |= 0x8;
#if OSSDEBUG > 1
		    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

		}   break;

		case 0x5: {
#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[23];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_out_data->bit_mask & R4_Fin_M_cAMELDestinationSubscriberNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
		    _out_data->bit_mask |= R4_Fin_M_cAMELDestinationSubscriberNumber_present;
		    _oss_c->_tag_decoded = FALSE;
		    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->cAMELDestinationSubscriberNumber, 2, 12, _constructed);
		    _present_flags |= 0x10;
#if OSSDEBUG > 1
		    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

		}   break;

		case 0x6: {
#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[6];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_out_data->bit_mask & R4_Fin_M_cAMELSMSCAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
		    _out_data->bit_mask |= R4_Fin_M_cAMELSMSCAddress_present;
		    _oss_c->_tag_decoded = FALSE;
		    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->cAMELSMSCAddress, 2, 20, _constructed);
		    _present_flags |= 0x20;
#if OSSDEBUG > 1
		    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

		}   break;

		case 0x7: {
#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[22];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_out_data->bit_mask & R4_Fin_M_CAMELInformationSMS_freeFormatData_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
		    _out_data->bit_mask |= R4_Fin_M_CAMELInformationSMS_freeFormatData_present;
		    _oss_c->_tag_decoded = FALSE;
		    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->freeFormatData, 2, 160, _constructed);
		    _present_flags |= 0x40;
#if OSSDEBUG > 1
		    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

		}   break;

		case 0x8: {
#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[21];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_out_data->bit_mask & R4_Fin_M_smsReferenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
		    _out_data->bit_mask |= R4_Fin_M_smsReferenceNumber_present;
		    _oss_c->_tag_decoded = FALSE;
		    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->smsReferenceNumber, 2, 8, _constructed);
		    _present_flags |= 0x80;
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

	if (!(_present_flags & 0x1)) {
	    _out_data->bit_mask &= ~R4_Fin_M_CAMELInformationSMS_sCFAddress_present;
	}
	if (!(_present_flags & 0x2)) {
	    _out_data->bit_mask &= ~R4_Fin_M_CAMELInformationSMS_serviceKey_present;
	}
	if (!(_present_flags & 0x4)) {
	    _out_data->bit_mask &= ~R4_Fin_M_defaultSMSHandling_present;
	}
	if (!(_present_flags & 0x8)) {
	    _out_data->bit_mask &= ~R4_Fin_M_cAMELCallingPartyNumber_present;
	}
	if (!(_present_flags & 0x10)) {
	    _out_data->bit_mask &= ~R4_Fin_M_cAMELDestinationSubscriberNumber_present;
	}
	if (!(_present_flags & 0x20)) {
	    _out_data->bit_mask &= ~R4_Fin_M_cAMELSMSCAddress_present;
	}
	if (!(_present_flags & 0x40)) {
	    _out_data->bit_mask &= ~R4_Fin_M_CAMELInformationSMS_freeFormatData_present;
	}
	if (!(_present_flags & 0x80)) {
	    _out_data->bit_mask &= ~R4_Fin_M_smsReferenceNumber_present;
	}
    }

}

static void _d_R4_Fin_M_LCSClientExternalID(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_LCSClientExternalID *_out_data)
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
	if (_data_tag != 0x8000 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~R4_Fin_M_externalAddress_present;
	else {
	    _out_data->bit_mask |= R4_Fin_M_externalAddress_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[0];
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
	    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->externalAddress, 2, 20, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (_total_len < 0) {
	    if (!_oss_c->_tag_decoded)
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _oss_c->_tag_decoded = FALSE;
	} else {
	    if (*_in_pos != _end_pos) _oss_dec_error(_g, TRUE, _inconsis_len, 0L);
	}
    }

}

static void _d_R4_Fin_M_LCSClientIdentity(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_LCSClientIdentity *_out_data)
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
	if (_data_tag != 0x8000 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~R4_Fin_M_lcsClientExternalID_present;
	else {
	    _out_data->bit_mask |= R4_Fin_M_lcsClientExternalID_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[26];
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
	    _d_R4_Fin_M_LCSClientExternalID(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->lcsClientExternalID);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (!_oss_c->_tag_decoded && (_total_len < 0 || *_in_pos < _end_pos)) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8001 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~R4_Fin_M_lcsClientDialedByMS_present;
	else {
	    _out_data->bit_mask |= R4_Fin_M_lcsClientDialedByMS_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[1];
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
	    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->lcsClientDialedByMS, 2, 20, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (!_oss_c->_tag_decoded && (_total_len < 0 || *_in_pos < _end_pos)) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~R4_Fin_M_lcsClientInternalID_present;
	else {
	    _out_data->bit_mask |= R4_Fin_M_lcsClientInternalID_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[25];
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
	    _out_data->lcsClientInternalID = (enum R4_Fin_M_LCSClientInternalID) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (_total_len < 0) {
	    if (!_oss_c->_tag_decoded)
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
	    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
	    _oss_c->_tag_decoded = FALSE;
	} else {
	    if (*_in_pos != _end_pos) _oss_dec_error(_g, TRUE, _inconsis_len, 0L);
	}
    }

}

static void _d_R4_Fin_M_LocationAreaAndCell(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_LocationAreaAndCell *_out_data)
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
	_element._id = (_Context_Id *)&_context_id[0];
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
	_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->locationAreaCode, 2, 2, _constructed);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[1];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->cellId, 2, 2, _constructed);
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

static void _d_3R4_Fin_M_ManagementExtensio(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, R4_Fin_M_ManagementExtensions *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	struct R4_Fin_M_ManagementExtensions_	*_temp;
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
	    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
	    if (_temp) {
		_temp->next = (struct R4_Fin_M_ManagementExtensions_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M_ManagementExtensions_)*1, _oss_c->_buffer_provided);
		_temp = _temp->next;
	    } else {
		_temp = (struct R4_Fin_M_ManagementExtensions_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M_ManagementExtensions_)*1, _oss_c->_buffer_provided);
		*_out_data = _temp;
	    }
	    _temp->next = NULL;
#if OSSDEBUG > 1
	    _element._occurrence = count;
#endif

	    _oss_c->_tag_decoded = FALSE;
	    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	    _d_R4_Fin_M_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value);
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

    }
}

static int _dR4_Fin_M_BSRecord(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    R4_Fin_M_BSRecord	*_out_data;
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
    _out_data = (struct R4_Fin_M_BSRecord *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M_BSRecord)*1, _oss_c->_buffer_provided);
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
	_element._id = (_Context_Id *)&_context_id[30];
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
			_element._id = (_Context_Id *)&_context_id[0];
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
			_element._id = (_Context_Id *)&_context_id[1];
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
			_element._id = (_Context_Id *)&_context_id[10];
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
			_element._id = (_Context_Id *)&_context_id[24];
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
			_element._id = (_Context_Id *)&_context_id[23];
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
			_element._id = (_Context_Id *)&_context_id[6];
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
			_element._id = (_Context_Id *)&_context_id[22];
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
			_element._id = (_Context_Id *)&_context_id[21];
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
			_element._id = (_Context_Id *)&_context_id[61];
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
			_element._id = (_Context_Id *)&_context_id[97];
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
			_element._id = (_Context_Id *)&_context_id[108];
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
	_element._id = (_Context_Id *)&_context_id[20];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	{
	    struct R4_Fin_M__seqof8	*_temp;
	    long _total_len = _data_length;
	    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

	    unsigned long count = 0;

#if OSSDEBUG > 1
	    _Context_Element _element;

	    _element._occurrence = 0;
#endif

	    _temp = _out_data->listOfCDR;
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
		if (_temp) {
		    _temp->next = (struct R4_Fin_M__seqof8 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof8)*1, _oss_c->_buffer_provided);
		    _temp = _temp->next;
		} else {
		    _temp = (struct R4_Fin_M__seqof8 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof8)*1, _oss_c->_buffer_provided);
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
			case 0x14: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[30];
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
				unsigned char _present_flags[5];
#if OSSDEBUG > 1
				_Context_Element _element;
#endif

				memset(_present_flags, 0, 5);
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
					    _element._id = (_Context_Id *)&_context_id[13];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[0] |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[11];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_networkInitiation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_networkInitiation_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.networkInitiation = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[0] |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[10];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.servedIMSI, 2, 8, _constructed);
					    _present_flags[0] |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x4: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[24];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_servedIMEI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.servedIMEI, 2, 8, _constructed);
					    _present_flags[0] |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[62];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_sgsnAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_sgsnAddress_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct R4_Fin_M__seqof2_	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnPDPRecord.sgsnAddress;
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
						    if (_temp1) {
							_temp1->next = (struct R4_Fin_M__seqof2_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof2_)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct R4_Fin_M__seqof2_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof2_)*1, _oss_c->_buffer_provided);
							_temp->value.u.sgsnPDPRecord.sgsnAddress = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
						}
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }
					    _present_flags[0] |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[6];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_msNetworkCapability_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.msNetworkCapability, 2, 8, _constructed);
					    _present_flags[0] |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x7: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[22];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_routingArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.routingArea, 2, 1, _constructed);
					    _present_flags[0] |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[21];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_locationAreaCode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_locationAreaCode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.locationAreaCode, 2, 2, _constructed);
					    _present_flags[0] |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x9: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_cellIdentifier_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_cellIdentifier_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.cellIdentifier, 2, 2, _constructed);
					    _present_flags[1] |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[60];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.chargingID = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[1] |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[59];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnPDPRecord.ggsnAddressUsed);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
					    _present_flags[1] |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xC: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[58];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_accessPointNameNI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_accessPointNameNI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnPDPRecord.accessPointNameNI, 63, _constructed);
					    _present_flags[1] |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xD: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[57];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_pdpType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_pdpType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.pdpType, 2, 2, _constructed);
					    _present_flags[1] |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[56];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_servedPDPAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_servedPDPAddress_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnPDPRecord.servedPDPAddress);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
					    _present_flags[1] |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xF: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[55];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_listOfTrafficVolumes_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_listOfTrafficVolumes_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct R4_Fin_M__seqof3_	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnPDPRecord.listOfTrafficVolumes;
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
						    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
						    if (_temp1) {
							_temp1->next = (struct R4_Fin_M__seqof3_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof3_)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct R4_Fin_M__seqof3_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof3_)*1, _oss_c->_buffer_provided);
							_temp->value.u.sgsnPDPRecord.listOfTrafficVolumes = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _oss_c->_tag_decoded = FALSE;
						    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						    _d_1R4_Fin_M_ChangeOfCharCondit(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
						}
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }
					    _present_flags[1] |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x10: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[54];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.recordOpeningTime, 2, 9, _constructed);
					    _present_flags[1] |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[53];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[2] |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x12: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[52];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_sgsnChange_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_sgsnChange_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.sgsnChange = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[2] |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x13: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[51];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[2] |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x14: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[50];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_diagnostics_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnPDPRecord.diagnostics);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
					    _present_flags[2] |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x15: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[49];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_recSequenceNumList_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_recSequenceNumList_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct R4_Fin_M__seqof4	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnPDPRecord.recSequenceNumList;
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
						    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
						    if (_temp1) {
							_temp1->next = (struct R4_Fin_M__seqof4 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof4)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct R4_Fin_M__seqof4 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof4)*1, _oss_c->_buffer_provided);
							_temp->value.u.sgsnPDPRecord.recSequenceNumList = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _oss_c->_tag_decoded = FALSE;
						    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						    _d_R4_Fin_M_SequenceList(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
						}
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }
					    _present_flags[2] |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x16: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[48];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnPDPRecord.nodeID, 20, _constructed);
					    _present_flags[2] |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x17: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[47];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_3R4_Fin_M_ManagementExtensio(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnPDPRecord.recordExtensions);
					    _present_flags[2] |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x18: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[46];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_localSequenceNumberList_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_localSequenceNumberList_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct R4_Fin_M__seqof5	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnPDPRecord.localSequenceNumberList;
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
						    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
						    if (_temp1) {
							_temp1->next = (struct R4_Fin_M__seqof5 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof5)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct R4_Fin_M__seqof5 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof5)*1, _oss_c->_buffer_provided);
							_temp->value.u.sgsnPDPRecord.localSequenceNumberList = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _oss_c->_tag_decoded = FALSE;
						    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						    _d_2R4_Fin_M_LocalSequenceNumbe(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
						}
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }
					    _present_flags[2] |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x19: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[45];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_apnSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_apnSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.apnSelectionMode = (enum R4_Fin_M_APNSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[3] |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1A: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[44];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_accessPointNameOI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_accessPointNameOI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnPDPRecord.accessPointNameOI, 37, _constructed);
					    _present_flags[3] |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1B: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[43];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.servedMSISDN, 2, 9, _constructed);
					    _present_flags[3] |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1C: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[42];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.chargingCharacteristics, 2, 2, _constructed);
					    _present_flags[3] |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1D: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[41];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_systemType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.systemType = (enum R4_Fin_M_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[3] |= 0x10;
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

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_cAMELInformationPDP_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_cAMELInformationPDP_present;
					    _oss_c->_tag_decoded = FALSE;
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
							case 0x1: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[0];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_sCFAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= R4_Fin_M_CAMELInformationPDP_sCFAddress_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.sCFAddress, 2, 20, _constructed);
							    _present_flags |= 0x1;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x2: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[14];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_serviceKey_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= R4_Fin_M_CAMELInformationPDP_serviceKey_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.serviceKey = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
							    _present_flags |= 0x2;
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

							    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_defaultTransactionHandling_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= R4_Fin_M_CAMELInformationPDP_defaultTransactionHandling_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.defaultTransactionHandling = (enum R4_Fin_M_DefaultGPRS_Handling) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
							    _present_flags |= 0x4;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x4: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[40];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_cAMELAccessPointNameNI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= R4_Fin_M_cAMELAccessPointNameNI_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.cAMELAccessPointNameNI, 63, _constructed);
							    _present_flags |= 0x8;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x5: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[39];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_cAMELAccessPointNameOI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= R4_Fin_M_cAMELAccessPointNameOI_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.cAMELAccessPointNameOI, 37, _constructed);
							    _present_flags |= 0x10;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x6: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[38];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_numberOfDPEncountered_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= R4_Fin_M_CAMELInformationPDP_numberOfDPEncountered_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.numberOfDPEncountered = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
							    _present_flags |= 0x20;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x7: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[37];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_levelOfCAMELService_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= R4_Fin_M_CAMELInformationPDP_levelOfCAMELService_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _oss_dec_pbit(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.levelOfCAMELService, 3, _constructed, 1);
							    _present_flags |= 0x40;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x8: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[21];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_freeFormatData_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= R4_Fin_M_CAMELInformationPDP_freeFormatData_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.freeFormatData, 2, 160, _constructed);
							    _present_flags |= 0x80;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x9: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[36];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_fFDAppendIndicator_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= R4_Fin_M_CAMELInformationPDP_fFDAppendIndicator_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.fFDAppendIndicator = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
							    _present_flags |= 0x100;
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

						if (!(_present_flags & 0x1)) {
						    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~R4_Fin_M_CAMELInformationPDP_sCFAddress_present;
						}
						if (!(_present_flags & 0x2)) {
						    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~R4_Fin_M_CAMELInformationPDP_serviceKey_present;
						}
						if (!(_present_flags & 0x4)) {
						    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~R4_Fin_M_CAMELInformationPDP_defaultTransactionHandling_present;
						}
						if (!(_present_flags & 0x8)) {
						    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~R4_Fin_M_cAMELAccessPointNameNI_present;
						}
						if (!(_present_flags & 0x10)) {
						    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~R4_Fin_M_cAMELAccessPointNameOI_present;
						}
						if (!(_present_flags & 0x20)) {
						    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~R4_Fin_M_CAMELInformationPDP_numberOfDPEncountered_present;
						}
						if (!(_present_flags & 0x40)) {
						    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~R4_Fin_M_CAMELInformationPDP_levelOfCAMELService_present;
						}
						if (!(_present_flags & 0x80)) {
						    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~R4_Fin_M_CAMELInformationPDP_freeFormatData_present;
						}
						if (!(_present_flags & 0x100)) {
						    _temp->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~R4_Fin_M_CAMELInformationPDP_fFDAppendIndicator_present;
						}
					    }

					    _present_flags[3] |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1F: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[34];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_rNCUnsentDownlinkVolumeList_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_rNCUnsentDownlinkVolumeList_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct R4_Fin_M__seqof6	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnPDPRecord.rNCUnsentDownlinkVolumeList;
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
						    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
						    if (_temp1) {
							_temp1->next = (struct R4_Fin_M__seqof6 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof6)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct R4_Fin_M__seqof6 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof6)*1, _oss_c->_buffer_provided);
							_temp->value.u.sgsnPDPRecord.rNCUnsentDownlinkVolumeList = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _oss_c->_tag_decoded = FALSE;
						    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						    {
							short _indef_tags = 0;
							long _total_len = _data_length;
							char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

#if OSSDEBUG > 1
							_Context_Element _element;

							_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
							_element._id = (_Context_Id *)&_context_id[4];
							_element._parent = _oss_c->_oss_context_anchor._last;
							_oss_c->_oss_context_anchor._last = &_element;
#endif

							_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
							if (_data_tag != 0x8001) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

							_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							if (_data_length < 0) ++_indef_tags;
							_d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value.sgsnAddress);
							while (_indef_tags > 0) {
							    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
							    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
							    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
							    _indef_tags--;
							}
#if OSSDEBUG > 1
							_oss_c->_oss_context_anchor._last = _element._parent;
#endif

							if (_total_len < 0 || *_in_pos < _end_pos) {
							    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
							    _oss_c->_tag_decoded = TRUE;
							}
							if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
							    _temp1->value.bit_mask &= ~R4_Fin_M_rNCUnsentDnVolumeList_present;
							else {
							    _temp1->value.bit_mask |= R4_Fin_M_rNCUnsentDnVolumeList_present;
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[20];
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
							    {
								struct R4_Fin_M__seqof1_	*_temp2;
								long _total_len = _data_length;
								char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

								unsigned long count = 0;

#if OSSDEBUG > 1
								_Context_Element _element;

								_element._occurrence = 0;
#endif

								_temp2 = _temp1->value.rNCUnsentDnVolumeList;
#if OSSDEBUG > 1
								_element._id = (_Context_Id *)&_context_id[19];
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
								    if (_temp2) {
									_temp2->next = (struct R4_Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
									_temp2 = _temp2->next;
								    } else {
									_temp2 = (struct R4_Fin_M__seqof1_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof1_)*1, _oss_c->_buffer_provided);
									_temp1->value.rNCUnsentDnVolumeList = _temp2;
								    }
								    _temp2->next = NULL;
#if OSSDEBUG > 1
								    _element._occurrence = count;
#endif

								    _oss_c->_tag_decoded = FALSE;
								    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
								    _temp2->value = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
								}
#if OSSDEBUG > 1
								_oss_c->_oss_context_anchor._last = _element._parent;
#endif

							    }
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}

							if (_total_len < 0) {
							    if (!_oss_c->_tag_decoded)
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
					    _present_flags[3] |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x20: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[33];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_chChSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_chChSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.chChSelectionMode = (enum R4_Fin_M_ChChSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[3] |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x21: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[32];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_dynamicAddressFlag_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnPDPRecord.bit_mask |= R4_Fin_M_SGSNPDPRecord_dynamicAddressFlag_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.dynamicAddressFlag = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[4] |= 0x1;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x22: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[31];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnPDPRecord.consolidationResult = (enum R4_Fin_M_ConsolidationResult) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags[4] |= 0x2;
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

				if (!(_present_flags[0] & 0x2)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_networkInitiation_present;
				}
				if (!(_present_flags[0] & 0x8)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_servedIMEI_present;
				}
				if (!(_present_flags[0] & 0x10)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_sgsnAddress_present;
				}
				if (!(_present_flags[0] & 0x20)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_msNetworkCapability_present;
				}
				if (!(_present_flags[0] & 0x40)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_routingArea_present;
				}
				if (!(_present_flags[0] & 0x80)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_locationAreaCode_present;
				}
				if (!(_present_flags[1] & 0x1)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_cellIdentifier_present;
				}
				if ((_present_flags[0] & 0x5) != 0x5) _oss_dec_error(_g, TRUE, _field_omit, 0L);
				if (!(_present_flags[1] & 0x8)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_accessPointNameNI_present;
				}
				if (!(_present_flags[1] & 0x10)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_pdpType_present;
				}
				if (!(_present_flags[1] & 0x20)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_servedPDPAddress_present;
				}
				if (!(_present_flags[1] & 0x40)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_listOfTrafficVolumes_present;
				}
				if ((_present_flags[1] & 0x86) != 0x86) _oss_dec_error(_g, TRUE, _field_omit, 0L);
				if (!(_present_flags[2] & 0x2)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_sgsnChange_present;
				}
				if (!(_present_flags[2] & 0x8)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_diagnostics_present;
				}
				if (!(_present_flags[2] & 0x10)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_recSequenceNumList_present;
				}
				if (!(_present_flags[2] & 0x20)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_nodeID_present;
				}
				if (!(_present_flags[2] & 0x40)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_recordExtensions_present;
				}
				if (!(_present_flags[2] & 0x80)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_localSequenceNumberList_present;
				}
				if (!(_present_flags[3] & 0x1)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_apnSelectionMode_present;
				}
				if (!(_present_flags[3] & 0x2)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_accessPointNameOI_present;
				}
				if (!(_present_flags[3] & 0x4)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_servedMSISDN_present;
				}
				if ((_present_flags[2] & 0x5) != 0x5) _oss_dec_error(_g, TRUE, _field_omit, 0L);
				if (!(_present_flags[3] & 0x10)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_systemType_present;
				}
				if (!(_present_flags[3] & 0x20)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_cAMELInformationPDP_present;
				}
				if (!(_present_flags[3] & 0x40)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_rNCUnsentDownlinkVolumeList_present;
				}
				if (!(_present_flags[3] & 0x80)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_chChSelectionMode_present;
				}
				if (!(_present_flags[4] & 0x1)) {
				    _temp->value.u.sgsnPDPRecord.bit_mask &= ~R4_Fin_M_SGSNPDPRecord_dynamicAddressFlag_present;
				}
				if ((_present_flags[3] & 0x8) != 0x8) _oss_dec_error(_g, TRUE, _field_omit, 0L);
				if ((_present_flags[4] & 0x2) != 0x2) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x15: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[63];
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
					    _element._id = (_Context_Id *)&_context_id[13];
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
					    _element._id = (_Context_Id *)&_context_id[11];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_networkInitiation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_networkInitiation_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.networkInitiation = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[10];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.servedIMSI, 2, 8, _constructed);
					    _present_flags |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x4: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[83];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.ggsnAddress);
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

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[82];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.chargingID = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[81];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct R4_Fin_M__seqof2_	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.ggsnPDPRecord.sgsnAddress;
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
						    if (_temp1) {
							_temp1->next = (struct R4_Fin_M__seqof2_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof2_)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct R4_Fin_M__seqof2_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof2_)*1, _oss_c->_buffer_provided);
							_temp->value.u.ggsnPDPRecord.sgsnAddress = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
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

					case 0x7: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[80];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_accessPointNameNI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_accessPointNameNI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.ggsnPDPRecord.accessPointNameNI, 63, _constructed);
					    _present_flags |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[21];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_pdpType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_pdpType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.pdpType, 2, 2, _constructed);
					    _present_flags |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x9: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[79];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_servedPDPAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_servedPDPAddress_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.servedPDPAddress);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
					    _present_flags |= 0x100;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[78];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_dynamicAddressFlag_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_dynamicAddressFlag_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.dynamicAddressFlag = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xC: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[77];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_listOfTrafficVolumes_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_listOfTrafficVolumes_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct R4_Fin_M__seqof3_	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.ggsnPDPRecord.listOfTrafficVolumes;
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
						    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
						    if (_temp1) {
							_temp1->next = (struct R4_Fin_M__seqof3_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof3_)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct R4_Fin_M__seqof3_ *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof3_)*1, _oss_c->_buffer_provided);
							_temp->value.u.ggsnPDPRecord.listOfTrafficVolumes = _temp1;
						    }
						    _temp1->next = NULL;
#if OSSDEBUG > 1
						    _element._occurrence = count;
#endif

						    _oss_c->_tag_decoded = FALSE;
						    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						    _d_1R4_Fin_M_ChangeOfCharCondit(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp1->value);
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

					case 0xD: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[76];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.recordOpeningTime, 2, 9, _constructed);
					    _present_flags |= 0x800;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[75];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xF: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[74];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x2000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x10: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[73];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_diagnostics_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.diagnostics);
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

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[72];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_recSequenceNumList_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_recSequenceNumList_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_R4_Fin_M_SequenceList(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.recSequenceNumList);
					    _present_flags |= 0x8000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x12: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[71];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.ggsnPDPRecord.nodeID, 20, _constructed);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x13: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[70];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_3R4_Fin_M_ManagementExtensio(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.recordExtensions);
					    _present_flags |= 0x20000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x14: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[69];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_localSequenceNumberList_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_localSequenceNumberList_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_2R4_Fin_M_LocalSequenceNumbe(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.ggsnPDPRecord.localSequenceNumberList);
					    _present_flags |= 0x40000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x15: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[68];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_apnSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_apnSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.apnSelectionMode = (enum R4_Fin_M_APNSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x80000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x16: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[67];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.servedMSISDN, 2, 9, _constructed);
					    _present_flags |= 0x100000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x17: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[66];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.chargingCharacteristics, 2, 2, _constructed);
					    _present_flags |= 0x200000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x18: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[65];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_chChSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_GGSNPDPRecord_chChSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.chChSelectionMode = (enum R4_Fin_M_ChChSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x400000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1B: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[64];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_sgsnPLMNIdentifier_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.ggsnPDPRecord.bit_mask |= R4_Fin_M_sgsnPLMNIdentifier_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.ggsnPDPRecord.sgsnPLMNIdentifier, 2, 3, _constructed);
					    _present_flags |= 0x800000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1E: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[45];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.ggsnPDPRecord.consolidationResult = (enum R4_Fin_M_ConsolidationResult) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1000000;
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
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_networkInitiation_present;
				}
				if (!(_present_flags & 0x40)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_accessPointNameNI_present;
				}
				if (!(_present_flags & 0x80)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_pdpType_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_servedPDPAddress_present;
				}
				if (!(_present_flags & 0x200)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_dynamicAddressFlag_present;
				}
				if (!(_present_flags & 0x400)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_listOfTrafficVolumes_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_diagnostics_present;
				}
				if (!(_present_flags & 0x8000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_recSequenceNumList_present;
				}
				if (!(_present_flags & 0x10000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_nodeID_present;
				}
				if (!(_present_flags & 0x20000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_recordExtensions_present;
				}
				if (!(_present_flags & 0x40000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_localSequenceNumberList_present;
				}
				if (!(_present_flags & 0x80000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_apnSelectionMode_present;
				}
				if (!(_present_flags & 0x100000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_servedMSISDN_present;
				}
				if (!(_present_flags & 0x400000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_GGSNPDPRecord_chChSelectionMode_present;
				}
				if (!(_present_flags & 0x800000)) {
				    _temp->value.u.ggsnPDPRecord.bit_mask &= ~R4_Fin_M_sgsnPLMNIdentifier_present;
				}
				if ((_present_flags & 0x120383D) != 0x120383D) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x16: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[84];
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
					    _element._id = (_Context_Id *)&_context_id[13];
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
					    _element._id = (_Context_Id *)&_context_id[1];
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
					    _element._id = (_Context_Id *)&_context_id[10];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_servedIMEI_present;
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
					    _element._id = (_Context_Id *)&_context_id[83];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_sgsnAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_sgsnAddress_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnMMRecord.sgsnAddress);
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
					    _element._id = (_Context_Id *)&_context_id[23];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_msNetworkCapability_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.msNetworkCapability, 2, 8, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[6];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_routingArea_present;
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
					    _element._id = (_Context_Id *)&_context_id[22];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_locationAreaCode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_locationAreaCode_present;
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
					    _element._id = (_Context_Id *)&_context_id[21];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_cellIdentifier_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_cellIdentifier_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.cellIdentifier, 2, 2, _constructed);
					    _present_flags |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[98];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_changeLocation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_changeLocation_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    {
						struct R4_Fin_M__seqof7	*_temp1;
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

						unsigned long count = 0;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

						_temp1 = _temp->value.u.sgsnMMRecord.changeLocation;
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
						    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
						    if (_temp1) {
							_temp1->next = (struct R4_Fin_M__seqof7 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof7)*1, _oss_c->_buffer_provided);
							_temp1 = _temp1->next;
						    } else {
							_temp1 = (struct R4_Fin_M__seqof7 *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M__seqof7)*1, _oss_c->_buffer_provided);
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
							_element._id = (_Context_Id *)&_context_id[0];
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
							_element._id = (_Context_Id *)&_context_id[1];
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
							    _temp1->value.bit_mask &= ~R4_Fin_M_cellId_present;
							else {
							    _temp1->value.bit_mask |= R4_Fin_M_cellId_present;
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[10];
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
							_element._id = (_Context_Id *)&_context_id[24];
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
					    _element._id = (_Context_Id *)&_context_id[97];
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
					    _element._id = (_Context_Id *)&_context_id[96];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_duration_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_duration_present;
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
					    _element._id = (_Context_Id *)&_context_id[95];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_sgsnChange_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_sgsnChange_present;
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
					    _element._id = (_Context_Id *)&_context_id[75];
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
					    _element._id = (_Context_Id *)&_context_id[56];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_diagnostics_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnMMRecord.diagnostics);
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
					    _element._id = (_Context_Id *)&_context_id[94];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_recordSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_recordSequenceNumber_present;
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
					    _element._id = (_Context_Id *)&_context_id[93];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_nodeID_present;
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
					    _element._id = (_Context_Id *)&_context_id[92];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_3R4_Fin_M_ManagementExtensio(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnMMRecord.recordExtensions);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[91];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_localSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnMMRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x20000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x12: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[90];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.servedMSISDN, 2, 9, _constructed);
					    _present_flags |= 0x40000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x13: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[89];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.chargingCharacteristics, 2, 2, _constructed);
					    _present_flags |= 0x80000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x14: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[86];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_cAMELInformationMM_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_cAMELInformationMM_present;
					    _oss_c->_tag_decoded = FALSE;
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
							case 0x1: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[0];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_sCFAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask |= R4_Fin_M_CAMELInformationMM_sCFAddress_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.cAMELInformationMM.sCFAddress, 2, 20, _constructed);
							    _present_flags |= 0x1;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x2: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[14];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_serviceKey_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask |= R4_Fin_M_CAMELInformationMM_serviceKey_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.serviceKey = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
							    _present_flags |= 0x2;
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

							    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_defaultTransactionHandling_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask |= R4_Fin_M_CAMELInformationMM_defaultTransactionHandling_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.defaultTransactionHandling = (enum R4_Fin_M_DefaultGPRS_Handling) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
							    _present_flags |= 0x4;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x4: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[8];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_numberOfDPEncountered_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask |= R4_Fin_M_CAMELInformationMM_numberOfDPEncountered_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.numberOfDPEncountered = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
							    _present_flags |= 0x8;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x5: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[88];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_levelOfCAMELService_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask |= R4_Fin_M_CAMELInformationMM_levelOfCAMELService_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _oss_dec_pbit(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.cAMELInformationMM.levelOfCAMELService, 3, _constructed, 1);
							    _present_flags |= 0x10;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x6: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[6];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_freeFormatData_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask |= R4_Fin_M_CAMELInformationMM_freeFormatData_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnMMRecord.cAMELInformationMM.freeFormatData, 2, 160, _constructed);
							    _present_flags |= 0x20;
#if OSSDEBUG > 1
							    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

							}   break;

							case 0x7: {
#if OSSDEBUG > 1
							    _element._id = (_Context_Id *)&_context_id[87];
							    _element._parent = _oss_c->_oss_context_anchor._last;
							    _oss_c->_oss_context_anchor._last = &_element;
#endif

							    if (_temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_fFDAppendIndicator_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask |= R4_Fin_M_CAMELInformationMM_fFDAppendIndicator_present;
							    _oss_c->_tag_decoded = FALSE;
							    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
							    _temp->value.u.sgsnMMRecord.cAMELInformationMM.fFDAppendIndicator = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
							    _present_flags |= 0x40;
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

						if (!(_present_flags & 0x1)) {
						    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~R4_Fin_M_CAMELInformationMM_sCFAddress_present;
						}
						if (!(_present_flags & 0x2)) {
						    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~R4_Fin_M_CAMELInformationMM_serviceKey_present;
						}
						if (!(_present_flags & 0x4)) {
						    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~R4_Fin_M_CAMELInformationMM_defaultTransactionHandling_present;
						}
						if (!(_present_flags & 0x8)) {
						    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~R4_Fin_M_CAMELInformationMM_numberOfDPEncountered_present;
						}
						if (!(_present_flags & 0x10)) {
						    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~R4_Fin_M_CAMELInformationMM_levelOfCAMELService_present;
						}
						if (!(_present_flags & 0x20)) {
						    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~R4_Fin_M_CAMELInformationMM_freeFormatData_present;
						}
						if (!(_present_flags & 0x40)) {
						    _temp->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~R4_Fin_M_CAMELInformationMM_fFDAppendIndicator_present;
						}
					    }

					    _present_flags |= 0x100000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x15: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[85];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_systemType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnMMRecord.systemType = (enum R4_Fin_M_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x200000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x16: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[65];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_chChSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnMMRecord.bit_mask |= R4_Fin_M_SGSNMMRecord_chChSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnMMRecord.chChSelectionMode = (enum R4_Fin_M_ChChSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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

				if (!(_present_flags & 0x4)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_servedIMEI_present;
				}
				if (!(_present_flags & 0x8)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_sgsnAddress_present;
				}
				if (!(_present_flags & 0x10)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_msNetworkCapability_present;
				}
				if (!(_present_flags & 0x20)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_routingArea_present;
				}
				if (!(_present_flags & 0x40)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_locationAreaCode_present;
				}
				if (!(_present_flags & 0x80)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_cellIdentifier_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_changeLocation_present;
				}
				if (!(_present_flags & 0x400)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_duration_present;
				}
				if (!(_present_flags & 0x800)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_sgsnChange_present;
				}
				if (!(_present_flags & 0x2000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_diagnostics_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_recordSequenceNumber_present;
				}
				if (!(_present_flags & 0x8000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_nodeID_present;
				}
				if (!(_present_flags & 0x10000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_recordExtensions_present;
				}
				if (!(_present_flags & 0x20000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_localSequenceNumber_present;
				}
				if (!(_present_flags & 0x40000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_servedMSISDN_present;
				}
				if (!(_present_flags & 0x100000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_cAMELInformationMM_present;
				}
				if (!(_present_flags & 0x200000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_systemType_present;
				}
				if (!(_present_flags & 0x400000)) {
				    _temp->value.u.sgsnMMRecord.bit_mask &= ~R4_Fin_M_SGSNMMRecord_chChSelectionMode_present;
				}
				if ((_present_flags & 0x81203) != 0x81203) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x17: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[99];
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
					    _element._id = (_Context_Id *)&_context_id[13];
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
					    _element._id = (_Context_Id *)&_context_id[1];
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
					    _element._id = (_Context_Id *)&_context_id[10];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_servedIMEI_present;
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
					    _element._id = (_Context_Id *)&_context_id[24];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.servedMSISDN, 2, 9, _constructed);
					    _present_flags |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x4: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[23];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_msNetworkCapability_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.msNetworkCapability, 2, 8, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[6];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_serviceCentre_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_serviceCentre_present;
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
					    _element._id = (_Context_Id *)&_context_id[22];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_recordingEntity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_recordingEntity_present;
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
					    _element._id = (_Context_Id *)&_context_id[21];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_locationArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_locationArea_present;
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
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_routingArea_present;
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
					    _element._id = (_Context_Id *)&_context_id[97];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_cellIdentifier_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_cellIdentifier_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.cellIdentifier, 2, 2, _constructed);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[108];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_uoct(_g, _in_pos, _buf_len, _data_length, _oss_c->_buffer_provided, &_temp->value.u.sgsnSMORecord.messageReference, 4, -1, _constructed);
					    _present_flags |= 0x400;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[76];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.eventTimeStamp, 2, 9, _constructed);
					    _present_flags |= 0x800;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xC: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[107];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_smsResult_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_smsResult_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMORecord.smsResult);
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
					    _element._id = (_Context_Id *)&_context_id[106];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_3R4_Fin_M_ManagementExtensio(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMORecord.recordExtensions);
					    _present_flags |= 0x2000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[105];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_nodeID_present;
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
					    _element._id = (_Context_Id *)&_context_id[104];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_localSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMORecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x8000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x10: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[103];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.chargingCharacteristics, 2, 2, _constructed);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[102];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_systemType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMORecord.systemType = (enum R4_Fin_M_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x20000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x12: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[90];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_destinationNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_destinationNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMORecord.destinationNumber, 2, 12, _constructed);
					    _present_flags |= 0x40000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x13: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[101];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_cAMELInformationSMS_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_cAMELInformationSMS_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_R4_Fin_M_CAMELInformationSMS(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMORecord.cAMELInformationSMS);
					    _present_flags |= 0x80000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x14: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[100];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_chChSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMORecord.bit_mask |= R4_Fin_M_SGSNSMORecord_chChSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMORecord.chChSelectionMode = (enum R4_Fin_M_ChChSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x100000;
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
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_servedIMEI_present;
				}
				if (!(_present_flags & 0x8)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_servedMSISDN_present;
				}
				if (!(_present_flags & 0x10)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_msNetworkCapability_present;
				}
				if (!(_present_flags & 0x20)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_serviceCentre_present;
				}
				if (!(_present_flags & 0x40)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_recordingEntity_present;
				}
				if (!(_present_flags & 0x80)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_locationArea_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_routingArea_present;
				}
				if (!(_present_flags & 0x200)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_cellIdentifier_present;
				}
				if (!(_present_flags & 0x1000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_smsResult_present;
				}
				if (!(_present_flags & 0x2000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_recordExtensions_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_nodeID_present;
				}
				if (!(_present_flags & 0x8000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_localSequenceNumber_present;
				}
				if (!(_present_flags & 0x20000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_systemType_present;
				}
				if (!(_present_flags & 0x40000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_destinationNumber_present;
				}
				if (!(_present_flags & 0x80000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_cAMELInformationSMS_present;
				}
				if (!(_present_flags & 0x100000)) {
				    _temp->value.u.sgsnSMORecord.bit_mask &= ~R4_Fin_M_SGSNSMORecord_chChSelectionMode_present;
				}
				if ((_present_flags & 0x10C03) != 0x10C03) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x18: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[109];
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
					    _element._id = (_Context_Id *)&_context_id[13];
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
					    _element._id = (_Context_Id *)&_context_id[1];
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
					    _element._id = (_Context_Id *)&_context_id[10];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_servedIMEI_present;
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
					    _element._id = (_Context_Id *)&_context_id[24];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.servedMSISDN, 2, 9, _constructed);
					    _present_flags |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x4: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[23];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_msNetworkCapability_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.msNetworkCapability, 2, 8, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[6];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_serviceCentre_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_serviceCentre_present;
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
					    _element._id = (_Context_Id *)&_context_id[22];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_recordingEntity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_recordingEntity_present;
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
					    _element._id = (_Context_Id *)&_context_id[21];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_locationArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_locationArea_present;
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
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_routingArea_present;
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
					    _element._id = (_Context_Id *)&_context_id[97];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_cellIdentifier_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_cellIdentifier_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.cellIdentifier, 2, 2, _constructed);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[108];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.eventTimeStamp, 2, 9, _constructed);
					    _present_flags |= 0x400;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[114];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_smsResult_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_smsResult_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMTRecord.smsResult);
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
					    _element._id = (_Context_Id *)&_context_id[113];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_3R4_Fin_M_ManagementExtensio(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMTRecord.recordExtensions);
					    _present_flags |= 0x1000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xD: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[112];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_nodeID_present;
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
					    _element._id = (_Context_Id *)&_context_id[94];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_localSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMTRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x4000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xF: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[54];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnSMTRecord.chargingCharacteristics, 2, 2, _constructed);
					    _present_flags |= 0x8000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x10: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[111];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_systemType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMTRecord.systemType = (enum R4_Fin_M_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[102];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_chChSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_chChSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnSMTRecord.chChSelectionMode = (enum R4_Fin_M_ChChSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x20000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x12: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[110];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_cAMELInformationSMS_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnSMTRecord.bit_mask |= R4_Fin_M_SGSNSMTRecord_cAMELInformationSMS_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_R4_Fin_M_CAMELInformationSMS(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnSMTRecord.cAMELInformationSMS);
					    _present_flags |= 0x40000;
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
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_servedIMEI_present;
				}
				if (!(_present_flags & 0x8)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_servedMSISDN_present;
				}
				if (!(_present_flags & 0x10)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_msNetworkCapability_present;
				}
				if (!(_present_flags & 0x20)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_serviceCentre_present;
				}
				if (!(_present_flags & 0x40)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_recordingEntity_present;
				}
				if (!(_present_flags & 0x80)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_locationArea_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_routingArea_present;
				}
				if (!(_present_flags & 0x200)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_cellIdentifier_present;
				}
				if (!(_present_flags & 0x800)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_smsResult_present;
				}
				if (!(_present_flags & 0x1000)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_recordExtensions_present;
				}
				if (!(_present_flags & 0x2000)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_nodeID_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_localSequenceNumber_present;
				}
				if (!(_present_flags & 0x10000)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_systemType_present;
				}
				if (!(_present_flags & 0x20000)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_chChSelectionMode_present;
				}
				if (!(_present_flags & 0x40000)) {
				    _temp->value.u.sgsnSMTRecord.bit_mask &= ~R4_Fin_M_SGSNSMTRecord_cAMELInformationSMS_present;
				}
				if ((_present_flags & 0x8403) != 0x8403) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x19: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[115];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _temp->value.choice = 6;
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
					    _element._id = (_Context_Id *)&_context_id[13];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCTRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1;
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

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.recordingEntity, 2, 20, _constructed);
					    _present_flags |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x2: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[25];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCTRecord.lcsClientType = (enum R4_Fin_M_LCSClientType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[15];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_R4_Fin_M_LCSClientIdentity(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCTRecord.lcsClientIdentity);
					    _present_flags |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x4: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[23];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.servedIMSI, 2, 8, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[6];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.servedMSISDN, 2, 9, _constructed);
					    _present_flags |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[128];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_sgsnAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_sgsnAddress_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCTRecord.sgsnAddress);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
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
					    {
						long _total_len = _data_length;
						char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;

#if OSSDEBUG > 1
						_Context_Element _element;

						_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[127];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0x8000) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_temp->value.u.sgsnLCTRecord.locationType.locationEstimateType = (enum R4_Fin_M_LocationEstimateType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

						if (_total_len < 0 || *_in_pos < _end_pos) {
						    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						    _oss_c->_tag_decoded = TRUE;
						}
						if (_data_tag != 0x8001 || (_total_len >= 0 && *_in_pos >= _end_pos))
						    _temp->value.u.sgsnLCTRecord.locationType.bit_mask &= ~R4_Fin_M_deferredLocationEventType_present;
						else {
						    _temp->value.u.sgsnLCTRecord.locationType.bit_mask |= R4_Fin_M_deferredLocationEventType_present;
#if OSSDEBUG > 1
						    _element._id = (_Context_Id *)&_context_id[126];
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
						    _oss_dec_pbit(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.locationType.deferredLocationEventType, 16, _constructed, 1);
#if OSSDEBUG > 1
						    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

						}

						if (_total_len < 0) {
						    if (!_oss_c->_tag_decoded)
							_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						    _oss_c->_tag_decoded = FALSE;
						} else {
						    if (*_in_pos != _end_pos) _oss_dec_error(_g, TRUE, _inconsis_len, 0L);
						}
					    }

					    _present_flags |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_lcsQos_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_lcsQos_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.lcsQos, 2, 4, _constructed);
					    _present_flags |= 0x100;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x9: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[97];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_lcsPriority_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_lcsPriority_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.lcsPriority, 2, 1, _constructed);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[108];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.mlcNumber, 2, 9, _constructed);
					    _present_flags |= 0x400;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[76];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.eventTimeStamp, 2, 9, _constructed);
					    _present_flags |= 0x800;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xC: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[75];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_measurementDuration_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_measurementDuration_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCTRecord.measurementDuration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xD: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[124];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_notificationToMSUser_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_notificationToMSUser_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCTRecord.notificationToMSUser = (enum R4_Fin_M_NotificationToMSUser) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x2000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[123];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_privacyOverride_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_privacyOverride_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCTRecord.privacyOverride = 0;
					    if (_data_length != 0) _oss_dec_error(_g, TRUE, _inval_enc, 0L);
					    _present_flags |= 0x4000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xF: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[72];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_location_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_location_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_R4_Fin_M_LocationAreaAndCell(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCTRecord.location);
					    _present_flags |= 0x8000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x10: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[103];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_routingArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.routingArea, 2, 1, _constructed);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[122];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_locationEstimate_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_locationEstimate_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.locationEstimate, 2, 20, _constructed);
					    _present_flags |= 0x20000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x13: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[90];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_lcsCause_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_lcsCause_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.lcsCause, 2, 1, _constructed);
					    _present_flags |= 0x40000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x14: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[50];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_diagnostics_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCTRecord.diagnostics);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
					    _present_flags |= 0x80000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x15: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[121];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnLCTRecord.nodeID, 20, _constructed);
					    _present_flags |= 0x100000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x16: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[120];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_localSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCTRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x200000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x17: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[119];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCTRecord.chargingCharacteristics, 2, 2, _constructed);
					    _present_flags |= 0x400000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x18: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[118];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_chChSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_chChSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCTRecord.chChSelectionMode = (enum R4_Fin_M_ChChSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x800000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x19: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[45];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_systemType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCTRecord.systemType = (enum R4_Fin_M_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1000000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1A: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[117];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCTRecord.bit_mask |= R4_Fin_M_SGSNLCTRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_3R4_Fin_M_ManagementExtensio(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCTRecord.recordExtensions);
					    _present_flags |= 0x2000000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x1B: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[116];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCTRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
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

				if (!(_present_flags & 0x20)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_servedMSISDN_present;
				}
				if (!(_present_flags & 0x40)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_sgsnAddress_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_lcsQos_present;
				}
				if (!(_present_flags & 0x200)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_lcsPriority_present;
				}
				if (!(_present_flags & 0x1000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_measurementDuration_present;
				}
				if (!(_present_flags & 0x2000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_notificationToMSUser_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_privacyOverride_present;
				}
				if (!(_present_flags & 0x8000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_location_present;
				}
				if (!(_present_flags & 0x10000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_routingArea_present;
				}
				if (!(_present_flags & 0x20000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_locationEstimate_present;
				}
				if (!(_present_flags & 0x40000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_lcsCause_present;
				}
				if (!(_present_flags & 0x80000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_diagnostics_present;
				}
				if (!(_present_flags & 0x100000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_nodeID_present;
				}
				if (!(_present_flags & 0x200000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_localSequenceNumber_present;
				}
				if (!(_present_flags & 0x800000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_chChSelectionMode_present;
				}
				if (!(_present_flags & 0x1000000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_systemType_present;
				}
				if (!(_present_flags & 0x2000000)) {
				    _temp->value.u.sgsnLCTRecord.bit_mask &= ~R4_Fin_M_SGSNLCTRecord_recordExtensions_present;
				}
				if ((_present_flags & 0x4400C9F) != 0x4400C9F) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x1A: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[129];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _temp->value.choice = 7;
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
					    _element._id = (_Context_Id *)&_context_id[13];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCORecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1;
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

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.recordingEntity, 2, 20, _constructed);
					    _present_flags |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x2: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[25];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsClientType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_lcsClientType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCORecord.lcsClientType = (enum R4_Fin_M_LCSClientType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[15];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsClientIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_lcsClientIdentity_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_R4_Fin_M_LCSClientIdentity(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCORecord.lcsClientIdentity);
					    _present_flags |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x4: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[23];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.servedIMSI, 2, 8, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[6];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.servedMSISDN, 2, 9, _constructed);
					    _present_flags |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[128];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_sgsnAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_sgsnAddress_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCORecord.sgsnAddress);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
					    _present_flags |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x7: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[137];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCORecord.locationMethod = (enum R4_Fin_M_LocationMethod) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsQos_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_lcsQos_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.lcsQos, 2, 4, _constructed);
					    _present_flags |= 0x100;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x9: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[97];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsPriority_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_lcsPriority_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.lcsPriority, 2, 1, _constructed);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[108];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_mlcNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_mlcNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.mlcNumber, 2, 9, _constructed);
					    _present_flags |= 0x400;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[76];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.eventTimeStamp, 2, 9, _constructed);
					    _present_flags |= 0x800;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xC: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[75];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_measurementDuration_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_measurementDuration_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCORecord.measurementDuration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xD: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[136];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_location_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_location_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_R4_Fin_M_LocationAreaAndCell(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCORecord.location);
					    _present_flags |= 0x2000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[135];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_routingArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.routingArea, 2, 1, _constructed);
					    _present_flags |= 0x4000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xF: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[54];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_locationEstimate_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_locationEstimate_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.locationEstimate, 2, 20, _constructed);
					    _present_flags |= 0x8000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[103];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsCause_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_lcsCause_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.lcsCause, 2, 1, _constructed);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x12: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[134];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_diagnostics_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCORecord.diagnostics);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
					    _present_flags |= 0x20000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x13: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[133];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnLCORecord.nodeID, 20, _constructed);
					    _present_flags |= 0x40000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x14: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[132];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_localSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCORecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x80000;
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

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCORecord.chargingCharacteristics, 2, 2, _constructed);
					    _present_flags |= 0x100000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x16: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[85];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_chChSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_chChSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCORecord.chChSelectionMode = (enum R4_Fin_M_ChChSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x200000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x17: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[65];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_systemType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCORecord.systemType = (enum R4_Fin_M_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x400000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x18: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[131];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCORecord.bit_mask |= R4_Fin_M_SGSNLCORecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_3R4_Fin_M_ManagementExtensio(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCORecord.recordExtensions);
					    _present_flags |= 0x800000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x19: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[130];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCORecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1000000;
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
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_lcsClientType_present;
				}
				if (!(_present_flags & 0x8)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_lcsClientIdentity_present;
				}
				if (!(_present_flags & 0x20)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_servedMSISDN_present;
				}
				if (!(_present_flags & 0x40)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_sgsnAddress_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_lcsQos_present;
				}
				if (!(_present_flags & 0x200)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_lcsPriority_present;
				}
				if (!(_present_flags & 0x400)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_mlcNumber_present;
				}
				if (!(_present_flags & 0x1000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_measurementDuration_present;
				}
				if (!(_present_flags & 0x2000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_location_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_routingArea_present;
				}
				if (!(_present_flags & 0x8000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_locationEstimate_present;
				}
				if (!(_present_flags & 0x10000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_lcsCause_present;
				}
				if (!(_present_flags & 0x20000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_diagnostics_present;
				}
				if (!(_present_flags & 0x40000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_nodeID_present;
				}
				if (!(_present_flags & 0x80000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_localSequenceNumber_present;
				}
				if (!(_present_flags & 0x200000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_chChSelectionMode_present;
				}
				if (!(_present_flags & 0x400000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_systemType_present;
				}
				if (!(_present_flags & 0x800000)) {
				    _temp->value.u.sgsnLCORecord.bit_mask &= ~R4_Fin_M_SGSNLCORecord_recordExtensions_present;
				}
				if ((_present_flags & 0x1100893) != 0x1100893) _oss_dec_error(_g, TRUE, _field_omit, 0L);
			    }

#if OSSDEBUG > 1
			    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

			}   break;

			case 0x1B: {
#if OSSDEBUG > 1
			    _element._id = (_Context_Id *)&_context_id[138];
			    _element._parent = _oss_c->_oss_context_anchor._last;
			    _oss_c->_oss_context_anchor._last = &_element;
#endif

			    _temp->value.choice = 8;
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
					    _element._id = (_Context_Id *)&_context_id[13];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCNRecord.recordType = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1;
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

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.recordingEntity, 2, 20, _constructed);
					    _present_flags |= 0x2;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x2: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[25];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsClientType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_lcsClientType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCNRecord.lcsClientType = (enum R4_Fin_M_LCSClientType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x4;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x3: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[15];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsClientIdentity_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_lcsClientIdentity_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_R4_Fin_M_LCSClientIdentity(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCNRecord.lcsClientIdentity);
					    _present_flags |= 0x8;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x4: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[23];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_servedIMSI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_servedIMSI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.servedIMSI, 2, 8, _constructed);
					    _present_flags |= 0x10;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x5: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[6];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_servedMSISDN_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.servedMSISDN, 2, 9, _constructed);
					    _present_flags |= 0x20;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x6: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[128];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_sgsnAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_sgsnAddress_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCNRecord.sgsnAddress);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
					    _present_flags |= 0x40;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x7: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[21];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_servedIMEI_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.servedIMEI, 2, 8, _constructed);
					    _present_flags |= 0x80;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x8: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[61];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsQos_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_lcsQos_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.lcsQos, 2, 4, _constructed);
					    _present_flags |= 0x100;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x9: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[97];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsPriority_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_lcsPriority_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.lcsPriority, 2, 1, _constructed);
					    _present_flags |= 0x200;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xA: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[108];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_mlcNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_mlcNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.mlcNumber, 2, 9, _constructed);
					    _present_flags |= 0x400;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xB: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[76];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.eventTimeStamp, 2, 9, _constructed);
					    _present_flags |= 0x800;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xC: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[75];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_measurementDuration_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_measurementDuration_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCNRecord.measurementDuration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xD: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[136];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_location_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_location_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_R4_Fin_M_LocationAreaAndCell(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCNRecord.location);
					    _present_flags |= 0x2000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xE: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[135];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_routingArea_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.routingArea, 2, 1, _constructed);
					    _present_flags |= 0x4000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0xF: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[54];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_locationEstimate_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_locationEstimate_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.locationEstimate, 2, 20, _constructed);
					    _present_flags |= 0x8000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x11: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[103];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsCause_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_lcsCause_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.lcsCause, 2, 1, _constructed);
					    _present_flags |= 0x10000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x12: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[134];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_diagnostics_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    if (_data_length < 0) ++_indef_tags;
					    _d_R4_Fin_M_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCNRecord.diagnostics);
					    while (_indef_tags > 0) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
						if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
						_indef_tags--;
					    }
					    _present_flags |= 0x20000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x13: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[133];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_nodeID_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _temp->value.u.sgsnLCNRecord.nodeID, 20, _constructed);
					    _present_flags |= 0x40000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x14: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[132];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_localSequenceNumber_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCNRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x80000;
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

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_temp->value.u.sgsnLCNRecord.chargingCharacteristics, 2, 2, _constructed);
					    _present_flags |= 0x100000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x16: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[85];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_chChSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_chChSelectionMode_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCNRecord.chChSelectionMode = (enum R4_Fin_M_ChChSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x200000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x17: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[65];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_systemType_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCNRecord.systemType = (enum R4_Fin_M_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x400000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x18: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[131];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    if (_temp->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
					    _temp->value.u.sgsnLCNRecord.bit_mask |= R4_Fin_M_SGSNLCNRecord_recordExtensions_present;
					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _d_3R4_Fin_M_ManagementExtensio(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_temp->value.u.sgsnLCNRecord.recordExtensions);
					    _present_flags |= 0x800000;
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					}   break;

					case 0x19: {
#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[130];
					    _element._parent = _oss_c->_oss_context_anchor._last;
					    _oss_c->_oss_context_anchor._last = &_element;
#endif

					    _oss_c->_tag_decoded = FALSE;
					    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					    _temp->value.u.sgsnLCNRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
					    _present_flags |= 0x1000000;
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
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_lcsClientType_present;
				}
				if (!(_present_flags & 0x8)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_lcsClientIdentity_present;
				}
				if (!(_present_flags & 0x10)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_servedIMSI_present;
				}
				if (!(_present_flags & 0x20)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_servedMSISDN_present;
				}
				if (!(_present_flags & 0x40)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_sgsnAddress_present;
				}
				if (!(_present_flags & 0x80)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_servedIMEI_present;
				}
				if (!(_present_flags & 0x100)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_lcsQos_present;
				}
				if (!(_present_flags & 0x200)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_lcsPriority_present;
				}
				if (!(_present_flags & 0x400)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_mlcNumber_present;
				}
				if (!(_present_flags & 0x1000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_measurementDuration_present;
				}
				if (!(_present_flags & 0x2000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_location_present;
				}
				if (!(_present_flags & 0x4000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_routingArea_present;
				}
				if (!(_present_flags & 0x8000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_locationEstimate_present;
				}
				if (!(_present_flags & 0x10000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_lcsCause_present;
				}
				if (!(_present_flags & 0x20000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_diagnostics_present;
				}
				if (!(_present_flags & 0x40000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_nodeID_present;
				}
				if (!(_present_flags & 0x80000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_localSequenceNumber_present;
				}
				if (!(_present_flags & 0x200000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_chChSelectionMode_present;
				}
				if (!(_present_flags & 0x400000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_systemType_present;
				}
				if (!(_present_flags & 0x800000)) {
				    _temp->value.u.sgsnLCNRecord.bit_mask &= ~R4_Fin_M_SGSNLCNRecord_recordExtensions_present;
				}
				if ((_present_flags & 0x1100803) != 0x1100803) _oss_dec_error(_g, TRUE, _field_omit, 0L);
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

static int _dR4_Fin_M_Horizontal_Accuracy(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    R4_Fin_M_Horizontal_Accuracy	*_out_data;
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
    _oss_c->_oss_context_anchor._pdu_number = 2;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[139];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    _out_data = (struct R4_Fin_M_Horizontal_Accuracy *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M_Horizontal_Accuracy)*1, _oss_c->_buffer_provided);
    _oss_c->_tag_decoded = FALSE;
    _data_length = *_buf_len;
    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
    if (_data_tag != 0x4) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, _out_data, 2, 1, _constructed);
    *_outbuf = _out_data;
    if (_oss_c->_buffer_provided) *_outlen -= _oss_c->_oss_outbuflen;
    else if (_outlen != NULL) *_outlen = _oss_c->_oss_out_alloc;
    if (!_oss_c->_buffer_provided) _oss_releaseMem(_g, &_oss_c->_oss_mem);
    return(PDU_DECODED);
}

static int _dR4_Fin_M_ResponseTime(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    R4_Fin_M_ResponseTime	*_out_data;
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
    _oss_c->_oss_context_anchor._pdu_number = 3;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[140];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    _out_data = (struct R4_Fin_M_ResponseTime *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M_ResponseTime)*1, _oss_c->_buffer_provided);
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
	_element._id = (_Context_Id *)&_context_id[127];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0xA) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->responseTimeCategory = (enum R4_Fin_M_ResponseTimeCategory) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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

static int _dR4_Fin_M_Vertical_Accuracy(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    R4_Fin_M_Vertical_Accuracy	*_out_data;
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
    _oss_c->_oss_context_anchor._pdu_number = 4;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[141];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    _out_data = (struct R4_Fin_M_Vertical_Accuracy *) _oss_dec_getmem(_g, sizeof(struct R4_Fin_M_Vertical_Accuracy)*1, _oss_c->_buffer_provided);
    _oss_c->_tag_decoded = FALSE;
    _data_length = *_buf_len;
    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
    if (_data_tag != 0x4) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, _out_data, 2, 1, _constructed);
    *_outbuf = _out_data;
    if (_oss_c->_buffer_provided) *_outlen -= _oss_c->_oss_outbuflen;
    else if (_outlen != NULL) *_outlen = _oss_c->_oss_out_alloc;
    if (!_oss_c->_buffer_provided) _oss_releaseMem(_g, &_oss_c->_oss_mem);
    return(PDU_DECODED);
}

static int _dmCDRF_R4(struct ossGlobal *_g, int *_pdunum, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH])
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

    switch (*_pdunum) {
	case 1:
	    _rc = _dR4_Fin_M_BSRecord(_g, _inbuf, _inlen, _outbuf, _outlen);
	    break;

	case 2:
	    _rc = _dR4_Fin_M_Horizontal_Accuracy(_g, _inbuf, _inlen, _outbuf, _outlen);
	    break;

	case 3:
	    _rc = _dR4_Fin_M_ResponseTime(_g, _inbuf, _inlen, _outbuf, _outlen);
	    break;

	case 4:
	    _rc = _dR4_Fin_M_Vertical_Accuracy(_g, _inbuf, _inlen, _outbuf, _outlen);
	    break;

	default:
	    _oss_dec_error(_g, TRUE, _pdu_range, (long)*_pdunum);
    }
    _oss_c->_oss_err_msg = NULL;
    return _rc;
}

static int _f_R4_Fin_M_IPBinaryAddress(struct ossGlobal *_g, R4_Fin_M_IPBinaryAddress *_data_ptr)
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

static int _f_0R4_Fin_M_IPTextRepresentedA(struct ossGlobal *_g, R4_Fin_M_IPTextRepresentedAddress *_data_ptr)
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

static int _f_R4_Fin_M_IPAddress(struct ossGlobal *_g, R4_Fin_M_IPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	_f_R4_Fin_M_IPBinaryAddress(_g, &_data_ptr->u.iPBinaryAddress);
	break;

    case 2:
	_f_0R4_Fin_M_IPTextRepresentedA(_g, &_data_ptr->u.iPTextRepresentedAddress);
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_R4_Fin_M_PDPAddress(struct ossGlobal *_g, R4_Fin_M_PDPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	_f_R4_Fin_M_IPAddress(_g, &_data_ptr->u.iPAddress);
	break;

    case 2:
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_1R4_Fin_M_ChangeOfCharCondit(struct ossGlobal *_g, R4_Fin_M_ChangeOfCharCondition *_data_ptr)
{
    if (_data_ptr->bit_mask & R4_Fin_M_qosRequested_present) {
    }

    if (_data_ptr->bit_mask & R4_Fin_M_qosNegotiated_present) {
    }

    return(PDU_FREED);
}

static int _f_R4_Fin_M_ManagementExtension(struct ossGlobal *_g, R4_Fin_M_ManagementExtension *_data_ptr)
{
    if (_data_ptr->identifier.value)
	(*_g->freep)(_data_ptr->identifier.value);
    if (_data_ptr->bit_mask & R4_Fin_M_significance_present) {
    }

    if (_data_ptr->information.value)
	(*_g->freep)(_data_ptr->information.value);
    return(PDU_FREED);
}

static int _f_R4_Fin_M_Diagnostics(struct ossGlobal *_g, R4_Fin_M_Diagnostics *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	break;

    case 2:
	break;

    case 3:
	break;

    case 4:
	_f_R4_Fin_M_ManagementExtension(_g, &_data_ptr->u.networkSpecificCause);
	break;

    case 5:
	_f_R4_Fin_M_ManagementExtension(_g, &_data_ptr->u.manufacturerSpecificCause);
	break;

    case 6:
	break;

    case 7:
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f_R4_Fin_M__seqof1(struct ossGlobal *_g, R4_Fin_M__seqof1 *_data_ptr)
{
    if (*_data_ptr) {
	struct R4_Fin_M__seqof1_	*_tempa = *_data_ptr;
	struct R4_Fin_M__seqof1_	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    (*_g->freep)(_tempa);
	    _tempa = _tempb;
	}
    }

    return(PDU_FREED);
}

static int _f_R4_Fin_M_SequenceList(struct ossGlobal *_g, R4_Fin_M_SequenceList *_data_ptr)
{
    _f_R4_Fin_M_IPAddress(_g, &_data_ptr->sgsnAddress);
    if (_data_ptr->bit_mask & R4_Fin_M_recSequenceNumber_present) {
	_f_R4_Fin_M__seqof1(_g, &_data_ptr->recSequenceNumber);
    }

    return(PDU_FREED);
}

static int _f_R4_Fin_M_7(struct ossGlobal *_g, struct R4_Fin_M__seqof1_ **_data_ptr)
{
    if (*_data_ptr) {
	struct R4_Fin_M__seqof1_	*_tempa = *_data_ptr;
	struct R4_Fin_M__seqof1_	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    (*_g->freep)(_tempa);
	    _tempa = _tempb;
	}
    }

    return(PDU_FREED);
}

static int _f_2R4_Fin_M_LocalSequenceNumbe(struct ossGlobal *_g, R4_Fin_M_LocalSequenceNumberList *_data_ptr)
{
    _f_R4_Fin_M_IPAddress(_g, &_data_ptr->gsnAddress);
    if (_data_ptr->bit_mask & R4_Fin_M_localSeqNumberList_present) {
	_f_R4_Fin_M_7(_g, &_data_ptr->localSeqNumberList);
    }

    return(PDU_FREED);
}

static int _f_R4_Fin_M_CAMELInformationSMS(struct ossGlobal *_g, R4_Fin_M_CAMELInformationSMS *_data_ptr)
{
    if (_data_ptr->bit_mask & R4_Fin_M_CAMELInformationSMS_sCFAddress_present) {
    }

    if (_data_ptr->bit_mask & R4_Fin_M_CAMELInformationSMS_serviceKey_present) {
    }

    if (_data_ptr->bit_mask & R4_Fin_M_defaultSMSHandling_present) {
    }

    if (_data_ptr->bit_mask & R4_Fin_M_cAMELCallingPartyNumber_present) {
    }

    if (_data_ptr->bit_mask & R4_Fin_M_cAMELDestinationSubscriberNumber_present) {
    }

    if (_data_ptr->bit_mask & R4_Fin_M_cAMELSMSCAddress_present) {
    }

    if (_data_ptr->bit_mask & R4_Fin_M_CAMELInformationSMS_freeFormatData_present) {
    }

    if (_data_ptr->bit_mask & R4_Fin_M_smsReferenceNumber_present) {
    }

    return(PDU_FREED);
}

static int _f_R4_Fin_M_LCSClientExternalID(struct ossGlobal *_g, R4_Fin_M_LCSClientExternalID *_data_ptr)
{
    if (_data_ptr->bit_mask & R4_Fin_M_externalAddress_present) {
    }

    return(PDU_FREED);
}

static int _f_R4_Fin_M_LCSClientIdentity(struct ossGlobal *_g, R4_Fin_M_LCSClientIdentity *_data_ptr)
{
    if (_data_ptr->bit_mask & R4_Fin_M_lcsClientExternalID_present) {
	_f_R4_Fin_M_LCSClientExternalID(_g, &_data_ptr->lcsClientExternalID);
    }

    if (_data_ptr->bit_mask & R4_Fin_M_lcsClientDialedByMS_present) {
    }

    if (_data_ptr->bit_mask & R4_Fin_M_lcsClientInternalID_present) {
    }

    return(PDU_FREED);
}

static int _f_R4_Fin_M_LocationAreaAndCell(struct ossGlobal *_g, R4_Fin_M_LocationAreaAndCell *_data_ptr)
{
    return(PDU_FREED);
}

static int _f_3R4_Fin_M_ManagementExtensio(struct ossGlobal *_g, R4_Fin_M_ManagementExtensions *_data_ptr)
{
    if (*_data_ptr) {
	struct R4_Fin_M_ManagementExtensions_	*_tempa = *_data_ptr;
	struct R4_Fin_M_ManagementExtensions_	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    _f_R4_Fin_M_ManagementExtension(_g, &_tempa->value);
	    (*_g->freep)(_tempa);
	    _tempa = _tempb;
	}
    }

    return(PDU_FREED);
}

static int _fR4_Fin_M_BSRecord(struct ossGlobal *_g, void *_data)
{
    R4_Fin_M_BSRecord	*_data_ptr;
    _data_ptr = (R4_Fin_M_BSRecord *) _data;
    if (_data_ptr->listOfCDR) {
	struct R4_Fin_M__seqof8	*_tempa = _data_ptr->listOfCDR;
	struct R4_Fin_M__seqof8	*_tempb;
	while (_tempa) {
	    _tempb = _tempa->next;
	    switch (_tempa->value.choice) {
	    case 1:
		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_networkInitiation_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_servedIMEI_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_sgsnAddress_present) {
		    if (_tempa->value.u.sgsnPDPRecord.sgsnAddress) {
			struct R4_Fin_M__seqof2_	*_temp1a = _tempa->value.u.sgsnPDPRecord.sgsnAddress;
			struct R4_Fin_M__seqof2_	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    _f_R4_Fin_M_IPAddress(_g, &_temp1a->value);
			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_msNetworkCapability_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_locationAreaCode_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_cellIdentifier_present) {
		}

		_f_R4_Fin_M_IPAddress(_g, &_tempa->value.u.sgsnPDPRecord.ggsnAddressUsed);
		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_accessPointNameNI_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_pdpType_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_servedPDPAddress_present) {
		    _f_R4_Fin_M_PDPAddress(_g, &_tempa->value.u.sgsnPDPRecord.servedPDPAddress);
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_listOfTrafficVolumes_present) {
		    if (_tempa->value.u.sgsnPDPRecord.listOfTrafficVolumes) {
			struct R4_Fin_M__seqof3_	*_temp1a = _tempa->value.u.sgsnPDPRecord.listOfTrafficVolumes;
			struct R4_Fin_M__seqof3_	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    _f_1R4_Fin_M_ChangeOfCharCondit(_g, &_temp1a->value);
			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_sgsnChange_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_diagnostics_present) {
		    _f_R4_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnPDPRecord.diagnostics);
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_recSequenceNumList_present) {
		    if (_tempa->value.u.sgsnPDPRecord.recSequenceNumList) {
			struct R4_Fin_M__seqof4	*_temp1a = _tempa->value.u.sgsnPDPRecord.recSequenceNumList;
			struct R4_Fin_M__seqof4	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    _f_R4_Fin_M_SequenceList(_g, &_temp1a->value);
			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_recordExtensions_present) {
		    _f_3R4_Fin_M_ManagementExtensio(_g, &_tempa->value.u.sgsnPDPRecord.recordExtensions);
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_localSequenceNumberList_present) {
		    if (_tempa->value.u.sgsnPDPRecord.localSequenceNumberList) {
			struct R4_Fin_M__seqof5	*_temp1a = _tempa->value.u.sgsnPDPRecord.localSequenceNumberList;
			struct R4_Fin_M__seqof5	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    _f_2R4_Fin_M_LocalSequenceNumbe(_g, &_temp1a->value);
			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_apnSelectionMode_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_accessPointNameOI_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_systemType_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_cAMELInformationPDP_present) {
		    if (_tempa->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_sCFAddress_present) {
		    }

		    if (_tempa->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_serviceKey_present) {
		    }

		    if (_tempa->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_defaultTransactionHandling_present) {
		    }

		    if (_tempa->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_cAMELAccessPointNameNI_present) {
		    }

		    if (_tempa->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_cAMELAccessPointNameOI_present) {
		    }

		    if (_tempa->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_numberOfDPEncountered_present) {
		    }

		    if (_tempa->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_levelOfCAMELService_present) {
		    }

		    if (_tempa->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_freeFormatData_present) {
		    }

		    if (_tempa->value.u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & R4_Fin_M_CAMELInformationPDP_fFDAppendIndicator_present) {
		    }

		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_rNCUnsentDownlinkVolumeList_present) {
		    if (_tempa->value.u.sgsnPDPRecord.rNCUnsentDownlinkVolumeList) {
			struct R4_Fin_M__seqof6	*_temp1a = _tempa->value.u.sgsnPDPRecord.rNCUnsentDownlinkVolumeList;
			struct R4_Fin_M__seqof6	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    _f_R4_Fin_M_IPAddress(_g, &_temp1a->value.sgsnAddress);
			    if (_temp1a->value.bit_mask & R4_Fin_M_rNCUnsentDnVolumeList_present) {
				if (_temp1a->value.rNCUnsentDnVolumeList) {
				    struct R4_Fin_M__seqof1_	*_temp2a = _temp1a->value.rNCUnsentDnVolumeList;
				    struct R4_Fin_M__seqof1_	*_temp2b;
				    while (_temp2a) {
					_temp2b = _temp2a->next;
					(*_g->freep)(_temp2a);
					_temp2a = _temp2b;
				    }
				}

			    }

			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_chChSelectionMode_present) {
		}

		if (_tempa->value.u.sgsnPDPRecord.bit_mask & R4_Fin_M_SGSNPDPRecord_dynamicAddressFlag_present) {
		}

		break;

	    case 2:
		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_networkInitiation_present) {
		}

		_f_R4_Fin_M_IPAddress(_g, &_tempa->value.u.ggsnPDPRecord.ggsnAddress);
		if (_tempa->value.u.ggsnPDPRecord.sgsnAddress) {
		    struct R4_Fin_M__seqof2_	*_temp1a = _tempa->value.u.ggsnPDPRecord.sgsnAddress;
		    struct R4_Fin_M__seqof2_	*_temp1b;
		    while (_temp1a) {
			_temp1b = _temp1a->next;
			_f_R4_Fin_M_IPAddress(_g, &_temp1a->value);
			(*_g->freep)(_temp1a);
			_temp1a = _temp1b;
		    }
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_accessPointNameNI_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_pdpType_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_servedPDPAddress_present) {
		    _f_R4_Fin_M_PDPAddress(_g, &_tempa->value.u.ggsnPDPRecord.servedPDPAddress);
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_dynamicAddressFlag_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_listOfTrafficVolumes_present) {
		    if (_tempa->value.u.ggsnPDPRecord.listOfTrafficVolumes) {
			struct R4_Fin_M__seqof3_	*_temp1a = _tempa->value.u.ggsnPDPRecord.listOfTrafficVolumes;
			struct R4_Fin_M__seqof3_	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    _f_1R4_Fin_M_ChangeOfCharCondit(_g, &_temp1a->value);
			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_diagnostics_present) {
		    _f_R4_Fin_M_Diagnostics(_g, &_tempa->value.u.ggsnPDPRecord.diagnostics);
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_recSequenceNumList_present) {
		    _f_R4_Fin_M_SequenceList(_g, &_tempa->value.u.ggsnPDPRecord.recSequenceNumList);
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_nodeID_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_recordExtensions_present) {
		    _f_3R4_Fin_M_ManagementExtensio(_g, &_tempa->value.u.ggsnPDPRecord.recordExtensions);
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_localSequenceNumberList_present) {
		    _f_2R4_Fin_M_LocalSequenceNumbe(_g, &_tempa->value.u.ggsnPDPRecord.localSequenceNumberList);
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_apnSelectionMode_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_GGSNPDPRecord_chChSelectionMode_present) {
		}

		if (_tempa->value.u.ggsnPDPRecord.bit_mask & R4_Fin_M_sgsnPLMNIdentifier_present) {
		}

		break;

	    case 3:
		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_servedIMEI_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_sgsnAddress_present) {
		    _f_R4_Fin_M_IPAddress(_g, &_tempa->value.u.sgsnMMRecord.sgsnAddress);
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_msNetworkCapability_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_locationAreaCode_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_cellIdentifier_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_changeLocation_present) {
		    if (_tempa->value.u.sgsnMMRecord.changeLocation) {
			struct R4_Fin_M__seqof7	*_temp1a = _tempa->value.u.sgsnMMRecord.changeLocation;
			struct R4_Fin_M__seqof7	*_temp1b;
			while (_temp1a) {
			    _temp1b = _temp1a->next;
			    if (_temp1a->value.bit_mask & R4_Fin_M_cellId_present) {
			    }

			    (*_g->freep)(_temp1a);
			    _temp1a = _temp1b;
			}
		    }

		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_duration_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_sgsnChange_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_diagnostics_present) {
		    _f_R4_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnMMRecord.diagnostics);
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_recordSequenceNumber_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_recordExtensions_present) {
		    _f_3R4_Fin_M_ManagementExtensio(_g, &_tempa->value.u.sgsnMMRecord.recordExtensions);
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_localSequenceNumber_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_cAMELInformationMM_present) {
		    if (_tempa->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_sCFAddress_present) {
		    }

		    if (_tempa->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_serviceKey_present) {
		    }

		    if (_tempa->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_defaultTransactionHandling_present) {
		    }

		    if (_tempa->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_numberOfDPEncountered_present) {
		    }

		    if (_tempa->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_levelOfCAMELService_present) {
		    }

		    if (_tempa->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_freeFormatData_present) {
		    }

		    if (_tempa->value.u.sgsnMMRecord.cAMELInformationMM.bit_mask & R4_Fin_M_CAMELInformationMM_fFDAppendIndicator_present) {
		    }

		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_systemType_present) {
		}

		if (_tempa->value.u.sgsnMMRecord.bit_mask & R4_Fin_M_SGSNMMRecord_chChSelectionMode_present) {
		}

		break;

	    case 4:
		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_servedIMEI_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_msNetworkCapability_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_serviceCentre_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_recordingEntity_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_locationArea_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_cellIdentifier_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.messageReference.value)
		    (*_g->freep)(_tempa->value.u.sgsnSMORecord.messageReference.value);
		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_smsResult_present) {
		    _f_R4_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnSMORecord.smsResult);
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_recordExtensions_present) {
		    _f_3R4_Fin_M_ManagementExtensio(_g, &_tempa->value.u.sgsnSMORecord.recordExtensions);
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_localSequenceNumber_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_systemType_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_destinationNumber_present) {
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_cAMELInformationSMS_present) {
		    _f_R4_Fin_M_CAMELInformationSMS(_g, &_tempa->value.u.sgsnSMORecord.cAMELInformationSMS);
		}

		if (_tempa->value.u.sgsnSMORecord.bit_mask & R4_Fin_M_SGSNSMORecord_chChSelectionMode_present) {
		}

		break;

	    case 5:
		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_servedIMEI_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_msNetworkCapability_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_serviceCentre_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_recordingEntity_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_locationArea_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_cellIdentifier_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_smsResult_present) {
		    _f_R4_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnSMTRecord.smsResult);
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_recordExtensions_present) {
		    _f_3R4_Fin_M_ManagementExtensio(_g, &_tempa->value.u.sgsnSMTRecord.recordExtensions);
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_localSequenceNumber_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_systemType_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_chChSelectionMode_present) {
		}

		if (_tempa->value.u.sgsnSMTRecord.bit_mask & R4_Fin_M_SGSNSMTRecord_cAMELInformationSMS_present) {
		    _f_R4_Fin_M_CAMELInformationSMS(_g, &_tempa->value.u.sgsnSMTRecord.cAMELInformationSMS);
		}

		break;

	    case 6:
		_f_R4_Fin_M_LCSClientIdentity(_g, &_tempa->value.u.sgsnLCTRecord.lcsClientIdentity);
		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_sgsnAddress_present) {
		    _f_R4_Fin_M_IPAddress(_g, &_tempa->value.u.sgsnLCTRecord.sgsnAddress);
		}

		if (_tempa->value.u.sgsnLCTRecord.locationType.bit_mask & R4_Fin_M_deferredLocationEventType_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_lcsQos_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_lcsPriority_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_measurementDuration_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_notificationToMSUser_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_privacyOverride_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_location_present) {
		    _f_R4_Fin_M_LocationAreaAndCell(_g, &_tempa->value.u.sgsnLCTRecord.location);
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_locationEstimate_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_lcsCause_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_diagnostics_present) {
		    _f_R4_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnLCTRecord.diagnostics);
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_localSequenceNumber_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_chChSelectionMode_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_systemType_present) {
		}

		if (_tempa->value.u.sgsnLCTRecord.bit_mask & R4_Fin_M_SGSNLCTRecord_recordExtensions_present) {
		    _f_3R4_Fin_M_ManagementExtensio(_g, &_tempa->value.u.sgsnLCTRecord.recordExtensions);
		}

		break;

	    case 7:
		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsClientType_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsClientIdentity_present) {
		    _f_R4_Fin_M_LCSClientIdentity(_g, &_tempa->value.u.sgsnLCORecord.lcsClientIdentity);
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_sgsnAddress_present) {
		    _f_R4_Fin_M_IPAddress(_g, &_tempa->value.u.sgsnLCORecord.sgsnAddress);
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsQos_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsPriority_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_mlcNumber_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_measurementDuration_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_location_present) {
		    _f_R4_Fin_M_LocationAreaAndCell(_g, &_tempa->value.u.sgsnLCORecord.location);
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_locationEstimate_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_lcsCause_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_diagnostics_present) {
		    _f_R4_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnLCORecord.diagnostics);
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_localSequenceNumber_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_chChSelectionMode_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_systemType_present) {
		}

		if (_tempa->value.u.sgsnLCORecord.bit_mask & R4_Fin_M_SGSNLCORecord_recordExtensions_present) {
		    _f_3R4_Fin_M_ManagementExtensio(_g, &_tempa->value.u.sgsnLCORecord.recordExtensions);
		}

		break;

	    case 8:
		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsClientType_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsClientIdentity_present) {
		    _f_R4_Fin_M_LCSClientIdentity(_g, &_tempa->value.u.sgsnLCNRecord.lcsClientIdentity);
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_servedIMSI_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_servedMSISDN_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_sgsnAddress_present) {
		    _f_R4_Fin_M_IPAddress(_g, &_tempa->value.u.sgsnLCNRecord.sgsnAddress);
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_servedIMEI_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsQos_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsPriority_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_mlcNumber_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_measurementDuration_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_location_present) {
		    _f_R4_Fin_M_LocationAreaAndCell(_g, &_tempa->value.u.sgsnLCNRecord.location);
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_routingArea_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_locationEstimate_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_lcsCause_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_diagnostics_present) {
		    _f_R4_Fin_M_Diagnostics(_g, &_tempa->value.u.sgsnLCNRecord.diagnostics);
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_nodeID_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_localSequenceNumber_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_chChSelectionMode_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_systemType_present) {
		}

		if (_tempa->value.u.sgsnLCNRecord.bit_mask & R4_Fin_M_SGSNLCNRecord_recordExtensions_present) {
		    _f_3R4_Fin_M_ManagementExtensio(_g, &_tempa->value.u.sgsnLCNRecord.recordExtensions);
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

static int _fR4_Fin_M_Horizontal_Accuracy(struct ossGlobal *_g, void *_data)
{
    R4_Fin_M_Horizontal_Accuracy	*_data_ptr;
    _data_ptr = (R4_Fin_M_Horizontal_Accuracy *) _data;
    (*_g->freep)(_data_ptr);
    return(PDU_FREED);
}

static int _fR4_Fin_M_ResponseTime(struct ossGlobal *_g, void *_data)
{
    R4_Fin_M_ResponseTime	*_data_ptr;
    _data_ptr = (R4_Fin_M_ResponseTime *) _data;
    (*_g->freep)(_data_ptr);
    return(PDU_FREED);
}

static int _fR4_Fin_M_Vertical_Accuracy(struct ossGlobal *_g, void *_data)
{
    R4_Fin_M_Vertical_Accuracy	*_data_ptr;
    _data_ptr = (R4_Fin_M_Vertical_Accuracy *) _data;
    (*_g->freep)(_data_ptr);
    return(PDU_FREED);
}

static int _fmCDRF_R4(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl)
{
    int _rc = 0;

    switch (_pdunum)
    {
	case 1:
	    _rc = _fR4_Fin_M_BSRecord(_g, _data);
	    break;

	case 2:
	    _rc = _fR4_Fin_M_Horizontal_Accuracy(_g, _data);
	    break;

	case 3:
	    _rc = _fR4_Fin_M_ResponseTime(_g, _data);
	    break;

	case 4:
	    _rc = _fR4_Fin_M_Vertical_Accuracy(_g, _data);
	    break;

	default:  return(PDU_RANGE);
    }
    return _rc;
}

/* Entry points */
static const _entry_point_struct _entry_points = {_ossinit_CDRF_R4_Fin_M, _emCDRF_R4, _dmCDRF_R4, _fmCDRF_R4, 16,
 NULL, NULL, 0, 0, NULL};
static void _oss_post_init(struct ossGlobal *world) {
    static const unsigned char _oss_typeinfo[] = {
        0x00, 0x7c, 0x33, 0xa6, 0x1f, 0x08, 0xb5, 0x3b, 0xfd, 0xfe,
        0x94, 0x23, 0xfd, 0xe3, 0x94, 0xfa, 0xdb, 0x80, 0xdc, 0xa6,
        0x6e, 0xfe, 0x09, 0x1a, 0xe3, 0x96, 0x12, 0xb9, 0x0b, 0x4d,
        0xe0, 0xd6, 0x24, 0x4d, 0x06, 0xd0, 0x8b, 0x28, 0xed, 0x01,
        0x91, 0xe3, 0x4a, 0x5f, 0x5f, 0x3f, 0x05, 0x65, 0xb0, 0x35,
        0x29, 0x99, 0x84, 0x86, 0xdd, 0xa6, 0xbd, 0xf7, 0xa1, 0x5d,
        0xf7, 0x8f, 0xe1, 0x3e, 0xf7, 0xf4, 0x13, 0x72, 0xc6, 0x9c,
        0x2e, 0xf1, 0xbe, 0x4b, 0x30, 0xd9, 0x59, 0x55, 0xfe, 0x57,
        0xea, 0x13, 0x3e, 0x06, 0xbf, 0x52, 0xbc, 0x9a, 0x60, 0x34,
        0x42, 0x63, 0x44, 0x2d, 0xa1, 0x2c, 0x08, 0x6c, 0x01, 0xc3,
        0x88, 0x1d, 0x1b, 0xec, 0x7d, 0x9c, 0x8d, 0x73, 0x67, 0x2e,
        0x3e, 0x9c, 0x46, 0x62, 0xba, 0x50, 0x4f, 0xe6, 0x09, 0x7c,
        0xfe, 0x96, 0x39, 0x12, 0x97, 0x3d
    };
    ossInitRootContext(world, (unsigned char *)_oss_typeinfo);
}

#ifdef _OSSGETHEADER
void *DLL_ENTRY_FDEF ossGetHeader()
{
    return (void *)&_entry_points;
}
#endif /* _OSSGETHEADER */

void *CDRF_R4_Fin_M = (void *)&_entry_points;
