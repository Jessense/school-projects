/* Symbol Table Module */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbolTable.h"

#define HASH_SIZE 128
#define HASH_TYPE int
#define STACK_SIZE 512
#define BLOCK_SIZE 512
#define PARAMETER_SIZE 32

/* 函数参数结构体 */
typedef struct _parameter_info
{
	HASH_TYPE* hash_table;       // 参数的散列表
	struct _symbol* para_stack;  // 参数的栈式散列符号表
	unsigned int para_top;
} parameter_info;

/* 符号元素结构体 */
typedef struct _symbol
{
	const char* name;           // 符号名
	symbol_type    type;           // 符号类型
	symbol_type    sub_type;       // 子类型
	unsigned int   dimension;      // 维数
	int            is_const;       // 是否常量
	int            is_reference;   // 是否引用
	parameter_info parameter_list; // 函数参数
	unsigned int   next;           // 链域
} symbol;

/* 散列表 */
HASH_TYPE hash_table[HASH_SIZE] = { 0 };

/* 符号表 */
symbol symbol_stack[STACK_SIZE];
unsigned int ss_top = 0;

/* 块索引表 */
unsigned int table_of_block[BLOCK_SIZE] = { 0 };
unsigned int tob_top = 0;

void print_symbol(symbol s);

symbol_type str2symtype(char *a)
{
	if (strcmp(a, "integer") == 0 || strcmp(a, "int") == 0)
	{
		return INT;
	}
	else if (strcmp(a, "char") == 0)
	{
		return CHAR;
	}
	else if (strcmp(a, "boolean") == 0 || strcmp(a, "bool") == 0)
	{
		return BOOL;
	}
	else if (strcmp(a, "real") == 0 || strcmp(a, "float") == 0 || strcmp(a, "double") == 0)
	{
		return FLOAT;
	}
	else if (strcmp(a, "char*") == 0) {
		return ARRAY;
	} else {
		return TYPE_ERROR;
	}
}

char *symtype2str(symbol_type a)
{
	switch (a)
	{
	case INT:
		return "int";
		break;
	case CHAR:
		return "char";
		break;
	default:
		break;
	}
	return "undefined";
}

/* 散列函数 */
int hash(const char* name)
{
	return (int)name[0];
}

/* 符号表的查找操作 */
unsigned int find_element(const char* name)
{
	unsigned int pos;
	pos = hash_table[hash(name)];
	while (strcmp(symbol_stack[pos].name, name))
	{
		pos = symbol_stack[pos].next;
	}
	return pos;
}


/* 元素在符号表的添加操作 */
STATUS add_element(const char* name, const symbol_type type, const symbol_type sub_type,
	const unsigned int dimension, const int is_const, const int is_reference)
{
	HASH_TYPE hash_value = hash(name);
	symbol sym = { name, type, sub_type, dimension, is_const, is_reference,
		{NULL, NULL, 0}, hash_table[hash_value] };

	/* 检查本定义域内是否有重复 */
	if (hash_table[hash_value] >= table_of_block[tob_top]) // 在本定义域内
	{
		for (int j = tob_top; symbol_stack[j].name != NULL; j++)
		{
			if (!strcmp(name, symbol_stack[j].name))
			{
				return FAILED;
			}
		}
	}

	/* 分配函数参数表的空间 */
	if (type == FUNCTION || type == PROCEDURE)
	{
		sym.parameter_list.hash_table = (HASH_TYPE*)malloc(HASH_SIZE * sizeof(HASH_TYPE));
		sym.parameter_list.para_stack = (symbol*)malloc(PARAMETER_SIZE * sizeof(symbol));

		/* 检查空间是否分配成功 */
		if (!sym.parameter_list.hash_table || !sym.parameter_list.para_stack)
			return FAILED;
	}

	/* 存入符号表并更新散列表 */
	symbol_stack[++ss_top] = sym;
	hash_table[hash_value] = ss_top;

	return SUCCESS;
}

/* 函数参数元素在符号表的添加操作 */
STATUS add_parameter_element(const char* function_name, const char* name, symbol_type type, symbol_type sub_type,
	unsigned int dimension, const int is_const, const int is_reference)
{
	HASH_TYPE hash_value = hash(name);

	unsigned int pos = find_element(function_name);

	/* 检查函数是否存在 */
	if (!pos || (symbol_stack[pos].type != FUNCTION && symbol_stack[pos].type != PROCEDURE))
		return FAILED;

	parameter_info * para = &symbol_stack[pos].parameter_list;

	/* 检查提供的参数是否超过上限 */
	if (para->para_top >= symbol_stack[pos].dimension)
		return FAILED;

	/* 添加到符号表中 */
	if (add_element(name, type, sub_type, dimension, is_const, is_reference) == FAILED)
		return FAILED;

	symbol sym = { name, type, sub_type, dimension, is_const, is_reference,
	{NULL, NULL, 0}, 0 };

	/* 添加到函数参数列表中 */
	sym.next = para->hash_table[hash_value];
	para->para_stack[++para->para_top] = sym;
	para->hash_table[hash_value] = para->para_top;

	return SUCCESS;
}

/* 元素在符号表的删除操作 */
STATUS delete_element(const char* name)
{
	symbol sym = symbol_stack[hash_table[hash(name)]];

	/* 释放参数表的空间 */
	if (sym.parameter_list.hash_table)
		free(sym.parameter_list.hash_table);
	if (sym.parameter_list.para_stack)
		free(sym.parameter_list.para_stack);

	/* 调整栈 */
	hash_table[hash(name)] = sym.next;
	return SUCCESS;
}

/* 符号表的定位操作 */
STATUS locate_table(void)
{
	table_of_block[++tob_top] = ss_top + 1;
	return SUCCESS;
}

/* 符号表的重定位操作 */
STATUS relocate_table(void)
{
	int next_ss_top = table_of_block[tob_top--];
	for (; ss_top > next_ss_top; ss_top--)
		delete_element(symbol_stack[ss_top].name);
	return SUCCESS;
}

/* 返回符号的类型信息 */
symbol_type get_type(const char* name)
{
	return symbol_stack[find_element(name)].type;
}

/* 返回符号的返回值类型信息 */
symbol_type get_return_type(const char* name)
{
	return symbol_stack[find_element(name)].sub_type;
}

/* 返回函数声明的形参个数 */
unsigned int get_parameter_number(const char* name)
{
	return symbol_stack[find_element(name)].dimension;
}

/* 判断一个符号是不是常量 */
int is_const(const char* name)
{
	return symbol_stack[find_element(name)].is_const;
}

/* 判断一个符号是不是引用 */
int is_reference(const char* name)
{
	return symbol_stack[find_element(name)].is_reference;
}

/* 判断一个函数参数是不是引用 */
int is_reference_parameter(const char* function_name, const char* name)
{
	unsigned int pos = find_element(function_name);

	/* 判断函数是否存在 */
	if (!pos)
		return FAILED;

	const parameter_info* ppi = &symbol_stack[pos].parameter_list;
	for (pos = ppi->hash_table[hash(name)]; pos; pos = ppi->para_stack[pos].next)
		if (!strcmp(ppi->para_stack[pos].name, name))
			return ppi->para_stack[pos].is_reference;

	return FAILED; /* 没有找到这个参数 */
}

/* 比对函数参数类型 */
STATUS check_parameter_types(const char* function_name, const int n, const symbol_type st[])
{
	unsigned int i, pos;
	parameter_info* ppi;

	pos = find_element(function_name);
	if (!pos)
		return FAILED; // 符号不存在
	if (symbol_stack[pos].dimension != n)
		return FAILED; // 维数不匹配

	ppi = &symbol_stack[pos].parameter_list;
	for (i = 1; i <= n; i++)
		if (ppi->para_stack[i].type != st[i - 1])
		{
			return FAILED;
		}

	return SUCCESS;
}

int check_element(const char* name)
{
	for (int j = table_of_block[tob_top-1]; j<=ss_top; j++)
	{
		if (!strcmp(name, symbol_stack[j].name))
		{
			return SUCCESS;
		}
	}

	if (tob_top > 2)
	{
		for (int j = 1; j < table_of_block[2]; j++)
		{
			if (!strcmp(name, symbol_stack[j].name))
			{
				return SUCCESS;
			}
		}
	}
	return FAILED;
}

void know_sub(parameter_info p)
{

	for (int i = 1; i <= p.para_top; i++)
	{
		print_symbol(symbol_stack[i]);
	}
}

void print_symbol(symbol s)
{
	printf("%s:%d %d", s.name, s.type, s.sub_type);
	if (s.type == FUNCTION || s.type == PROCEDURE)
	{
		printf("(");
		if (!s.parameter_list.para_stack)
		{
			know_sub(s.parameter_list);
		}
		printf(")");
	}
}

void know()
{
	printf("块索引表：\n");
	for (int i = 0; i < tob_top; i++)
	{
		printf("%d ", table_of_block[i]);
	}
	printf("\n符号栈:\n{\n");
	for (int i = 1; i <= ss_top; i++)
	{
		print_symbol(symbol_stack[i]);
		printf("\n");
	}
	printf("}\n");
}

int get_type_value(char* type)
{
	if (!strcmp(type, "integer"))
	{
		return INT;
	}
	else if (!strcmp(type, "char"))
	{
		return CHAR;
	}
	else if (!strcmp(type, "real"))
	{
		return FLOAT;
	}
	else if (!strcmp(type, "boolean"))
	{
		return BOOL;
	}
	else if (!strcmp(type, "array"))
	{
		return ARRAY;
	}
	else if (!strcmp(type, "procedure"))
	{
		return PROCEDURE;
	}
	else if (!strcmp(type, "function"))
	{
		return FUNCTION;
	}
	else
	{
		return TYPE_ERROR;
	}
}