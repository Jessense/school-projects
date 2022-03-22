struct id_node
{
	char* name;
	int id_node_id;
	char* type;
	struct id_node* next;
	int var_flag;//确定是传值引用还是传地址引用
}id_node;

extern char* char_strcpy(char* src, char* target);

extern char* code_get_type(char* src);

extern char* get_function_name(const char* src);

extern void print(const char* src, FILE* FP);
