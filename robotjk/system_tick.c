#include "system.h"

void InitializeSystemTick(void)
{
	ET0=0;
    TH0 = 0x4c;//50ms
    TL0 = 0x0;

    IPLT0 = 0;
}


void SystemTickInterruptHandler(void) interrupt 1
{
    ET0 = 0;
    TF0 = 0;
    TH0=0x4c;
    TL0=0;

    KeySystemTickService();
    RtcSystemTickService();
    SendMessage(SystemTickMessageType, 0);
    ET0 = 1;
}

