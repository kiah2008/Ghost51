/*
 * device_uart.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */

#include "data_type.h"
#include "misc_utils.h"
#include "system_queue.h"
#include <stdlib.h>
#include <string.h>

#include "stc516rd.h"
#include "request.h"

static serial_recv_ctx serial_rctx;
static volatile byte serial_snd_size;

#define SERIAL_NO_RECEIVING 0
typedef enum {
    STATE_MAGIC,
    STATE_TOKEN,
    STATE_LENGTH,
    STATE_COTENT,
    STATE_PROCESSING,
    STATE_INVALID = 0x7 //max allow value
} SERIAL_STATE;

char putChar(char c) {
    if (c == '\n') {
        ES = 0;
        SBUF = '\r';
        while (!TI)
            ;
        TI = 0;
        ES = 1;
    }
    ES = 0;
    SBUF = c;
    while (!TI)
        ;
    TI = 0;
    ES = 1;
    return (1);
}

void putByte(byte byte) {
    ES = 0;
    SBUF = byte;
    while (!TI)
        ;
    TI = 0;
    ES = 1;
}

void putString(string string) {
    ushort len = strlen(string);
    ushort i = 0;
    for (; i < len; i++) {
        putByte(string[i]);
    }
}

void initializeUart(void) {
    ES = 0;
    ET1 = 0;

    //SMOD=1,double
    SetBit(PCON, 7);
    TH1 = 0xFD; //19200
    TL1 = 0xFD;

    //MODE 1, and enable REN
    SCON = 0x50;
    IPLS = 1;
    // for receive
    memset(&serial_rctx, 0, sizeof(serial_rctx));
    serial_rctx.state = 1 << STATE_MAGIC;
    serial_rctx.token = INVALID;
    serial_rctx.datablock = malloc(sizeof(serial_data_t));
    serial_rctx.datablock->len = 0;
    serial_rctx.datablock->sdata = (byte*) malloc(MAX_SERIAL_RECV_NUM);
    memset(serial_rctx.datablock->sdata, 0, MAX_SERIAL_RECV_NUM);
    RI = 0;
    ES = 1;
    REN = 1;
}

void restoreReceive() {
    serial_rctx.state = (1 << STATE_MAGIC);
    serial_rctx.token = INVALID;
    serial_rctx.idx = 0;
    serial_rctx.datablock->len = 0;
    serial_rctx.datablock->recvToken = INVALID;
    memset(serial_rctx.datablock->sdata, 0, MAX_SERIAL_RECV_NUM);
    ES = 1;
    REN = 1;
}

byte uartSendData(char* sdata, byte len) {
    byte size = 0;
    byte* snddata = null;
    serial_data_t* snd_data = null;
    byte ret = E_OK;
    if (MAX_SND_BLOCK_NUM <= serial_snd_size) {
        return ES_OVERCACHE;
    }
    if (len == 0) {
        //cstr
        size = strlen(sdata);
    } else {
        //byte array
        size = len;
    }

    if (size > MAX_SERIAL_RECV_NUM - 1) { //for '\n'
        size = MAX_SERIAL_RECV_NUM - 2; //error , and append '@'
        ret = ES_OVERMAX;
        //return ES_OVERMAX;
    }
    snddata = (byte*) malloc(MAX_SERIAL_RECV_NUM);
    memset(snddata, 0, MAX_SERIAL_RECV_NUM);
    if (len != 0) {
        byte i;
        for (i = 0; i < size; i++) {
            snddata[i] = sdata[i];
        }
    } else {
        strcpy(snddata, sdata);
    }
    if (ret == ES_OVERMAX) {
        *(snddata + size++) = '@';
    }
    snd_data = (serial_data_t*) malloc(sizeof(serial_data_t));
    snd_data->sdata = snddata;
    snd_data->len = size;
    serial_snd_size++;
    if ((ret = sendMessage(MSG_UART_SEND, snd_data)) != E_OK) {
        free(snddata);
        free(snd_data);
    }
    return ret;
}

/*
 * data format
 * MAGIC(0xEF) TOKEN(0xFF) LEN(0xFF) CONTENT
 */
static void uartInterruptHandler(void)
interrupt 4
{
    byte ret;
    ES = 0;
    if (RI == 1) {
        RI = 0;
        if (serial_rctx.state == (1 << STATE_MAGIC) && SBUF == CMD_SERIAL_MAGIC) {
            serial_rctx.state = serial_rctx.state << 1;
        } else if (serial_rctx.state == (1 << STATE_TOKEN)) {
            serial_rctx.state = serial_rctx.state << 1;
            serial_rctx.token = SBUF;
        } else if (serial_rctx.state == (1 << STATE_LENGTH)) {
            serial_rctx.state = serial_rctx.state << 1;
            serial_rctx.len = SBUF;
            if (serial_rctx.len > (MAX_SERIAL_RECV_NUM - 1)) {
                ret = SERIAL_ERROR_OVERLOAD;
                goto SERIAL_ERROR;
            }

        } else if (serial_rctx.state == (1 << STATE_COTENT)) {
            if (serial_rctx.idx >= serial_rctx.len) {
                ret = SERIAL_ERROR_OVERLOAD;
                goto SERIAL_ERROR;
            }
            serial_rctx.datablock->sdata[serial_rctx.idx] = SBUF;
            if (++serial_rctx.idx == serial_rctx.len) {
                // received done!
                serial_rctx.state = serial_rctx.state << 1;
                serial_rctx.datablock->len = serial_rctx.len;
                serial_rctx.datablock->recvToken = serial_rctx.token;
                if(sendMessage(MSG_UART_RECV,
                                (void*) serial_rctx.datablock) != E_OK) {
                    restoreReceive();
                    return;
                }
                REN = 0;
                return;
            }
        } else {
            /*if(serial_rctx.state == (1 << STATE_MAGIC)) {
             ES = 1;
             return ;
             }*/
            ret = SERIAL_ERROR_RETRY;
            goto SERIAL_ERROR;
        }
    }

    ES = 1;
    return;

    SERIAL_ERROR:
    REN = 0;
    serial_rctx.datablock->error = ret;
    sendMessage(MSG_UART_IO_ERROR, (void*) &serial_rctx);
}
