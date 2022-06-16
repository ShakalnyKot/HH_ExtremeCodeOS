#ifndef MEM_H
#define MEM_H
#include <types.h>
#include <multiboot.h>

#define BANK_SIZE 4096
#define BUFFER_END 0xA0000
#define MEMHEAD_ENTRY_COUNT 4096
typedef multiboot_memory_map_t mmap_entry_t;

typedef struct page_dir_entry {
    unsigned int present    : 1;
    unsigned int rw         : 1;
    unsigned int user       : 1;
    unsigned int w_through  : 1;
    unsigned int cache      : 1;
    unsigned int access     : 1;
    unsigned int reserved   : 1;
    unsigned int page_size  : 1;
    unsigned int global     : 1;
    unsigned int available  : 3;
    unsigned int frame      : 20;
} page_dir_entry_t;

typedef struct page_table_entry {
    unsigned int present    : 1;
    unsigned int rw         : 1;
    unsigned int user       : 1;
    unsigned int reserved   : 2;
    unsigned int accessed   : 1;
    unsigned int dirty      : 1;
    unsigned int reserved2  : 2;
    unsigned int available  : 3;
    unsigned int frame      : 20;
} page_table_entry_t;

typedef struct page_table
{
    page_table_entry_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    // The actual page directory entries(note that the frame number it stores is physical address)
    page_dir_entry_t tables[1024];
    // We need a table that contains virtual address, so that we can actually get to the tables
    page_table_t* ref_tables[1024];
} page_directory_t;

typedef struct MemEntry {
    uint32_t addr;
    uint16_t size;
    uint8_t owner;
    uint8_t rw;
    uint8_t rs;
    uint8_t used;
    char eoe[2];
} MemEntry_t;

typedef struct MemHead {
    char* mem_magic;
    uint16_t size;
    MemEntry_t entries[4096];
} MemHead_t;

void mp_enablepaging();
void mp_filltable();
void mm_map_page(void *physaddr, void *virtualaddr, unsigned int flags);
bool init_mem();
mmap_entry_t* mm_get_entry();

#endif // !MEM_H