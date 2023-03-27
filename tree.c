/*
ID: 2020A7PS0297P                             NAME: Sarang Sridhar 
ID: 2020A7PS0995P                             NAME: Kashish Mahajan 
ID: 2020A7PS0993P                             NAME: Satvik Sinha 
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma 
*/
#include "tree.h"

void addChild(struct treeNode* parent, struct node* child)
{
    struct treeNode* temp = malloc(sizeof(struct treeNode));
    strcpy(temp->value,child->value);
    // if(!child->isTerminal)
    //     strcpy(temp->tk_data.lexeme," "); 
    temp->isTerminal = child->isTerminal;
    temp->parent = parent;
    temp->prevSibling = NULL;
    parent->children = temp;
    child = child->forward_link;
    
    while(child != NULL)
    {   
        struct treeNode* temp2 = malloc(sizeof(struct treeNode));
        strcpy(temp2->value,child->value);
        if(!strcmp(child->value,"DOLLAR"))
            temp2->children = NULL;
        temp2->isTerminal = child->isTerminal;
        temp2->parent = parent;
        temp2->prevSibling = temp;
        temp->nextSibling = temp2;
        temp = temp2;
        child = child->forward_link;
    }
    temp->nextSibling = NULL;
}


