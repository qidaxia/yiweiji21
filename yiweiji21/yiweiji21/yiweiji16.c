#include "designed.h"
#include "hooks.h"
#include "soft.h"
#include "powerManager.h"
#include "usart.h"
#include "TLC5615.h"
#include "ad.h"





int main(void)
{
	CMD cmd = STOP;
	u32 params = 0x00;
	u8 i = 0;
	u16 adcVal = 0;
	u16 chargeCount = 0;	//
	u16 frequencyCount = 0; //
	delay_ms(500);			//Ensure power supply
	ioInit();
	usartInit(19200);
	SPI_MasterInit();
	powerManageCFG();
	watchDog_init();
	adc_init();

	speed.MaxSpeed_lift = 1022;
	speed.MaxSpeed_walk = 1024;
	speed.MinSpeed_lift = 100;
	speed.MinSpeed_walk = 400;
	speed.SpeedDownDelay_lift = 0;
	speed.SpeedDownDelay_walk = 0;
	speed.SpeedUpDelay_lift = 1;
	speed.SpeedUpDelay_walk = 1;
	speed.BrakeReleaseDelay = 0;

	energy.Current_energy = 0;
	energy.Threshole_bottom = 856; //59%
	energy.Threshole_top = 901;	//97%

	//
	delay_ms(3000);
	for (i = 0; i < 20; i++)
	{
		get_adc();
		delay_ms(20);
	}
	onceBeep();

	while (TRUE)
	{
		/*!
		* @brief:
		*/
		if (IsKeyPress())
		{
			_WDR();
			keyDown();
			switch (keyScan())
			{
			case UP:
				handlerLiftForKey(toUp, IsLimitUp, IsUpRequest);
				break;
			case DOWN:
				handlerLiftForKey(toDown, IsLimitDown, IsDownRequest);
				break;
			case BACK:
				handlerWalkForKey(toBack, IsCharge, IsBackRequest);
				break;
			case FORWARD:
				handlerWalkForKey(toForward, IsInPlace, IsForwardRequest);
				break;
			case POWER:
				handlerPower(1);
				break;
			default:
				break;
			}
			keyOff();
		}
		/*!
		* @brief:
		*/
		cmd = checkFrame(&params);
		switch (cmd)
		{
			_WDR();
			keyDown();
			sendCMD(ack, 3);
		case UP:
			handlerLiftForCmd(UP, IsLimitUp);
			break;
		case DOWN:
			handlerLiftForCmd(DOWN, IsLimitDown);
			break;
		case FORWARD:
			handlerWalkForCmd(FORWARD, IsInPlace);
			break;
		case BACK:
			handlerWalkForCmd(BACK, IsCharge);
			break;
		case POWER:
			handlerPower(0);
			break;

		case SET_MAX_SPEED_LIFT:
			speed.MaxSpeed_lift = params;
			break;
		case SET_MAX_SPEED_WALK:
			speed.MaxSpeed_walk = params;
			break;
		case SET_MIN_SPEED_LIFT:
			speed.MinSpeed_lift = params;
			break;
		case SET_MIN_SPEED_WALK:
			speed.MinSpeed_walk = params;
			break;
		case SET_CHARG_BOTTOM:
			energy.Threshole_bottom = params;
			break;
		case SET_CHARG_TOP:
			energy.Threshole_top = params;
			break;

		case READ_CFG:
			energy.Current_energy = get_adc();
			msg();
			break;
		case RESTART:
			while (1);
			break;

		default:
			toStop();
			break;
		}

		keyOff();
		if (IsCharge())
		{
#if EN_CHARGE_MANAGE == 1
			if (IsContact())
			{
				LED_ON;
			}
			else
			{
				LED_OFF;
				//goto FINISHED;
			}

			if (chargeCount++ >= 30000)
			{
				chargeCount = 0;

				adcVal = get_adc();
				if (adcVal < energy.Threshole_bottom)
				{
					LED_CHARGE_ON;
				}
				else if (adcVal >= energy.Threshole_top)
				{
					frequencyCount++;
					if (frequencyCount >= 6)
					{
						frequencyCount = 0;
						LED_CHARGE_OFF;
					}
				}
				else if (adcVal < energy.Threshole_top)
				{
					if (frequencyCount >= 1)
					{
						frequencyCount = 0;
					}
				}
			}
#else
			LED_CHARGE_ON;
#endif
		}
		else
		{
			LED_CHARGE_OFF;
			LED_OFF;
		}
	FINISHED:
		_WDR();
		toStop();
	}
	return 0;
}
