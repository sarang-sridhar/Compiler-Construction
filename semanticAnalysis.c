#include "symbolTable.c"

void semanticAnalysis(struct treeNode *root, struct id_symbol_table *id_table, int nesting_num)
{
    if (root == NULL)
        return;

    /*pass symbol table in which data is to be inserted in the function call
    Constructs to be checked:- MODULE_DEF,FOR,DECLARE*/
    semanticAnalysis(root->children, id_table, nesting_num);

    if (root->addr != NULL)
    {
        semanticAnalysis(root->addr, id_table, nesting_num);
    }
    else
    {
        // check for respective nodes
        if (!strcmp(root->value, "MODULE_DEF"))
        {
            // check for pair
            struct treeNode *child1 = root->children;         // ID
            struct treeNode *child2 = child1->astnextSibling; // ip_list
            struct treeNode *child3 = child2->astnextSibling; // op_list
            struct treeNode *child4;                          // module_def
            LISTNODE *ip_list = makeListNode(child2);
            LISTNODE *op_list;
            if (child3 != NULL)
            {
                child4 = child3->astnextSibling;
                if (child4 == NULL)
                {
                    if (child3->pair == NULL)
                    {
                        FN_ENTRY *fn_entry = create_entry_and_insert_in_FST(fn_table, child1, ip_list, NULL);
                        // id-symbol table
                        struct id_symbol_table *child_table = initST(nesting_num);
                        child_table->parent_function = fn_entry;
                        if (fn_entry->child_table == NULL)
                            fn_entry->child_table = child_table;
                        else
                        {
                            struct id_symbol_table *temp = fn_entry->child_table;
                            if (temp->right_sibling != NULL)
                            {
                                temp = temp->right_sibling;
                            }
                            child_table->left_sibling = temp;
                            temp->right_sibling = child_table;
                        }

                        // change table being passed in function call
                        id_table = child_table;
                    }
                    else
                    {
                        op_list = makeListNode(child3);
                        create_entry_and_insert_in_FST(fn_table, child1, ip_list, op_list);
                    }
                }
                else
                {
                    op_list = makeListNode(child3);
                    FN_ENTRY *fn_entry = create_entry_and_insert_in_FST(fn_table, child1, ip_list, op_list);
                    // id-symbol table
                    struct id_symbol_table *child_table = initST(nesting_num);
                    child_table->parent_function = fn_entry;
                    if (fn_entry->child_table == NULL)
                        fn_entry->child_table = child_table;
                    else
                    {
                        struct id_symbol_table *temp = fn_entry->child_table;
                        if (temp->right_sibling != NULL)
                        {
                            temp = temp->right_sibling;
                        }
                        child_table->left_sibling = temp;
                        temp->right_sibling = child_table;
                    }
                    // change table being passed in function call
                    id_table = child_table;
                }
            }
            else
            {
                create_entry_and_insert_in_FST(fn_table, child1, ip_list, NULL);
            }
        }
        else if (!strcmp(root->value, "FOR"))
        {
            TYPE type;
            struct treeNode *rangeNode = root->children->astnextSibling;
            if (!strcmp(rangeNode->children->children->value, "NUM"))
            {
                type.for_type.low_range = rangeNode->children->children->tk_data.val;
            }
            else
            {
                if (!strcmp(rangeNode->children->children->value, "MINUS"))
                {
                    type.for_type.low_range = -1 * (rangeNode->children->children->astnextSibling->tk_data.val);
                }
                else
                {
                    type.for_type.low_range = rangeNode->children->children->astnextSibling->tk_data.val;
                }
            }

            if (!strcmp(rangeNode->children->astnextSibling->children->value, "NUM"))
            {
                type.for_type.high_range = rangeNode->children->astnextSibling->children->tk_data.val;
            }
            else
            {
                if (!strcmp(rangeNode->children->astnextSibling->children->value, "MINUS"))
                {
                    type.for_type.high_range = -1 * (rangeNode->children->astnextSibling->children->astnextSibling->tk_data.val);
                }
                else
                {
                    type.for_type.high_range = rangeNode->children->astnextSibling->children->astnextSibling->tk_data.val;
                }
            }

            struct id_symbol_table *child_table = initST(nesting_num + 1);
            create_entry_and_insert(child_table, root->children, type);
            child_table->parent_table = id_table;
            if (id_table->child_table == NULL)
            {
                id_table->child_table = child_table;
            }
            else
            {
                struct id_symbol_table *temp = id_table->child_table;
                while (temp->right_sibling != NULL)
                {
                    temp = temp->right_sibling;
                }
                child_table->left_sibling = temp;
                temp->right_sibling = child_table;
            }
            id_table = child_table;
        }
        else if (!strcmp(root->value, "DECLARE"))
        {
            struct treeNode *type_attribute = root->children;
            struct treeNode *idList = root->children->astnextSibling;
            TYPE t;
            t.id_type.id_dt = type_attribute->value;
            while (idList != NULL)
            {
                create_entry_and_insert(id_table, idList, t);
                idList = idList->next;
            }
        }
        else if (!strcmp(root->value, "WHILE"))
        {
            struct id_symbol_table *child_table = initST(nesting_num + 1);
            child_table->parent_table = id_table;
            if (id_table->child_table == NULL)
            {
                id_table->child_table = child_table;
            }
            else
            {
                struct id_symbol_table *temp = id_table->child_table;
                while (temp->right_sibling != NULL)
                {
                    temp = temp->right_sibling;
                }
                child_table->left_sibling = temp;
                temp->right_sibling = child_table;
            }
            id_table = child_table;
        }

        else if (!strcmp(root->value, "SWITCH"))
        {
            struct treeNode *caseList = root->children;
            struct treeNode *defaultStmt = root->children->astnextSibling;
            while (caseList != NULL)
            {
                // value->pair = stmts
                struct id_symbol_table *child_table = initST(nesting_num + 1);
                child_table->parent_table = id_table;
                if (id_table->child_table == NULL)
                {
                    id_table->child_table = child_table;
                }
                else
                {
                    struct id_symbol_table *temp = id_table->child_table;
                    while (temp->right_sibling != NULL)
                    {
                        temp = temp->right_sibling;
                    }
                    child_table->left_sibling = temp;
                    temp->right_sibling = child_table;
                }
                id_table = child_table;

                struct treeNode *stmts = caseList->pair;

                while (stmts != NULL)
                {
                    semanticAnalysis(stmts, id_table, nesting_num);
                    stmts = stmts->next;
                }
                caseList = caseList->next;
            }

            struct treeNode *stmts = defaultStmt->pair; // right child
            // open new symbol table
            struct id_symbol_table *child_table = initST(nesting_num + 1);
            child_table->parent_table = id_table;
            if (id_table->child_table == NULL)
            {
                id_table->child_table = child_table;
            }
            else
            {
                struct id_symbol_table *temp = id_table->child_table;
                while (temp->right_sibling != NULL)
                {
                    temp = temp->right_sibling;
                }
                child_table->left_sibling = temp;
                temp->right_sibling = child_table;
            }
            id_table = child_table;
            while (stmts != NULL)
            {
                semanticAnalysis(stmts, id_table, nesting_num);
                stmts = stmts->next;
            }
        }
    }

    if (root->children != NULL)
    {
        struct treeNode *temp;
        temp = root->children->astnextSibling;

        while (temp != NULL)
        {
            semanticAnalysis(temp, id_table, nesting_num);
            temp = temp->astnextSibling;
        }
    }

    // needs to happen only for statements wala node
    root = root->next;
    if (root != NULL)
    {
        semanticAnalysis(root, id_table, nesting_num);
    }
}

// 0 for int, 1 for real, 2 for bool, -1 for error
int get_type(struct treeNode *node, struct id_symbol_table *table)
{
    if (node == NULL)
    {
        return;
    }

    int leftOpType = get_type(node->children, table);
    int rightOpType = get_type(node->children->astnextSibling, table);
    int ans;

    if (leftOpType == -1 || rightOpType == -1)
    {
        return -1;
    }

    if (!strcmp(node->value, "PLUS") || !strcmp(node->value,"MINUS"))
    {
        if (leftOpType == 0 && rightOpType == 0)
        {
            return 0;
        }
        if (leftOpType == 1 && rightOpType == 1)
        {
            return 1;
        }
        else
        {
            printf("\n Incompatible addition/subtraction \n");
            return -1;
        }
    }

    if (!strcmp(node->value, "DIV"))
    {
        if ((leftOpType == 0 && rightOpType == 0) || (leftOpType == 1 && rightOpType == 1) || (leftOpType == 0 && rightOpType == 1) || (leftOpType == 1 && rightOpType == 0))
        {
            return 1;
        }
        else
        {
            printf("\n Incompatible Division");
            return -1;
        }
    }

    if (!strcmp(node->value, "MUL"))
    {
        if ((leftOpType == 1 && rightOpType == 1))
        {
            return 1;
        }
        else if ((leftOpType == 0 && rightOpType == 0))
        {
            return 0;
        }
        else
        {
            printf("\n Incompatible Multiplication");
            return -1;
        }
    }
    if(!strcmp(node->value,"AND") || !strcmp(node->value,"OR")){
        if(leftOpType==2 && rightOpType==2){
            return 2;
        }
        else{
            printf("\n Incompatible AND/OR ");
            return -1;
        }
    }
    if(!strcmp(node->value,"LT") || !strcmp(node->value,"LE") || !strcmp(node->value,"GT") || !strcmp(node->value,"GE") || !strcmp(node->value,"EQ") || !strcmp(node->value,"NE")){
        if((leftOpType==0 && rightOpType==0) || (leftOpType==1 && rightOpType==1)){
            return 2;
        }
        else{
            printf("\n Incompatible relational op ");
            return -1;
        }
    }
    if (!strcmp(node->value, "ARRAY_ACCESS"))
    {
        ST_ENTRY *temp = get_lexeme(table, node->children->tk_data.lexeme);
        char *datatype = temp->type.arr_type.arr_dt;
        if (!strcmp(datatype, "INTEGER"))
        {
            ans = 0;
        }
        else if (!strcmp(datatype, "REAL"))
        {
            ans = 1;
        }
        else if (!strcmp(datatype, "BOOLEAN"))
        {
            ans = 2;
        }
        // check array bounds
        int lr = temp->type.arr_type.lowRange.start;
        int hr = temp->type.arr_type.highRange.end;
        if (!strcmp(node->children->astnextSibling->value, "NUM"))
        {
            int arr_index = node->children->astnextSibling->tk_data.val;
            if (arr_index >= lr && arr_index < hr)
            {
                printf("\n Array index falls in range ");
            }
            else
            {
                printf("\n Array index not in range ");
                return -1;
            }
        }

        return ans;
    }
    if (!strcmp(node->value, "ID"))
    {
        ST_ENTRY *temp = get_lexeme(table, node->tk_data.lexeme);
        char *datatype = temp->type.id_type.id_dt;
        if (!strcmp(datatype, "INTEGER"))
        {
            ans = 0;
        }
        else if (!strcmp(datatype, "REAL"))
        {
            ans = 1;
        }
        else if (!strcmp(datatype, "BOOLEAN"))
        {
            ans = 2;
        }
        return ans;
    }
}

int check_assignment(struct treeNode *root, struct id_symbol_table *table) //-1 error, 1 fine, root = LVALUEID/LVALUEARR
{
    if (root == NULL)
    {
        return;
    }
    int lval;
    struct treeNode* lchild = root->children;

    if(!strcmp(root->value,"LVALUEID")){ //lchild will be id
        ST_ENTRY* temp = get_lexeme(table,lchild->tk_data.lexeme);
        char* datatype = temp->type.id_type.id_dt;
        if (!strcmp(datatype, "INTEGER"))
        {
            lval = 0;
        }
        else if (!strcmp(datatype, "REAL"))
        {
            lval = 1;
        }
        else if (!strcmp(datatype, "BOOLEAN"))
        {
            lval = 2;
        }
        int rval = get_type(lchild->astnextSibling,table);
        if(lval == rval){
            printf("\n Assignment statement type match \n");
            return 1;
        }
        else{
            printf("\n Assignment statement type mis-match \n");
            return -1;

        }
    }
    else if(!strcmp(root->value,"LVALUEARRAY")){  //lchild will be array access
        ST_ENTRY* temp = get_lexeme(table,lchild->children);
        char* datatype = temp->type.arr_type.arr_dt;
        if (!strcmp(datatype, "INTEGER"))
        {
            lval = 0;
        }
        else if (!strcmp(datatype, "REAL"))
        {
            lval = 1;
        }
        else if (!strcmp(datatype, "BOOLEAN"))
        {
            lval = 2;
        }
        int rval = get_type(lchild->astnextSibling,table);
        if(lval == rval){
            printf("\n Assignment statement type match \n");
            return 1;
        }
        else{
            printf("\n Assignment statement type mis-match \n");
            return -1;
        }

    }
}

int check_function_call(struct treeNode *root, struct id_symbol_table *table)
{
    // Semantic rule is : moduleReuseStmt=make_new_node(“MODULE-INVOKE”,ID.addr,(make_new_node(“ARGUMENTS”,optional.syn,idList.syn));
    FN_ENTRY *func = get_func_name(fn_table, root->children->value);
    if (func == NULL)
    {
        printf("Function not defined");
        return 0;
    }
    struct treeNode *called_op_list = root->children->astnextSibling->children;

    struct treeNode *called_ip_list = root->children->astnextSibling->children->astnextSibling;
    int comparison_flag_type_checking = 0;
    LISTNODE *defined_op_list = func->op_head;
    LISTNODE *defined_ip_list = func->ip_head;

    // checking if output params in both lists have the same type
    while (defined_op_list != NULL)
    {
        if (called_op_list == NULL)
        { // if one is null but the other list is not null
            comparison_flag_type_checking = 1;
            printf("No of parameters is different in function call, line no: %d \n", root->line_no);
            return 0;
        }

        ST_ENTRY *op_entry = get_lexeme(table, called_op_list->addr->value);
        if (strcmp(defined_op_list->parameter_type.id_type.id_dt, op_entry->type.id_type.id_dt)) // if comparison results in non 0 meaning both arent the same hence error
        {
            comparison_flag_type_checking = 1;
            printf("\n type doesnt match in function call, line no: %d \n", root->line_no);
            return 0;
        }
        called_op_list = called_op_list->next;
        defined_op_list = defined_op_list->next;
        if (defined_op_list == NULL && called_op_list != NULL)
        { // if one is null but the other is not null
            comparison_flag_type_checking = 1;
            printf("No of parameters is different in function call, line no: %d \n", root->line_no);
            return 0;
        }
    }

    // checking if input params in both lists have the same type
    while (defined_ip_list != NULL)
    {
        if (called_ip_list == NULL)
        { // if one is null but the other list is not null
            comparison_flag_type_checking = 1;
            printf("No of parameters is different in function call, line no: %d \n", root->line_no);
            return 0;
        }

        ST_ENTRY *ip_entry = get_lexeme(table, called_ip_list->addr->value);

        // Flag to choose which struct to access to obtain type
        int flag_for_type = 1; // 1- to access id struct, 2 to access array struct, 3 to access for loop struct

        if (strcmp(called_ip_list->children->value, "ARRAY_ACCESS"))
        {
            flag_for_type = 2;
        }
        else if (strcmp(called_ip_list->children->value, "FOR"))
        {
            flag_for_type = 3;
        }

        if (flag_for_type == 1 && strcmp(defined_ip_list->parameter_type.id_type.id_dt, ip_entry->type.id_type.id_dt)) // if comparison results in non 0 meaning both arent the same hence error
        {
            comparison_flag_type_checking = 1;
            printf("\n type doesnt match in function call, line no: %d \n", root->line_no);
            return 0;
        }
        else if (flag_for_type == 2 && strcmp(defined_ip_list->parameter_type.arr_type.arr_dt, ip_entry->type.arr_type.arr_dt)) // if comparison results in non 0 meaning both arent the same hence error
        {
            comparison_flag_type_checking = 1;
            printf("\n type doesnt match in function call, line no: %d \n", root->line_no);
            return 0;
        }
        called_ip_list = called_ip_list->next;
        defined_op_list = defined_ip_list->next;
        if (defined_ip_list == NULL && called_ip_list != NULL)
        { // if one is null but the other is not null
            comparison_flag_type_checking = 1;
            printf("No of parameters is different in function call, line no: %d \n", root->line_no);
            return 0;
        }
    }

    return 1; // all types match
}