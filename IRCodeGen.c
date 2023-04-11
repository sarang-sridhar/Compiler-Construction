#include "IRCodeGen.h"
// #include "symbolTable.c

char *newlabel()
{
    char *lab = (char *)malloc(TEMP_LEN * sizeof(char));
    sprintf(lab, "L%d:", label_count);
    label_count++;
    return lab;
}

void assignlabel(struct treeNode* root)
{
    if (root->constructLabel == NULL){
        if(!strcmp(root->value,"DECLARE") || !strcmp(root->value,"GET-VALUE") || !strcmp(root->value,"PRINT") || !strcmp(root->value,"WHILE") || !strcmp(root->value,"FOR") || !strcmp(root->value,"SWITCH") || (root->parent != NULL && !strcmp(root->parent->value,"SWITCH")) ||!strcmp(root->value,"LVALUEID") || !strcmp(root->value,"LVALUEARR") || !strcmp(root->value,"MODULE-INVOKE"))
            root->constructLabel = newlabel();
    }
}

ST_ENTRY *newTemp(TYPE t)
{
    //char *temp = (char *)malloc(TEMP_LEN * sizeof(char));
    char temp[TEMP_LEN];
    sprintf(temp, "t_%d", variable_count);
    variable_count++;
    struct treeNode *temp_node = malloc(sizeof(struct treeNode));
    strcpy(temp_node->tk_data.lexeme, temp);
    temp_node->addr = NULL;
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
    return create_entry_and_insert(temporaries_st, temp_node, t);
}

// CHANGE TYPE ACCORDING TO PRECEDENCE
void operator_appears(struct treeNode *root)
{
    //YE HATAYA HAI
    // if (root->visited)
    //     return;
    // root->visited = 1;
    struct treeNode *leftChild = root->children;
    if (leftChild == NULL)
        return;
    struct treeNode *rightChild = root->children->astnextSibling;
    TYPE t;
    // for left-child
    printf("before leftChild:%s\n",leftChild->tk_data.lexeme);
    if (leftChild->temp_variable_entry == NULL)
    {
        printf("idhar aaya \n");
        if (!strcmp(leftChild->value, "ID"))
        {
            if(leftChild->symbol_table_entry == NULL)
                printf("THIS IS NULL\n");
            quadTable[count].arg1.arg.arg_var = leftChild->symbol_table_entry;
            quadTable[count].arg1.entry = 0;
            printf("Left child ka st entry is:%s \n",leftChild->symbol_table_entry->id_lexeme);
            t = leftChild->symbol_table_entry->type;
        }
        else if (!strcmp(leftChild->value, "NUM"))
        {
            quadTable[count].arg1.arg.arg_num = leftChild->tk_data.val;
            quadTable[count].arg1.entry = 1;
            t.id_type.id_dt = "INTEGER";
        }
        else if (!strcmp(leftChild->value, "RNUM"))
        {
            quadTable[count].arg1.arg.arg_rnum = leftChild->tk_data.realVal;
            quadTable[count].arg1.entry = 2;
            t.id_type.id_dt = "REAL";
        }
        else if (!strcmp(leftChild->value, "BOOLEAN"))
        {
            quadTable[count].arg1.arg.arg_bool = leftChild->tk_data.lexeme;
            quadTable[count].arg1.entry = 3;
            t.id_type.id_dt = "BOOLEAN";
        }
        else
        {
            // ARRAY CASE
            t = leftChild->children->symbol_table_entry->type;
            quadTable[count].arg1.arg.arg_var = leftChild->children->symbol_table_entry;
            quadTable[count].arg1.entry = 0;
            if (leftChild->children->astnextSibling->temp_variable_entry != NULL)
            {
                quadTable[count].index1.arg.arg_var = leftChild->children->astnextSibling->temp_variable_entry;
                quadTable[count].index1.entry = 0;
            }
            else if (!strcmp(leftChild->children->astnextSibling->value, "NUM"))
            {
                quadTable[count].index1.arg.arg_num = leftChild->children->astnextSibling->tk_data.val;
                quadTable[count].index1.entry = 1;
            }
            else
            { // id case
                quadTable[count].index1.arg.arg_var = leftChild->children->astnextSibling->symbol_table_entry;
                quadTable[count].index1.entry = 0;
            }
        }
    }
    else
    {
        quadTable[count].arg1.arg.arg_var = leftChild->temp_variable_entry;
        quadTable[count].arg1.entry = 0;
        t = leftChild->temp_variable_entry->type;
    }

    // for right-child
    if (rightChild->temp_variable_entry == NULL)
    {
        if (!strcmp(rightChild->value, "ID")){
            //printf("Right child ka symbol table entry is:%s\n",rightChild->symbol_table_entry->id_lexeme);
            quadTable[count].arg2.arg.arg_var = rightChild->symbol_table_entry;
            quadTable[count].arg2.entry = 0;
        }
        else if (!strcmp(rightChild->value, "NUM")){
            quadTable[count].arg2.arg.arg_num = rightChild->tk_data.val;
            quadTable[count].arg2.entry = 1;}
        else if (!strcmp(rightChild->value, "RNUM")) // RNUM case 
        {   quadTable[count].arg2.arg.arg_num = rightChild->tk_data.realVal;
            quadTable[count].arg2.entry = 2;
        }
        else if (!strcmp(rightChild->value, "BOOLEAN")){
            quadTable[count].arg2.arg.arg_bool = rightChild->tk_data.lexeme;
            quadTable[count].arg2.entry = 3;
            }
        else
        {
            // ARRAY CASE

            quadTable[count].arg2.arg.arg_var = rightChild->children->symbol_table_entry;
            quadTable[count].arg2.entry = 0;
            if (rightChild->children->astnextSibling->temp_variable_entry != NULL)
            {
                quadTable[count].index2.arg.arg_var = rightChild->children->astnextSibling->temp_variable_entry;
                quadTable[count].index2.entry = 0;
            }
            else if (!strcmp(rightChild->children->astnextSibling->value, "NUM"))
            {
                quadTable[count].index2.arg.arg_num = rightChild->children->astnextSibling->tk_data.val;
                quadTable[count].index2.entry = 1;
            }
            else
            { // id case
                quadTable[count].index2.arg.arg_var = rightChild->children->astnextSibling->symbol_table_entry;
                quadTable[count].index2.entry = 0;
            }
        }
    }
    else
    {
        quadTable[count].arg2.arg.arg_var = rightChild->temp_variable_entry;
        quadTable[count].arg2.entry = 0;
    }

    // set operator
    strcpy(quadTable[count].op, root->value);

    //printf("Before new temp:%s\n",t.id_type.id_dt);
    ST_ENTRY *tempVarEntry = newTemp(t);
    //printf("After new temp\n");
    quadTable[count].result.arg.arg_var = tempVarEntry;
    quadTable[count].result.entry = 0;
    root->temp_variable_entry = tempVarEntry; // set temp_variable entry of node
    count++;
}

void while_expression_appears(struct treeNode *root)
{
    if (!strcmp(root->value, "AND"))
    {
        // assignlabel(root->children->astnextSibling);
        if(root->children->astnextSibling->constructLabel == NULL)
            root->children->astnextSibling->constructLabel = newlabel();
        root->children->true_label = root->children->astnextSibling->constructLabel;
        printf("AND ke left bacche ka true label: %s \n",root->children->true_label);
        root->children->false_label = root->false_label;
        root->children->astnextSibling->true_label = root->true_label;
        root->children->astnextSibling->false_label = root->false_label;
    }
    else if (!strcmp(root->value, "OR"))
    {
        if (root->children->astnextSibling->constructLabel == NULL)
            root->children->astnextSibling->constructLabel = newlabel();
        // assignlabel(root->children->astnextSibling);
        root->children->true_label = root->true_label;
        root->children->false_label = root->children->astnextSibling->constructLabel;
        root->children->astnextSibling->true_label = root->true_label;
        root->children->astnextSibling->false_label = root->false_label;
    }
}

void while_appears(struct treeNode *root)
{
    quadTable[count].goTolabel = root->constructLabel;
    quadTable[count].instruction = "GOTO";
    quadTable[count].arg1.arg.arg_var = NULL;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].label = NULL;
    count++;
}

void while_expression_codeGen(struct treeNode *root)
{
    if(root->while_visited == 1)
        return;
    root->while_visited = 1;
    // if (!strcmp(root->value, "AND"))
    // {
    // }
    // else if (!strcmp(root->value, "OR"))
    // {
    // }
    if (!strcmp(root->value, "BOOLEAN"))
    {
        if (!strcmp(root->tk_data.lexeme, "false"))
        {
            quadTable[count].goTolabel = root->false_label;
            quadTable[count].instruction = "GOTO";
            count++;
        }
        else
        {
            quadTable[count].goTolabel = root->true_label;
            quadTable[count].instruction = "GOTO";
            count++;
        }
    }
    else if(!strcmp(root->value,"ID")){
        quadTable[count].instruction = "IF";
        quadTable[count].arg1.arg.arg_var = root->symbol_table_entry;
        quadTable[count].arg1.entry = 0;
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        quadTable[count].goTolabel = root->true_label;
        printf("Root's true label: %s\n", root->true_label);
        if(root->astnextSibling == NULL)
        {
            if(!strcmp(root->parent->value,"AND"))
                quadTable[count].label = root->parent->children->true_label;
            else if(!strcmp(root->parent->value,"OR"))
                quadTable[count].label = root->parent->children->false_label;
        }
        count++;
        quadTable[count].goTolabel = root->false_label;
        quadTable[count].instruction = "GOTO";
        quadTable[count].arg1.arg.arg_var = NULL;
        quadTable[count].arg1.entry = 0;
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        count++;
    }
    else if (!strcmp(root->value, "ARRAY_ACCESS"))
    {
        struct treeNode* idNode = root->children; //id for array
        quadTable[count].instruction = "IF";
        quadTable[count].arg1.arg.arg_var = idNode->symbol_table_entry;
        quadTable[count].arg1.entry = 0;
        struct treeNode* rightChild = root->children->astnextSibling;
        if(rightChild->temp_variable_entry == NULL){
            if(!strcmp(rightChild->value,"NUM")){
                quadTable[count].index1.arg.arg_num = rightChild->tk_data.val;
                quadTable[count].index1.entry = 1;
            }
            else{ //ID
                quadTable[count].index1.arg.arg_var = rightChild->symbol_table_entry;
                quadTable[count].index1.entry = 0;
            }
        }
        else{//Temp
            quadTable[count].index1.arg.arg_var = rightChild->temp_variable_entry;
            quadTable[count].index1.entry = 0;
        }
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        quadTable[count].goTolabel = root->true_label;
        count++;
        quadTable[count].goTolabel = root->false_label;
        quadTable[count].instruction = "GOTO";
        quadTable[count].arg1.arg.arg_var = NULL;
        quadTable[count].arg1.entry = 0;
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        count++;
    }
    else
    {
        // relational Operator
        struct treeNode *leftChild = root->children;
        struct treeNode *rightChild = root->children->astnextSibling;
        if (leftChild->temp_variable_entry == NULL)
        {
            if (!strcmp(leftChild->value, "ID"))
            {
                quadTable[count].arg1.arg.arg_var = leftChild->symbol_table_entry;
                quadTable[count].arg1.entry = 0;
            }
            else if (!strcmp(leftChild->value, "NUM"))
            {
                quadTable[count].arg1.arg.arg_num = leftChild->tk_data.val;
                quadTable[count].arg1.entry = 1;
            }
            else if (!strcmp(leftChild->value, "RNUM"))
            {
                quadTable[count].arg1.arg.arg_rnum = leftChild->tk_data.realVal;
                quadTable[count].arg1.entry = 2;
            }
            else
            {
                // ARRAY CASE
                quadTable[count].arg1.arg.arg_var = leftChild->children->symbol_table_entry;
                quadTable[count].arg1.entry = 0;
                if (leftChild->children->astnextSibling->temp_variable_entry != NULL)
                {
                    quadTable[count].index1.arg.arg_var = leftChild->children->astnextSibling->temp_variable_entry;
                    quadTable[count].index1.entry = 0;
                }
                else if (!strcmp(leftChild->children->astnextSibling->value, "NUM"))
                {
                    quadTable[count].index1.arg.arg_num = leftChild->children->astnextSibling->tk_data.val;
                    quadTable[count].index1.entry = 1;
                }
                else
                { // id case
                    quadTable[count].index1.arg.arg_var = leftChild->children->astnextSibling->symbol_table_entry;
                    quadTable[count].index1.entry = 0;
                }
            }
        }
        else
        {
            quadTable[count].arg1.arg.arg_var = leftChild->temp_variable_entry;
            quadTable[count].arg1.entry = 0;
        }

        // for right-child
        if (rightChild->temp_variable_entry == NULL)
        {
            if (!strcmp(rightChild->value, "ID")){
                quadTable[count].arg2.arg.arg_var = rightChild->symbol_table_entry;
                quadTable[count].arg2.entry = 0;
            }
            else if (!strcmp(rightChild->value, "NUM")){
                quadTable[count].arg2.arg.arg_num = rightChild->tk_data.val;
                quadTable[count].arg2.entry = 1;}
            else if (!strcmp(rightChild->value, "RNUM")) // RNUM case
            {   quadTable[count].arg2.arg.arg_rnum = rightChild->tk_data.realVal;
                quadTable[count].arg2.entry = 2;}
            else if (!strcmp(rightChild->value, "BOOLEAN")){
                quadTable[count].arg2.arg.arg_bool = rightChild->tk_data.lexeme;
                quadTable[count].arg2.entry = 3;}
            else
            {
                // ARRAY CASE

                quadTable[count].arg2.arg.arg_var = rightChild->children->symbol_table_entry;
                quadTable[count].arg2.entry = 0;
                if (rightChild->children->astnextSibling->temp_variable_entry != NULL)
                {
                    quadTable[count].index2.arg.arg_var = rightChild->children->astnextSibling->temp_variable_entry;
                    quadTable[count].index2.entry = 0;
                }
                else if (!strcmp(rightChild->children->astnextSibling->value, "NUM"))
                {
                    quadTable[count].index2.arg.arg_num = rightChild->children->astnextSibling->tk_data.val;
                    quadTable[count].index2.entry = 1;
                }
                else
                { // id case
                    quadTable[count].index2.arg.arg_var = rightChild->children->astnextSibling->symbol_table_entry;
                    quadTable[count].index2.entry = 0;
                }
            }
        }
        else
        {
            quadTable[count].arg2.arg.arg_var = rightChild->temp_variable_entry;
            quadTable[count].arg2.entry = 0;
        }

        // //-------------------------------------------------------------------------------
        // // left-child
        // if (leftChild->temp_variable_entry == NULL)
        // {
        //     if (!strcmp(leftChild->value, "ID"))
        //         quadTable[count].arg1.arg_var = leftChild->symbol_table_entry;
        //     else if (!strcmp(leftChild->value, "NUM"))
        //         quadTable[count].arg1.arg_num = leftChild->tk_data.val;
        //     else if (!strcmp(leftChild->value, "RNUM"))
        //         quadTable[count].arg1.arg_num = leftChild->tk_data.realVal;
        // }
        // else
        // {
        //     quadTable[count].arg1.arg_var = leftChild->temp_variable_entry;
        // }
        // // right-child
        // if (rightChild->temp_variable_entry == NULL)
        // {
        //     if (!strcmp(rightChild->value, "ID"))
        //         quadTable[count].arg2.arg_var = rightChild->symbol_table_entry;
        //     else if (!strcmp(rightChild->value, "NUM"))
        //         quadTable[count].arg2.arg_num = rightChild->tk_data.val;
        //     else if (!strcmp(rightChild->value, "RNUM")) // RNUM case
        //         quadTable[count].arg2.arg_num = rightChild->tk_data.realVal;
        // }
        // else
        // {
        //     quadTable[count].arg2.arg_var = rightChild->temp_variable_entry;
        // }

        strcpy(quadTable[count].op, root->value);
        quadTable[count].goTolabel = root->true_label;
        quadTable[count].instruction = "IF";
        count++;
        quadTable[count].instruction = "GOTO";
        quadTable[count].goTolabel = root->false_label;
        quadTable[count].arg1.arg.arg_var = NULL;
        quadTable[count].arg1.entry = 0;
        quadTable[count].arg2.arg.arg_var = NULL;
        quadTable[count].arg2.entry = 0;
        quadTable[count].result.arg.arg_var = NULL;
        quadTable[count].result.entry = 0;
        count++;
    }
}

void for_appears_top(struct treeNode *root)
{
    struct treeNode *idNode = root->children;
    struct treeNode *rangeNode = root->children->astnextSibling;
    struct treeNode *statementsNode = root->children->astnextSibling->astnextSibling;

    // assignment instruction
    strcpy(quadTable[count].op, "=");
    quadTable[count].arg1.arg.arg_num = idNode->symbol_table_entry->type.for_type.low_range;
    quadTable[count].arg1.entry = 1;
    quadTable[count].result.arg.arg_var = idNode->symbol_table_entry;
    quadTable[count].result.entry = 0;
    count++;

    // condition check
    // label for condition check
    if (root->constructLabel == NULL)
        root->constructLabel = newlabel();
    quadTable[count].label = newlabel();
    root->for_construct_label = quadTable[count].label; //setting for's construct label
    quadTable[count].instruction = "IF";
    strcpy(quadTable[count].op, "<=");
    quadTable[count].arg1.arg.arg_var = idNode->symbol_table_entry;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_num = idNode->symbol_table_entry->type.for_type.high_range;
    quadTable[count].arg2.entry = 1;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    // if (statementsNode!=NULL && statementsNode->constructLabel == NULL)
    //     statementsNode->constructLabel = newlabel();
    assignlabel(statementsNode);

    if(statementsNode != NULL)
        quadTable[count].goTolabel = statementsNode->constructLabel;
    count++;

    printf("FOR's parent is:%s",root->parent->value);
    // goto for false condition
    if (root->next == NULL && root->parent!=NULL &&  (!strcmp(root->parent->value, "MODULE_DEF") || !strcmp(root->parent->value,"PROGRAM")))
        quadTable[count].goTolabel = "return";
    // doubtful case
    else if (root->next == NULL && root->parent!=NULL && !strcmp(root->parent->value,"WHILE"))
    {
        // if (root->parent->constructLabel == NULL)
        //     root->parent->constructLabel = newlabel();
        assignlabel(root->parent);
        quadTable[count].goTolabel = root->parent->constructLabel;
    }
    else if(root->next == NULL && root->parent!=NULL && !strcmp(root->parent->value,"FOR")){
        root->parent->for_increment_label = newlabel();
        quadTable[count].goTolabel = root->parent->for_increment_label;
    }
    // KYA YE SAHI HAI?
    else if (root->next == NULL)
    {
        struct treeNode *temp = root->parent->next;
        while (temp == NULL)
            temp = temp->parent->next;
        quadTable[count].goTolabel = temp->constructLabel;
    }
    else
    {
        // if (root->next->constructLabel == NULL)
        //     root->next->constructLabel = newlabel();
        assignlabel(root->next);
        
        quadTable[count].goTolabel = root->next->constructLabel;
    }
    quadTable[count].instruction = "GOTO";
    quadTable[count].arg1.arg.arg_var = NULL;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    count++;

    if(statementsNode != NULL){
        //printf("LABEL HAI:%s\n",statementsNode->constructLabel);
        quadTable[count].label = statementsNode->constructLabel;
    }
}

void for_appears_bottom(struct treeNode *root)
{
    struct treeNode *idNode = root->children;
    // increment operation
    quadTable[count].arg1.arg.arg_var = idNode->symbol_table_entry;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].instruction = "INCREMENT"; // increment operation
    quadTable[count].goTolabel = NULL;
    if(root->for_increment_label!=NULL)
        quadTable[count].label = root->for_increment_label;
    else
        quadTable[count].label = NULL;
    count++;

    // goto for condition check
    quadTable[count].instruction = "GOTO";
    quadTable[count].goTolabel = root->for_construct_label;
    quadTable[count].arg1.arg.arg_var = NULL;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].label = NULL;
    count++;
}

void case_appears_top(struct treeNode *root)
{
    struct treeNode *idNode = root->parent->children;
    struct treeNode *stmtsNode = root->pair;
    // if (stmtsNode->constructLabel == NULL)
    //     stmtsNode->constructLabel = newlabel();
    assignlabel(stmtsNode);
    strcpy(quadTable[count].op, "==");
    if (root->constructLabel == NULL) // generating label for case
        root->constructLabel = newlabel();
    //assignlabel(root);
    quadTable[count].label = root->constructLabel;
    quadTable[count].arg1.arg.arg_var = idNode->symbol_table_entry;
    quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_num = root->tk_data.val;
    quadTable[count].arg2.entry = 1;
    quadTable[count].goTolabel = stmtsNode->constructLabel;
    quadTable[count].instruction = "IF";
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    count++;
    quadTable[count].instruction = "GOTO";
    //default exists
    if(root->next == NULL && root->parent->children->astnextSibling->astnextSibling!=NULL){
        //printf("Inside\n");
        assignlabel(root->parent->children->astnextSibling->astnextSibling);
        //printf("after\n");
        quadTable[count].goTolabel = root->parent->children->astnextSibling->astnextSibling->constructLabel;
    }
    else if (root->next == NULL && root->parent->next != NULL)
        quadTable[count].goTolabel = root->parent->next->constructLabel;
    else if (root->next == NULL && (!strcmp(root->parent->parent->value, "MODULE_DEF") || !strcmp(root->parent->parent->value,"PROGRAM")))
    {
        quadTable[count].goTolabel = "return";     
    }
    // recursive like case, case ki nesting
    else if (root->next == NULL && strcmp(root->parent->parent->parent->value, "SWITCH"))
    {
        quadTable[count].goTolabel = root->parent->parent->constructLabel;
    }
    else if (root->next == NULL)
    {
        struct treeNode *temp = root->parent->parent;
        while (strcmp(temp->value, "MODULE_DEF") && strcmp(temp->value, "PROGRAM") && temp->next == NULL)
            temp = temp->parent;
        if (!strcmp(temp->value, "MODULE_DEF") || !strcmp(temp->value,"PROGRAM"))
            quadTable[count].goTolabel = "return";
        else
            quadTable[count].goTolabel = temp->next->constructLabel;
    }
    else
    {
        // if (root->next->constructLabel == NULL)
        //     root->next->constructLabel = newlabel();
        assignlabel(root->next);
        quadTable[count].goTolabel = root->next->constructLabel;
    }

    quadTable[count].arg1.arg.arg_var = NULL;
     quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].label = NULL;
    count++;
}

void case_appears_bottom(struct treeNode *root)
{   printf("Isme ghusa \n");
    struct treeNode *switchNext = root->parent->next;
    if (switchNext == NULL && (!strcmp(root->parent->parent->value, "MODULE_DEF") || !strcmp(root->parent->parent->value,"PROGRAM")))
    {
        quadTable[count].goTolabel = "return";
    }
    // doubtful case(agar parent case stmt hua to)
    else if (switchNext == NULL && !strcmp(root->parent->parent->value,"WHILE"))
    {
        quadTable[count].goTolabel = root->parent->parent->constructLabel;
    }
    else if(switchNext == NULL && !strcmp(root->parent->parent->value,"FOR")){
        root->parent->parent->for_increment_label = newlabel();
        quadTable[count].goTolabel = root->parent->parent->for_increment_label;
    }
    // KYA YE SAHI HAI?
    else if (switchNext == NULL)
    {
        struct treeNode *temp = root->parent->parent;
        while (strcmp(temp->value, "MODULE_DEF") && strcmp(temp->value,"PROGRAM") && temp->next == NULL)
            temp = temp->parent;
        if (!strcmp(temp->value, "MODULE_DEF") || !strcmp(temp->value,"PROGRAM"))
            quadTable[count].goTolabel = "return";
        else
            quadTable[count].goTolabel = temp->next->constructLabel;
    }
    else
    {
        quadTable[count].goTolabel = switchNext->constructLabel;
    }
    quadTable[count].instruction = "GOTO";
    quadTable[count].arg1.arg.arg_var = NULL;
     quadTable[count].arg1.entry = 0;
    quadTable[count].arg2.arg.arg_var = NULL;
    quadTable[count].arg2.entry = 0;
    quadTable[count].result.arg.arg_var = NULL;
    quadTable[count].result.entry = 0;
    quadTable[count].label = NULL;
    count++;
}

void IRcodegenerate(struct treeNode *root)
{
    if (root == NULL || root->visited == 1)
        return;
    root->visited = 1;

    printf("Called for:%s\n",root->value);

    //printf("Before label\n");

    if (root->constructLabel == NULL){
        if(!strcmp(root->value,"DECLARE") || !strcmp(root->value,"GET-VALUE") || !strcmp(root->value,"PRINT") || !strcmp(root->value,"WHILE") || !strcmp(root->value,"FOR") || !strcmp(root->value,"SWITCH") || (root->parent != NULL && !strcmp(root->parent->value,"SWITCH")) ||!strcmp(root->value,"LVALUEID") || !strcmp(root->value,"LVALUEARR") || !strcmp(root->value,"MODULE-INVOKE"))
            root->constructLabel = newlabel();
    }

   // printf("After label\n");
    //printf("Root : %s Root's construct label: %s\n ",root->value,root->constructLabel);
    if(quadTable[count].label == NULL && root->constructLabel!=NULL)
        quadTable[count].label = root->constructLabel; // set label at the beginning of each construct

    //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // top down
    if (!strcmp(root->value, "WHILE"))
    {
        struct treeNode *expr = root->children;                            // expression
        struct treeNode *statementsChild = root->children->astnextSibling; // statements
        // set labels
        // CHECK KI NEXT NULL TO NI HO GAYA
        // if (statementsChild->constructLabel == NULL)
        //     statementsChild->constructLabel = newlabel();
        assignlabel(statementsChild);
        expr->true_label = statementsChild->constructLabel;
        if (root->next == NULL &&  (!strcmp(root->parent->value, "MODULE_DEF")))
        {
            expr->false_label = "return";
        }
        /**************DOUBTFUL CASE(parent is case)**************/
        else if (root->next == NULL && !strcmp(root->parent->value,"WHILE"))
        {
            // if (root->parent->constructLabel == NULL)
            //     root->parent->constructLabel = newlabel();
            assignlabel(root->parent);
            expr->false_label = root->parent->constructLabel;
        }
        else if(root->next!=NULL && !strcmp(root->parent->value,"FOR")){
            root->parent->for_increment_label = newlabel();
            expr->false_label = root->parent->for_increment_label;
        }
        // MARK THIS FOR LATER
        else if (root->next == NULL)
        {
            struct treeNode *temp = root->parent;
            while (strcmp(temp->value, "MODULE_DEF") && strcmp(temp->value,"PROGRAM") && temp->next == NULL)
            {
                temp = temp->parent;
            }
            if (!strcmp(temp->value, "MODULE_DEF") || !strcmp(temp->value,"PROGRAM"))
                expr->false_label = "return";
            else
                expr->false_label = temp->next->constructLabel;
        }
        else
        {
            // if (root->next->constructLabel == NULL)
            //     root->next->constructLabel = newlabel();
            assignlabel(root->next);
            expr->false_label = root->next->constructLabel;
        }
    }
    
    else if (!strcmp(root->value, "MODULE_DEF"))
    {
        // if (root->constructLabel == NULL)
        //     root->constructLabel = newlabel();
        //store label in symbol table entry
        quadTable[count].instruction = "DECLAREMOD";
        quadTable[count].arg_fn_name = root->children->function_table_entry;
        count++;
    }
    
    else if (!strcmp(root->value, "FOR"))
    {
        //printf("Before top\n");
        for_appears_top(root);
        //printf("After top down\n");
    }
    // case stmt top-down
    else if (root->parent!=NULL && !strcmp(root->parent->value, "SWITCH") && root->pair != NULL)
    {
        case_appears_top(root);
        printf("After case appears top\n");
    }
    // only for AND and OR
    else if (!strcmp(root->value, "AND") || !strcmp(root->value, "OR"))
    {
        if (!root->isChildOfAssign)
            while_expression_appears(root);
    }
    
    //printf("Before this\n");
    if(root->children!=NULL){
        if (root->parent != NULL && (!strcmp(root->parent->value, "LVALUEID") || !strcmp(root->parent->value, "LVALUEARRAY") || root->parent->isChildOfAssign))
            root->isChildOfAssign = 1;
        else{
            root->isChildOfAssign = 0;
        }
    }
    //printf("After this\n");
        
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // left-child expand
    //printf("Before call\n");
    IRcodegenerate(root->children);
    //printf("After call\n");

    if (!strcmp(root->value, "AND") && !root->isChildOfAssign)
    {
        quadTable[count].goTolabel = root->children->true_label;
        // POINT OF DOUBT/ERROR
        //  count++;
    }
    else if (!strcmp(root->value, "OR") && !root->isChildOfAssign)
        quadTable[count].goTolabel = root->children->false_label;

    // right-child expand
    if (root->children != NULL)
    {
        struct treeNode *temp;
        temp = root->children->astnextSibling;

        while (temp != NULL)
        {
            if (!strcmp(root->value, "LVALUEID") || !strcmp(root->value, "LVALUEARR") || root->isChildOfAssign)
                temp->isChildOfAssign = 1;
            else
                temp->isChildOfAssign = 0;
            IRcodegenerate(temp);
            temp = temp->astnextSibling;
        }
    }

    if (root->pair != NULL)
        IRcodegenerate(root->pair);
    
    printf("Bottom up starts:%s,isDefault:%d\n",root->value,root->isDefault);

   
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //control comes to a node
    if (root->addr != NULL)
    {
        printf("then called for:%s \n",root->addr->value);
        IRcodegenerate(root->addr);
    }
    else
    {
        //printf("Here:%s\n",root->value);
        if (!strcmp(root->value, "LVALUEID"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            struct treeNode *id = root->children;
            struct treeNode *temporary = id->astnextSibling;
            if (temporary->temp_variable_entry != NULL){
                quadTable[count].arg1.arg.arg_var = temporary->temp_variable_entry;
                 quadTable[count].arg1.entry = 0;}
            else
            {
                if (!strcmp(temporary->value, "NUM"))
                {
                    quadTable[count].arg1.arg.arg_num = temporary->tk_data.val;
                    quadTable[count].arg1.entry = 1;
                }
                else if (!strcmp(temporary->value, "RNUM"))
                {
                    quadTable[count].arg1.arg.arg_rnum = temporary->tk_data.realVal;
                    quadTable[count].arg1.entry = 2;
                }
                // introduce Array case later
                else
                {
                    quadTable[count].arg1.arg.arg_var = temporary->symbol_table_entry;
                    quadTable[count].arg1.entry = 0;
                }
            }
            // setting arg2 to 0 in the case of LVALUEID
            quadTable[count].arg2.arg.arg_var = NULL;
            quadTable[count].arg2.entry = 0;
            strcpy(quadTable[count].op, "=");
            quadTable[count].result.arg.arg_var = id->symbol_table_entry;
            quadTable[count].result.entry = 0;
            count++;
        }

        else if (!strcmp(root->value, "LVALUEARRAY"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            struct treeNode *id = root->children;
            struct treeNode *temporary = id->astnextSibling;
            
            if (temporary->temp_variable_entry != NULL){ 
                quadTable[count].arg1.arg.arg_var = temporary->temp_variable_entry;
                quadTable[count].arg1.entry = 0;
            }
                
            else
            {
                if (!strcmp(temporary->value, "NUM"))
                {
                    quadTable[count].arg1.arg.arg_num = temporary->tk_data.val;
                    quadTable[count].arg1.entry = 1;
                }
                else if (!strcmp(temporary->value, "RNUM"))
                {
                    quadTable[count].arg1.arg.arg_rnum = temporary->tk_data.realVal;
                    quadTable[count].arg1.entry = 2;
                }
                // introduce Array case later
                else
                {
                    quadTable[count].arg1.arg.arg_var = temporary->symbol_table_entry;
                    quadTable[count].arg1.entry = 0;
                }
            }

            // setting arg2 to 0 in the case of LVALUEID
            if (id->children->astnextSibling->temp_variable_entry != NULL)
            {
                quadTable[count].arg2.arg.arg_var = id->children->astnextSibling->temp_variable_entry;
                quadTable[count].arg2.entry = 0;
            }
            else
            {
                if (!strcmp(temporary->value,"NUM"))
                {
                    quadTable[count].arg2.arg.arg_num = id->children->astnextSibling->tk_data.val;
                    quadTable[count].arg2.entry = 1;
                }
                else if (!strcmp(temporary->value, "RNUM"))
                {
                    quadTable[count].arg2.arg.arg_rnum = id->children->astnextSibling->tk_data.realVal;
                    quadTable[count].arg2.entry = 2;
                }
                else
                {
                    quadTable[count].arg2.arg.arg_var = id->children->astnextSibling->symbol_table_entry;
                    quadTable[count].arg2.entry = 0;
                }
            }
            strcpy(quadTable[count].op, "=");
            quadTable[count].result.arg.arg_var = id->children->symbol_table_entry;
            quadTable[count].result.entry = 0;
            count++;
        }
        // relational-op pe bhi same hi cheez? have removed boolean and relational ops for now
        else if (!strcmp(root->value, "PLUS") || !strcmp(root->value, "MINUS") || !strcmp(root->value, "MUL") || !strcmp(root->value, "DIV"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            printf("Before Operator appears called\n");
            operator_appears(root);
            printf("After Operator appears called\n");
        }
        else if (!strcmp(root->value, "AND") || !strcmp(root->value, "OR") || !strcmp(root->value, "LT") || !strcmp(root->value, "LE") || !strcmp(root->value, "GT") || !strcmp(root->value, "GE") || !strcmp(root->value, "NE") || !strcmp(root->value, "EQ"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel(); // label for the expression
            if (root->isChildOfAssign)
            {
                printf("operator\n");
                operator_appears(root);
            }
            else
            { // WHILE ka case
                printf("while expr\n");
                if(!strcmp(root->value,"AND") ||!strcmp(root->value,"OR")){
                    while_expression_codeGen(root->children);
                    while_expression_codeGen(root->children->astnextSibling);
                }
                else    
                    while_expression_codeGen(root);

            }
        }
        else if (!strcmp(root->value, "U_MINUS") || !strcmp(root->value, "U_PLUS"))
        {   printf("Entered for %s \n",root->value);
            // set operator
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            strcpy(quadTable[count].op, root->value);
            printf("After strcpy\n");
            TYPE t;
            struct treeNode *rightChild = root->children->astnextSibling;
            if (rightChild->temp_variable_entry != NULL){
                quadTable[count].arg1.arg.arg_var = rightChild->temp_variable_entry;
                quadTable[count].arg1.entry = 0;
                t = rightChild->temp_variable_entry->type;
            }
            else if (!strcmp(rightChild->value, "ID")){
                quadTable[count].arg1.arg.arg_var = rightChild->symbol_table_entry;
                quadTable[count].arg1.entry = 0;
                t = rightChild->symbol_table_entry->type;
            }
            else if (!strcmp(rightChild->value, "NUM")){
                quadTable[count].arg1.arg.arg_num = rightChild->tk_data.val;
                quadTable[count].arg1.entry = 1;
                t.id_type.id_dt = "INTEGER";
            }
            else{ // RNUM case
                quadTable[count].arg1.arg.arg_rnum = rightChild->tk_data.realVal;
                quadTable[count].arg1.entry = 2;
                t.id_type.id_dt = "REAL";
            }
            quadTable[count].arg2.arg.arg_var = NULL;
            quadTable[count].arg2.entry = 0;
            
            // NUM KA SYMBOL TABLE ENTRY NAHI HAI
            ST_ENTRY *tempVarEntry = newTemp(t);
           
            quadTable[count].result.arg.arg_var = tempVarEntry;
            quadTable[count].result.entry = 0;
            root->temp_variable_entry = tempVarEntry; // set temp_variable entry of node
            count++;
        }
        // aakhri jagah
        else if (!strcmp(root->value, "GET-VALUE"))
        {
            // set operator
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel(); // giving construct label to get-value
            strcpy(quadTable[count].op, "GET");
            quadTable[count].arg1.arg.arg_var = NULL;
            quadTable[count].arg1.entry = 0;            
            quadTable[count].arg2.arg.arg_var = NULL;
            quadTable[count].arg2.entry = 0;
            quadTable[count].result.arg.arg_var = root->children->symbol_table_entry;
            quadTable[count].result.entry = 0;
            count++;
        }
        else if (!strcmp(root->value, "PRINT"))
        {
            // set operator
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel(); // giving construct label to print
            strcpy(quadTable[count].op, "PRINT");
            quadTable[count].arg1.arg.arg_var = NULL;
            quadTable[count].arg1.entry = 0;
            quadTable[count].arg2.arg.arg_var = NULL;
            quadTable[count].arg2.entry = 0;
            if (!strcmp(root->children->value, "NUM"))
            {
                quadTable[count].result.arg.arg_num = root->children->tk_data.realVal;
                quadTable[count].result.entry = 1;
            }
            else if (!strcmp(root->children->value, "RNUM"))
            {
                quadTable[count].result.arg.arg_rnum = root->children->tk_data.val;
                quadTable[count].result.entry = 2;
            }
            else
            { // boolean case
                quadTable[count].result.arg.arg_var = root->children->symbol_table_entry;
                quadTable[count].result.entry = 0;
            }
            count++;
        }
        else if (!strcmp(root->value, "WHILE"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            while_appears(root);
        }
        else if (!strcmp(root->value, "FOR"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            for_appears_bottom(root);
        }
        else if (!strcmp(root->value, "SWITCH"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
        }
        else if (!strcmp(root->value, "MODULE-INVOKE"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            struct treeNode* moduleID = root->children;
            struct treeNode* arguments = root->children->astnextSibling->children;
            if(arguments->astnextSibling != NULL){
                //optional statment
                while(arguments!=NULL){
                    quadTable[count].instruction = "RETURN-TO";
                    quadTable[count].result.arg.arg_var = arguments->symbol_table_entry;
                    quadTable[count].result.entry = 0;
                    count++;
                    arguments = arguments->next;
                }
                arguments = arguments->astnextSibling;
            }
            while(arguments!=NULL){
                //idList
                quadTable[count].instruction = "MODULE-INVOKE";
                quadTable[count].result.arg.arg_var = arguments->symbol_table_entry;
                quadTable[count].result.entry = 0;
                count++;
                arguments = arguments->next;
            }
        }
        
        else if (!strcmp(root->value, "DECLARE"))
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            printf("Inside declare\n");
            struct treeNode* variables = root->children->astnextSibling;
            while(variables != NULL)
            {
                //quadTable[count].instruction = "declare";
                strcpy(quadTable[count].op,"declare");
                quadTable[count].result.arg.arg_var = variables->symbol_table_entry;
                quadTable[count].result.entry = 0;
                // quadTable[count].arg1.arg_var = NULL;
                // quadTable[count].arg2.arg_var = NULL;
                count++;
                variables = variables->next;
            }
            printf("after declare\n");
        }


        // check-kar lena ek baar switch(essentially checking for case stmts)
        if (root->parent!=NULL && !strcmp(root->parent->value, "SWITCH") && root->pair != NULL)
        {
            // if (root->constructLabel == NULL)
            //     root->constructLabel = newlabel();
            printf("hello");
            case_appears_bottom(root);
        }
        // check for default
        if (root->parent!=NULL && !strcmp(root->parent->value, "SWITCH") && root->isDefault == 1)
        {
            printf("Enters here\n");
            // if (root->constructLabel = NULL)
            //     root->constructLabel = newlabel();
            quadTable[count].instruction = "GOTO";
            if (root->parent->next == NULL && !strcmp(root->parent->parent->value, "MODULE_DEF") && !strcmp(root->parent->parent->value,"PROGRAM"))
            {
                quadTable[count].goTolabel = "return";
            }
            else if (root->parent->next == NULL && root->parent->parent->parent!=NULL && strcmp(root->parent->parent->parent->value, "SWITCH"))
            {
                quadTable[count].goTolabel = root->parent->parent->constructLabel;
            }
            // KYA YE SHI HAI?? Bhagwan jaane
            else if (root->parent->next == NULL)
            {
                struct treeNode *temp = root->parent->parent;
                while (strcmp(temp->value, "MODULE_DEF") && strcmp(temp->value, "PROGRAM") && temp->next == NULL)
                    temp = temp->parent;
                if (!strcmp(temp->value, "MODULE_DEF") || !strcmp(temp->value,"PROGRAM"))
                    quadTable[count].goTolabel = "return";
                else
                    quadTable[count].goTolabel = temp->next->constructLabel;
            }
            else
            {
                quadTable[count].goTolabel = root->parent->next->constructLabel;
            }
            quadTable[count].arg1.arg.arg_var = NULL;
            quadTable[count].arg1.entry = 0;
            quadTable[count].arg2.arg.arg_var = NULL;
            quadTable[count].arg2.entry = 0;
            quadTable[count].result.arg.arg_var = NULL;
            quadTable[count].result.entry = 0;
            quadTable[count].label = NULL;
            count++;
        }
    }

    // if (root->pair != NULL)
    //     IRcodegenerate(root->pair);

    root = root->next;
    if (root != NULL)
    {
        IRcodegenerate(root);
        // root = root->next;
    }
}

// int main(){
//     return 0;
// }