
%{
    /*
        YACC�﷨��������������macro.iniת��Ϊ5�ű�Ĺ���:

        1.���ں�ʱ���������Ϊ���Ӳ���,ת���Ľ����һ��CPersistPara����
          ��������Ӳ�������(Ҳ��CPersistPara����).
          �������͵Ĳ���ʹ��'&'�ָ������Ӳ�����������.

        2.λ���ͱ���Ϊ�Ƕ������ֵ,ת���Ľ����һ��CPersistPara�������
          һ��ö��ֵ��ӳ���.
          �������͵Ĳ���ʹ��'&'�ָ���������ֵ��������.

        3.�������͵Ĳ�������һ������ֵ,ת���Ľ����һ��CPersistPara����.
    */

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "l_input.h"
    #include "../include/toolbox.h"
    
    /* �﷨����������ʹ�õĻ������ĳ��� */

    #define TEMP_BUF_LEN            1024

    /* �﷨�����Ĵ�����Ϣ */
    /* ��Ӧ���ⵥD18847 2002-10-31 by ldf */
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
    /* �޸���� */
    /* �����﷨�������صĴ����� */

    #define SYNTAX_ERR              1       /* �﷨����               */
    #define NO_MUST_GIVE_PARA       2       /* ��������Ĳ�����û��ֵ */

    #define INVALID_PARA            3       /* �������Ϸ�             */

    #define REPEATED_PARA           4       /* ���������ظ��Ĳ���     */
    #define TOO_MANY_PARA           5       /* ָ���˹������ֵ       */
    #define TOO_FEW_PARA            6       /* ָ���˹��ٲ���ֵ       */

    #define CMD_NOT_DEFINED         7       /* ����ʶ���������       */      
    #define NOT_SUPPORT             8       /* ��֧�ֵĲ���           */
    #define SYSTEM_NOT_READY        9       /* ϵͳδ׼����           */

    /* �����﷨����ʹ�õ�ȫ�ֱ��� */

    char*           g_pbuf = NULL;          /* �﷨������������       */
    CParseResult*   g_pParse = NULL;        /* �﷨�����Ľ��         */
    CParameter*     g_pPara = NULL;         /* �﷨����ʹ�õ���ʱ���� */
    CParaValue*     g_pValue = NULL;        /* �﷨����ʹ�õ���ʱ���� */
    CPersistCmd*    g_pPersistCmd = NULL;   /* �﷨����ʹ�õ���ʱ���� */
    CPersistPara*   g_pPersistPara = NULL;  /* �﷨����ʹ�õ���ʱ���� */
    UINT2           g_u2ChildParaNo = 0;    /* �Ӳ�����               */

%}

%union
{
    /* �������ϳ��ֵı�ʶ��/�ַ�������󳤶� */

    #define MAX_STRING_LEN          256

    long    intval;                    /* ����ֵ       */
    char    strval[MAX_STRING_LEN];    /* �ַ���       */
    double  fltval;                    /* ������       */
    CParameter* paraval;               /* �������     */
    CParaValue* valueval;              /* �������ֵ   */
}

/* �����ս�� */

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

/* ������ս�� */

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
            ��������в���.
            ����g_pPersistCmd�������־ò�����ϢCPersistPara:

            1.����Ǳ�������Ĳ���,����g_pParse�������Ƿ�δ����,
              ���δ�����򷵻ش���.

            2.�����������δ�����ķǱظ�����,����ȱʡֵ,��ʹ��ȱʡֵ.

            3.�����������δ�����ķǱظ�����,��û��ȱʡֵ,
              �򱣳ֲ���Ϊ��.
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
                    /* �Ӳ�������ȱʡֵ. */
                    continue;
                }

                char* pDefVal = g_pPersistPara->GetDefaultValue();
                if(*pDefVal != 0)
                {
                    /*
                        �Ǳ�ѡ��������ȱʡֵ,��ʹ��ȱʡ����.
                        ȱʡ����ֻ���ǵ�ֵ,��ֻ���Ǹ�ֵ����.
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
                    ��������в����������Ƿ�ƥ��.
                */
                g_pPersistPara = g_pPersistCmd->GetPara(i);
                g_pPara = g_pParse->GetPara(i);

                /*  
                    λ���Ͳ������������������ֵ.
                */
                if(g_pPara->GetDataType() == PT_BITFIELD)
                {
                    g_pPara = NULL;
                    continue;
                }

                if(g_pPersistPara->GetChildParaNum() == 0)
                {
                    /*
                        ����ֻ��һ������,�Ҳ������Ͳ���λ����,
                        ����������Ŀ.
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
                        �������ڶ���Ӳ���,����Ӳ�������Ŀ.
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
                    ��һ������������֮��,����LEX����Ļ������л�������,
                    �Ӷ������﷨����,��ʱ��ɾ��g_pPara.
                    Ϊ�˷�ֹ�ظ�ɾ��һ������,�����g_pPara����ΪNULL.
                */
                g_pPara = NULL;
            }
        }
    }
    | error
    {
        g_pParse->SetErrMsg(S_SYNTAX_ERR);

        /*
            �����﷨�������������ɵ���ʱ����.
            g_pParse���ⲿ��ʼ�����ͷ�.
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
            ������һ��������.
            ��ʼ��g_pPersistCmdָ��,ָ��ǰ��������Ķ���.
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
            �����ɵ�CParameter�������g_pParse��.
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
            ������һ������,���ݲ����빹��CParameter����.
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

        /* ������ָ�����ظ��Ĳ���. */
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

        /* ���Ӳ�����Ϊ1. */
        g_u2ChildParaNo = 1;

        $$ = g_pPara;
    }
    | NUMBER
    {
        #ifdef YACC_DEBUG
        printf("field_id:NUMBER=%d\n", $1);
        #endif

        /*
            ������һ������,���ݲ����빹��CParameter����.
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

        /* ������ָ�����ظ��Ĳ���. */
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

        /* ���Ӳ�����Ϊ1. */
        g_u2ChildParaNo = 1;

        $$ = g_pPara;
    }
    ;

vals : vals AND val
    {
        /* ʹ�Ӳ��������. */
        g_u2ChildParaNo++;
    }
    |
    val
    {
        g_u2ChildParaNo++;
    }
    ;

val : from_to   /* ��������������. */
    {
    }
    | simpleval /* ������Ǽ򵥲���. */
    {
    }
    | bit_field /* �������λ���Ͳ��� */
    {
    }
    ;

from_to : NUMBER FROM_TO NUMBER
    {
        /*
            �����������Ͳ���ֵ,ȱʡ����Ϊ1.
            1.�����������Ӳ���,��ֻ��Ҫ����Ӳ��������Ƿ�Ϸ�.
            2.����������Լ��Ƿ�Ϸ�.
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
            �����������Ͳ���ֵ,ָ������.
            1.�����������Ӳ���,��ֻ��Ҫ����Ӳ��������Ƿ�Ϸ�.
            2.����������Լ��Ƿ�Ϸ�.
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
            ���������������ֵ��.
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
            ���ֵ�ֵ�������ֲ���ֵ.
            1.�����������Ӳ���,��ֻ��Ҫ����Ӳ��������Ƿ�Ϸ�.
            2.����������Լ��Ƿ�Ϸ�.
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
            ���ֵ�ֵ�����ַ�������ֵ.
            1.�����������Ӳ���,��ֻ��Ҫ����Ӳ��������Ƿ�Ϸ�.
            2.����������Լ��Ƿ�Ϸ�.
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
            ���ֵ�ֵ�����ַ�������ֵ.
            1.�����������Ӳ���,��ֻ��Ҫ����Ӳ��������Ƿ�Ϸ�.
            2.����������Լ��Ƿ�Ϸ�.
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
            ���ֵ�ֵ���͸���������ֵ.
            1.�����������Ӳ���,��ֻ��Ҫ����Ӳ��������Ƿ�Ϸ�.
            2.����������Լ��Ƿ�Ϸ�.
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
            ����λ�����Ͳ���,λ�����Ͳ�������Ϊ�Ƕ���Ӳ���.
            λ������Ķ��ֵ�����Ľ���Ƕ��CParaValue.

            1.�������������λ�������������ʽ:λ����-1,
            2.���������������������ʽ:λ���-1.
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

