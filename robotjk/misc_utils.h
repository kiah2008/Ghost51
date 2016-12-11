/*
 * misc_utils.h
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */

#ifndef MISC_UTILS_H_
#define MISC_UTILS_H_

#include "data_type.h"

extern void delay(uint delayClock);
extern void delayUs(uint delayTime);
extern void delay10Ms(uint delayTime);
extern char decimal2Hex(byte n);

#define SetBit(regiset, offset)         regiset |= 1U << offset
#define ResetBit(regiset, offset)       regiset &= ~(1U << offset)
#define GetBit(regiset, offset)         ((regiset >> offset) &0x01)

#define UshortToByte1(data)               ((byte *)(&data))[0]
#define UshortToByte0(data)               ((byte *)(&data))[1]

#define UintToByte3(data)                 ((byte *)(&data))[0]
#define UintToByte2(data)                 ((byte *)(&data))[1]
#define UintToByte1(data)                 ((byte *)(&data))[2]
#define UintToByte0(data)                 ((byte *)(&data))[3]

#define UintToUshort1(data)                 ((ushort *)(&data))[0]
#define UintToUshort0(data)                 ((ushort *)(&data))[1]

#define Assert(express)  if (!(express)){printf("\nASSERT: " #express "\n");}   //(void)0

#endif /* MISC_UTILS_H_ */
