#include <stdio.h>
#include "ast.c"

#define TABLE_SIZE 1619

enum primitiveDatatype{INTEGER,REAL,BOOLEAN};

typedef union type{
    struct array{
        //range
        union low_range{
            char* low_id;
            int start;
        };
        union high_range{
            char* high_id;
            int end;
        };
        //datatype
        enum primitiveDatatype arr_dt;
    };
    struct id{
        //datatype
        enum primitiveDatatype id_dt;
    };
}TYPE;

typedef struct node{
    TYPE parameter_type;
    char* parameter_name;
    LISTNODE* next;
}LISTNODE;

typedef struct entry{
    TYPE type;
    char* id_lexeme;
    ST_ENTRY* next;
}ST_ENTRY;

typedef struct entry2{
//i/p list
    LISTNODE* ip_head;
//o/p list
    LISTNODE* op_head;
    char* fn_name;
    FN_ENTRY* next;
}FN_ENTRY;

struct id_symbol_table{
    ST_ENTRY *arr[TABLE_SIZE];
    int nesting_value;
    struct id_symbol_table* child_table;
    struct id_symbol_table* parent_table;
};

struct fn_symbol_table{
    FN_ENTRY* arr[TABLE_SIZE]; //fn entry created for future use
    //aur kya aayega isme
};

