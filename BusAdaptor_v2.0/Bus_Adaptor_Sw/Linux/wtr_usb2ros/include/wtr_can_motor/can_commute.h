//
// Created by kevin on 2020/6/22.
//

#ifndef WTR_CAN_MOTOR_CAN_COMMUTE_H
#define WTR_CAN_MOTOR_CAN_COMMUTE_H
#include "wtr_can_motor/common_include.h"

namespace wtr{

    static std::vector<libusb_device_handle *> devices;

    static int device_satus(libusb_device_handle *hd) {

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
    bool USB_OpenDevice(uint32_t vid, uint32_t pid) {
        libusb_device **devs;
        libusb_device_handle *dev_handle;
        libusb_context *ctx;//a libusb session

        int r; //for return values
        ssize_t cnt; //holding number of devices in list
        r = libusb_init(&ctx); //initialize the library for the session we just declared
        if (r < 0) {
            perror("Init Error\n"); //there was an error
            return false;
        }

        libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO); //set verbosity level to 3, as suggested in the documentation

        cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
        if (cnt < 0) {
            perror("Get Device Error\n"); //there was an error
            return false;
        }
        printf("%d Devices in list.\n", cnt);

        dev_handle = libusb_open_device_with_vid_pid(ctx, vid,
                                                     pid); //these are vendorID and productID I found for my usb device
        if (dev_handle == NULL)
            perror("Cannot open device\n");
        else
            printf("Device Opened\n");
        libusb_free_device_list(devs, 1); //free the list, unref the devices in it

        if (libusb_kernel_driver_active(dev_handle, 0) == 1) { //find out if kernel driver is attached
            printf("Kernel Driver Active\n");
            if (libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
                printf("Kernel Driver Detached!\n");
        }
        r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
        if (r < 0) {
            perror("Cannot Claim Interface\n");
            return false;
        }
        printf("Claimed Interface\n");
        devices.push_back(dev_handle);
        return true;
    };
    bool USB_CloseDevice(uint8_t handleidx) {
        libusb_close(devices[handleidx]);
    }

    bool inline USB_Send(int dev_idx, uint8_t *buf, int length, int timeout) {

        int rv = libusb_bulk_transfer(devices[dev_idx], 0X01, buf, length, &length, timeout);
        if (rv < 0) {
            printf("*** bulk_transfer failed! \n");
            return false;
        }
        return true;
    }

    bool inline USB_Recv(int dev_idx, uint8_t *buf, int length, int timeout) {
        int rv = libusb_bulk_transfer(devices[dev_idx], 0X81, buf, length, &length, timeout);
        if (rv < 0) {
            printf("*** bulk_recv failed! \n");
            return false;
        }
        return true;
    }



    class Motor{
    private:
        void upDateMotorType()
        {
            switch (type) {
                case M3508:
                    reductionRatio = 3591.0f/187.9f;
                    break;
                case M2006:
                    reductionRatio = 36.0f;
                    break;
                case M6020:
                    reductionRatio = 1.0f;
                    break;
                default:{}
            }
        }
    public:
        float _target;
        int32_t msg_cnt;
        float rotor_angle[2], rotor_vol, rotor_pos[2], rotor_round;
        motor_pub_t motorInfo;
        int32_t offsetPos;
        float reductionRatio;
        int mode;
        int type;
        Motor(){
            mode = UNDEFINED;
            motorInfo.output_angle = 0;
            motorInfo.output_vol = 0;
            motorInfo.current = 0;
            motorInfo.temperature=0;
            offsetPos = 0;
            reductionRatio = 36;

        }
        /**
* @brief 设置电机初始角度
* @param 电机数据结构体
* @retval None
*/
        void getRotorOffset(uint8_t * CanMsgBuffer)
        {
            rotor_pos[NOW] = (uint16_t)(CanMsgBuffer[0]<<8 | CanMsgBuffer[1]);
            offsetPos= rotor_pos[NOW];
        }

/**
* @brief 3508CAN通信的反馈数据具体解析函数
* @param 电机数据结构体
* @retval None
*/
        void canDataDecoder(uint8_t* CanMsgBuffer){
            upDateMotorType();

            // 更新转矩电流
            motorInfo.current = CanMsgBuffer[4]<<8|CanMsgBuffer[5];
            if( motorInfo.current > 32768)
            {
                motorInfo.current-= 65536;
            }

            // 更新电机温度
            motorInfo.temperature = CanMsgBuffer[6];

            //更新位置
            rotor_pos[LAST] = rotor_pos[NOW];
            rotor_angle[LAST] = rotor_angle[NOW];
            rotor_pos[NOW] = CanMsgBuffer[0]<<8|CanMsgBuffer[1];
            //更新速度
            rotor_vol= CanMsgBuffer[2]<<8|CanMsgBuffer[3];
            //更新总转数
            /* 将电机速度反馈值由无符号整型转变为有符号整型 */
            if(rotor_pos[NOW] > 32768)
            {
                rotor_pos[NOW] -= 65536;
            }

            /* 电机圈数更新 */
            if(rotor_pos[NOW] - rotor_pos[LAST] > (ROTER_RANGE / 2))
            {
                rotor_round --;
            }
            else if( rotor_pos[NOW] - rotor_pos[LAST] < -(ROTER_RANGE/2))
            {
                rotor_round ++;
            }
            /* 更新转子当前角度 */
            rotor_angle[NOW] = rotor_pos[NOW] + ROTER_RANGE * rotor_round - offsetPos;
            /* 更新输出轴角度 */
            motorInfo.output_angle = rotor_angle[NOW] *  PI * 2 / reductionRatio  / ROTER_RANGE;
            motorInfo.output_vol = rotor_vol;// / reductionRatio;
            if( motorInfo.output_vol > 32768)
            {
                motorInfo.output_vol-= 65536;
            }
            motorInfo.output_vol /= reductionRatio;
        }
    };

    class CAN{
    private:
        void deInit(){
            uint32_t can_id = 0x300;
            uint8_t stop[8] = {0,0,0,0,0,0,0,0};
            canTransmit(can_id, stop, DJI);
            can_id = 0x302;
            canTransmit(can_id, stop, DJI);

//            for (int i=0;i<4;i++){
//                can_id = i;
//                canTransmit(can_id, stop, VESC);
//            }
        }

        int CANIndex;
        int type;
        Motor Motors[8];
        void canDataDecoder(uint8_t* CanMsgBuffer);
//        float fdb[8]={0};

//        int16_t buffer_get_int16(uint8_t* buf,int32_t *index){
//            int16_t result= ((uint16_t)buf[*index]) <<8 | ((uint16_t)buf[*index+1]);
//            (*index)+=2;
//            return result;
//        }
//        uint16_t buffer_get_uint16(uint8_t* buf,int32_t *index){
//            uint16_t result= ((uint16_t)buf[*index]) <<8 | ((uint16_t)buf[*index+1]);
//            (*index)+=2;
//            return result;
//        }
//        int32_t buffer_get_int32(uint8_t* buf,int32_t *index){
//            int32_t result= ((uint32_t)buf[*index]) <<24 |
//                            ((uint32_t)buf[*index+1]) <<16 |((uint32_t)buf[*index+2]) <<8  |
//                            ((uint32_t)buf[*index+3]);
//            (*index)+=4;
//            return result;
//        }
//        float buffer_get_float16(uint8_t* buf,int32_t *index){
//            return (float)buffer_get_int16(buf,index);
//        }
//        float buffer_get_float32(uint8_t* buf,int32_t *index){
//            return (float)buffer_get_int32(buf,index);
//        }
    public:
//        uint32_t vesc_can_id;
//        uint8_t  vesc_can_msg[8];
        CAN(){
            for(int i = 0;i<8;i++){
                Motors[i].rotor_angle[0] = 0;
            }
            CANIndex = 0;
            USB_OpenDevice(USB_VID,USB_PID);
        };
        ~CAN() {
            deInit();
            usleep(500 * 1000);
            USB_CloseDevice(0);
            LOG(INFO)<<"Closing USB";
        }

        Motor* getMotorsHandle(){
            return Motors;
        }


        /**
       * @brief  初始化USB设备，并扫描设备连接数，必须调用
       * @param
       * @retval
       */


        /**
        * @brief 初始化CAN通讯
        * @param None
        * @retval None
        */
        void canInit() {
//            CANConfig.CAN_Mode = 0;//正常模式
//            CANConfig.CAN_ABOM = 0;//禁止自动离线
//            CANConfig.CAN_NART = 1;//禁止报文重传
//            CANConfig.CAN_RFLM = 0;//FIFO满之后覆盖旧报文
//            CANConfig.CAN_TXFP = 1;//发送请求决定发送顺序
//            //配置波特率,波特率 = 100M/(BRP*(SJW+BS1+BS2))
//            CANConfig.CAN_BRP = 25;
//            CANConfig.CAN_BS1 = 2;
//            CANConfig.CAN_BS2 = 1;
//            CANConfig.CAN_SJW = 1;
//            int ret;
//            ret = CAN_Init(DevHandle[0],CANIndex,&CANConfig);
//            if(ret != CAN_SUCCESS){
//                LOG(FATAL)<<"Config CAN failed!\n";
//            }else{
//                printf("Config CAN Success!\n");
//            }
//            //配置过滤器，必须配置，否则可能无法收到数据
//
//            CANFilter.Enable = 1;
//            CANFilter.ExtFrame = 0;
//            CANFilter.FilterIndex = 0;
//            CANFilter.FilterMode = 0;
//            CANFilter.MASK_IDE = 0;
//            CANFilter.MASK_RTR = 0;
//            CANFilter.MASK_Std_Ext = 0;
//            CAN_Filter_Init(DevHandle[0],CANIndex,&CANFilter);
        }

        /**
        * @brief 接收CAN消息
        * @param None
        * @retval None
        */
        void canReadMsg() {
                ;
        }


        void upDateMotors(uint32_t can_id, uint8_t *msg, int type){
            if(type == DJI){
                if(can_id == 0x300 ){
                    for(int i = 0 ;i < 4 ; i ++){
                        Motors[i].mode = msg[2 * i + 1];
                        Motors[i].type = msg[2 * i];
                    }
                }
                else if( can_id == 0x302){
                    for(int i = 0 ;i < 4 ; i ++){
                        Motors[i+4].mode = msg[2 * i + 1];
                        Motors[i+4].type = msg[2 * i];
                    }
                }
            }

        }
        /**
          * @brief 发送电机电流
          * @param 电机ID号
          * @retval None
          */
        void canTransmit(const uint32_t  can_id, uint8_t *msg, int type){
            upDateMotors(can_id, msg, type);

            uint8_t rxdata[14]={'B','A'};
            rxdata[2]=can_id>>24;
            rxdata[3]=can_id>>16;
            rxdata[4]=can_id>>8;
            rxdata[5]=can_id;
            memcpy(&rxdata[6],msg,8);
            USB_Send(0,rxdata,14,0xff);
//            switch (type) {
//                case VESC:
//                {
////                    CanMsg.RemoteFlag = 0;
////                    CanMsg.ExternFlag = 1;
////                    CanMsg.DataLen = 4;
////                    CanMsg.ID = can_id | CAN_PACKET_SET_RPM;
////                    for(int i = 0;i < 4; i++)
////                        CanMsg.Data[i] = msg[i];
////                    break;
//                }
//                case DJI:
//                {
////                    CanMsg.RemoteFlag = 0;
////                    CanMsg.ExternFlag = 0;
////                    CanMsg.DataLen = 8;
////                    CanMsg.ID = can_id;
////                    for(int i = 0;i < 8 ; i++)
////                        CanMsg.Data[i] = msg[i];
////                    break;
//
//                }
//                default:{break;}
//            }
//
//            int SendedNum = CAN_SendMsg(DevHandle[0],0,&CanMsg,1);
//            if(SendedNum >= 0){
////       LOG(INFO)<<"Send success";
//            }else{
//                LOG(ERROR)<<"Send CAN data failed!\n";
//            }
        }

    };




    class Node{
        ros::NodeHandle nh_;
        ros::Publisher motor_pub[8];
        ros::Publisher vesc_raw_pub;
        ros::Subscriber motor_sub;
        ros::Publisher angle_pub;
        std_msgs::UInt8MultiArray msgs;

        void motorCb(const std_msgs::UInt8MultiArray::ConstPtr &msg) {
            msgs = *msg;
            ID_x = msg->data[0] << 24 | msg->data[1] << 16 |
                    msg->data[2] << 8 | msg->data[3];

            for( int i = 0;i<8;i++){
                Data[i] = msg->data[i+4];
            }
            if(msg->layout.dim[0].label == "DJI"){
                type = DJI;
            }
            else if (msg -> layout.dim[0].label == "VESC")
            {
                type = VESC;
            }
            else{
                LOG(ERROR)<<"The can message type is not defined. Is it a VESC or DJI?";
            }
//            LOG(INFO)<<Data[1];
        }
    public:
        uint32_t ID_x;
        uint8_t Data[8];
        int type;
        int types[8];
        int modes[8];
        Node()
        {
            motor_sub = nh_.subscribe<std_msgs::UInt8MultiArray>("/wtr/canbus", 1, boost::bind(&Node::motorCb, this, _1));
//            vesc_raw_pub = nh_.advertise<std_msgs::UInt8MultiArray>("/wtr/canbus/vesc/feedback",1);
//            angle_pub = nh_.advertise   <std_msgs::Float32MultiArray> ( "/wtr/canbus/DJI/angles", 1);
//            motor_pub[0] = nh_.advertise<wtr_usb2ros::motor_msgs>("/wtr/canbus/DJI/0", 1);
//            motor_pub[1] = nh_.advertise<wtr_usb2ros::motor_msgs>("/wtr/canbus/DJI/1", 1);
//            motor_pub[2] = nh_.advertise<wtr_usb2ros::motor_msgs>("/wtr/canbus/DJI/2", 1);
//            motor_pub[3] = nh_.advertise<wtr_usb2ros::motor_msgs>("/wtr/canbus/DJI/3", 1);
//            motor_pub[4] = nh_.advertise<wtr_usb2ros::motor_msgs>("/wtr/canbus/DJI/4", 1);
//            motor_pub[5] = nh_.advertise<wtr_usb2ros::motor_msgs>("/wtr/canbus/DJI/5", 1);
//            motor_pub[6] = nh_.advertise<wtr_usb2ros::motor_msgs>("/wtr/canbus/DJI/6", 1);
//            motor_pub[7] = nh_.advertise<wtr_usb2ros::motor_msgs>("/wtr/canbus/DJI/7", 1);

        }
//        /**
//        * @brief  电机状态发布函数
//        * @param
//        * @retval
//        */
//        void motorPub(Motor* motor)
//        {
//            ros::Time time_now = ros::Time::now();
//            wtr_usb2ros::motor_msgs pub;
//            pub.header.stamp = time_now;
//            for(int i = 0; i < 8 ; i++ ){
//                if(motor[i].mode != UNDEFINED){
//                    if(motor[i].type != M2006)
//                        pub.temperature = motor[i].motorInfo.temperature;
//
//                    pub.angle =motor[i].motorInfo.output_angle;
//                    pub.speed = motor[i].motorInfo.output_vol;
//                    pub.current = motor[i].motorInfo.current;
//                    motor_pub[i].publish(pub);
//
//                }
//            }
//        }
//
//        /**
//        * @brief  vesc can 接受发布函数
//        * @param
//        * @retval
//        */
//        void vescPub(uint32_t can_id, uint8_t* msg)
//        {
//            std_msgs::UInt8MultiArray pub;
//            const uint32_t dataSize = 12 ;
//            pub.layout.dim.push_back(std_msgs::MultiArrayDimension());
//            pub.layout.dim[0].size = dataSize;
//            pub.layout.dim[0].label = "VESC_BACK";
//            pub.data.resize(dataSize);
//
//            pub.data[0]=((uint8_t)(can_id>>24));
//            pub.data[1]=((uint8_t)(can_id>>16));
//            pub.data[2]=((uint8_t)(can_id>>8));
//            pub.data[3]=((uint8_t)(can_id));
//            for(int i = 0 ; i < 8 ; i ++){
//                pub.data[i+4]=msg[i];
//
//            }
//            vesc_raw_pub.publish(pub);
//        }
//        /**
//        * @brief  电机状态发布函数
//        * @param
//        * @retval
//        */
//
//        void pubAngles(float *msg){
//            std_msgs::Float32MultiArray MSG;
//            if(msg!=NULL){
//                for(int i=0;i<8;i++)
//                {
//
//                    MSG.data.push_back(msg[i]);
//                }
//                angle_pub.publish(MSG);}
//
//        }

    };

}

#endif //WTR_CAN_MOTOR_CAN_COMMUTE_H
