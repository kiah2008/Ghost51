/*
 * system.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */

#include <string.h>
#include "system.h"
#include "misc_utils.h"
#include "stc516rd.h"
#include "system_queue.h"
#include "log.h"

byte code led_table[16] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
        0x7f,
        0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71 };
static bool EASave;
static byte volatile CriticalNesting = 0;

sbit KEYEXT0 = P3 ^ 2;    //INT0
sbit KEYEXT1 = P3 ^ 3;

extern void initializeSystemTick(void);
extern void initializeUart(void);
extern byte uartSendData(char* sdata, byte len);


void initialize(void) {
    EA = 0;
    initializeMessageQueue();
    initializeSystemTick();
    initializeUart();

    IPLX0 = 1;
    IPLX1 = 1;
    IPLT0 = 1;
    EX0 = 1;
    EX1 = 1;
    //timer 1 8bit reload, timer 0 mode 1 16bit
    TMOD = 0X21;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
    TR1 = 1;
    if(GetBit(PCON, 4) == 1) {
        ResetBit(PCON, 4);
        uartSendData("RDY", 0);
    } else {
        uartSendData("RST", 0);
    }
    WDT_CONTR = 0x35;
}

void display(ushort num) {
    int i = 0;
    byte rp0 = 0xF;
    byte rp1 = 0xF;
    byte bits[4];
    byte j = 0;
    bits[0] = (num >> 12) & 0x0F;
    bits[1] = (num >> 8) & 0x0F;
    bits[2] = (num >> 4) & 0x0F;
    bits[3] = num & 0x0F;
    enterCritical();
    rp0 = P0;
    rp1 = P2;
    for (i = 0; i < 60; i++) {
        for (j = 0; j < 4; j++) {
            P0 = led_table[bits[j]]; //LED Value
            P2 = j; //bit select
            delay(5);
        }
    }
    exitCritical();
    delay(10);
}

void enterCritical(void) {
    if (CriticalNesting == 0) {
        EASave = EA;
        EA = 0;
    }
    CriticalNesting++;
}

void exitCritical(void) {
    if (CriticalNesting > 0) {
        CriticalNesting--;
        if (CriticalNesting == 0) {
            EA = EASave;
        }
    }
}

static void ExternalInterruptHandler0(void)
interrupt 0 {
    EX0=0;
    delay(5);
    if(KEYEXT0 == 0) {
        sendMessage(MSG_KEY_MODE_CHANGE, 0);
    }
    delay(100);
    EX0=1;
}

//static void ExternalInterruptHandler1(void)
//interrupt 2 {
//  EX1=0;
//  Delay(1);
//  if(P3^3 == 0) {
//      displayValue=0;
//      display((DisplayMessageType<<8) + displayValue);
//  }
//  Delay(3);
//  EX1=1;
//}

//dump context
void dump() {
    //RTC:h-m-s;Queue:((in+MESSAGE_QUEUE_NUM-out)%MESSAGE_QUEUE_NUM)/MESSAGE_QUEUE_NUM;
    const char* format = "RTC:%d-%d-%s;Queue:%d";

}

