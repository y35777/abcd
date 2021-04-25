
%{
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

%}

%union
{
    /* 命令行上出现的标识符/字符串的最大长度 */

    #define MAX_STRING_LEN          256

    long    intval;                    /* 整数值       */
    char    strval[MAX_STRING_LEN];    /* 字符串       */
    double  fltval;                    /* 浮点数       */
    CParameter* paraval;               /* 命令参数     */
    CParaValue* valueval;              /* 命令参数值   */
}

/* 定义终结符 */

%token  <intval> NUMBER
%token  <fltval> FLOAT
%token  <strval> STRINGS
%token  <strval> IDENT

%token  AND
%token  FROM_TO

%token  PLUS
%token  MINUS
%token  ASSIGN
%token  PLUS_PLUS
%token  MINUS_MINUS

%token  EQUAL
%token  LARGER
%token  SMALLER
%token  NOT_EQUAL
%token  LARGER_EQUAL
%token  SMALLER_EQUAL

%token  OTHER

/* 定义非终结符 */

%type   <strval>    macro_name
%type   <paraval>   parameter
%type   <paraval>   field_id
%type   <valueval>  val
%type   <valueval>  from_to
%type   <valueval>  simpleval
%type   <valueval>  bit_field


%left   error   /* error nonterminal, system nonterminal */

%start  s       /* start nonterminal */

%%              /* End of declaration section */

s   :   /* empty */
    |   macro_clause
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
    }
    | error
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
    }
    ;

macro_clause : macro_code_para':'parameters
    {
    }
    ;

macro_code_para : macro_name
    {
        #ifdef YACC_DEBUG
        printf("macro_code_para: macro_name=%s\n",$1);
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

        g_pPersistCmd = pCmdMan->GetCmd($1);
        if(g_pPersistCmd == NULL)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_UNRECOGNIZED_CMD);
            sprintf(err_msg,fmt,$1);
            g_pParse->SetErrMsg(err_msg);

            return CMD_NOT_DEFINED;
        }

        g_pParse->SetCmdCode(g_pPersistCmd->GetCmdCode(),$1);
        g_pParse->SetCmdDspName(g_pPersistCmd->GetCmdDspName());
        g_pParse->SetCmdLevel(g_pPersistCmd->GetCmdLevel());
    }
    ;

macro_name : macro_name IDENT
    {
        strcat($1 , " ");
        strcat($1 , $2);
        strcpy($$ , $1);
    }
    | IDENT
    {
        strcpy($$ , $1);
    }
    ;

parameters: /* empty */
    | parameter
    {
        /*
            把生成的CParameter对象加入g_pParse中.
        */
        g_pParse->AddPara($1);
        g_pPara = NULL;
    }
    | parameters ',' parameter
    {
        g_pParse->AddPara($3);
        g_pPara = NULL;
    }
    ;

parameter : field_id ASSIGN vals
    {
        $1->SetOperType(OT_ASSIGN);
        $$ = $1;
    }
    |   field_id EQUAL vals
    {
        $1->SetOperType(OT_EQUAL);
        $$ = $1;
    }
    |   field_id LARGER vals
    {
        $1->SetOperType(OT_LARGER);
        $$ = $1;
    }
    |   field_id SMALLER vals
    {
        $1->SetOperType(OT_SMALLER);
        $$ = $1;
    }
        field_id NOT_EQUAL vals
    {
        $1->SetOperType(OT_NOT_EQUAL);
        $$ = $1;
    }
    |   field_id LARGER_EQUAL vals
    {
        $1->SetOperType(OT_LARGER_EQUAL);
        $$ = $1;
    }
    |   field_id SMALLER_EQUAL vals
    {
        $1->SetOperType(OT_SMALLER_EQUAL);
        $$ = $1;
    }
    |   field_id PLUS_PLUS vals
    {
        $1->SetOperType(OT_ASCEND);
        $$ = $1;
    }
    |   field_id MINUS_MINUS vals
    {
        $1->SetOperType(OT_DESCEND);
        $$ = $1;
    }
    ;

field_id : IDENT
    {
        #ifdef YACC_DEBUG
        printf("field_id:IDENT=%s\n", $1);
        #endif

        /*
            分析出一个参数,根据参数码构造CParameter对象.
        */
        g_pPersistPara = g_pPersistCmd->GetPara($1);
        if(g_pPersistPara == NULL)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_UNRECOGNIZED_PARA);
            sprintf(err_msg,fmt,$1);
            g_pParse->SetErrMsg(err_msg);

            return INVALID_PARA;
        }

        /* 命令中指定了重复的参数. */
        if(g_pParse->GetPara($1))
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_REPEATED_PARA);
            sprintf(err_msg,fmt,$1);
            g_pParse->SetErrMsg(err_msg);

            return REPEATED_PARA;
        }

        g_pPara = new CParameter();

        g_pPara->SetParaCode(g_pPersistPara->GetParaCode());
        g_pPara->SetParaName($1);
        g_pPara->SetDataType(g_pPersistPara->GetDataType());

        /* 置子参数码为1. */
        g_u2ChildParaNo = 1;

        $$ = g_pPara;
    }
    | NUMBER
    {
        #ifdef YACC_DEBUG
        printf("field_id:NUMBER=%d\n", $1);
        #endif

        /*
            分析出一个参数,根据参数码构造CParameter对象.
        */
        g_pPersistPara = g_pPersistCmd->GetPara($1);
        if(g_pPersistPara == NULL)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%d",S_UNRECOGNIZED_PARA);
            sprintf(err_msg,fmt,$1);
            g_pParse->SetErrMsg(err_msg);

            return INVALID_PARA;
        }

        /* 命令中指定了重复的参数. */
        if(g_pParse->GetPara($1))
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%d",S_REPEATED_PARA);
            sprintf(err_msg,fmt,$1);
            g_pParse->SetErrMsg(err_msg);

            return REPEATED_PARA;
        }

        g_pPara = new CParameter();

        g_pPara->SetParaCode($1);
        g_pPara->SetParaName(g_pPersistPara->GetParaName());
        g_pPara->SetDataType(g_pPersistPara->GetDataType());

        /* 置子参数码为1. */
        g_u2ChildParaNo = 1;

        $$ = g_pPara;
    }
    ;

vals : vals AND val
    {
        /* 使子参数码递增. */
        g_u2ChildParaNo++;
    }
    |
    val
    {
        g_u2ChildParaNo++;
    }
    ;

val : from_to   /* 输入的是区间参数. */
    {
    }
    | simpleval /* 输入的是简单参数. */
    {
    }
    | bit_field /* 输入的是位域型参数 */
    {
    }
    ;

from_to : NUMBER FROM_TO NUMBER
    {
        /*
            发现区间类型参数值,缺省步长为1.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildRangeValue(g_u2ChildParaNo,
                                                $1,$3,1) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%d&&%%d",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,$1,$3);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckRangeValue($1,$3,1) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%d&&%%d",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,$1,$3);
            g_pParse->SetErrMsg(err_msg);

            delete g_pPara;
            g_pPara = NULL;
            return INVALID_PARA;
        }

        g_pValue = new CParaValue();
        g_pValue->SetValueFormat(VF_REGION);
        char sstmp[TEMP_BUF_LEN];
        sprintf(sstmp,"%d", $1);
        g_pValue->SetMinValue(sstmp);
        sprintf(sstmp,"%d", $3);
        g_pValue->SetMaxValue(sstmp);
        g_pValue->SetStep(1);

        g_pPara->AddValue(g_pValue);
        g_pValue = NULL;
    }
    | NUMBER FROM_TO NUMBER PLUS_PLUS NUMBER
    {
        /*
            发现区间类型参数值,指定步长.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildRangeValue(g_u2ChildParaNo,
                                                $1,$3,$5) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%d&&%%d++%%d",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,$1,$3,$5);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckRangeValue($1,$3,$5) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%d&&%%d++%%d",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,$1,$3,$5);
            g_pParse->SetErrMsg(err_msg);

            delete g_pPara;
            g_pPara = NULL;
            return INVALID_PARA;
        }

        g_pValue = new CParaValue();
        g_pValue->SetValueFormat(VF_REGION);
        char sstmp[TEMP_BUF_LEN];
        sprintf(sstmp,"%d", $1);
        g_pValue->SetMinValue(sstmp);
        sprintf(sstmp,"%d", $3);
        g_pValue->SetMaxValue(sstmp);
        g_pValue->SetStep($5);

        g_pPara->AddValue(g_pValue);
        g_pValue = NULL;
    }
    | STRINGS FROM_TO STRINGS PLUS_PLUS NUMBER
    {
        /*
            区间参数必须是数值型.
        */
        delete g_pPara;
        g_pPara = NULL;

        g_pParse->SetErrMsg(S_PARA_NOT_SUPPORT);
        return NOT_SUPPORT;
    }
    ;

simpleval : NUMBER
    {
        #ifdef YACC_DEBUG
        printf("val:NUMBER=%d\n", $1);
        #endif
        /*
            发现单值类型数字参数值.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */
        char sstmp[TEMP_BUF_LEN], result[TEMP_BUF_LEN];
        sprintf(sstmp,"%d", $1);

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildParaValue(g_u2ChildParaNo,
                                            sstmp,result,0) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%d",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,$1);
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
            sprintf(err_msg,fmt,$1);
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
    }
    | STRINGS
    {
        #ifdef YACC_DEBUG
        printf("val:STRING=%s\n",$1);
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
                                                $1,result,1) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%s",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,$1);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckParaValue($1,result,1) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,$1);
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
    }
    | IDENT
    {
        #ifdef YACC_DEBUG
        printf("val:IDENT=%s\n",$1);
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
                                                $1,result,2) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%s",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,$1);
                g_pParse->SetErrMsg(err_msg);

                delete g_pPara;
                g_pPara = NULL;
                return INVALID_PARA;
            }
        }
        else if(g_pPersistPara->CheckParaValue($1,result,2) == FALSE)
        {
            char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
            sprintf(fmt,"%s:%%s",S_INVALID_PARA_VALUE);
            sprintf(err_msg,fmt,$1);
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
    }
    | FLOAT
    {
        #ifdef YACC_DEBUG
        printf("val:STRING=%s\n",$1 );
        #endif
        /*
            发现单值类型浮点数参数值.
            1.若参数存在子参数,则只需要检查子参数参数是否合法.
            2.否则检查参数自己是否合法.
        */
        char sstmp[TEMP_BUF_LEN], result[TEMP_BUF_LEN];
        sprintf(sstmp,"%f",$1);

        if(g_pPersistPara->GetChildParaNum() > 0)
        {
            if(g_pPersistPara->CheckChildParaValue(g_u2ChildParaNo,
                                            sstmp,result,3) == FALSE)
            {
                char fmt[TEMP_BUF_LEN],err_msg[TEMP_BUF_LEN];
                sprintf(fmt,"%s:%%f",S_INVALID_PARA_VALUE);
                sprintf(err_msg,fmt,$1);
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
            sprintf(err_msg,fmt,$1);
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
    }

bit_field : IDENT '-' NUMBER
    {
        #ifdef YACC_DEBUG
        printf("val:BITS=%s-%d\n", $1, $3);
        #endif
         /*
            发现位域类型参数,位域类型参数不认为是多个子参数.
            位域参数的多个值分析的结果是多个CParaValue.

            1.命令行上输入的位域参数是如下形式:位域名-1,
            2.经过参数检查后输出如下形式:位域号-1.
        */
        char sstmp[TEMP_BUF_LEN],result[TEMP_BUF_LEN];
        sprintf(sstmp,"%s-%d",$1,$3);

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
    }
    ;

%%

void yyerror(YYCONST char *s)
{
  printf("err=%s\n",s);
}

