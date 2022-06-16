#ifndef ELF_H
#define ELF_H
#include <types.h>

typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;
typedef uint16_t Elf32_Section;
typedef uint16_t Elf32_Versym;
typedef unsigned char Elf_Byte;
typedef uint16_t Elf32_Half;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf32_Word;
typedef int64_t Elf32_Sxword;
typedef uint64_t Elf32_Xword;

#define EI_NIDENT 16

#define EI_MAG0 0x7F
#define EI_MAG1 'E'
#define EI_MAG2 'L'
#define EI_MAG3 'F'

#define EI_CLASS_NONE 0
#define EI_CLASS_32 1
#define EI_CLASS_64 2

#define EI_DATA_NONE 0
#define EI_DATA_LITTLE 1
#define EI_DATA_BIG 2

#define ET_NONE 0x00
#define ET_REL 0x01
#define ET_EXEC 0x02
#define ET_DYN 0x03
#define ET_CORE 0x04
#define ET_LOOS 0xFE00
#define ET_HIOS 0xFEFF
#define ET_LOPROC 0xFF00
#define ET_HIPROC 0xFFFF

#define SHT_NULL 0x0
#define SHT_PROGBITS 0x1
#define SHT_SYMTAB 0x2
#define SHT_STRTAB 0x3
#define SHT_RELA 0x4
#define SHT_HASH 0x5
#define SHT_DYNAMIC 0x6
#define SHT_NOTE 0x7
#define SHT_NOBITS 0x8
#define SHT_REL 0x9
#define SHT_SHLIB 0x0A
#define SHT_DYNSYM 0x0B
#define SHT_INIT_ARRAY 0x0E
#define SHT_FINI_ARRAY 0x0F
#define SHT_PREINIT_ARRAY 0x10
#define SHT_GROUP 0x11
#define SHT_SYMTAB_SHNDX 0x12
#define SHT_NUM 0x13
#define SHT_LOOS 0x60000000

#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MERGE 0x10
#define SHF_STRINGS 0x20
#define SHF_INFO_LINK 0x40
#define SHF_LINK_ORDER 0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP 0x200
#define SHF_TLS 0x400
#define SHF_MASKOS 0x0FF00000
#define SHF_MASKPROC 0xF0000000
#define SHF_ORDERED 0x4000000
#define SHF_EXCLUDE 0x8000000

#define SHN_UNDEF 0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC 0xff00
#define SHN_BEFORE 0xff00
#define SHN_AFTER 0xff01
#define SHN_HIPROC 0xff1f
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
#define SHN_HIRESERVE 0xffff

typedef struct 
{
	uint8_t 	   e_ident[EI_NIDENT];
	uint16_t       e_type;
	uint16_t       e_machine;
	uint32_t       e_version;
	Elf32_Addr     e_entry;
	Elf32_Off      e_phoff;
	Elf32_Off      e_shoff;
	uint32_t       e_flags;
	uint16_t       e_ehsize;
	uint16_t       e_phentsize;
	uint16_t       e_phnum;
	uint16_t       e_shentsize;
	uint16_t       e_shnum;
	uint16_t       e_shstrndx;
} Elf32_Ehdr;

typedef struct
{
	uint32_t       p_type;
	Elf32_Off      p_offset;
	Elf32_Addr     p_vaddr;
	Elf32_Addr     p_paddr;
	uint32_t       p_filesz;
	uint32_t       p_memsz;
	uint32_t       p_flags;
	uint32_t       p_align;
} Elf32_Phdr;

typedef struct
{
	uint32_t       sh_name;
	Elf32_Off      sh_type;
	Elf32_Addr     sh_flags;
	Elf32_Addr     sh_addr;
	uint32_t       sh_offset;
	uint32_t       sh_size;
	uint32_t       sh_link;
	uint32_t       sh_info;
	uint32_t       sh_addralign;
	uint32_t       sh_entsize;
} Elf32_Shdr;

typedef struct {
	uint32_t      st_name;
	Elf32_Addr    st_value;
	uint32_t      st_size;
	unsigned char st_info;
	unsigned char st_other;
	uint16_t      st_shndx;
} Elf32_Sym;
typedef struct {
	Elf32_Addr r_offset;
	uint32_t   r_info;
} Elf32_Rel;

typedef struct {
	Elf32_Addr r_offset;
	uint32_t   r_info;
	int32_t    r_addend;
} Elf32_Rela;

#endif //  ELF_H