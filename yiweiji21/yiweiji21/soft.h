#ifndef _SOFT_H_
#define _SOFT_H_
#include "sys.h"
#include "TLC5615.h"


extern void speedToMax(DIRECT dir);

extern void speedToMin(DIRECT dir);


extern void soft(SpeedType speedType, DIRECT dir, u8(*LimitA)(void), u8(*requestFun)(void));

#endif // !_SOFT_H_
