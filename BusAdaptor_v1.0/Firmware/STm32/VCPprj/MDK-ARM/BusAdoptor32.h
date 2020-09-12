#ifndef BUSADOPTOR_H
#define BUSADOPTOR_H
#include "can.h"
#include "usbd_cdc_if.h"

/******************************************************
USB帧格式:要求总共小于512字节
———————————————————————————————————————————————————————
HEADER :=BA  2字节   帧头   
MsgType:=XXX 1字节   数据内容类型:外设类型|操作类型
DLC    :=xxx 2字节   数据长度
DATA   :=XXX n字节   数据内容  
长度2字节    
*******************************************************/


typedef struct{
  uint16_t HEADER;
  uint8_t TYPE;
  uint16_t DLC;
}USB_MSG_HEAD;
extern USB_MSG_HEAD *pmsg;
uint8_t BA_Decoding(uint8_t *buf);
void BA_CAN_Init(uint8_t *buf);
void BA_CAN_Filter(uint8_t *buf);
void BA_CAN_Send(uint8_t *buf);
#endif