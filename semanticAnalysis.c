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
    if (node->children == NULL || !strcasecmp(node->value, "U1_MINUS") || !strcasecmp(node->value, "U1_PLUS") || !strcasecmp(node->value, "U_MINUS") || !strcasecmp(node->value, "U_PLUS") || !strcasecmp(node->value,"ARRAY_ACCESS")) 
    {   
        if (!strcasecmp(node->value, "ID"))
        {
            printf("\n**get lexeme called for:%s \n",node->tk_data.lexeme);
            ST_ENTRY *temp = get_lexeme(table, node->tk_data.lexeme);
           
            printf("\n**get lexeme after call for:%s \n",temp->id_lexeme);
            if(temp==NULL){
                printf("ID IS NULL");
                return -1;
            }
            printf("\n HEre before type\n");
            char *datatype = temp->type.id_type.id_dt;
            printf("\nTYPE is:%s \n",datatype);
            printf("\nHERE AFTER PRINTING TYPE\n");
            // id should not be of array type
            if (strlen(datatype) == 0)
            {
                printf("\n Expression cannot contain array type variable. ");
                return -1;
            }
            if (!strcasecmp(datatype, "INTEGER"))
            {
                return 0;
            }
            else if (!strcasecmp(datatype, "REAL"))
            {
                return 1;
            }
            else if (!strcasecmp(datatype, "BOOLEAN"))
            {
                return 2;
            }
        }
        if (!strcasecmp(node->value, "NUM"))
        {
            return 0;
        }
        if (!strcasecmp(node->value, "RNUM"))
        {
            return 1;
        }
        if (!strcasecmp(node->value, "TRUE") || !strcasecmp(node->value, "FALSE"))
        {
            return 2;
        }
        if (!strcasecmp(node->value, "U1_MINUS") || !strcasecmp(node->value, "U1_PLUS"))
        {
            return get_type(node->children->astnextSibling, table);
        }
        if (!strcasecmp(node->value, "U_MINUS") || !strcasecmp(node->value, "U_PLUS"))
        {   
        
            return get_type(node->children->astnextSibling, table);
        }
            if (!strcasecmp(node->value, "ARRAY_ACCESS"))
    {   
        int ans;
        ST_ENTRY *temp = get_lexeme(table, node->children->tk_data.lexeme);
        if(temp==NULL){return -1;}
        char *datatype;
        datatype = temp->type.arr_type.arr_dt;
        if (!strcasecmp(datatype, "integer"))
        {
            ans = 0;
        }
        else if (!strcasecmp(datatype, "real"))
        {
            ans = 1;
        }
        else if (!strcasecmp(datatype, "boolean"))
        {
            ans = 2;
        }
        // check array bounds if array index is a num
        int lr = temp->type.arr_type.lowRange.start;
        int hr = temp->type.arr_type.highRange.end;
        if (!strcasecmp(node->children->astnextSibling->value, "NUM"))
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

    if (!strcasecmp(node->value, "PLUS") || !strcasecmp(node->value, "MINUS"))
    {
        printf("being plus \n");
        if (leftOpType == 0 && rightOpType == 0)
        {
            printf("correct %s\n",node->value);
            return 0;
        }
        if (leftOpType == 1 && rightOpType == 1)
        {
            printf(" sahi hai \n");
            return 1;
        }
        else
        {
            printf("idhar \n");
            printf("\n Incompatible addition/subtraction %s leftOp:%d rightOp:%d \n",node->value,leftOpType,rightOpType);
            return -1;
        }
    }

    if (!strcasecmp(node->value, "DIV"))
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

    if (!strcasecmp(node->value, "MUL"))
    {
        printf("being mul \n");
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
    if (!strcasecmp(node->value, "AND") || !strcasecmp(node->value, "OR"))
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
    if (!strcasecmp(node->value, "LT") || !strcasecmp(node->value, "LE") || !strcasecmp(node->value, "GT") || !strcasecmp(node->value, "GE") || !strcasecmp(node->value, "EQ") || !strcasecmp(node->value, "NE"))
    {
        if ((leftOpType == 0 && rightOpType == 0) || (leftOpType == 1 && rightOpType == 1))
        {
            printf("Returning \n");
            return 2;
        }
        else
        {
            printf("\n Incompatible relational op \n");
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
    // printf("\nRoot is:%s \n",root->value);
    if (!strcasecmp(root->value, "LVALUEID"))
    { // lchild will be id

        if(!strcmp(root->children->value,"ID") &&!strcmp(root->children->astnextSibling->value,"ID")){
        struct treeNode* lchild = root->children;
        struct treeNode* rchild = root->children->astnextSibling;
        ST_ENTRY* lentry = get_lexeme(table,lchild->tk_data.lexeme);
        ST_ENTRY* rentry = get_lexeme(table,rchild->tk_data.lexeme);
        if(lentry!=NULL && rentry!=NULL){
            printf("\n**Entered here for:%s ",lchild->tk_data.lexeme);
            if(lentry->is_array ==1 && rentry->is_array == 1){
                if(!strcasecmp(lentry->type.arr_type.arr_dt,rentry->type.arr_type.arr_dt)){
                    if(lentry->type.arr_type.isStatic==1 && rentry->type.arr_type.isStatic==1){
                        if(lentry->type.arr_type.lowRange.start == rentry->type.arr_type.lowRange.start && (lentry->type.arr_type.highRange.end==rentry->type.arr_type.highRange.end)){
                            printf("\n Assignment statement type match(array) ");
                            return 1;
                        }
                    }
                }
                else{
                    printf("\n Assignment statement type mismatch(array)");
                    return -1;
                }
            }

            if(lentry->is_array + rentry->is_array > 0){
                printf("\n Assignment statement type mismatch(array - id)");
                return -1;
            }
        }
    }
        ST_ENTRY *temp = get_lexeme(table, lchild->tk_data.lexeme);
        if(temp==NULL){
            return -1;
        }
        char *datatype = temp->type.id_type.id_dt;
        if (!strcasecmp(datatype, "INTEGER"))
        {
            lval = 0;
        }
        else if (!strcasecmp(datatype, "REAL"))
        {
            lval = 1;
        }
        else if (!strcasecmp(datatype, "BOOLEAN"))
        {
            lval = 2;
        }
        printf("lval is:%d \n",lval);
        int rval = get_type(lchild->astnextSibling, table);
        printf("rval is:%d \n",rval);
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
    else if (!strcasecmp(root->value, "LVALUEARRAY"))
    { // lchild will be array access
    
        ST_ENTRY *temp = get_lexeme(table, lchild->children->tk_data.lexeme);
        if(temp==NULL){
            return -1;
        }
        char *datatype = temp->type.arr_type.arr_dt;
        if (!strcasecmp(datatype, "INTEGER"))
        {
            lval = 0;
        }
        else if (!strcasecmp(datatype, "REAL"))
        {
            lval = 1;
        }
        else if (!strcasecmp(datatype, "BOOLEAN"))
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
    FN_ENTRY *func = get_func_name(fn_table, root->children->tk_data.lexeme);
    if (func == NULL)
    {
        printf("\n Function not defined");
        return -1;
    }
    
    struct treeNode *called_op_list = NULL;
    struct treeNode *called_ip_list = NULL;
    if(root->children->astnextSibling->children->astnextSibling !=NULL){
        
    called_op_list = root->children->astnextSibling->children;
    called_ip_list = root->children->astnextSibling->children->astnextSibling;
    }
    else{
        // The case where opLIst is null so first child will be ip list
        called_op_list = NULL;
        called_ip_list = root->children->astnextSibling->children;
    }
    //
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
        
        // printf("\n inside check function call, the lexeme going into get_lexeme is '%s' \n",called_op_list->tk_data.lexeme);
        ST_ENTRY *op_entry = get_lexeme(table, called_op_list->tk_data.lexeme);
        if(op_entry==NULL){
            return -1;
        }
        // if(op_entry==NULL)printf("\n OP ENTRY IS NULL \n");
        if (strcasecmp(defined_op_list->parameter_type.id_type.id_dt, op_entry->type.id_type.id_dt)) // if comparison results in non 0 meaning both arent the same hence error
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
            printf("No of parameters is different in function call, line no: %d \n", called_op_list->line_no);
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
        struct treeNode *actual_child = NULL;
        if(called_ip_list->children->astnextSibling==NULL){
              actual_child = called_ip_list->children;
        }
        else actual_child = called_ip_list->children->astnextSibling;

        // Flag to choose which struct to access to obtain type
        int flag_for_type = 1; // 1- to access id struct, 2 to access array struct
        if (!strcasecmp(actual_child->value, "ARRAY_ACCESS"))
        {
            flag_for_type = 2;
        }
        if (flag_for_type == 1) // if comparison results in non 0 meaning both arent the same hence error
        {
            if(!strcasecmp(actual_child->value,"ID")){
                // printf("\n inside check function call, the lexeme going into get_lexeme for ip_entry is '%s' \n",actual_child->tk_data.lexeme);
                ST_ENTRY *ip_entry = get_lexeme(table, actual_child->tk_data.lexeme);
                if(ip_entry==NULL){
                    return -1;
                }
                if(ip_entry->is_array){
                    if(strcasecmp(defined_ip_list->parameter_type.arr_type.arr_dt,ip_entry->type.arr_type.arr_dt))
                        comparison_flag_type_checking =1;
                }
                else{
                if(strcasecmp(defined_ip_list->parameter_type.id_type.id_dt,ip_entry->type.id_type.id_dt))
                    comparison_flag_type_checking =1;
                }
            }
            if(!strcasecmp(actual_child->value,"NUM")){
                if(strcasecmp(defined_ip_list->parameter_type.id_type.id_dt,"INTEGER"))
                    comparison_flag_type_checking =1;
            }
            if(!strcasecmp(actual_child->value,"RNUM")){
                if(strcasecmp(defined_ip_list->parameter_type.id_type.id_dt,"REAL"))
                    comparison_flag_type_checking =1;
            }

            if(comparison_flag_type_checking){
                printf("\n type doesnt match in function call, line no: %d \n", root->line_no);
                return -1;
            }
        }
        else if (flag_for_type == 2) // if comparison results in non 0 meaning both arent the same hence error
        {

            
            ST_ENTRY *ip_entry = get_lexeme(table, actual_child->children->tk_data.lexeme);
            if(ip_entry==NULL){
                    return -1;
                }
            if(strcasecmp(defined_ip_list->parameter_type.id_type.id_dt,ip_entry->type.arr_type.arr_dt)){
                comparison_flag_type_checking =1;
            }
            if(comparison_flag_type_checking){
                printf("\n type doesnt match in function call, line no: %d \n", root->line_no);
                return -1;
            }
            int lr = ip_entry->type.arr_type.lowRange.start;
            int hr = ip_entry->type.arr_type.highRange.end;
            if (!strcasecmp(actual_child->children->astnextSibling->value, "NUM"))
            {
                int arr_index = actual_child->children->astnextSibling->tk_data.val;
                if (arr_index >= lr && arr_index < hr)
                {
                    printf("\n Array index: %d falls in range", actual_child->children->astnextSibling->tk_data.val);

                }
                else
                {
                    printf("\n Array index: %d not in range ", actual_child->children->astnextSibling->tk_data.val);
                    return -1;
                }
            }
        }
        called_ip_list = called_ip_list->next;
        defined_ip_list = defined_ip_list->next;
        if (defined_ip_list == NULL && called_ip_list != NULL)
        { // if one is null but the other is not null
            comparison_flag_type_checking = 1;
            printf("No of parameters is different in function call, line no: %d \n", called_ip_list->line_no);
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
    printf("\n All types match in function call");
    return 1; // all types match
}

int check_switch_case(struct treeNode *root, struct id_symbol_table *table)
{
    // conditionalStmt.syn = make_new_node(“SWITCH”,ID.addr, caseStmt.syn,default.syn);
    struct treeNode *switch_id = root->children;
    ST_ENTRY *temp = get_lexeme(table, switch_id->tk_data.lexeme);
    if(temp==NULL){
        return -1;
    }
    char *datatype = temp->type.id_type.id_dt;
    int switch_type;
    if (!strcasecmp(datatype, "REAL"))
    {
        printf("\n Switch case cant have real value ");
        return -1;
    }
    else
    {
        if (!strcasecmp(datatype, "INTEGER"))
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
            if (!strcasecmp(caseList->value, "NUM"))
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
            else if (!strcasecmp(caseList->value, "TRUE") || !strcasecmp(caseList->value, "FALSE"))
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
        if (!strcasecmp(root->value, "MODULE_DEF"))
        {
            // check for pair
            struct treeNode *child1 = root->children;         // ID
            struct treeNode *child2 = child1->astnextSibling; // ip_list
            struct treeNode *child3 = child2->astnextSibling; // op_list
            struct treeNode *child4;                         // module_def
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

            //fill symbol table for i/p list
            LISTNODE* temp = ip_list;
            while(temp!=NULL){
                char* name = temp->parameter_name;
                int hash_value = get_sym_table_hash(name);
                if(id_table->arr[hash_value]!=NULL){
            ST_ENTRY* curr = id_table->arr[hash_value];
            while(curr){
                if(!strcmp(curr->id_lexeme,name)){
                    printf("\n Input list cannot have same variable appearing more than once %s \n",name);
                    return;
                }
                curr = curr->next;
            }
        }

        ST_ENTRY* block = malloc(sizeof(ST_ENTRY));
        block->id_lexeme = name;
        block->next = NULL;
        block->type = temp->parameter_type;
        block->is_array=temp->is_array;

        insert_in_table(id_table,block);
        printf("**Entry done for:%s** \n",name);
        temp = temp->next;
            }

            //fill symbol table for o/p list

            LISTNODE* temp2 = op_list;
            while(temp2!=NULL){
                char* name = temp2->parameter_name;
                int hash_value = get_sym_table_hash(name);
                if(id_table->arr[hash_value]!=NULL){
            ST_ENTRY* curr = id_table->arr[hash_value];
            while(curr){
                if(!strcmp(curr->id_lexeme,name)){
                    printf("\n Output list cannot have same variable appearing more than once %s \n",name);
                    return;
                }
                curr = curr->next;
            }
        }

        ST_ENTRY* block = malloc(sizeof(ST_ENTRY));
        block->id_lexeme = name;
        block->next = NULL;
        block->type = temp2->parameter_type;
        block->is_array=temp2->is_array;

        insert_in_table(id_table,block);
        printf("**Entry done for:%s** \n",name);
        temp2 = temp2->next;
            }

        }
        else if (!strcasecmp(root->value, "FOR"))
        {
            TYPE type;
            struct treeNode *rangeNode = root->children->astnextSibling;
            if (!strcasecmp(rangeNode->children->children->value, "NUM"))
            {
                type.for_type.low_range = rangeNode->children->children->tk_data.val;
            }
            else
            {
                if (!strcasecmp(rangeNode->children->children->value, "MINUS"))
                {
                    type.for_type.low_range = -1 * (rangeNode->children->children->astnextSibling->tk_data.val);
                }
                else
                {
                    type.for_type.low_range = rangeNode->children->children->astnextSibling->tk_data.val;
                }
            }

            if (!strcasecmp(rangeNode->children->astnextSibling->children->value, "NUM"))
            {
                type.for_type.high_range = rangeNode->children->astnextSibling->children->tk_data.val;
            }
            else
            {
                if (!strcasecmp(rangeNode->children->astnextSibling->children->value, "MINUS"))
                {
                    type.for_type.high_range = -1 * (rangeNode->children->astnextSibling->children->astnextSibling->tk_data.val);
                }
                else
                {
                    type.for_type.high_range = rangeNode->children->astnextSibling->children->astnextSibling->tk_data.val;
                }
            }

            struct id_symbol_table *child_table = initST(nesting_num + 1);
            ST_ENTRY* temp = create_entry_and_insert(child_table, root->children, type);
            if(temp!=NULL){
                temp->is_array=0;
            }
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
                if(!strcasecmp(statements->value, "LVALUEID")){
                    if(!strcasecmp(statements->children->tk_data.lexeme,child1->tk_data.lexeme)){
                        printf("\n error- A for statement must not redefine the variable that participates in the iterating over the range, line no %d ",statements->children->line_no);
                        flag_for_for_in_semantic_analysis=0;
                        if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
                        return;
                    }
                }
                statements = statements->next;
            }
        }
        else if (!strcasecmp(root->value, "DECLARE"))
        {
            struct treeNode *type_attribute = root->children;
            struct treeNode *idList = root->children->astnextSibling;
            TYPE t;
            int array=0;
            if(!strcasecmp(type_attribute->value,"ARRAY-DCL")){
                array=1;
                t.arr_type.arr_dt = type_attribute->children->astnextSibling->tk_data.lexeme;
                struct treeNode* li = type_attribute->children->children;
                struct treeNode* ri = li->astnextSibling;
            if(!strcasecmp(li->children->value,"ID")){
            t.arr_type.lowRange.low_id = li->children->tk_data.lexeme;
            t.arr_type.isStatic = 0; //dynamic array
        }
        else if(!strcasecmp(li->children->value,"NUM")){
            t.arr_type.lowRange.start = li->children->tk_data.val;
            t.arr_type.isStatic = 1; //static array
        }
        else{
            if(!strcasecmp(li->children->astnextSibling->value,"ID"))
            {
                   t.arr_type.lowRange.low_id = strcat(strcat(li->children->value,","),li->children->astnextSibling->tk_data.lexeme);
                   t.arr_type.isStatic = 0; //dynamic array
            }
            else
            {
                if(!strcasecmp(li->children->value,"MINUS"))
                    t.arr_type.lowRange.start = -1*li->children->astnextSibling->tk_data.val;
                else   
                    t.arr_type.lowRange.start = li->children->astnextSibling->tk_data.val;
                t.arr_type.isStatic = 1; //static array
            }
        }
        
        // ri //
        if(!strcasecmp(ri->children->value,"ID")){
            t.arr_type.highRange.high_id = ri->children->tk_data.lexeme;
            t.arr_type.isStatic = 0; //dynamic array
        }
        else if(!strcasecmp(ri->children->value,"NUM")){
            t.arr_type.highRange.end = ri->children->tk_data.val;
            t.arr_type.isStatic = 1; //static array
        }
        else{
            if(!strcasecmp(ri->children->astnextSibling->value,"ID"))
            {
                   t.arr_type.highRange.high_id = strcat(strcat(ri->children->value,","),ri->children->astnextSibling->tk_data.lexeme);
                   t.arr_type.isStatic = 0; //dynamic array
            }
            else
            {
                if(!strcasecmp(ri->children->value,"MINUS"))
                    t.arr_type.highRange.end = -1*ri->children->astnextSibling->tk_data.val;
                else   
                    t.arr_type.highRange.end = ri->children->astnextSibling->tk_data.val;
                t.arr_type.isStatic = 1; //static array
            }
        }
        
            }
            else{
                t.id_type.id_dt = type_attribute->value;
                printf("****************being populated");
            }
            
            while (idList != NULL)
            {
                ST_ENTRY* temp = create_entry_and_insert(id_table, idList, t);
                if(temp!=NULL){
                    temp->is_array=array;
                }
                idList = idList->next;
            }
        }
        else if (!strcasecmp(root->value, "WHILE"))
        {
            type_check = check_while(root, id_table); // checks if while has a boolean expr
            if (type_check == -1)
            {
                if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
                return;
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

        else if (!strcasecmp(root->value, "SWITCH"))
        {
            type_check = check_switch_case(root, id_table); // checks if while has a boolean expr
            if (type_check == -1)
            {
                if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
                return;
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

        else if (!strcasecmp(root->value, "LVALUEID") || !strcasecmp(root->value, "LVALUEARRAY"))
        {          

            type_check = check_assignment(root, id_table);
            if (type_check == -1)
            {
                printf("INSIDE LVALUEID ERROR");
                if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
                return;
            }

             // Setting is_used in that function to be one to idnetify that that variable has been assigned a value in that function

            int hash_value = get_sym_table_hash(root->children->tk_data.lexeme);
            if (id_table->arr[hash_value] != NULL)
            {
                ST_ENTRY *temp = id_table->arr[hash_value];
                int found_entry_for_assign_op = 1;
                while (temp && found_entry_for_assign_op)
                {
                    if (!strcasecmp(temp->id_lexeme, root->children->tk_data.lexeme))
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
        else if (!strcasecmp(root->value, "MODULE-INVOKE"))
        {
            // a)moduleReuseStmt=make_new_node(“MODULE-INVOKE”,ID.addr,(make_new_node(“ARGUMENTS”,optional.syn,idList.syn)); //bottom up
            
            if(root->children->astnextSibling->children->astnextSibling!=NULL){
                struct treeNode* opList = root->children->astnextSibling->children;
                while(opList!=NULL){
                    ST_ENTRY* temp = get_lexeme(id_table,opList->tk_data.lexeme);
                    if(temp!=NULL){
                        temp->is_used=1;
                    }
                    opList = opList->next;
                }
            }
            type_check = check_function_call(root, id_table);
            
            if (type_check == -1)
            {
                if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
                return;
            }

        }

        else if(!strcasecmp(root->value,"GET-VALUE")){
            struct treeNode* getVal = root->children;
            ST_ENTRY* temp = get_lexeme(id_table,getVal->tk_data.lexeme);
            if(temp==NULL){
                if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
                return;   
            }
            else if(temp->is_array==1){
                printf("\n Error: Get-Value for array \n");
                if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
                return;
            }
        }
        else if(!strcasecmp(root->value,"PRINT")){
            //can be id/num/rnum/boolvar/arr-print
            ST_ENTRY* temp;
            if(!strcasecmp(root->children->value,"ARR-PRINT")){
                temp=get_lexeme(id_table,root->children->children->tk_data.lexeme);
            }
            else if(!strcasecmp(root->children->value,"ID")){
                temp = get_lexeme(id_table,root->children->tk_data.lexeme);
                
            }

                if(temp==NULL){
                    if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
                    return;
                }
                if(temp->is_used!=1){
                    printf("\nError: Print variable should be assigned some value \n");
                    if(root->next!=NULL)semanticAnalysis(root->next, id_table, nesting_num);
                    return;
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
}




// if(id_table->parent_function!=NULL){
//         printf("\n Entered gadbad area ,root is:%s ",root->value);
//         //o.p list check
//                             //every output param should be assigned value inside the fn
//                 FN_ENTRY* fn = id_table->parent_function;
//                 printf("\n Function:%s \n",fn->fn_name);
//                     LISTNODE* temp = fn->op_head;
//                     while(temp!=NULL){
//                         char* op_param = temp->parameter_name;
//                         ST_ENTRY* t = get_lexeme(id_table,op_param);
//                         if(t==NULL){
//                             printf("Output param;%s not declared inside fn definition \n",op_param);
//                         }
//                         else if(t->is_used!=1){
//                             printf("\n Output param:%s not assigned value inside the function.",op_param);
                            
//                         }
//                         temp = temp->next;
//                     }
//     }
//         }