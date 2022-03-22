#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"util.h"


char* char_strcpy(char* src, char* target)
{
	char* c = (char*)malloc((strlen(src) + strlen(target) + 5) * sizeof(char));
	strcpy(c, src);
	strcat(c, target);
	return c;
}

char* code_get_type(char* src)
{
	if (strcmp(src, "integer") == 0)
	{
		return "int";
	}
	if (strcmp(src, "char") == 0)
	{
		return "char";
	}
	if (strcmp(src, "boolean") == 0)
	{
		return "bool";
	}
	if (strcmp(src, "real") == 0)
	{
		return "float";
	}
}

char* get_function_name(const char* src)
{
	if (!strcmp(src, "exit"))
	{
		return "return";
	}
	else if (!strcmp(src, "write"))
	{
		return "scanf";
	}
	else if (!strcmp(src, "writeln"))
	{
		return "printf";
	}
	else
	{
		return src;
	}
}

void print(const char* src, FILE * FP)
{
	fprintf(FP, "%s", src);
}