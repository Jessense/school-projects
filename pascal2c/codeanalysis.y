%locations
%error-verbose
%debug

%token id
%token num
%token letter
%token digits
%token string
%token K_PROGRAM K_FUNCTION K_PROCEDURE K_ARRAY K_CONST
    K_VAR K_CASE K_BOOLEAN K_CHAR
    K_OF K_DO K_FOR K_WHILE  K_IF K_THEN
    K_ELSE K_TO K_AND K_OR K_NOT
    K_DIV K_MOD K_BEGIN K_END K_INTEGER K_REAL
    
%token O_BECOME O_PLUS O_MINUS O_TIMES O_SLASH O_LSS O_LEQ
    O_GTR O_GEQ O_NEQ O_EQL O_JIN O_EVAL
%token P_LPAREN P_RPAREN P_COMMA P_SEMICOLON P_LHUA P_RHUA P_COLON
    P_SQUO P_LFANG P_RFANG P_DOT P_DDOT 

%nonassoc LOWER_THAN_ELSE
%nonassoc K_ELSE

%code requires{
#define YYSTYPE char*
}

%code{
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<malloc.h>
#include <stdarg.h>
#include"util.h"
#include "symbolTable.h"

extern FILE* yyin;
extern FILE* yyout;
extern char* patt;
extern int yylineno;
char*  filename;
char*  sentence;
char*  content;
char*  var_name;
char*  variable_value;
char* array_num;
unsigned int error_num = 0;
char * function_type[3];
char* function_name="";
int flag=0;
int array_flag=0;
int compile_state = 1;

int function_id=0;
int var_flag=0;
int id_type;
int fun_param_num = 0;
int array_subscript;
char* const_type;
char* const_result;
char* arrary_name;


symbol_type left_type;
symbol_type right_type;

symbol_type exp_type;
symbol_type exp_left_type;
symbol_type exp_right_type;

symbol_type simexp_type;
symbol_type simexp_left_type;
symbol_type simexp_right_type;

symbol_type term_type;
symbol_type term_left_type;
symbol_type term_right_type;

symbol_type factor_type;

symbol_type variable_type;


symbol_type sym_type;
symbol_type sym_subtype;
int sym_dim;
int sym_const;
int sym_ref;

symbol_type function_return_type;

void lyyerror(YYLTYPE t, const char *s, ...)
{
    va_list ap;
    va_start(ap, s);

    if(t.first_line)
        fprintf(stderr, "%s: line %d-%d: column %d-%d: error: ", filename,
            t.first_line, t.last_line, 
            t.first_column, t.last_column);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
    error_num++;
}

void yyerror(const char* str, ...)
{
    lyyerror(yylloc, str);
	compile_state = 0;
}

int yywrap()
{
    return 1;
}

void help(const char* command_line)
{
    printf("Pascal-S Compiler 1.0.0. Build Time: %s %s\n", __DATE__, __TIME__);
    puts("This software is written by Jinbin Zhang, Shuaishuai Zhang, Junzhi Zhang.");
    putchar('\n');
    printf("Usage: %s <filename>\n", command_line);
}

struct id_node* head;
struct id_node* tmp_ptr;
struct id_node* tmp2;
struct id_node* para_ptr;
struct id_node* p1;
struct id_node* p2;

void print_head()
{
    struct id_node* a=head->next;
    while(a->next!=NULL)
    {
        printf("\n\n");
        printf("%s",a->name);
        printf("%s",a->type);
        a=a->next;

    }
}

int main(int argc, char* argv[])
{
	char target[128]="";
    if(argc == 1)
    {
        help(argv[0]);
        puts("No input file.");
        return -1;
    }

    // Call Parser
    yyin = fopen(argv[1], "r");
    if(yyin == NULL)
    {
        printf("%s: error: Can't open file.\n", argv[1]);
        exit(255);
    }

	content = "";
    yyparse();
	if(compile_state)
	{
		strcat(target,argv[1]);
		strcat(target,".c");
		yyout = fopen(target,"w");
		if(yyout == NULL)
		{
			printf("%s: error: Can't open file.\n", target);
		}
		
		print(content, yyout);
		fclose(yyin);
	}
    printf("%s Finished. %u error(s).\n", argv[1], error_num);
    return 0;
}
}

%%
programstruct:
    program_head    { 
		locate_table();
        /* programstruct  →  program_head ; program_body . */
        sentence = "#include<stdio.h>\n#include<stdlib.h>\n\n";
		content = char_strcpy(content,sentence);
    }
    P_SEMICOLON program_body P_DOT{content = char_strcpy(content,"\n}");}
    | error P_SEMICOLON program_body P_DOT
    ;
program_head:
    K_PROGRAM id{
	}
	P_LPAREN idlist P_RPAREN
    { 
        /* program_head → id ( idlist ) */
    }
	| K_PROGRAM id{
	}
    | error P_RPAREN
    ;
program_body:
    const_declarations
    var_declarations
    subprogram_declarations
    program_compound_statement
    { 
        /* program_body */
    }
    ;
idlist:
    idlist P_COMMA id 
    { 
        var_name=char_strcpy(var_name,",");
        var_name=char_strcpy(var_name,$3);
        /* idlist → idlist , id */
    }
    |
    id 
    {   
		var_name=$1;
        /* idlist → id */ 
    }
    ;
function_idlist:
    function_idlist P_COMMA id 
    { 
        p1=(struct id_node*)malloc(sizeof(struct id_node));
        p1->type="";
        p1->next=NULL;
        p1->name=$3;
        p1->id_node_id=function_id;
        tmp_ptr->next=p1;
        tmp_ptr=tmp_ptr->next;
        /* idlist → idlist , id */
    }
    |
    id 
    { 
		if(head->next==NULL)
        {
			tmp_ptr->type="";    
			tmp_ptr->name=$1;
			tmp_ptr->id_node_id=function_id;
			head->next=tmp_ptr;
        }
        else
        {
			p2=(struct id_node*)malloc(sizeof(struct id_node));
			p2->type="";
			p2->name=$1;
			p2->id_node_id=function_id;
			tmp_ptr->next=p2;
			tmp_ptr=tmp_ptr->next;
        }

        
        /* idlist → id */ 
    }
    ;
const_declarations:
    K_CONST
    {
    }
    const_declaration P_SEMICOLON
    {    
        content = char_strcpy(content,";\n");
        /* const_declarations → const const_declaration ; */
    }
    |
    {
        /* const_declarations → ε */ 
    }
    ;
const_declaration:
    const_declaration P_SEMICOLON
    {
        content=char_strcpy(content, ";");
    } 
    id O_EQL const_value
    {
        if (add_element($4, str2symtype(const_type), 0, 0, 1, 0) == FAILED) {
            yyerror(char_strcpy($4, " redeclaration！"));
        }                
        content = char_strcpy(content, "const ");
        content = char_strcpy(content, const_type);
        content = char_strcpy(content, " ");
        content = char_strcpy(content, $4);
        content = char_strcpy(content, "=");
        content = char_strcpy(content, const_result);
        const_result = NULL;
        /* const_declaration → id = const_value */ 
    }
    |
    id O_EQL const_value
    {    
        if (add_element($1, str2symtype(const_type), 0, 0, 1, 0) == FAILED) {
            yyerror(char_strcpy($1," redeclaration！"));
        }         
        content = char_strcpy(content, "const ");
        content = char_strcpy(content, const_type);
        content = char_strcpy(content, " ");
        content = char_strcpy(content, $1);
        content = char_strcpy(content, "=");
        content = char_strcpy(content, const_result);
        const_result = NULL;
        /* const_declaration → id = const_value */ 
    }
    | error P_SEMICOLON id O_EQL const_value;
const_value:
    O_PLUS num
    {
        if (strchr($2, '.')) {
            const_type = "float";
        } else {
            const_type = "int";
        }
        const_result = char_strcpy("+", $2);
        /* const_value → + num */ 
    }
    |
    O_MINUS num
    {
        if (strchr($2, '.')) {
            const_type = "float";
        } else {
            const_type = "int";
        }
        const_result = char_strcpy("-", $2);         
        /* const_value → - num */ 
    }
    |
    num
    {
        if (strchr($1, '.')) {
            const_type = "float";
        } else {
            const_type = "int";
        }
        const_result = $1;        
        /* const_value → num */ 
    }
	;
var_declarations:
    K_VAR var_declaration P_SEMICOLON
    {
		content = char_strcpy(content,";\n");
        /* var_declarations → var var_declaration ; */
    }
    |
    {
        /* var_declarations → ε")*/
	};
var_declaration:
    var_declaration P_SEMICOLON{content = char_strcpy(content,";\n");} idlist P_COLON type
    {
        char* token = strtok(var_name, ",");
        while( token != NULL )
        {
            if (add_element(token, sym_type, sym_subtype, sym_dim, 0, 0) == FAILED) {
                yyerror(char_strcpy(token, " redeclaration！"));
            }            
            content = char_strcpy(content,token);
            if (sym_type == ARRAY) {
                content = char_strcpy(content,array_num);
            }
            
            token = strtok( NULL, ",");
            if(token!=NULL)
            {
                content = char_strcpy(content,",");
            }
        }
        array_flag = 0;

        /* var_declaration → var_declaration ; idlist : type */ 
    }
    |
    idlist P_COLON type
    {

        char* token = strtok(var_name, ",");
        while( token != NULL )
        {
            if (add_element(token, sym_type, sym_subtype, sym_dim, 0, 0) == FAILED) {
                yyerror(char_strcpy(token," redeclaration！"));
            }            
            content = char_strcpy(content,token);
            if (sym_type == ARRAY) {
                content = char_strcpy(content,array_num);
            }

            token = strtok( NULL, ",");
            if(token!=NULL)
            {
                content = char_strcpy(content,",");
            }
        }

        array_flag=0;        
        /* var_declaration → idlist : type */ 
    }
    | error P_SEMICOLON idlist P_COLON type
    ;
type:
    basic_type
    {
        sym_type = str2symtype($1);
		content = char_strcpy(content,code_get_type($1));
		content = char_strcpy(content," ");
        /* type → basic_type */ 
    }
    |
    K_ARRAY
    {
 
        array_num="";/*将array_num值清空*/
        array_flag=1;
    } 
    P_LFANG period P_RFANG K_OF basic_type
    {
        sym_type = ARRAY;
        sym_subtype = str2symtype($7);        
        content = char_strcpy(content,code_get_type($7));
		content = char_strcpy(content," ");
        /* type → array [ period ] of basic_type */ 
    }
    ;
basic_type:
    K_INTEGER
    {
        /* basic_type → integer */ 
    }
    |
    K_REAL
    {
        /* basic_type → real */ 
    }
    |
    K_BOOLEAN
    {
        /* basic_type → boolean */ 
    }
    |
    K_CHAR
    {
        /* basic_type → char */ 
    }
    ;
period:
    period P_COMMA num P_DDOT num
    {
        sym_dim = atoi($5);
        array_num=char_strcpy(array_num,"[");
        array_num=char_strcpy(array_num,$5);
        array_num=char_strcpy(array_num,"]");
        /* period → period , num .. num */ 
    }
    |
    num P_DDOT num
    {
        sym_dim = atoi($3);
        array_num=char_strcpy(array_num,"[");
        array_num=char_strcpy(array_num,$3);
        array_num=char_strcpy(array_num,"]");
        /* period → num .. num */ 
    }
    ;
subprogram_declarations:
    subprogram_declarations{}
    subprogram P_SEMICOLON
    {
        
        /* subprogram_declarations → subprogram_declarations \\n subprogram ; */ 
    }
    |
    {
        /* subprogram_declarations → ε */
    }
    ;
subprogram:
    subprogram_head P_SEMICOLON {content = char_strcpy(content, "{\n");} subprogram_body
    {
        content = char_strcpy(content, "}\n");
		relocate_table();
        /* subprogram → subprogram_head ; subprogram_body */
    }
    | error P_SEMICOLON subprogram_body;
subprogram_head:
    K_PROCEDURE
    {
    head=(struct id_node*)malloc(sizeof(struct id_node));
    head->next=NULL;
    tmp_ptr=(struct id_node*)malloc(sizeof(struct id_node));
    tmp_ptr->next=NULL;
    } id {locate_table();} formal_parameter
    {   
		content = char_strcpy(content, "void");
		content = char_strcpy(content, " ");
		content = char_strcpy(content, $3);
		content = char_strcpy(content, " (");
        para_ptr=(struct id_node*)malloc(sizeof(struct id_node));
        para_ptr=head->next;

        struct id_node* temp = para_ptr;
        sym_dim = 0;
        while(temp) {
            sym_dim ++;
            temp = temp->next;
        }
        if (add_element($3, PROCEDURE, 0, sym_dim, 0, 0) == FAILED) { //将过程名添加至符号表，应该比参数先添加至符号表
            yyerror(char_strcpy($3," redeclaration！"));
        }        
        
        
        while(para_ptr!=NULL)
        {
			content = char_strcpy(content, para_ptr->type);
			content = char_strcpy(content, " ");
            if(para_ptr->var_flag==1)
            {
				content = char_strcpy(content, "*");
                if (add_parameter_element($3, para_ptr->name, str2symtype(para_ptr->type), 0, 0, 0, 1) == FAILED) {
                    yyerror(char_strcpy($3, " redeclaration！"));
                }                
            } else {
                if (add_parameter_element($3, para_ptr->name, str2symtype(para_ptr->type), 0, 0, 0, 0) == FAILED) {
                    yyerror(char_strcpy($3,"redeclaration！"));
                }              
            }
			content = char_strcpy(content, para_ptr->name);
            if(para_ptr->next!=NULL)
            {
				content = char_strcpy(content, ",");
            }
            para_ptr=para_ptr->next;
        }
		content = char_strcpy(content, " )\n");
        /* subprogram_head → procedure id formal_parameter */
    }
    |
    K_FUNCTION
    {
	locate_table();
    head=(struct id_node*)malloc(sizeof(struct id_node));
    head->next=NULL;
    tmp_ptr=(struct id_node*)malloc(sizeof(struct id_node));
    tmp_ptr->next=NULL;

    } id formal_parameter P_COLON basic_type
    {
		content = char_strcpy(content, code_get_type($6));
		content = char_strcpy(content, " ");
		content = char_strcpy(content, $3);
		content = char_strcpy(content, " (");

        para_ptr=(struct id_node*)malloc(sizeof(struct id_node));
        para_ptr=head->next;

        struct id_node* temp = para_ptr;
        sym_dim = 0;
        while(temp) {
            sym_dim ++;
            temp = temp->next;
        }
        if (add_element($3, FUNCTION, str2symtype($6), sym_dim, 0, 0) == FAILED) { //将函数名添加至符号表，应该比参数先添加至符号表
            yyerror(char_strcpy($3, " redeclaration! "));
        } 

        while(para_ptr!=NULL)
        {
			content = char_strcpy(content, para_ptr->type);
			content = char_strcpy(content, " ");        
            if(para_ptr->var_flag==1)
            {
				content = char_strcpy(content, "*");
                if (add_parameter_element($3, para_ptr->name, str2symtype(para_ptr->type), 0, 0, 0, 1) == FAILED) {
                    yyerror(char_strcpy($3, " redeclaration! "));
                }                                
            } else {
                if (add_parameter_element($3, para_ptr->name, str2symtype(para_ptr->type), 0, 0, 0, 0) == FAILED) {
                    yyerror(char_strcpy($3, " redeclaration! "));
                }                
            }
			content = char_strcpy(content, para_ptr->name);
            if(para_ptr->next!=NULL)
            {
				content = char_strcpy(content, ",");
            }
            para_ptr=para_ptr->next;
        }
		content = char_strcpy(content, " )\n");

        /* subprogram_head → function id formal_parameter : basic_type */ 
    }
    ;
formal_parameter:
    P_LPAREN parameter_list P_RPAREN
    {
        /* formal_parameter → ( parameter_list ) */
    }
    |
    {
        
        /* formal_parameter → ε */
    }
    | error P_RPAREN;
parameter_list:
    parameter_list P_SEMICOLON parameter
    {
        /* parameter_list → parameter_list ; parameter */
    }
    |
    parameter
    {
        /* parameter_list → parameter */ 
    }
    | error P_SEMICOLON parameter;
parameter:
    var_parameter
    {
        /* parameter → var_parameter */ 
    }
    |
    value_parameter
    {
        /* parameter → value_parameter */ 
    };
var_parameter:
    K_VAR{var_flag=1;} value_parameter
    {
        
        /* var_parameter → var value_parameter */ 
    }
value_parameter:
    function_idlist P_COLON basic_type
    {
        function_type[function_id]=code_get_type($3);
        tmp2=head->next;
        while(tmp2!=NULL)
        {   if(strlen(tmp2->type)==0)
            {
                tmp2->type=function_type[function_id];
                tmp2->var_flag=var_flag;
            }
            tmp2=tmp2->next;
        }
        function_id++;
        var_flag=0;
        /* value_parameter → function_idlist : basic_type */ 
    };
subprogram_body:
    const_declarations
    var_declarations
    compound_statement
    {
        /* subprogram_body */ 
    };
compound_statement:
    K_BEGIN 
    {
        content = char_strcpy(content, "{\n");
    }
    statement_list K_END
    {
        content = char_strcpy(content, "}\n");
        /* compound_statement → begin statement_list end */
    };
program_compound_statement:
    K_BEGIN
    {
		locate_table();
        content = char_strcpy(content, "int main()\n{\n");
    }
    statement_list K_END
    {
        content = char_strcpy(content, "return 0;");
        /* compound_statement → begin statement_list end */
    };
statement_list:
    statement_list P_SEMICOLON statement
    {
        /* statement_list → statement_list ; statement */
    }
    |
    statement
    {
        /* statement_list → statement */ 
    }
    | error P_SEMICOLON statement;
statement:
    variable
    {
        left_type = variable_type;
    }assignop
    {
        if(flag==1)
        {
            flag=0;
        }
        else{
			content = char_strcpy(content, "=");
        }
    } expression
    {
		content = char_strcpy(content, ";\n");
        right_type = exp_type;
        if (left_type != right_type) {
            yyerror("TypeError");
        }
        /* statement → variable assignop expression */ 
    }
    |
    procedure_call
    {
        /* statement → procedure_call */ 
        content = char_strcpy(content, ";\n");
    }
    |
    compound_statement
    {
        /* statement → compound_statement */ 
    }
    |
    K_IF{content = char_strcpy(content, "if(");} expression K_THEN{content = char_strcpy(content, ")\n");} statement{} else_part
    {
        /* statement → if expression then statement else_part */ 
    }
    |
    K_FOR{content = char_strcpy(content, "for(");} id{content = char_strcpy(content, $3);content = char_strcpy(content, "=");} assignop expression{content = char_strcpy(content, ";");} K_TO {content = char_strcpy(content, $3);content = char_strcpy(content, "<=");}expression {content = char_strcpy(content, ";");content = char_strcpy(content,$3);content = char_strcpy(content, "++)\n");}K_DO statement
    {
		content = char_strcpy(content, "\n");
        /* statement → for id assignop expression to expression do statement */ 
    }
    |
    K_WHILE {content = char_strcpy(content, "while(");} expression {content = char_strcpy(content, ")");} K_DO statement
    {
    }
    |
    {
        /* statement → ε */
    };
variable:
    id
    {
		if(!check_element($1))
		{
			yyerror(char_strcpy($1, " undeclaim"));
		} else {
            variable_type = get_type($1);
        }
        content = char_strcpy(content, $1);
		arrary_name = $1;

    }
    |
    id
    {
		if(!check_element($1))
		{
			yyerror(char_strcpy($1, " undeclaim"));
		} else if (get_type($1) == ARRAY){
            variable_type = get_return_type($1);
        } else {
            yyerror("can't get element of None-Array type");
        }
        content = char_strcpy(content, $1); 
        arrary_name = $1;       
    }
    id_varpart
    {
        /* variable → id id_varpart */ 
    };
id_varpart:
    P_LFANG{content = char_strcpy(content, "[");} array_expression_list P_RFANG
    {
        content = char_strcpy(content, "]");
		char* num = $3;
		if(get_parameter_number(arrary_name)<=strtol($3,NULL,10)&&(num[0]>='0'&&num[0]<='9'))
		{
			yyerror("index out of range");
		}
        /* id_varpart → [ expression_list ] */ 
    };
procedure_call:
    id
    {
        char *function = get_function_name($1);
		content = char_strcpy(content, function);
        /* procedure_call → id */ 
    }
    |
    id
    {
        char *function = get_function_name($1);
		content = char_strcpy(content, function);
    } P_LPAREN{
		content = char_strcpy(content,"(");
		fun_param_num = 0;
	} expression_list P_RPAREN
    {
		content = char_strcpy(content,")");
		if(check_element($1))
		{
			int dimension = get_parameter_number($1);
			if(fun_param_num < dimension)
			{
				yyerror(char_strcpy($1," too few parameters"));
			}
			else if(fun_param_num > dimension)
			{
				yyerror(char_strcpy($1," too many parameters"));
			}
		}
        /* procedure_call → id ( expression_list ) */ 
    };
else_part:
    K_ELSE{content = char_strcpy(content,"\nelse\n");} statement
    {
        /* else_part → else statement */ 
    }
    | 
    %prec LOWER_THAN_ELSE
    {
        /* else_part → ε */
    }
    ;
expression_list:
    expression_list P_COMMA{
		content = char_strcpy(content,",");
		fun_param_num += 1;
	} expression
    {
        /* expression_list → expression_list , expression */
    }
    |
    expression
    {
		fun_param_num += 1;
        /* expression_list → expression */ 
    }
    | error P_COMMA expression;
expression:
    simple_expression relop
    {
		content = char_strcpy(content,$2);
        if(strcmp($2,"=")==0)
        {
			content = char_strcpy(content,"=");
        }
        exp_left_type = simexp_type;
        
    } 
    simple_expression
    {
        /* expression → simple_expression relop simple_expression */ 
        exp_right_type = simexp_type;
        if (exp_left_type != exp_right_type) {
            yyerror("TypeError");
        } else {
            exp_type = exp_left_type;
        }
    }
    |
    simple_expression
    {
        exp_type = simexp_type;
        /* expression → simple_expression */ 
    };
simple_expression:
    simple_expression
    {
        simexp_left_type = simexp_type;
    }
    addop term
    {
        simexp_right_type = term_type;
        if (simexp_left_type != simexp_right_type) {
            yyerror("TypeError");
        } else {
            simexp_type = simexp_left_type;
        }
        /* simple_expression → simple_expression addop term */
    }
    |
    term
    {
        simexp_type = term_type;
        /* simple_expression → term */ 
    };
term:
    term
    {
        term_left_type = term_type;
    }
    mulop factor
    {
        term_right_type = factor_type;
        if (term_left_type != term_right_type) {
            yyerror("TypeError");
        } else {
            term_type = term_left_type;
        }
        /* term → term mulop factor */ 
    }
    |
    factor
    {
        term_type = factor_type;
        /* term → factor */ 
    };
factor:
    num
    {
        if (strchr($1, '.')) {
            factor_type = FLOAT;
        } else {
            factor_type = INT;
        }
        content = char_strcpy(content,$1);
        /* factor → num */ 
    }
    |
    str
    |
    variable
    {
        factor_type = variable_type;
        /* factor → variable */ 
    }
    |
    id{content = char_strcpy(content,$1);} P_LPAREN{content = char_strcpy(content,"(");} expression_list P_RPAREN
    {
        factor_type = get_return_type($1);
		content = char_strcpy(content,")");
        /* factor → id ( expression_list ) */ 
    }
    |
    P_LPAREN{content = char_strcpy(content,"(");} expression P_RPAREN{content = char_strcpy(content,")");}
    {
        factor_type = exp_type;
        /* factor → ( expression) */ 
    }
    |
    K_NOT factor
    {
		content = char_strcpy(content,"!");
		content = char_strcpy(content,$1);
        /* factor → not factor */ 
    }
    |
    uminus factor
    {
		content = char_strcpy(content,"-");
		content = char_strcpy(content,$1);
        /* factor → uminus factor */ 
    };
array_expression_list:
    array_expression_list{content = char_strcpy(content,"]");} P_COMMA{content = char_strcpy(content,"[");} array_expression
    {
        /* expression_list → expression_list , expression */
    }
    |
    array_expression
    {
        /* expression_list → expression */ 
    };
array_expression:
    array_simple_expression
    {
        /* expression → simple_expression */ 
    };
array_simple_expression:
    array_simple_expression addop array_term
    {
        /* simple_expression → simple_expression addop term */
    }
    |
    array_term
    {
        /* simple_expression → term */ 
    };
array_term:
    array_term mulop{content = char_strcpy(content,$2);} array_factor
    {
        /* term → term mulop factor */ 
    }
    |
    array_factor
    {
        /* term → factor */ 
    };
array_factor:
    num
    {  
        content = char_strcpy(content,$1);
        /* factor → num */ 
    }
    |
    variable
    {
        
        /* factor → variable */ 
    }
    |
    id{content = char_strcpy(content,$1);} P_LPAREN{content = char_strcpy(content,"(");} expression_list P_RPAREN
    {
		content = char_strcpy(content,")");
        /* factor → id ( expression_list ) */ 
    }
    |
    P_LPAREN{content = char_strcpy(content,"(");} expression_list P_RPAREN{content = char_strcpy(content,")");}
    {
        /* factor → ( expression_list ) */ 
    }
    |
    K_NOT array_factor
    {
		content = char_strcpy(content,"!");
		content = char_strcpy(content,$1);
        /* factor → not factor */ 
    }
    |
    uminus array_factor
    {
		content = char_strcpy(content,"-");
		content = char_strcpy(content,$1);
        /* factor → uminus factor */ 
    };
str:
    str string
    {
    }
    |
    string
    {
		int len = strlen($1)-2;
		char *s = (char *)malloc(len*sizeof(char));
		strncpy(s, $1+1, len-2);
		content = char_strcpy(content, "\"");
		content = char_strcpy(content, s);
		content = char_strcpy(content, "\"");
    }
    ;

assignop:
    O_BECOME;
relop:
    O_EQL  | O_NEQ | O_LSS | O_LEQ | O_GTR | O_GEQ;
addop:
    O_PLUS {content = char_strcpy(content,"+");} | O_MINUS {content = char_strcpy(content,"-");} | K_OR {content = char_strcpy(content,"||");};
mulop:
    O_TIMES {content = char_strcpy(content,"*");} | O_SLASH {content = char_strcpy(content,"/");} | K_DIV {content = char_strcpy(content,"/");} | K_MOD {content = char_strcpy(content,"%");} | K_AND {content = char_strcpy(content,"&&");};
uminus:
    O_MINUS;
%%
