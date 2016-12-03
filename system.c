#include "system.h"

#define MessageBufferSum 32
struct MessageQueue { /* 定义一个队列*/
	byte In; /* 插入一个消息  */
	byte Out; /* 取出一个消息  */
	byte Entries; /* 消息长度      */
};

static struct MessageQueue idata MessageQueue;
static ushort idata MessageBuffer[MessageBufferSum];

static bool EaSave;
static byte CriticalNesting = 0;
byte code HextoAscii[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F' };

void Delay(uint delayClock) {
	byte x,j;
	for(j=0;j<delayClock;j++)
	for(x=0;x<=148;x++);
}

void DelayUs(uint delayTime) {
	while (delayTime--)
		;
}

void DelayMs(uint delayTime) {
	while (delayTime--) {
		DelayUs(1000);
	}
}

void Delay10Ms(uint delayTime) {
	uint i, j;
	for (i = 0; i < delayTime; i++)
		for (j = 0; j < 1827; j++)
			//这个是通过软件仿真得出的数
			;
}

byte HexToAscii(byte hex) {
	Assert(hex < 16);

	return HextoAscii[hex];
}

void EnterCritical(void) {
	if (CriticalNesting == 0) {
		EaSave = EA;
		EA = 0;
	}
	CriticalNesting++;
}

void ExitCritical(void) {
	if (CriticalNesting > 0) {
		CriticalNesting--;
		if (CriticalNesting == 0) {
			EA = EaSave;
		}
	}
}

void InitializeMessageQueue(void) {
	MessageQueue.In = 0;
	MessageQueue.Out = 0;
	MessageQueue.Entries = 0;
}

/*****************************************************************************
 * 函数名	: SendMessage
 * 描述	    : 发送消息
 * 输入参数  : message: 消息类型,
 *             value:消息值，MessageTimer类型为16bit地址，其他类型下都是8bit数据
 * 返回参数  : 无
 *****************************************************************************/
void SendMessage(MessageType messageType, ushort value) {
	ushort* input = null;
	EnterCritical();
	if (MessageQueue.Entries >= MessageBufferSum) {
		//full
		ExitCritical();
		return;
	}

	input = (ushort*) (MessageBuffer + MessageQueue.In);
	if (messageType == TimerMessageType) {
		*input = value;
	} else {
		((byte *) input)[0] = messageType;
		((byte *) input)[1] = value & 0xFF;
	}

	MessageQueue.In = (MessageQueue.In + 1 + MessageQueue.Out)
			% MessageBufferSum;
	MessageQueue.Entries++;
	ExitCritical();
	return;
}

ushort PendMessageQueue(void) {
	ushort * out;

	while (MessageQueue.Entries == 0)
		;
	EnterCritical();
	out = (ushort*) (MessageBuffer + MessageQueue.Out);
	MessageQueue.Out = (MessageQueue.Out + 1) % MessageBufferSum;
	MessageQueue.Entries--;

	ExitCritical();
	return (*out);
}

