bits 32
section .text

global read_cr0
global write_cr0
global read_cr3
global write_cr3
global read_cr2

read_cr0:
	mov eax, cr0
	retn

write_cr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr0,  eax
	pop ebp
	retn

read_cr3:
	mov eax, cr3
	retn

write_cr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	retn

read_cr2:
	mov eax, cr2
	retn