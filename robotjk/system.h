
#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <string.h>
#include <stdio.h>

#ifdef __TEST__
#include "reg52x2_emu.h"
#else
#include "reg52x2.h"
#endif

#include "device.h"

//8bit
typedef unsigned char byte;
//16bit
typedef unsigned short ushort;
//32bit
typedef unsigned int uint;

typedef char * string;

typedef unsigned char bool;

typedef void (*function)(void);

#define Function(address)	(*(function)address)()

#define S_CACHE_SIZE 16
typedef struct {
	byte cache[S_CACHE_SIZE + 1];
	byte size;
} s_cache_t;

#define true				1
#define false				0
#define null                ((void *) 0)		/*Stdlib.h and String.h already include NULL */
#define INVALID             0xFF

#define MainClock           11059200

#define SetBit(regiset, offset)    		regiset |= 1U << offset
#define ResetBit(regiset, offset)  		regiset &= ~(1U << offset)
#define GetBit(regiset, offset)	   		((regiset >> offset) &0x01)

#define UshortToByte1(data)               ((byte *)(&data))[0]
#define UshortToByte0(data)               ((byte *)(&data))[1]

#define UintToByte3(data)                 ((byte *)(&data))[0]
#define UintToByte2(data)                 ((byte *)(&data))[1]
#define UintToByte1(data)                 ((byte *)(&data))[2]
#define UintToByte0(data)                 ((byte *)(&data))[3]

#define UintToUshort1(data)                 ((ushort *)(&data))[0]
#define UintToUshort0(data)                 ((ushort *)(&data))[1]

#define GetMessageType(data)                UshortToByte1(data)
#define GetMessageData(data)                UshortToByte0(data)

#define Assert(express)  if (!(express)){printf("\nASSERT: " #express "\n");}   //(void)0

typedef enum {
	TimerMessageType = 0xF0 /*最小值,不允许有值小于它*/,
	SystemTickMessageType = 0xF1,
	PherialMessageType = 0xF2,
	ExternalInterrupt = 0xF3,
	DisplayMessageType = 0xF4,
	InputMessageType = 0xF5,
	KeyMessageType = 0xFF
} MessageType;

extern void EnterCritical(void);
extern void exitCritical(void);

extern void initializeMessageQueue(void);
extern void sendMessage(MessageType messageType, ushort value);
extern ushort pendMessageQueue(void);

extern void delay(uint delayClock);

extern void delayUs(uint delayTime);

extern void delayMs(uint delayTime);

extern byte hexToAscii(byte hex);

extern void PutByte(byte byte);
extern void PutString(string string, byte sum);

extern bool timerStart(TimerhandleMode handleMode, ushort delay,
		function callBackFunction);
extern void timerStop(byte id);
extern void handleTimerSystem(void);
extern void display(ushort num);

#endif

