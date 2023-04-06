#include "IRCodeGendef.h"
#include "symbolTable.h"

#define QUAD_SIZE 5000
#define TEMP_LEN 10

union argument
{
    ST_ENTRY* arg_var;
    int arg_num;
    double arg_rnum;
    char* arg_bool;
};

typedef struct Quadruple
{
    char op[7];
    union argument arg1;
    union argument arg2;
    union argument result;
}quad;

quad quadTable[QUAD_SIZE];
int count; // initialize count to 0 in the driver function

//symbol table for storing temporaries,allocate memory in driver function
struct id_symbol_table* temporaries_st;
//in order to give names to temporaries
int variable_count; //initialize to zero in the driver function