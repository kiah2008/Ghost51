/*
 * log.h
 *
 *  Created on: 2016��12��11��
 *      Author: Kiah
 */

#ifndef LOG_H_
#define LOG_H_
extern byte uartSendData(char* sdata, byte len);
//#define log(x, y) uartSendData(x##y, 0)
#define log(x) uartSendData(x, 0)

#endif /* LOG_H_ */
