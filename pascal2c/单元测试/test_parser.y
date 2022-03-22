%{
#define YYSTYPE char*
#include <stdio.h>
#include <stdlib.h>

#define YYERROR_VERBOSE 1

extern FILE* yyin;
extern char* patt;
extern int yylineno;
char* filename;
unsigned int error_num = 0;

/* 打开此开关可以输出调试信息 */
//int yydebug = 1; 

void yyerror(const char* str)
{
    printf("%s: Line %u: %s\n", filename, yylineno, str);
    error_num++;
}

int yywrap()
{
    return 1;
}

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        printf("Usage: %s <filename>\nNo input file.", argv[0]);
        return -1;
    }

    // Call Parser
    yyin=fopen(argv[1], "r");
    if(yyin == NULL)
    {
        puts("Can't open file.");
        return -1;
    }
    filename = argv[1];
    yyparse();
    fclose(yyin);
    printf("Finished. %u error(s).", error_num);
    return 0;
}
%}

%token id
%token num
%token letter
%token digits
%token SYM_program, SYM_function, SYM_procedure, SYM_array, SYM_const, SYM_file,
    SYM_lable, SYM_packed, SYM_var, SYM_record, SYM_set, SYM_type, SYM_case,
    SYM_of, SYM_do, SYM_for, SYM_while, SYM_repeat, SYM_until, SYM_if, SYM_then,
    SYM_else, SYM_to, SYM_downto, SYM_goto, SYM_with, SYM_and, SYM_or, SYM_not,
    SYM_div, SYM_mod, SYM_in, SYM_begin, SYM_end, SYM_nil, SYM_integer, SYM_real,
    SYM_boolean, SYM_char
%token OPR_become, OPR_plus, OPR_minus, OPR_times, OPR_slash, OPR_lss, OPR_leq,
    OPR_gtr, OPR_geq, OPR_neq, OPR_eql, OPR_jin, OPR_eval
%token BDY_lparen, BDY_rparen, BDY_comma, BDY_semicolon, BDY_lhua, BDY_rhua, BDY_colon,
    BDY_squo, BDY_lfang, BDY_rfang, BDY_dot, BDY_ddot

%nonassoc LOWER_THAN_ELSE
%nonassoc SYM_else

%%
programstruct:
    program_head BDY_semicolon program_body BDY_dot
    { 
        /* programstruct  →  program_head ; program_body . */
        /* 翻译方案 1 */ 
    }
    ;
program_head:
    SYM_program id BDY_lparen idlist BDY_rparen
    { 
        /* program_head → id ( idlist ) */
        /* 翻译方案 2 */
    }
    ;
program_body:
    const_declarations
    var_declarations
    subprogram_declarations
    compound_statement
    { 
        /* program_body */
        /* 翻译方案 3 */ 
    }
    ;
idlist:
    idlist BDY_comma id 
    { 
        /* idlist → idlist , id */
        /* 翻译方案 4 */
    }
    |
    id 
    { 
        /* idlist → id */ 
        /* 翻译方案 5 */
    }
    ;
const_declarations:
    SYM_const const_declaration BDY_semicolon
    { 
        /* const_declarations → const const_declaration ; */
        /* 翻译方案 6 */  
    }
    |
    {
        /* const_declarations → ε */ 
        /* 翻译方案 7 */
    };
const_declaration:
    const_declaration BDY_semicolon id OPR_eql const_value
    {
        /* const_declaration → const_declaration ; id = const_value */
        /* 翻译方案 8 */ 
    }
    |
    id OPR_eql const_value
    {
        /* const_declaration → id = const_value */ 
        /* 翻译方案 9 */
    }
    ;
const_value:
    OPR_plus id
    {
        /* const_value → + id */ 
        /* 翻译方案 10 */
    }
    |
    OPR_minus id
    {
        /* const_value → - id */ 
        /* 翻译方案 11 */
    }
    |
    id
    {
        /* const_value → id */ 
        /* 翻译方案 12 */
    }
    |
    OPR_plus num
    {
        /* const_value → + num */ 
        /* 翻译方案 13 */
    }
    |
    OPR_minus num
    {
        /* const_value → - num */ 
        /* 翻译方案 14 */
    }
    |
    num
    {
        /* const_value → num */ 
        /* 翻译方案 15 */
    }
    |
    BDY_squo letter BDY_squo
    {
        /* const_value → ' letter ' */ 
        /* 翻译方案 16 */
    };
var_declarations:
    SYM_var var_declaration BDY_semicolon
    {
        /* var_declarations → var var_declaration ; */
        /* 翻译方案 17 */ 
    }
    |
    {
        /* var_declarations → ε")
        /* 翻译方案 18 */
    }
    ;
var_declaration:
    var_declaration BDY_semicolon idlist BDY_colon type
    {
        /* var_declaration → var_declaration ; idlist : type */ 
        /* 翻译方案 19 */
    }
    |
    idlist BDY_colon type
    {
        /* var_declaration → idlist : type */ 
        /* 翻译方案 20 */
    }
    ;
type:
    simple_type
    {
        /* type → simple_type */ 
        /* 翻译方案 21 */
    }
    |
    SYM_array BDY_lfang period BDY_rfang SYM_of simple_type
    {
        /* type → array [ period ] of simple_type */ 
        /* 翻译方案 22 */
    }
    ;
simple_type:
    SYM_integer
    {
        /* simple_type → integer */ 
        /* 翻译方案 23 */
    }
    |
    SYM_real
    {
        /* simple_type → real */ 
        /* 翻译方案 24 */
    }
    |
    SYM_boolean
    {
        /* simple_type → boolean */ 
        /* 翻译方案 25 */
    }
    |
    SYM_char
    {
        /* simple_type → char */ 
        /* 翻译方案 26 */
    }
    ;
period:
    period BDY_comma num BDY_ddot num
    {
        /* period → period , num .. num */ 
        /* 翻译方案 27 */
    }
    |
    num BDY_ddot num
    {
        /* period → num .. num */ 
        /* 翻译方案 28 */
    }
    ;
subprogram_declarations:
    subprogram_declarations
    subprogram BDY_semicolon
    {
        /* subprogram_declarations → subprogram_declarations \\n subprogram ; */ 
        /* 翻译方案 29 */
    }
    |
    {
        /* subprogram_declarations → ε */
        /* 翻译方案 30 */
    }
    ;
subprogram:
    subprogram_head BDY_semicolon subprogram_body
    {
        /* subprogram → subprogram_head ; subprogram_body */
        /* 翻译方案 31 */ 
    }
    ;
subprogram_head:
    SYM_procedure id formal_parameter
    {
        /* subprogram_head → procedure id formal_parameter */
        /* 翻译方案 32 */ 
    }
    |
    SYM_function id formal_parameter BDY_colon simple_type
    {
        /* subprogram_head → function id formal_parameter : simple_type */ 
        /* 翻译方案 33 */
    }
    ;
formal_parameter:
    BDY_lparen parameter_list BDY_rparen
    {
        /* formal_parameter → ( parameter_list ) */
        /* 翻译方案 34 */ 
    }
    |
    {
        /* formal_parameter → ε */
        /* 翻译方案 35 */
    }
    ;
parameter_list:
    parameter_list BDY_semicolon parameter
    {
        /* parameter_list → parameter_list ; parameter */
        /* 翻译方案 36 */ 
    }
    |
    parameter
    {
        /* parameter_list → parameter */ 
        /* 翻译方案 37 */
    };
parameter:
    var_parameter
    {
        /* parameter → var_parameter */ 
        /* 翻译方案 38 */
    }
    |
    value_parameter
    {
        /* parameter → value_parameter */ 
        /* 翻译方案 39 */
    };
var_parameter:
    SYM_var value_parameter BDY_semicolon
    {
        /* var_parameter → var value_parameter ; */ 
        /* 翻译方案 40 */
    };
value_parameter:
    idlist BDY_colon simple_type
    {
        /* value_parameter → idlist : simple_type */ 
        /* 翻译方案 41 */
    };
subprogram_body:
    const_declarations
    var_declarations
    compound_statement
    {
        /* subprogram_body */ 
        /* 翻译方案 42 */
    };
compound_statement:
    SYM_begin statement_list SYM_end
    {
        /* compound_statement → begin statement_list end */
        /* 翻译方案 43 */ 
    };
statement_list:
    statement_list BDY_semicolon statement
    {
        /* statement_list → statement_list ; statement */
        /* 翻译方案 44 */ 
    }
    |
    statement
    {
        /* statement_list → statement */ 
        /* 翻译方案 45 */
    };
statement:
    variable assignop expression
    {
        /* statement → variable assignop expression */ 
        /* 翻译方案 46 */
    }
    |
    procedure_call
    {
        /* statement → procedure_call */ 
        /* 翻译方案 47 */
    }
    |
    compound_statement
    {
        /* statement → compound_statement */ 
        /* 翻译方案 48 */
    }
    |
    SYM_if expression SYM_then statement else_part
    {
        /* statement → if expression then statement else_part */ 
        /* 翻译方案 49 */
    }
    |
    SYM_for id assignop expression SYM_to expression SYM_do statement
    {
        /* statement → for id assignop expression to expression do statement */ 
        /* 翻译方案 50 */
    }
    |
    {
        /* statement → ε */
        /* 翻译方案 51 */
    };
variable:
    id id_varpart
    {
        /* variable → id id_varpart */ 
        /* 翻译方案 52 */
    };
id_varpart:
    BDY_lfang expression_list BDY_rfang
    {
        /* id_varpart → [ expression_list ] */ 
        /* 翻译方案 53 */
    }
    |
    {
        /* id_varpart → ε */
        /* 翻译方案 54 */
    };
procedure_call:
    id
    {
        /* procedure_call → id */ 
        /* 翻译方案 55 */
    }
    |
    id BDY_lparen expression_list BDY_rparen
    {
        /* procedure_call → id ( expression_list ) */ 
        /* 翻译方案 56 */
    };
else_part:
    SYM_else statement
    {
        /* else_part → else statement */ 
        /* 翻译方案 57 */
    }
    | 
    %prec LOWER_THAN_ELSE
    {
        /* else_part → ε */
        /* 翻译方案 58 */ 
    }
    ;
expression_list:
    expression_list BDY_comma expression
    {
        /* expression_list → expression_list , expression */
        /* 翻译方案 59 */ 
    }
    |
    expression
    {
        /* expression_list → expression */ 
        /* 翻译方案 60 */
    };
expression:
    simple_expression relop simple_expression
    {
        /* expression → simple_expression relop simple_expression */ 
        /* 翻译方案 61 */
    }
    |
    simple_expression
    {
        /* expression → simple_expression */ 
        /* 翻译方案 62 */
    };
simple_expression:
    simple_expression addop term
    {
        /* simple_expression → simple_expression addop term */
        /* 翻译方案 63 */ 
    }
    |
    term
    {
        /* simple_expression → term */ 
        /* 翻译方案 64 */
    };
term:
    term mulop factor
    {
        /* term → term mulop factor */ 
        /* 翻译方案 65 */
    }
    |
    factor
    {
        /* term → factor */ 
        /* 翻译方案 66 */
    };
factor:
    num
    {
        /* factor → num */ 
        /* 翻译方案 67 */
    }
    |
    variable
    {
        /* factor → variable */ 
        /* 翻译方案 68 */
    }
    |
    id BDY_lparen expression_list BDY_rparen
    {
        /* factor → id ( expression_list ) */ 
        /* 翻译方案 69 */
    }
    |
    BDY_lparen expression_list BDY_rparen
    {
        /* factor → ( expression_list ) */ 
        /* 翻译方案 70 */
    }
    |
    SYM_not factor
    {
        /* factor → not factor */ 
        /* 翻译方案 71 */
    }
    |
    uminus factor
    {
        /* factor → uminus factor */ 
        /* 翻译方案 72 */
    };
/* 额外处理 */
assignop:
    OPR_become;
relop:
    OPR_eql | OPR_neq | OPR_lss | OPR_leq | OPR_gtr | OPR_geq;
addop:
    OPR_plus | OPR_minus | SYM_or;
mulop:
    OPR_times | OPR_slash | SYM_div | SYM_mod | SYM_and;
uminus:
    OPR_minus;
%%
