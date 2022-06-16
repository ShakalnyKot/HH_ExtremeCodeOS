bits 32
section .text

; this functions is global
global read_port
global write_port
global io_delay

read_port:                    ; port reading
    mov edx, [esp + 4]
    in al, dx   
    ret

write_port:                   ; port writing
    mov   edx, [esp + 4]    
    mov   al, [esp + 4 + 4]  
    out   dx, al  
    ret

io_delay:                     ; idfk why this shit was here
    nop
    nop
    nop
    nop
    ret