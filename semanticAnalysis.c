#include "symbolTable.c"

// to populate symbol table and open new symbol tables

// 0 for int, 1 for real, 2 for bool, -1 for error
int get_type(struct treeNode *node, struct id_symbol_table *table) // to get type for expression construct
{
    if (node == NULL)
    {
        printf("\n Node null inside get_type");
    }

    // if there is no child (expression is either id/num/rnum/true/false) **array access has children and is handled below**
    if (node->children == NULL || !strcmpi(node->value, "U1_MINUS") || !strcmpi(node->value, "U1_PLUS") || !strcmpi(node->value, "U_MINUS") || !strcmpi(node->value, "U_PLUS") || !strcmpi(node->value,"ARRAY_ACCESS")) 
    {   
        if (!strcmpi(node->value, "ID"))
        {
            ST_ENTRY *temp = get_lexeme(table, node->tk_data.lexeme);
            char *datatype = temp->type.id_type.id_dt;

            // id should not be of array type
            if (strlen(datatype) == 0)
            {
                printf("\n Expression cannot contain array type variable. ");
                return -1;
            }
            if (!strcmpi(datatype, "INTEGER"))
            {
                return 0;
            }
            else if (!strcmpi(datatype, "REAL"))
            {
                return 1;
            }
            else if (!strcmpi(datatype, "BOOLEAN"))
            {
                return 2;
            }
        }
        if (!strcmpi(node->value, "NUM"))
        {
            return 0;
        }
        if (!strcmpi(node->value, "RNUM"))
        {
            return 1;
        }
        if (!strcmpi(node->value, "TRUE") || !strcmpi(node->value, "FALSE"))
        {
            return 2;
        }
        if (!strcmpi(node->value, "U1_MINUS") || !strcmpi(node->value, "U1_PLUS"))
        {
            return get_type(node->children->astnextSibling, table);
        }
        if (!strcmpi(node->value, "U_MINUS") || !strcmpi(node->value, "U_PLUS"))
        {
            return get_type(node->children->astnextSibling, table);
        }
            if (!strcmpi(node->value, "ARRAY_ACCESS"))
    {   
        int ans;
        ST_ENTRY *temp = get_lexeme(table, node->children->tk_data.lexeme);
        if(temp==NULL)printf("Array not declared previously");
        char *datatype;
        datatype = temp->type.arr_type.arr_dt;
        if (!strcmpi(datatype, "integer"))
        {
            ans = 0;
        }
        else if (!strcmpi(datatype, "real"))
        {
            ans = 1;
        }
        else if (!strcmpi(datatype, "boolean"))
        {
            ans = 2;
        }
        // check array bounds if array index is a num
        int lr = temp->type.arr_type.lowRange.start;
        int hr = temp->type.arr_type.highRange.end;
        if (!strcmpi(node->children->astnextSibling->value, "NUM"))
        {
            int arr_index = node->children->astnextSibling->tk_data.val;
            if (arr_index >= lr && arr_index < hr)
            {
                printf("\n Array index: %d falls in range, ans = %d ", node->children->astnextSibling->tk_data.val,ans);

            }
            else
            {
                printf("\n Array index: %d not in range ", node->children->astnextSibling->tk_data.val);
                return -1;
            }
        }

        // check array index type (should be integer)
        int array_expr_type = get_type(node->children->astnextSibling, table);

        if (array_expr_type != 0)
        {
            printf("\n Array index is not integer, at line no: %d", node->children->line_no);
            return -1;
        }
        return ans;
    }
    }

    // if there are 2 children of an operator node (left operand, right operand)
    int leftOpType = get_type(node->children, table);
    int rightOpType = get_type(node->children->astnextSibling, table);
    int ans;

    if (leftOpType == -1 || rightOpType == -1) // if there is a type error coming from below
    {
        return -1;
    }

    if (!strcmpi(node->value, "PLUS") || !strcmpi(node->value, "MINUS"))
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

    if (!strcmpi(node->value, "DIV"))
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

    if (!strcmpi(node->value, "MUL"))
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
    if (!strcmpi(node->value, "AND") || !strcmpi(node->value, "OR"))
    {
        if (leftOpType == 2 && rightOpType == 2)
        {
            return 2;
        }
        else
        {
            printf("\n Incompatible AND/OR ");
            return -1;
        }
    }
    if (!strcmpi(node->value, "LT") || !strcmpi(node->value, "LE") || !strcmpi(node->value, "GT") || !strcmpi(node->value, "GE") || !strcmpi(node->value, "EQ") || !strcmpi(node->value, "NE"))
    {
        if ((leftOpType == 0 && rightOpType == 0) || (leftOpType == 1 && rightOpType == 1))
        {
            return 2;
        }
        else
        {
            printf("\n Incompatible relational op ");
            return -1;
        }
    }
}

int check_assignment(struct treeNode *root, struct id_symbol_table *table) //-1 error, 1 fine, root = LVALUEID/LVALUEARR
{
    if (root == NULL)
    {
        printf("\n root null inside check_assignment"); 
    }
    int lval;
    struct treeNode *lchild = root->children;

    if (!strcmpi(root->value, "LVALUEID"))
    { // lchild will be id
        ST_ENTRY *temp = get_lexeme(table, lchild->tk_data.lexeme);
        char *datatype = temp->type.id_type.id_dt;
        if (!strcmpi(datatype, "INTEGER"))
        {
            lval = 0;
        }
        else if (!strcmpi(datatype, "REAL"))
        {
            lval = 1;
        }
        else if (!strcmpi(datatype, "BOOLEAN"))
        {
            lval = 2;
        }
        int rval = get_type(lchild->astnextSibling, table);
        if (lval == rval)
        {
            printf("\n Assignment statement type match \n");
            return 1;
        }
        else
        {
            printf("\n Assignment statement type mis-match \n");
            return -1;
        }
    }
    else if (!strcmpi(root->value, "LVALUEARRAY"))
    { // lchild will be array access
        ST_ENTRY *temp = get_lexeme(table, lchild->children->tk_data.lexeme);
        char *datatype = temp->type.arr_type.arr_dt;
        if (!strcmpi(datatype, "INTEGER"))
        {
            lval = 0;
        }
        else if (!strcmpi(datatype, "REAL"))
        {
            lval = 1;
        }
        else if (!strcmpi(datatype, "BOOLEAN"))
        {
            lval = 2;
        }
        int rval = get_type(lchild->astnextSibling, table);
        if (lval == rval)
        {
            printf("\n Assignment statement type match, types = %d ", lval);
            return 1;
        }
        else
        {
            printf("\n Assignment statement type mis-match, lval type = %d, rval type = %d ", lval, rval);
            return -1;
        }
    }
}

int check_while(struct treeNode *root, struct id_symbol_table *table)
{
    // Rule is : iterativeStmt.addr = make_new_node("WHILE",arithmeticOrBooleanExpr.addr,statements.syn);
    int type = get_type(root->children, table);
    if (type == 2)
    {
        printf("\n While condition is boolean \n");
        return 1;
    }
    else
    {
        printf("\n While condition is not boolean \n");
        return -1;
    }
}

int check_function_call(struct treeNode *root, struct id_symbol_table *table)
{
    // Semantic rule is : moduleReuseStmt=make_new_node(“MODULE-INVOKE”,ID.addr,(make_new_node(“ARGUMENTS”,optional.syn,idList.syn));
    FN_ENTRY *func = get_func_name(fn_table, root->children->value);
    if (func == NULL)
    {
        printf("\n Function not defined");
        return -1;
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
            return -1;
        }

        ST_ENTRY *op_entry = get_lexeme(table, called_op_list->tk_data.lexeme);
        if (strcmpi(defined_op_list->parameter_type.id_type.id_dt, op_entry->type.id_type.id_dt)) // if comparison results in non 0 meaning both arent the same hence error
        {
            comparison_flag_type_checking = 1;
            printf("\n type doesnt match in function call, line no: %d \n", root->line_no);
            return -1;
        }
        called_op_list = called_op_list->next;
        defined_op_list = defined_op_list->next;
        if (defined_op_list == NULL && called_op_list != NULL)
        { // if one is null but the other is not null
            comparison_flag_type_checking = 1;
            printf("No of parameters is different in function call, line no: %d \n", root->line_no);
            return -1;
        }
    }

    // checking if input params in both lists have the same type
    while (defined_ip_list != NULL)
    {
        if (called_ip_list == NULL)
        { // if one is null but the other list is not null
            comparison_flag_type_checking = 1;
            printf("No of parameters is different in function call, line no: %d \n", root->line_no);
            return -1;
        }

        ST_ENTRY *ip_entry = get_lexeme(table, called_ip_list->tk_data.lexeme);

        // Flag to choose which struct to access to obtain type
        int flag_for_type = 1; // 1- to access id struct, 2 to access array struct

        if (!strcmpi(called_ip_list->children->value, "ARRAY_ACCESS"))
        {
            flag_for_type = 2;
        }
        if (flag_for_type == 1 && strcmpi(defined_ip_list->parameter_type.id_type.id_dt, ip_entry->type.id_type.id_dt)) // if comparison results in non 0 meaning both arent the same hence error
        {
            comparison_flag_type_checking = 1;
            printf("\n type doesnt match in function call, line no: %d \n", root->line_no);
            return -1;
        }
        else if (flag_for_type == 2 && strcmpi(defined_ip_list->parameter_type.arr_type.arr_dt, ip_entry->type.arr_type.arr_dt)) // if comparison results in non 0 meaning both arent the same hence error
        {
            comparison_flag_type_checking = 1;
            printf("\n type doesnt match in function call, line no: %d \n", root->line_no);
            return -1;
        }
        called_ip_list = called_ip_list->next;
        defined_ip_list = defined_ip_list->next;
        if (defined_ip_list == NULL && called_ip_list != NULL)
        { // if one is null but the other is not null
            comparison_flag_type_checking = 1;
            printf("No of parameters is different in function call, line no: %d \n", root->line_no);
            return -1;
        }
    }


    //function cannot be invoked recursively
    if(table->parent_function!=NULL){
        FN_ENTRY* fn = table->parent_function;
        if(func == fn){
            printf("\n Recursive function call ");
            return -1;
        }
    }

    return 1; // all types match
}

int check_switch_case(struct treeNode *root, struct id_symbol_table *table)
{
    // conditionalStmt.syn = make_new_node(“SWITCH”,ID.addr, caseStmt.syn,default.syn);
    struct treeNode *switch_id = root->children;
    ST_ENTRY *temp = get_lexeme(table, switch_id->tk_data.lexeme);
    char *datatype = temp->type.id_type.id_dt;
    int switch_type;
    if (!strcmpi(datatype, "REAL"))
    {
        printf("\n Switch case cant have real value ");
        return -1;
    }
    else
    {
        if (!strcmpi(datatype, "INTEGER"))
        {
            switch_type = 0;
        }
        else
        {
            switch_type = 2;
        }
        struct treeNode *caseList = switch_id->astnextSibling;

        while (caseList != NULL)
        {
            if (!strcmpi(caseList->value, "NUM"))
            {
                if ((switch_type == 0))
                {
                    return 1;
                }
                else
                {
                    printf("\n Switch type is not integer, case type is integer");
                    return -1;
                }
            }
            else if (!strcmpi(caseList->value, "TRUE") || !strcmpi(caseList->value, "FALSE"))
            {
                if (switch_type == 2)
                {
                    return 1;
                }
                else
                {
                    printf("\n Switch type is not boolean, case type is boolean");
                    return -1;
                }
            }
            caseList = caseList->next;
        }

        struct treeNode *defaultNode = switch_id->astnextSibling->astnextSibling;
        if (switch_type == 0)
        {
            if (defaultNode == NULL)
            {
                printf("\n Switch cases for integer type must have default statement");
                return -1;
            }
        }
        if (switch_type == 2)
        {
            if (defaultNode != NULL)
            {
                printf("\n Switch cases for boolean type must not have default statement");
                return -1;
            }
        }
    }
}

void semanticAnalysis(struct treeNode *root, struct id_symbol_table *id_table, int nesting_num)
{
    if (root == NULL)
        return;
    int type_check;
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
        if (!strcmpi(root->value, "MODULE_DEF"))
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
                { //either module def is null, or op_list is null
                    if (child3->pair == NULL)
                    { //op_list is null,module def non null
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
                    { //module def null ,op list non null
                        op_list = makeListNode(child3);
                        create_entry_and_insert_in_FST(fn_table, child1, ip_list, op_list);
                    }
                }
                else
                {//both module def and op_list non null
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
            {//both module def and op_list is null
                create_entry_and_insert_in_FST(fn_table, child1, ip_list, NULL);

            }
        }
        else if (!strcmpi(root->value, "FOR"))
        {
            TYPE type;
            struct treeNode *rangeNode = root->children->astnextSibling;
            if (!strcmpi(rangeNode->children->children->value, "NUM"))
            {
                type.for_type.low_range = rangeNode->children->children->tk_data.val;
            }
            else
            {
                if (!strcmpi(rangeNode->children->children->value, "MINUS"))
                {
                    type.for_type.low_range = -1 * (rangeNode->children->children->astnextSibling->tk_data.val);
                }
                else
                {
                    type.for_type.low_range = rangeNode->children->children->astnextSibling->tk_data.val;
                }
            }

            if (!strcmpi(rangeNode->children->astnextSibling->children->value, "NUM"))
            {
                type.for_type.high_range = rangeNode->children->astnextSibling->children->tk_data.val;
            }
            else
            {
                if (!strcmpi(rangeNode->children->astnextSibling->children->value, "MINUS"))
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


            // Rule is iterativeStmt.addr = make_new_node("FOR",ID.addr ,range_for.addr,statements.syn);
            struct treeNode* child1= root->children;
            struct treeNode* statements = root->children->astnextSibling->astnextSibling;
            int flag_for_for_in_semantic_analysis = 1;
            while(statements!=NULL && flag_for_for_in_semantic_analysis){
                if(!strcmpi(statements->value, "LVALUEID")){
                    if(!strcmpi(statements->children->tk_data.lexeme,child1->tk_data.lexeme)){
                        printf("\n error- A for statement must not redefine the variable that participates in the iterating over the range, line no %d ",statements->children->line_no);
                        flag_for_for_in_semantic_analysis=0;
                        exit(1);
                    }
                }
                statements = statements->next;
            }
        }
        else if (!strcmpi(root->value, "DECLARE"))
        {
            struct treeNode *type_attribute = root->children;
            struct treeNode *idList = root->children->astnextSibling;
            TYPE t;
            if(!strcmpi(type_attribute->value,"ARRAY-DCL")){
                t.arr_type.arr_dt = type_attribute->children->astnextSibling->tk_data.lexeme;
                struct treeNode* li = type_attribute->children->children;
                struct treeNode* ri = li->astnextSibling;
        if(!strcmpi(li->children->value,"ID")){
            t.arr_type.lowRange.low_id = li->children->tk_data.lexeme;
            t.arr_type.isStatic = 0; //dynamic array
        }
        else if(!strcmpi(li->children->value,"NUM")){
            t.arr_type.lowRange.start = li->children->tk_data.val;
            t.arr_type.isStatic = 1; //static array
        }
        else{
            if(!strcmpi(li->children->astnextSibling->value,"ID"))
            {
                   t.arr_type.lowRange.low_id = strcat(strcat(li->children->value,","),li->children->astnextSibling->tk_data.lexeme);
                   t.arr_type.isStatic = 0; //dynamic array
            }
            else
            {
                if(!strcmpi(li->children->value,"MINUS"))
                    t.arr_type.lowRange.start = -1*li->children->astnextSibling->tk_data.val;
                else   
                    t.arr_type.lowRange.start = li->children->astnextSibling->tk_data.val;
                t.arr_type.isStatic = 1; //static array
            }
        }
        
        // ri //
        if(!strcmpi(ri->children->value,"ID")){
            t.arr_type.highRange.high_id = ri->children->tk_data.lexeme;
            t.arr_type.isStatic = 0; //dynamic array
        }
        else if(!strcmpi(ri->children->value,"NUM")){
            t.arr_type.highRange.end = ri->children->tk_data.val;
            t.arr_type.isStatic = 1; //static array
        }
        else{
            if(!strcmpi(ri->children->astnextSibling->value,"ID"))
            {
                   t.arr_type.highRange.high_id = strcat(strcat(ri->children->value,","),ri->children->astnextSibling->tk_data.lexeme);
                   t.arr_type.isStatic = 0; //dynamic array
            }
            else
            {
                if(!strcmpi(ri->children->value,"MINUS"))
                    t.arr_type.highRange.end = -1*ri->children->astnextSibling->tk_data.val;
                else   
                    t.arr_type.highRange.end = ri->children->astnextSibling->tk_data.val;
                t.arr_type.isStatic = 1; //static array
            }
        }
        
            }
            else{
                t.id_type.id_dt = type_attribute->value;
                 printf("ID me populate:%s ",t.id_type.id_dt);
            }
            
            while (idList != NULL)
            {
                create_entry_and_insert(id_table, idList, t);
                idList = idList->next;
            }
        }
        else if (!strcmpi(root->value, "WHILE"))
        {
            type_check = check_while(root, id_table); // checks if while has a boolean expr
            if (type_check == -1)
            {
                exit(1);
            }
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

        else if (!strcmpi(root->value, "SWITCH"))
        {
            type_check = check_switch_case(root, id_table); // checks if while has a boolean expr
            if (type_check == -1)
            {
                exit(1);
            }
            struct treeNode *caseList = root->children->astnextSibling;
            struct treeNode *defaultStmt = caseList->astnextSibling;
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
        }

        else if (!strcmpi(root->value, "LVALUEID") || !strcmpi(root->value, "LVALUEARR"))
        {          

            type_check = check_assignment(root, id_table);
            if (type_check == -1)
            {
                exit(1);
            }

             // Setting is_used in that function to be one to idnetify that that variable has been assigned a value in that function

            int hash_value = get_sym_table_hash(root->children->tk_data.lexeme);
            if (id_table->arr[hash_value] != NULL)
            {
                ST_ENTRY *temp = id_table->arr[hash_value];
                int found_entry_for_assign_op = 1;
                while (temp && found_entry_for_assign_op)
                {
                    if (!strcmpi(temp->id_lexeme, root->children->tk_data.lexeme))
                    {
                        temp->is_used = 1;
                        found_entry_for_assign_op = 0;
                    }
                    temp = temp->next;
                }
            }
        }
        // Checking if variable in for is assigned value anywhere inside loop
        

        // function call
        else if (!strcmpi(root->value, "MODULE-INVOKE"))
        {
            type_check = check_function_call(root, id_table);
            if (type_check == -1)
            {
                exit(1);
            }

        }
    }

    if (root->children != NULL)
    {
        struct treeNode *temp;
        temp = root->children->astnextSibling;
        // z = x + arr;
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
    //if statements executing are of function block
    if(id_table->parent_function!=NULL){
        printf("Entered gadbad area");
        //o.p list check
                            //every output param should be assigned value inside the fn
                FN_ENTRY* fn = id_table->parent_function;

                    LISTNODE* temp = fn->op_head;
                    while(temp!=NULL){
                        char* op_param = temp->parameter_name;
                        ST_ENTRY* t = get_lexeme(id_table,op_param);
                        if(t==NULL){
                            printf("\n Output param not declared inside fn definition");
                        }
                        else if(t->is_used!=1){
                            printf("\n Output param:%s not assigned value inside the function.",op_param);
                            
                        }
                        temp = temp->next;
                    }
    }
}