/*
 * device_key.c
 *
 *  Created on: 2016年12月10日
 *      Author: Kiah
 */

#include "data_type.h"
#include "misc_utils.h"
#include "system_queue.h"

#include "stc516rd.h"

#define SHORTINTERVAL			3
#define LONGINTERVAL			20
#define JITTERINTERVAL	        1

static byte idata ScanValueSave = 0xFF;
static byte JitterCounter = JITTERINTERVAL;

/*****************************************************************************
 * ScandPin
 * P1口4*4
 *****************************************************************************/
static byte scandPin(void) {
    byte i, j, temp, Buffer[4] = { 0xef, 0xdf, 0xbf, 0x7f };
    for (j = 0; j < 4; j++) {
        P1 = Buffer[j];
        delayUs(5);
        temp = 0x01;
        for (i = 0; i < 4; i++) {
            if (!(P1 & temp)) {
                return (i + j * 4);
            }
            temp <<= 1;
        }
    }
    return INVALID;
}

void handleKeyService(void) {
    byte rp1;
    if (JitterCounter != 0) {
        JitterCounter--;
        return;
    }
    enterCritical();
    rp1 = P1;
    P1 = 0xf0;
    if (P1 != 0xf0) {
        delay(15);
        ScanValueSave = scandPin();
        sendMessage(MSG_KEY_INPUT, (void*)ScanValueSave);
        JitterCounter = JITTERINTERVAL;
    } else {
        ScanValueSave = INVALID;
    }
    P1 = rp1;
    exitCritical();
}

