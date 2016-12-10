/*
 * device_rtc.c
 *
 *  Created on: 2016Äê12ÔÂ10ÈÕ
 *      Author: Kiah
 */

#include "data_type.h"
#include "device.h"
#include "system_queue.h"
#include <stdlib.h>
#include "stc516rd.h"

static rtc_t idata sRtc;

#ifdef __RTC_SUPPORT__
rtc_t* getRtc() {
    return &sRtc;
}

bool isRtcTimeout(rtc_t* check) {
    if (check->rtc_h >= sRtc.rtc_h) {
        return true;
    }
    if (check->rtc_l >= sRtc.rtc_l) {
        return true;
    } else {
        return false;
    }
}

rtc_t* getDelayRtc(ulong high, ulong low) {
    rtc_t* rtcp = (rtc_t*) malloc(sizeof(rtc_t));
    rtcp->rtc_h = sRtc.rtc_h + high;
    rtcp->rtc_l = sRtc.rtc_l + low;
    return rtcp;
}

void handleRtcService() {
    if (((sRtc.rtc_l)++) == 0xFFFFFFFF) {
        sRtc.rtc_l = 0;
        sRtc.rtc_h++;
    }
    sendMessage(MSG_TIMER, null);
}
#endif
