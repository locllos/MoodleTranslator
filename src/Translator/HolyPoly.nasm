global HolyPoly_asm

prime_number equ 7988587

section .text

HolyPoly_asm:
    movzx rcx, byte [rdi]
    inc rdi
    mov rax, 1
        
.loop:
    movzx rdx, byte [rdi]
    test rdx, rdx
    je .endLoop

    imul rdx, rax
    add rcx, rdx

    imul rax, prime_number

    inc rdi
    jmp .loop
.endLoop:

    mov rax, rcx
    xor rdx, rdx
    div rsi
    mov rax, rdx

    ret









