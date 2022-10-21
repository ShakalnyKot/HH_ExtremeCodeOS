#include <printf.h>
#include <types.h>
#include <ver.h>
#include <hyperhal/string.h>
#include <hyperhal/basefnc.h>
#include <hyperhal/arch_hal/hal_bvideo.h>
#include <hyperhal/arch_hal/hal_keyboard.h>
#include <hyperhal/arch_hal/hal_interrupt.h>
#include <hyperhal/arch_hal/hal_serial.h>
#include <hyperhal/arch_hal/hal_interrupt_dispatcher.h>
#include <logos.h>

char cursor[16] = "-> ";

void hhec_entry()
{
	bvideo_clearscr();
	printf(HH_LOGO);
	printf("HyperHAL Internal Console\nType \"help\" to check commands\n");
	while(true)
	{
		printf(cursor);
		char* a = (char *)hal_kbd_getstring();
		char* b = strtok(a, " ");
		if(strcmp(b, "help") == 0)
		{
			printf("list of commands:\nhelp - this list\nreset - reset pc\nver - verison\nhlt - halt pc\necho <args> - print args\nexit - exit from HyperHAL Internal Console\ncls - clear screen\ncurs <cursor> - set new cursor for shell\n");
		}
		else if(strcmp(b, "reset") == 0)
		{
			do_reset();
		}
		else if(strcmp(b, "ver") == 0)
		{
			printf("HyperHAL v%d.%d.%d.%04d:%s compiled %s\n\n", GVER, LVER, PATCH, BUILD, COMMIT_ID, __TIMESTAMP__);
		}
		else if(strcmp(b, "hlt") == 0)
			do_halt();
		else if(strcmp(b, "exit") == 0)
			break;
		else if(strcmp(b, "cls") == 0)
			bvideo_clearscr();
		else if(strcmp(b, "curs") == 0)
		{
			char* c = strtok(NULL, " ");
			printf("New cursor: %s\n", c);
			strcpy(cursor, c);
		}
		else if(strcmp(b, "ex") == 0)
		{
			printf("Committing exception...\n");
			CALL_INTERRUPT("0x80");
		}
		else if (strcmp(b, "echo") == 0)
		{
			char* oldc;
			while (true)
			{
				char* c = strtok(NULL, " ");
				if(strcmp(c, oldc) == 0) break;
				printf("%s ", c);
				oldc = c;
			}
			printf("\n");
		}
		else
		{
			printf("Unknown: %s\n", a);
		}
	}
}