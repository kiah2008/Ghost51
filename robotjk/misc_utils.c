/*
 * misc_utils.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */
#include "data_type.h"

//ms
void delay(ushort delayClock) {
    byte x, j;
    for (j = 0; j < delayClock; j++) {
        for (x = 0; x <= 148; x++)
            ;
	}
}

void delayUs(uint delayTime) {
    while (delayTime--)
        ;
}

char decimal2Hex(byte n) {
    if(n>9) {
        return 'A'+(n-10);
    } else {
        return '0' + n;
    }
}
