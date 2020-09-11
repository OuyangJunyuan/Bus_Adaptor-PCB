#ifndef __DECODE__
#define __DECODE__

#include "can_if.h"
#include "algorithm.h"

/* 定义各个电机的CAN ID号 */
#define CAN_CHASSIS_MOTOR1_ID 0x201
#define CAN_CHASSIS_MOTOR2_ID 0x202
#define CAN_CHASSIS_MOTOR3_ID 0x203
#define CAN_CHASSIS_MOTOR4_ID 0x204
#define CAN_CHASSIS_MOTOR5_ID 0x205
#define CAN_BUF_SIZE 4





void CanDataPreDecode(CanRxFrame_t *read_ptr);
void rosCommondDecoder(uint8_t *UserRxBufferHS ,int pack_idx);
void CanTransmit_motor(void);


extern PID_t pos;
extern canFrame_t tx_Frame1_4,tx_Frame5_8,tx_Frame6020;
extern canFrame_t rRosFrame[4], sRosFrame;

#endif

