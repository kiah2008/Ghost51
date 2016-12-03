#include "system.h"
#include <stdlib.h>
#include <string.h>

char putchar(char c) {
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

void PutByte(byte byte) {
	ES = 0;
	SBUF = byte;
	while (!TI)
		;
	TI = 0;

	ES = 1;
}

void PutString(string string, byte sum) {
	byte i;
	if (sum) {
		i = sum;
		while (i--) {
			PutByte(*string++);
		}
	} else {
		while (*string) {
			if (*string == '\n') {
				PutByte('\r');
			}
			PutByte(*string++);
		}
	}
}

void InitializeTrace(void) {
	ES = 0;
	ET1 = 0;

	//SMOD=1,double
	SetBit(PCON, 7);
	TH1 = 0xFD; //19200
	TL1 = 0xFD;

	//MODE 1, and enable REN
	SCON = 0x50;
	RI = 0;
	IPLS = 1;
	ES = 1;
}

s_cache_t* s_rcache = null;
static void UartInterruptHandler(void)
interrupt 4
{
	ES = 0;
	if(RI == 1 ) {
		RI = 0;
		if(s_rcache == null) {
			s_rcache = (s_cache_t*)malloc(sizeof(s_cache_t));
			memset(s_rcache, 0, sizeof(s_cache_t));
		}
		if(s_rcache->size<S_CACHE_SIZE) {
			s_rcache->cache[s_rcache->size++] = SBUF;
			if(SBUF=='\n') {
				SendMessage(PherialMessageType, s_rcache->cache);
			}
			if(s_rcache->size==S_CACHE_SIZE) {
				s_rcache->cache[S_CACHE_SIZE] ='\0';
				SendMessage(PherialMessageType, s_rcache->cache);
			}
		}
	}
	ES=1;
}
