#include "symbolTable.h"
#include<string.h>
#include<stdio.h>

int get_sym_table_hash(char* str){
    //implement hash fn
    int n = strlen(str);
    long long p = 31, m = 10e9+7;
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < n; i++)
    {
        hash = (hash + (str[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash)%TABLE_SIZE;
}


void insert_fn_in_table(struct fn_symbol_table* table, FN_ENTRY* entry){
    int hash_value = get_sym_table_hash(entry->fn_name);
    if(table->arr[hash_value]==NULL){
        table->arr[hash_value] = entry;
        return;
    }
    else{
        FN_ENTRY* temp = table->arr[hash_value];
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next = entry;
        return;
    }
}

void insert_in_table(struct id_symbol_table* table,  ST_ENTRY * entry){
    int hash_value = get_sym_table_hash(entry->id_lexeme);
    //printf("\n %s:%d \n",entry->id_lexeme,hash_value);
    if(table->arr[hash_value] == NULL){
        //printf("First-filling:%s \n",entry->id_lexeme);
        table->arr[hash_value]= entry;
        return;
    }
    else{
        ST_ENTRY* temp = table->arr[hash_value];

        //printf("\n Not-vacant:%s ",entry->id_lexeme);
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next = entry;
        return;
    }
}

FN_ENTRY* get_func_name(struct fn_symbol_table* table, char* str){
    int hash_value= get_sym_table_hash(str);
    if(table->arr[hash_value]==NULL){
        return NULL;
    }
    else{
        FN_ENTRY* temp = table->arr[hash_value];
        while(temp){
            if(!strcmp(temp->fn_name,str)){
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }
}

ST_ENTRY* get_lexeme(struct id_symbol_table* table, char* str){
    int hash_value= get_sym_table_hash(str);
    if(table->arr[hash_value]==NULL){
        return NULL;
    }
    else{
        ST_ENTRY* temp = table->arr[hash_value];
        while(temp){
            if(!strcmp(temp->id_lexeme,str)){
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }
}

void create_entry_and_insert(struct id_symbol_table* table,struct treeNode* node,TYPE t1){
        if(node==NULL){
            printf("AST Node:%s is null \n",node->value);
            return;
        }
        ST_ENTRY* temp = malloc(sizeof(ST_ENTRY));
        temp->id_lexeme = node->addr->value;
        temp->next = NULL;
        temp->type = t1;
        insert_in_table(table,temp);
        return;
}

void create_entry_and_insert_in_FST(struct fn_symbol_table* table,struct treeNode* node,LISTNODE* ip_list,LISTNODE* op_list){
        if(node==NULL){
            printf("AST Node:%s is null \n",node->value);
            return;
        }
        FN_ENTRY* temp = malloc(sizeof(FN_ENTRY));
        temp->ip_head = ip_list;
        temp->op_head = op_list;
        temp->fn_name = node->addr->value;
        
        insert_fn_in_table(table,temp);
        return;
}

struct id_symbol_table* initST(int nesting_num){
    //use this fn to initialise ST after offset,scopes etc have been added
    struct id_symbol_table* t = malloc(sizeof(struct id_symbol_table));
    t->child_table=NULL;
    t->parent_table=NULL;
    t->right_sibling=NULL;
    t->left_sibling=NULL;
    t->nesting_value = nesting_num;
    for(int i=0;i<TABLE_SIZE;i++){
        t->arr[i]=NULL;
    }
    return t;
}

struct fn_symbol_table* initFST(int nesting_num){
    struct fn_symbol_table* t = malloc(sizeof(struct fn_symbol_table));
    t->child_table=NULL;
    t->parent_table=NULL;
    t->right_sibling=NULL;
    t->left_sibling=NULL;
    t->nesting_value = nesting_num;
    for(int i=0;i<TABLE_SIZE;i++){
        t->arr[i]=NULL;
    }
    return t;
}



// int main(){
//     struct id_symbol_table* st = initST(1);
//     printf("\ninit success \n");

//     struct treeNode* n1 = malloc(sizeof(struct treeNode));

//     struct treeNode* n11 = malloc(sizeof(struct treeNode));
//     n1->addr = n11;
//     strcpy(n11->value,"abc");
//    // strcpy(n1->addr->value,"abc");
//     TYPE t1;
//     t1.id_type.id_dt=INTEGER;

//     create_entry_and_insert(st,n1,t1);


//     struct treeNode* n2 = malloc(sizeof(struct treeNode));
//     struct treeNode* n22 = malloc(sizeof(struct treeNode));
//     n2->addr = n22;
//     strcpy(n22->value,"def");
//    // strcpy(n2->addr->value,"def");
//     TYPE t2;
//     t2.id_type.id_dt=REAL;
//     create_entry_and_insert(st,n2,t2);

//     struct treeNode* n3 = malloc(sizeof(struct treeNode));
//     struct treeNode* n33 = malloc(sizeof(struct treeNode));
//     n3->addr = n33;
//     strcpy(n33->value,"aarya");
//     //strcpy(n3->addr->value,"aarya");
//     TYPE t3;
//     t3.id_type.id_dt=INTEGER;
    
//     create_entry_and_insert(st,n3,t3);


//     struct treeNode* n4 = malloc(sizeof(struct treeNode));
//     struct treeNode* n44 = malloc(sizeof(struct treeNode));
//     n4->addr = n44;
//     strcpy(n44->value,"great");
//     //strcpy(n4->addr->value,"great");
//     TYPE t4;
//     t4.id_type.id_dt=BOOLEAN;
//     create_entry_and_insert(st,n4,t4);
    


//     ST_ENTRY* temp = get_lexeme(st,"def");
//     // printf("IDHAR");
//     if(temp==NULL){
//         printf("FAIL \n");
//         return 0;
//     }
//     printf("** %s ** \n",temp->id_lexeme);

//     return 0;
// }




