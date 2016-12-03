

#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <string.h>
#include <stdio.h>

#ifdef __TEST__
#include "reg52x2_emu.h"
#else
#include "reg52x2.h"
#endif

/*
1、类型定义兼容C#，尽可能用C#标准，常用的为int
2、char在C#中为16bit的unicode，keil C下为8bit
*/
//8bit
//typedef char				sbyte;	    /*char 不能用于负数，实际测试跟unsigned char等价*/
typedef unsigned char		byte;       /*实际测试byte = char*/
//16bit
//typedef short			short
typedef unsigned short		ushort;
//32bit
//typedef int				int;
typedef unsigned int	 	uint;

//字符串
typedef char *			    string;

typedef unsigned char		bool;

typedef void (*function)(void);
#define Function(address)	(*(function)address)()

#define S_CACHE_SIZE 16
typedef struct {
	byte cache[S_CACHE_SIZE +1];
	byte size;
}s_cache_t;

/*定义常量*/
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


typedef enum
{
    KeyMessageType      = 0xFF,
	InputMessageType = 0xF5,
	DisplayMessageType = 0xF4,
	ExternalInterrupt = 0xF3,
    PherialMessageType =0xF2,
	SystemTickMessageType = 0xF1,
    TimerMessageType    = 0xF0  /*最小值,不允许有值小于它*/
}MessageType;


#include "device.h"

extern void EnterCritical(void);
extern void ExitCritical(void);

extern void InitializeMessageQueue(void);
extern void SendMessage(MessageType messageType, ushort value);
extern ushort PendMessageQueue(void);

extern void Delay(uint delayClock);

extern void DelayUs(uint delayTime);

extern void DelayMs(uint delayTime);


extern byte HexToAscii(byte hex);

extern void PutByte(byte byte);
extern void PutString(string string, byte sum);

extern bool TimerStart(TimerhandleMode handleMode, ushort delay, function callBackFunction);
extern void TimerStop(byte id);
extern void HandleTimerSystem(void);
extern void display(ushort num);

#endif

