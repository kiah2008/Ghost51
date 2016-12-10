/*
 * system_queue.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */
#include "data_type.h"
#include "system_queue.h"
#include <string.h>
#include <stdlib.h>
#include "stc516rd.h"
#include "request.h"
#include "log.h"

static MessageQueueCtx idata sMessageQueue;
static byte idata secInterval = 0;
static volatile ushort message_queue_ind = 0;

void initializeMessageQueue(void) {
    memset(&sMessageQueue, 0, sizeof(MessageQueueCtx));
    sMessageQueue.isInited = 1;
}

extern void notifyInfo(ushort num);
/*****************************************************************************
 * SendMessage
 * @message
 * @value
 * @return
 *****************************************************************************/
byte sendMessage(MessageType messageType, void* value) {
    msg_t* msg = null;
    byte total = 0;
    if (sMessageQueue.isInited != true) {
        return EQ_NONINITED;
    }
    enterCritical();

    total = sMessageQueue.total;
    if (total >= MESSAGE_QUEUE_NUM) {
        //full
        exitCritical();
        return EQ_FULL;
    }
    //check duplicated
    switch (messageType) {
    case MSG_SYSTEM_TICK:
        case MSG_UART_IO_ERROR:
        if (message_queue_ind & (1 << messageType)) {
            exitCritical();
            return EQ_DUP_MSG;
        } else {
            message_queue_ind |= (1 << messageType);
        }
        break;
    }

    msg = (msg_t*) malloc(sizeof(msg_t));
    msg->msgtype = messageType;
    msg->datavalue = value;
    msg->next = null;
    if (total == 0) {
        sMessageQueue.top = msg;
        msg->next = null;
    } else {
        (sMessageQueue.top + total - 1)->next = msg;
    }
    sMessageQueue.total++;
    exitCritical();
    return E_OK;
}

msg_t* pendMessageQueue(void) {
    msg_t* out = null;
    if (sMessageQueue.isInited != true) {
        return null;
    }
    while (sMessageQueue.total == 0)
        ;

    enterCritical();
    out = sMessageQueue.top;
    sMessageQueue.top = sMessageQueue.top->next;
    sMessageQueue.total--;
    //remote duplicated
    switch (out->msgtype) {
    case MSG_SYSTEM_TICK:
        case MSG_UART_IO_ERROR:
        message_queue_ind &= ~(1 << out->msgtype);
        break;
    }
    exitCritical();
    return out;
}

extern void handleKeyService(void);
//extern void handleRtcService();
//extern void handleTimerSystem(void);
extern void putString(string string, byte sum);
extern void restoreReceive();
extern void putByte(byte byte);
extern byte code disp_table[16];
extern byte uartSendData(char* sdata, byte len);
extern bool g_debug;
void startMessageLoop() {
    msg_t* messageData = null;
    ushort key;
    serial_recv_ctx* serial_data = null;
    serial_data_t* serial_block = null;
    byte i = 0;
    if (g_debug) {
        log("loop entered!");
    }
    while (true) {
        messageData = pendMessageQueue();
        if (messageData == null) {
            continue;
        }
        switch (messageData->msgtype) {
        case MSG_SYSTEM_TICK:
            if (secInterval % 4 == 0) {
                //about 4*50=200ms>>>200*2=400ms interval at least
                handleKeyService();
            }

#ifdef __RTC_SUPPORT__
            if (secInterval++ == SECOND_INTERVAL_MAX) {
                handleRtcService();
            }
#endif

            if (secInterval++ > SECOND_INTERVAL_MAX) {
                secInterval = 0;
            }
            if (g_debug && sMessageQueue.total >= 10) {
                //begin dump
                log("queue warning!");
            }
            break;
#ifdef TIMER_SUPPORT
            case MSG_TIMER:
            handleTimerSystem();
            break;
#endif
        case MSG_UART_RECV:
            serial_data = (serial_recv_ctx*) messageData->datavalue;
            putString("OK\n", 3);
            restoreReceive();
            break;
        case MSG_UART_SEND:
            serial_block = (serial_data_t*) messageData->datavalue;
            if (serial_block != 0) {
                if (serial_block->sdata != null) {
                    for (i = 0; i < serial_block->len; i++) {
                        putByte(serial_block->sdata[i]);
                    }
                    putByte('\n');
                    //free data
                    free(serial_block->sdata);
                }
                free(serial_block);
            }

            break;
        case MSG_DUMP_STATE:

            break;
        case MSG_UART_IO_ERROR:
            serial_data = (serial_recv_ctx*) messageData->datavalue;
            //putString("BAD", 3);
            if (serial_data->token != INVALID) {
                putByte(disp_table[serial_data->token / 16]);
                putByte(disp_table[serial_data->token % 16]);
            }
            putByte('0' + serial_data->dataun.error);
            putByte('\n');
            restoreReceive();
            break;
        default:
            break;
        }
        free(messageData);
    }
}
