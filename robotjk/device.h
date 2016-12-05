
#ifndef __DEVICE_H
#define __DEVICE_H


/*按键根据自己定义来修改*/
typedef enum 
{
    AddKey                  = 0x00,
    AddLongKey              = 0x30,
    
    SubKey                  = 0x04,
    SubLongKey              = 0x34,

    AssistUpKey             = 0x01,
    AssistUpLongKey         = 0x31,

    AssistDownKey           = 0x05,
    AssistDownLongKey       = 0x35,
    
    SaveKey                 = 0x02,
    SaveLongKey             = 0x32,

    TextBoxFocusKey         = 0x06,
    FormFocusLongKey        = 0x36,
    
    StartKey                = 0x03,
    StartLongKey            = 0x33,
    
    StopKey                 = 0x07,
    StopLongKey             = 0x37
}Key;

typedef enum 
{
    TimerInterruptHandle    = 0,
    TimerMessageHandle      = 1	
}TimerhandleMode;	

struct Rtc
{
    byte Second;
    byte Minute;
    byte Hour;
    uint Day;
} ;

typedef enum
{
    SystemTick10000,
    SystemTick1000,
    SystemTick100
}SystemTick;


extern struct Rtc idata Rtc;

extern void KeySystemTickService(void);

extern void RtcSystemTickService(void);

extern void TimerSystemTickService(void);


extern void InitializeRtc(void);

extern void InitializeTrace(void);

extern void InitializeSystemTick(void);



#endif

