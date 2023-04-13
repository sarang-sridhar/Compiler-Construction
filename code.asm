global main
extern printf,scanf

section .data
			 inputMsgInteger: db "Enter an integer value ",0
			 trueMsg: db "true",0 
			 falseMsg: db "false",0 
			 format: db "%hi",0 
section .bss
			 x resb 2 
			 y resb 2 
			 z resb 2 
			 a resb 2 
			 b resb 2 
			 c resb 2 
section .text
main:
			 push rbp
			 mov rbp,rsp
			 sub rsp,16
			 mov WORD [rbp - 6],5 ;loading into a 
			 mov WORD [rbp - 8],9 ;loading into b 
			 L4:
 			 xor eax, eax
			 lea rdi, [inputMsgInteger]
			 call printf
			 mov eax,0
			 lea rdi, [format]
			 lea rsi, [rbp - 0]
			 call scanf
			 L5:
 			 xor eax, eax
			 lea rdi, [inputMsgInteger]
			 call printf
			 mov eax,0
			 lea rdi, [format]
			 lea rsi, [rbp - 2]
			 call scanf
			 mov ax,[rbp-2];moving leftop:y from memory to ax 
 			 mov bx,[rbp-8] ;moving b from memory to rbx 
			 imul ax,bx 
 			 mov [rbp -12],ax ;done for mul
			 mov ax,[rbp-0];moving leftop:x from memory to ax 
 			 mov bx,[rbp-12] ;moving t_0 from memory to rbx 
			 add ax,bx 
 			 mov [rbp -14],ax ;done for PLUS 
			 mov ax,[rbp-6];moving leftop:a from memory to ax 
 			 mov bx,[rbp-8] ;moving b from memory to rbx 
			 sub ax,bx 
 			 mov [rbp -16],ax ;done for MINUS 
			 mov ax,[rbp-16];moving leftop:t_2 from memory to ax 
 			 mov bx,[rbp-2] ;moving y from memory to rbx 
			 imul ax,bx 
 			 mov [rbp -18],ax ;done for mul
			 mov ax,[rbp-14];moving leftop:t_1 from memory to ax 
 			 mov bx,[rbp-18] ;moving t_3 from memory to rbx 
			 add ax,bx 
 			 mov [rbp -20],ax ;done for PLUS 
			 mov ax,[rbp-6];moving leftop:a from memory to ax 
 			 mov bx,2 
			 imul ax,bx 
 			 mov [rbp -22],ax ;done for mul
			 mov ax,[rbp-20];moving leftop:t_4 from memory to ax 
 			 mov bx,[rbp-22] ;moving t_5 from memory to rbx 
			 add ax,bx 
 			 mov [rbp -24],ax ;done for PLUS 
			 mov ax,[rbp-8];moving leftop:b from memory to ax 
 			 mov bx,[rbp-0] ;moving x from memory to rbx 
			 imul ax,bx 
 			 mov [rbp -26],ax ;done for mul
			 mov ax,[rbp-24];moving leftop:t_6 from memory to ax 
 			 mov bx,[rbp-26] ;moving t_7 from memory to rbx 
			 sub ax,bx 
 			 mov [rbp -28],ax ;done for MINUS 
			 mov cx,[rbp - 28];moving rhs temporary/id:t_8 to temp reg rcx 
 			 mov [rbp - 4],cx ;loading into z's memory 
 			 L7:
 			 xor eax, eax 
 			 mov esi,[rbp - 4]
 			 lea rdi,[format]
			 call printf  
			 return:
			 add rsp,16
			 leave
			 ret
