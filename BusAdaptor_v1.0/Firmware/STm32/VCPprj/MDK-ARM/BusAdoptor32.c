#include "BusAdoptor32.h"

static uint32_t dummydata;

#define UNION(TYPE)     typedef union{TYPE data;uint8_t u8data[sizeof(TYPE)];}UNION##TYPE
#define _USB_MSG(TYPE)    typedef struct{USB_MSG_HEAD head;TYPE PL;}_##TYPE

/* --- 类型定义 --- */
#define USB_MSG_HEADER_BYTE 2
#define USB_MSG_TYPE_BYTE 1
#define USB_MSG_DLC_BYTE 2
#define USB_MSG_DATA_MASK 5




typedef enum{
  BA_USB=0x00,
  BA_CAN=0x10
}BA_PERITYPE_TYPE;
typedef enum{
  BA_INIT=0x00,
  BA_CONFIG,
  BA_SEND,
  BA_RECV
}BA_OP_TYPE;

/* --- 方便发送做的类型转换 --- */
_USB_MSG(CAN_TxHeaderTypeDef);UNION(_CAN_TxHeaderTypeDef);
_USB_MSG(CAN_RxHeaderTypeDef);UNION(_CAN_RxHeaderTypeDef);
_USB_MSG(CAN_InitTypeDef);UNION(_CAN_InitTypeDef);
_USB_MSG(CAN_FilterTypeDef);UNION(_CAN_FilterTypeDef);


/* --- linux传来数据进行打表 --- */
static uint32_t __TimeSeg1[]={CAN_BS1_1TQ,CAN_BS1_2TQ,CAN_BS1_3TQ,CAN_BS1_4TQ,
                              CAN_BS1_5TQ,CAN_BS1_6TQ,CAN_BS1_7TQ,CAN_BS1_8TQ,
                              CAN_BS1_9TQ,CAN_BS1_10TQ,CAN_BS1_11TQ,CAN_BS1_12TQ,
                              CAN_BS1_13TQ,CAN_BS1_14TQ,CAN_BS1_15TQ,CAN_BS1_16TQ};
static uint32_t __TimeSeg2[]={CAN_BS2_1TQ,CAN_BS2_2TQ,CAN_BS2_3TQ,CAN_BS2_4TQ,
                              CAN_BS2_5TQ,CAN_BS2_6TQ,CAN_BS2_7TQ,CAN_BS2_8TQ};
static uint32_t __SyncJumpWidth[]={CAN_SJW_1TQ,CAN_SJW_2TQ,CAN_SJW_3TQ,CAN_SJW_4TQ};
static uint32_t __Mode[]={CAN_MODE_NORMAL,CAN_MODE_LOOPBACK,CAN_MODE_SILENT,CAN_MODE_SILENT_LOOPBACK};


static uint32_t __FilterMode[]={CAN_FILTERMODE_IDMASK,CAN_FILTERMODE_IDLIST};
static uint32_t __FilterScale[]={CAN_FILTERSCALE_16BIT,CAN_FILTERSCALE_32BIT};
static uint32_t __FilterFIFOAssignment[]={CAN_RX_FIFO0,CAN_RX_FIFO1};


/* --- 实现代码 --- */
#define BA_CAN_Rcev0_IT HAL_CAN_RxFifo0MsgPendingCallback
#define BA_CAN_Rcev1_IT HAL_CAN_RxFifo1MsgPendingCallback






/* --- 接收部分 --- */

/************************************************************************/

void BA_CAN_Init(uint8_t *buf)
{
  HAL_CAN_Stop(&hcan1);
  HAL_CAN_DeInit(&hcan1);
  
  
  CAN_InitTypeDef *_caninit = & (((UNION_CAN_InitTypeDef *)buf)->data.PL);
  _caninit->Mode=__Mode[_caninit->Mode];
  _caninit->SyncJumpWidth=__SyncJumpWidth[_caninit->SyncJumpWidth-1];
  _caninit->TimeSeg1=__TimeSeg1[_caninit->TimeSeg1-1];
  _caninit->TimeSeg2=__TimeSeg2[_caninit->TimeSeg2-1];
  
  
  hcan1.Init=*_caninit;
  
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
}

void BA_CAN_Filter(uint8_t *buf)
{
  CAN_FilterTypeDef *filterconfig = & (((UNION_CAN_FilterTypeDef *)buf)->data.PL);
  filterconfig->FilterMode=__FilterMode[filterconfig->FilterMode];
  filterconfig->FilterScale=__FilterScale[filterconfig->FilterScale];
  filterconfig->FilterFIFOAssignment=__FilterFIFOAssignment[filterconfig->FilterFIFOAssignment];
  
  if(HAL_CAN_ConfigFilter(&hcan1,filterconfig)!= HAL_OK)
  {
    Error_Handler();
  }
  
  if (HAL_CAN_Start(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
  {
    Error_Handler();
  }
}



void BA_CAN_Send(uint8_t *buf)
{
	CAN_TxHeaderTypeDef *cantxheader= &(((UNION_CAN_TxHeaderTypeDef *)buf)->data.PL);
  //本来觉得直接把接收buf当作can发送buf可能can发的慢了，接收buf会覆盖。
  //后来发现接收buf好像是在can发送的时候把里面的数据先放入硬件buf了，然后can慢慢从硬件buf读走发送。
  //先这样写试试
	if(HAL_CAN_AddTxMessage(&hcan1,cantxheader,(uint8_t*)(cantxheader+1),&dummydata)!=HAL_OK)
	{
		 Error_Handler();      
	}
}


/* --- 发送部分 --- */
USB_MSG_HEAD *pmsg=NULL;
void BA_CAN_Rcev0_IT(CAN_HandleTypeDef *hcan)
{
  //切换请求发送buff
  curTxIdx_Wait = 0x01 - curTxIdx_OK;
  
  UNION_CAN_RxHeaderTypeDef  *RxHeader  =  (void *)UserTxPingPong[curRxIdx_Wait];
  pmsg = (void *)RxHeader;
  
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &(RxHeader->data.PL),(uint8_t*)(RxHeader+1)) != HAL_OK)
  {
    Error_Handler();            //错误状濁回调函数
  }
  RxHeader->data.head.TYPE = BA_CAN|BA_RECV;
  RxHeader->data.head.DLC = sizeof(UNION_CAN_RxHeaderTypeDef)+RxHeader->data.PL.DLC;
}

void BA_CAN_Rcev1_IT(CAN_HandleTypeDef *hcan)
{
  //切换请求发送buff
  curTxIdx_Wait = 0x01 - curTxIdx_OK;
 
  UNION_CAN_RxHeaderTypeDef  *RxHeader  = (void *)UserTxPingPong[curRxIdx_Wait];
  pmsg = (void *)RxHeader;
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &(RxHeader->data.PL),(uint8_t*)(RxHeader+1)) != HAL_OK)
  {
    Error_Handler();            //错误状濁回调函数
  }
  RxHeader->data.head.TYPE = BA_CAN|BA_RECV;
  RxHeader->data.head.DLC = sizeof(UNION_CAN_RxHeaderTypeDef)+RxHeader->data.PL.DLC;
}


int idx=0;
uint8_t BA_Decoding(uint8_t *buf)
{
  idx=0;
  if(buf[idx++] != 'B' || buf[idx++] != 'A')
    return HAL_ERROR;
  switch(buf[idx]&0xF0)
  {
    case BA_USB:
    {
      break;
    }
    case BA_CAN:
    {
      switch(buf[idx]&0x0F)
      {
        case BA_INIT:
        {
          BA_CAN_Init(buf);
          break;
        }
        case BA_CONFIG:
        {
          BA_CAN_Filter(buf);
          break;
        }
        case BA_SEND:
        {
          BA_CAN_Send(buf);
          break;
        }

      }
      break;
    }
    defalt:
    {
      ;
    }
  }
}
