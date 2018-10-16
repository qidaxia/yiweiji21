#include "eeprom.h"

extern void eepromWrite(u16 address, u8 dat)
{
	while (EECR &BIT(EEWE));
	EEAR = address;
	EEDR = dat;
	EECR |= BIT(EEMWE);

	SREG &= ~BIT(7);
	EECR |= BIT(EEWE);
	SREG |= BIT(7);
	delay_ms(1);
	return;
}

extern u8 eepromRead(u16 address)
{
	while (EECR&BIT(EEWE));
	EEAR = address;
	EECR |= BIT(EERE);
	return EEDR;
}

