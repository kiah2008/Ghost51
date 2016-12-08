#ifndef __DEVICE_H
#define __DEVICE_H

typedef enum {
	TimerInterruptHandle = 0, TimerMessageHandle = 1
} TimerhandleMode;

struct Rtc {
	byte Second;
	byte Minute;
	byte Hour;
	uint Day;
};

typedef enum {
	SystemTick10000, SystemTick1000, SystemTick100
} SystemTick;

extern struct Rtc idata Rtc;

extern void keySystemTickService(void);

extern void rtcSystemTickService(void);

extern void timerSystemTickService(void);

extern void initializeRtc(void);

extern void InitializeTrace(void);

extern void initializeSystemTick(void);

#endif

