#ifndef PIC8259A_H
#define PIC8259A_H

#define PIC_1_CTRL 0x20
#define PIC_2_CTRL 0xA0
#define PIC_1_DATA 0x21
#define PIC_2_DATA 0xA1
#define PIC_EOI		0x20		/* End-of-interrupt command code */

#include <types.h>

bool init_pic8259a();
void PIC_sendEOI(unsigned char irq);

#endif // IRQ_H
