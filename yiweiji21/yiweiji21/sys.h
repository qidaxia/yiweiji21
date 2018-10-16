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
	STOP = 0,		///< ֹͣ
	UP,				///< ����
	DOWN  ,			///< �½�
	FORWARD ,		///< ǰ��
	BACK ,			///< ����
	POWER ,			///< ��Դ

	SET_MAX_SPEED_WALK,	//������������ٶ�
	SET_MIN_SPEED_WALK,	//������С�����ٶ�
	SET_MAX_SPEED_LIFT,	//������������ٶ�
	SET_MIN_SPEED_LIFT,	//������С�����ٶ�

	RESTART,			//��λ
	READ_CFG,			//��ȡ��ǰ����

	SET_CHARG_TOP,		//���ó������
	SET_CHARG_BOTTOM,	//���ó������

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