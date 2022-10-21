#include <arch/i386/hyperhal/asm/io.h>
#include <hyperhal/arch_hal/hal_init.h>
#include <hyperhal/arch_hal/hal_bvideo.h>
#include <hyperhal/arch_hal/hal_interrupt.h>
#include <hyperhal/arch_hal/hal_infostruct.h>
#include <arch/i386/hyperhal/drv/vcursor.h>
#include <arch/i386/hyperhal/drv/serial.h>
#include <arch/i386/hyperhal/mm/mem.h>
#include <hyperhal/loadelf.h>
#include <hyperhal/string.h>
#include <hyperhal/oops.h>
#include <hyperhal/hhsc.h>
#include <printf.h>
#include <hyperhal/basefnc.h>
#include <ver.h>
#include <multiboot.h>
#include <types.h>
#include <logos.h>

multiboot_info_t *mb_info1;
bool verbose = false;
extern uint32_t end; 					// link.ld
extern void hhec_entry(); 				// hhec.cpp
extern void main(hal_info_t* info); 	// main.c

int __stack_chk_fail_local = 0; 		// stub thing for src/hyperhal/printf.c

void print_multiboot_info(int mb_magic, multiboot_info_t *mb_info)
{
	kmsg("\n**%s**\n", __func__);
	kmsg("mb_magic=0x%x\t mb_info at 0x%x:                \n", mb_magic, mb_info);
	kmsg("mb_info->flags=0x%x\t mb_info->mem_lower=%d KiB  \n", mb_info->flags, mb_info->mem_lower);
	kmsg("mb_info->mem_upper=%d KiB                      \n", mb_info->mem_upper);
	kmsg("mb_info->boot_device=0x%x\t mb_info->mmap_addr=%x\n", mb_info->boot_device, (uintptr_t)mb_info->mmap_addr);
	kmsg("mb_info->mmap_length=%d\t bytes mb_info->cmdline at 0x%x\n", mb_info->mmap_length, (uintptr_t)mb_info->cmdline);
	kmsg("mb_info->cmdline=\"%s\"\n", (char*)mb_info->cmdline);
	kmsg("mb_info->mods_count: %d\n", mb_info->mods_count);
	kmsg("**%s**\n\n", __func__);
}

void _entry(int mb_magic, multiboot_info_t *mb_info)
{
	mb_info1 = mb_info; // for oops hhec
	init_bvideo(WHITE, BLUE);
#ifdef HEADLESS
	bputs("This are headless version of HyperHAL.\nAll system output/input redirected to COM1.\nIf you use QEMU, add to arguments -serial stdio, if you use other VM, set up COM1 port by another way.\n");
#endif
	disable_cursor();
	i386_init_serial();
	serial_printf(NEONOS_LOGO);
	kmsg("HyperHAL arch bootstrap b%04d\n", BUILD);
	kmsg("HyperHAL/%s HyperHAL v%d.%d.%d.%04d:%s compiled %s\n\n", BRANCH_NAME, GVER, LVER, PATCH, BUILD, COMMIT_ID, __TIMESTAMP__);
#ifdef HEADLESS
	kmsg("Running headless mode...\n");
#endif
	if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		kmsg("HyperHAL detected non-Multiboot bootloader\nPlease, load HyperHAL from Multiboot bootloader (GRUB, for example)\n");
		do_halt();
	}

	// Check bit 6 to see if we have a valid memory map
    if (!(mb_info->flags >> 6 & 0x1)) 
	{
        kmsg("HyperHAL detected incorrect memory map\nPlease, restart PC manually\n");
		do_halt();
    }
	
	print_multiboot_info(mb_magic, mb_info);
	lock_printf();

	hal_info_t info = {
		.cmdline = (char*)mb_info->cmdline,
		.memory_size = mb_info->mem_lower + mb_info->mem_upper,
		.mods_address = mb_info->mods_addr,
		.mods_count = mb_info->mods_count,
	};

	kmsg("Bootstrapping HyperHAL...\n");
	main(&info);
}
