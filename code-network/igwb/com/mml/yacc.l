%{
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "y.tab.h"
 #include "l_input.h"
 
 void upcase(char *s);
%}

DIGIT       [0-9]
NUMBER      {DIGIT}+
IDENT       [_a-zA-Z][_a-zA-Z0-9]*
ANYCHAR     [^"]
STRINGS     \"({ANYCHAR}|\\\")*\"
COMMENT     "/*"

%%

{NUMBER}        {
            #ifdef  LEX_DEBUG
            printf("Number: %s \n", yytext, atoi(yytext) );
            #endif
            yylval.intval = atoi(yytext);
            return( NUMBER );
        }

{DIGIT}+"."{DIGIT}* {
            #ifdef  LEX_DEBUG
            printf("FLOAT: %s \n", yytext );
            #endif
            yylval.fltval = atof(yytext);
            return( FLOAT );
        }

{IDENT}     {
            #ifdef  LEX_DEBUG
            printf("An identifier: %s\n", yytext);
            #endif

            /*
                命令行上输入的标识符长度超过最大值MAX_STRING_LEN - 1,
                截断为MAX_STRING_LEN - 1
            */
            if(strlen(yytext) > MAX_STRING_LEN - 1)
            {
                memcpy(yylval.strval,yytext,MAX_STRING_LEN - 1);
                yylval.strval[MAX_STRING_LEN - 1] = 0;
            }
            else
            {
                strcpy( yylval.strval, yytext );
            }

            upcase( yylval.strval );
            return( IDENT );
        }

{STRINGS}   {
            #ifdef  LEX_DEBUG
            printf("An string: %s\n", yytext);
            #endif

            /*
                命令行上输入的字符串长度超过最大值MAX_STRING_LEN - 1,
                截断为MAX_STRING_LEN - 1
            */
            if(strlen(yytext) > MAX_STRING_LEN)
            {
                memcpy(yylval.strval,yytext + 1,MAX_STRING_LEN - 1);
                yylval.strval[MAX_STRING_LEN - 1] = 0;
            }
            else
            {
                strcpy( yylval.strval, yytext + 1 );
                yylval.strval[strlen(yytext)-2] = 0;
            }

            return( STRINGS );
        }

"=="        {
            #ifdef  LEX_DEBUG
            printf ( "equal %c \n", yytext[0] );
            #endif
            return( EQUAL );
        }

">"     {
            #ifdef  LEX_DEBUG
            printf ( "larger %c \n", yytext[0] );
            #endif
            return( LARGER );
        }

"<"     {
            #ifdef  LEX_DEBUG
            printf ( "smaller %c \n", yytext[0] );
            #endif
            return( SMALLER );
        }

">="        {
            #ifdef  LEX_DEBUG
            printf ( "larger and equal %s \n", yytext);
            #endif
            return( LARGER_EQUAL );
        }

"<="        {
            #ifdef  LEX_DEBUG
            printf ( "smaller and equal %s \n", yytext);
            #endif
            return( SMALLER_EQUAL );
        }

"<>"        {
            #ifdef  LEX_DEBUG
            printf ( "not equal %s \n", yytext);
            #endif
            return( NOT_EQUAL );
        }

"="     {
            #ifdef  LEX_DEBUG
            printf ( "equal %c \n", yytext[0] );
            #endif
            return( ASSIGN );
        }

"++"        {
            #ifdef  LEX_DEBUG
            printf ( "plus plus %s \n", yytext);
            #endif
            return( PLUS_PLUS );
        }

"--"        {
            #ifdef  LEX_DEBUG
            printf ( "minus minus %s \n", yytext);
            #endif
            return( MINUS_MINUS );
        }

"&"     {
            #ifdef  LEX_DEBUG
            printf( "and\n" );
            #endif
            return( AND );
        }

"&&"        {
            #ifdef  LEX_DEBUG
            printf( "to\n" );
            #endif
            return( FROM_TO );
        }

[ \t]+      {
            #ifdef  LEX_DEBUG
            #endif
            yytext[0] = '\0';
        }    /* eat up blank space */

[\n]        {
            #ifdef  LEX_DEBUG
            printf("return\n");
            #endif
        /* to definite error position. */
        }

{ANYCHAR}   {
            #ifdef  LEX_DEBUG
            printf( "One char %c \n", yytext[0] );
            #endif
            return( yytext[0] );
        }

{COMMENT}   {/* Comment */
            #ifdef  LEX_DEBUG
            printf("An comment: %s \n", yytext);
            #endif
            char c1 = 0, c2 = yyinput();
            bool bflag = false;
            for(;;){
                    if(c2 == EOF)
                        break;
                    if(c1 == '*' && c2 == '/')
                    {
                        bflag = true;
                        break;
                    }
                    c1 = c2;
                    c2 = yyinput();
            }
            if(!bflag)
            {
                #ifdef  LEX_DEBUG
                printf("wrong notation\n");
                #endif
                return -1;
            }
        }


%%

int yywrap()
{
    return 1;
}

void upcase(char *s)
{
   int i, len;
   len = strlen(s);
   for ( i = 0; i < len; i++ )
   {
    if ( s[i] >= 'a' && s[i] <= 'z' )
    {
       s[i] = s[i] - ('a'-'A');
    }
   }
}
