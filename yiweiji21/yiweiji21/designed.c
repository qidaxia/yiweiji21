
#include "designed.h"
#include "soft.h"
#include "hooks.h"
#include "usart.h"
#include "eeprom.h"
#include "TLC5615.h"
#include <stdio.h>

u8 echoBuf[128] = { 0 };


extern void setRunSpeed(u16 speed);

/**
 *
 */
u8 savePowerFlag = 0;


/**
 *
 */
u16 powerTimeCNT = 0;


/**
 *
 */
volatile SpeedLimit speed;

/**
 *
 */
volatile ENERGY energy;


u8 ack[] = { 'A','C' ,'K','\r','\n' };
u8 ackLenError[] = { 'L','E','N','_', 'E','R','R','\r','\n' };
u8 ackCheckError[] = { 'C','H','K','_', 'E','R','R','\r','\n' };
u8 ackIdle[] = { 'I','D','L','E','\r','\n' };
u8 ackOk[] = { '_', 'O', 'K','\r','\n' };

u8 ackLimitA[] = { 'L','I','M','_','A','\r','\n' };
u8 ackLimitB[] = { 'L','I','M','_','B','\r','\n' };

u8 ackCharging[] = { 'C','H','A','R','G','\r','\n' };
u8 ackUlink[] = { 'U','L','L','I','N','K','\r','\n' };




extern void toUp(void)
{
	GO_UP;
	EN_T;
	delay_ms(speed.BrakeReleaseDelay);
	DIS_BRK_T;

}

extern void toDown(void)
{
	GO_DOWN;
	EN_T;
	delay_ms(speed.BrakeReleaseDelay);
	DIS_BRK_T;

}

extern void toForward(void)
{
	GO_FORWARD; delay_ms(speed.BrakeReleaseDelay);
	DIS_BRK_W;
}

extern void toBack(void)
{
	GO_BACK; delay_ms(speed.BrakeReleaseDelay);
	DIS_BRK_W;
}

extern void toStopT(void)
{
	speedToMin(LIFT);
	DIS_T; //delay_ms(5);
	EN_BRK_T;
}

extern void toStopW(void)
{
	speedToMin(WALK);
	EN_BRK_W;
}

extern void toStop(void)
{
	toStopT();
	toStopW();
}


extern void handlerLiftForKey(void(*dirFun)(void), u8(*limitFun)(void), u8(*requestFun)(void))
{
	u16 timeout = 0;
	speedToMin(LIFT);
	if (savePowerFlag || !requestFun())
	{
		onceBeep();
		return;
	}
	if (limitFun())
	{
		sendCMD(ackLimitA, 5);
		return;
	}
	//
	dirFun();
	soft(SPEED_UP, LIFT, limitFun, requestFun);
	//speedToMax(LIFT);
	if (limitFun())
	{
		toStopT();
		sendMsg(ackLimitA);
		goto END;
	}
	delay_ms(5);

	while (TRUE)
	{
		_WDR();
		timeout++;
		delay_ms(10);
		if (timeout >= 400)
		{
			timeout = 400;
		}
		if (!requestFun())
		{
			delay_ms(20);
			if (!requestFun())
			{
				break;
			}
		}
		setRunSpeed(speed.MaxSpeed_lift);
		if (limitFun())
		{
			delay_ms(10);
			if (limitFun())
			{
				toStopT();
				sendMsg(ackLimitA);
				goto END;
			}
		}
	}
	if (timeout >= 400)
	{
		soft(SPEED_DOWN, LIFT, limitFun, requestFun);
	}
END:

	toStopT();
	delay_ms(600);
	return;
}

extern void handlerWalkForKey(void(*dirFun)(void), u8(*limitFun)(void), u8(*requestFun)(void))
{
	u16 timeout = 0;
	speedToMin(WALK);
	delay_ms(10);
	if (savePowerFlag || !requestFun())
	{
		onceBeep();
		return;
	}
	if (limitFun())
	{
		sendMsg(ackLimitB);
		return;
	}

	dirFun();
	soft(SPEED_UP, WALK, limitFun, requestFun);

	if (limitFun())
	{
		toStopW();
		sendMsg(ackLimitB);
		goto END;
	}

	while (TRUE)
	{
		_WDR();
		timeout++;
		delay_ms(10);
		if (timeout >= 400)
		{
			timeout = 400;
		}
		if (!requestFun())//break
		{
			delay_ms(10);
			if (!requestFun())
			{
				break;
			}
		}
		setRunSpeed(speed.MaxSpeed_walk);
		if (limitFun())
		{
			toStopW();
			sendMsg(ackLimitB);
			goto END;
		}
	}
	if (timeout >= 400)
	{
		soft(SPEED_DOWN, WALK, limitFun, requestFun);
	}
END:
	//
	toStopW();
	delay_ms(600);
	return;

}

extern void setRunSpeed(u16 speed)
{
	SPI_MasterTransmit(speed);
}

extern void handlerLiftForCmd(CMD dir, u8(*limitFun)(void))
{
	CMD cmd = FORWARD;
	u32 params = 0;
	u16 timeout = 0;
	speedToMin(LIFT);
	delay_ms(10);
	if (savePowerFlag)
	{
		onceBeep();
		return;
	}
	if (limitFun())
	{
		sendMsg(ackLimitA);
		return;
	}
	//
	if (dir == DOWN)
	{
		toDown();
	}
	else
	{
		toUp();
	}
	soft(SPEED_UP, LIFT, limitFun, idleHooks);
	if (limitFun())
	{
		toStopT();
		sendMsg(ackLimitA);
		goto END;
	}
	delay_ms(5);

	while (TRUE)
	{
		if (checkFrame(&params) == STOP)
		{
			goto END;
		}
		setRunSpeed(speed.MaxSpeed_lift);
		if (limitFun())
		{
			toStopT();
			sendMsg(ackLimitA);
			goto END;
		}
	}

END:
	//stop
	toStopT();
	delay_ms(600);
	return;
}


extern void handlerWalkForCmd(CMD dir, u8(*limitfun)(void))
{
	CMD cmd = UP;
	u16	timeout = 0;
	u32 params = 0;
	speedToMin(WALK);
	if (savePowerFlag || limitfun())
	{
		onceBeep();
		return;
	}
	if (limitfun())
	{
		sendMsg(ackLimitB);
		return;
	}
	if (dir == FORWARD)
	{
		toForward();
	}
	else
	{
		toBack();
	}
	soft(SPEED_UP, WALK, limitfun, idleHooks);
	if (limitfun())
	{
		toStopW();
		sendMsg(ackLimitB);
		goto END;
	}

	while (TRUE)
	{
		if (checkFrame(&params) == STOP)
		{
			goto END;
		}
		setRunSpeed(speed.MaxSpeed_walk);
		if (limitfun())
		{
			toStopW();
			sendMsg(ackLimitB);
			goto END;
		}
	}

END:
	//stop 
	toStopW();
	delay_ms(600);
	return;
}


extern void handlerPower(u8 flag)
{
	if (savePowerFlag == 1)//
	{
		POWER_OPEN;
		savePowerFlag = 0;
	}
	else
	{
		POWER_CLOS;
		savePowerFlag = 1;
	}
	onceBeep();
	if (flag)
	{
		while (IsPowerRequest())
		{
			_WDR();
		}
	}
	delay_ms(100);
}




extern void onceBeep(void)
{
	BEEP_ON;
	delay_ms(50);
	BEEP_OFF;
}



extern void ioInit(void)
{
	//---
	EN_BRK_T;
	EN_BRK_W;

	//---Vin
	DDRA &= ~BIT(0);
	PORTA &= ~BIT(0);

	DDRA |= 0xfe;
	EN_BRK_T;
	EN_BRK_W;
	POWER_OPEN;
	DIS_T;

	//---------------
	DDRC = 0x00;
	PORTC = 0xff;//20K鈥斺�?0K
	DDRC |= BIT(IO_LED);

	DDRD |= BIT(IO_BEEP);
	DDRD |= BIT(IO_LED_CHAGRGE);
	LED_CHARGE_OFF;
	BEEP_OFF;
	DDRD &= ~BIT(IO_LIMIT_UP);
	DDRD &= ~BIT(IO_LIMIT_DOWN);
	DDRD &= ~BIT(IO_INPLACE);
	DDRD &= ~BIT(IO_CHARGE);
	PORTD |= BIT(IO_LIMIT_DOWN);
	PORTD |= BIT(IO_LIMIT_UP);
	PORTD |= BIT(IO_INPLACE);
	PORTD |= BIT(IO_CHARGE);
	LED_OFF;
}


extern void keyDown(void)
{
	LED_ON;
	if (savePowerFlag == 0)
	{
		powerTimeCNT = 0;
	}
	_WDR();
}

extern void keyOff(void)
{
	LED_OFF;
	delay_ms(200);
}

extern void watchDog_init(void)
{

	_WDR();
	WDTCR = 0X1F;
	WDTCR = 0X0F;
	return;
}

/*
命令头	命令码	参数						命令尾
start	0xff	0x00,0x00,0x00,0x00		end

命令码：
错误		0xff
前进		0x01
后退		0x02
上升		0x03
下降		0x04
停止		0x05(全部停止)
*/


//返回命令码和参数(低字节在前)
//0xff,无数据或者错误
extern u8 checkFrame(u32 * params)
{
	u8 *cmdPtr = (void *)0;
	u8 cmdLen = 0;
	u8 cmdCode = 0xff;

	_WDR();
	*params = 0;
	cmdPtr = getFrameData(&cmdLen);
	if (cmdPtr == (void *)0)
	{
		return 0xff;
	}
	else if (cmdLen != 5)	//命令码（1）+参数（4）
	{
		sendMsg(ackLenError);
		return 0xff;
	}
	else
	{
		cmdCode = cmdPtr[0];
		*params = *params + cmdPtr[4] << 24;
		*params = *params + cmdPtr[3] << 16;
		*params = *params + cmdPtr[2] << 8;
		*params = *params + cmdPtr[1];
		
	}

	return cmdCode;
}


extern void echo(void)
{

}


extern void saveParameterToEeprom(void)
{
	u16 addr = 0x00;
	u8 *base = (u8 *)(&energy.Current_energy);
	u8 i = 0;
	for (i = 0; i < 3 * 2; i++)
	{
		eepromWrite(addr++, *(base + i));
	}
	base = (u8 *)(&speed.MaxSpeed_walk);
	for (i = 0; i < 9 * 2; i++)
	{
		eepromWrite(addr++, *(base + i));
	}
	return;
}

extern void readParameterAtEeprom(void)
{
	u16 addr = 0x00;
	u8 *base = (u8 *)(&energy.Current_energy);
	u8 i = 0;
	for (i = 0; i < 3 * 2; i++)
	{
		*(base + i) = eepromRead(addr++);
	}
	base = (u8 *)(&speed.MaxSpeed_walk);
	for (i = 0; i < 9 * 2; i++)
	{
		*(base + i) = eepromRead(addr++);
	}
}

u8 idleHooks(void)
{
	return TRUE;
}

extern void msg(void)
{
	sprintf(echoBuf, "speed.MaxSpeed_walk:%d\r\n",speed.MaxSpeed_walk);
	sendMsg(echoBuf);
	sprintf(echoBuf, "speed.MaxSpeed_lift:%d\r\n",speed.MaxSpeed_lift);
	sendMsg(echoBuf);
	sprintf(echoBuf, "speed.MinSpeed_walk:%d\r\n",speed.MinSpeed_walk);
	sendMsg(echoBuf);
	sprintf(echoBuf, "speed.MinSpeed_lift:%d\r\n",speed.MinSpeed_lift);
	sendMsg(echoBuf);
	sprintf(echoBuf, "energy.Current_energy:%d\r\n",energy.Current_energy);
	sendMsg(echoBuf);
	sprintf(echoBuf, "energy.Threshole_bottom:%d\r\n",energy.Threshole_bottom);
	sendMsg(echoBuf);
	sprintf(echoBuf, "energy.Threshole_top:%d\r\n",energy.Threshole_top);
	sendMsg(echoBuf);
	return;
}
