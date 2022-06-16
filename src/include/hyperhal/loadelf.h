#ifndef LOADELF_H
#define LOADELF_H
#include <exec/elf.h>
#include <types.h>

Elf32_Ehdr* loadelf_get_header(void* data);
Elf32_Phdr* loadelf_get_program_header(void* data, int number);
void loadelf_print_header(Elf32_Ehdr* header);
void loadelf_print_program_header(Elf32_Phdr* header);

#endif //  LOADELF_H