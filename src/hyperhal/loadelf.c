#include <hyperhal/loadelf.h>
#include <exec/elf.h>
#include <types.h>
#include <printf.h>

const char* elf_class_for_human[3] = { "Invalid", "32 bit", "64 bit" };
const char* elf_data_for_human[3] = { "Invalid", "Little", "Big" };
const char* elf_osabi_for_human[18] = { "System V", "HP-UX", "NetBSD", "Linux", "GNU Hurd", "Solaris", "AIX (Monterey)", "IRIX", "FreeBSD", "Novell Modesto", "OpenBSD", "OpenVMS", "NonStop Kernel", "AROS", "FenixOS", "Nuxi CloudABI", "NetBSD", "Stratus Technologies OpenVOS" };

/*

ELF OS ABIs (get from Wikipedia):

0x00	System V
0x01	HP-UX
0x02	NetBSD
0x03	Linux
0x04	GNU Hurd
0x06	Solaris
0x07	AIX (Monterey)
0x08	IRIX
0x09	FreeBSD
0x0A	Tru64
0x0B	Novell Modesto
0x0C	OpenBSD
0x0D	OpenVMS
0x0E	NonStop Kernel
0x0F	AROS
0x10	FenixOS
0x11	Nuxi CloudABI
0x12	Stratus Technologies OpenVOS

*/

Elf32_Ehdr* loadelf_get_header(void* data)
{
	return (Elf32_Ehdr*)data;
}

Elf32_Phdr* loadelf_get_program_header(void* data, int number)
{
	int offset = 0;
	if(number > 1)
	{
		offset = sizeof(Elf32_Phdr) * number;
	}
	return (Elf32_Phdr*)((uint8_t*)data + ((Elf32_Ehdr*)data)->e_phoff + offset);
}

void loadelf_print_header(Elf32_Ehdr* header)
{
	kmsg("%s: ELF File Magic: (0x%x, %c%c%c)\n", __FUNCTION__, 
		header->e_ident[0],
		header->e_ident[1],
		header->e_ident[2],
		header->e_ident[3]
	);
	kmsg("%s: ELF File Class: (%s)\n", __FUNCTION__, 
		elf_class_for_human[header->e_ident[4]]
	);
	kmsg("%s: ELF File Data Endian: (%s)\n", __FUNCTION__, 
		elf_data_for_human[header->e_ident[5]]
	);
	kmsg("%s: ELF File Version: (%x)\n", __FUNCTION__, 
		header->e_ident[6]
	);
	if(header->e_ident[7] != 0x35)
	{
		kmsg("%s: ELF File OS ABI / OS: (0x%x - %s)\n", __FUNCTION__, 
			header->e_ident[7],
			elf_osabi_for_human[header->e_ident[7]] 
		);
	}
	else
	{
		kmsg("%s: ELF File OS ABI / OS: (0x35 - nK-OS (neonOS))\n", __FUNCTION__);
	}
	kmsg("%s: ELF File ABI Version: (0x%x)\n", __FUNCTION__, 
		header->e_ident[8]
	);
	kmsg("%s: ELF File Type: (0x%x)\n", __FUNCTION__, 
		header->e_type
	);
	kmsg("%s: ELF File Instruction Set: (0x%x)\n", __FUNCTION__, 
		header->e_machine
	);
	kmsg("%s: ELF File Version: (%d)\n", __FUNCTION__, 
		header->e_version
	);
	kmsg("%s: ELF File Entry Point: (0x%x)\n", __FUNCTION__, 
		header->e_entry
	);
	kmsg("%s: ELF File Program Header Table: (0x%x)\n", __FUNCTION__, 
		header->e_phoff
	);
	kmsg("%s: ELF File Program Header Count: (%d)\n", __FUNCTION__, 
		header->e_phnum
	);
	kmsg("%s: ELF File Section Header Offset: (0x%x)\n", __FUNCTION__, 
		header->e_shoff
	);
}

void loadelf_print_program_header(Elf32_Phdr* header)
{
	kmsg("%s: ELF Program Header Segment Type: (0x%x)\n", __FUNCTION__, 
		header->p_type
	);
	kmsg("%s: ELF Program Header Flags: (0x%x)\n", __FUNCTION__, 
		header->p_flags
	);
	kmsg("%s: ELF Program Header Offset: (0x%x)\n", __FUNCTION__, 
		header->p_offset
	);
	kmsg("%s: ELF Program Header Virtual Address: (0x%x)\n", __FUNCTION__, 
		header->p_vaddr
	);
	kmsg("%s: ELF Program Header Physical Address: (0x%x)\n", __FUNCTION__, 
		header->p_paddr
	);
	kmsg("%s: ELF Program Header Segment Size: (%d)\n", __FUNCTION__, 
		header->p_filesz
	);
	kmsg("%s: ELF Program Header Memory Segment Size: (0x%x)\n", __FUNCTION__, 
		header->p_memsz
	);
	kmsg("%s: ELF Program Header Alignment: (%d)\n", __FUNCTION__, 
		header->p_align
	);
}