/*
 * system_queue.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */
#include "data_type.h"
#include "system_queue.h"
#include "misc_utils.h"
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
extern byte code led_table[16];
extern byte uartSendData(char* sdata, byte len);

void startMessageLoop() {
    msg_t* messageData = null;
    byte key;
    serial_recv_ctx* serial_rctx = null;
    serial_data_t* serial_data = null;
    byte i = 0;
    log("loop entered!");

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

            if (secInterval++ >= SECOND_INTERVAL_MAX) {
                secInterval = 0;
            }

            if (sMessageQueue.total >= 10) {
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
            serial_data = (serial_data_t*) messageData->datavalue;
            putString("OK");
            putByte(decimal2Hex(serial_data->recvToken / 16));
            putByte(decimal2Hex(serial_data->recvToken % 16));
            putByte('\n');
            //handle data
            restoreReceive();
            break;
        case MSG_UART_SEND:
            serial_data = (serial_data_t*) messageData->datavalue;
            if (serial_data != 0) {
                if (serial_data->sdata != null) {
                    for (i = 0; i < serial_data->len; i++) {
                        putByte(serial_data->sdata[i]);
                    }
                    putByte('\n');
                    //free data
                    free(serial_data->sdata);
                }
                free(serial_data);
            }

            break;
        case MSG_KEY_INPUT:
            key = (byte) messageData->datavalue;
            putString("KEY");
            putByte(decimal2Hex(key / 16));
            putByte(decimal2Hex(key % 16));
            putByte('\n');
            break;
        case MSG_DUMP_STATE:

            break;
        case MSG_UART_IO_ERROR:
            serial_rctx = (serial_recv_ctx*) messageData->datavalue;
            putString("BAD");
            if (serial_rctx->datablock->recvToken != INVALID) {
                putByte(decimal2Hex(serial_rctx->datablock->recvToken / 16));
                putByte(decimal2Hex(serial_rctx->datablock->recvToken % 16));
            } else {
                putByte(decimal2Hex(0));
                putByte(decimal2Hex(0));
            }
            putByte(decimal2Hex(serial_rctx->datablock->error / 16));
            putByte(decimal2Hex(serial_rctx->datablock->error % 16));
            putByte('\n');
            restoreReceive();
            break;
        default:
            break;
        }
        free(messageData);
    }
}
