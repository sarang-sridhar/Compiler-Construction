#include<stdio.h>
#include "ast.c"

struct id_sym_table{
    ST_ENTRY identifier;
    //type
    struct id_sym_table* child_table; 
    int nesting_level
};