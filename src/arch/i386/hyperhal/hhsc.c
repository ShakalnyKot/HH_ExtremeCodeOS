#include <hyperhal/arch_hal/hal_interrupt.h>
#include <arch/i386/hyperhal/asm/io.h>
#include <arch/i386/hyperhal/drv/bvideo.h>
#include <hyperhal/msg.h>
#include <hyperhal/hhsc.h>
#include <hyperhal/oops.h>
#include <types.h>
#include <printf.h>

/*

Avaliaible HHSCs:

eax=0x1 ecx=[ADDRESS_TO_STRING] - puts
eax=0x2 ecx=[ADDRESS_TO_STRING] - panic
eax=0x3 edx=vectror ecx=[ADDRESS_TO_HANDLER]- registrate interrupt
eax=0x4 ecx=[ADDRESS_TO_MESSAGE] - send message

*/

extern void hhsc_asm_isr(); // hyperhal/asm/hhsc.S
hal_easy_interrupt_t* interrupt;

void hhsc_c_isr()
{
	uint32_t eax, ebx, ecx, edx, esi, edi;
	asm volatile("movl %%eax, %0\n"
				"movl %%ebx, %1\n"
				"movl %%ecx, %2\n"
				"movl %%edx, %3\n"
				"movl %%esi, %4\n"
				"movl %%edi, %5\n":
				"=g" (eax),
	 			"=g" (ebx),
	 			"=g" (ecx),
	 			"=g" (edx),
	 			"=g" (esi),
	 			"=g" (edi));
	// chgColor(GREY, BLUE);
	serial_printf("\n**%s**\neax=%08x ebx=%08x\necx=%08x edx=%08x\nesi=%08x edi=%08x\n**%s**\n\n", __func__, eax, ebx, ecx, edx, esi, edi, __func__);
	// chgColor(WHITE, BLUE);
	switch (eax)
	{
	case 0x1:
		kmsg((char*)ecx); // ecx points to string
		break;
	case 0x2:
		panic((char*)ecx);  // ecx points to string
	case 0x3:
        hal_reg_interrupt(edx, ecx);
		break;
	case 0x4:
		if(((message_t*)ecx)->sender < 1)
			absorb_msg((message_t*)ecx); // send_msg for HH
		break;
	default:
		panic("Invalid HAL Syscall");
		break;
	}
}

bool init_hhsc(uint8_t vector)
{
	hal_reg_interrupt(vector, (uint32_t)hhsc_asm_isr);
}

/* u/Octocontrabass's code */
uint32_t hhsc_call( uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi, uint32_t edi )
{
    asm volatile( "int $0x50" : "+a"(eax) : "b"(ebx), "c"(ecx), "d"(edx), "S"(esi), "D"(edi) );
    return eax;
}
/* End Of Code */

void hhsc_print(const char* msg)
{
	hhsc_call(0x1, NULL, (uint32_t*)msg, NULL, NULL, NULL);
}

void hhsc_reg_interrupt_hander(uint32_t vector, uint32_t handler)
{
    hhsc_call(0x3, NULL, handler, vector, NULL, NULL);
}

void hhsc_send_msg(message_t* msg)
{
    hhsc_call(0x4, NULL, (uint32_t*)msg, NULL, NULL, NULL);
}
