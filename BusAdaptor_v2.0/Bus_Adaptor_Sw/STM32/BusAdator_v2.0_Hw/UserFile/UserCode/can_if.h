/*
 * can_if.h
 *
 *  Created on: Sep 7, 2020
 *      Author: ou
 */

#ifndef CAN_IF_H_
#define CAN_IF_H_
#include "can.h"
#include "ringbuf.h"

typedef struct {

	CAN_HandleTypeDef canHandle;
	CAN_TxHeaderTypeDef Header;
	uint8_t Data[8];
	int isValid;
} canFrame_t;

typedef struct {
	struct {
		uint8_t DLC :4;
		uint8_t RTR :2;
		uint8_t IDE :3;
	};
	union {
		struct {
			uint32_t SID :11;
		};
		struct {
			uint32_t EID :29;
		};
	};
	uint8_t Data[8];
} CanRxFrame_t;



extern canFrame_t tx_Frame1_4, tx_Frame5_8, tx_Frame6020;
extern canFrame_t rRosFrame[4], sRosFrame;
extern RingBuf_t CanRxRingBuf;





HAL_StatusTypeDef Can_Init_User();
void canTransmit(canFrame_t *Msg);
#endif /* CAN_IF_H_ */
