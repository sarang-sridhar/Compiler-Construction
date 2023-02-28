
#include "parserDef.h"
#include "lexerDef.h"
#include "tree.c"
#include "stackAdt.c"

//hash function for terminals and non-terminals
int get_hash(const char *s)
{
    int n = strlen(s);
    long long p = 31, m = 10e9+7;
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < n; i++)
    {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash)%HASH_MOD;
}

void getNextToken()
{
    generateToken = 1;  
}

//function for displaying error while parsing
void display_error()
{
   if(global_token.hasError)
   {
        printf("Lexical error at line no. %d \n",global_token.line_no);
   }

   else
   {
        printf("Parsing error at line no. %d\n", global_token.line_no);
   }
    exit(0);
}

// function for storing grammar rules in the form of linked list
void makeGrammar(FILE* fp)
{
    char buff[1024];
    int counter = 0;
    char delim[] = {", \n"};
    
    while(fgets(buff, 1024, fp))
    {
        char * token = strtok(buff, delim);
        struct node* nonTerminal = (struct node*) malloc(sizeof(struct node));
        nonTerminal->isTerminal = 0;
        nonTerminal->backward_link = NULL;
        //nonTerminal->value = token;
        strcpy(nonTerminal->value,token);
        grammar[counter] = nonTerminal;
        struct node* curr_token = nonTerminal;
        //printf("%s->",token);

        while((token = strtok(NULL, delim)) != NULL)
        {
            struct node* temp = (struct node*) malloc(sizeof(struct node));
            if(islower(token[0])) 
                temp->isTerminal = 0;  
            else
                temp->isTerminal = 1;

            // temp->value = token;
            strcpy(temp->value,token);
            curr_token->forward_link = temp;
            temp->backward_link = curr_token;
            curr_token = temp;
            //printf("%s,",token);
        }
        curr_token->forward_link = NULL;
        counter++;
        //printf("\n");
    }
}

void createParseTable()
{
    for(int i = 0; i < NUM_RULES; i++)
    {
        for(int j = 0; firstAndFollow[i][j] != NULL; j++)
        {
            // if(!strcmp(firstAndFollow[i][j],"ID"))
            //     printf("lol,%d,%d",get_hash(firstAndFollow[i][j]),get_hash("ID"));
            parseTable[get_hash(grammar[i]->value)][get_hash(firstAndFollow[i][j])] = grammar[i];
        }
    }
}


void runPDA()
{
    if(s_top == NULL)
        display_error();
    else if(global_token.hasError)
        display_error();
    else
    {
        // printf("%d",s_top->isTerminal);
        if(!strcmp(currExpand->value,"EPSILON")){
            while(currExpand->parent->nextSibling == NULL && currExpand->parent != root)
            {
                currExpand = currExpand->parent;
                while(currExpand->prevSibling != NULL)
                    currExpand = currExpand->prevSibling;
            }

            if(currExpand->parent != root)
            {
                currExpand = currExpand->parent->nextSibling;
                // printf("**%s**\n",currExpand->value);
                // printf("//%s,%d//",s_top->value,global_token.tk_name);
            }
        }
        if(s_top->isTerminal)
        {
            if(!strcmp(s_top->value,token_strings[global_token.tk_name]))
            {
                //match
                printf("match,%s\n",s_top->value);
                s_pop();
                //tree-code

                printf("%s\n",currExpand->value);
                if(currExpand->nextSibling != NULL){
                    //printf("%s\n",currExpand->value);
                    currExpand = currExpand->nextSibling;
                }
                else
                {
                    while(currExpand->prevSibling != NULL){
                        currExpand = currExpand->prevSibling;
                    }

                    while(currExpand->parent->nextSibling == NULL && currExpand->parent != root)
                    {
                        currExpand = currExpand->parent;
                        while(currExpand->prevSibling != NULL)
                            currExpand = currExpand->prevSibling;
                    }

                    if(currExpand->parent != root)
                    {
                        currExpand = currExpand->parent->nextSibling;
                    }
                }
            }
            else{
                 printf("not match");
                display_error();
            }
        }
        else
        {   
            if(!strcmp(s_top->value,"a1")){
                printf("a1 pushed\n");
                printf("**%s**",parseTable[get_hash(s_top->value)][get_hash(token_strings[global_token.tk_name])]->value);
            }
            if(parseTable[get_hash(s_top->value)][get_hash(token_strings[global_token.tk_name])] != NULL)
            {
                if(!strcmp(s_top->value,"a1"))
                    printf("%s",parseTable[get_hash(s_top->value)][get_hash(token_strings[global_token.tk_name])]->value);
                struct node* curr;
                curr = parseTable[get_hash(s_top->value)][get_hash(token_strings[global_token.tk_name])];
                s_pop();
                //tree generation
                //printf("Before add child");
                if(!strcmp(currExpand->value,"EPSILON")){
                     while(currExpand->parent->nextSibling == NULL && currExpand->parent != root)
                    {
                        currExpand = currExpand->parent;
                        while(currExpand->prevSibling != NULL)
                            currExpand = currExpand->prevSibling;
                    }

                    if(currExpand->parent != root)
                    {
                        currExpand = currExpand->parent->nextSibling;
                    }
                }
                printf("%s\n",currExpand->value);
                addChild(currExpand,curr->forward_link);
                    // printf("%s\n",currExpand->value);
                // else{
                //     while(currExpand->parent->nextSibling == NULL && currExpand->parent != root)
                //     {
                //         currExpand = currExpand->parent;
                //         while(currExpand->prevSibling != NULL)
                //             currExpand = currExpand->prevSibling;
                //     }

                //     if(currExpand->parent != root)
                //     {
                //         currExpand = currExpand->parent->nextSibling;
                //         printf("%s",currExpand->value);
                //     }
                // }
                //printf("After add child");.................
                currExpand = currExpand->children;
                //printf("After curr expand");
                while(curr->forward_link != NULL)
                {
                    curr = curr->forward_link;
                }
                while(curr->backward_link != NULL)
                {
                    
                    stackElement* stackNode = malloc(sizeof(stackElement));
                    stackNode->isTerminal = curr->isTerminal;
                    strcpy(stackNode->value,curr->value);
                    if(strcmp(stackNode->value,"EPSILON")){
                        s_push(stackNode);
                        // if(!strcmp(stackNode->value,"a1"))
                        //     printf("****");
                    }
                    
                    curr = curr->backward_link;
                }
                runPDA();
            }
            else
            {
                printf("lol,%d",global_token.tk_name);
                display_error();
            }
        }
    }
}


// int already_visited[128];
char ntFirst[HASH_MOD][300];  // size to be changed later
char ntFollow[HASH_MOD][300]; // size to be changed later

int isEpsilon[HASH_MOD];
int cnt = 0;
int createfirst(char *term)
{ // returns 0 if first contains epsilon,otherwise returns 1
  

    if (strlen(ntFirst[get_hash(term)]))
    {
        
        return isEpsilon[get_hash(term)];
    }
    
    if (isupper(term[0]))
    { // if term is a terminal
        if (strcmp(term, "EPSILON"))
        {                                              // and its not an epsilon
            if (!strlen(ntFirst[get_hash(term)]))      // if ntFirst of that term is empty
                strcat(ntFirst[get_hash(term)], term); // append term
                                                       // printf("%s\n",ntFirst[get_hash(term)]);
            return 0;
        }
        else
        {
            isEpsilon[get_hash(term)] = 1; // if term is epsilon set isEpsilon 1 and return 1
            return 1;
        }
    }

    // if term is a non terminal,find first
    for (int i = 0; i < NUM_RULES; i++)
    {
        if (!strcmp(grammar[i]->value, term))
        {                                         // matching nt found
            struct node *temp = grammar[i]->forward_link; // LHS
            while (temp != NULL && createfirst(temp->value))
            {                                                                                                               // if temp is a terminal (except epsilon) or a nt (which doesnt derive epsilon)
                if (strlen(ntFirst[get_hash(term)]) && ntFirst[get_hash(term)][strlen(ntFirst[get_hash(term)]) - 1] != ',') // if ntFirst of that term isnt empty
                    strcat(ntFirst[get_hash(term)], ",");
                strcat(ntFirst[get_hash(term)], ntFirst[get_hash(temp->value)]);
                temp = temp->forward_link;
            }
            if (temp == NULL)
                isEpsilon[get_hash(term)] = 1;
            else
            {
                if (strlen(ntFirst[get_hash(term)]) && ntFirst[get_hash(term)][strlen(ntFirst[get_hash(term)]) - 1] != ',') // if ntFirst of that term isnt empty
                    strcat(ntFirst[get_hash(term)], ",");
                
                strcat(ntFirst[get_hash(term)], ntFirst[get_hash(temp->value)]);
            }
        }
    }
    return isEpsilon[get_hash(term)];
}

void bubbleSort(char *arr[500], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (strcmp(arr[j], arr[j + 1]) > 0)
            {
                char *temp;
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}

void removeDuplicates(char *str)
{
    char *arr[500];
    char *token = strtok(str, ",");
    int i = 0;
    while (token != NULL)
    {
        arr[i] = token;
        i++;
        token = strtok(NULL, ",");
    }
    bubbleSort(arr, i);
    int n = i;
    char str1[2000] = {};
    for (i = 0; i < n; i++)
    {
        if (i != n - 1 && strcmp(arr[i], arr[i + 1]))
        {
            if (strlen(str1))
            {
                strcat(str1, ",");
            }
            strcat(str1, arr[i]);
        }
        if (i == n - 1)
        {
            if (strlen(str1))
            {
                strcat(str1, ",");
            }
            strcat(str1, arr[i]);
        }
    }
    
    strcpy(str, str1);
}

void createFollow(char *non_terminal)
{
    if (strlen(ntFollow[get_hash(non_terminal)]))
        return;
    for (int i = 0; i < NUM_RULES; i++)
    {
        struct node *temp = grammar[i]->forward_link;
        while (temp != NULL && strcmp(temp->value, non_terminal))
        {
            temp = temp->forward_link;
        }

        if (temp != NULL)
        {
            temp = temp->forward_link;
            
            while (temp != NULL && isEpsilon[get_hash(temp->value)])
            {
                if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                    strcat(ntFollow[get_hash(non_terminal)], ",");
                strcat(ntFollow[get_hash(non_terminal)], ntFirst[get_hash(temp->value)]);
                temp = temp->forward_link;
            }

            if (temp == NULL)
            {
                if (strcmp(grammar[i]->value, non_terminal))
                {
                    if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                        strcat(ntFollow[get_hash(non_terminal)], ",");
                    if (!strlen(ntFollow[get_hash(grammar[i]->value)]))
                        createFollow(grammar[i]->value);
                    strcat(ntFollow[get_hash(non_terminal)], ntFollow[get_hash(grammar[i]->value)]);
                }
            }
            else
            {
                
                if (strlen(ntFollow[get_hash(non_terminal)]) && ntFollow[get_hash(non_terminal)][strlen(ntFollow[get_hash(non_terminal)]) - 1] != ',')
                    strcat(ntFollow[get_hash(non_terminal)], ",");
                
                strcat(ntFollow[get_hash(non_terminal)], ntFirst[get_hash(temp->value)]);
            }
        }
    }
}

void computeFirstAndFollow()
{
    for (int i = 0; i < NUM_RULES; i++)
    {
        int j = 0;
        char *first;
        char *follow;
        char *token;
        char temp2[300];
        struct node *temp = grammar[i]->forward_link;
        //printf("%s\n",ntFirst[get_hash(temp->value)]);
        while (temp != NULL && isEpsilon[get_hash(temp->value)])
        {
            if(strcmp(temp->value,"EPSILON")){
                first = ntFirst[get_hash(temp->value)];
                strcpy(temp2,ntFirst[get_hash(temp->value)]);
                token = strtok(temp2, comma); 
                //printf("%s\n",token);
                firstAndFollow[i][j++]=strdup(token);
                while ((token = strtok(NULL, comma)) != NULL) 
                {
                    firstAndFollow[i][j++]=strdup(token);
                }
            }
            //strcpy(ntFirst[get_hash(temp->value)],temp2);
            temp = temp->forward_link;
        }
        if (temp == NULL)
        {
            follow = ntFollow[get_hash(grammar[i]->value)];
            strcpy(temp2,ntFollow[get_hash(grammar[i]->value)]);
            token = strtok(temp2, comma);
            //printf("%s\n",token);
            firstAndFollow[i][j++]=strdup(token);
            while ((token = strtok(NULL, comma)) != NULL)
            {
                firstAndFollow[i][j++]=strdup(token);
            }
            //strcpy(ntFollow[get_hash(grammar[i]->value)],temp2);
        }
        else
        {
            //printf("***%s****",temp->value);
            // printf("%s",ntFirst[get_hash(temp->value)]);
            first = ntFirst[get_hash(temp->value)];
            strcpy(temp2,ntFirst[get_hash(temp->value)]);
            //printf("%s\n",temp2);
            token = strtok(temp2, comma);
            //printf("%s,%d\n",token,i+1);
            firstAndFollow[i][j++] = strdup(token);
            while ((token = strtok(NULL, comma)) != NULL)
            {
                // if(!strcmp(temp->value,"op1"))
                //       printf("**RULE NO.,%d**\n",i+1);
                firstAndFollow[i][j++] = strdup(token);
            }
            //strcpy(ntFirst[get_hash(temp->value)],temp2);
            // for(int k=0;firstAndFollow[i][k]!=NULL;k++)
            // printf("%s ",firstAndFollow[i][k]);
            // printf("\n");
            // if(!strcmp(temp->value,"op1"))
            //     printf("%s\n",ntFirst[get_hash(temp->value)]);
        }
        // printf("RULE %d:",i+1);
        // for(int k=0;firstAndFollow[i][k]!=NULL;k++){
        //     printf(" %s ",firstAndFollow[i][k]);
        // }
        // printf("\n");
    }
}

// int main()
// {
//     FILE *fp = fopen("grammar.txt", "r");
//     makeGrammar(fp);
//     memset(isEpsilon, 0, sizeof(isEpsilon));
//     for (int i = 0; i < 126; i++)
//     {
//         char *non_terminal = nonTerminals[i];
//         createfirst(non_terminal); // calculate first of lhs
//     }
//     isEpsilon[get_hash("EPSILON")] = 1;
//     // createfirst("arithmeticOrBooleanExpr");
//     // for (int i = 0; i < 126; i++)
//     //     printf("%s:%s\n", nonTerminals[i], ntFirst[get_hash(nonTerminals[i])]);
//     for (int i = 0; i < 68; i++)
//     {
//         char *non_terminal = nonTerminals[i];
//         // printf("%s\n", non_terminal);
//         createFollow(non_terminal); // calculate first of lhs
//         removeDuplicates(ntFollow[get_hash(non_terminal)]);
//     }
//     // createFollow("whichId");

//     // //printf("%s\n\n",ntFollow[get_hash("whichId")]);
//     // removeDuplicates(ntFollow[get_hash("whichId")]);
//     // printf("%s",ntFollow[get_hash("whichId")]);

//     // for (int i = 0; i < 68; i++)
//     // {
//     //     printf("%s:%s\n", nonTerminals[i], ntFollow[get_hash(nonTerminals[i])]);
//     // }
//     // printf("%s", ntFirst[get_hash("arithmeticOrBooleanExpr")]);
//     computeFirstAndFollow();
//     // for (int i = 0; i <NUM_RULES; i++)
//     // {
//     //     printf("RULE:%d  ", i+1);
//     //     for (int j = 0; firstAndFollow[i][j] != NULL; j++)
//     //         printf("%s ", firstAndFollow[i][j]);
//     //     printf("\n");
//     // }
//     //printf("%s*",firstAndFollow[11][0]);
//     createParseTable();
//     // for(int i=0;i<HASH_MOD;i++){
//     //     for(int j=0;j<HASH_MOD;j++){
//     //         if(parseTable[i][j]!=NULL)
//     //             printf("%d,%d:%s\n",i,j,parseTable[i][j]->value);
//     //     }
//     // }
    
//     //stack initialisation
//     stackElement* element = malloc(sizeof(stackElement));
//     element->isTerminal = 0;
//     strcpy(element->value,"startprogram");
//     s_push(element);

//     //root element of tree creation
//     struct treeNode* tree_node = malloc(sizeof(struct treeNode));
//     tree_node->children = NULL;
//     tree_node->isTerminal = 0;
//     tree_node->parent = NULL;
//     tree_node->nextSibling = NULL;
//     tree_node->prevSibling = NULL;
//     strcpy(tree_node->value,"startprogram");

//     //setting root and currentExpand
//     root = tree_node;
//     currExpand = tree_node;

//     global_token.hasError = 0;
//     global_token.line_no = 1;
//     strcpy(global_token.tk_data.lexeme,"<<<");
//     global_token.tk_name = DRIVERDEF;

//     runPDA();

//     global_token.hasError = 0;
//     global_token.line_no = 1;
//     strcpy(global_token.tk_data.lexeme,"driver");
//     global_token.tk_name = DRIVER;

//     runPDA();

//     global_token.hasError = 0;
//     global_token.line_no = 1;
//     strcpy(global_token.tk_data.lexeme,"program");
//     global_token.tk_name = PROGRAM ;

//     runPDA();

//     global_token.hasError = 0;
//     global_token.line_no = 1;
//     strcpy(global_token.tk_data.lexeme,">>>");
//     global_token.tk_name = DRIVERENDDEF;

//     runPDA();

//     global_token.hasError = 0;
//     global_token.line_no = 2;
//     strcpy(global_token.tk_data.lexeme,"start");
//     global_token.tk_name = START;

//     runPDA();

//     global_token.hasError = 0;
//     global_token.line_no = 3;
//     strcpy(global_token.tk_data.lexeme,"end");
//     global_token.tk_name = END;

//     runPDA();
//     //printf("%s",currExpand->value);
    
//     return 0;
// }

