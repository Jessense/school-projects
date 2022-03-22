%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int yywrap(void);

void yyerror(char*);

char keyword[35][20]={
	"program","function","procedure","array","const",
	"file","label","packed","var","record","set",
	"type","case","of","do","for","while","repeat",
	"until","if","then","else","to","downto","goto",
	"with","and","or","not","div","mod","in","begin",
	"end","nil"
	};
char* token = NULL;
char* patt = NULL;


%}

digit 	 [0-9]
letter 	 [A-Za-z]
id   	 {letter}({letter}|{digit})*
num      [0-9]*(\.[0-9]+)?
error	 [0-9]{id}

%%
[\t]		{}
[\n]	   	{}
"program"  	{token="SYM_program";patt = "program";printf("%s,%s",token,patt); 
}
"function" 	{token="SYM_function";patt = "function";printf("%s,%s",token,patt);
}	
"procedure" 	{token="SYM_procedure";patt = "procedure";printf("%s,%s",token,patt);
}
"array"  	{token="SYM_array";patt = "array";printf("%s,%s",token,patt);
}
"const"	   	{token="SYM_const";patt = "const";printf("%s,%s",token,patt);
}
"file"  	{token="SYM_file";patt = "file";printf("%s,%s",token,patt);
}
"label"  	{token="SYM_lable";patt = "lable";printf("%s,%s",token,patt);
}
"packed"  	{token="SYM_packed";patt = "packed";printf("%s,%s",token,patt);
}
"var"  		{token="SYM_var";patt = "var";printf("%s,%s",token,patt);
}
"record"  	{token="SYM_record";patt = "record";printf("%s,%s",token,patt);
}
"set"  		{token="SYM_set";patt = "set";printf("%s,%s",token,patt);
}
"type"  	{token="SYM_type";patt = "type";printf("%s,%s",token,patt);
}
"case"  	{token="SYM_case";patt = "case";printf("%s,%s",token,patt);
}
"of"  		{token="SYM_of";patt = "of";printf("%s,%s",token,patt);
}
"do"  		{token="SYM_do";patt = "do";printf("%s,%s",token,patt);
}
"for"  		{token="SYM_for";patt = "for";printf("%s,%s",token,patt);
}
"while"  	{token="SYM_while";patt = "while";printf("%s,%s",token,patt);
}
"repeat"  	{token="SYM_repeat";patt = "repeat";printf("%s,%s",token,patt);
}
"until"  	{token="SYM_until";patt = "until";printf("%s,%s",token,patt);
}
"if"  		{token="SYM_if";patt = "if";printf("%s,%s",token,patt);
}
"then"  	{token="SYM_then";patt = "then";printf("%s,%s",token,patt);
}
"else"  	{token="SYM_else";patt = "else";printf("%s,%s",token,patt);
}
"to"  		{token="SYM_to";patt = "to";printf("%s,%s",token,patt);
}
"downto"  	{token="SYM_downto";patt = "downto";printf("%s,%s",token,patt);
}
"goto"  	{token="SYM_goto";patt = "goto";printf("%s,%s",token,patt);
}
"with"  	{token="SYM_with";patt = "with";printf("%s,%s",token,patt);
}
"and"  		{token="SYM_and";patt = "and";printf("%s,%s",token,patt);
}		
"or"  		{token="SYM_or";patt = "or";printf("%s,%s",token,patt);;
}
"not"  		{token="SYM_not";patt = "not";printf("%s,%s",token,patt);
}
"div"  		{token="SYM_div";patt = "div";printf("%s,%s",token,patt);
}
"mod"  		{token="SYM_mod";patt = "mod";printf("%s,%s",token,patt);
}
"in"  		{token="SYM_in";patt = "in";printf("%s,%s",token,patt);
}
"begin"  	{token="SYM_begin";patt = "begin";printf("%s,%s",token,patt);
}
"end"  		{token="SYM_end";patt = "end";printf("%s,%s",token,patt);
}
"nil"  		{token="SYM_nil";patt = "program";printf("%s,%s",token,patt);
}
":="  	{token="OPR_become";patt = "become";printf("%s,%s",token,patt);
}
"+"  	{token="OPR_plus";patt = "plus";printf("%s,%s",token,patt);
}
"-"  	{token="OPR_minus";patt = "minus";printf("%s,%s",token,patt);
}
"*"  	{token="OPR_times";patt = "times";printf("%s,%s",token,patt);
}
"/"  	{token="OPR_slash";patt = "slash";printf("%s,%s",token,patt);
}
"<"  	{token="OPR_lss";patt = "lss";printf("%s,%s",token,patt);
}
"<="  	{token="OPR_leq";patt = "leq";printf("%s,%s",token,patt);
}
">"  	{token="OPR_gtr";patt = "gtr";printf("%s,%s",token,patt);
}
">="  	{token="OPR_geq";patt = "geq";printf("%s,%s",token,patt);
}
"!="  	{token="OPR_neq";patt = "neq";printf("%s,%s",token,patt);
}
"=="  	{token="OPR_eql";patt = "eql";printf("%s,%s",token,patt);
}
"#"  	{token="OPR_jin";patt = "jin";printf("%s,%s",token,patt);
}
"("  	{token="BDY_lparen";patt = "lparen";printf("%s,%s",token,patt);
}
")"  	{token="BDY_rparen";patt = "rparen";printf("%s,%s",token,patt);
}
","  	{token="BDY_comma";patt = "comma";printf("%s,%s",token,patt);
}

";"  	{token="BDY_semicolon";patt = "semicolon";printf("%s,%s",token,patt);
}
"{"  	{token="BDY_lhua";patt = "lhua";printf("%s,%s",token,patt);
}
"}"  	{token="BDY_rhua";patt = "rhua";printf("%s,%s",token,patt);
}
":"  	{token="BDY_colon";patt = "colon";printf("%s,%s",token,patt);
}

{id}	{
	char *temp = (char *)malloc(strlen(yytext)+1);
	strcpy(temp,yytext);
	token="id";patt = temp;printf("%s,%s",token,patt);
	}

{num} 	{
	float number =atof(yytext); token = "NUM";
	printf("%s,%f",token,number);

	}

{error} {}

%%


int yywrap(void){	
	return 1;
}

void main(){
	yyin=fopen("test.txt","r");   
        yylex(); 
    	fclose(yyin);  
    	system("PAUSE");	
}
	
