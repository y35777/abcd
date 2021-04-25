#include "../include/toolbox.h"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif

{
    /* �������ϳ��ֵı�ʶ��/�ַ�������󳤶� */

    #define MAX_STRING_LEN          256

    long    intval;                    /* ����ֵ       */
    char    strval[MAX_STRING_LEN];    /* �ַ���       */
    double  fltval;                    /* ������       */
    CParameter* paraval;               /* �������     */
    CParaValue* valueval;              /* �������ֵ   */
} YYSTYPE;
extern YYSTYPE yylval;
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
