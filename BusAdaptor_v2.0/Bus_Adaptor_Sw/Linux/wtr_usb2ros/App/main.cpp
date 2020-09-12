
//
//
// Created by kevin on 2020/5/31.
//

#include "wtr_can_motor/common_include.h"
#include "motionplanning/MotionPlan.h"
using namespace std;
void pkg_init(std_msgs::UInt8MultiArray &pub){
    const uint32_t dataSize = 12 ;
    pub.layout.dim.push_back(std_msgs::MultiArrayDimension());
    pub.layout.dim[0].size = dataSize;
    pub.layout.dim[0].label = "DJI";
    pub.data.resize(dataSize);


    int32_t can_id = 0x0300;
    pub.data[0]=((uint8_t)(can_id>>24));
    pub.data[1]=((uint8_t)(can_id>>16));
    pub.data[2]=((uint8_t)(can_id>>8));
    pub.data[3]=((uint8_t)(can_id));

    pub.data[4]=(M3508);
    pub.data[5]=(POSITION);
    pub.data[6]=(M3508);
    pub.data[7]=(UNDEFINED);
    pub.data[8]=(M3508);
    pub.data[9]=(UNDEFINED);
    pub.data[10]=(M3508);
    pub.data[11]=(UNDEFINED);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller");
    ros::NodeHandle nh;
    ros::Publisher pub_ctrl;
    pub_ctrl = nh.advertise<std_msgs::UInt8MultiArray>("/wtr/canbus",1,true);


    std_msgs::UInt8MultiArray pub;
    pkg_init(pub);
    pub_ctrl.publish(pub);
    sleep(1);

    uint32_t can_id=0x0301;
    pub.data[0]=((uint8_t)(can_id>>24));
    pub.data[1]=((uint8_t)(can_id>>16));
    pub.data[2]=((uint8_t)(can_id>>8));
    pub.data[3]=((uint8_t)(can_id));
    memset(&pub.data[4],0,8);


    double poseset[] =
            {
                    0,      0,  0,  0,  0,  0,
                    10,     0,  0,  0,  0,  0,
                    20,     0,  0,  0,  0,  0,
                    50,     0,  0,  0,  0,  0,
                    20,     0,  0,  0,  0,  0,
                    10,     0,  0,  0,  0,  0,
                    0,      0,  0,  0,  0,  0,
            };

    vector<Pose> Points;
    vector<int>  output;
    Pose::PoseSets(Points, poseset, sizeof(poseset) / sizeof(double));

    MotionPlan trajectory;
    trajectory.SetProfile(0.01, 0.1, 0.1, 5, 30, 30);
    trajectory.SetTotalTime(1);
    trajectory.SetPlanPointSet(Points);
    trajectory.GetPlan(B_Spline_c2, output);


    int count=0;
    cout<<"begin 2 run"<<endl;
    while(!ros::isShuttingDown()) {
        pub.data[4] = output[count] >> 8;
        pub.data[5] = output[count];
        pub_ctrl.publish(pub);
        count++;
        if(count==1000)
            count=0;
        usleep(1000);
    }

    return 0;
}
