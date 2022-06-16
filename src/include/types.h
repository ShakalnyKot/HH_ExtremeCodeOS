#ifndef _TYPES_H
#define _TYPES_H

#define DEBUG
//#define HEADLESS
#define NULL 0

typedef unsigned int size_t;

typedef unsigned long long uint64_t;
typedef unsigned long uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef unsigned int uint;

typedef long long int64_t;
typedef long int32_t;
typedef short int16_t;
typedef char int8_t;

typedef signed long long intptr_t;
typedef unsigned long long uintptr_t;

typedef signed long long reg_t;
typedef unsigned long long ureg_t;

typedef signed long long ptrdiff_t;

#ifndef __cplusplus
typedef enum { false, true } bool;
#endif

#endif
