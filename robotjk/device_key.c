#include "system.h"

#define ShortInterval			3		/*�̰�������� 150ms*/
#define LongInterval			20		/*�����������  1s */
#define JitterInterval	        2		/*���󶯰������ 100ms*/

static byte ScanValueSave = 0xFF;
static byte ScandCounter = 0;
static byte JitterCounter = 0;

byte getMatrixKey() {
	return ScanValueSave;
}

/*****************************************************************************
 * ������	: ScandPin
 * ����	    : P1��4*4����ɨ��
 * �������  : ��
 * ���ز���  : byte���Ͱ���ɨ��ֵ
 *****************************************************************************/
static byte scandPin(void) {
	byte i, j, temp, Buffer[4] = { 0xef, 0xdf, 0xbf, 0x7f };
	for (j = 0; j < 4; j++) {
		P1 = Buffer[j];
		delayUs(5);
		temp = 0x01;
		for (i = 0; i < 4; i++) {
			if (!(P1 & temp)) {
				return (i + j * 4); //����ȡ�õİ���ֵ
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
		delay(15);	//��������
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

