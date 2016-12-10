/*
 * main.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */
#include <stdlib.h>

#define MAX_HEAP_SIZE 0x300

extern void initialize(void);
extern void startMessageLoop();

static unsigned char xdata XBYTE [MAX_HEAP_SIZE];

void main(void) {
    init_mempool(XBYTE, MAX_HEAP_SIZE);
    initialize();
    startMessageLoop();		      
}
