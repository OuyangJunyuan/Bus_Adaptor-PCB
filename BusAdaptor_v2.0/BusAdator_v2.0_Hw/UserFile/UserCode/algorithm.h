#ifndef __ALGORITHMH
#define __ALGORITHMH

#include "stdint.h"

#define PI 3.14159
#define NOW  0
#define LAST 1

/**
* @brief 電機工作状态枚举类型
* @param None
* @retval None
*/
typedef enum
{
	UNDEFINED=0,
	SPEED,
	POSITION

}mode_e;

/**
* @brief 電機種類枚举类型
* @param None
* @retval None
*/
typedef enum
{
	m3508=0,
	m2006,
	m6020
}type_e;

typedef struct{
	float KP;
	float KI;
	float KD;
	float fdb;
	float error[2];
	float ref;
	int outputMax;
	float cur_error;
	int16_t output;
}PID_t;
/**
* @brief 舵轮角度结构体

* @param
*   Raw data
*				转子角速度：			rotor_vol
*				转子编码器数值：	rotor_pos
*		Calc data
*				初始角度计数器：	msg_cnt
*				机械转子总位置：	rotor_angle
*				输出轴角度：			output_angle
*				输出轴角速度：		output_vol
*				转子总圈数：			rotor_round
*		Support data
*				角速度暂存数组：	DeltaPosbuf[30]
*				转子寻零初始角度：offsetPos
*				计算角速度累计标识：PosIndex
* @retval None
*/
typedef struct{
	mode_e mode;
	type_e type;
	uint32_t can_id;
	int getCAN;
	float reductionRatio;
	int32_t msg_cnt;
	int32_t rotor_pos[2];
	float 	rotor_angle[2],rotor_vol,rotor_round;
	float		output_angle, output_vol;
	int32_t offsetPos;
	float target;
	PID_t speed_pid;
	PID_t pos_pid;
}Motor_t;

typedef enum
{
	Writing,
	Moving,
	Wating
}WorkState_e;

typedef struct
{
	Motor_t Motor[8];
	double TargetAngle[2];
	double Temp_target[2];
	WorkState_e WorkState;
}Chassis_t;

void controllerInit(void);
void speedServoRun(Motor_t *motor);
void positionServoRun(Motor_t *motor);
void CanTransmit_motor(void);

extern Chassis_t Controller;

#endif
