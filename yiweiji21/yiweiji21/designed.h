#ifndef _DESIGNED_H
#define _DESIGNED_H
#include "sys.h"


#define DRIVE_OLD		1

#define EN_CHARGE_MANAGE 1


#define  IO_EN_M1	PA1
#define  IO_T_EN	PA2
#define  IO_T_FR	PA3
#define  IO_T_BRK	PA4
#define  IO_W_DIR	PA5
#define  IO_W_BRK	PA6
#define  IO_EN_M2	PA7
 
#define IO_BACK		PC0
#define IO_FORWARD	PC1
#define IO_SETTING	PC2
#define IO_POWER	PC3
#define IO_UP		PC4
#define IO_DOWN		PC5

 
#define ST1				PD2
#define ST2				PD3
#define ST3				PD4
#define ST4				PD5

#define IO_LIMIT_UP		ST1
#define IO_LIMIT_DOWN	ST2
#define IO_INPLACE		ST3
#define IO_CHARGE		ST4
 
#define IO_LED_CHAGRGE	PD6
#define IO_BEEP			PD7
#define IO_CONTACT		PC6
#define IO_LED			PC7
 

#define LED_ON			PORTC&=~BIT(IO_LED)
#define LED_OFF			PORTC|=BIT(IO_LED)
#define LED_CHARGE_ON	PORTD&=~BIT(IO_LED_CHAGRGE)
#define LED_CHARGE_OFF	PORTD|=BIT(IO_LED_CHAGRGE)
#define BEEP_ON			PORTD|=BIT(IO_BEEP)
#define BEEP_OFF		PORTD&=~BIT(IO_BEEP)

#if DRIVE_OLD==1
#define GO_UP			PORTA|=BIT(IO_T_FR)			
#define GO_DOWN			PORTA&=~BIT(IO_T_FR)		
#else
#define GO_UP			PORTA&=~BIT(IO_T_FR)		
#define GO_DOWN			PORTA|=BIT(IO_T_FR)			

#endif


#define EN_BRK_T		PORTA&=~BIT(IO_T_BRK)
#define DIS_BRK_T		PORTA|=BIT(IO_T_BRK)

#if DRIVE_OLD==1
#define DIS_T			PORTA&=~BIT(IO_T_EN)
#define EN_T			PORTA|=BIT(IO_T_EN)
#else
#define EN_T			PORTA&=~BIT(IO_T_EN)
#define DIS_T			PORTA|=BIT(IO_T_EN)
#endif


#define GO_FORWARD		PORTA&=~BIT(IO_W_DIR)
#define GO_BACK			PORTA|=BIT(IO_W_DIR)
#define EN_BRK_W		PORTA|=BIT(IO_W_BRK)
#define DIS_BRK_W		PORTA&=~BIT(IO_W_BRK)

#define POWER_OPEN		PORTA&=~BIT(IO_EN_M1)
#define POWER_CLOS		PORTA|=BIT(IO_EN_M1) 


extern u8 ack[10];					
extern u8 ackLenError[10];			
extern u8 ackCheckError[10];			
extern u8 ackIdle[10];				
extern u8 ackOk[10];					
extern u8 ackCharging[10];			
extern u8 ackUlink[10];	

extern u8 savePowerFlag;			
extern u16 powerTimeCNT;			
extern volatile SpeedLimit speed;	
extern volatile ENERGY energy;		


extern void toUp(void);			
extern void toDown(void);		
extern void toForward(void);	
extern void toBack(void);		
extern void toStopT(void);		
extern void toStopW(void);		
extern void toStop(void);		

extern void handlerLiftForKey(void(*dirFun)(void), u8(*limitFun)(void), u8(*requestFun)(void));
extern void handlerWalkForKey(void(*dirFun)(void), u8(*limitFun)(void), u8(*requestFun)(void));
extern void handlerLiftForCmd(CMD dir, u8(*limitFun)(void));
extern void handlerWalkForCmd(CMD dir, u8(*limitfun)(void));
extern void handlerPower(u8 flag);

extern void onceBeep(void);
extern void ioInit(void);
extern void keyDown(void);
extern void keyOff(void); 
extern void watchDog_init(void);
extern u8 checkFrame(u32 * params);
extern void echo(void);

extern void saveParameterToEeprom(void);
extern void readParameterAtEeprom(void);
u8 idleHooks(void);

extern void msg(void);

#endif // !_DESIGNED_H
