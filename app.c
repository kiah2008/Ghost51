#include "system.h"
#include "app.h"

void Initialize(void) {
	EA = 0;
	InitializeRtc();

	InitializeTrace();

	InitializeMessageQueue();

	InitializeSystemTick();
	IPLX0 = 1;
	IPLX1 = 1;
	IPLT0 = 1;
	EX0 = 1;
	EX1 = 1;
	TMOD = 0X21;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
	TR1 = 1;
}

byte code table[16] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f,
		0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71 };
byte code LED_W[8] = { 0, 1, 2, 3, 4, 5, 6, 7 }; //这里是需要往三八译码器送的数据

void display(ushort num) {
	int i = 0;
	byte rp0 = 0xF;
	byte rp1 = 0xF;
	byte bits[4];
	byte j = 0;
	bits[0] = (num >> 12) & 0x0F;
	bits[1] = (num >> 8) & 0x0F;
	bits[2] = (num >> 4) & 0x0F;
	bits[3] = num & 0x0F;
	EnterCritical();
	rp0 = P0;
	rp1 = P2;
	for (i = 0; i < 60; i++) {
		for (j = 0; j < 4; j++) {
			P0 = table[bits[j]]; //数码管段值
			P2 = LED_W[j]; //点亮某一位数码管
			Delay(5);
		}
	}
	ExitCritical();
	Delay(10);
}

sbit keyext0 = P3 ^ 4;	  //独立按键的T0
static ushort matraxkey = 0;
static byte freshdisplay = 0;

int main(void) {
	ushort messageData;
	ushort key;

	Initialize();
	keyext0 = 1;

	while (true) {
		messageData = PendMessageQueue();

		switch (GetMessageType(messageData)) {
		case SystemTickMessageType:
//			if (keyext0 == 0) {
//				Delay(5);              //软件消抖，试验为10ms
//				if (keyext0 == 0) {
//					freshdisplay = !freshdisplay;
//				}
//				display((SystemTickMessageType << 8) + freshdisplay);
//				keyext0 = 1;
//			}
			KeySystemTickService();
			HandleTimerSystem();
			goto display;
		case PherialMessageType:
			key = GetMessageData(messageData);
			display((PherialMessageType << 8) + key);
			break;
		default:
			break;
		}

		display: if (freshdisplay) {
			display((KeyMessageType << 8) + getMatrixKey());
		}
	}
}

static void ExternalInterruptHandler0(void)
interrupt 0 {
	EA=0;
	Delay(5);
	if(P3^2 == 0) {
		freshdisplay = !freshdisplay;
		display((DisplayMessageType<<8)+freshdisplay);
	}
	Delay(50);
	EA=1;
}

//
//static void ExternalInterruptHandler1(void)
//interrupt 2 {
//	EX1=0;
//	Delay(1);
//	if(P3^3 == 0) {
//		displayValue=0;
//		display((DisplayMessageType<<8) + displayValue);
//	}
//	Delay(3);
//	EX1=1;
//}
