#include <stdio.h>

#define TABLE_SIZE 1619

typedef struct entry2 FN_ENTRY;
typedef struct entry ST_ENTRY;
typedef struct listnode LISTNODE;

int isFirstChild = 1; //1 for first child, 0 for others

struct for_loop{
    // char* id_name; not required
    int low_range;
    int high_range;
    //type is integer
};

union low_range
{
    char *low_id; // "-a" will be id not just a
    int start; 
};
   
union high_range
{
    char *high_id;
    int end;
};

struct array
{
    // range
    union low_range lowRange;
    union high_range highRange;
    // datatype
    char* arr_dt;
    int isStatic; // 1 for static, 0 for dynamic
};

struct id
{
    // datatype
    char* id_dt;
};

typedef union type
{
    struct array arr_type;
    struct id id_type;
    struct for_loop for_type;
} TYPE;

struct listnode
{   int is_array;
    TYPE parameter_type;
    char *parameter_name;
    LISTNODE *next;
};

struct entry2
{
    // i/p list
    LISTNODE *ip_head;
    // o/p list
    LISTNODE *op_head;
    char *fn_name;
    int is_declared;  //for function declaration
    FN_ENTRY *next;                      // linear probing
    struct id_symbol_table *child_table; // children
    // check for parent
};

struct entry
{
    int is_used;
    int is_array; // 0- not array 1-array
    int is_for; // 0- not for 1-for
    int offset;
    TYPE type;
    char *id_lexeme;
    struct entry *next; // linear probing
};

struct id_symbol_table
{
    ST_ENTRY *arr[TABLE_SIZE];
    int nesting_value;
    struct id_symbol_table *child_table;
    // if fn_entry is NULL, parent is symbol table else parent is fn_entry
    struct id_symbol_table *parent_table;
    FN_ENTRY* parent_function;
    struct id_symbol_table *right_sibling;
    struct id_symbol_table *left_sibling;
};

struct fn_symbol_table
{
    FN_ENTRY *arr[TABLE_SIZE]; // fn entry created for future use
    // aur kya aayega isme
};

//declare function symbol table
struct fn_symbol_table *fn_table; 