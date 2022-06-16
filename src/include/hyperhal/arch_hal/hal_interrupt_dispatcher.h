#ifndef HAL_INTERRUPT_DISPATCHER_H
#define HAL_INTERRUPT_DISPATCHER_H

#ifdef __i386__
#define CALL_INTERRUPT(vector) asm volatile("int $" vector "\n")
#else
#define CALL_INTERRUPT(vector)
#endif

#endif