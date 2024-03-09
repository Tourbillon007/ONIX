[bits 32]

extern exit

test:
    ret

global main
main:
    ; push 5;
    ; push eax

    ; pop ebx
    ; pop ecx

    ; pusha

    ; popa
    call test
    push 0

    call exit;