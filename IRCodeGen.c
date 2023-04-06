#include<stdio.h>
#include "treeDef.h"
#include "IRCodeGen.h"
#include "symbolTable.c"

ST_ENTRY* newTemp(TYPE t)
{
    char* temp = (char*) malloc(TEMP_LEN*sizeof(char));
    sprintf(temp,"t_%d",variable_count);
    variable_count++;
    struct treeNode* temp_node;
    strcpy(temp_node->tk_data.lexeme,temp);
    temp_node->addr =NULL;
    temp_node->astnextSibling = NULL;
    temp_node->astprevSibling = NULL;
    temp_node->children = NULL;
    temp_node->function_table_entry = NULL;
    temp_node->inh = NULL;
    temp_node->next = NULL;
    temp_node->nextSibling = NULL;
    temp_node->pair = NULL;
    temp_node->parent = NULL;
    temp_node->prevSibling = NULL;
    temp_node->syn = NULL;
    temp_node->type_syn = NULL;
    temp_node->type_inh = NULL;
    return create_entry_and_insert(temporaries_st,temp_node,t);
}

void operator_appears(struct treeNode* root){
    if(root->visited)
        return;
    root->visited = 1;
    struct treeNode* leftChild = root->children;
    if(leftChild == NULL)
        return;
    struct treeNode* rightChild = root->children->astnextSibling;
    TYPE t;
    //for left-child
    //code for ARRAY ACCESS ALSO TO BE WRITTEN, Kaha pe likhna hai ye? :'(
    if(leftChild->temp_variable_entry == NULL){
        if(!strcmp(leftChild->value,"ID")){
            quadTable[count].arg1.arg_var = leftChild->symbol_table_entry;
            t = leftChild->symbol_table_entry->type;
        }
        else if(!strcmp(leftChild->value,"NUM")){
            quadTable[count].arg1.arg_num = leftChild->tk_data.val;
            t.id_type.id_dt = "INTEGER";
        }
        else if(!strcmp(leftChild->value,"RNUM")){
            quadTable[count].arg1.arg_num = leftChild->tk_data.realVal;
            t.id_type.id_dt = "REAL";
        }
        else{
            //BOOLEAN case
            quadTable[count].arg1.arg_bool = leftChild->tk_data.lexeme;
            t.id_type.id_dt = "BOOLEAN";
        }
    }
    else{
        quadTable[count].arg1.arg_var = leftChild->temp_variable_entry;
        t = leftChild->temp_variable_entry->type;
    }

    //for right-child
    if(rightChild->temp_variable_entry == NULL){
        if(!strcmp(rightChild->value,"ID"))
            quadTable[count].arg2.arg_var = rightChild->symbol_table_entry;
        else if(!strcmp(rightChild->value,"NUM"))
            quadTable[count].arg2.arg_num = rightChild->tk_data.val;
        else if(!strcmp(rightChild->value,"RNUM")) //RNUM case
            quadTable[count].arg2.arg_num = rightChild->tk_data.realVal;
        else //BOOLEAN case
            quadTable[count].arg2.arg_bool = rightChild->tk_data.lexeme;
    }
    else{
        quadTable[count].arg2.arg_var = rightChild->temp_variable_entry;
    }

    //set operator
    strcpy(quadTable[count].op,root->tk_data.lexeme);

    ST_ENTRY* tempVarEntry = newTemp(t);
    quadTable[count].result.arg_var = tempVarEntry;
    root->temp_variable_entry = tempVarEntry; //set temp_variable entry of node
    count++;
}

void while_appears(struct treeNode* root){
    struct treeNode* leftChild = root->children; //operator
    struct treeNode* rightChild = root->children->astnextSibling; //statements
}

void IRcodegenerate(struct treeNode* root){
    if (root == NULL || root->visited == 1)
        return;
    root->visited = 1;
    IRcodegenerate(root->children);

    if (root->children != NULL){
         struct treeNode *temp;
        temp = root->children->astnextSibling;

        while (temp != NULL){
            IRCodegenerate(temp);
            temp = temp->astnextSibling;
        }
    }

    if (root->addr != NULL){
        IRCodegenerate(root->addr);
    }
    else{
        //control comes to a node
        if(!strcmp(root->value,"LVALUEID")){
            struct treeNode* id = root->children;
            struct treeNode* temporary = id->astnextSibling;
            quadTable[count].arg1.arg_var = temporary->temp_variable_entry;
            //setting arg2 to 0 in the case of LVALUEID
            quadTable[count].arg2.arg_var = NULL;
            strcpy(quadTable[count].op,"=");
            quadTable[count].result.arg_var = id->symbol_table_entry;
            count++;
        }
        //relational-op pe bhi same hi cheez?
        else if(!strcmp(root->value,"PLUS") || !strcmp(root->value,"MINUS") || !strcmp(root->value,"MUL") || !strcmp(root->value,"DIV") || !strcmp(root->value,"AND") || !strcmp(root->value,"OR") || !strcmp(root->value,"LT") || !strcmp(root->value,"LE") || !strcmp(root->value,"GT") || !strcmp(root->value,"GE") || !strcmp(root->value,"EQ") || !strcmp(root->value,"NE")){
            operator_appears(root);
        }
        else if(!strcmp(root->value,"U_MINUS") || !strcmp(root->value,"U_PLUS")){
            //set operator
            strcpy(quadTable[count].op,root->tk_data.lexeme);
            struct treeNode* rightChild = root->children->astnextSibling;
            if(rightChild->temp_variable_entry!=NULL)
                quadTable[count].arg1.arg_var = rightChild->temp_variable_entry;
            else if(!strcmp(rightChild->value,"ID"))
                quadTable[count].arg1.arg_var = rightChild->symbol_table_entry;
            else if(!strcmp(rightChild->value,"NUM"))
                quadTable[count].arg1.arg_num = rightChild->tk_data.val;
            else //RNUM case
                quadTable[count].arg1.arg_num = rightChild->tk_data.realVal;
            quadTable[count].arg2.arg_var = NULL;
            ST_ENTRY* tempVarEntry = newTemp(rightChild->symbol_table_entry->type);
            quadTable[count].result.arg_var = tempVarEntry;
            root->temp_variable_entry = tempVarEntry; //set temp_variable entry of node
            count++;
        }
        //aakhri jagah
        else if(!strcmp(root->value,"GET-VALUE")){
            //set operator
            strcpy(quadTable[count].op,"GET");
            quadTable[count].arg1.arg_var = NULL;
            quadTable[count].arg2.arg_var = NULL;
            quadTable[count].result.arg_var = root->children->symbol_table_entry;
            count++;
        }
        else if(!strcmp(root->value,"PRINT")){
            //set operator
            strcpy(quadTable[count].op,"PRINT");
            quadTable[count].arg1.arg_var = NULL;
            quadTable[count].arg2.arg_var = NULL;
            if(!strcmp(root->children,"NUM")){
                quadTable[count].result.arg_rnum = root->children->tk_data.realVal;
            }
            else if(!strcmp(root->children,"RNUM")){
                quadTable[count].result.arg_num = root->children->tk_data.val;
            }
            else{ //boolean case
                quadTable[count].result.arg_var = root->children->symbol_table_entry;
            }
        }
        else if(!strcmp(root->value,"WHILE")){
            while_appears(root);
        }
    }
}