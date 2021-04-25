/********************************************************/
/* Copyright (C) 2003 OSS Nokalva.  All rights reserved.*/
/********************************************************/

/* THIS FILE IS PROPRIETARY MATERIAL OF OSS NOKALVA
 * AND MAY BE USED ONLY BY DIRECT LICENSEES OF OSS NOKALVA.
 * THIS FILE MAY NOT BE DISTRIBUTED. */

/* Generated for: HuaWei Technologies, Inc., Shenzhen, China - license 8088 for Windows NT */
/* Abstract syntax: CDRF_3GCDR_Final */
/* Created: Fri Apr 25 09:48:53 2003 */
/* ASN.1 compiler version: 5.3 */
/* Target operating system: Windows NT/Windows 9x */
/* Target machine type: Intel x86 */
/* C compiler options required: -Zp4 (Microsoft) */
/* ASN.1 compiler options and file names specified:
 * -codefile CDRF_3GCDR_CG.c -headerfile CDRF_3GCDR_CG.h -errorfile
 * C:\WINDOWS\TEMP\2. -prefix _3GCDR_ -nodebug -dualheader -ansi -verbose -ber
 * -root D:\application\ossasn1\win32\5.3.0\asn1dflt\asn1dflt.ms.zp4
 * D:\project\530\asn1app\maimaoshi\CDRF_3GCDR_Final\CDRF_3GCDR_Final.asn
 */

#include   <stddef.h>
#include   <string.h>
#include   <stdio.h>
#include   <limits.h>
#include   "asn1code.h"
#include   "asn1util.h"
#include   "ossber.h"
#include   "../asn1_r99/CDRF_3GCDR_CG.h"

#if ((OSS_TOED_API_LEVEL + 0) < 3)
    #define _oss_dec_getmem_internal	_oss_dec_getmem
#endif /* OSS_TOED_API_LEVEL */

static unsigned char _v0[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00 };
_3GCDR__ObjectID _3GCDR_gsm1205InformationModel = {
    6,
    _v0
};

static unsigned char _v1[] = { 0x04, 0x00, 0x00, 0x03, 0x05, 0x00, 0x02 };
_3GCDR__ObjectID _3GCDR_gsm1205ASN1Module = {
    7,
    _v1
};

int _3GCDR_maxAddressLength = 20;

int _3GCDR_maxISDN_AddressLength = 9;

static ossBoolean _v2 = FALSE;


extern void DLL_ENTRY _oss_run_TOED_on_INTEL_X86_WINDOWS_AND_WINNT(void);
static void _oss_post_init(struct ossGlobal *world);

void DLL_ENTRY_FDEF _ossinit_CDRF_3GCDR_Final(struct ossGlobal *world) {
    _oss_run_TOED_on_INTEL_X86_WINDOWS_AND_WINNT();
    ossLinkBer(world);
    _oss_post_init(world);
}

static long _ed_3GCDR_IPBinaryAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_IPBinaryAddress *_in_data);
static void _d__3GCDR_IPBinaryAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_IPBinaryAddress *_out_data);
static int _f__3GCDR_IPBinaryAddress(struct ossGlobal *_g, _3GCDR_IPBinaryAddress *_data_ptr);
static long _ed0_3GCDR_IPTextRepresentedAdd(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_IPTextRepresentedAddress *_in_data);
static void _d_0_3GCDR_IPTextRepresentedAdd(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_IPTextRepresentedAddress *_out_data);
static int _f_0_3GCDR_IPTextRepresentedAdd(struct ossGlobal *_g, _3GCDR_IPTextRepresentedAddress *_data_ptr);
static long _ed_3GCDR_IPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_IPAddress *_in_data);
static void _d__3GCDR_IPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_IPAddress *_out_data);
static int _f__3GCDR_IPAddress(struct ossGlobal *_g, _3GCDR_IPAddress *_data_ptr);
static long _ed_3GCDR_PDPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_PDPAddress *_in_data);
static void _d__3GCDR_PDPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_PDPAddress *_out_data);
static int _f__3GCDR_PDPAddress(struct ossGlobal *_g, _3GCDR_PDPAddress *_data_ptr);
static long _ed_3GCDR_ManagementExtension(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_ManagementExtension *_in_data);
static void _d__3GCDR_ManagementExtension(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_ManagementExtension *_out_data);
static int _f__3GCDR_ManagementExtension(struct ossGlobal *_g, _3GCDR_ManagementExtension *_data_ptr);
static long _ed_3GCDR_Diagnostics(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_Diagnostics *_in_data);
static void _d__3GCDR_Diagnostics(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_Diagnostics *_out_data);
static int _f__3GCDR_Diagnostics(struct ossGlobal *_g, _3GCDR_Diagnostics *_data_ptr);
static long _ed_3GCDR__seqof1(struct ossGlobal *_g, char **_out_pos, long *_max_len, struct _3GCDR__seqof1 *_in_data);
static void _d__3GCDR__seqof1(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, struct _3GCDR__seqof1 *_out_data);
static int _f__3GCDR__seqof1(struct ossGlobal *_g, struct _3GCDR__seqof1 *_data_ptr);
static long _ed_3GCDR_SequenceList(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_SequenceList *_in_data);
static void _d__3GCDR_SequenceList(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_SequenceList *_out_data);
static int _f__3GCDR_SequenceList(struct ossGlobal *_g, _3GCDR_SequenceList *_data_ptr);
static long _ed_3GCDR_ManagementExtensions(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_ManagementExtensions *_in_data);
static void _d__3GCDR_ManagementExtensions(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_ManagementExtensions *_out_data);
static int _f__3GCDR_ManagementExtensions(struct ossGlobal *_g, _3GCDR_ManagementExtensions *_data_ptr);
static long _ed_3GCDR_GSMQoSInformation(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_GSMQoSInformation *_in_data);
static void _d__3GCDR_GSMQoSInformation(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_GSMQoSInformation *_out_data);
static int _f__3GCDR_GSMQoSInformation(struct ossGlobal *_g, _3GCDR_GSMQoSInformation *_data_ptr);
static long _ed_3GCDR_QoSInformation(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_QoSInformation *_in_data);
static void _d__3GCDR_QoSInformation(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_QoSInformation *_out_data);
static int _f__3GCDR_QoSInformation(struct ossGlobal *_g, _3GCDR_QoSInformation *_data_ptr);
static long _ed_3GCDR_ChangeOfCharCondition(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_ChangeOfCharCondition *_in_data);
static void _d__3GCDR_ChangeOfCharCondition(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_ChangeOfCharCondition *_out_data);
static int _f__3GCDR_ChangeOfCharCondition(struct ossGlobal *_g, _3GCDR_ChangeOfCharCondition *_data_ptr);
static int _e_3GCDR_CallEventRecord(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags);
static int _d_3GCDR_CallEventRecord(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen);
static int _f_3GCDR_CallEventRecord(struct ossGlobal *_g, void *_data);
static int DLL_ENTRY _emCDR3G(struct ossGlobal *_g, int _pdunum, void *_inbuf, char **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH]);
static int DLL_ENTRY _dmCDR3G(struct ossGlobal *_g, int *_pdunum, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH]);
static int DLL_ENTRY _fmCDR3G(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl);



#if OSSDEBUG > 1
static const _Context_Id _context_id[] = {
    {"field #1", "OCTET STRING", _octetstring_type},
    {"field #2", "OCTET STRING", _octetstring_type},
    {"field #1", "Character String", _charstring_type},
    {"field #2", "Character String", _charstring_type},
    {"field #1", "CHOICE", _choice_type},
    {"field #2", "CHOICE", _choice_type},
    {"field #3", "ANY", _any_type},
    {"field #2", "BOOLEAN", _boolean_type},
    {"field #1", "OBJECT IDENTIFIER", _object_identifier_type},
    {"field #1", "INTEGER", _integer_type},
    {"field #2", "INTEGER", _integer_type},
    {"field #3", "INTEGER", _integer_type},
    {"field #4", "SEQUENCE", _sequence_type},
    {"field #5", "SEQUENCE", _sequence_type},
    {NULL, "INTEGER", _integer_type},
    {"field #2", "SEQUENCE OF", _sequence_of_type},
    {NULL, "SEQUENCE", _sequence_type},
    {"field #5", "ENUMERATED", _enumerated_type},
    {"field #4", "ENUMERATED", _enumerated_type},
    {"field #3", "ENUMERATED", _enumerated_type},
    {"field #2", "ENUMERATED", _enumerated_type},
    {"field #1", "ENUMERATED", _enumerated_type},
    {"field #1", "SEQUENCE", _sequence_type},
    {"field #6", "OCTET STRING", _octetstring_type},
    {"field #4", "INTEGER", _integer_type},
    {"PDU #1", "CHOICE", _choice_type},
    {"field #1", "SET", _set_type},
    {"field #31", "ENUMERATED", _enumerated_type},
    {"field #30", "INTEGER", _integer_type},
    {"field #29", "SET", _set_type},
    {"field #9", "BOOLEAN", _boolean_type},
    {"field #8", "OCTET STRING", _octetstring_type},
    {"field #7", "BIT STRING", _bitstring_type},
    {"field #6", "INTEGER", _integer_type},
    {"field #5", "Character String", _charstring_type},
    {"field #4", "Character String", _charstring_type},
    {"field #28", "ENUMERATED", _enumerated_type},
    {"field #27", "OCTET STRING", _octetstring_type},
    {"field #26", "OCTET STRING", _octetstring_type},
    {"field #25", "Character String", _charstring_type},
    {"field #24", "ENUMERATED", _enumerated_type},
    {"field #23", "SET OF", _set_of_type},
    {"field #22", "Character String", _charstring_type},
    {"field #21", "SEQUENCE", _sequence_type},
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
    {"field #7", "OCTET STRING", _octetstring_type},
    {"field #5", "CHOICE", _choice_type},
    {"field #4", "OCTET STRING", _octetstring_type},
    {"field #3", "OCTET STRING", _octetstring_type},
    {"field #2", "SET", _set_type},
    {"field #22", "ENUMERATED", _enumerated_type},
    {"field #21", "OCTET STRING", _octetstring_type},
    {"field #20", "OCTET STRING", _octetstring_type},
    {"field #19", "ENUMERATED", _enumerated_type},
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
    {NULL, "CHOICE", _choice_type},
    {"field #5", "INTEGER", _integer_type},
    {"field #4", "CHOICE", _choice_type},
    {"field #3", "SET", _set_type},
    {"field #21", "SET", _set_type},
    {"field #7", "BOOLEAN", _boolean_type},
    {"field #5", "BIT STRING", _bitstring_type},
    {"field #19", "OCTET STRING", _octetstring_type},
    {"field #18", "INTEGER", _integer_type},
    {"field #17", "SET OF", _set_of_type},
    {"field #16", "Character String", _charstring_type},
    {"field #15", "INTEGER", _integer_type},
    {"field #12", "BOOLEAN", _boolean_type},
    {"field #11", "INTEGER", _integer_type},
    {"field #10", "OCTET STRING", _octetstring_type},
    {"field #9", "SEQUENCE OF", _sequence_of_type},
    {"field #5", "OCTET STRING", _octetstring_type},
    {"field #4", "SET", _set_type},
    {"field #20", "SET", _set_type},
    {"field #18", "ENUMERATED", _enumerated_type},
    {"field #17", "OCTET STRING", _octetstring_type},
    {"field #16", "INTEGER", _integer_type},
    {"field #15", "Character String", _charstring_type},
    {"field #14", "SET OF", _set_of_type},
    {"field #13", "CHOICE", _choice_type},
    {"field #11", "OCTET STRING", _octetstring_type},
    {"field #5", "SET", _set_type},
    {"field #17", "ENUMERATED", _enumerated_type},
    {"field #14", "Character String", _charstring_type},
    {"field #13", "SET OF", _set_of_type},
    {"field #12", "CHOICE", _choice_type}
};
#endif


static long _ed_3GCDR_IPBinaryAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_IPBinaryAddress *_in_data)
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

static long _ed0_3GCDR_IPTextRepresentedAdd(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_IPTextRepresentedAddress *_in_data)
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

static long _ed_3GCDR_IPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_IPAddress *_in_data)
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

	    _data_len = _ed_3GCDR_IPBinaryAddress(_g, _out_pos, &_out_len, &_in_data->u.iPBinaryAddress);
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

	    _data_len = _ed0_3GCDR_IPTextRepresentedAdd(_g, _out_pos, &_out_len, &_in_data->u.iPTextRepresentedAddress);
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

static long _ed_3GCDR_PDPAddress(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_PDPAddress *_in_data)
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

	    _data_len = _ed_3GCDR_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.iPAddress);
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

static long _ed_3GCDR_ManagementExtension(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_ManagementExtension *_in_data)
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

	_data_len = _oss_encd_uany(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->information);
	_constructed = TRUE;
	_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8002, _constructed);
	_total_len += _data_len;
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[7];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & _3GCDR_significance_present) {
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
	_element._id = (_Context_Id *)&_context_id[8];
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

static long _ed_3GCDR_Diagnostics(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_Diagnostics *_in_data)
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
	    _element._id = (_Context_Id *)&_context_id[9];
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
	    _element._id = (_Context_Id *)&_context_id[10];
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
	    _element._id = (_Context_Id *)&_context_id[11];
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
	    _element._id = (_Context_Id *)&_context_id[12];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _ed_3GCDR_ManagementExtension(_g, _out_pos, &_out_len, &_in_data->u.networkSpecificCause);
	    _constructed = TRUE;
	    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 5:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[13];
	    _element._parent = _oss_c->_oss_context_anchor._last;
	    _oss_c->_oss_context_anchor._last = &_element;
#endif

	    _data_len = _ed_3GCDR_ManagementExtension(_g, _out_pos, &_out_len, &_in_data->u.manufacturerSpecificCause);
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

static long _ed_3GCDR__seqof1(struct ossGlobal *_g, char **_out_pos, long *_max_len, struct _3GCDR__seqof1 *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	unsigned long _index;
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[14];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->count) {
	    _index = _in_data->count;
	    do {
#if OSSDEBUG > 1
		_element._occurrence = _index;
#endif

		_index--;
		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->value[_index]);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x2, _constructed);
		_total_len += _data_len;
	    } while (_index > 0);
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

static long _ed_3GCDR_SequenceList(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_SequenceList *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[15];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & _3GCDR_recSequenceNumber_present) {
	    _data_len = _ed_3GCDR__seqof1(_g, _out_pos, &_out_len, &_in_data->recSequenceNumber);
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

	_data_len = _ed_3GCDR_IPAddress(_g, _out_pos, &_out_len, &_in_data->sgsnAddress);
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

static long _ed_3GCDR_ManagementExtensions(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_ManagementExtensions *_in_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    long _out_len = *_max_len;
    long _data_len = 0;
    ossBoolean _constructed = FALSE;

    if (_in_data == NULL)
	_oss_enc_error(_g, TRUE, _bad_pointer, 0L);
    {
	unsigned long _index;
	long _total_len = 0;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[16];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->count) {
	    _index = _in_data->count;
	    do {
#if OSSDEBUG > 1
		_element._occurrence = _index;
#endif

		_index--;
		_data_len = _ed_3GCDR_ManagementExtension(_g, _out_pos, &_out_len, &_in_data->value[_index]);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x10, _constructed);
		_total_len += _data_len;
	    } while (_index > 0);
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

static long _ed_3GCDR_GSMQoSInformation(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_GSMQoSInformation *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[17];
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
	_element._id = (_Context_Id *)&_context_id[18];
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
	_element._id = (_Context_Id *)&_context_id[19];
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
	_element._id = (_Context_Id *)&_context_id[20];
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
	_element._id = (_Context_Id *)&_context_id[21];
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

static long _ed_3GCDR_QoSInformation(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_QoSInformation *_in_data)
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

	    _data_len = _ed_3GCDR_GSMQoSInformation(_g, _out_pos, &_out_len, &_in_data->u.gsmQosInformation);
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

	    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.umtsQosInformation, 2, 12);
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

static long _ed_3GCDR_ChangeOfCharCondition(struct ossGlobal *_g, char **_out_pos, long *_max_len, _3GCDR_ChangeOfCharCondition *_in_data)
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
	_element._id = (_Context_Id *)&_context_id[23];
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
	_element._id = (_Context_Id *)&_context_id[17];
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
	_element._id = (_Context_Id *)&_context_id[24];
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
	_element._id = (_Context_Id *)&_context_id[11];
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
	_element._id = (_Context_Id *)&_context_id[5];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	if (_in_data->bit_mask & _3GCDR_qosNegotiated_present) {
	    _data_len = _ed_3GCDR_QoSInformation(_g, _out_pos, &_out_len, &_in_data->qosNegotiated);
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

	if (_in_data->bit_mask & _3GCDR_qosRequested_present) {
	    _data_len = _ed_3GCDR_QoSInformation(_g, _out_pos, &_out_len, &_in_data->qosRequested);
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

static int _e_3GCDR_CallEventRecord(struct ossGlobal *_g, void *_inbuf, char **_outbuf, long *_outlen, unsigned _flags)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    _3GCDR_CallEventRecord	*_in_data;
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

    _in_data = (_3GCDR_CallEventRecord *) _inbuf;
    if (!_oss_c->_buffer_provided) *_outbuf = NULL;
    _out_buf = *_outbuf;
    _out_len = _oss_c->_oss_outbuflen = (_oss_c->_buffer_provided) ? *_outlen : 0;
    _out_buf += _out_len;
#if OSSDEBUG > 1
    _oss_c->_oss_context_anchor._pdu_number = 1;
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[25];
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
	    _element._id = (_Context_Id *)&_context_id[26];
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
		_element._id = (_Context_Id *)&_context_id[27];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.consolidationResult);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801F, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[28];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_rNCUnsentDownlinkVolume_present) {
		    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.rNCUnsentDownlinkVolume);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801E, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[29];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_cAMELInformationPDP_present) {
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

			if (_in_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_fFDAppendIndicator_present) {
			    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.cAMELInformationPDP.fFDAppendIndicator);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
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

			if (_in_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_freeFormatData_present) {
			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.cAMELInformationPDP.freeFormatData, 2, 160);
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
			_element._id = (_Context_Id *)&_context_id[32];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_levelOfCAMELService_present) {
			    _data_len = _oss_encd_pbit(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.cAMELInformationPDP.levelOfCAMELService,3);
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
			_element._id = (_Context_Id *)&_context_id[33];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_numberOfDPEncountered_present) {
			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.cAMELInformationPDP.numberOfDPEncountered);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8006, _constructed);
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

			if (_in_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_cAMELAccessPointNameOI_present) {
			    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.cAMELInformationPDP.cAMELAccessPointNameOI, 37);
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
			_element._id = (_Context_Id *)&_context_id[35];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_cAMELAccessPointNameNI_present) {
			    _data_len = _oss_encd_nstr(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.cAMELInformationPDP.cAMELAccessPointNameNI, 63);
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
			_element._id = (_Context_Id *)&_context_id[19];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_defaultTransactionHandling_present) {
			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.cAMELInformationPDP.defaultTransactionHandling);
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

			if (_in_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_serviceKey_present) {
			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.cAMELInformationPDP.serviceKey);
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

			if (_in_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_sCFAddress_present) {
			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.cAMELInformationPDP.sCFAddress, 2, 20);
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
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801D, _constructed);
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

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_systemType_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.systemType);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x801C, _constructed);
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

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_chargingCharacteristics_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.chargingCharacteristics, 2, 2);
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
		_element._id = (_Context_Id *)&_context_id[38];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_servedMSISDN_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.servedMSISDN, 2, 9);
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
		_element._id = (_Context_Id *)&_context_id[39];
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
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8019, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[40];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_apnSelectionMode_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.apnSelectionMode);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8018, _constructed);
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

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_recordExtensions_present) {
		    _data_len = _ed_3GCDR_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
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

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_nodeID_present) {
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
		_element._id = (_Context_Id *)&_context_id[43];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_recSequenceNumList_present) {
		    _data_len = _ed_3GCDR_SequenceList(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.recSequenceNumList);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
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

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_diagnostics_present) {
		    _data_len = _ed_3GCDR_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.diagnostics);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
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

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnPDPRecord.causeForRecClosing);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[46];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_sgsnChange_present) {
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
		_element._id = (_Context_Id *)&_context_id[47];
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
		_element._id = (_Context_Id *)&_context_id[48];
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
		_element._id = (_Context_Id *)&_context_id[49];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		{
		    unsigned long _index;
		    long _total_len = 0;

#if OSSDEBUG > 1
		    _Context_Element _element;

		    _element._occurrence = 0;
#endif

#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[16];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_in_data->u.sgsnPDPRecord.listOfTrafficVolumes.count) {
			_index = _in_data->u.sgsnPDPRecord.listOfTrafficVolumes.count;
			do {
#if OSSDEBUG > 1
			    _element._occurrence = _index;
#endif

			    _index--;
			    _data_len = _ed_3GCDR_ChangeOfCharCondition(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.listOfTrafficVolumes.value[_index]);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x10, _constructed);
			    _total_len += _data_len;
			} while (_index > 0);
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
		_element._id = (_Context_Id *)&_context_id[50];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_servedPDPAddress_present) {
		    _data_len = _ed_3GCDR_PDPAddress(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.servedPDPAddress);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800E, _constructed);
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
		_element._id = (_Context_Id *)&_context_id[52];
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
		_element._id = (_Context_Id *)&_context_id[53];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _ed_3GCDR_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.ggsnAddressUsed);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[54];
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
		_element._id = (_Context_Id *)&_context_id[55];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_cellIdentifier_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnPDPRecord.cellIdentifier, 2, 2);
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
		_element._id = (_Context_Id *)&_context_id[31];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_locationAreaCode_present) {
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
		_element._id = (_Context_Id *)&_context_id[56];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_routingArea_present) {
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
		_element._id = (_Context_Id *)&_context_id[23];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_msNetworkCapability_present) {
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
		_element._id = (_Context_Id *)&_context_id[57];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _ed_3GCDR_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.sgsnPDPRecord.sgsnAddress);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8005, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[58];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_servedIMEI_present) {
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
		_element._id = (_Context_Id *)&_context_id[59];
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
		_element._id = (_Context_Id *)&_context_id[7];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_networkInitiation_present) {
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
		_element._id = (_Context_Id *)&_context_id[9];
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
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 2:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[60];
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
		_element._id = (_Context_Id *)&_context_id[61];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.consolidationResult);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[62];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_chargingCharacteristics_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.ggsnPDPRecord.chargingCharacteristics, 2, 2);
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
		_element._id = (_Context_Id *)&_context_id[63];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_servedMSISDN_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.ggsnPDPRecord.servedMSISDN, 2, 9);
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
		_element._id = (_Context_Id *)&_context_id[64];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_apnSelectionMode_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.apnSelectionMode);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8014, _constructed);
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

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_recordExtensions_present) {
		    _data_len = _ed_3GCDR_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
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

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_nodeID_present) {
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
		_element._id = (_Context_Id *)&_context_id[67];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_recSequenceNumList_present) {
		    _data_len = _ed_3GCDR_SequenceList(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.recSequenceNumList);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
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

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_diagnostics_present) {
		    _data_len = _ed_3GCDR_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.diagnostics);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
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

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.ggsnPDPRecord.causeForRecClosing);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800F, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[70];
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
		_element._id = (_Context_Id *)&_context_id[71];
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
		_element._id = (_Context_Id *)&_context_id[72];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		{
		    unsigned long _index;
		    long _total_len = 0;

#if OSSDEBUG > 1
		    _Context_Element _element;

		    _element._occurrence = 0;
#endif

#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[16];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_in_data->u.ggsnPDPRecord.listOfTrafficVolumes.count) {
			_index = _in_data->u.ggsnPDPRecord.listOfTrafficVolumes.count;
			do {
#if OSSDEBUG > 1
			    _element._occurrence = _index;
#endif

			    _index--;
			    _data_len = _ed_3GCDR_ChangeOfCharCondition(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.listOfTrafficVolumes.value[_index]);
			    _constructed = TRUE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x10, _constructed);
			    _total_len += _data_len;
			} while (_index > 0);
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
		_element._id = (_Context_Id *)&_context_id[73];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_dynamicAddressFlag_present) {
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
		_element._id = (_Context_Id *)&_context_id[74];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_servedPDPAddress_present) {
		    _data_len = _ed_3GCDR_PDPAddress(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.servedPDPAddress);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8009, _constructed);
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
		_element._id = (_Context_Id *)&_context_id[75];
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
		_element._id = (_Context_Id *)&_context_id[76];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		{
		    unsigned long _index;
		    long _total_len = 0;

#if OSSDEBUG > 1
		    _Context_Element _element;

		    _element._occurrence = 0;
#endif

#if OSSDEBUG > 1
		    _element._id = (_Context_Id *)&_context_id[77];
		    _element._parent = _oss_c->_oss_context_anchor._last;
		    _oss_c->_oss_context_anchor._last = &_element;
#endif

		    if (_in_data->u.ggsnPDPRecord.sgsnAddress.count) {
			_index = _in_data->u.ggsnPDPRecord.sgsnAddress.count;
			do {
#if OSSDEBUG > 1
			    _element._occurrence = _index;
#endif

			    _index--;
			    _data_len = _ed_3GCDR_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.sgsnAddress.value[_index]);
			    _constructed = TRUE;
			    _total_len += _data_len;
			} while (_index > 0);
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
		_element._id = (_Context_Id *)&_context_id[78];
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
		_element._id = (_Context_Id *)&_context_id[79];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _ed_3GCDR_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.ggsnPDPRecord.ggsnAddress);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[59];
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
		_element._id = (_Context_Id *)&_context_id[7];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_networkInitiation_present) {
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
		_element._id = (_Context_Id *)&_context_id[9];
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
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 3:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[80];
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
		_element._id = (_Context_Id *)&_context_id[61];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_systemType_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.systemType);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8015, _constructed);
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

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_cAMELInformationMM_present) {
		    {
			long _total_len = 0;

#if OSSDEBUG > 1
			_Context_Element _element;

			_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[82];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_fFDAppendIndicator_present) {
			    _data_len = _oss_encd_bool(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.cAMELInformationMM.fFDAppendIndicator);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8007, _constructed);
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

			if (_in_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_freeFormatData_present) {
			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.cAMELInformationMM.freeFormatData, 2, 160);
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
			_element._id = (_Context_Id *)&_context_id[83];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_levelOfCAMELService_present) {
			    _data_len = _oss_encd_pbit(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.cAMELInformationMM.levelOfCAMELService,3);
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

			if (_in_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_numberOfDPEncountered_present) {
			    _data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.cAMELInformationMM.numberOfDPEncountered);
			    _constructed = FALSE;
			    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
			    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8004, _constructed);
			    _total_len += _data_len;
			}

#if OSSDEBUG > 1
			_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[19];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_defaultTransactionHandling_present) {
			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.cAMELInformationMM.defaultTransactionHandling);
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

			if (_in_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_serviceKey_present) {
			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.cAMELInformationMM.serviceKey);
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

			if (_in_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_sCFAddress_present) {
			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.cAMELInformationMM.sCFAddress, 2, 20);
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
		_element._id = (_Context_Id *)&_context_id[63];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_chargingCharacteristics_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.chargingCharacteristics, 2, 2);
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
		_element._id = (_Context_Id *)&_context_id[84];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_servedMSISDN_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.servedMSISDN, 2, 9);
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
		_element._id = (_Context_Id *)&_context_id[85];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_localSequenceNumber_present) {
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
		_element._id = (_Context_Id *)&_context_id[86];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_recordExtensions_present) {
		    _data_len = _ed_3GCDR_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.sgsnMMRecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
		    _total_len += _data_len;
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[87];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_nodeID_present) {
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
		_element._id = (_Context_Id *)&_context_id[88];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_recordSequenceNumber_present) {
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
		_element._id = (_Context_Id *)&_context_id[50];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_diagnostics_present) {
		    _data_len = _ed_3GCDR_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.sgsnMMRecord.diagnostics);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
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

		_data_len = _oss_encd_int(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnMMRecord.causeForRecClosing);
		_constructed = FALSE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[89];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_sgsnChange_present) {
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
		_element._id = (_Context_Id *)&_context_id[90];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_duration_present) {
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
		_element._id = (_Context_Id *)&_context_id[91];
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
		_element._id = (_Context_Id *)&_context_id[92];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_changeLocation_present) {
		    {
			unsigned long _index;
			long _total_len = 0;

#if OSSDEBUG > 1
			_Context_Element _element;

			_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[16];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnMMRecord.changeLocation.count) {
			    _index = _in_data->u.sgsnMMRecord.changeLocation.count;
			    do {
#if OSSDEBUG > 1
				_element._occurrence = _index;
#endif

				_index--;
				{
				    long _total_len = 0;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[58];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.changeLocation.value[_index].changeTime, 2, 9);
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
				    _element._id = (_Context_Id *)&_context_id[59];
				    _element._parent = _oss_c->_oss_context_anchor._last;
				    _oss_c->_oss_context_anchor._last = &_element;
#endif

				    if (_in_data->u.sgsnMMRecord.changeLocation.value[_index].bit_mask & _3GCDR_cellId_present) {
					_data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.changeLocation.value[_index].cellId, 2, 2);
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

				    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.changeLocation.value[_index].routingAreaCode, 2, 1);
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

				    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.changeLocation.value[_index].locationAreaCode, 2, 2);
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
			    } while (_index > 0);
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
		_element._id = (_Context_Id *)&_context_id[31];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_cellIdentifier_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnMMRecord.cellIdentifier, 2, 2);
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
		_element._id = (_Context_Id *)&_context_id[56];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_locationAreaCode_present) {
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
		_element._id = (_Context_Id *)&_context_id[23];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_routingArea_present) {
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
		_element._id = (_Context_Id *)&_context_id[93];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_msNetworkCapability_present) {
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
		_element._id = (_Context_Id *)&_context_id[79];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _ed_3GCDR_IPAddress(_g, _out_pos, &_out_len, &_in_data->u.sgsnMMRecord.sgsnAddress);
		_constructed = TRUE;
		_data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		_data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8003, _constructed);
		_total_len += _data_len;
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[59];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_servedIMEI_present) {
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
		_element._id = (_Context_Id *)&_context_id[1];
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
		_element._id = (_Context_Id *)&_context_id[9];
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
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8016, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 4:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[94];
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
		_element._id = (_Context_Id *)&_context_id[95];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_cAMELInformationSMS_present) {
		    {
			long _total_len = 0;

#if OSSDEBUG > 1
			_Context_Element _element;

			_element._occurrence = 0;
#endif

#if OSSDEBUG > 1
			_element._id = (_Context_Id *)&_context_id[56];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_CAMELInformationSMS_freeFormatData_present) {
			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.cAMELInformationSMS.freeFormatData, 2, 160);
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
			_element._id = (_Context_Id *)&_context_id[23];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_cAMELSMSCAddress_present) {
			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.cAMELInformationSMS.cAMELSMSCAddress, 2, 20);
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
			_element._id = (_Context_Id *)&_context_id[93];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_cAMELDestinationSubscriberNumber_present) {
			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.cAMELInformationSMS.cAMELDestinationSubscriberNumber, 2, 10);
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
			_element._id = (_Context_Id *)&_context_id[58];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_cAMELCallingPartyNumber_present) {
			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.cAMELInformationSMS.cAMELCallingPartyNumber, 2, 10);
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
			_element._id = (_Context_Id *)&_context_id[19];
			_element._parent = _oss_c->_oss_context_anchor._last;
			_oss_c->_oss_context_anchor._last = &_element;
#endif

			if (_in_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_defaultSMSHandling_present) {
			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMORecord.cAMELInformationSMS.defaultSMSHandling);
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

			if (_in_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_CAMELInformationSMS_serviceKey_present) {
			    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMORecord.cAMELInformationSMS.serviceKey);
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

			if (_in_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_CAMELInformationSMS_sCFAddress_present) {
			    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.cAMELInformationSMS.sCFAddress, 2, 20);
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
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8013, _constructed);
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

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_destinationNumber_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.destinationNumber, 2, 10);
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
		_element._id = (_Context_Id *)&_context_id[96];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_systemType_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMORecord.systemType);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8011, _constructed);
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

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_chargingCharacteristics_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.chargingCharacteristics, 2, 2);
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
		_element._id = (_Context_Id *)&_context_id[98];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_localSequenceNumber_present) {
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
		_element._id = (_Context_Id *)&_context_id[99];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_nodeID_present) {
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
		_element._id = (_Context_Id *)&_context_id[100];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_recordExtensions_present) {
		    _data_len = _ed_3GCDR_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.sgsnSMORecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800D, _constructed);
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

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_smsResult_present) {
		    _data_len = _ed_3GCDR_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.sgsnSMORecord.smsResult);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
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
		_element._id = (_Context_Id *)&_context_id[102];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_data_len = _oss_encd_uoct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.messageReference, 4, -1);
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
		_element._id = (_Context_Id *)&_context_id[91];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_cellIdentifier_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.cellIdentifier, 2, 2);
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

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_routingArea_present) {
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
		_element._id = (_Context_Id *)&_context_id[31];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_locationArea_present) {
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
		_element._id = (_Context_Id *)&_context_id[56];
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
		_element._id = (_Context_Id *)&_context_id[23];
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
		_element._id = (_Context_Id *)&_context_id[93];
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
		_element._id = (_Context_Id *)&_context_id[58];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_servedMSISDN_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMORecord.servedMSISDN, 2, 9);
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
		_element._id = (_Context_Id *)&_context_id[59];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_servedIMEI_present) {
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
		_element._id = (_Context_Id *)&_context_id[1];
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
		_element._id = (_Context_Id *)&_context_id[9];
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
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8017, _constructed);
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    break;

	case 5:
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[103];
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
		_element._id = (_Context_Id *)&_context_id[104];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_systemType_present) {
		    _data_len = _oss_encd_uint(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _in_data->u.sgsnSMTRecord.systemType);
		    _constructed = FALSE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8010, _constructed);
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

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_chargingCharacteristics_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.chargingCharacteristics, 2, 2);
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
		_element._id = (_Context_Id *)&_context_id[88];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_localSequenceNumber_present) {
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
		_element._id = (_Context_Id *)&_context_id[105];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_nodeID_present) {
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
		_element._id = (_Context_Id *)&_context_id[106];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_recordExtensions_present) {
		    _data_len = _ed_3GCDR_ManagementExtensions(_g, _out_pos, &_out_len, &_in_data->u.sgsnSMTRecord.recordExtensions);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800C, _constructed);
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

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_smsResult_present) {
		    _data_len = _ed_3GCDR_Diagnostics(_g, _out_pos, &_out_len, &_in_data->u.sgsnSMTRecord.smsResult);
		    _constructed = TRUE;
		    _data_len += _oss_encd_length(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, _data_len);
		    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x800B, _constructed);
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
		_element._id = (_Context_Id *)&_context_id[91];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_cellIdentifier_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.cellIdentifier, 2, 2);
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

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_routingArea_present) {
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
		_element._id = (_Context_Id *)&_context_id[31];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_locationArea_present) {
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
		_element._id = (_Context_Id *)&_context_id[56];
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
		_element._id = (_Context_Id *)&_context_id[23];
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
		_element._id = (_Context_Id *)&_context_id[93];
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
		_element._id = (_Context_Id *)&_context_id[58];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_servedMSISDN_present) {
		    _data_len = _oss_encd_voct(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, &_in_data->u.sgsnSMTRecord.servedMSISDN, 2, 9);
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
		_element._id = (_Context_Id *)&_context_id[59];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		if (_in_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_servedIMEI_present) {
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
		_element._id = (_Context_Id *)&_context_id[1];
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
		_element._id = (_Context_Id *)&_context_id[9];
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
	    _data_len += _oss_encd_tag(_g, _out_pos, &_out_len, _oss_c->_buffer_provided, (unsigned short) 0x8018, _constructed);
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

static int DLL_ENTRY_FDEF _emCDR3G(struct ossGlobal *_g, int _pdunum, void *_inbuf, char **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH])
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
	    _rc = _e_3GCDR_CallEventRecord(_g, _inbuf, _outbuf, _outlen, _flags);
	    break;

	default:
	    _oss_enc_error(_g, TRUE, _pdu_range, (long) _pdunum);
    }

    _oss_c->_oss_err_msg = NULL;
    return _rc;
}

static void _d__3GCDR_IPBinaryAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_IPBinaryAddress *_out_data)
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

static void _d_0_3GCDR_IPTextRepresentedAdd(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_IPTextRepresentedAddress *_out_data)
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

static void _d__3GCDR_IPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_IPAddress *_out_data)
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
	    _d__3GCDR_IPBinaryAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPBinaryAddress);
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
	    _d_0_3GCDR_IPTextRepresentedAdd(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPTextRepresentedAddress);
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

static void _d__3GCDR_PDPAddress(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_PDPAddress *_out_data)
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
		_d__3GCDR_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.iPAddress);
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

static void _d__3GCDR_ManagementExtension(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_ManagementExtension *_out_data)
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
	_element._id = (_Context_Id *)&_context_id[8];
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
	    _out_data->bit_mask &= ~_3GCDR_significance_present;
	    memcpy(&_out_data->significance, &_v2, sizeof(_v2));
	}
	else {
	    _out_data->bit_mask |= _3GCDR_significance_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[7];
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
	_element._id = (_Context_Id *)&_context_id[6];
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

static void _d__3GCDR_Diagnostics(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_Diagnostics *_out_data)
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
		_element._id = (_Context_Id *)&_context_id[9];
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
		_element._id = (_Context_Id *)&_context_id[10];
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
		_element._id = (_Context_Id *)&_context_id[11];
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
		_element._id = (_Context_Id *)&_context_id[12];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 4;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_d__3GCDR_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.networkSpecificCause);
#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x4: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[13];
		_element._parent = _oss_c->_oss_context_anchor._last;
		_oss_c->_oss_context_anchor._last = &_element;
#endif

		_out_data->choice = 5;
		_oss_c->_tag_decoded = FALSE;
		_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
		_d__3GCDR_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.manufacturerSpecificCause);
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

static void _d__3GCDR__seqof1(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, struct _3GCDR__seqof1 *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
	unsigned long count;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	_out_data->count = 0;
	count = _oss_count_setof_items(_g, _in_pos, _buf_len, _total_len);
	if (count > 0) {
	    _out_data->value = (int *) _oss_dec_getmem(_g, sizeof(int)*count, _oss_c->_buffer_provided);
	}
#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[14];
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
	    if (_out_data->count >= count) _oss_dec_error(_g, TRUE, _too_long, (long)_out_data->count);
	    if (_data_tag != 0x2) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
#if OSSDEBUG > 1
	    _element._occurrence = _out_data->count + 1;
#endif

	    _oss_c->_tag_decoded = FALSE;
	    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	    _out_data->value[_out_data->count] = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
	    _out_data->count++;
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

    }
}

static void _d__3GCDR_SequenceList(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_SequenceList *_out_data)
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
	_d__3GCDR_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->sgsnAddress);
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
	    _out_data->bit_mask &= ~_3GCDR_recSequenceNumber_present;
	else {
	    _out_data->bit_mask |= _3GCDR_recSequenceNumber_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[15];
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
	    _d__3GCDR__seqof1(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->recSequenceNumber);
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

static void _d__3GCDR_ManagementExtensions(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_ManagementExtensions *_out_data)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    ossBoolean _constructed;

    {
	long _total_len = _data_length;
	char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
	unsigned long count;

#if OSSDEBUG > 1
	_Context_Element _element;

	_element._occurrence = 0;
#endif

	_out_data->count = 0;
	count = _oss_count_setof_items(_g, _in_pos, _buf_len, _total_len);
	if (count > 0) {
	    _out_data->value = (struct _3GCDR_ManagementExtension *) _oss_dec_getmem(_g, sizeof(struct _3GCDR_ManagementExtension)*count, _oss_c->_buffer_provided);
	}
#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[16];
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
	    if (_out_data->count >= count) _oss_dec_error(_g, TRUE, _too_long, (long)_out_data->count);
	    if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
#if OSSDEBUG > 1
	    _element._occurrence = _out_data->count + 1;
#endif

	    _oss_c->_tag_decoded = FALSE;
	    _data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	    _d__3GCDR_ManagementExtension(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->value[_out_data->count]);
	    _out_data->count++;
	}
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

    }
}

static void _d__3GCDR_GSMQoSInformation(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_GSMQoSInformation *_out_data)
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
	_element._id = (_Context_Id *)&_context_id[21];
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
	_out_data->reliability = (enum _3GCDR_QoSReliability) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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
	_out_data->delay = (enum _3GCDR_QoSDelay) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[19];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8002) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->precedence = (enum _3GCDR_QoSPrecedence) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[18];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8003) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->peakThroughput = (enum _3GCDR_QoSPeakThroughput) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[17];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8004) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->meanThroughput = (enum _3GCDR_QoSMeanThroughput) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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

static void _d__3GCDR_QoSInformation(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_QoSInformation *_out_data)
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
		_d__3GCDR_GSMQoSInformation(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.gsmQosInformation);
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
		_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.umtsQosInformation, 2, 12, _constructed);
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

static void _d__3GCDR_ChangeOfCharCondition(struct ossGlobal *_g, char **_in_pos, long *_buf_len, long _data_length, unsigned short _data_tag, ossBoolean _tag_constructed, _3GCDR_ChangeOfCharCondition *_out_data)
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

	if (!_oss_c->_tag_decoded && (_total_len < 0 || *_in_pos < _end_pos)) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8001 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~_3GCDR_qosRequested_present;
	else {
	    _out_data->bit_mask |= _3GCDR_qosRequested_present;
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
	    _d__3GCDR_QoSInformation(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->qosRequested);
	    while (_indef_tags > 0) {
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
		if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
		if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
		_indef_tags--;
	    }
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

	if (!_oss_c->_tag_decoded && (_total_len < 0 || *_in_pos < _end_pos)) {
	    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	    _oss_c->_tag_decoded = TRUE;
	}
	if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
	    _out_data->bit_mask &= ~_3GCDR_qosNegotiated_present;
	else {
	    _out_data->bit_mask |= _3GCDR_qosNegotiated_present;
#if OSSDEBUG > 1
	    _element._id = (_Context_Id *)&_context_id[5];
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
	    if (_data_length < 0) ++_indef_tags;
	    _d__3GCDR_QoSInformation(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->qosNegotiated);
	    while (_indef_tags > 0) {
		_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
		if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
		if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
		_indef_tags--;
	    }
#if OSSDEBUG > 1
	    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

	}

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[11];
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
	_element._id = (_Context_Id *)&_context_id[24];
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
	_element._id = (_Context_Id *)&_context_id[17];
	_element._parent = _oss_c->_oss_context_anchor._last;
	_oss_c->_oss_context_anchor._last = &_element;
#endif

	_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
	if (_data_tag != 0x8005) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);

	_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
	_out_data->changeCondition = (enum _3GCDR_ChangeCondition) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
#if OSSDEBUG > 1
	_oss_c->_oss_context_anchor._last = _element._parent;
#endif

#if OSSDEBUG > 1
	_element._id = (_Context_Id *)&_context_id[23];
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

static int _d_3GCDR_CallEventRecord(struct ossGlobal *_g, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen)
{
    _EncDecGlobals *_oss_c = (_EncDecGlobals*)_g->encDecVar;
    _3GCDR_CallEventRecord	*_out_data;
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
    _oss_c->_oss_context_anchor._pdu_id = _element._id = (_Context_Id *)&_context_id[25];
    _oss_c->_oss_context_anchor._last = &_element;
#else
    _oss_c->_oss_context_anchor._pdu_number = 0;
#endif
    _out_data = (struct _3GCDR_CallEventRecord *) _oss_dec_getmem(_g, sizeof(struct _3GCDR_CallEventRecord)*1, _oss_c->_buffer_provided);
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
	    case 0x14: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[26];
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
				_element._id = (_Context_Id *)&_context_id[9];
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
				_element._id = (_Context_Id *)&_context_id[7];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_networkInitiation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_networkInitiation_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.networkInitiation = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x2;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x3: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[59];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.servedIMSI, 2, 8, _constructed);
				_present_flags |= 0x4;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x4: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[58];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_servedIMEI_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.servedIMEI, 2, 8, _constructed);
				_present_flags |= 0x8;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x5: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[57];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.sgsnAddress);
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

			    case 0x6: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[23];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_msNetworkCapability_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.msNetworkCapability, 2, 1, _constructed);
				_present_flags |= 0x20;
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

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_routingArea_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.routingArea, 2, 1, _constructed);
				_present_flags |= 0x40;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x8: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[31];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_locationAreaCode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_locationAreaCode_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.locationAreaCode, 2, 2, _constructed);
				_present_flags |= 0x80;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x9: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[55];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_cellIdentifier_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_cellIdentifier_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.cellIdentifier, 2, 2, _constructed);
				_present_flags |= 0x100;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xA: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[54];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.chargingID = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x200;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xB: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[53];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.ggsnAddressUsed);
				while (_indef_tags > 0) {
				    _oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
				    if (_data_tag != 0) _oss_dec_error(_g, TRUE, _expec_eoc, 0L);
				    if (_oss_dec_length(_g, _in_pos, _buf_len) != 0) _oss_dec_error(_g, TRUE, _non_std_eoc, 0L);
				    _indef_tags--;
				}
				_present_flags |= 0x400;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xC: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[52];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnPDPRecord.accessPointNameNI, 63, _constructed);
				_present_flags |= 0x800;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xD: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[51];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.pdpType, 2, 2, _constructed);
				_present_flags |= 0x1000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xE: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[50];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_servedPDPAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_servedPDPAddress_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.servedPDPAddress);
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

			    case 0xF: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[49];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				{
				    long _total_len = _data_length;
				    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
				    unsigned long count;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

				    _out_data->u.sgsnPDPRecord.listOfTrafficVolumes.count = 0;
				    count = _oss_count_setof_items(_g, _in_pos, _buf_len, _total_len);
				    if (count > 0) {
					_out_data->u.sgsnPDPRecord.listOfTrafficVolumes.value = (struct _3GCDR_ChangeOfCharCondition *) _oss_dec_getmem(_g, sizeof(struct _3GCDR_ChangeOfCharCondition)*count, _oss_c->_buffer_provided);
				    }
#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[16];
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
					if (_out_data->u.sgsnPDPRecord.listOfTrafficVolumes.count >= count) _oss_dec_error(_g, TRUE, _too_long, (long)_out_data->u.sgsnPDPRecord.listOfTrafficVolumes.count);
					if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
#if OSSDEBUG > 1
					_element._occurrence = _out_data->u.sgsnPDPRecord.listOfTrafficVolumes.count + 1;
#endif

					_oss_c->_tag_decoded = FALSE;
					_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					_d__3GCDR_ChangeOfCharCondition(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.listOfTrafficVolumes.value[_out_data->u.sgsnPDPRecord.listOfTrafficVolumes.count]);
					_out_data->u.sgsnPDPRecord.listOfTrafficVolumes.count++;
				    }
#if OSSDEBUG > 1
				    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

				}
				_present_flags |= 0x4000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x10: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[48];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.recordOpeningTime, 2, 9, _constructed);
				_present_flags |= 0x8000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x11: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[47];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x10000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x12: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[46];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_sgsnChange_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_sgsnChange_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.sgsnChange = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x20000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x13: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[45];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x40000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x14: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[44];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_diagnostics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.diagnostics);
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
				_element._id = (_Context_Id *)&_context_id[43];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_recSequenceNumList_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_recSequenceNumList_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d__3GCDR_SequenceList(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.recSequenceNumList);
				_present_flags |= 0x100000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x16: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[42];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_nodeID_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnPDPRecord.nodeID, 20, _constructed);
				_present_flags |= 0x200000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x17: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[41];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d__3GCDR_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnPDPRecord.recordExtensions);
				_present_flags |= 0x400000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x18: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[40];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_apnSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_apnSelectionMode_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.apnSelectionMode = (enum _3GCDR_APNSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x800000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x19: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[39];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnPDPRecord.accessPointNameOI, 37, _constructed);
				_present_flags |= 0x1000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1A: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[38];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_servedMSISDN_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.servedMSISDN, 2, 9, _constructed);
				_present_flags |= 0x2000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1B: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[37];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_chargingCharacteristics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_chargingCharacteristics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.chargingCharacteristics, 2, 2, _constructed);
				_present_flags |= 0x4000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1C: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[36];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_SGSNPDPRecord_systemType_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.systemType = (enum _3GCDR_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x8000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1D: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[29];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_cAMELInformationPDP_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_cAMELInformationPDP_present;
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

						if (_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_sCFAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= _3GCDR_CAMELInformationPDP_sCFAddress_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.cAMELInformationPDP.sCFAddress, 2, 20, _constructed);
						_present_flags |= 0x1;
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

						if (_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_serviceKey_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= _3GCDR_CAMELInformationPDP_serviceKey_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.serviceKey = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
						_present_flags |= 0x2;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x3: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[19];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_defaultTransactionHandling_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= _3GCDR_CAMELInformationPDP_defaultTransactionHandling_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.defaultTransactionHandling = (enum _3GCDR_DefaultGPRS_Handling) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
						_present_flags |= 0x4;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x4: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[35];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_cAMELAccessPointNameNI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= _3GCDR_cAMELAccessPointNameNI_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnPDPRecord.cAMELInformationPDP.cAMELAccessPointNameNI, 63, _constructed);
						_present_flags |= 0x8;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x5: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[34];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_cAMELAccessPointNameOI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= _3GCDR_cAMELAccessPointNameOI_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.sgsnPDPRecord.cAMELInformationPDP.cAMELAccessPointNameOI, 37, _constructed);
						_present_flags |= 0x10;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x6: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[33];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_numberOfDPEncountered_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= _3GCDR_CAMELInformationPDP_numberOfDPEncountered_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.numberOfDPEncountered = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
						_present_flags |= 0x20;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x7: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[32];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_levelOfCAMELService_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= _3GCDR_CAMELInformationPDP_levelOfCAMELService_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_pbit(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.cAMELInformationPDP.levelOfCAMELService, 3, _constructed, 1);
						_present_flags |= 0x40;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x8: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[31];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_freeFormatData_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= _3GCDR_CAMELInformationPDP_freeFormatData_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnPDPRecord.cAMELInformationPDP.freeFormatData, 2, 160, _constructed);
						_present_flags |= 0x80;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x9: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[30];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_fFDAppendIndicator_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask |= _3GCDR_CAMELInformationPDP_fFDAppendIndicator_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnPDPRecord.cAMELInformationPDP.fFDAppendIndicator = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
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
					_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~_3GCDR_CAMELInformationPDP_sCFAddress_present;
				    }
				    if (!(_present_flags & 0x2)) {
					_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~_3GCDR_CAMELInformationPDP_serviceKey_present;
				    }
				    if (!(_present_flags & 0x4)) {
					_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~_3GCDR_CAMELInformationPDP_defaultTransactionHandling_present;
				    }
				    if (!(_present_flags & 0x8)) {
					_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~_3GCDR_cAMELAccessPointNameNI_present;
				    }
				    if (!(_present_flags & 0x10)) {
					_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~_3GCDR_cAMELAccessPointNameOI_present;
				    }
				    if (!(_present_flags & 0x20)) {
					_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~_3GCDR_CAMELInformationPDP_numberOfDPEncountered_present;
				    }
				    if (!(_present_flags & 0x40)) {
					_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~_3GCDR_CAMELInformationPDP_levelOfCAMELService_present;
				    }
				    if (!(_present_flags & 0x80)) {
					_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~_3GCDR_CAMELInformationPDP_freeFormatData_present;
				    }
				    if (!(_present_flags & 0x100)) {
					_out_data->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask &= ~_3GCDR_CAMELInformationPDP_fFDAppendIndicator_present;
				    }
				}

				_present_flags |= 0x10000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1E: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[28];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnPDPRecord.bit_mask & _3GCDR_rNCUnsentDownlinkVolume_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnPDPRecord.bit_mask |= _3GCDR_rNCUnsentDownlinkVolume_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.rNCUnsentDownlinkVolume = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x20000000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x1F: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[27];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnPDPRecord.consolidationResult = (enum _3GCDR_ConsolidationResult) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x40000000;
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
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_networkInitiation_present;
		    }
		    if (!(_present_flags & 0x8)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_servedIMEI_present;
		    }
		    if (!(_present_flags & 0x20)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_msNetworkCapability_present;
		    }
		    if (!(_present_flags & 0x40)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_routingArea_present;
		    }
		    if (!(_present_flags & 0x80)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_locationAreaCode_present;
		    }
		    if (!(_present_flags & 0x100)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_cellIdentifier_present;
		    }
		    if (!(_present_flags & 0x2000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_servedPDPAddress_present;
		    }
		    if (!(_present_flags & 0x20000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_sgsnChange_present;
		    }
		    if (!(_present_flags & 0x80000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_diagnostics_present;
		    }
		    if (!(_present_flags & 0x100000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_recSequenceNumList_present;
		    }
		    if (!(_present_flags & 0x200000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x400000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x800000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_apnSelectionMode_present;
		    }
		    if (!(_present_flags & 0x2000000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_servedMSISDN_present;
		    }
		    if (!(_present_flags & 0x4000000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_chargingCharacteristics_present;
		    }
		    if (!(_present_flags & 0x8000000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_SGSNPDPRecord_systemType_present;
		    }
		    if (!(_present_flags & 0x10000000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_cAMELInformationPDP_present;
		    }
		    if (!(_present_flags & 0x20000000)) {
			_out_data->u.sgsnPDPRecord.bit_mask &= ~_3GCDR_rNCUnsentDownlinkVolume_present;
		    }
		    if ((_present_flags & 0x4105DE15) != 0x4105DE15) _oss_dec_error(_g, TRUE, _field_omit, 0L);
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x15: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[60];
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
				_element._id = (_Context_Id *)&_context_id[9];
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
				_element._id = (_Context_Id *)&_context_id[7];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_networkInitiation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_GGSNPDPRecord_networkInitiation_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.networkInitiation = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x2;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x3: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[59];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.ggsnPDPRecord.servedIMSI, 2, 8, _constructed);
				_present_flags |= 0x4;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x4: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[79];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.ggsnAddress);
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
				_element._id = (_Context_Id *)&_context_id[78];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.chargingID = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x10;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x6: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[76];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				{
				    long _total_len = _data_length;
				    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
				    unsigned long count;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

				    _out_data->u.ggsnPDPRecord.sgsnAddress.count = 0;
				    count = _oss_count_setof_items(_g, _in_pos, _buf_len, _total_len);
				    if (count > 0) {
					_out_data->u.ggsnPDPRecord.sgsnAddress.value = (struct _3GCDR_IPAddress *) _oss_dec_getmem(_g, sizeof(struct _3GCDR_IPAddress)*count, _oss_c->_buffer_provided);
				    }
#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[77];
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
					if (_out_data->u.ggsnPDPRecord.sgsnAddress.count >= count) _oss_dec_error(_g, TRUE, _too_long, (long)_out_data->u.ggsnPDPRecord.sgsnAddress.count);
#if OSSDEBUG > 1
					_element._occurrence = _out_data->u.ggsnPDPRecord.sgsnAddress.count + 1;
#endif

					_d__3GCDR_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.sgsnAddress.value[_out_data->u.ggsnPDPRecord.sgsnAddress.count]);
					_out_data->u.ggsnPDPRecord.sgsnAddress.count++;
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
				_element._id = (_Context_Id *)&_context_id[75];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.ggsnPDPRecord.accessPointNameNI, 63, _constructed);
				_present_flags |= 0x40;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x8: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[31];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.ggsnPDPRecord.pdpType, 2, 2, _constructed);
				_present_flags |= 0x80;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x9: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[74];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_servedPDPAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_GGSNPDPRecord_servedPDPAddress_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_PDPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.servedPDPAddress);
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
				_element._id = (_Context_Id *)&_context_id[73];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_dynamicAddressFlag_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_dynamicAddressFlag_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.dynamicAddressFlag = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x200;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xC: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[72];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				{
				    long _total_len = _data_length;
				    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
				    unsigned long count;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

				    _out_data->u.ggsnPDPRecord.listOfTrafficVolumes.count = 0;
				    count = _oss_count_setof_items(_g, _in_pos, _buf_len, _total_len);
				    if (count > 0) {
					_out_data->u.ggsnPDPRecord.listOfTrafficVolumes.value = (struct _3GCDR_ChangeOfCharCondition *) _oss_dec_getmem(_g, sizeof(struct _3GCDR_ChangeOfCharCondition)*count, _oss_c->_buffer_provided);
				    }
#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[16];
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
					if (_out_data->u.ggsnPDPRecord.listOfTrafficVolumes.count >= count) _oss_dec_error(_g, TRUE, _too_long, (long)_out_data->u.ggsnPDPRecord.listOfTrafficVolumes.count);
					if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
#if OSSDEBUG > 1
					_element._occurrence = _out_data->u.ggsnPDPRecord.listOfTrafficVolumes.count + 1;
#endif

					_oss_c->_tag_decoded = FALSE;
					_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
					_d__3GCDR_ChangeOfCharCondition(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.listOfTrafficVolumes.value[_out_data->u.ggsnPDPRecord.listOfTrafficVolumes.count]);
					_out_data->u.ggsnPDPRecord.listOfTrafficVolumes.count++;
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
				_element._id = (_Context_Id *)&_context_id[71];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.ggsnPDPRecord.recordOpeningTime, 2, 9, _constructed);
				_present_flags |= 0x800;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xE: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[70];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.duration = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x1000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xF: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[69];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.causeForRecClosing = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x2000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x10: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[68];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_GGSNPDPRecord_diagnostics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.diagnostics);
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
				_element._id = (_Context_Id *)&_context_id[67];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_recSequenceNumList_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_GGSNPDPRecord_recSequenceNumList_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d__3GCDR_SequenceList(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.recSequenceNumList);
				_present_flags |= 0x8000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x12: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[66];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_GGSNPDPRecord_nodeID_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_nstr(_g, _in_pos, _buf_len, _data_length, _out_data->u.ggsnPDPRecord.nodeID, 20, _constructed);
				_present_flags |= 0x10000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x13: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[65];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_GGSNPDPRecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d__3GCDR_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.ggsnPDPRecord.recordExtensions);
				_present_flags |= 0x20000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x14: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[64];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_apnSelectionMode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_GGSNPDPRecord_apnSelectionMode_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.apnSelectionMode = (enum _3GCDR_APNSelectionMode) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x40000;
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

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_GGSNPDPRecord_servedMSISDN_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.ggsnPDPRecord.servedMSISDN, 2, 9, _constructed);
				_present_flags |= 0x80000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x16: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[62];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_chargingCharacteristics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.ggsnPDPRecord.bit_mask |= _3GCDR_GGSNPDPRecord_chargingCharacteristics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.ggsnPDPRecord.chargingCharacteristics, 2, 2, _constructed);
				_present_flags |= 0x100000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x17: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[61];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.ggsnPDPRecord.consolidationResult = (enum _3GCDR_ConsolidationResult) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_GGSNPDPRecord_networkInitiation_present;
		    }
		    if (!(_present_flags & 0x100)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_GGSNPDPRecord_servedPDPAddress_present;
		    }
		    if (!(_present_flags & 0x200)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_dynamicAddressFlag_present;
		    }
		    if (!(_present_flags & 0x4000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_GGSNPDPRecord_diagnostics_present;
		    }
		    if (!(_present_flags & 0x8000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_GGSNPDPRecord_recSequenceNumList_present;
		    }
		    if (!(_present_flags & 0x10000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_GGSNPDPRecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x20000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_GGSNPDPRecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x40000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_GGSNPDPRecord_apnSelectionMode_present;
		    }
		    if (!(_present_flags & 0x80000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_GGSNPDPRecord_servedMSISDN_present;
		    }
		    if (!(_present_flags & 0x100000)) {
			_out_data->u.ggsnPDPRecord.bit_mask &= ~_3GCDR_GGSNPDPRecord_chargingCharacteristics_present;
		    }
		    if ((_present_flags & 0x203CFD) != 0x203CFD) _oss_dec_error(_g, TRUE, _field_omit, 0L);
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x16: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[80];
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
				_element._id = (_Context_Id *)&_context_id[9];
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
				_element._id = (_Context_Id *)&_context_id[1];
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
				_element._id = (_Context_Id *)&_context_id[59];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_servedIMEI_present;
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
				_element._id = (_Context_Id *)&_context_id[79];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_IPAddress(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnMMRecord.sgsnAddress);
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

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_msNetworkCapability_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_msNetworkCapability_present;
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
				_element._id = (_Context_Id *)&_context_id[23];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_routingArea_present;
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
				_element._id = (_Context_Id *)&_context_id[56];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_locationAreaCode_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_locationAreaCode_present;
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
				_element._id = (_Context_Id *)&_context_id[31];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_cellIdentifier_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_cellIdentifier_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.cellIdentifier, 2, 2, _constructed);
				_present_flags |= 0x80;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x8: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[92];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_changeLocation_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_changeLocation_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				{
				    long _total_len = _data_length;
				    char *_end_pos = (_total_len < 0) ? 0 : *_in_pos + _total_len;
				    unsigned long count;

#if OSSDEBUG > 1
				    _Context_Element _element;

				    _element._occurrence = 0;
#endif

				    _out_data->u.sgsnMMRecord.changeLocation.count = 0;
				    count = _oss_count_setof_items(_g, _in_pos, _buf_len, _total_len);
				    if (count > 0) {
					_out_data->u.sgsnMMRecord.changeLocation.value = (struct _3GCDR_ChangeLocation *) _oss_dec_getmem(_g, sizeof(struct _3GCDR_ChangeLocation)*count, _oss_c->_buffer_provided);
				    }
#if OSSDEBUG > 1
				    _element._id = (_Context_Id *)&_context_id[16];
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
					if (_out_data->u.sgsnMMRecord.changeLocation.count >= count) _oss_dec_error(_g, TRUE, _too_long, (long)_out_data->u.sgsnMMRecord.changeLocation.count);
					if (_data_tag != 0x10) _oss_dec_error(_g, TRUE, _tag_mismatch, (long)_data_tag);
#if OSSDEBUG > 1
					_element._occurrence = _out_data->u.sgsnMMRecord.changeLocation.count + 1;
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
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.changeLocation.value[_out_data->u.sgsnMMRecord.changeLocation.count].locationAreaCode, 2, 2, _constructed);
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
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.changeLocation.value[_out_data->u.sgsnMMRecord.changeLocation.count].routingAreaCode, 2, 1, _constructed);
#if OSSDEBUG > 1
					    _oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    if (_total_len < 0 || *_in_pos < _end_pos) {
						_oss_dec_tag(_g, _in_pos, _buf_len, &_data_tag, &_constructed);
						_oss_c->_tag_decoded = TRUE;
					    }
					    if (_data_tag != 0x8002 || (_total_len >= 0 && *_in_pos >= _end_pos))
						_out_data->u.sgsnMMRecord.changeLocation.value[_out_data->u.sgsnMMRecord.changeLocation.count].bit_mask &= ~_3GCDR_cellId_present;
					    else {
						_out_data->u.sgsnMMRecord.changeLocation.value[_out_data->u.sgsnMMRecord.changeLocation.count].bit_mask |= _3GCDR_cellId_present;
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[59];
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
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.changeLocation.value[_out_data->u.sgsnMMRecord.changeLocation.count].cellId, 2, 2, _constructed);
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }

#if OSSDEBUG > 1
					    _element._id = (_Context_Id *)&_context_id[58];
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
					    _oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.changeLocation.value[_out_data->u.sgsnMMRecord.changeLocation.count].changeTime, 2, 9, _constructed);
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

					_out_data->u.sgsnMMRecord.changeLocation.count++;
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
				_element._id = (_Context_Id *)&_context_id[91];
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
				_element._id = (_Context_Id *)&_context_id[90];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_duration_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_duration_present;
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
				_element._id = (_Context_Id *)&_context_id[89];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_sgsnChange_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_sgsnChange_present;
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
				_element._id = (_Context_Id *)&_context_id[70];
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
				_element._id = (_Context_Id *)&_context_id[50];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_diagnostics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_diagnostics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnMMRecord.diagnostics);
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
				_element._id = (_Context_Id *)&_context_id[88];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_recordSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_recordSequenceNumber_present;
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
				_element._id = (_Context_Id *)&_context_id[87];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_nodeID_present;
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
				_element._id = (_Context_Id *)&_context_id[86];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d__3GCDR_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnMMRecord.recordExtensions);
				_present_flags |= 0x10000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x11: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[85];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_localSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnMMRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x20000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x12: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[84];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_servedMSISDN_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.servedMSISDN, 2, 9, _constructed);
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

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_chargingCharacteristics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_chargingCharacteristics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.chargingCharacteristics, 2, 2, _constructed);
				_present_flags |= 0x80000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x14: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[81];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_cAMELInformationMM_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_cAMELInformationMM_present;
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

						if (_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_sCFAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask |= _3GCDR_CAMELInformationMM_sCFAddress_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.cAMELInformationMM.sCFAddress, 2, 20, _constructed);
						_present_flags |= 0x1;
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

						if (_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_serviceKey_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask |= _3GCDR_CAMELInformationMM_serviceKey_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnMMRecord.cAMELInformationMM.serviceKey = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
						_present_flags |= 0x2;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x3: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[19];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_defaultTransactionHandling_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask |= _3GCDR_CAMELInformationMM_defaultTransactionHandling_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnMMRecord.cAMELInformationMM.defaultTransactionHandling = (enum _3GCDR_DefaultGPRS_Handling) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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

						if (_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_numberOfDPEncountered_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask |= _3GCDR_CAMELInformationMM_numberOfDPEncountered_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnMMRecord.cAMELInformationMM.numberOfDPEncountered = _oss_dec_iint(_g, _in_pos, _buf_len, _data_length);
						_present_flags |= 0x8;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x5: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[83];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_levelOfCAMELService_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask |= _3GCDR_CAMELInformationMM_levelOfCAMELService_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_pbit(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.cAMELInformationMM.levelOfCAMELService, 3, _constructed, 1);
						_present_flags |= 0x10;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x6: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[23];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_freeFormatData_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask |= _3GCDR_CAMELInformationMM_freeFormatData_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnMMRecord.cAMELInformationMM.freeFormatData, 2, 160, _constructed);
						_present_flags |= 0x20;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x7: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[82];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_fFDAppendIndicator_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask |= _3GCDR_CAMELInformationMM_fFDAppendIndicator_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnMMRecord.cAMELInformationMM.fFDAppendIndicator = _oss_dec_bool(_g, _in_pos, _buf_len, _data_length);
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
					_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~_3GCDR_CAMELInformationMM_sCFAddress_present;
				    }
				    if (!(_present_flags & 0x2)) {
					_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~_3GCDR_CAMELInformationMM_serviceKey_present;
				    }
				    if (!(_present_flags & 0x4)) {
					_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~_3GCDR_CAMELInformationMM_defaultTransactionHandling_present;
				    }
				    if (!(_present_flags & 0x8)) {
					_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~_3GCDR_CAMELInformationMM_numberOfDPEncountered_present;
				    }
				    if (!(_present_flags & 0x10)) {
					_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~_3GCDR_CAMELInformationMM_levelOfCAMELService_present;
				    }
				    if (!(_present_flags & 0x20)) {
					_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~_3GCDR_CAMELInformationMM_freeFormatData_present;
				    }
				    if (!(_present_flags & 0x40)) {
					_out_data->u.sgsnMMRecord.cAMELInformationMM.bit_mask &= ~_3GCDR_CAMELInformationMM_fFDAppendIndicator_present;
				    }
				}

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

				if (_out_data->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnMMRecord.bit_mask |= _3GCDR_SGSNMMRecord_systemType_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnMMRecord.systemType = (enum _3GCDR_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
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

		    if (!(_present_flags & 0x4)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_servedIMEI_present;
		    }
		    if (!(_present_flags & 0x10)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_msNetworkCapability_present;
		    }
		    if (!(_present_flags & 0x20)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_routingArea_present;
		    }
		    if (!(_present_flags & 0x40)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_locationAreaCode_present;
		    }
		    if (!(_present_flags & 0x80)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_cellIdentifier_present;
		    }
		    if (!(_present_flags & 0x100)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_changeLocation_present;
		    }
		    if (!(_present_flags & 0x400)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_duration_present;
		    }
		    if (!(_present_flags & 0x800)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_sgsnChange_present;
		    }
		    if (!(_present_flags & 0x2000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_diagnostics_present;
		    }
		    if (!(_present_flags & 0x4000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_recordSequenceNumber_present;
		    }
		    if (!(_present_flags & 0x8000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x10000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x20000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_localSequenceNumber_present;
		    }
		    if (!(_present_flags & 0x40000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_servedMSISDN_present;
		    }
		    if (!(_present_flags & 0x80000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_chargingCharacteristics_present;
		    }
		    if (!(_present_flags & 0x100000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_cAMELInformationMM_present;
		    }
		    if (!(_present_flags & 0x200000)) {
			_out_data->u.sgsnMMRecord.bit_mask &= ~_3GCDR_SGSNMMRecord_systemType_present;
		    }
		    if ((_present_flags & 0x120B) != 0x120B) _oss_dec_error(_g, TRUE, _field_omit, 0L);
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x17: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[94];
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
				_element._id = (_Context_Id *)&_context_id[9];
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
				_element._id = (_Context_Id *)&_context_id[1];
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
				_element._id = (_Context_Id *)&_context_id[59];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_servedIMEI_present;
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
				_element._id = (_Context_Id *)&_context_id[58];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_servedMSISDN_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.servedMSISDN, 2, 9, _constructed);
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
				_element._id = (_Context_Id *)&_context_id[23];
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
				_element._id = (_Context_Id *)&_context_id[56];
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
				_element._id = (_Context_Id *)&_context_id[31];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_locationArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_locationArea_present;
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
				_element._id = (_Context_Id *)&_context_id[55];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_routingArea_present;
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
				_element._id = (_Context_Id *)&_context_id[91];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_cellIdentifier_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_cellIdentifier_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.cellIdentifier, 2, 2, _constructed);
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
				_oss_dec_uoct(_g, _in_pos, _buf_len, _data_length, _oss_c->_buffer_provided, &_out_data->u.sgsnSMORecord.messageReference, 4, -1, _constructed);
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
				_element._id = (_Context_Id *)&_context_id[101];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_smsResult_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_smsResult_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnSMORecord.smsResult);
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
				_element._id = (_Context_Id *)&_context_id[100];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d__3GCDR_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnSMORecord.recordExtensions);
				_present_flags |= 0x2000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xE: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[99];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_nodeID_present;
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
				_element._id = (_Context_Id *)&_context_id[98];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_localSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnSMORecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x8000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x10: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[97];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_chargingCharacteristics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_chargingCharacteristics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.chargingCharacteristics, 2, 2, _constructed);
				_present_flags |= 0x10000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x11: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[96];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_SGSNSMORecord_systemType_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnSMORecord.systemType = (enum _3GCDR_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x20000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x12: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[84];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_destinationNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_destinationNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.destinationNumber, 2, 10, _constructed);
				_present_flags |= 0x40000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x13: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[95];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMORecord.bit_mask & _3GCDR_cAMELInformationSMS_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMORecord.bit_mask |= _3GCDR_cAMELInformationSMS_present;
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

						if (_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_CAMELInformationSMS_sCFAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask |= _3GCDR_CAMELInformationSMS_sCFAddress_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.cAMELInformationSMS.sCFAddress, 2, 20, _constructed);
						_present_flags |= 0x1;
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

						if (_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_CAMELInformationSMS_serviceKey_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask |= _3GCDR_CAMELInformationSMS_serviceKey_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnSMORecord.cAMELInformationSMS.serviceKey = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
						_present_flags |= 0x2;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x3: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[19];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_defaultSMSHandling_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask |= _3GCDR_defaultSMSHandling_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_out_data->u.sgsnSMORecord.cAMELInformationSMS.defaultSMSHandling = (enum _3GCDR_DefaultSMS_Handling) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
						_present_flags |= 0x4;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x4: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[58];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_cAMELCallingPartyNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask |= _3GCDR_cAMELCallingPartyNumber_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.cAMELInformationSMS.cAMELCallingPartyNumber, 2, 10, _constructed);
						_present_flags |= 0x8;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x5: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[93];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_cAMELDestinationSubscriberNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask |= _3GCDR_cAMELDestinationSubscriberNumber_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.cAMELInformationSMS.cAMELDestinationSubscriberNumber, 2, 10, _constructed);
						_present_flags |= 0x10;
#if OSSDEBUG > 1
						_oss_c->_oss_context_anchor._last = _element._parent;
#endif

					    }   break;

					    case 0x6: {
#if OSSDEBUG > 1
						_element._id = (_Context_Id *)&_context_id[23];
						_element._parent = _oss_c->_oss_context_anchor._last;
						_oss_c->_oss_context_anchor._last = &_element;
#endif

						if (_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_cAMELSMSCAddress_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask |= _3GCDR_cAMELSMSCAddress_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.cAMELInformationSMS.cAMELSMSCAddress, 2, 20, _constructed);
						_present_flags |= 0x20;
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

						if (_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_CAMELInformationSMS_freeFormatData_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
						_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask |= _3GCDR_CAMELInformationSMS_freeFormatData_present;
						_oss_c->_tag_decoded = FALSE;
						_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
						_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMORecord.cAMELInformationSMS.freeFormatData, 2, 160, _constructed);
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
					_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask &= ~_3GCDR_CAMELInformationSMS_sCFAddress_present;
				    }
				    if (!(_present_flags & 0x2)) {
					_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask &= ~_3GCDR_CAMELInformationSMS_serviceKey_present;
				    }
				    if (!(_present_flags & 0x4)) {
					_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask &= ~_3GCDR_defaultSMSHandling_present;
				    }
				    if (!(_present_flags & 0x8)) {
					_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask &= ~_3GCDR_cAMELCallingPartyNumber_present;
				    }
				    if (!(_present_flags & 0x10)) {
					_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask &= ~_3GCDR_cAMELDestinationSubscriberNumber_present;
				    }
				    if (!(_present_flags & 0x20)) {
					_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask &= ~_3GCDR_cAMELSMSCAddress_present;
				    }
				    if (!(_present_flags & 0x40)) {
					_out_data->u.sgsnSMORecord.cAMELInformationSMS.bit_mask &= ~_3GCDR_CAMELInformationSMS_freeFormatData_present;
				    }
				}

				_present_flags |= 0x80000;
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
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_servedIMEI_present;
		    }
		    if (!(_present_flags & 0x8)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_servedMSISDN_present;
		    }
		    if (!(_present_flags & 0x80)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_locationArea_present;
		    }
		    if (!(_present_flags & 0x100)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_routingArea_present;
		    }
		    if (!(_present_flags & 0x200)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_cellIdentifier_present;
		    }
		    if (!(_present_flags & 0x1000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_smsResult_present;
		    }
		    if (!(_present_flags & 0x2000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x4000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x8000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_localSequenceNumber_present;
		    }
		    if (!(_present_flags & 0x10000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_chargingCharacteristics_present;
		    }
		    if (!(_present_flags & 0x20000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_SGSNSMORecord_systemType_present;
		    }
		    if (!(_present_flags & 0x40000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_destinationNumber_present;
		    }
		    if (!(_present_flags & 0x80000)) {
			_out_data->u.sgsnSMORecord.bit_mask &= ~_3GCDR_cAMELInformationSMS_present;
		    }
		    if ((_present_flags & 0xC73) != 0xC73) _oss_dec_error(_g, TRUE, _field_omit, 0L);
		}

#if OSSDEBUG > 1
		_oss_c->_oss_context_anchor._last = _element._parent;
#endif

	    }   break;

	    case 0x18: {
#if OSSDEBUG > 1
		_element._id = (_Context_Id *)&_context_id[103];
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
				_element._id = (_Context_Id *)&_context_id[9];
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
				_element._id = (_Context_Id *)&_context_id[1];
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
				_element._id = (_Context_Id *)&_context_id[59];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_servedIMEI_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_servedIMEI_present;
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
				_element._id = (_Context_Id *)&_context_id[58];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_servedMSISDN_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_servedMSISDN_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.servedMSISDN, 2, 9, _constructed);
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
				_element._id = (_Context_Id *)&_context_id[23];
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
				_element._id = (_Context_Id *)&_context_id[56];
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
				_element._id = (_Context_Id *)&_context_id[31];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_locationArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_locationArea_present;
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
				_element._id = (_Context_Id *)&_context_id[55];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_routingArea_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_routingArea_present;
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
				_element._id = (_Context_Id *)&_context_id[91];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_cellIdentifier_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_cellIdentifier_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.cellIdentifier, 2, 2, _constructed);
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
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.originationTime, 2, 9, _constructed);
				_present_flags |= 0x400;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xB: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[107];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_smsResult_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_smsResult_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				if (_data_length < 0) ++_indef_tags;
				_d__3GCDR_Diagnostics(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnSMTRecord.smsResult);
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
				_element._id = (_Context_Id *)&_context_id[106];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_recordExtensions_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_recordExtensions_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_d__3GCDR_ManagementExtensions(_g, _in_pos, _buf_len, _data_length, _data_tag, _constructed, &_out_data->u.sgsnSMTRecord.recordExtensions);
				_present_flags |= 0x1000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xD: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[105];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_nodeID_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_nodeID_present;
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
				_element._id = (_Context_Id *)&_context_id[88];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_localSequenceNumber_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_localSequenceNumber_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnSMTRecord.localSequenceNumber = _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x4000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0xF: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[48];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_chargingCharacteristics_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_chargingCharacteristics_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_oss_dec_voct(_g, _in_pos, _buf_len, _data_length, &_out_data->u.sgsnSMTRecord.chargingCharacteristics, 2, 2, _constructed);
				_present_flags |= 0x8000;
#if OSSDEBUG > 1
				_oss_c->_oss_context_anchor._last = _element._parent;
#endif

			    }   break;

			    case 0x10: {
#if OSSDEBUG > 1
				_element._id = (_Context_Id *)&_context_id[104];
				_element._parent = _oss_c->_oss_context_anchor._last;
				_oss_c->_oss_context_anchor._last = &_element;
#endif

				if (_out_data->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_systemType_present) _oss_dec_error(_g, TRUE, _field_repeat, 0L);	/* We already decoded this. */
				_out_data->u.sgsnSMTRecord.bit_mask |= _3GCDR_SGSNSMTRecord_systemType_present;
				_oss_c->_tag_decoded = FALSE;
				_data_length = _oss_dec_length(_g, _in_pos, _buf_len);
				_out_data->u.sgsnSMTRecord.systemType = (enum _3GCDR_SystemType) _oss_dec_uiint(_g, _in_pos, _buf_len, _data_length);
				_present_flags |= 0x10000;
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
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_servedIMEI_present;
		    }
		    if (!(_present_flags & 0x8)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_servedMSISDN_present;
		    }
		    if (!(_present_flags & 0x80)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_locationArea_present;
		    }
		    if (!(_present_flags & 0x100)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_routingArea_present;
		    }
		    if (!(_present_flags & 0x200)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_cellIdentifier_present;
		    }
		    if (!(_present_flags & 0x800)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_smsResult_present;
		    }
		    if (!(_present_flags & 0x1000)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_recordExtensions_present;
		    }
		    if (!(_present_flags & 0x2000)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_nodeID_present;
		    }
		    if (!(_present_flags & 0x4000)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_localSequenceNumber_present;
		    }
		    if (!(_present_flags & 0x8000)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_chargingCharacteristics_present;
		    }
		    if (!(_present_flags & 0x10000)) {
			_out_data->u.sgsnSMTRecord.bit_mask &= ~_3GCDR_SGSNSMTRecord_systemType_present;
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

static int DLL_ENTRY_FDEF _dmCDR3G(struct ossGlobal *_g, int *_pdunum, char **_inbuf, long *_inlen, void **_outbuf, long *_outlen, void *_ctl_tbl, unsigned _flags, char _errmsg[ERR_MSG_LENGTH])
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
    _rc = _d_3GCDR_CallEventRecord(_g, _inbuf, _inlen, _outbuf, _outlen);
    _oss_c->_oss_err_msg = NULL;
    return _rc;
}

static int _f__3GCDR_IPBinaryAddress(struct ossGlobal *_g, _3GCDR_IPBinaryAddress *_data_ptr)
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

static int _f_0_3GCDR_IPTextRepresentedAdd(struct ossGlobal *_g, _3GCDR_IPTextRepresentedAddress *_data_ptr)
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

static int _f__3GCDR_IPAddress(struct ossGlobal *_g, _3GCDR_IPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	_f__3GCDR_IPBinaryAddress(_g, &_data_ptr->u.iPBinaryAddress);
	break;

    case 2:
	_f_0_3GCDR_IPTextRepresentedAdd(_g, &_data_ptr->u.iPTextRepresentedAddress);
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f__3GCDR_PDPAddress(struct ossGlobal *_g, _3GCDR_PDPAddress *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	_f__3GCDR_IPAddress(_g, &_data_ptr->u.iPAddress);
	break;

    case 2:
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f__3GCDR_ManagementExtension(struct ossGlobal *_g, _3GCDR_ManagementExtension *_data_ptr)
{
    if (_data_ptr->identifier.value)
	(*_g->freep)(_data_ptr->identifier.value);
    if (_data_ptr->bit_mask & _3GCDR_significance_present) {
    }

    if (_data_ptr->information.value)
	(*_g->freep)(_data_ptr->information.value);
    return(PDU_FREED);
}

static int _f__3GCDR_Diagnostics(struct ossGlobal *_g, _3GCDR_Diagnostics *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	break;

    case 2:
	break;

    case 3:
	break;

    case 4:
	_f__3GCDR_ManagementExtension(_g, &_data_ptr->u.networkSpecificCause);
	break;

    case 5:
	_f__3GCDR_ManagementExtension(_g, &_data_ptr->u.manufacturerSpecificCause);
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f__3GCDR__seqof1(struct ossGlobal *_g, struct _3GCDR__seqof1 *_data_ptr)
{
    {
	unsigned long _index;
	if (_data_ptr->count) {
	    _index = _data_ptr->count;
	    do {
		_index--;
	    } while (_index > 0);
	}
	if (_data_ptr->value)
	    (*_g->freep)(_data_ptr->value);
    }

    return(PDU_FREED);
}

static int _f__3GCDR_SequenceList(struct ossGlobal *_g, _3GCDR_SequenceList *_data_ptr)
{
    _f__3GCDR_IPAddress(_g, &_data_ptr->sgsnAddress);
    if (_data_ptr->bit_mask & _3GCDR_recSequenceNumber_present) {
	_f__3GCDR__seqof1(_g, &_data_ptr->recSequenceNumber);
    }

    return(PDU_FREED);
}

static int _f__3GCDR_ManagementExtensions(struct ossGlobal *_g, _3GCDR_ManagementExtensions *_data_ptr)
{
    {
	unsigned long _index;
	if (_data_ptr->count) {
	    _index = _data_ptr->count;
	    do {
		_index--;
		_f__3GCDR_ManagementExtension(_g, &_data_ptr->value[_index]);
	    } while (_index > 0);
	}
	if (_data_ptr->value)
	    (*_g->freep)(_data_ptr->value);
    }

    return(PDU_FREED);
}

static int _f__3GCDR_GSMQoSInformation(struct ossGlobal *_g, _3GCDR_GSMQoSInformation *_data_ptr)
{
    return(PDU_FREED);
}

static int _f__3GCDR_QoSInformation(struct ossGlobal *_g, _3GCDR_QoSInformation *_data_ptr)
{
    switch (_data_ptr->choice) {
    case 1:
	_f__3GCDR_GSMQoSInformation(_g, &_data_ptr->u.gsmQosInformation);
	break;

    case 2:
	break;

    default:
	/* Ignore for now */	;
    }

    return(PDU_FREED);
}

static int _f__3GCDR_ChangeOfCharCondition(struct ossGlobal *_g, _3GCDR_ChangeOfCharCondition *_data_ptr)
{
    if (_data_ptr->bit_mask & _3GCDR_qosRequested_present) {
	_f__3GCDR_QoSInformation(_g, &_data_ptr->qosRequested);
    }

    if (_data_ptr->bit_mask & _3GCDR_qosNegotiated_present) {
	_f__3GCDR_QoSInformation(_g, &_data_ptr->qosNegotiated);
    }

    return(PDU_FREED);
}

static int _f_3GCDR_CallEventRecord(struct ossGlobal *_g, void *_data)
{
    _3GCDR_CallEventRecord	*_data_ptr;
    _data_ptr = (_3GCDR_CallEventRecord *) _data;
    switch (_data_ptr->choice) {
    case 1:
	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_networkInitiation_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_servedIMEI_present) {
	}

	_f__3GCDR_IPAddress(_g, &_data_ptr->u.sgsnPDPRecord.sgsnAddress);
	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_msNetworkCapability_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_routingArea_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_locationAreaCode_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_cellIdentifier_present) {
	}

	_f__3GCDR_IPAddress(_g, &_data_ptr->u.sgsnPDPRecord.ggsnAddressUsed);
	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_servedPDPAddress_present) {
	    _f__3GCDR_PDPAddress(_g, &_data_ptr->u.sgsnPDPRecord.servedPDPAddress);
	}

	{
	    unsigned long _index;
	    if (_data_ptr->u.sgsnPDPRecord.listOfTrafficVolumes.count) {
		_index = _data_ptr->u.sgsnPDPRecord.listOfTrafficVolumes.count;
		do {
		    _index--;
		    _f__3GCDR_ChangeOfCharCondition(_g, &_data_ptr->u.sgsnPDPRecord.listOfTrafficVolumes.value[_index]);
		} while (_index > 0);
	    }
	    if (_data_ptr->u.sgsnPDPRecord.listOfTrafficVolumes.value)
		(*_g->freep)(_data_ptr->u.sgsnPDPRecord.listOfTrafficVolumes.value);
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_sgsnChange_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_diagnostics_present) {
	    _f__3GCDR_Diagnostics(_g, &_data_ptr->u.sgsnPDPRecord.diagnostics);
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_recSequenceNumList_present) {
	    _f__3GCDR_SequenceList(_g, &_data_ptr->u.sgsnPDPRecord.recSequenceNumList);
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_nodeID_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_recordExtensions_present) {
	    _f__3GCDR_ManagementExtensions(_g, &_data_ptr->u.sgsnPDPRecord.recordExtensions);
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_apnSelectionMode_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_servedMSISDN_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_chargingCharacteristics_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_SGSNPDPRecord_systemType_present) {
	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_cAMELInformationPDP_present) {
	    if (_data_ptr->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_sCFAddress_present) {
	    }

	    if (_data_ptr->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_serviceKey_present) {
	    }

	    if (_data_ptr->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_defaultTransactionHandling_present) {
	    }

	    if (_data_ptr->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_cAMELAccessPointNameNI_present) {
	    }

	    if (_data_ptr->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_cAMELAccessPointNameOI_present) {
	    }

	    if (_data_ptr->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_numberOfDPEncountered_present) {
	    }

	    if (_data_ptr->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_levelOfCAMELService_present) {
	    }

	    if (_data_ptr->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_freeFormatData_present) {
	    }

	    if (_data_ptr->u.sgsnPDPRecord.cAMELInformationPDP.bit_mask & _3GCDR_CAMELInformationPDP_fFDAppendIndicator_present) {
	    }

	}

	if (_data_ptr->u.sgsnPDPRecord.bit_mask & _3GCDR_rNCUnsentDownlinkVolume_present) {
	}

	break;

    case 2:
	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_networkInitiation_present) {
	}

	_f__3GCDR_IPAddress(_g, &_data_ptr->u.ggsnPDPRecord.ggsnAddress);
	{
	    unsigned long _index;
	    if (_data_ptr->u.ggsnPDPRecord.sgsnAddress.count) {
		_index = _data_ptr->u.ggsnPDPRecord.sgsnAddress.count;
		do {
		    _index--;
		    _f__3GCDR_IPAddress(_g, &_data_ptr->u.ggsnPDPRecord.sgsnAddress.value[_index]);
		} while (_index > 0);
	    }
	    if (_data_ptr->u.ggsnPDPRecord.sgsnAddress.value)
		(*_g->freep)(_data_ptr->u.ggsnPDPRecord.sgsnAddress.value);
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_servedPDPAddress_present) {
	    _f__3GCDR_PDPAddress(_g, &_data_ptr->u.ggsnPDPRecord.servedPDPAddress);
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_dynamicAddressFlag_present) {
	}

	{
	    unsigned long _index;
	    if (_data_ptr->u.ggsnPDPRecord.listOfTrafficVolumes.count) {
		_index = _data_ptr->u.ggsnPDPRecord.listOfTrafficVolumes.count;
		do {
		    _index--;
		    _f__3GCDR_ChangeOfCharCondition(_g, &_data_ptr->u.ggsnPDPRecord.listOfTrafficVolumes.value[_index]);
		} while (_index > 0);
	    }
	    if (_data_ptr->u.ggsnPDPRecord.listOfTrafficVolumes.value)
		(*_g->freep)(_data_ptr->u.ggsnPDPRecord.listOfTrafficVolumes.value);
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_diagnostics_present) {
	    _f__3GCDR_Diagnostics(_g, &_data_ptr->u.ggsnPDPRecord.diagnostics);
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_recSequenceNumList_present) {
	    _f__3GCDR_SequenceList(_g, &_data_ptr->u.ggsnPDPRecord.recSequenceNumList);
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_nodeID_present) {
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_recordExtensions_present) {
	    _f__3GCDR_ManagementExtensions(_g, &_data_ptr->u.ggsnPDPRecord.recordExtensions);
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_apnSelectionMode_present) {
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_servedMSISDN_present) {
	}

	if (_data_ptr->u.ggsnPDPRecord.bit_mask & _3GCDR_GGSNPDPRecord_chargingCharacteristics_present) {
	}

	break;

    case 3:
	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_servedIMEI_present) {
	}

	_f__3GCDR_IPAddress(_g, &_data_ptr->u.sgsnMMRecord.sgsnAddress);
	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_msNetworkCapability_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_routingArea_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_locationAreaCode_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_cellIdentifier_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_changeLocation_present) {
	    {
		unsigned long _index;
		if (_data_ptr->u.sgsnMMRecord.changeLocation.count) {
		    _index = _data_ptr->u.sgsnMMRecord.changeLocation.count;
		    do {
			_index--;
			if (_data_ptr->u.sgsnMMRecord.changeLocation.value[_index].bit_mask & _3GCDR_cellId_present) {
			}

		    } while (_index > 0);
		}
		if (_data_ptr->u.sgsnMMRecord.changeLocation.value)
		    (*_g->freep)(_data_ptr->u.sgsnMMRecord.changeLocation.value);
	    }

	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_duration_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_sgsnChange_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_diagnostics_present) {
	    _f__3GCDR_Diagnostics(_g, &_data_ptr->u.sgsnMMRecord.diagnostics);
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_recordSequenceNumber_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_nodeID_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_recordExtensions_present) {
	    _f__3GCDR_ManagementExtensions(_g, &_data_ptr->u.sgsnMMRecord.recordExtensions);
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_localSequenceNumber_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_servedMSISDN_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_chargingCharacteristics_present) {
	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_cAMELInformationMM_present) {
	    if (_data_ptr->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_sCFAddress_present) {
	    }

	    if (_data_ptr->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_serviceKey_present) {
	    }

	    if (_data_ptr->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_defaultTransactionHandling_present) {
	    }

	    if (_data_ptr->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_numberOfDPEncountered_present) {
	    }

	    if (_data_ptr->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_levelOfCAMELService_present) {
	    }

	    if (_data_ptr->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_freeFormatData_present) {
	    }

	    if (_data_ptr->u.sgsnMMRecord.cAMELInformationMM.bit_mask & _3GCDR_CAMELInformationMM_fFDAppendIndicator_present) {
	    }

	}

	if (_data_ptr->u.sgsnMMRecord.bit_mask & _3GCDR_SGSNMMRecord_systemType_present) {
	}

	break;

    case 4:
	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_servedIMEI_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_servedMSISDN_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_locationArea_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_routingArea_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_cellIdentifier_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.messageReference.value)
	    (*_g->freep)(_data_ptr->u.sgsnSMORecord.messageReference.value);
	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_smsResult_present) {
	    _f__3GCDR_Diagnostics(_g, &_data_ptr->u.sgsnSMORecord.smsResult);
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_recordExtensions_present) {
	    _f__3GCDR_ManagementExtensions(_g, &_data_ptr->u.sgsnSMORecord.recordExtensions);
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_nodeID_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_localSequenceNumber_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_chargingCharacteristics_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_SGSNSMORecord_systemType_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_destinationNumber_present) {
	}

	if (_data_ptr->u.sgsnSMORecord.bit_mask & _3GCDR_cAMELInformationSMS_present) {
	    if (_data_ptr->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_CAMELInformationSMS_sCFAddress_present) {
	    }

	    if (_data_ptr->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_CAMELInformationSMS_serviceKey_present) {
	    }

	    if (_data_ptr->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_defaultSMSHandling_present) {
	    }

	    if (_data_ptr->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_cAMELCallingPartyNumber_present) {
	    }

	    if (_data_ptr->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_cAMELDestinationSubscriberNumber_present) {
	    }

	    if (_data_ptr->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_cAMELSMSCAddress_present) {
	    }

	    if (_data_ptr->u.sgsnSMORecord.cAMELInformationSMS.bit_mask & _3GCDR_CAMELInformationSMS_freeFormatData_present) {
	    }

	}

	break;

    case 5:
	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_servedIMEI_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_servedMSISDN_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_locationArea_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_routingArea_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_cellIdentifier_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_smsResult_present) {
	    _f__3GCDR_Diagnostics(_g, &_data_ptr->u.sgsnSMTRecord.smsResult);
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_recordExtensions_present) {
	    _f__3GCDR_ManagementExtensions(_g, &_data_ptr->u.sgsnSMTRecord.recordExtensions);
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_nodeID_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_localSequenceNumber_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_chargingCharacteristics_present) {
	}

	if (_data_ptr->u.sgsnSMTRecord.bit_mask & _3GCDR_SGSNSMTRecord_systemType_present) {
	}

	break;

    default:
	/* Ignore for now */	;
    }

    (*_g->freep)(_data_ptr);
    return(PDU_FREED);
}

static int DLL_ENTRY_FDEF _fmCDR3G(struct ossGlobal *_g, int _pdunum, void *_data, void *_ctl_tbl)
{
    int _rc = 0;

    switch (_pdunum)
    {
	case 1:
	    _rc = _f_3GCDR_CallEventRecord(_g, _data);
	    break;

	default:  return(PDU_RANGE);
    }
    return _rc;
}

/* Entry points */
static const _entry_point_struct _entry_points = {_ossinit_CDRF_3GCDR_Final, _emCDR3G, _dmCDR3G, _fmCDR3G, 16,
 NULL, NULL, 0, 0, NULL};
static void _oss_post_init(struct ossGlobal *world) {
    static const unsigned char _oss_typeinfo[] = {
        0x00, 0x7c, 0x33, 0x36, 0xa9, 0x26, 0x06, 0xab, 0x4b, 0xd0,
        0x27, 0xa9, 0x4b, 0xd3, 0x27, 0x7b, 0xd3, 0x8b, 0xe8, 0x93,
        0xc3, 0x2c, 0x2e, 0x6f, 0xc2, 0x0f, 0x7f, 0x65, 0xf8, 0x04,
        0x7a, 0xb7, 0xd0, 0x06, 0xa1, 0x79, 0x2a, 0xb5, 0x09, 0x4a,
        0xd4, 0xbb, 0xfc, 0x7e, 0x24, 0x7a, 0x4f, 0x54, 0xf7, 0x1d,
        0xa8, 0xff, 0x12, 0xe9, 0x7c, 0x65, 0xd0, 0xea, 0xf1, 0x1c,
        0x29, 0x1d, 0xd5, 0xc7, 0x90, 0xd9, 0xdd, 0xfd, 0x8e, 0xf0,
        0x9a, 0x79, 0xe5, 0x88, 0x52, 0x4d, 0x0e, 0xd4, 0x36, 0xc7,
        0x53, 0x9f, 0x4b, 0xfb, 0x9d, 0x68, 0x13, 0xed, 0x6e, 0x99,
        0x9c, 0x54, 0x4f, 0x1c, 0x95, 0xc6, 0x5f, 0x81, 0xf4, 0xf4,
        0x8f, 0xa9, 0x16, 0xff, 0x9c, 0xb5, 0x25, 0x6f, 0xdc, 0xf5,
        0x7e, 0x1d, 0x4e, 0x69, 0x8e, 0x65, 0xe2, 0x2e, 0x2e, 0x09,
        0xc1, 0x0f, 0x76, 0xa6, 0x72, 0xdf
    };
    ossInitRootContext(world, (unsigned char *)_oss_typeinfo);
}

#ifdef _OSSGETHEADER
void *DLL_ENTRY_FDEF ossGetHeader()
{
    return (void *)&_entry_points;
}
#endif /* _OSSGETHEADER */

void *CDRF_3GCDR_Final = (void *)&_entry_points;
