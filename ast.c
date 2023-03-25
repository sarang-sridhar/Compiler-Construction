#include "tree.c"

int is_important_terminal(char *t)
{
    if (!(strcmp(t, "NUM") && strcmp(t, "RNUM") && strcmp(t, "ID") && strcmp(t, "INTEGER") && strcmp(t, "REAL") && strcmp(t, "BOOLEAN") && strcmp(t, "TRUE") && strcmp(t, "FALSE") && strcmp(t, "AND") && strcmp(t, "OR") && strcmp(t, "LT") && strcmp(t, "LE") && strcmp(t, "GT") && strcmp(t, "GE") && strcmp(t, "EQ") && strcmp(t, "NE") && strcmp(t, "PLUS") && strcmp(t, "MINUS") && strcmp(t, "DIV") && strcmp(t, "MUL")))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

struct treeNode *insert_at_beginning(struct treeNode *head, struct treeNode *node)
{

    if (head == NULL)
    {
        head = node;
        node->next = NULL;
        return head;
    }

    node->next = head->next;
    head = node;
    return head;
}

struct treeNode *insert_pair_at_beginning(struct treeNode *head, struct treeNode *node1, struct treeNode *node2)
{

    if (head == NULL)
    {
        node1->pair = node2;
        node1->next = NULL;
        head = node1;
        return head;
    }

    node1->pair = node2;
    node1->next = head->next;
    head = node1;
    return head;
}

struct treeNode *makeNewNode(char *name, struct treeNode *list[], int count)
{
    struct treeNode *node = malloc(sizeof(struct treeNode));
    strcpy(node->value, name);
    int i;
    node->children = list[0];
    struct treeNode *temp;
    temp = node->children;

    for (i = 1; i < count; i++)
    {
        temp->nextSibling = list[i];
        temp = temp->nextSibling;
    }

    return node;
}

void createAST(struct treeNode *root)
{
    char *node_name;        // name of a node
    struct treeNode *temp;  // for storing list
    struct treeNode *temp1; // for stroing pair in list
    struct treeNode *children[4];
    int count = 0;
    struct treeNode *freenode;

    // post-order traversal

    // capture leftmost-child
    struct treeNode *temp_child = root->children;

    // base-case
    if (root == NULL)
        return;

    // capture rule number from treeNode
    int rule_no = root->rule_no;

    // base-case, is Epsilon's isTerminal == 1?
    // problem
    if (root->isTerminal)
    {
        return;
    }

    // iterate through all children(top-down rules to be added here)
    while (temp_child != NULL)
    {
        freenode = temp_child;
        if (temp_child->isTerminal && !is_important_terminal(temp_child->value))
        {
            temp_child = temp_child->nextSibling;
            free(freenode);
            continue;
        }

        switch (rule_no)
        {
        case 1:
            // top-down
            createAST(temp_child);
            if (!strcmp(temp_child->value, "program"))
            {
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 2:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "moduleDeclarations"))
            {
                children[count++] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "otherModules"))
            {
                children[count++] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "driverModule"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("PROGRAM", children, 4);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            break;

        case 3:
            createAST(temp_child);

            if (!strcmp(temp_child->value, "moduleDeclaration"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moduleDeclarations"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            break;

        case 4:
            root->syn = NULL;
            createAST(temp_child);
            break;

        case 5:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                root->addr = temp_child;
            }
            break;

        case 6:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "module"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "otherModules"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 7:
            createAST(temp_child);
            root->syn = NULL;
            break;

        case 8:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "moduleDef"))
            {
                root->addr = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 9:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                children[count++] = temp_child;
            }
            if (!strcmp(temp_child->value, "input_plist"))
            {
                children[count++] = temp_child->syn;
                temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "ret"))
            {
                children[count++] = temp_child->syn;
                temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moduleDef"))
            {
                children[count++] = temp_child->syn;
                root->addr = makeNewNode("MODULE_DEF", children, 4);
                temp_child->nextSibling;
                free(freenode);
                continue;
            }

            break;
        case 10:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "output_plist"))
            {
                root->syn = temp_child->syn;
                temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 11:
            createAST(temp_child);
            root->syn = NULL;
            break;

        case 12:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            if (!strcmp(temp_child->value, "dataType"))
            {
                temp1 = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moreList"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 13:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            if (!strcmp(temp_child->value, "dataType"))
            {
                temp1 = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moreList"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 14:
            createAST(temp_child);
            root->syn = NULL;
            break;

        case 15:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            if (!strcmp(temp_child->value, "type"))
            {
                temp1 = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moreOutput"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 16:
            createAST(temp_child);
            root->syn = NULL;
            break;

        case 17:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            if (!strcmp(temp_child->value, "type"))
            {
                temp1 = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "moreOutput"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 18:
            createAST(temp_child);
            root->syn = NULL;
            break;

        case 19:
            root->addr = temp_child;
            break;

        case 20:
            root->addr = temp_child;
            break;

        case 21:
            root->addr = temp_child;
            break;

        case 22:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "range"))
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "type"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("ARRAY-DCL", children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 23:
            root->addr = temp_child;
            break;

        case 24:
            root->addr = temp_child;
            break;

        case 25:
            root->addr = temp_child;
            break;

        case 26:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "statements"))
            {
                root->syn = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 27:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "statement"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            if (!strcmp(temp_child->value, "statements"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 28:
            createAST(temp_child);
            root->syn = NULL;
            break;

        case 29:
            createAST(temp_child);
            root->addr = temp_child->addr;
            temp_child = temp_child->nextSibling;
            free(freenode);
            continue;

        case 30:
            createAST(temp_child);
            root->addr = temp_child->addr;
            temp_child = temp_child->nextSibling;
            free(freenode);
            continue;

        case 31:
            createAST(temp_child);
            root->addr = temp_child->addr;
            temp_child = temp_child->nextSibling;
            free(freenode);
            continue;

        case 32:
            createAST(temp_child);
            root->addr = temp_child->syn;
            temp_child = temp_child->nextSibling;
            free(freenode);
            continue;

        case 33:
            // only one child possible
            createAST(temp_child);
            root->addr = temp_child->addr;
            temp_child = temp_child->nextSibling;
            free(freenode);
            break;

        case 34:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "ID"))
            {
                root->addr = temp_child->addr;
            }
            break;

        case 35:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "print_var"))
            {
                root->addr = temp_child->addr;
                free(freenode);
            }
            break;

        case 36:
            if (!strcmp(temp_child->value, "whichId2"))
                temp_child->inh = root->syn;

            createAST(temp_child);

            if (!strcmp(temp_child->value, "whichId2"))
            {
                root->addr = temp_child->addr;
                free(freenode);
            }

            if (!strcmp(temp_child->value, "ID"))
                root->syn = temp_child->addr;

            break;

        case 37:
            createAST(temp_child);
            root->addr = temp_child->addr;
            free(freenode);
            break;

        case 38:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 39:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 40:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "sign"))
            {
                children[count++] = temp_child->addr;
                free(freenode);
            }
            if (!strcmp(temp_child->value, "index"))
            {
                children[count++] = temp_child->addr;
                free(freenode);
            }
            if (temp_child->nextSibling == NULL)
            {
                // WHY children2???????????????
                children[0] = root->inh;
                children[1] = makeNewNode("INDEX", children, 2);
                root->addr = makeNewNode("ARR-PRINT", children, 2);
            }
            break;
        case 41:
            createAST(temp_child);
            root->addr = root->inh;
            break;

        case 42:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 43:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 44:
            createAST(temp_child);
            if (count == 0)
                children[count++] = root->inh;
            if (!strcmp(temp_child->value, "newArithmeticExpr"))
            {
                children[count++] = temp_child->addr;
                free(freenode);
            }
            if (temp_child->nextSibling == NULL)
            {
                root->addr = makemakeNewNode("ARRAY_ACCESS", children, 2);
            }
            break;

        case 45:
            // CHECK RULE ONCE SEEMS SUS !!!!!!!!, WHY??
            createAST(temp_child);
            root->addr = root->inh;
            break;

        case 46:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 47:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 48:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 49:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 50:
            createAST(temp_child);
            root->addr = NULL;
            break;

        case 51:
            if (!strcmp(temp_child->value, "whichId"))
                temp_child->inh = root->syn;

            createAST(temp_child);

            if (!strcmp(temp_child->value, "whichId"))
            {
                root->addr = temp_child->addr;
                free(freenode);
            }

            if (!strcmp(temp_child->value, "ID"))
                root->syn = temp_child->addr;
            break;

        case 52:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;
        case 53:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;
        case 54:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 55:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 56:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 57:
            createAST(temp_child);
            root->addr = temp_child->addr;
            free(freenode);
            break;

        case 58:
            createAST(temp_child);
            root->addr = temp_child->addr;
            free(freenode);
            break;

        case 59:
            if (!strcmp(temp_child->value, "newA1"))
                temp_child->inh = root->syn;

            createAST(temp_child);
            if (!strcmp(temp_child->value, "newA1"))
            {
                root->addr = temp_child->syn;
                free(freenode);
            }

            if (!strcmp(temp_child->value, "newTerm"))
            {
                root->syn = temp_child->addr;
                free(freenode);
            }
            break;

        case 60:
            if (!strcmp(temp_child->value, "newA11"))
            temp_child->inh=root->addr;
            
            createAST(temp_child);
            if(!strcmp(temp_child->value,"op1")){
                strcpy(node_name,temp_child->addr->value);
                free(freenode);
            }            
            if(!strcmp(temp_child->value,"newA11")){
                children[0]=temp_child->inh;
                root->syn=temp_child->syn;
                free(freenode);
            }
            if(!strcmp(temp_child->value,"newTerm")){
                children[1]=temp_child->addr;
                free(freenode);
            }
            if(temp_child->nextSibling==NULL){
                root->addr=makeNewNode(node_name,children,2);
            }
            break;

                case 61:
                // check rule once
                createAST(temp_child);
                root->addr = root->inh;
            break;

        case 62:
            if (!strcmp(temp_child->value, "newA2"))
                temp_child->inh = root->syn;

            createAST(temp_child);

            if (!strcmp(temp_child->value, "newA2"))
            {
                root->addr = temp_child->syn;
                free(freenode);
            }

            if (!strcmp(temp_child->value, "newNextTerm"))
            {
                root->syn = temp_child->addr;
                free(freenode);
            }
            break;

        case 63:
            if(!strcmp(temp_child->value,"newA21"))
                temp_child->inh=root->addr;
            
            createAST(temp_child);
            if(!strcmp(temp_child->value,"op2")){
                strcpy(node_name,temp_child->addr->value);
                free(freenode);
            }            
            if(!strcmp(temp_child->value,"newA21")){
                children[0]=temp_child->inh;
                root->syn=temp_child->syn;
                free(freenode);
            }
            if(!strcmp(temp_child->value,"newNextTerm")){
                children[1]=temp_child->addr;
                free(freenode);
            }
            if(temp_child->nextSibling==NULL){
                root->addr=makeNewNode(node_name,children,2);
            }
            break;

        case 64:
            // check rule once
            createAST(temp_child);
            root->syn = root->inh;
            break;

        case 65:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "startExpr"))
            {
                root->addr = temp_child->addr;
                free(freenode);
            }
            break;
        
        case 66:
            // top down
            createAST(temp_child);
            // only one child possible
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 67:
            // top down
            createAST(temp_child);
            if (!strcmp(temp_child->value, "after_u1"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("U1_MINUS", children, 2);
                free(freenode);
                return;
            }

            if (!strcmp(temp_child->value, "MINUS"))
            {
                children[count++] = temp_child;
            }

            break;

        case 68:
            // top down
            createAST(temp_child);
            if (!strcmp(temp_child->value, "after_u1"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("U1_PLUS", children, 2);
                free(freenode);
                return;
            }

            if (!strcmp(temp_child->value, "PLUS"))
            {
                children[count++] = temp_child;
            }

        case 69:

            createAST(temp_child);
            if (!strcmp(temp_child->value, "startExpr"))
            {
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 70:
            // top down
            createAST(temp_child);
            // only one child possible
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 71:

            // don't call createast
            root->addr = temp_child;
            return;

        case 72:
            // don't call createast
            root->addr = temp_child;
            return;

        case 73:
            // top down
            createAST(temp_child);
            // only one child possible
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 74:
            // top down
            createAST(temp_child);
            // only one child possible
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 75:
            // top down
            if (!strcmp(temp_child->value, "ID"))
            {
                root->syn = temp_child;
            }
            if (!strcmp(temp_child->value, "whichStmt"))
            {
                temp_child->inh = root->syn;
            }

            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "whichStmt"))
            {
                root->addr = temp_child->addr;
                free(freenode);
                return;
            }

            break;

        case 76:
            // only one child possible
            temp_child->inh = root->inh;
            createAST(temp_child);
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 77:
            // only one child possible
            temp_child->inh = root->inh;
            createAST(temp_child);
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 78:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "expression"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("LVALUEID", children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            break;

        case 79:
            // top down
            createAST(temp_child);
            if (!strcmp(temp_child->value, "newArithmeticExpr"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                temp = makeNewNode("ARRAY_ACCESS", children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "expression"))
            {
                children[--count] = temp_child->addr;
                children[--count] = temp;
                root->addr = makeNewNode("LVALUEARRAY", children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            break;

        case 80:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "optional"))
            {
                children[count++] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "ID"))
            {
                temp = temp_child;
            }
            else if (!strcmp(temp_child->value, "idList"))
            {
                children[count++] = temp_child->syn;
                children[--count] = makeNewNode("ARGUMENTS", children, 2);
                children[--count] = temp;
                root->addr = makeNewNode("MODULE-INVOKE", children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 81:
            // top down
            createAst(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "idList"))
            {
                root->syn = temp_child->syn;
                temp_child = temp_child->next;
                free(freenode);
                continue;
            }

            break;

        case 82:
            root->syn = NULL;
            free(temp_child);
            return;

        case 83:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "actualParameter"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "moreId"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                free(freenode);
                return;
            }

        case 84:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "actualParameter"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "moreId"))
            {
                root->syn = insert_at_beginning(temp_child->syn, temp);
                free(freenode);
                return;
            }

        case 85:
            root->syn = NULL;
            free(freenode);
            return;

        case 86:
            // topdown
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "sign"))
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "aVar"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("PARAMETER_NUM", children, 2);
                free(freenode);
                return;
            }

        case 87:
            // top down
            createAST(temp_child);
            // bottom up
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 88:
            // top down
            createAST(temp_child);
            // bottom up
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 89:
            // top down
            createAST(temp_child);
            // bottom up
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 90:
            // top down
            if (!strcmp(temp_child->value, "ab1"))
            {
                temp_child->inh;
                root->syn;
            }
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "ab1"))
            {
                root->addr = temp_child->syn;
                free(freenode);
                return;
            }
            if (!strcmp(temp_child->value, "anyTerm"))
            {
                root->syn = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue; 
            }

        case 91:
            // top down
            if (!strcmp(temp_child->value, "ab1"))
            {
                temp_child->inh = root->addr;
            }
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "bop"))
            {
                node_name = temp_child->addr->value;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "anyTerm"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode(node_name, children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            else if (!strcmp(temp_child->value, "ab1"))
            {
                root->syn = temp_child->syn;
                free(freenode);
                return;
            }

        case 92:
            root->syn = root->inh;
            free(temp_child);
            return;

        case 93:
            // top down
            if (!strcmp(temp_child->value, "ab2"))
            {
                temp_child->inh = root->syn;
            }
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "ab2"))
            {
                root->addr = temp_child->addr;
                free(freenode);
                return;
            }

            else if (!strcmp(temp_child->value, "arithmeticExpr"))
            {
                root->syn = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

        case 94:
            // top down
            createAST(temp_child);
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 95:
            //top down
            createAST(temp_child);
            //bottom up
            if(!strcmp(temp_child->value,"relationalOp"))
            {
                node_name = temp_child->addr->value;  
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            else if(!strcmp(temp_child->value,"arithmeticExpr"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode(node_name,children,2);
                free(freenode);
                return;
            }


        case 96:
            root->addr = root->inh;
            free(temp_child);
            return;

        case 97:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "MINUS"))
            {
                children[count++] = temp_child;
            }

            else if (!strcmp(temp_child->value, "after_unary"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("U_MINUS", children, 2);
                free(freenode);
                return;
            }

            break;

        case 98:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "PLUS"))
            {
                children[count++] = temp_child;
            }

            else if (!strcmp(temp_child->value, "after_unary"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("U_PLUS", children, 2);
                free(freenode);
                return;
            }

            break;

        case 99:
            // top down
            createAST(temp_child);
            // bottom up
            if (!strcmp(temp_child->value, "arithmeticExpr"))
            {
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }

            break;

        case 100:
            // top-down
            createAST(temp_child);
            // only one child possible
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 101:
            // top-down
            if (!strcmp(temp_child->value, "a1"))
                temp_child->inh = root->syn;
            createAST(temp_child);
            // bottom-up
            if (!strcmp(temp_child->value, "term"))
                root->syn = temp_child->addr;
            else if (!strcmp(temp_child->value, "a1"))
                root->addr = temp_child->syn;
            temp_child = temp_child->nextSibling;
            free(freenode);
            continue;

        case 102:
            // top-down
            if (!strcmp(temp_child->value, "a1"))
                temp_child->inh = root->addr;
            else if (!strcmp(temp_child->value, "op1"))
                node_name = temp_child->addr->value;
            createAST(temp_child);
            // bottom-up
            if (!strcmp(temp_child->value, "a1"))
                root->syn = temp_child->syn;
            else if (!strcmp(temp_child->value, "term"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode(node_name, children, 2);
            }
            temp_child = temp_child->nextSibling;
            free(freenode);
            continue;

        case 103:
            // top-down
            root->syn = root->inh;
            createAST(temp_child);
            break;

        case 104:
            // top-down
            if (!strcmp(temp_child->value, "a2"))
                temp_child->inh = root->syn;
            createAST(temp_child);
            // bottom-up
            if (!strcmp(temp_child->value, "nextTerm"))
                root->syn = temp_child->addr;
            if (!strcmp(temp_child->value, "a2"))
                root->addr = temp_child->syn;
            temp_child = temp_child->nextSibling;
            free(freenode);
            continue;

        case 105:
            // top-down
            if (!strcmp(temp_child->value, "a2"))
                temp_child->inh = root->addr;
            if (!strcmp(temp_child->value, "op2"))
                node_name = temp_child->addr->value;
            createAST(temp_child);
            // bottom-up
            if (!strcmp(temp_child->value, "a2"))
                root->syn = temp_child->syn;
            if (!strcmp(temp_child->value, "nextTerm"))
            {
                children[count++] = root->inh;
                children[count++] = temp_child->addr;
                root->addr = makeNewNode(node_name, children, 2);
            }
            temp_child = temp_child->nextSibling;
            free(freenode);
            continue;

        case 106:
            root->syn = root->inh;
            createAST(temp_child);
            break;

        case 107:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "arithmeticOrBooleanExpr"))
            {
                root->addr = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 108:
            createAST(temp_child);
            root->addr = temp_child->addr;
            free(freenode);
            return;

        case 109:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 110:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 111:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 112:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 113:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 114:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 115:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 116:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 117:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 118:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 119:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 120:
            root->addr = temp_child;
            createAST(temp_child);
            break;

        case 121:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "idList"))
            {
                children[count++] = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child, "dataType"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("DECLARE", children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 122:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "caseStmt"))
            {
                root->syn = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "default"))
            {
                root->syn->pair = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 123:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "value"))
            {
                temp = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "statements"))
            {
                temp1 = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "post"))
            {
                root->syn = insert_pair_at_beginning(temp_child->syn, temp, temp1);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 124:
            root->syn = temp_child->syn;
            createAST(temp_child);
            free(freenode);
            return;

        case 125:
            root->syn = NULL;
            createAST(temp_child);
            break;

        case 126:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 127:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 128:
            createAST(temp_child);
            root->addr = temp_child->addr;
            break;

        case 129:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "statements"))
            {
                root->syn = temp_child->syn;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 130:
            root->syn = NULL;
            createAST(temp_child);
            break;

        case 131:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "range_for"))
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "statements"))
            {
                children[count++] = temp_child->syn;
                root->addr = makeNewNode("FOR", children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 132:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "arithmeticOrBooleanExpr"))
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "statements"))
            {
                children[count++] = temp_child->addr;
                root->addr = makeNewNode("WHILE", children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            break;

        case 133:
            //top down
            createAST(temp_child);
            //bottom up
            if (!strcmp(temp_child->value, "sign") && temp_child->prevSibling == NULL)
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "NUM") && temp_child->nextSibling!=NULL)
            {
                children[count++] = temp_child;
                temp = makeNewNode("LEFT-FOR", children, 2);
            }
            else if (!strcmp(temp_child->value, "sign") && temp_child->prevSibling != NULL)
            {
                children[0] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "NUM") && temp_child->nextSibling == NULL)
            {
                children[1] = temp_child;
                children[--count] = makeNewNode("RIGHT-FOR", children, 2);
                children[--count] = temp;
                root->addr = makeNewNode("RANGE_FOR", children, 2);
                return;
            }
            break;

        case 134:
            createAST(temp_child);
            if (!strcmp(temp_child->value, "sign") && temp_child->prevSibling == NULL)
            {
                children[count++] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "index") && temp_child->nextSibling!=NULL)
            {
                children[count++] = temp_child->addr;
                temp = makeNewNode("LEFT-INDEX", children, 2);
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "sign") && temp_child->prevSibling != NULL)
            {
                children[0] = temp_child->addr;
                temp_child = temp_child->nextSibling;
                free(freenode);
                continue;
            }
            else if (!strcmp(temp_child->value, "index") && temp_child->nextSibling == NULL)
            {
                children[1] = temp_child->addr;
                children[--count] = makeNewNode("RIGHT-INDEX", children, 2);
                children[--count] = temp;
                root->addr = makeNewNode("RANGE", children, 2);
                free(freenode);
                return;
            }
            break;

            default:
                printf("No such rule exists");
        }

        temp_child = temp_child->nextSibling;
    }

    return;
}
