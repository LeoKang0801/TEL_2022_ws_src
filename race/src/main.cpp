#include "chassis_control.h"

// 建構子
int main(int argc, char **argv)
{
    ros::init(argc, argv, "strategy");
 
    while (ros::ok())
    {
        // MoveTo(x,y,max_vel,acceleration);
        MoveTo(0,10,0.5,0.02);
        // ROS_INFO("hello");
        // ros::Duration(1).sleep();
    }
}