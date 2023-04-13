global main
extern printf,scanf

section .data
			 inputMsgInteger: db "Enter an integer value",0
			 trueMsg: db "true",0 
			 falseMsg: db "false",0 
			 format: db "%hi",0 
			 formatBool: db "%hhi",0 
			 lineBreak: db 0ah
section .bss
			 x resb 2 
			 y resb 2 
			 z resb 2 
			 a resb 2 
			 b resb 2 
			 c resb 2 
			 u resb 1 
			 v resb 1 
section .text
main:
			 push rbp
			 mov rbp,rsp
			 sub rsp,16
			 mov BYTE [rbp - 12],1 
			 mov WORD [rbp - 6],5 ;loading into a 
			 mov WORD [rbp - 8],9 ;loading into b 
			 L6:
 			 xor eax, eax
			 lea rdi, [inputMsgInteger]
			 call printf
			 xor eax,eax
			 lea rdi,[lineBreak]
			 call printf
			 mov eax,0
			 lea rdi, [format]
			 lea rsi, [rbp - 0]
			 call scanf
			 L7:
 			 xor eax, eax
			 lea rdi, [inputMsgInteger]
			 call printf
			 xor eax,eax
			 lea rdi,[lineBreak]
			 call printf
			 mov eax,0
			 lea rdi, [format]
			 lea rsi, [rbp - 2]
			 call scanf
			 mov ax,[rbp-2];moving leftop:y from memory to ax 
 			 mov bx,[rbp-8] ;moving b from memory to rbx 
			 imul ax,bx 
 			 mov [rbp -14],ax ;done for mul
			 mov ax,[rbp-0];moving leftop:x from memory to ax 
 			 mov bx,[rbp-14] ;moving t_0 from memory to rbx 
			 add ax,bx 
 			 mov [rbp -16],ax ;done for PLUS 
			 mov ax,[rbp-6];moving leftop:a from memory to ax 
 			 mov bx,[rbp-8] ;moving b from memory to rbx 
			 sub ax,bx 
 			 mov [rbp -18],ax ;done for MINUS 
			 mov ax,[rbp-18];moving leftop:t_2 from memory to ax 
 			 mov bx,[rbp-2] ;moving y from memory to rbx 
			 imul ax,bx 
 			 mov [rbp -20],ax ;done for mul
			 mov ax,[rbp-16];moving leftop:t_1 from memory to ax 
 			 mov bx,[rbp-20] ;moving t_3 from memory to rbx 
			 add ax,bx 
 			 mov [rbp -22],ax ;done for PLUS 
			 mov ax,[rbp-6];moving leftop:a from memory to ax 
 			 mov bx,2 
			 imul ax,bx 
 			 mov [rbp -24],ax ;done for mul
			 mov ax,[rbp-22];moving leftop:t_4 from memory to ax 
 			 mov bx,[rbp-24] ;moving t_5 from memory to rbx 
			 add ax,bx 
 			 mov [rbp -26],ax ;done for PLUS 
			 mov ax,[rbp-8];moving leftop:b from memory to ax 
 			 mov bx,[rbp-0] ;moving x from memory to rbx 
			 imul ax,bx 
 			 mov [rbp -28],ax ;done for mul
			 mov ax,[rbp-26];moving leftop:t_6 from memory to ax 
 			 mov bx,[rbp-28] ;moving t_7 from memory to rbx 
			 sub ax,bx 
 			 mov [rbp -30],ax ;done for MINUS 
			 mov cx,[rbp - 30];moving rhs temporary/id:t_8 to temp reg rcx 
 			 mov [rbp - 4],cx ;loading into z's memory 
 			 mov ax,[rbp-4] 
 			 mov bx,10 
 			 cmp ax,bx 
			 setg [rbp - 32]
			 mov ax,[rbp-6] 
 			 mov bx,[rbp-8] 
 			 cmp ax,bx 
			 setle [rbp - 34]
			 mov al,[rbp-32] 
			 mov bl,[rbp-34]
			 or al,bl
 mov [rbp -36],al
			 mov ax,[rbp-0] 
 			 mov bx,[rbp-2] 
 			 cmp ax,bx 
			 setl [rbp - 38]
			 mov al,[rbp-36] 
			 mov bl,[rbp-38]
			 and al,bl
 mov [rbp -40],al
			 mov al,[rbp-40] 
			 mov bl,[rbp-12]
			 and al,bl
 mov [rbp -42],al
			 mov cl,[rbp - 42];moving rhs temporary/id:t_14 to temp reg rcx 
 			 mov [rbp - 13],cl ;loading into v's memory 
 			 L12:
 			 xor eax, eax 
 			 mov esi,[rbp - 4]
 			 lea rdi,[format]
			 call printf  
			 xor eax,eax
			 lea rdi,[lineBreak]
			 call printf
			 L13:
 			 xor eax, eax 
 			 mov esi,[rbp - 13]
 			 lea rdi,[formatBool]
			 call printf  
			 xor eax,eax
			 lea rdi,[lineBreak]
			 call printf
			 return:
			 add rsp,16
			 leave
			 ret
