
#ifndef _TLC5615_H
#define _TLC5615_H
#include "sys.h"


#define START_DELAY		2 //2
#define STOP_DELAY		2 //2


#define SPI_SCK			PB7		
#define SPI_MISO		PB6		
#define SPI_MOSI		PB5		
#define SPI_SS			PB4		

#define TLC5615_CS		PB4							
#define TLC5615_ENABLE	PORTB &=~BIT(TLC5615_CS)	
#define TLC5615_DISABLE	PORTB |=BIT(TLC5615_CS)		


extern void SPI_MasterInit(void);


extern void SPI_MasterTransmit(u16 iData);


#endif