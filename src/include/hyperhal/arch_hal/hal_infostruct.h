#pragma once
#include <types.h>

typedef struct hal_info
{
    char* cmdline;
    uint32_t memory_size;

    uint32_t mods_count;
    uint32_t mods_address;
} hal_info_t;