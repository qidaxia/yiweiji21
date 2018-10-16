#ifndef _HOOKS_H
#define  _HOOKS_H
#include "sys.h"


extern u8 IsBackRequest(void);		
extern u8 IsForwardRequest(void);	
extern u8 IsUpRequest(void);		
extern u8 IsDownRequest(void);		
extern u8 IsSettingRequest(void);	
extern u8 IsPowerRequest(void);		
 


extern u8 IsContact(void);			


extern u8 IsLimitUp(void);		
extern u8 IsLimitDown(void);	
extern u8 IsCharge(void);		
extern u8 IsInPlace(void);		



extern u8 IsKeyPress(void);


CMD keyScan(void);

#endif // !_HOOKS_H
