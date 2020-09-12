//
// Created by kevin on 2020/6/22.
//

#include "wtr_can_motor/vesc_Tmotor.h"
#include "wtr_can_motor/can_commute.h"

// 创建互斥锁
pthread_mutex_t mutexs;

using namespace std;



int main(int argc, char **argv){
    ros::init(argc, argv, "can_motor");

    wtr::CAN canHandle;
    wtr::Node nh;

    int cntt = 0;
    while(!ros::isShuttingDown())
    {
//        canHandle.canReadMsg();
        canHandle.canTransmit(nh.ID_x,nh.Data,nh.type);
        if(cntt++>100){
//            nh.pubAngles(canHandle.getMotorAngles());
//            nh.motorPub(canHandle.getMotorsHandle());
//            nh.vescPub(canHandle.vesc_can_id, canHandle.vesc_can_msg);
            cntt = 0;
        }
        cout<<"send once :"<<cntt<<endl;
        ros::spinOnce();
        usleep(10);
    }
    return 0;

}
