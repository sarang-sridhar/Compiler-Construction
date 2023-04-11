#include<stdio.h>
#include "IRCodeGen.c"
#include "IRCodeGen.h"
#include "symbolTable.c"
#include<string.h>

void initUnitialisedData(FILE* fp){  //to reserve memory for uninitialised data section: section.bss
    fprintf(fp,"section .bss");
    for(int i=0;i<count;i++){
        quad temp = quadTable[i];
        if(!strcmp(temp.op,"declare") && temp.result.arg_var->is_array!=1){  //for non array elements
            char* label = temp.label;
            fprintf(fp,"/t/t %s resb %d /n/ ",label,temp.result->width); //storing width bytes for temporary 
        }
    }
}

void initDataSection(FILE* fp){    //to store messages :section.data
    fprintf(fp,"section .data");

}


void codegen(){
    FILE* fp = fopen("code.asm","w");

    for(int i=0;i<count;i++){
       quad curr = quadTable[i];
       if(!strcmp(curr.op,"=")){
        ST_ENTRY* lval = curr.result.arg.arg_var;
        if(lval->is_array==0){
            fprintf(fp,"\t\t\t xor rcx,rcx\n "); //to flush previously existing value in rcx
            fprintf(fp,"\t\t\tmov [rbp + %d],rcx\n ",lval->width);
        }
       }    
       if(!strcmp(curr.op,"PLUS") ){
            qEn res = curr.result;
            qEn a1 = curr.arg1;
            qEn a2 = curr.arg2;

            if(arg1.entry==0){
                fprintf(fp,"\t\t\t mov rax,[rbp+%d] \n",arg1.arg.arg_var->width);
            }
            else if(arg1.entry==1){
                fprintf(fp,"\t\t\t mov rax,%d \n",arg1.arg.arg_num);
            }
            else{
                //how to handle real case
            }


            if(arg2.entry==0){
                fprintf(fp,"\t\t\t mov rbx,[rsp+%d] \n",arg2.arg.arg_var->width);
            }
            else if(arg2.entry==1){
                fprintf(fp,"\t\t\t mov rbx,%d \n",arg2.arg.arg_num);
            }
            else{
                //how to handle real case
            }


            


       }
    }
}