#include <hyperhal/arch_hal/hal_bvideo.h>
#include <hyperhal/arch_hal/hal_interrupt.h>
#include <hyperhal/arch_hal/hal_keyboard.h>
#include <hyperhal/arch_hal/hal_init.h>
#include <hyperhal/arch_hal/hal_infostruct.h>
#include <arch/i386/hyperhal/mm/mem.h>
#include <hyperhal/hhsc.h>
#include <hyperhal/loadelf.h>
#include <hyperhal/basefnc.h>
#include <hyperhal/string.h>
#include <ver.h>
#include <types.h>
#include <printf.h>
#include <msg.h>

extern bool verbose;
extern void ecos_entry(); // src/hyperhal/kernel.c

void main(hal_info_t* info)
{    
    // ************************ PARSE ARGS ************************

	char* str;
	char* ostr;
	while(1)
	{
		str = strtok(info->cmdline, ' ');
		if (strcmp(str, ostr) == 0) break;
		if (strcmp(str, "everbose=1") == 0) verbose = true;
		ostr = str;
	}

	if (verbose) unlock_printf();

	// ************************ PARSE ARGS ************************

	if (info->memory_size < 7040)
	{
		kmsg("HyperHAL detected memory size less than 8 MB\n");
		do_halt();
	}
	arch_init();
	unlock_printf();
	bool meminit = init_mem();
	if(!meminit)
	{
		kmsg("Memory initialization cannot proceed\n");
		do_halt();
	}
	kmsg("mem\n");
	if (info->mods_count < 1)
	{
		kmsg("HyperHAL cannot load nK without loading them from bootloader\n(press any key to continue)\n");
		hal_kbd_getkey();
		hal_save_current_bvideo_state();
		ecos_entry();
		hal_restore_current_bvideo_state();
		do_halt();
	}
	multiboot_module_t* kernelm = info->mods_address;
	void* kernel = (uint32_t*)kernelm->mod_start;
#ifdef DEBUG
	hhsc_print("syscalls are working!\n");
#endif
	if(kernel != NULL)
	{
		kmsg("kernel addr: %x\n", kernel);
		loadelf_print_header(loadelf_get_header(kernel));
		Elf32_Ehdr* kernel_header = loadelf_get_header(kernel);
		loadelf_print_program_header(loadelf_get_program_header(kernel, 0));
		
		// int (*kmain)() = kernel;
		// kmsg("\nKernel returned code: %x\n", kmain());
		message_t test_msg = {
			.priority    = 1,
			.type 	     = 0,
			.sender      = 0,
			.destination = 0,
			.dsize       = 0,
			.divided     = false,
		}; // do_halt() as message

		hhsc_send_msg(&test_msg);
	}
	else
	{
		kmsg("HyperHAL cannot bootstrap nK\nSystem unable to start :(\nGo to HHEC! (press any key to continue)\n");
		hal_kbd_getkey();
		hal_save_current_bvideo_state();
		ecos_entry();
		hal_restore_current_bvideo_state();
		do_halt();
	}
}
