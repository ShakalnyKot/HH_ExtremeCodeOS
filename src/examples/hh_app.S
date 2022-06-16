bits 32
org 0x111000
section .text

start:
    mov ebx, 01h
    lea ecx, [teststr2]
    mov eax, 0xfacefeed
    ; int 50h
    ; cli
    ; .l: 
    ; hlt
    ; jmp .l
    ret

; teststr: db 'Test stub', 0

teststr2: db 'Idiot! neonOS are not completed, just delete this ISO', 0