
# line 3 "yacc.y"
    /*
        YACC语法分析规则依赖于macro.ini转换为5张表的规则:

        1.日期和时间参数被认为是子参数,转换的结果是一个CPersistPara对象
          包含多个子参数对象(也是CPersistPara对象).
          这种类型的参数使用'&'分隔各个子参数进行输入.

        2.位域型被认为是多个参数值,转换的结果是一个CPersistPara对象包含
          一个枚举值的映射表.
          这种类型的参数使用'&'分隔各个参数值进行输入.

        3.其他类型的参数仅有一个参数值,转换的结果是一个CPersistPara对象.
    */

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "l_input.h"
    #include "../include/toolbox.h"
    
    /* 语法分析过程中使用的缓冲区的长度 */

    #define TEMP_BUF_LEN            1024

    /* 语法分析的错误消息 */
    /* 对应问题单D18847 2002-10-31 by ldf */
        #define S_UNRECOGNIZED_PARA     GetStringFromID(1300)
        #define S_UNRECOGNIZED_CMD      GetStringFromID(1301)
        #define S_INVALID_PARA_VALUE    GetStringFromID(1302)
        #define S_REPEATED_PARA         GetStringFromID(1303)
        #define S_TOO_MANY_VALUES       GetStringFromID(1304)
        #define S_TOO_FEW_VALUES        GetStringFromID(1305)
        #define S_NO_MUST_GIVE_PARA     GetStringFromID(1306)
        #define S_SYNTAX_ERR            GetStringFromID(1307)
        #define S_SYSTEM_NOT_READY      GetStringFromID(1308)
        #define S_PARA_NOT_SUPPORT      GetStringFromID(1309)
    /* 修改完毕 */
    /* 定义语法分析返回的错误码 */

    #define SYNTAX_ERR              1       /* 语法错误               */
    #define NO_MUST_GIVE_PARA       2       /* 必须给出的参数而没有值 */

    #define INVALID_PARA            3       /* 参数不合法             */

    #define REPEATED_PARA           4       /* 命令中有重复的参数     */
    #define TOO_MANY_PARA           5       /* 指定了过多参数值       */
    #define TOO_FEW_PARA            6       /* 指定了过少参数值       */

    #define CMD_NOT_DEFINED         7       /* 不能识别的命令字       */      
    #define NOT_SUPPORT             8       /* 不支持的参数           */
    #define SYSTEM_NOT_READY        9       /* 系统未准备好           */

    /* 定义语法分析使用的全局变量 */

    char*           g_pbuf = NULL;          /* 语法分析器的输入       */
    CParseResult*   g_pParse = NULL;        /* 语法分析的结果         */
    CParameter*     g_pPara = NULL;         /* 语法分析使用的临时变量 */
    CParaValue*     g_pValue = NULL;        /* 语法分析使用的临时变量 */
    CPersistCmd*    g_pPersistCmd = NULL;   /* 语法分析使用的临时变量 */
    CPersistPara*   g_pPersistPara = NULL;  /* 语法分析使用的临时变量 */
    UINT2           g_u2ChildParaNo = 0;    /* 子参数码               */


# line 67 "yacc.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif

{
    /* 命令行上出现的标识符/字符串的最大长度 */

    #define MAX_STRING_LEN          256

    long    intval;                    /* 整数值       */
    char    strval[MAX_STRING_LEN];    /* 字符串       */
    double  fltval;                    /* 浮点数       */
    CParameter* paraval;               /* 命令参数     */
    CParaValue* valueval;              /* 命令参数值   */
} YYSTYPE;
# define NUMBER 257
# define FLOAT 258
# define STRINGS 259
# define IDENT 260
# define AND 261
# define FROM_TO 262
# define PLUS 263
# define MINUS 264
# define ASSIGN 265
# define PLUS_PLUS 266
# define MINUS_MINUS 267
# define EQUAL 268
# define LARGER 269
# define SMALLER 270
# define NOT_EQUAL 271
# define LARGER_EQUAL 272
# define SMALLER_EQUAL 273
# define OTHER 274

//#include <inttypes.h>

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#define	YYCONST	const
#else
#include <malloc.h>
#include <memory.h>
#define	YYCONST
#endif

//#include <values.h>

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
#ifndef yyerror
#if defined(__cplusplus)
	void yyerror(YYCONST char *);
#endif
#endif
#ifndef yylex
	int yylex(void);
#endif
	int yyparse(void);
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#endif

#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 822 "yacc.y"


void yyerror(YYCONST char *s)
{
  printf("err=%s\n",s);
}

static YYCONST yytabelem yyexca[] ={
-1, 0,
	0, 1,
	-2, 0,
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 35
# define YYLAST 82
static YYCONST yytabelem yyact[]={

    15,    21,    22,    16,    17,    18,    51,    19,    20,    52,
    50,    29,    32,    30,    31,    42,    13,    41,    40,    12,
     3,     8,    47,    54,     6,    53,    48,    46,    24,     7,
    43,    11,    14,    25,    44,     9,     4,    10,     2,     1,
    28,    27,    26,     5,     0,    33,    34,    35,    36,    37,
    38,    39,    23,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    45,     0,    49,     0,     0,     0,
     0,    55 };
static YYCONST yytabelem yypact[]={

  -236,-10000000,-10000000,-10000000,   -29,  -239,-10000000,  -241,-10000000,   -12,
-10000000,  -265,-10000000,-10000000,  -241,  -246,  -246,  -246,  -246,  -246,
  -246,  -246,  -246,-10000000,  -243,-10000000,-10000000,-10000000,-10000000,  -245,
  -247,   -15,-10000000,  -243,  -243,  -243,  -243,  -243,  -243,  -243,
  -246,  -230,  -237,  -231,  -241,-10000000,  -256,  -260,-10000000,  -262,
  -232,  -234,  -246,-10000000,-10000000,  -243 };
static YYCONST yytabelem yypgo[]={

     0,    43,    37,    31,    33,    42,    41,    40,    39,    38,
    36,    35,    28,    34 };
static YYCONST yytabelem yyr1[]={

     0,     8,     8,     8,     9,    10,     1,     1,    11,    11,
    11,     2,     2,     2,    13,     2,     2,     2,     2,     2,
     3,     3,    12,    12,     4,     4,     4,     5,     5,     5,
     6,     6,     6,     6,     7 };
static YYCONST yytabelem yyr2[]={

     0,     0,     3,     3,     7,     3,     5,     3,     0,     3,
     7,     7,     7,     7,     1,    15,     7,     7,     7,     7,
     3,     3,     7,     3,     3,     3,     3,     7,    11,    11,
     3,     3,     3,     3,     7 };
static YYCONST yytabelem yychk[]={

-10000000,    -8,    -9,   256,   -10,    -1,   260,    58,   260,   -11,
    -2,    -3,   260,   257,    44,   265,   268,   269,   270,   272,
   273,   266,   267,    -2,   -12,    -4,    -5,    -6,    -7,   257,
   259,   260,   258,   -12,   -12,   -12,   -12,   -12,   -12,   -12,
   261,   262,   262,    45,   -13,    -4,   257,   259,   257,    -3,
   266,   266,   271,   257,   257,   -12 };
static YYCONST yytabelem yydef[]={

    -2,    -2,     2,     3,     0,     5,     7,     8,     6,     4,
     9,     0,    20,    21,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    10,    11,    23,    24,    25,    26,    30,
    31,    32,    33,    12,    13,    14,    16,    17,    18,    19,
     0,     0,     0,     0,     0,    22,    27,     0,    34,     0,
     0,     0,     0,    28,    29,    15 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{ char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"NUMBER",	257,
	"FLOAT",	258,
	"STRINGS",	259,
	"IDENT",	260,
	"AND",	261,
	"FROM_TO",	262,
	"PLUS",	263,
	"MINUS",	264,
	"ASSIGN",	265,
	"PLUS_PLUS",	266,
	"MINUS_MINUS",	267,
	"EQUAL",	268,
	"LARGER",	269,
	"SMALLER",	270,
	"NOT_EQUAL",	271,
	"LARGER_EQUAL",	272,
	"SMALLER_EQUAL",	273,
	"OTHER",	274,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"s : /* empty */",
	"s : macro_clause",
	"s : error",
	"macro_clause : macro_code_para ':' parameters",
	"macro_code_para : macro_name",
	"macro_name : macro_name IDENT",
	"macro_name : IDENT",
	"parameters : /* empty */",
	"parameters : parameter",
	"parameters : parameters ',' parameter",
	"parameter : field_id ASSIGN vals",
	"parameter : field_id EQUAL vals",
	"parameter : field_id LARGER vals",
	"parameter : field_id SMALLER vals",
	"parameter : field_id SMALLER vals field_id NOT_EQUAL vals",
	"parameter : field_id LARGER_EQUAL vals",
	"parameter : field_id SMALLER_EQUAL vals",
	"parameter : field_id PLUS_PLUS vals",
	"parameter : field_id MINUS_MINUS vals",
	"field_id : IDENT",
	"field_id : NUMBER",
	"vals : vals AND val",
	"vals : val",
	"val : from_to",
	"val : simpleval",
	"val : bit_field",
	"from_to : NUMBER FROM_TO NUMBER",
	"from_to : NUMBER FROM_TO NUMBER PLUS_PLUS NUMBER",
	"from_to : STRINGS FROM_TO STRINGS PLUS_PLUS NUMBER",
	"simpleval : NUMBER",
	"simpleval : STRINGS",
	"simpleval : IDENT",
	"simpleval : FLOAT",
	"bit_field : IDENT '-' NUMBER",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.16	99/01/20 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yymaxdepth * sizeof (type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			long yyps_index = (yy_ps - yys);
			long yypv_index = (yy_pv - yyv);
			long yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register YYCONST int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 2:
# line 124 "yacc.y"
{
        /*
            检查命令行参数.
            根据g_pPersistCmd遍历各持久参数信息CPersistPara:

            1.如果是必须给出的参数,则检查g_pParse对象中是否未给出,
              如果未给出则返回错误.

            2.如果是命令中未给出的非必给参数,且有缺省值,则使用缺省值.

            3.如果是命令中未给出的非必给参数,且没有缺省值,
              则保持参数为空.
        */
        int nNum = g_pPersistCmd->GetParaNum();
        for(int i = 1;i <= nNum;i++)
        {
            if(g_pParse->GetPara(i) == NULL)
            {
                g_pPersistPara = g_pPersistCmd->GetPara(i);
                if(g_pPersistPara->IsMustGive() == TRUE)
                {
                    char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                    sprintf(fmt,"%s:%%s",S_NO_MUST_GIVE_PARA);
                    sprintf(err_msg,fmt,g_pPersistPara->GetParaName());
                    g_pParse->SetErrMsg(err_msg);

                    return NO_MUST_GIVE_PARA;
                }

                if(g_pPersistPara->GetChildParaNum() > 0)
                {
                    /* 子参数不用缺省值. */
                    continue;
                }

                char* pDefVal = g_pPersistPara->GetDefaultValue();
                if(*pDefVal != 0)
                {
                    /*
                        非必选参数且有缺省值,则使用缺省参数.
                        缺省参数只能是单值,且只能是赋值运算.
                    */
                    g_pValue = new CParaValue();
                    g_pValue->SetValueFormat(VF_SINGLE);
                    g_pValue->SetMinValue(pDefVal);

                    g_pPara = new CParameter();
                    g_pPara->SetParaCode(g_pPersistPara->GetParaCode());
                    g_pPara->SetParaName(g_pPersistPara->GetParaName());
                    g_pPara->SetDataType(g_pPersistPara->GetDataType());
                    g_pPara->SetOperType(OT_ASSIGN);

                    g_pPara->AddValue(g_pValue);
                    g_pParse->AddPara(g_pPara);
                    g_pValue = NULL;
                    g_pPara = NULL;
                }
            }
            else
            {
                /*
                    检查命令中参数的数量是否匹配.
                */
                g_pPersistPara = g_pPersistCmd->GetPara(i);
                g_pPara = g_pParse->GetPara(i);

                /*  
                    位域型参数允许有任意个参数值.
                */
                if(g_pPara->GetDataType() == PT_BITFIELD)
                {
                    g_pPara = NULL;
                    continue;
                }

                if(g_pPersistPara->GetChildParaNum() == 0)
                {
                    /*
                        参数只有一个参数,且参数类型不是位域型,
                        检查参数的数目.
                    */
                    if(g_pPara->GetValueNum() > 1)
                    {
                        char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                        sprintf(fmt,"%s:%%s",S_TOO_MANY_VALUES);
                        sprintf(err_msg,fmt,g_pPara->GetParaName());
                        g_pParse->SetErrMsg(err_msg);

                        return TOO_MANY_PARA;
                    }
                }
                else
                {
                    /*
                        参数存在多个子参数,检查子参数的数目.
                    */
                    if(g_pPara->GetValueNum()
                        > g_pPersistPara->GetChildParaNum())
                    {
                        char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                        sprintf(fmt,"%s:%%s",S_TOO_MANY_VALUES);
                        sprintf(err_msg,fmt,g_pPara->GetParaName());
                        g_pParse->SetErrMsg(err_msg);

                        return TOO_MANY_PARA;
                    }
                    else if(g_pPara->GetValueNum()
                        < g_pPersistPara->GetChildParaNum())
                    {
                        char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                        sprintf(fmt,"%s:%%s",S_TOO_FEW_VALUES);
                        sprintf(err_msg,fmt,g_pPara->GetParaName());
                        g_pParse->SetErrMsg(err_msg);

                        return TOO_FEW_PARA;
                    }
                }

                /*
                    在一条完整的命令之后,可能LEX输入的缓冲区中还有数据,
                    从而导致语法错误,此时将删除g_pPara.
                    为了防止重复删除一个参数,必须把g_pPara设置为NULL.
                */
                g_pPara = NULL;
            }
        }
    } break;
case 3:
# line 252 "yacc.y"
{
        g_pParse->SetErrMsg(S_SYNTAX_ERR);

        /*
            回收语法分析过程中生成的临时变量.
            g_pParse由外部初始化和释放.
        */
        delete g_pPara;
        g_pPara = NULL;

        delete g_pValue;
        g_pValue = NULL;

        return SYNTAX_ERR;
    } break;
case 4:
# line 270 "yacc.y"
{
    } break;
case 5:
# line 275 "yacc.y"
{
        #ifdef YACC_DEBUG
        printf("macro_code_para: macro_name=%s\n",yypvt[-0].strval);
        #endif

        /*
            分析出一个命令名.
            初始化g_pPersistCmd指针,指向当前分析命令的对象.
        */
        CPersistCmdMan* pCmdMan = CPersistCmdMan::Instance();
        if(pCmdMan == NULL)
        {
            g_pParse->SetErrMsg(S_SYSTEM_NOT_READY);
            return SYSTEM_NOT_READY;
        }

        g_pPersistCmd = pCmdMan->GetCmd(yypvt[-0].strval);
        if(g_pPersistCmd == NULL)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_UNRECOGNIZED_CMD);
            sprintf(err_msg,fmt,yypvt[-0].strval);
            g_pParse->SetErrMsg(err_msg);

            return CMD_NOT_DEFINED;
        }

        g_pParse->SetCmdCode(g_pPersistCmd->GetCmdCode(),yypvt[-0].strval);
        g_pParse->SetCmdDspName(g_pPersistCmd->GetCmdDspName());
        g_pParse->SetCmdLevel(g_pPersistCmd->GetCmdLevel());
    } break;
case 6:
# line 309 "yacc.y"
{
        strcat(yypvt[-1].strval , " ");
        strcat(yypvt[-1].strval , yypvt[-0].strval);
        strcpy(yyval.strval , yypvt[-1].strval);
    } break;
case 7:
# line 315 "yacc.y"
{
        strcpy(yyval.strval , yypvt[-0].strval);
    } break;
case 9:
# line 322 "yacc.y"
{
        /*
            把生成的CParameter对象加入g_pParse中.
        */
        g_pParse->AddPara(yypvt[-0].paraval);
        g_pPara = NULL;
    } break;
case 10:
# line 330 "yacc.y"
{
        g_pParse->AddPara(yypvt[-0].paraval);
        g_pPara = NULL;
    } break;
case 11:
# line 337 "yacc.y"
{
        yypvt[-2].paraval->SetOperType(OT_ASSIGN);
        yyval.paraval = yypvt[-2].paraval;
    } break;
case 12:
# line 342 "yacc.y"
{
        yypvt[-2].paraval->SetOperType(OT_EQUAL);
        yyval.paraval = yypvt[-2].paraval;
    } break;
case 13:
# line 347 "yacc.y"
{
        yypvt[-2].paraval->SetOperType(OT_LARGER);
        yyval.paraval = yypvt[-2].paraval;
    } break;
case 14:
# line 352 "yacc.y"
{
        yypvt[-2].paraval->SetOperType(OT_SMALLER);
        yyval.paraval = yypvt[-2].paraval;
    } break;
case 15:
# line 357 "yacc.y"
{
        yypvt[-6].paraval->SetOperType(OT_NOT_EQUAL);
        yyval.paraval = yypvt[-6].paraval;
    } break;
case 16:
# line 362 "yacc.y"
{
        yypvt[-2].paraval->SetOperType(OT_LARGER_EQUAL);
        yyval.paraval = yypvt[-2].paraval;
    } break;
case 17:
# line 367 "yacc.y"
{
        yypvt[-2].paraval->SetOperType(OT_SMALLER_EQUAL);
        yyval.paraval = yypvt[-2].paraval;
    } break;
case 18:
# line 372 "yacc.y"
{
        yypvt[-2].paraval->SetOperType(OT_ASCEND);
        yyval.paraval = yypvt[-2].paraval;
    } break;
case 19:
# line 377 "yacc.y"
{
        yypvt[-2].paraval->SetOperType(OT_DESCEND);
        yyval.paraval = yypvt[-2].paraval;
    } break;
case 20:
# line 384 "yacc.y"
{
        #ifdef YACC_DEBUG
        printf("field_id:IDENT=%s\n", yypvt[-0].strval);
        #endif

        /*
            分析出一个参数,根据参数码构造CParameter对象.
        */
        g_pPersistPara = g_pPersistCmd->GetPara(yypvt[-0].strval);
        if(g_pPersistPara == NULL)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_UNRECOGNIZED_PARA);
            sprintf(err_msg,fmt,yypvt[-0].strval);
            g_pParse->SetErrMsg(err_msg);

            return INVALID_PARA;
        }

        /* 命令中指定了重复的参数. */
        if(g_pParse->GetPara(yypvt[-0].strval))
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_REPEATED_PARA);
            sprintf(err_msg,fmt,yypvt[-0].strval);
            g_pParse->SetErrMsg(err_msg);

            return REPEATED_PARA;
        }

        g_pPara = new CParameter();

        g_pPara->SetParaCode(g_pPersistPara->GetParaCode());
        g_pPara->SetParaName(yypvt[-0].strval);
        g_pPara->SetDataType(g_pPersistPara->GetDataType());

        /* 置子参数码为1. */
        g_u2ChildParaNo = 1;

        yyval.paraval = g_pPara;
    } break;
case 21:
# line 426 "yacc.y"
{
        #ifdef YACC_DEBUG
        printf("field_id:NUMBER=%d\n", yypvt[-0].intval);
        #endif

        /*
            分析出一个参数,根据参数码构造CParameter对象.
        */
        g_pPersistPara = g_pPersistCmd->GetPara(yypvt[-0].intval);
        if(g_pPersistPara == NULL)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%d",S_UNRECOGNIZED_PARA);
            sprintf(err_msg,fmt,yypvt[-0].intval);
            g_pParse->SetErrMsg(err_msg);

            return INVALID_PARA;
        }

        /* 命令中指定了重复的参数. */
        if(g_pParse->GetPara(yypvt[-0].intval))
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%d",S_REPEATED_PARA);
            sprintf(err_msg,fmt,yypvt[-0].intval);
            g_pParse->SetErrMsg(err_msg);

            return REPEATED_PARA;
        }

        g_pPara = new CParameter();

        g_pPara->SetParaCode(yypvt[-0].intval);
        g_pPara->SetParaName(g_pPersistPara->GetParaName());
        g_pPara->SetDataType(g_pPersistPara->GetDataType());

        /* 置子参数码为1. */
        g_u2ChildParaNo = 1;

        yyval.paraval = g_pPara;
    } break;
case 22:
# line 470 "yacc.y"
{
        /* 使子参数码递增. */
        g_u2ChildParaNo++;
    } break;
case 23:
# line 476 "yacc.y"
{
        g_u2ChildParaNo++;
    } break;
case 24:
# line 482 "yacc.y"
{
    } break;
case 25:
# line 485 "yacc.y"
{
    } break;
case 26:
# line 488 "yacc.y"
{
    } break;
case 27:
# line 493 "yacc.y"
{
        /*
            发现区间类型参数值,缺省步长为1.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildRangeValue(g_u2ChildParaNo,
                                                yypvt[-2].intval,yypvt[-0].intval,1) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%d&&%%d",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,yypvt[-2].intval,yypvt[-0].intval);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckRangeValue(yypvt[-2].intval,yypvt[-0].intval,1) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%d&&%%d",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,yypvt[-2].intval,yypvt[-0].intval);
            g_pParse->SetErrMsg(err_msg);

            delete g_pPara;
            g_pPara = NULL;
            return INVALID_PARA;
        }

        g_pValue = new CParaValue();
        g_pValue->SetValueFormat(VF_REGION);
        char sstmp[TEMP_BUF_LEN];
        sprintf(sstmp,"%d", yypvt[-2].intval);
        g_pValue->SetMinValue(sstmp);
        sprintf(sstmp,"%d", yypvt[-0].intval);
        g_pValue->SetMaxValue(sstmp);
        g_pValue->SetStep(1);

        g_pPara->AddValue(g_pValue);
        g_pValue = NULL;
    } break;
case 28:
# line 540 "yacc.y"
{
        /*
            发现区间类型参数值,指定步长.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildRangeValue(g_u2ChildParaNo,
                                                yypvt[-4].intval,yypvt[-2].intval,yypvt[-0].intval) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%d&&%%d++%%d",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,yypvt[-4].intval,yypvt[-2].intval,yypvt[-0].intval);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckRangeValue(yypvt[-4].intval,yypvt[-2].intval,yypvt[-0].intval) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%d&&%%d++%%d",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,yypvt[-4].intval,yypvt[-2].intval,yypvt[-0].intval);
            g_pParse->SetErrMsg(err_msg);

            delete g_pPara;
            g_pPara = NULL;
            return INVALID_PARA;
        }

        g_pValue = new CParaValue();
        g_pValue->SetValueFormat(VF_REGION);
        char sstmp[TEMP_BUF_LEN];
        sprintf(sstmp,"%d", yypvt[-4].intval);
        g_pValue->SetMinValue(sstmp);
        sprintf(sstmp,"%d", yypvt[-2].intval);
        g_pValue->SetMaxValue(sstmp);
        g_pValue->SetStep(yypvt[-0].intval);

        g_pPara->AddValue(g_pValue);
        g_pValue = NULL;
    } break;
case 29:
# line 587 "yacc.y"
{
        /*
            区间参数必须是数值型.
        */
        delete g_pPara;
        g_pPara = NULL;

        g_pParse->SetErrMsg(S_PARA_NOT_SUPPORT);
        return NOT_SUPPORT;
    } break;
case 30:
# line 600 "yacc.y"
{
        #ifdef YACC_DEBUG
        printf("val:NUMBER=%d\n", yypvt[-0].intval);
        #endif
        /*
            发现单值类型数字参数值.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */
        char sstmp[TEMP_BUF_LEN], result[TEMP_BUF_LEN];
        sprintf(sstmp,"%d", yypvt[-0].intval);

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildParaValue(g_u2ChildParaNo,
                                            sstmp,result,0) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%d",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,yypvt[-0].intval);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckParaValue(sstmp,result,0) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%d",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,yypvt[-0].intval);
            g_pParse->SetErrMsg(err_msg);

            delete g_pPara;
            g_pPara = NULL;
            return INVALID_PARA;
        }

        g_pValue = new CParaValue();
        g_pValue->SetValueFormat(VF_SINGLE);
        g_pValue->SetMinValue(result);

        g_pPara->AddValue(g_pValue);
        g_pValue = NULL;
    } break;
case 31:
# line 647 "yacc.y"
{
        #ifdef YACC_DEBUG
        printf("val:STRING=%s\n",yypvt[-0].strval);
        #endif
        /*
            发现单值类型字符串参数值.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */
        char result[TEMP_BUF_LEN];

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildParaValue(g_u2ChildParaNo,
                                                yypvt[-0].strval,result,1) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%s",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,yypvt[-0].strval);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckParaValue(yypvt[-0].strval,result,1) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,yypvt[-0].strval);
            g_pParse->SetErrMsg(err_msg);

            delete g_pPara;
            g_pPara = NULL;
            return INVALID_PARA;
        }

        g_pValue = new CParaValue();
        g_pValue->SetValueFormat(VF_SINGLE);
        g_pValue->SetMinValue(result);

        g_pPara->AddValue(g_pValue);
        g_pValue = NULL;
    } break;
case 32:
# line 693 "yacc.y"
{
        #ifdef YACC_DEBUG
        printf("val:IDENT=%s\n",yypvt[-0].strval);
        #endif
        /*
            发现单值类型字符串参数值.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */
        char result[TEMP_BUF_LEN];

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildParaValue(g_u2ChildParaNo,
                                                yypvt[-0].strval,result,2) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%s",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,yypvt[-0].strval);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckParaValue(yypvt[-0].strval,result,2) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,yypvt[-0].strval);
            g_pParse->SetErrMsg(err_msg);

            delete g_pPara;
            g_pPara = NULL;
            return INVALID_PARA;
        }

        g_pValue = new CParaValue();
        g_pValue->SetValueFormat(VF_SINGLE);
        g_pValue->SetMinValue(result);

        g_pPara->AddValue(g_pValue);
        g_pValue = NULL;
    } break;
case 33:
# line 739 "yacc.y"
{
        #ifdef YACC_DEBUG
        printf("val:STRING=%s\n",yypvt[-0].fltval );
        #endif
        /*
            发现单值类型浮点数参数值.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */
        char sstmp[TEMP_BUF_LEN], result[TEMP_BUF_LEN];
        sprintf(sstmp,"%f",yypvt[-0].fltval);

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildParaValue(g_u2ChildParaNo,
                                            sstmp,result,3) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%f",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,yypvt[-0].fltval);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckParaValue(sstmp,result,3) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%f",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,yypvt[-0].fltval);
            g_pParse->SetErrMsg(err_msg);

            delete g_pPara;
            g_pPara = NULL;
            return INVALID_PARA;
        }

        g_pValue = new CParaValue();
        g_pValue->SetValueFormat(VF_SINGLE);
        g_pValue->SetMinValue(result);

        g_pPara->AddValue(g_pValue);
        g_pValue = NULL;
    } break;
case 34:
# line 787 "yacc.y"
{
        #ifdef YACC_DEBUG
        printf("val:BITS=%s-%d\n", yypvt[-2].strval, yypvt[-0].intval);
        #endif
         /*
            发现位域类型参数,位域类型参数不认为是多个子参数.
            位域参数的多个值分析的结果是多个CParaValue.

            1.命令行上输入的位域参数是如下形式:位域名-1,
            2.经过参数检查后输出如下形式:位域号-1.
        */
        char sstmp[TEMP_BUF_LEN],result[TEMP_BUF_LEN];
        sprintf(sstmp,"%s-%d",yypvt[-2].strval,yypvt[-0].intval);

        if(g_pPersistPara->CheckParaValue(sstmp,result,4) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,sstmp);
            g_pParse->SetErrMsg(err_msg);

            delete g_pPara;
            g_pPara = NULL;
            return INVALID_PARA;
        }

        g_pValue = new CParaValue();
        g_pValue->SetValueFormat(VF_SINGLE);
        g_pValue->SetMinValue(result);

        g_pPara->AddValue(g_pValue);
        g_pValue = NULL;
    } break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

