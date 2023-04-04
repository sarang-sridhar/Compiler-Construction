#include <stdio.h>
#include "ast.c" //change it to "ast.c"

#define TABLE_SIZE 1619

typedef struct entry2 FN_ENTRY;
typedef struct entry ST_ENTRY;
typedef struct node LISTNODE;

enum primitiveDatatype{INTEGER,REAL,BOOLEAN};
struct array{
        //range
        union low_range{
            char* low_id; // "-a" will be id not just a
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
typedef union type{
    struct array arr_type;
    struct id id_type;
}TYPE;

struct node{  
    TYPE parameter_type;
    char* parameter_name;
    LISTNODE* next;
};
struct entry2{
    //i/p list
    LISTNODE* ip_head; 
    //o/p list
    LISTNODE* op_head;
    char* fn_name;
    FN_ENTRY* next;
};

struct entry{
    TYPE type;
    char* id_lexeme;
    struct entry* next;
};

struct id_symbol_table{
    ST_ENTRY *arr[TABLE_SIZE];
    int nesting_value;
    struct id_symbol_table* child_table;
    struct id_symbol_table* parent_table;
    struct id_symbol_table* right_sibling;
    struct id_symbol_table* left_sibling; 
};

struct fn_symbol_table{
    FN_ENTRY* arr[TABLE_SIZE]; //fn entry created for future use
    int nesting_value;
    struct id_symbol_table* child_table;
    struct id_symbol_table* parent_table;
    struct id_symbol_table* right_sibling;
    struct id_symbol_table* left_sibling; 
    //aur kya aayega isme
};
