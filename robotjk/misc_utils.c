/*
 * misc_utils.c
 *
 *  Created on: 2016��12��10��
 *      Author: Kiah
 */
#include "data_type.h"

void delay(uint delayClock) {
    byte x, j;
    for (j = 0; j < delayClock; j++)
        for (x = 0; x <= 148; x++)
            ;
}

void delayUs(uint delayTime) {
    while (delayTime--)
        ;
}

void delay10Ms(uint delayTime) {
    uint i, j;
    for (i = 0; i < delayTime; i++)
        for (j = 0; j < 1827; j++)
            //�����ͨ���������ó�����
            ;
}

char decimal2Hex(byte n) {
    if(n>9) {
        return 'A'+(n-10);
    } else {
        return '0' + n;
    }
}
