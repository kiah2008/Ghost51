/*
 * system_queue.h
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */

#ifndef SYSTEM_QUEUE_H_
#define SYSTEM_QUEUE_H_

#include "data_type.h"
#include "system.h"
#include "misc_utils.h"

#define MESSAGE_QUEUE_NUM 16

typedef enum {
    //MSG_TIMER,
    MSG_SYSTEM_TICK, //about 50 ms
    MSG_UART_RECV,
    MSG_UART_SEND,
    MSG_UART_IO_ERROR,
    //MSG_EXT_INT,
    MSG_DUMP_STATE,
    MSG_KEY_INPUT,
    MSG_KEY_MODE_CHANGE,
    MSG_INVALID = 0x0F
} MessageType;

typedef struct _msg_t {
    MessageType msgtype;
    char* datavalue;
    struct _msg_t* next;
} msg_t;

typedef struct {
    byte total;
    msg_t* top;
    byte serial;
    bool isInited;
} MessageQueueCtx;

#define SECOND_INTERVAL_MAX 20

extern void initializeMessageQueue(void);
extern byte sendMessage(MessageType messageType, char* value);
extern msg_t* pendMessageQueue(void);

#endif /* SYSTEM_QUEUE_H_ */
