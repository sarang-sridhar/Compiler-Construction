#include "treeDef.h"

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
    union argument index1;
    union argument index2;
    union argument result;
    char* goTolabel; //jump to this label
    char* instruction;  //if or goTO or blah blah
    char* label;  //contsructs own label
}quad;

quad quadTable[QUAD_SIZE];
//for quad entry

int count; // initialize count to 0 in the driver function

//in order to give names to temporaries
int variable_count; //initialize to zero in the driver function

//for labels
int label_count;

//symbol table for storing temporaries,allocate memory in driver function
struct id_symbol_table* temporaries_st;

//for function calls
char* return_label;
