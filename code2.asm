global main
extern printf, scanf

section .text
main:
    push rbp           ; Keep the stack aligned
    mov  rbp, rsp 
    sub rsp, 16        ;//must be 16 byte increments  to maintain state alignment

    mov DWORD [rbp-4],5
    mov rcx,[rbp-4]
    mov [rbp-8],rcx

    ;write our body
    xor eax, eax        ;
    mov esi,[rbp-8]
    lea rdi,[format]    ;first parameter
    call printf

    add rsp, 16 
    leave               ; mov rsp, rbp, pop rbp
    ret

section .data
   msg: db "Enter a: ",0
   msg2: db "Looping %d of %d",10,0
   format:db "%d",0

section .bss
   number resb 4
   z resb 4