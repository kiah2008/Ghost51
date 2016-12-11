/*
 * device_tick.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */

#include "data_type.h"
#include "system_queue.h"
#include "stc516rd.h"

void initializeSystemTick(void) {
    ET0 = 0;
    TH0 = 0x4c; //50ms
    TL0 = 0x0;
}

void systemTickInterruptHandler(void)
interrupt 1
{
    ET0 = 0;
    TF0 = 0;
    TH0=0x4c;
    TL0=0;
    sendMessage(MSG_SYSTEM_TICK, 0);
    ET0 = 1;
}
