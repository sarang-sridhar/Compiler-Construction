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
    if(table->arr[hash_value]==NULL){
        table->arr[hash_value] = entry;
        return;
    }
    else{
        ST_ENTRY* temp = table->arr[hash_value];
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

        ST_ENTRY* temp;
        temp->id_lexeme = node->addr->value;
        temp->next=NULL;    
        temp->type=t1;      //set type from driver
        insert_in_table(temp,table);
        return;
}

void create_entry_and_insert_in_FST(struct fn_symbol_table* table,struct treeNode* node,LISTNODE* ip_list,LISTNODE* op_head){
        if(node==NULL){
            printf("AST Node:%s is null \n",node->value);
            return;
        }
        FN_ENTRY* temp;
        temp->ip_head = ip_list;
        temp->op_head = op_head;
        temp->fn_name = node->addr->value;
        
        insert_fn_in_table(temp,table);
        return;
}

struct id_symbol_table* initST(int nesting_num){
    //use this fn to initialise ST after offset,scopes etc have been added
    struct id_symbol_table* t = malloc(sizeof(struct id_symbol_table));
    t->child_table=NULL;
    t->nesting_value = nesting_num;
    return t;
}







