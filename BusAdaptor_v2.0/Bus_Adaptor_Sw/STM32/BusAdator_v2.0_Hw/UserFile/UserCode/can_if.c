/*
 * can_if.c
 *
 *  Created on: Sep 7, 2020
 *      Author: ou
 */

#include "can_if.h"
#include "FreeRTOS.h"
#include "task.h"
#define CAN_RINGBUF_LENGTH 100
CanRxFrame_t CanRxBuf[CAN_RINGBUF_LENGTH];
RingBuf_t CanRxRingBuf;

canFrame_t tx_Frame1_4, tx_Frame5_8, tx_Frame6020;
canFrame_t rRosFrame[4], sRosFrame;

HAL_StatusTypeDef Can_Init_User() {
	RingBuf_Creat(&CanRxRingBuf, CanRxBuf, CAN_RINGBUF_LENGTH,sizeof(CanRxFrame_t));

	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_CAN_Start(&hcan1) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING)
			!= HAL_OK) {
		Error_Handler();
	}
	return HAL_OK;
}

void canTransmit(canFrame_t *Msg) {
	if (HAL_CAN_AddTxMessage(&Msg->canHandle, &Msg->Header, Msg->Data,
			(uint32_t*) CAN_TX_MAILBOX0) != HAL_OK) {
		Error_Handler();       //如果CAN信息发送失败则进入死循环
	}
}

extern TaskHandle_t can_process_packet_thread_Handle;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_RxHeaderTypeDef RxHeader;
	CanRxFrame_t *frameptr;

	RingBuf_Write(&CanRxRingBuf, (void**) &frameptr);
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, frameptr->Data)
			!= HAL_OK) {
		Error_Handler();
	}
	frameptr->DLC = RxHeader.DLC;
	frameptr->IDE = RxHeader.IDE;
	frameptr->EID = frameptr->IDE == CAN_ID_STD ? RxHeader.StdId : RxHeader.ExtId;

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	//https://www.cnblogs.com/w-smile/p/11333950.html
	//外设中断优先级不能小于５否则　不能调用rtos ISRapi .
	vTaskNotifyGiveFromISR(can_process_packet_thread_Handle, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_RxHeaderTypeDef RxHeader;
	CanRxFrame_t *frameptr;
	RingBuf_Write(&CanRxRingBuf, (void**) &frameptr);
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, frameptr->Data)
			!= HAL_OK) {
		Error_Handler();
	}
	frameptr->DLC = RxHeader.DLC;
	frameptr->IDE = RxHeader.IDE;
	frameptr->EID = frameptr->IDE == CAN_ID_STD ? RxHeader.StdId : RxHeader.ExtId;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	//https://www.cnblogs.com/w-smile/p/11333950.html
	//外设中断优先级不能小于５否则　不能调用rtos ISRapi .
	vTaskNotifyGiveFromISR(can_process_packet_thread_Handle, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

}

