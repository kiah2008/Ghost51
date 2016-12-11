/*
 * request.h
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include "data_type.h"

typedef struct {
    byte magic;
    byte token;
    byte length;
    char* content;
} request_t;

//#define CMD_SERIAL_MAGIC 0xEF
#define CMD_SERIAL_MAGIC 'a'

#define SERIAL_ERROR_OVERLOAD 1
#define SERIAL_ERROR_RETRY 2
#define SERIAL_ERROR_REJECT 3

// one serial at one time
//#define SERIAL_CACHE_SIZE 1
//static char* idata seria_cache[SERIAL_CACHE_SIZE];
#define MAX_SERIAL_RECV_NUM 16
//#define MAX_SND_BLOCK_NUM 8

typedef struct _serial_data {
    byte* sdata;
    //todo: need check bit ?
    //byte checkbit;
    byte recvToken;
    byte error;
    byte len;
} serial_data_t;

typedef struct {
    byte token;
    byte len;
    byte idx;
    byte state;
    serial_data_t* datablock;
} serial_recv_ctx;

#endif /* REQUEST_H_ */
