/*
#define LEX_DEBUG
#define YACC_DEBUG
*/
#include "../utility/mml_para.h"
#include "mml_persist_cmd.h"
#include "mml_persist_para.h"

extern char*			g_pbuf;			/**/
extern CParseResult*	g_pParse;		/**/
extern CParameter*		g_pPara;		/**/
extern CParaValue* 		g_pValue;		/**/
extern CPersistCmd*		g_pPersistCmd;	/**/
extern CPersistPara* 	g_pPersistPara;	/**/


/*  重定义yylex()的输入  */

#define YY_INPUT(buf,result,max_size) \
	{ \
		result = strlen( g_pbuf ); \
		if ( result > max_size ) \
		{ \
           result = max_size; \
 		} \
		memcpy( buf, g_pbuf, result ); \
 		g_pbuf = g_pbuf + result; \
 	}
 	
