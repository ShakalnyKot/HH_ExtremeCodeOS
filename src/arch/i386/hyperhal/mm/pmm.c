#include <arch/i386/hyperhal/mm/mem.h>
#include <arch/i386/hyperhal/mm/mem_asm.h>
#include <arch/i386/hyperhal/drv/bvideo.h>
#include <hyperhal/string.h>
#include <multiboot.h>
#include <printf.h>
#include <hyperhal/basefnc.h>
#include <hyperhal/oops.h>

mmap_entry_t* entry;
extern multiboot_info_t* mb_info1;
unsigned long *page_directory = (unsigned long *) 0x9C000;
unsigned long *page_table = (unsigned long *) 0x9D000; // the page table comes right after the page directory
void* mem_end;
MemHead_t head;

// from Linux sources
static inline void __native_flush_tlb_single(unsigned long addr) {
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void mp_map_page(void *physaddr, void *virtualaddr, unsigned int flags) 
{
    // Make sure that both addresses are page-aligned.
 
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
    serial_printf("Mapping %x to %x:%x, %x - %x\n", physaddr, pdindex, ptindex, virtualaddr, flags);
    page_directory[pdindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
 
}

void mp_filltable()
{
    kmsg("%s...", __FUNCTION__);
    unsigned long address = 0; // holds the physical address of where a page is
    unsigned int i;

    serial_printf("Writing page_table...\n");
    // map the first 4MB of memory
    for(i=0; i<1024; i++)
    {
        page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
        address = address + 4096; // 4096 = 4kb
    };
    // fill the first entry of the page directory
    page_directory[0] = page_table; // attribute set to: supervisor level, read/write, present(011 in binary)
    page_directory[0] = page_directory[0] | 3;
    serial_printf("Writing page_directory...\n");
    for(i=1; i<1024; i++)
    {
        page_directory[i] = 0 | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
    };
}

void mp_enablepaging() 
{
    kmsg("%s...", __FUNCTION__);
    // write_cr3, read_cr3, write_cr0, and read_cr0 all come from the assembly functions
    write_cr3((uint32_t*)page_directory); // put that page directory address into CR3
    write_cr0(read_cr0() | 0x80000000); // set the paging bit in CR0 to 1

    // go celebrate or something 'cause PAGING IS ENABLED!!!!!!!!!!
}
void mp_setpagedir()
{

}
void *get_physaddr(void *virtualaddr) {
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    return (void *)((page_directory[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));
}

bool init_mem()
{
	kmsg("init_mem...");
    entry = mb_info1->mmap_addr;
	while(entry < mb_info1->mmap_addr + mb_info1->mmap_length) {
		// do something with the entry
		entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
        if(entry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            mem_end = (uint8_t*)((entry->addr_high + entry->addr_low) + (entry->len_high + entry->len_low));
            break;
        }
	}
    mp_filltable();
    // set up our page directory
    mp_enablepaging();
    uint32_t tlb_result;
    __native_flush_tlb_single(tlb_result);
    kmsg("Results of __native_flush_tlb_single:%x...", tlb_result);

    // uint32_t* ptr = (uint32_t*)0xDEADBEEF;
    // ((uint32_t*)ptr)[0] = 0xfacefeed;
    return true;

}

mmap_entry_t* mm_get_entry()
{
    return entry;
}