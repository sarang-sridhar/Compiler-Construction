#include <stdio.h>
#include <string.h>

char nonTerminals[130][24] = {"startprogram","program","moduleDeclarations","moduleDeclaration","otherModules",
"driverModule","module","ret","input_plist","moreList","output_plist","moreOutput",
"dataType","type","moduleDef","statements","statement","ioStmt","print_var","whichId2",
"boolvar","whichId","index","sign","aVar","var_id_num","newArithmeticExpr","startExpr",
"newA1","newA2","newTerm","newNextTerm","u1","after_u1","var_id_num2","simpleStmt","assignmentStmt",
"whichStmt","lvalueIDStmt","lvalueARRStmt","moduleReuseStmt","optional","idList",
"moreId","actualParameter","expression","arithmeticOrBooleanExpr","ab1","anyTerm","ab2","u","after_unary",
"arithmeticExpr","a1","term","a2","nextTerm","op1","op2","bop","relationalOp",
"declareStmt","conditionalStmt","caseStmt","post","value","default","iterativeStmt",
"range_for","range","idList2","moreId2","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER",
"PROGRAM","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TAKES","INPUT","RETURNS","FOR","IN","SWITCH",
"CASE","BREAK","DEFAULT","WHILE","AND","OR","TRUE","FALSE","ID","NUM","RNUM","PLUS","MINUS","MUL","DIV",
"LT","LE","GE","GT","EQ","NE","DEF","ENDDEF","DRIVERDEF","DRIVERENDDEF","COLON","RANGEOP","SEMICOL",
"COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK","DOLLAR"};

// 1949 - one clash
// 1521- zero clash

int get_hash_terminal(const char *s, int num)
{
    int n = strlen(s);
    long long p = 31, m = 10e9 + 7;
    long long hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < n; i++)
    {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return abs(hash) % num;
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(int *arr, int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

int main()
{
    int arr[130];
    for (int i = 0; i < 130; i++)
        arr[i] = get_hash_terminal(nonTerminals[i], 1619);
    bubbleSort(arr, 130);
    for (int i = 0; i < 130; i++)
    {
        printf("%d ", arr[i]);
        if (arr[i] == arr[i + 1])
            printf("Clash here");
    }
}