/*
 * main.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */
#include <stdlib.h>
#include "stc516rd.h"

#define MAX_HEAP_SIZE 0x200

extern void initialize(void);
extern void startMessageLoop();

static unsigned char xdata XBYTE [MAX_HEAP_SIZE];

void main(void) {
    P2=0x0;
    init_mempool(XBYTE, MAX_HEAP_SIZE);
    initialize();
    startMessageLoop();		      
}
