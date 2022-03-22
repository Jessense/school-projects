%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "codeAnalysis.tab.h"

char* token = NULL;
char* patt = NULL;
int   len=0,row = 1;

#define YY_USER_ACTION \
	yylloc.first_line = yylloc.last_line = yylineno; \
	yylloc.first_column = row; yylloc.last_column = row+yyleng; 

%}

digit 	 [0-9]
letter 	 [A-Za-z]
id   	 {letter}({letter}|{digit})*
num      [0-9]*(\.[0-9]+)?
string   '[^']*'
comment     \{[^}]*\}

%option yylineno

%%
[\t]		{}
[\n]	   	{row = 1;} 
[ ] 		{row++; }
"program"  	{ yylval = "program";len = yyleng;row += len;return K_PROGRAM ;
}
"function" 	{ yylval = "function";len = yyleng;row += len;return K_FUNCTION ;
}	
"procedure" 	{ yylval = "procedure";len = yyleng;row += len;return K_PROCEDURE ;
}
"array"  	{ yylval = "array";len = yyleng;row += len;return K_ARRAY ;
}
"const"	   	{ yylval = "const";len = yyleng;row += len;return K_CONST ;
}
"var"  		{ yylval = "var";len = yyleng;row += len;return K_VAR ;
}
"case"  	{ yylval = "case";len = yyleng;row += len;return K_CASE ;
}
"of"  		{ yylval = "of";len = yyleng;row += len;return K_OF ;
}
"do"  		{ yylval = "do";len = yyleng;row += len;return K_DO ;
}
"for"  		{ yylval = "for";len = yyleng;row += len;return K_FOR ;
}
"while"  	{ yylval = "while";len = yyleng;row += len;return K_WHILE ;
}
"if"  		{ yylval = "if";len = yyleng;row += len;return K_IF ;
}
"then"  	{ yylval = "then";len = yyleng;row += len;return K_THEN ;
}
"else"  	{ yylval = "else";len = yyleng;row += len;return K_ELSE ;
}
"to"  		{ yylval = "to";len = yyleng;row += len;return K_TO ;
}
"and"  		{ yylval = "and";len = yyleng;row += len;return K_AND ;
}		
"or"  		{ yylval = "or";len = yyleng;row += len;return K_OR ;
}
"not"  		{ yylval = "not";len = yyleng;row += len;return K_NOT ;
}
"div"  		{ yylval = "div";len = yyleng;row += len;return K_DIV ;
}
"mod"  		{ yylval = "mod";len = yyleng;row += len;return K_MOD ;
}
"begin"  	{ yylval = "begin";len = yyleng;row += len;return K_BEGIN ;
}
"end"  		{ yylval = "end";len = yyleng;row += len;return K_END ;
}
"integer"  	{ yylval = "integer";len = yyleng;row += len;return K_INTEGER ;
}
"boolean"  	{ yylval = "boolean";len = yyleng;row += len;return K_BOOLEAN ;
}
"char"  	{ yylval = "char";len = yyleng;row += len;return K_CHAR ;
}
"real"  	{ yylval = "real";len = yyleng;row += len;return K_REAL ;
}
":="  	{ yylval = ":=";len = yyleng;row += len;return O_BECOME ;
}
"+"  	{ yylval = "+" ;len = yyleng;row += len;return O_PLUS ;
}
"-"  	{ yylval = "-" ;len = yyleng;row += len;return O_MINUS ;
}
"*"  	{ yylval = "*" ;len = yyleng;row += len;return O_TIMES ;
}
"/"  	{ yylval = "/" ;len = yyleng;row += len;return O_SLASH ;
}
"<"  	{ yylval = "<" ;len = yyleng;row += len;return O_LSS ;
}
"<="  	{ yylval = "<=" ;len = yyleng;row += len;return O_LEQ ;
}
">"  	{ yylval = ">" ;len = yyleng;row += len;return O_GTR ;
}
">="  	{ yylval = ">=" ;len = yyleng;row += len;return O_GEQ ;
}
"<>"  	{ yylval = "<>" ;len = yyleng;row += len;return O_NEQ ;
}
"="  	{ yylval = "=" ;len = yyleng;row += len;return O_EQL ;
}
"#"  	{ yylval = "#" ;len = yyleng;row += len;return O_JIN ;
}
"("  	{ yylval = "(" ;len = yyleng;row += len;return P_LPAREN ;
}
")"  	{ yylval = ")" ;len = yyleng;row += len;return P_RPAREN ;
}
","  	{ yylval = "," ;len = yyleng;row += len;return P_COMMA ;
}
";"  	{ yylval = ";" ;len = yyleng;row += len;return P_SEMICOLON ;
}
"["  	{ yylval = "[" ;len = yyleng;row += len;return P_LFANG;
}
"]"  	{ yylval = "]" ;len = yyleng;row += len;return P_RFANG;
}
"{"  	{ yylval = "{" ;len = yyleng;row += len;return P_LHUA ;
}
"}"  	{ yylval = "}" ;len = yyleng;row += len;return P_RHUA ;
}
":"  	{ yylval = ":" ;len = yyleng;row += len;return P_COLON ;
}
"."  	{ yylval = "." ;len = yyleng;row += len;return P_DOT;
}

"'"  	{ yylval = "'" ;len = yyleng;row += len;return P_SQUO;
}
".."    { yylval = ".."  ; len = yyleng;row += len;return P_DDOT;
}



{id}	{
	char *temp = (char *)malloc(strlen(yytext)+1);
	strcpy(temp,yytext);
	yylval = temp;
	len = yyleng;
	row += len;
	return id;
	}

{num} 	{
	yylval=(char*)malloc(1000000);
	strcpy( yylval,yytext);
	len = yyleng;
	row += len;
	return num;
	}


{string} {
	char *temp = (char *)malloc(strlen(yytext)+1);
	strcpy(temp,yytext);
	yylval = temp;
	len = yyleng;
	row += len;
	return string;
}

{comment}	{
	row += strlen(yytext); 

}

%%

