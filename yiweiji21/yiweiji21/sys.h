#ifndef _SYS_H__
#define _SYS_H__
#include <iom16v.h>
#include <macros.h>

typedef unsigned char u8;

typedef unsigned int u16;

typedef  unsigned long  u32;

typedef enum { OK = 0, ERROR = !(OK) }Status;
typedef enum { NO = 0, YES = !(NO) }Whether;

//Enumeration definition of control instructions.


typedef enum
{
	STOP = 0,		///< 停止
	UP,				///< 上升
	DOWN  ,			///< 下降
	FORWARD ,		///< 前进
	BACK ,			///< 后退
	POWER ,			///< 电源

	SET_MAX_SPEED_WALK,	//设置最大行走速度
	SET_MIN_SPEED_WALK,	//设置最小行走速度
	SET_MAX_SPEED_LIFT,	//设置最大提升速度
	SET_MIN_SPEED_LIFT,	//设置最小提升速度

	RESTART,			//复位
	READ_CFG,			//读取当前参数

	SET_CHARG_TOP,		//设置充电上限
	SET_CHARG_BOTTOM,	//设置充电下限

}CMD;


typedef struct 
{
	u16 Current_energy;		///< 
	u16 Threshole_top;		///< 
	u16 Threshole_bottom;	///< 
}ENERGY;


typedef struct  
{
	u16 MaxSpeed_walk;			///< 
	u16 MaxSpeed_lift;			///< 
	u16 MinSpeed_walk;			///< 
	u16 MinSpeed_lift;			///< 

	u16 SpeedUpDelay_walk;		///< 
	u16 SpeedDownDelay_walk;	///< 
	u16 SpeedUpDelay_lift;		///< 
	u16 SpeedDownDelay_lift;	///< 
	u16 BrakeReleaseDelay;		///< 
}SpeedLimit;



typedef enum
{
	SPEED_UP = 0,		///< 
	SPEED_DOWN			///<
}SpeedType;

typedef enum
{
	LIFT = 0,			///< 
	WALK				///< 
}DIRECT;



#define  mclk   8000000

#define	TRUE		1
#define FALSE		!TRUE



void delay_ms(u16 ms);
void delay_us(u16 us);


#endif