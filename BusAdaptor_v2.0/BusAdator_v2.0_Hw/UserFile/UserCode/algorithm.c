#include "algorithm.h"
#include "can_if.h"



Chassis_t Controller = { 0 };




void controllerInit() {

	for (int i = 0; i < 8; i++) {
		// 默認初始化為3508且停止状态
		Controller.Motor[i].mode = UNDEFINED;
		Controller.Motor[i].type = m3508;
		Controller.Motor[i].can_id = 0x201 + i;
		Controller.Motor[i].speed_pid.KP = 15;
		Controller.Motor[i].speed_pid.KI = 1;
		Controller.Motor[i].speed_pid.KD = 1;
		Controller.Motor[i].speed_pid.outputMax = 10000;
		Controller.Motor[i].pos_pid.KP = 0.2f;
		Controller.Motor[i].pos_pid.KI = 0.0007f;
		Controller.Motor[i].pos_pid.KD = 0.001f;
		Controller.Motor[i].pos_pid.outputMax = 10000;
	}
	tx_Frame1_4.canHandle = hcan1;
	tx_Frame1_4.Header.DLC = 0x08;
	tx_Frame1_4.Header.StdId = 0x200;
	tx_Frame1_4.Header.IDE = CAN_ID_STD;
	tx_Frame1_4.Header.RTR = CAN_RTR_DATA;

	tx_Frame5_8.canHandle = hcan1;
	tx_Frame5_8.Header.DLC = 0x08;
	tx_Frame5_8.Header.StdId = 0x1FF;
	tx_Frame5_8.Header.IDE = CAN_ID_STD;
	tx_Frame5_8.Header.RTR = CAN_RTR_DATA;

	tx_Frame6020.canHandle = hcan1;
	tx_Frame6020.Header.DLC = 0x08;
	tx_Frame6020.Header.StdId = 0x1FF;
	tx_Frame6020.Header.IDE = CAN_ID_STD;
	tx_Frame6020.Header.RTR = CAN_RTR_DATA;
}

void PID_Calc(PID_t *pid) {
	pid->cur_error = pid->ref - pid->fdb;
	pid->output += pid->KP * (pid->cur_error - pid->error[1])
			+ pid->KI * pid->cur_error
			+ pid->KD * (pid->cur_error - 2 * pid->error[1] + pid->error[0]);
	pid->error[0] = pid->error[1];
	pid->error[1] = pid->ref - pid->fdb;
	/*设定输出上限*/
	if (pid->output > pid->outputMax)
		pid->output = pid->outputMax;
	if (pid->output < -pid->outputMax)
		pid->output = -pid->outputMax;
}
void speedServoRun(Motor_t *motor) {
	motor->speed_pid.ref = motor->target;
	PID_Calc(&motor->speed_pid);
}

void positionServoRun(Motor_t *motor) {
	motor->pos_pid.ref = (float) (motor->target * motor->reductionRatio * 8191
			/ 2 / PI);
	motor->pos_pid.fdb = (float) (motor->rotor_angle[NOW]);
	PID_Calc(&motor->pos_pid);
	motor->speed_pid.ref = motor->pos_pid.output;
	PID_Calc(&motor->speed_pid);
}


void CanTransmit_motor(void) {
	tx_Frame1_4.isValid = 0;
	tx_Frame5_8.isValid = 0;
	tx_Frame6020.isValid = 0;
	for (int i = 0; i < 4; i++) {
		if (Controller.Motor[i].type != m6020) {
			if (Controller.Motor[i].speed_pid.output != 0) {
				tx_Frame1_4.isValid = 1;
				tx_Frame1_4.Data[2 * i] =
						(uint8_t) (Controller.Motor[i].speed_pid.output >> 8);
				tx_Frame1_4.Data[2 * i + 1] =
						(uint8_t) (Controller.Motor[i].speed_pid.output);
			}

		} else {
			tx_Frame6020.isValid = 1;
			tx_Frame6020.Data[2 * i] =
					(uint8_t) (Controller.Motor[i].speed_pid.output >> 8);
			tx_Frame6020.Data[2 * i + 1] =
					(uint8_t) (Controller.Motor[i].speed_pid.output);
		}

	}
	for (int i = 4; i < 8; i++) {
		if (Controller.Motor[i].speed_pid.output != 0) {
			tx_Frame5_8.isValid = 1;
			tx_Frame5_8.Data[2 * i] =
					(uint8_t) (Controller.Motor[i].speed_pid.output >> 8);
			tx_Frame5_8.Data[2 * i + 1] =
					(uint8_t) (Controller.Motor[i].speed_pid.output);
		}
	}

	if (tx_Frame1_4.isValid) {
		canTransmit(&tx_Frame1_4);

	}
	if (tx_Frame5_8.isValid) {
		canTransmit(&tx_Frame5_8);
	}
	if (tx_Frame6020.isValid) {
		canTransmit(&tx_Frame6020);
	}
}
