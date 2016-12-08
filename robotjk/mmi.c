
#include "system.h"
#include "app.h"

void (*MmiFunction)(Key key);

static void InitializeIdleForm(void);
static void IdleFormProcess(Key key);

void Chuangyi(void) {
}

void InitializeMmi(void) {
	InitializeIdleForm();
	MmiFunction = IdleFormProcess;
}

void InitializeIdleForm(void) {
	printf("hello, world!");
}

void IdleFormProcess(Key key) {
	switch (key) {
	default:
		timerStart(TimerMessageHandle, 1000, Chuangyi);
		break;
	}
}

