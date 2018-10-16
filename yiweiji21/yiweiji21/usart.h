#ifndef _USART_H__
#define _USART_H__


#include "sys.h"

#define CMDLEN_MAX		20	
#define CMDLEN_MIN		12	
#define cmdSTART_LEN	5	
#define cmdSTOP_LEN		3	


extern const u8 cmdStart[cmdSTART_LEN];	
extern const u8 cmdStop[cmdSTOP_LEN];	
 

extern void usartInit(u16 baud);

extern void uartSendB(u8 dat);

extern void sendCMD(u8 *buf, u8 len);


u8 * getFrameData(u8 *datLen);

extern void sendMsg(u8 *buf);

#endif // !_USART_H

