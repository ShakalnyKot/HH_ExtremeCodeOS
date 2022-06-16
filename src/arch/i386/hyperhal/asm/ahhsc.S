bits 32
section .text

global hhsc_asm_isr
extern hhsc_c_isr

hhsc_asm_isr:
    pushad
    cld
    call hhsc_c_isr
    popad
    iretd