https://www.cnblogs.com/lvdongjie/p/7921433.html
https://www.cnblogs.com/zengjfgit/p/4943316.html
https://blog.csdn.net/gd6321374/article/details/79903132
https://blog.csdn.net/zhu1146734128/article/details/83904616















# 梳理STM32的USBD_CDC

## Cube配置

https://blog.csdn.net/zzzw0/article/details/88089449



![image-20200725143917927](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200725143917927.png)

在这里的512Byte，为一个CDC包的payload，如果linux主机发的一个包大于512B，则接收失败。

而下面的buf是缓存而已。





## 代码梳理

stm32上的CDC 端点。IN和OUT是相对于主机的。现在写的是从设备。故stm32从OUT端点接收，IN端点发送。上位机从IN端点接收，OUT端点发送。

![image-20200723152455359](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200723152455359.png)





这里梳理一下调用堆栈。不然可能让人头晕不知道数据是什么个流向。

![image-20200724132931685](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724132931685.png)默认勾选了全局中断。

当STM32做VCP设备时候接收到OUT端点数据，则调用堆栈如下

![image-20200724141543569](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724141543569.png)

其中中断服务函数中有这句话：

<img src="C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724141716774.png" alt="image-20200724141716774" style="zoom:50%;" />

这里是用户的接收回调，是个回调函数！将数据放入用户指定内存中。

![image-20200724142414302](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724142414302.png)

第一次指定是在

<img src="C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724142714108.png" alt="image-20200724142714108" style="zoom:50%;" />

而这个回调函数通过CDC回调函数结构体

<img src="C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724142526781.png" alt="image-20200724142526781" style="zoom:67%;" /><img src="C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724142546555.png" alt="image-20200724142546555" style="zoom:67%;" />

在main函数中

<img src="C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724142842582.png" alt="image-20200724142842582" style="zoom:100%;" />



![image-20200724142855849](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724142855849.png)将用户设定的回调函数集合(函数指针集合)结构体赋值给底层的句柄，然后脱离用户层，用底层名称接口进行底层编程。

![image-20200724143023398](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724143023398.png)

搜索pdev->pUserData。可以发现在USBD_CDC_DataOut中调用了用户接收回调函数。

![image-20200724143051292](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724143051292.png)



同样的操作。这里又封装了一层

<img src="C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724143138140.png" alt="image-20200724143138140" style="zoom:50%;" /> <img src="C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724143154360.png" alt="image-20200724143154360" style="zoom:50%;" />



在更底层的函数中，USBD_CDC_DataOut被以DataOut来调用。

<img src="C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724143604921.png" alt="image-20200724143604921" style="zoom:67%;" />

回到中断服务函数这

![image-20200724143650725](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724143650725.png)

进入为

<img src="C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724143738453.png" alt="image-20200724143738453" style="zoom:50%;" />

![image-20200724143748639](C:\Users\70654\AppData\Roaming\Typora\typora-user-images\image-20200724143748639.png)

总结以上回调过程：

STM32从USB的OUT端点上接收到数据后，进入

* OTG_HS_IRQHandler中断
  * HAL_PCD_IRQHandler
    * PCD_EP_OutXfrComplete_int
      * HAL_PCD_DataOutStageCallback
        * USBD_LL_DataOutStage
          * DataOut = USBD_CDC_DataOut
            * Receive = CDC_Receive_HS





# 修改程序为PingPongBuffer







# 解码部分

通过宏

```c
#define UNION(TYPE)     typedef union {TYPE typedata;uint8_t u8data[sizeof(TYPE)];}UNION_##TYPE
```

来生成TYPE类型对应的共用体方便发送。



接收端接收到u8的data。使用以下语句实现buf内容转换到结构体,语句开销微乎其微，感叹C的强大。

```c
  UNION_CAN_InitTypeDef *_caninit = (UNION_CAN_InitTypeDef *)&buf[USB_MSG_DATA_MASK];
  hcan1.Init=_caninit->typedata;
```













# Linux端绑定端口

sudo gedit /etc/udev/rules.d/50-BusAdoptor.rules

文件内容为

SUBSYSTEMS=="usb", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="5740", GROUP="ou", MODE="0666"

其中GROUP是用户组，填入你终端上那个@后面的那个为用户。

idVendorw为销售的设备号，idProduct为制造商号，可以通过lsusb指令得到。

```con
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 006: ID 0bda:b00b Realtek Semiconductor Corp. 
Bus 001 Device 005: ID 04f2:b5d6 Chicony Electronics Co., Ltd 

Bus 001 Device 003: ID 0483:5740 STMicroelectronics STM32F407

Bus 001 Device 002: ID 24ae:1100  
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
```









//结合一下两篇完成读
运行的时候需要sudo ./exe
这篇说了VID PID 输入输出端口号。上位读取端口号0x81 写估计就是0x01了。看看cube怎么配置的
https://blog.csdn.net/zhu1146734128/article/details/83904616
这篇说了如何打开设备等等操作
https://blog.csdn.net/u011598479/article/details/82705496



把端口号改成0x01就是发送，0x81就是读取。

```c++
#include <iostream>
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define USB_VID         0x0483          //USB设备的产商ID
#define USB_PID         0x5740         //USB设备的产品ID
#define EPIN_ADDR       0X81
#define EPOUT_ADDR      0X01
using namespace std;

static int device_satus(libusb_device_handle *hd)
{

    int interface = 0;
    unsigned char byte;
    libusb_control_transfer(hd, LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                            LIBUSB_REQUEST_CLEAR_FEATURE,
                            0,
                            interface,
                            &byte, 1, 5000);

    printf("status:0x%x\n", byte);
/**
 * byte
 * normal:0x18
 * other :0x10
 */
    return 0;
}

int main() {
    libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
    libusb_device_handle *dev_handle; //a device handle
    libusb_context *ctx = NULL; //a libusb session
    int r; //for return values
    ssize_t cnt; //holding number of devices in list
    r = libusb_init(&ctx); //initialize the library for the session we just declared
    if(r < 0) {
        perror("Init Error\n"); //there was an error
        return 1;
    }

    libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO); //set verbosity level to 3, as suggested in the documentation

    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
    if(cnt < 0) {
        perror("Get Device Error\n"); //there was an error
        return 1;
    }
    printf("%d Devices in list.\n", cnt);

    dev_handle = libusb_open_device_with_vid_pid(ctx, USB_VID, USB_PID); //these are vendorID and productID I found for my usb device
    if(dev_handle == NULL)
        perror("Cannot open device\n");
    else
        printf("Device Opened\n");
    libusb_free_device_list(devs, 1); //free the list, unref the devices in it

    if(libusb_kernel_driver_active(dev_handle, 0) == 1) { //find out if kernel driver is attached
        printf("Kernel Driver Active\n");
        if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
            printf("Kernel Driver Detached!\n");
    }
    r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
    if(r < 0) {
        perror("Cannot Claim Interface\n");
        return 1;
    }
    printf("Claimed Interface\n");

    device_satus(dev_handle);
    uint8_t  buf[512];
    int length=0;
    int rv =0;
    while(1)
    {
        libusb_bulk_transfer(dev_handle,0X01,buf,512,&length,10000);
    }
    if(rv < 0) {
        printf("*** bulk_transfer failed! \n");
        return -1;
    }



    libusb_close(dev_handle); //close the device we opened
    libusb_exit(ctx); //needs to be called to end the

    return 0;
}


```









# 参考

//开源上位机
https://bbs.21ic.com/icview-875805-1-1.html?ordertype=1



//测速与上位机很重要的网站

https://bbs.21ic.com/icview-811704-1-1.html





//代码交换

```
#ifndef BUSADOPTOR_H
#define BUSADOPTOR_H
#include "can.h"
#include "usbd_cdc_if.h"
#define UNION(TYPE)     typedef union {TYPE typedata;uint8_t u8data[sizeof(TYPE)];}UNION_##TYPE

/******************************************************
USB帧格式:要求总共小于512字节
———————————————————————————————————————————————————————
HEADER :=BA  2字节   帧头   
MsgType:=XXX 1字节   数据内容类型:外设类型|操作类型
DLC    :=xxx 2字节   数据长度
DATA   :=XXX n字节   数据内容  
长度2字节    
*******************************************************/
#define USB_MSG_HEADER_BYTE 2
#define USB_MSG_TYPE_BYTE 1
#define USB_MSG_DLC_BYTE 2
#define USB_MSG_DATA_MASK 5

typedef struct{
  uint16_t HEADER;
  uint8_t TYPE;
  uint16_t DLC;
}USB_MSG;


typedef enum{
  BA_USB=0xF0,
  BA_CAN
}BA_PERITYPE_TYPE;
typedef enum{
  BA_INIT=0x00,
  BA_SEND,
  BA_RECV
}BA_OP_TYPE;

/******************************************/
UNION(CAN_TxHeaderTypeDef);
UNION(CAN_RxHeaderTypeDef);
UNION(CAN_InitTypeDef);
UNION(USB_MSG);



uint8_t BA_Decoding(uint8_t *buf);
void BA_CAN_INIT(uint8_t *buf);
void BA_CAN_SEND(uint8_t *buf);
#endif
```



```
typedef enum 
{
  DISABLE = 0U, 
  ENABLE = !DISABLE
} FunctionalState;

typedef struct
{
  uint32_t StdId;
  uint32_t ExtId;
  uint32_t IDE;
  uint32_t RTR;
  uint32_t DLC;
  FunctionalState TransmitGlobalTime; /*!< Specifies whether the timestamp counter value captured on start
                          of frame transmission, is sent in DATA6 and DATA7 replacing pData[6] and pData[7].
                          @note: Time Triggered Communication Mode must be enabled.
                          @note: DLC must be programmed as 8 bytes, in order these 2 bytes are sent.
                          This parameter can be set to ENABLE or DISABLE. */
  //ubuntu上新增
  uint8_t data[8];
} CAN_TxHeaderTypeDef;
```

```
typedef struct
{
  uint32_t StdId;
  uint32_t ExtId;
  uint32_t IDE;
  uint32_t RTR;
  uint32_t DLC;
  uint32_t Timestamp;
  uint32_t FilterMatchIndex; /*!< Specifies the index of matching acceptance filter element.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF. */
  //ubuntu新增
  uint8_t data[8];
} CAN_RxHeaderTypeDef;
```



```
typedef struct
{
  uint32_t Prescaler;
  uint32_t Mode;
  uint32_t SyncJumpWidth;
  uint32_t TimeSeg1;
  uint32_t TimeSeg2;
  FunctionalState TimeTriggeredMode;
  FunctionalState AutoBusOff;
  FunctionalState AutoWakeUp;
  FunctionalState AutoRetransmission;
  FunctionalState ReceiveFifoLocked;
  FunctionalState TransmitFifoPriority;
} CAN_InitTypeDef;
```

```
typedef struct
{
  uint32_t FilterIdHigh;          /*!< Specifies the filter identification number (MSBs for a 32-bit
                                       configuration, first one for a 16-bit configuration).
                                       This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  uint32_t FilterIdLow;           /*!< Specifies the filter identification number (LSBs for a 32-bit
                                       configuration, second one for a 16-bit configuration).
                                       This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  uint32_t FilterMaskIdHigh;      /*!< Specifies the filter mask number or identification number,
                                       according to the mode (MSBs for a 32-bit configuration,
                                       first one for a 16-bit configuration).
                                       This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  uint32_t FilterMaskIdLow;       /*!< Specifies the filter mask number or identification number,
                                       according to the mode (LSBs for a 32-bit configuration,
                                       second one for a 16-bit configuration).
                                       This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  uint32_t FilterFIFOAssignment;  /*!< Specifies the FIFO (0 or 1U) which will be assigned to the filter.
                                       This parameter can be a value of @ref CAN_filter_FIFO */

  uint32_t FilterBank;            /*!< Specifies the filter bank which will be initialized.
                                       For single CAN instance(14 dedicated filter banks),
                                       this parameter must be a number between Min_Data = 0 and Max_Data = 13.
                                       For dual CAN instances(28 filter banks shared),
                                       this parameter must be a number between Min_Data = 0 and Max_Data = 27. */

  uint32_t FilterMode;            /*!< Specifies the filter mode to be initialized.
                                       This parameter can be a value of @ref CAN_filter_mode */

  uint32_t FilterScale;           /*!< Specifies the filter scale.
                                       This parameter can be a value of @ref CAN_filter_scale */

  uint32_t FilterActivation;      /*!< Enable or disable the filter.
                                       This parameter can be a value of @ref CAN_filter_activation */

  uint32_t SlaveStartFilterBank;  /*!< Select the start filter bank for the slave CAN instance.
                                       For single CAN instances, this parameter is meaningless.
                                       For dual CAN instances, all filter banks with lower index are assigned to master
                                       CAN instance, whereas all filter banks with greater index are assigned to slave
                                       CAN instance.
                                       This parameter must be a number between Min_Data = 0 and Max_Data = 27. */

} CAN_FilterTypeDef;
```

