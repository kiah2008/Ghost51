#include "system.h"

struct Timer {
	ushort Delay;
	function CallBackFuction;
};

#define TimerSum		0x4

static byte idata State = 0;
static TimerhandleMode idata Mode;
static struct Timer idata Block[TimerSum];

void TimerSystemTickService(void) {
	SendMessage(SystemTickMessageType, 0);
}

void HandleTimerSystem(void) {
	byte i = 0;
	byte stateCopy;

	if (State == 0x00) {
		return;
	}

	EnterCritical();

	stateCopy = State;
	while (stateCopy) {
		if ((stateCopy & 0x01) == 1) {
			if ((--Block[i].Delay) == 0) {
				(*(Block[i].CallBackFuction))();
				ResetBit(State, i);
			}
		}

		stateCopy = stateCopy >> 1;
		i++;
	}
	ExitCritical();
}

/**
 * delay, ms
 */
bool TimerStart(TimerhandleMode handleMode, ushort delay,
		function callBackFunction) {
	byte i;

	EnterCritical();

	for (i = 0; i < TimerSum; i++) {
		if (!GetBit(State, i)) {
			Block[i].Delay = delay / 50;/*rtc will interrupt per 50ms*/
			Block[i].CallBackFuction = callBackFunction;/*»Øµ÷º¯Êý*/
			if (handleMode) {
				SetBit(Mode, i);
			} else {
				ResetBit(Mode, i);
			}

			SetBit(State, i);
			ExitCritical();
			return (i);
		}
	}
	ExitCritical();
	return (INVALID);
}

void TimerStop(byte id) {
	Assert(id < TimerSum);

	ExitCritical();

	ResetBit(State, id);

	ExitCritical();
}

