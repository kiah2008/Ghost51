/*
 * log.h
 *
 *  Created on: 2016Äê12ÔÂ11ÈÕ
 *      Author: Kiah
 */

#ifndef LOG_H_
#define LOG_H_
extern byte uartSendData(char* sdata, byte len);
//#define log(x, y) uartSendData(x##y, 0)
#ifdef _G_DEBUG_
#define log(x) uartSendData(x, 0)
#else
#define log(x)
#endif

#endif /* LOG_H_ */
