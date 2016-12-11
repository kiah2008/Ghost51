/*
 * data_type.h
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */

#ifndef DATA_TYPE_H_
#define DATA_TYPE_H_

typedef unsigned char uchar;
typedef unsigned char bool;
//8bit
typedef unsigned char byte;
//16bit
typedef unsigned short ushort;
//16bit
typedef unsigned int uint;
//32bit
typedef unsigned long ulong;

typedef char* string;

typedef void (*function)(void);

typedef struct {
     volatile int counter;
} atomic_t;


//extern bool g_debug;

#define atomic_read(v)   (*(volatile int *)&(v)->counter)
#define atomic_set(v,i)    (((v)->counter) = (i))

#define true                1
#define false               0
#define null                ((void *) 0)        /*Stdlib.h and String.h already include NULL */
#define INVALID             0xFF

#define E_OK  0
#define EQ_FULL 1
#define EQ_EMPTY 2
#define EQ_NONINITED 3
#define EQ_DUP_MSG  4

//for serial
#define ES_OVERMAX  8
#define ES_OVERCACHE  9
#define ES_INVALID 10

#define E_MEMORY_OVER 15
#define E_UNKNOWN  0xEF

#define Function(address)   (*(function)address)()

#endif /* DATA_TYPE_H_ */
