#include "decode.h"
#include "usbd_cdc_if.h"
#define ROTER_RANGE	8192

void CanDataDecoder(Motor_t *motor, CanRxFrame_t *read_ptr) {
	//更新位置

	motor->rotor_pos[LAST] = motor->rotor_pos[NOW];
	motor->rotor_angle[LAST] = motor->rotor_angle[NOW];
	motor->rotor_pos[NOW] = read_ptr->Data[0] << 8 | read_ptr->Data[1];
	//更新速度
	motor->rotor_vol = read_ptr->Data[2] << 8 | read_ptr->Data[3];
	motor->speed_pid.fdb = read_ptr->Data[2] << 8 | read_ptr->Data[3];
	//更新总转数

	/* 将电机速度反馈值由无符号整型转变为有符号整型 */
	if (motor->rotor_pos[NOW] > 32768) {
		motor->rotor_pos[NOW] -= 65536;
	}
	/* 电机圈数更新 */
	if (motor->rotor_pos[NOW] - motor->rotor_pos[LAST] > (ROTER_RANGE / 2)) {
		motor->rotor_round--;
	} else if (motor->rotor_pos[NOW] - motor->rotor_pos[LAST]
			< -(ROTER_RANGE / 2)) {
		motor->rotor_round++;
	}
	/* 更新转子当前角度 */
	motor->rotor_angle[NOW] = motor->rotor_pos[NOW]
			+ ROTER_RANGE * motor->rotor_round - motor->offsetPos;
	/* 更新输出轴角度 */
	motor->output_angle = (motor->rotor_pos[NOW]
			+ ROTER_RANGE * motor->rotor_round - motor->offsetPos) * PI * 2
			/ motor->reductionRatio / 8192;
	if (motor->mode == SPEED) {
		motor->offsetPos = motor->rotor_pos[NOW];
		motor->rotor_round = 0;
	}
}

/**
 * @brief 根据电机信息的ID号进行对应的数据解析
 * @param 电机ID号
 * @retval None
 */
void CanDataPreDecode(CanRxFrame_t *read_ptr) {

	// 解析电机报文
	int motor_index = read_ptr->SID;
	for (int i = 0; i < 8; i++) {
		if ((Controller.Motor[i].type == m6020&& Controller.Motor[i].can_id == motor_index - 4)
			|| (Controller.Motor[i].type != m6020&& Controller.Motor[i].can_id == motor_index))
		{
			if (Controller.Motor[i].msg_cnt++ < 50) {
				Controller.Motor[i].rotor_pos[NOW] = (uint16_t) (read_ptr->Data[0] << 8 | read_ptr->Data[1]);
				Controller.Motor[i].offsetPos = Controller.Motor[i].rotor_pos[NOW];
			} else
				CanDataDecoder(&Controller.Motor[i], read_ptr);
			return;
		}
	}
}
// 更新电机PID和减速比
void updateMotor(Motor_t *motor) {
	switch (motor->type) {
	case m3508: {
		motor->speed_pid.KP = 15;
		motor->speed_pid.KI = 1;
		motor->speed_pid.KD = 1;
		motor->speed_pid.outputMax = 10000;
		motor->pos_pid.KP = 0.2f;
		motor->pos_pid.KI = 0.0007f;
		motor->pos_pid.KD = 0.001f;
		motor->pos_pid.outputMax = 10000;
		motor->reductionRatio = 3591.0f / 187.0f;

		break;
	}
	case m2006: {
		motor->speed_pid.KP = 15;
		motor->speed_pid.KI = 1;
		motor->speed_pid.KD = 1;
		motor->speed_pid.outputMax = 10000;
		motor->pos_pid.KP = 1.0f;
		motor->pos_pid.KI = 0.008f;
		motor->pos_pid.KD = 0.002f;
		motor->pos_pid.outputMax = 10000;
		motor->reductionRatio = 36.0f;

		break;
	}
	case m6020: {
		motor->speed_pid.KP = 15;
		motor->speed_pid.KI = 1;
		motor->speed_pid.KD = 1;
		motor->speed_pid.outputMax = 10000;
		motor->pos_pid.KP = 0.2f;
		motor->pos_pid.KI = 0.0007f;
		motor->pos_pid.KD = 0.001f;
		motor->pos_pid.outputMax = 10000;
		motor->reductionRatio = 1.0f;
		break;
	}

	default: {

		motor->speed_pid.KP = 15;
		motor->speed_pid.KI = 1;
		motor->speed_pid.KD = 1;
		motor->speed_pid.outputMax = 10000;
		motor->pos_pid.KP = 0.2f;
		motor->pos_pid.KI = 0.0007f;
		motor->pos_pid.KD = 0.001f;
		motor->pos_pid.outputMax = 10000;
		motor->reductionRatio = 3591.0f / 187.0f;

		break;

	}
	}
}

void rosCommondDecoder(uint8_t *UserRxBufferHS, int pack_idx) {

	// 1-4号电机状态设置
	switch (pack_idx) {
	case (0): {
		for (int i = 0; i < 4; i++) {
			Controller.Motor[i].type = UserRxBufferHS[2 * i];
			Controller.Motor[i].mode = UserRxBufferHS[2 * i + 1];
			updateMotor(&Controller.Motor[i]);
		}
		break;
	}
	case (1):
		// 1-4号电机目标设置
		for (int i = 0; i < 4; i++) {
			if (Controller.Motor[i].mode == 2) {
				Controller.Motor[i].target = (float) (UserRxBufferHS[2 * i] << 8
						| UserRxBufferHS[2 * i + 1]) / 100;
			} else if (Controller.Motor[i].mode == 1) {
				Controller.Motor[i].target = UserRxBufferHS[2 * i] << 8
						| UserRxBufferHS[2 * i + 1];
			} else {
				Controller.Motor[i].target = 0;
			}
		}
		break;
	}

	// 5-8号电机状态设置
	for (int i = 4; i < 8; i++) {
		Controller.Motor[i].type = UserRxBufferHS[2 * (i - 4)];
		Controller.Motor[i].mode = UserRxBufferHS[2 * (i - 4) + 1];
		updateMotor(&Controller.Motor[i]);
	}

	// 5-8号电机目标(i-4)置
	for (int i = 4; i < 8; i++) {
		if (Controller.Motor[i].mode == POSITION) {
			Controller.Motor[i].target = (UserRxBufferHS[2 * (i - 4)] << 8
					| UserRxBufferHS[2 * (i - 4) + 1]) / 100;
		} else if (Controller.Motor[i].mode == 1) {
			Controller.Motor[i].target = UserRxBufferHS[2 * (i - 4)] << 8
					| UserRxBufferHS[2 * (i - 4) + 1];
		} else {
			Controller.Motor[i].target = 0;
		}
	}
}


