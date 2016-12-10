/*
 * device_timer.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */
#ifdef TIMER_SUPPORT
#include "data_type.h"
#include "device.h"
#include "system_queue.h"
#include "misc_utils.h"
#include <string.h>
#include <stdio.h>

#include "stc516rd.h"

struct Timer {
    rtc_t* delay;
    function callback;
};

#define TIMER_SUM		0x4

static byte idata TimerStateMask = 0;
static struct Timer xdata Block[TIMER_SUM];

void timerSystemTickService(void) {
    sendMessage(MSG_TIMER, null);
}

void handleTimerSystem(void) {
    byte i = 0;
    byte stateCopy;

    if (TimerStateMask == 0x00) {
        return;
    }

    enterCritical();
    stateCopy = TimerStateMask;
    while (stateCopy) {
        if ((stateCopy & 0x01) == 1) {
            if (isRtcTimeout(Block[i].delay)) {
                (*(Block[i].callback))();
                ResetBit(TimerStateMask, i);
            }
        }

        stateCopy = stateCopy >> 1;
        i++;
    }
    exitCritical();
}

extern void free (void xdata *p); 

/**
 * delay, s
 * @return, id or INVALID
 */
byte timerStart(ulong high, ulong low, function callback) {
    byte i;
    enterCritical();
    for (i = 0; i < TIMER_SUM; i++) {
        if (!GetBit(TimerStateMask, i)) {
            Block[i].delay = getDelayRtc(high, low);
            Block[i].callback = callback;
            SetBit(TimerStateMask, i);
            exitCritical();
            return (i);
        }
    }
    exitCritical();
    return INVALID;
}

/**
 * id, the timerStart returned
 */
void timerStop(byte id) {
    if(id >= TIMER_SUM) {
        return;
    }

    enterCritical();
    if (GetBit(TimerStateMask, id)) {
        ResetBit(TimerStateMask, id);
        free(Block[id].delay);
        Block[id].callback = null;
    }
    exitCritical();
}
#endif

