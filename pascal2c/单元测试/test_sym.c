#include <stdio.h>
#include "../symbolTable.h"

void tester(STATUS status)
{
    printf("%d\n", status);
}

int main()
{
    symbol_type st1[] = {FLOAT,FLOAT,FLOAT,INT};
    symbol_type st2[] = {FLOAT,INT,FLOAT};
    symbol_type st3[] = {FLOAT,FLOAT,FLOAT};
    symbol_type st4[] = {FLOAT, FLOAT};

    tester(locate_table());
    tester(add_variable("a", INT));
	tester(add_variable("amk", INT));
	know();
    tester(add_variable("b", CHAR));
    tester(add_variable("a", BOOL));
    tester(add_const("ccc", INT));
    tester(add_procedure("test1", 3));
    tester(locate_table());
    tester(add_parameter("test1", "a", FLOAT));
    tester(add_parameter("test1", "b", FLOAT));
    tester(add_parameter("test1", "c", FLOAT));
    tester(add_variable("D", INT));
    tester(add_function("test2", FLOAT, 2));
    tester(locate_table());
    tester(add_parameter("test2", "e", FLOAT));
    tester(get_type("a"));
    tester(get_type("test1"));
    tester(get_return_type("test2"));
    tester(get_parameter_number("test1"));
    tester(get_type("e"));
    tester(add_reference_parameter("test2", "hh", FLOAT));
    tester(check_parameter_types("test2", 4, st1));
    tester(check_parameter_types("test2", 3, st2));
    tester(check_parameter_types("test2", 3, st3));
    tester(relocate_table());
    tester(check_parameter_types("test2", 2, st4));
    tester(is_reference_parameter("test2", "hh"));
    know();
	printf("succeed finishing\n");
    return 0;
}