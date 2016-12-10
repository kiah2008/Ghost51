/*
 * device.h
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */
#ifndef __DEVICE_H
#define __DEVICE_H
#include "data_type.h"

typedef enum {
    TimerInterruptHandle = 0, TimerMessageHandle = 1
} TimerhandleMode;

typedef struct {
    unsigned long rtc_l; //4bit
    unsigned long rtc_h;
} rtc_t;

//for uart
extern void initializeUart(void);
extern void putByte(byte byte);
extern void putString(char* string, byte sum);
extern void notifyInfo(ushort num);

extern void handleKeyService(void);

// timer
extern void timerSystemTickService(void);
extern void timerStop(byte id);
extern void handleTimerSystem(void);
extern byte timerStart(ulong high, ulong low, function callback);

extern void initializeSystemTick(void);

// for rtc
extern bool isRtcTimeout(rtc_t* check);
extern bool isRtcTimeout(rtc_t* check);
extern rtc_t* getDelayRtc(ulong high, ulong low);
extern void handleRtcService();

#endif

