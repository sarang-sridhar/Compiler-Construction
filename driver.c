/*
ID: 2020A7PS0297P                             NAME: Sarang Sridhar
ID: 2020A7PS0995P                             NAME: Kashish Mahajan
ID: 2020A7PS0993P                             NAME: Satvik Sinha
ID: 2020A7PS0036P                             NAME: Aarya Attrey
ID: 2020A7PS0017P                             NAME: Urvashi Sharma
*/

// #include "lexer.c"
#include "parser.c"
#include "ast.c"
#include <time.h>

void removeComments(char *name)
{
    FILE *cfp = fopen(name, "r");
    if (cfp == NULL)
    {
        printf("File not found");
        return;
    }
    int flag = 0;
    while (!feof(cfp))
    {
        char c = fgetc(cfp);
        int flag2 = 0;
        if (c == '*' && flag == 0)
        {
            char d = fgetc(cfp);
            if (d == '*')
            {
                flag = 1;
            }
            fseek(cfp, -1, SEEK_CUR);
        }
        else if (c == '*' && flag == 1)
        {
            char d = fgetc(cfp);
            if (d == '*')
            {
                flag = 0;
                printf("  ");
                flag2 = 1;
            }
            else
                fseek(cfp, -1, SEEK_CUR);
        }
        if (!flag2)
        {
            if (flag == 0)
                printf("%c", c);
            else
                printf(" ");
        }
    }
    fclose(cfp);
}

void print_token()
{
    if (global_token.hasError == 1)
    {
        printf("\n %d \t \t %s \t \t LEXICAL ERROR \n", global_token.line_no, global_token.tk_data.lexeme);
    }
    else
    {
        if (global_token.tk_name == ID)
        {
            printf("%d \t \t %s \t \t %s \n", global_token.line_no, global_token.tk_data.lexeme, token_strings[global_token.tk_name]);
        }
        else if (global_token.tk_name == NUM)
        {
            printf("%d \t \t %d \t \t %s \n", global_token.line_no, global_token.tk_data.val, token_strings[global_token.tk_name]);
        }
        else if (global_token.tk_name == RNUM)
        {
            printf("%d \t \t %f \t \t %s \n", global_token.line_no, global_token.tk_data.realVal, token_strings[global_token.tk_name]);
        }
        else if (global_token.tk_name == COMMENTMARK)
        {
            printf("%d \t \t %s \t \t %s \n", global_token.line_no, "", token_strings[global_token.tk_name]);
        }
        else
        {
            printf("%d \t \t %s \t \t %s \n", global_token.line_no, global_token.tk_data.lexeme, token_strings[global_token.tk_name]);
        }
    }
}
void print_choices()
{
    printf("\n Enter one option out of below: \n ***********");
    printf("\n 0. Exit");
    printf("\n 1. Print comment free code on terminal:");
    printf("\n 2. Print token list on terminal");
    printf("\n 3. Print parse tree in text file :");
    printf("\n 4. Print total time taken by the stage 1 code:");
    printf("\n 5. Print AST in text file :\n *********** \n");
}

void totalTime()
{

    // lexer initialisations
    if (fp == NULL)
    {
        printf("Source File not found");
        return;
    }

    // parser initialisations

    // grammar generate
    // stack initialisations
    stackElement *element = malloc(sizeof(stackElement));
    element->isTerminal = 0;
    strcpy(element->value, "startprogram");
    s_push(element);

    // root element of tree creation
    struct treeNode *tree_node = malloc(sizeof(struct treeNode));
    tree_node->children = NULL;
    tree_node->isTerminal = 0;
    tree_node->parent = NULL;
    tree_node->nextSibling = NULL;
    tree_node->prevSibling = NULL;
    strcpy(tree_node->value, "startprogram");

    // setting root and currentExpand
    root = tree_node;
    currExpand = tree_node;

    getnextblock(fp, buff1);
    driverFlag2 = 0;
    driverFlag = 0;
    while (!driverFlag)
    {
        getNextToken();
        runPDA();
        // if(driverFlag2)driverFlag=1;
    }
    while (isEpsilon[get_hash(s_top->value)])
    {
        s_pop();
    }

    if (!strcmp(s_top->value, "DOLLAR") && !errorToken)
        printf("Parsing successful \n");
    else if (errorToken)
        printf("Syntax Error\n");
    else
    {
        printf("Input finish but stack not empty");
        while (s_top != NULL)
        {
            printf("%s\n", s_top->value);
            s_pop();
        }
    }

    free(element);
    free(tree_node);
    return;
}

void printParseTree(struct treeNode *node, FILE *outfile)
{

    // EPSILON IS THERE IN THE PARSE TREE
    if (node == NULL || !strcmp(node->value, "EPSILON"))
    {
        if (!strcmp(node->value, "EPSILON"))
            fprintf(outfile, "Lexeme: ----, Line no: %d, Tk name: EPSILON, ValueIfNumber: ----, Parent: %s, isLeaf: YES, NodeSymbol: ----\n", node->line_no, node->parent->value);
        return;
    }

    // if(!strcmp(node->value,"DOLLAR")){
    //     printf("DOLLAR\n");
    //     return;
    // }

    printf("\n%s\n", node->value);

    if (node->children == NULL)
    { // fprintf(outfile,"getlost");
      // printf("*********%s,%d,%s,-,%s,YES,%s*********\n",node->tk_data.lexeme,node->line_no,node->value,node->parent->value,node->value);

        if (!strcmp(node->value, "NUM"))
            fprintf(outfile, "Lexeme: ----, Line no: %d, Tk name: %s, ValueIfNumber: %d, Parent: %s, isLeaf: YES, NodeSymbol: ----\n", node->line_no, node->value, node->tk_data.val, node->parent->value);
        else if (!strcmp(node->value, "RNUM"))
            fprintf(outfile, "Lexeme: ----, Line no: %d, Tk name: %s, ValueIfNumber: %f, Parent: %s, isLeaf: YES, NodeSymbol: ----\n", node->line_no, node->value, node->tk_data.realVal, node->parent->value);
        else if (node->parent == NULL)
            fprintf(outfile, "Lexeme: %s, Line no: %d, Tk_name: %s, ValueIfNumber: ----, Parent: ROOT, YES, NodeSymbol: ----\n", node->tk_data.lexeme, node->line_no, node->value);
        else
            fprintf(outfile, "Lexeme: %s, Line no: %d, Tk_name: %s, ValueIfNumber: YE CASE HAI KYA, Parent: %s, YES, NodeSymbol: ----\n", node->tk_data.lexeme, node->line_no, node->value, node->parent->value);
        // printf("%s\n", node->value);
        return;
    }
    // print left child
    if (node->children == NULL)
        printf("Root's child is null\n");
    else
        printf("Root's child is not null\n");
    printParseTree(node->children, outfile);

    // print node
    if (node->parent != NULL)
        fprintf(outfile, "RuleNo: %d, Lexeme: ----, Line no: ----, Tk name: ----, ValueIfNumber: ----, Parent: %s, IsLeaf: NO, NodeSymbol: %s\n", node->ruleNo, node->parent->value, node->value);
    else
        fprintf(outfile, "RuleNo: %d, Lexeme: ----, Line no: ----, Tk name: ----, ValueIfNumber: ----, Parent: ROOT, IsLeaf: NO, Node Symbol: %s\n", node->ruleNo, node->value);
    // fprintf(outfile,"%s\n", node->value);

    // print other children
    if (node->children == NULL)
        return;

    struct treeNode *temp;
    temp = node->children->nextSibling;

    while (temp != NULL)
    {
        printParseTree(temp, outfile);
        temp = temp->nextSibling;
    }
    return;
}

// prints AST in inorder traversal
void printAST(struct treeNode *root, FILE *outfile)
{
    if (root == NULL)
        return;

    printAST(root->children, outfile);

    if (root->addr != NULL)
    {
        printAST(root->addr, outfile);
    }
    else
    {
        // printf("Root ki value %s\n",root->value);
        fprintf(outfile, "%s\n", root->value);
       
        // while(root->next!=NULL){
        //     root=root->next;
        //     fprintf(outfile,"LIST:%s\n",root->value);
        //     if(root->pair!=NULL)
        //         fprintf(outfile,"PAIR:%s\n",root->pair->value);
        // }
    }

    if (root->children != NULL)
    {
        struct treeNode *temp;
        temp = root->children->astnextSibling;

        while (temp != NULL)
        {
            printAST(temp, outfile);
            temp = temp->astnextSibling;
        }
    }

    if(root->pair!=NULL)
        printAST(root->pair, outfile);

    root = root->next;
    while (root != NULL)
    {
        printAST(root, outfile);
        root = root->next;
    }
}

void runParser(FILE *fp2)
{

    // lexer initialisations

    if (fp == NULL)
    {
        printf("Source code file path incorrect \n");
        return;
    }

    // parser initialisations

    // grammar generate in main

    // create parse table in main

    // stack initialisations
    stackElement *element = malloc(sizeof(stackElement));
    element->isTerminal = 0;
    element->link = NULL;
    strcpy(element->value, "startprogram");
    s_push(element);

    // root element of tree creation
    struct treeNode *tree_node = malloc(sizeof(struct treeNode));
    tree_node->children = NULL;
    tree_node->isTerminal = 0;
    tree_node->parent = NULL;
    tree_node->nextSibling = NULL;
    tree_node->prevSibling = NULL;
    strcpy(tree_node->value, "startprogram");

    // setting root and currentExpand
    root = tree_node;
    currExpand = tree_node;

    getnextblock(fp, buff1);
    driverFlag2 = 0;
    driverFlag = 0;
    while (!driverFlag)
    {
        getNextToken();
        runPDA();
        // if(driverFlag2)driverFlag=1;
    }
    while (isEpsilon[get_hash(s_top->value)])
    {
        // printf("Current Expand is:%s\n", currExpand->value);
        currExpand->ruleNo = 7;
        s_pop();
    }

    if (!strcmp(s_top->value, "DOLLAR") && !errorToken)
        printf("Parsing successful");
    else if (errorToken)
        printf("Syntax Error\n");
    else
    {
        printf("Input finish but stack not empty");
        while (s_top != NULL)
        {
            printf("%s\n", s_top->value);
            s_pop();
        }
    }

    // printf("%s\n",root->value);
    createAST(root);
    printf("LMAOO DED\n");
    // printf("%s",root->value);
    // printParseTree(root,stdout);
    fprintf(fp2, "AST TREE:\n");
    // printParseTree(root, fp2);
    //  printf("VALUE OF ADDR BEFORE:%s\n",root->addr->value);
    printAST(root, fp2);
    // printf("VALUE OF ADDR AFTER:%s\n",root->addr->value);
    free(element);
    free(tree_node);
    return;
}

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("\n there must be three arguments- SourceCode ParsetreeOutput Buffer_size");
        return 0;
    }

    printf("\nImplementation Details\n(a) FIRST and FOLLOW set automated\n(b)Both lexical and syntax analysis modules implemented and work properly\n(c) modules pass all test cases\n(d) parse tree printed into file\n");
    int option;

    buffer_size = atoi(argv[3]);
    fp = fopen(argv[1], "r");

    clock_t t1_start, t1_end;
    double t1, t1_in_seconds;
    t1_start = clock();

    initHashTable();
    FILE *fp1 = fopen("grammar.txt", "r");
    makeGrammar(fp1);
    fclose(fp1);

    memset(isEpsilon, 0, sizeof(isEpsilon));

    // create first
    for (int i = 0; i < TOTAL_TERM; i++)
    {
        char *non_terminal = nonTerminals[i];
        createfirst(non_terminal); // calculate first of lhs
        if (ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] == ',')
            ntFirst[get_hash(non_terminal)][strlen(ntFirst[get_hash(non_terminal)]) - 1] = '\0';
    }
    isEpsilon[get_hash("EPSILON")] = 1;

    for (int i = 0; i < NON_TERM; i++)
    {
        char *non_terminal = nonTerminals[i];
        // printf("%s\n", non_terminal);
        createFollow(non_terminal); // calculate first of lhs
        removeDuplicates(ntFollow[get_hash(non_terminal)]);
    }

    // compute first and follow
    computeFirstAndFollow();
    createParseTable();

    t1_end = clock();
    t1 = (double)(t1_end - t1_start);
    t1_in_seconds = t1 / CLOCKS_PER_SEC;

    print_choices();
    scanf("%d", &option);

    while (1)
    {
        buff1 = (char *)malloc(buffer_size * sizeof(char));
        buff2 = (char *)malloc(buffer_size * sizeof(char));
        state = 0;
        forward = 0;
        begin = 0;
        do_not_refill = 0;
        flag = 0;
        current_line_no = 1;
        generateToken = 0;
        token_found = 0;
        errorToken = 0;

        switch (option)
        {
        case 0:
        {
            printf("\n Exiting succesfully \n");
            exit(0);
        }
        break;

        case 1:
        {
            // remove comments
            // printf("File name is %s \n",argv[1]);
            // FILE *input_file = fopen(argv[1], "r");
            removeComments(argv[1]);
        }
        break;

        case 2:
        {
            FILE *source_code = fopen(argv[1], "r");
            if (source_code == NULL)
            {
                printf("\n Source code file not found");
                exit(0);
            }
            printf("Line number  \t \t Lexeme \t \t Token Name: \n");
            getnextblock(source_code, buff1);
            int flag = 0;
            while (!flag)
            {
                // global_token.hasError = 0;
                generateToken = 1;
                while (generateToken)
                {
                    char input = getnextchar(source_code, buff1, buff2);
                    dfa(input);
                    if (input == EOF)
                    {
                        //   printf("input over\n");
                        flag = 1;
                        break;
                    }
                }

                if (!generateToken)
                    print_token();
            }
            fclose(source_code);
        }
        break;

        case 3:
        {
            fp = fopen(argv[1], "r");
            FILE *fp2 = fopen(argv[2], "w");
            if (fp2 == NULL)
            {
                printf("Output file not created, exit the program and start again.\n");
            }
            runParser(fp2);
            printf("\nParse Tree printed in txt file \n");
            fclose(fp);
            fclose(fp2);
        }
        break;

        case 4:
        {
            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();

            // invoke your lexer and parser here
            fp = fopen(argv[1], "r");
            totalTime();
            fclose(fp);

            end_time = clock();
            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
            // Print both total_CPU_time and total_CPU_time_in_seconds
            printf("Total CPU Time is: %f, totoal cpu time in seconds is %f \n", total_CPU_time + t1, total_CPU_time_in_seconds + t1_in_seconds);
        }
        break;
        case 5:
            createAST(root);
            if (root == NULL)
                printf("DED KHATAM\n");
            printf("%s\n", root->value);
            printParseTree(root, stdout);
            break;
        default:
            printf("\n choose one of the given options\n");
            break;
        }
        free(buff1);
        free(buff2);
        print_choices();
        scanf("%d", &option);
    }
}