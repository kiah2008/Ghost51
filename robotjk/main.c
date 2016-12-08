#include "system.h"

byte code table[16] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f,
		0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71 };
byte code LED_W[8] = { 0, 1, 2, 3, 4, 5, 6, 7 }; //bit selection

sbit keyext0 = P3 ^ 4;	  //INT0
static ushort matraxkey = 0;
static byte freshdisplay = 0;

extern void (*MmiFunction)(Key key);
extern void InitializeMmi(void);

void Initialize(void) {
	EA = 0;
	initializeRtc();
	InitializeTrace();
	initializeMessageQueue();
	initializeSystemTick();

	IPLX0 = 1;
	IPLX1 = 1;
	IPLT0 = 1;
	EX0 = 1;
	EX1 = 1;
	//timer 1 8bit reload, timer 0 mode 1 16bit
	TMOD = 0X21;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
	TR1 = 1;
}

void inline display(ushort num) {
#if 0
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
			P0 = table[bits[j]]; //LED Value
			P2 = LED_W[j]; //bit select
			delay(5);
		}
	}
	exitCritical();
	delay(10);
#endif
}

int main(void) {
	ushort messageData;
	ushort key;

	Initialize();

	while (true) {
		messageData = pendMessageQueue();

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
			keySystemTickService();
			handleTimerSystem();
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
	delay(5);
	if(P3^2 == 0) {
		freshdisplay = !freshdisplay;
		display((DisplayMessageType<<8)+freshdisplay);
	}
	delay(50);
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
