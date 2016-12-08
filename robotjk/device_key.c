#include "system.h"

#define ShortInterval			3		/*短按按键间隔 150ms*/
#define LongInterval			20		/*长按按键间隔  1s */
#define JitterInterval	        2		/*防误动按键间隔 100ms*/

static byte ScanValueSave = 0xFF;
static byte ScandCounter = 0;
static byte JitterCounter = 0;

byte getMatrixKey() {
	return ScanValueSave;
}

/*****************************************************************************
 * 函数名	: ScandPin
 * 描述	    : P1口4*4按键扫描
 * 输入参数  : 无
 * 返回参数  : byte类型按键扫描值
 *****************************************************************************/
static byte scandPin(void) {
	byte i, j, temp, Buffer[4] = { 0xef, 0xdf, 0xbf, 0x7f };
	for (j = 0; j < 4; j++) {
		P1 = Buffer[j];
		delayUs(5);
		temp = 0x01;
		for (i = 0; i < 4; i++) {
			if (!(P1 & temp)) {
				return (i + j * 4); //返回取得的按键值
			}
			temp <<= 1;
		}
	}
	return INVALID;
}

void keySystemTickService(void) {
	byte scanValue;
	byte rp1;
	if (JitterCounter != 0) {
		JitterCounter--;
		return;
	}
	EnterCritical();
	rp1 = P1;
	P1 = 0xf0;
	if (P1 != 0xf0) {
		delay(15);	//按键消抖
		scanValue = scandPin();
		ScanValueSave = scanValue;
		display((KeyMessageType << 8) + ScanValueSave);
		JitterCounter = JitterInterval;
	}
	P1 = rp1;
	exitCritical();

//   if (scanValue == INVALID)
//	{
//        if(JitterCounter)
//        {
//            JitterCounter--;
//            ScandCounter = 0;
//            ScanValueSave = INVALID;
//            return;
//        }
//
//        if(ScanValueSave == INVALID)
//        {
//            ScandCounter = 0;
//            return;
//        }
//
//        if (ScandCounter == LongInterval)
//        {
//            SendMessage(KeyMessageType, ScanValueSave);
//        }
//        else if (ScandCounter > ShortInterval)
//        {
//            SendMessage(KeyMessageType, ScanValueSave);
//        }
//        ScandCounter = 0;
//        ScanValueSave = INVALID;
//        JitterCounter = JitterInterval;
//	}
//    else
//    {
//        ScandCounter++;
//
//        if (ScandCounter == 1)
//        {
//            ScanValueSave = scanValue;
//        }
//        else if (ScandCounter == 2)
//        {
//            if (scanValue != ScanValueSave)
//            {
//                ScanValueSave = INVALID;
//            }
//        }
//        else if (ScandCounter >= LongInterval)
//        {
//            ScandCounter = LongInterval;
//        }
//    }
}

