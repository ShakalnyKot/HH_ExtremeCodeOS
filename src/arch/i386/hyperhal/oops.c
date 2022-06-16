#include <hyperhal/arch_hal/hal_keyboard.h>
#include <hyperhal/arch_hal/hal_bvideo.h>
#include <hyperhal/arch_hal/hal_serial.h>
#include <arch/i386/hyperhal/mm/mem_asm.h>
#include <arch/i386/hyperhal/interrupt.h>
#include <arch/i386/hyperhal/drv/pic8259a.h>
#include <arch/i386/hyperhal/asm/io.h>
#include <arch/i386/regs.h>
#include <hyperhal/string.h>
#include <hyperhal/hhsc.h>
#include <hyperhal/basefnc.h>
#include <hyperhal/oops.h>
#include <multiboot.h>
#include <ver.h>
#include <printf.h>

extern multiboot_info_t *mb_info1;
struct regs32 regs_current;
struct interrupt_frame;
uint8_t err_mem_snapshot[256];

bool panic_lock = false;
bool old_panic_lock = false;

/*
Bit 0 (P) is the Present flag.
Bit 1 (R/W) is the Read/Write flag.
Bit 2 (U/S) is the User/Supervisor flag.
Bit 3 (RSVD) indicates whether a reserved bit was set in some page-structure entry
Bit 4 (I/D) is the Instruction/Data flag (1=instruction fetch, 0=data access)
Bit 5 (PK) indicates a protection-key violation
Bit 6 (SS) indicates a shadow-stack access fault
Bit 15 (SGX) indicates an SGX violaton
*/

typedef struct pagefault_error_struct
{
	uint8_t p     : 1;
	uint8_t rw    : 1;
	uint8_t us    : 1;
	uint8_t rsvd  : 1;
	uint8_t id    : 1;
	uint8_t pk    : 1;
	uint8_t ss    : 1;
	uint8_t empty : 8;
	uint8_t sgx   : 1;
} pagefault_error_struct_t;

void hhib(struct interrupt_frame *frame, const char* msg)
{
	printf("Type \"help\" to view list of available commands\n");
	while(true)
	{
		printf("> ");
		char* a = (char*)hal_kbd_getstring();
		char* b = strtok(a, " ");
		if(strcmp(b, "help") == 0)
		{
			printf("list of commands:\nhelp - this list\nregs - pre-fault register states\nreadmem addr - read memory from addr(decremental)\nwritemem addr data - write data(0-255) to memory addr\nexecmem - execute from memory\nreset - reset pc\nver - verison\nhlt - halt pc\nsnapshot - view fault IP memory dump\n");
		}
		else if(strcmp(b, "writemem") == 0)
		{
			char* c = strtok(NULL, " ");
			char* d = strtok(NULL, " ");
			void* mem = (uint32_t*)atoi(c);
			uint8_t data = atoi(d);
			((uint8_t*)mem)[0] = (uint8_t*)data;
			printf("%x <- %02x\n", mem, data);
		}
		else if(strcmp(b, "readmem") == 0)
		{
			char* c = strtok(NULL, " ");
			void* mem = (uint32_t*)atoi(c);
			printf("%x: %02x\n", mem, ((uint8_t*)mem)[0]);

		}
		else if(strcmp(b, "execmem") == 0)
		{
			char* c = strtok(NULL, " ");
			void* mem = (uint32_t*)atoi(c);
			printf("ADDR:%x\n", mem);
			int (*run)() = mem;
			printf("RETURNED:%x\n", run());

		}
		else if(strcmp(b, "snapshot") == 0)
		{
			printf("Fault IP memory snapshot:\n");
			for(uint8_t j = 0; j <= 256; j += 16)
    		{
        		printf("%08x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", frame->ip + j,
               		err_mem_snapshot[j + 0],
               		err_mem_snapshot[j + 1],
               		err_mem_snapshot[j + 2],
               		err_mem_snapshot[j + 3],
               		err_mem_snapshot[j + 4],
               		err_mem_snapshot[j + 5],
               		err_mem_snapshot[j + 6],
               		err_mem_snapshot[j + 7],
               		err_mem_snapshot[j + 8],
               		err_mem_snapshot[j + 9],
               		err_mem_snapshot[j + 10],
               		err_mem_snapshot[j + 11],
               		err_mem_snapshot[j + 12],
               		err_mem_snapshot[j + 13],
               		err_mem_snapshot[j + 14],
               		err_mem_snapshot[j + 15]);
        		if(j == 0xF0)
            		break;
    		}
		}
		else if(strcmp(b, "regs") == 0)
		{
			printf("Registers: \n");
			printf("EAX=0x%08x  "
			"EBX=0x%08x  \n"
			"ECX=0x%08x  "
			"EDX=0x%08x  \n"
			"EDI=0x%08x  "
			"ESI=0x%08x  \n"
			"EBP=0x%08x	"
			"ESP=0x%08x \n"
			"IP=0x%08x  \n"
			"CS=0x%08x  "
			"FLAGS: 0x%x \n"
			"SP=0x%08x  "
			"SS=0x%08x  \n\n", regs_current.eax, 
				regs_current.ebx, 
				regs_current.ecx, 
				regs_current.edx, 
				regs_current.edi, 
				regs_current.esi, 
				regs_current.ebp,
				regs_current.esp, 
				frame->ip, 
				frame->cs, 
				frame->flags, 
				frame->sp, 
				frame->ss);
			if(!strcmp(msg, "Page Fault"))
			{
				// pagefault_error_struct_t* pfs = (pagefault_error_struct_t*)regs_current.esp + sizeof(struct interrupt_frame);
				pagefault_error_struct_t* pfs = (pagefault_error_struct_t*)regs_current.esp;
				printf("Page fault error code as %x: \n", pfs);
				printf("p: %d; rw: %d; us: %d; rsvd: %d; id: %d\npk: %d; ss: %d; sgx: %d\n", 
				(pfs->p), 
				(pfs->rw), 
				(pfs->us), 
				(pfs->rsvd), 
				(pfs->id), 
				(pfs->pk), 
				(pfs->ss), 
				(pfs->sgx));
				uint32_t cr2 = read_cr2();
				printf("CR2=0x%08x\n", cr2);
			}
#ifdef DEBUG
			uint32_t dr0, dr1, dr2, dr3, dr6, dr7;
			asm volatile("mov %%dr0, %0\n"
				"mov %%dr1, %1\n"
				"mov %%dr2, %2\n"
				"mov %%dr3, %3\n"
				"mov %%dr6, %4\n"
				"mov %%dr7, %5\n":
				"=r" (dr0),
				"=r" (dr1),
				"=r" (dr2),
				"=r" (dr3),
				"=r" (dr6),
				"=r" (dr7));
			printf("DR0=0x%016x  \n"
				"DR1=0x%016x  \n"
				"DR2=0x%016x  \n"
				"DR3=0x%016x  \n"
				"DR6=0x%016x  \n"
				"DR7=0x%016x  \n", dr0, dr1, dr2, dr3, dr6, dr7);
#endif
		}
		else if(strcmp(b, "reset") == 0)
		{
			printf("Resetting...");
			do_reset();
		}
		else if(strcmp(b, "ver") == 0)
		{
			printf("neonOS v%d.%d.%d.%04d:%s compiled %s\n", GVER, LVER, PATCH, BUILD, COMMIT_ID, __TIMESTAMP__);
			printf("HyperHAL Internal Fault Console b%04d\n", BUILD);
		}
		else if(strcmp(b, "hlt") == 0)
			do_halt();
		else
		{
			printf("Unknown: %s\n", a);
		}
	}
	bvideo_chgcolor(WHITE, BLUE);
}

__attribute__((no_caller_saved_registers)) void fault(struct interrupt_frame *frame, const char* msg)
{
	INT_OFF;
	asm volatile("mov %%eax, %0\n"
				"mov %%ebx, %1\n"
				"mov %%ecx, %2\n"
				"mov %%edx, %3\n"
				"mov %%esi, %4\n"
				"mov %%edi, %5\n"
				"mov %%esp, %6\n"
				"mov %%ebp, %7\n": 
				"=g" (regs_current.eax),
	 			"=g" (regs_current.ebx),
	 			"=g" (regs_current.ecx),
	 			"=g" (regs_current.edx),
	 			"=g" (regs_current.esi),
	 			"=g" (regs_current.edi),
	 			"=g" (regs_current.esp),
	 			"=g" (regs_current.ebp));
	old_panic_lock = panic_lock;
	panic_lock = true;
	unlock_printf();
	bvideo_chgcolor(WHITE, RED);
	kmsg("\nneonOS v%d.%d.%d.%04d:%s whispers:\n", GVER, LVER, PATCH, BUILD, COMMIT_ID);
	if(!strcmp(msg, "UNIX syscall (0x80) are used in neonOS"))
		kmsg("System Fault: %s\n\n", msg);
	else
		kmsg("CPU Exception: %s\n\n", msg);
	if(strcmp(msg, "Page Fault") != 0)
	{
		void* err_point = (uint32_t*)frame->ip;
		memcpy(&err_mem_snapshot, err_point, 256);
		serial_printf("Fault IP memory snapshot:\n");
		for(uint8_t j = 0; j <= 256; j += 16)
    	{
    		serial_printf("%08x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", frame->ip + j,
        		err_mem_snapshot[j + 0],
        		err_mem_snapshot[j + 1],
    			err_mem_snapshot[j + 2],
        		err_mem_snapshot[j + 3],
        		err_mem_snapshot[j + 4],
        		err_mem_snapshot[j + 5],
        		err_mem_snapshot[j + 6],
        		err_mem_snapshot[j + 7],
        		err_mem_snapshot[j + 8],
        		err_mem_snapshot[j + 9],
       			err_mem_snapshot[j + 10],
        		err_mem_snapshot[j + 11],
        		err_mem_snapshot[j + 12],
        		err_mem_snapshot[j + 13],
        		err_mem_snapshot[j + 14],
        		err_mem_snapshot[j + 15]);
    		if(j == 0xF0)
    			break;
    	}
	}
	serial_printf("\nRegisters: \n");
	serial_printf("EAX=0x%08x  "
			"EBX=0x%08x  \n"
			"ECX=0x%08x  "
			"EDX=0x%08x  \n"
			"EDI=0x%08x  "
			"ESI=0x%08x  \n"
			"EBP=0x%08x	"
			"ESP=0x%08x \n"
			"IP=0x%08x  \n"
			"CS=0x%08x  "
			"FLAGS: 0x%x \n"
			"SP=0x%08x  "
			"SS=0x%08x  \n\n", regs_current.eax, 
				regs_current.ebx, 
				regs_current.ecx, 
				regs_current.edx, 
				regs_current.edi, 
				regs_current.esi, 
				regs_current.ebp,
				regs_current.esp, 
				frame->ip, 
				frame->cs, 
				frame->flags, 
				frame->sp, 
				frame->ss);
	if(!strcmp(msg, "Page Fault"))
	{
		pagefault_error_struct_t* pfs = (pagefault_error_struct_t*)regs_current.esp;
		serial_printf("p: %d; rw: %d; us: %d; rsvd: %d; id: %d\npk: %d; ss: %d; sgx: %d\n", 
				(pfs->p), 
				(pfs->rw), 
				(pfs->us), 
				(pfs->rsvd), 
				(pfs->id), 
				(pfs->pk), 
				(pfs->ss), 
				(pfs->sgx));
		uint32_t cr2 = read_cr2();
		serial_printf("CR2=0x%08x\n", cr2);
	}
	INT_ON;
	if(old_panic_lock) do_halt();
	hhib(frame, msg);
}

void oops(const char* msg)
{
	INT_OFF;
	asm volatile("mov %%eax, %0\n"
				"mov %%ebx, %1\n"
				"mov %%ecx, %2\n"
				"mov %%edx, %3\n"
				"mov %%esi, %4\n"
				"mov %%edi, %5\n"
				"mov %%esp, %6\n"
				"mov %%ebp, %7\n": 
				"=g" (regs_current.eax),
	 			"=g" (regs_current.ebx),
	 			"=g" (regs_current.ecx),
	 			"=g" (regs_current.edx),
	 			"=g" (regs_current.esi),
	 			"=g" (regs_current.edi),
	 			"=g" (regs_current.esp),
	 			"=g" (regs_current.ebp));
	old_panic_lock = panic_lock;
	panic_lock = true;
	if(old_panic_lock) return;
	unlock_printf();
	bvideo_chgcolor(WHITE, BROWN);
	kmsg("\nneonOS v%d.%d.%d.%04d:%s whispers:\n", GVER, LVER, PATCH, BUILD, COMMIT_ID);
	kmsg("OOPS: %s\n\n", msg);
	INT_ON;
	hhib(NULL, msg);
	bvideo_chgcolor(WHITE, BLUE);
	return;
}

void panic(const char* msg)
{
	INT_OFF;
	asm volatile("mov %%eax, %0\n"
				"mov %%ebx, %1\n"
				"mov %%ecx, %2\n"
				"mov %%edx, %3\n"
				"mov %%esi, %4\n"
				"mov %%edi, %5\n"
				"mov %%esp, %6\n"
				"mov %%ebp, %7\n": 
				"=g" (regs_current.eax),
	 			"=g" (regs_current.ebx),
	 			"=g" (regs_current.ecx),
	 			"=g" (regs_current.edx),
	 			"=g" (regs_current.esi),
	 			"=g" (regs_current.edi),
	 			"=g" (regs_current.esp),
	 			"=g" (regs_current.ebp));
	old_panic_lock = panic_lock;
	panic_lock = true;
	if(old_panic_lock) return;
	unlock_printf();
	bvideo_chgcolor(BLACK, CYAN);
	kmsg("\nneonOS v%d.%d.%d.%04d:%s whispers:\n", GVER, LVER, PATCH, BUILD, COMMIT_ID);
	kmsg("HAL panic: %s\n\n", msg);
	INT_ON;
	hhib(NULL, msg);
	bvideo_chgcolor(WHITE, BLUE);
	return;
}

__attribute__((interrupt)) void divOnZero_PanicHandler(struct interrupt_frame *frame) { fault(frame, "Divide-by-zero Error"); }
__attribute__((interrupt)) void breakpoint_PanicHandler(struct interrupt_frame *frame) { fault(frame, "Breakpoint"); }
__attribute__((interrupt)) void overflow_PanicHandler(struct interrupt_frame *frame) { fault(frame, "Overflow"); }
__attribute__((interrupt)) void DeviceNotAvailable_PanicHandler(struct interrupt_frame *frame) { fault(frame, "Device Not Available"); }
__attribute__((interrupt)) void InvalidTSS_PanicHandler(struct interrupt_frame *frame) { fault(frame, "Invalid TSS"); }
__attribute__((interrupt)) void SegNotPres_PanicHandler(struct interrupt_frame *frame) { fault(frame, "Segment Not Present"); }
__attribute__((interrupt)) void StackSegmentFault_PanicHandler(struct interrupt_frame *frame) { fault(frame, "Stack-Segment Fault"); }
__attribute__((interrupt)) void GeneralProtectionFault_PanicHandler(struct interrupt_frame *frame) { fault(frame, "General Protection Fault"); }
__attribute__((interrupt)) void PageFault_PanicHandler(struct interrupt_frame *frame) { fault(frame, "Page Fault"); }
__attribute__((interrupt)) void InvalidOpcode_PanicHandler(struct interrupt_frame *frame) { fault(frame, "Invalid Opcode"); }
__attribute__((interrupt)) void x87FloatingPointException_PanicHandler(struct interrupt_frame *frame) { fault(frame, "x87 Floating-Point Exception"); }
__attribute__((interrupt)) void Int80h_PanicHandler(struct interrupt_frame *frame) { fault(frame, "UNIX syscall (0x80) are used in neonOS"); }

// Exeptions handler init
bool oops_isr_init()
{
  	i386_load_idt_entry(0x0, (uint32_t) divOnZero_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0x3, (uint32_t) breakpoint_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0x4, (uint32_t) overflow_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0x7, (uint32_t) DeviceNotAvailable_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0xA, (uint32_t) InvalidTSS_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0xB, (uint32_t) SegNotPres_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0xC, (uint32_t) StackSegmentFault_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0xD, (uint32_t) GeneralProtectionFault_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0xE, (uint32_t) PageFault_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0x6, (uint32_t) InvalidOpcode_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0x10, (uint32_t) x87FloatingPointException_PanicHandler, 0x08, 0x8e);
	i386_load_idt_entry(0x80, (uint32_t) Int80h_PanicHandler, 0x08, 0x8e); // heh why not?
	return true;
}