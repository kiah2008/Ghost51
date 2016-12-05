
#include "system.h"


struct Rtc idata Rtc;

void RtcSystemTickService(void) 
{
    static byte idata Counter = 0;
    
    if(++Counter == 20)
    {
        Counter = 0;
        Rtc.Second++;
    }

    if(Rtc.Second == 60)
    {
        Rtc.Second = 0;
        Rtc.Minute++;
    }

    if(Rtc.Minute == 60)
    {
        Rtc.Minute = 0;
        Rtc.Hour++;
    }
    
    if(Rtc.Hour == 24)
    {
        Rtc.Hour = 0;
        Rtc.Day++;
    }	
}


void InitializeRtc(void) 
{
    Rtc.Second = 0;
    Rtc.Minute = 0;
    Rtc.Hour = 0;
    Rtc.Day = 0;
}
